"""Release publication must fail closed before publishing unverified assets."""

import argparse
import copy
import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch


SCRIPT = Path(__file__).resolve().parents[1] / "publish-release.py"
SPEC = importlib.util.spec_from_file_location("publish_release", SCRIPT)
RELEASE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(RELEASE)
REPOSITORY = "example/restunts"
COMMIT = "a" * 40
SOURCE_REF = "refs/heads/main"
RUN_ID = "1234"
ATTEMPT = "2"


def attestation():
    return [{"verificationResult": {"signature": {"certificate": {
        "runInvocationURI": f"https://github.com/{REPOSITORY}/actions/runs/{RUN_ID}/attempts/{ATTEMPT}",
        "buildConfigURI": f"https://github.com/{REPOSITORY}/{RELEASE.RELEASE_WORKFLOW}@{SOURCE_REF}",
        "buildConfigDigest": COMMIT,
        "buildTrigger": "workflow_dispatch",
    }}}}]


class ReleaseIntegrityTests(unittest.TestCase):
    def test_requires_exact_signed_run_attempt_and_release_workflow(self):
        good = attestation()
        RELEASE.check_attestation_run(good, REPOSITORY, SOURCE_REF, COMMIT, RUN_ID, ATTEMPT)
        for key in good[0]["verificationResult"]["signature"]["certificate"]:
            with self.subTest(claim=key):
                changed = copy.deepcopy(good)
                changed[0]["verificationResult"]["signature"]["certificate"][key] += "-other"
                with self.assertRaisesRegex(ValueError, "No verified attestation"):
                    RELEASE.check_attestation_run(changed, REPOSITORY, SOURCE_REF, COMMIT, RUN_ID, ATTEMPT)

    def test_unsigned_predicate_metadata_cannot_supply_certificate_claims(self):
        claimed = {"verificationResult": {"statement": {"predicate": attestation()[0]}}}
        with self.assertRaises(ValueError):
            RELEASE.check_attestation_run([claimed], REPOSITORY, SOURCE_REF, COMMIT, RUN_ID, ATTEMPT)

    def test_rejects_empty_attestation_results(self):
        with self.assertRaises(ValueError):
            RELEASE.check_attestation_run([], REPOSITORY, SOURCE_REF, COMMIT, RUN_ID, ATTEMPT)

    def test_tag_created_during_upload_must_match_the_verified_commit(self):
        RELEASE.check_release_tag("", "v1.0", COMMIT)
        RELEASE.check_release_tag(f"{COMMIT}\trefs/tags/v1.0\n", "v1.0", COMMIT)
        for refs in (f"{'b' * 40}\trefs/tags/v1.0\n", f"{COMMIT}\trefs/tags/other\n",
                     f"{'b' * 40}\trefs/tags/v1.0\n{COMMIT}\trefs/tags/v1.0^{{}}\n"):
            with self.subTest(refs=refs), self.assertRaises(ValueError):
                RELEASE.check_release_tag(refs, "v1.0", COMMIT)

    def test_checks_all_assets_and_rejects_substitution(self):
        expected = {"restunts-dos16.zip": "a" * 64}
        assets = [{"name": "restunts-dos16.zip", "state": "uploaded", "digest": "sha256:" + "a" * 64}]
        RELEASE.check_assets(assets, expected)
        variants = [[], assets + assets,
                    assets + [{"name": "extra.zip"}],
                    [{**assets[0], "digest": "sha256:" + "b" * 64}],
                    [{**assets[0], "state": "starter"}]]
        for variant in variants:
            with self.subTest(assets=variant), self.assertRaises(ValueError):
                RELEASE.check_assets(variant, expected)

    def test_hashes_downloaded_bytes_and_rejects_symlinks(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            package = directory / "restunts-dos16.zip"
            package.write_bytes(b"original build artifact")
            expected = {package.name: RELEASE.sha256(package)}
            assets = [{"name": package.name, "state": "uploaded", "digest": f"sha256:{expected[package.name]}"}]
            RELEASE.check_assets(assets, expected, directory)
            package.write_bytes(b"replaced binary")
            with self.assertRaisesRegex(ValueError, "differs from build artifact"):
                RELEASE.check_assets(assets, expected, directory)
            package.unlink()
            real = directory / "real"
            real.write_bytes(b"original build artifact")
            package.symlink_to(real)
            with self.assertRaises(ValueError):
                RELEASE.check_assets(assets, expected, directory)

    @patch.object(RELEASE, "api")
    @patch.object(RELEASE, "command")
    def test_refuses_existing_draft_without_overwriting(self, run, api):
        run.return_value.stdout = ""
        api.return_value = [[{"tag_name": "v1.0", "draft": True}]]
        with self.assertRaisesRegex(ValueError, "draft already exists"):
            RELEASE.check_tag(REPOSITORY, "v1.0")
        self.assertEqual(api.call_count, 1)
        self.assertTrue(api.call_args.kwargs["paginate"])

    @patch.object(RELEASE, "api")
    @patch.object(RELEASE, "command")
    def test_refuses_existing_remote_tag(self, run, api):
        run.return_value.stdout = f"{COMMIT}\trefs/tags/v1.0\n"
        with self.assertRaisesRegex(ValueError, "Tag already exists"):
            RELEASE.check_tag(REPOSITORY, "v1.0")
        api.assert_not_called()

    @patch.object(RELEASE, "command")
    def test_tag_input_cannot_inject_cli_options(self, run):
        for tag in ("--draft", "v1\nmalicious", "v1; touch pwned", "$(whoami)"):
            with self.subTest(tag=tag), self.assertRaises(ValueError):
                RELEASE.check_tag(REPOSITORY, tag)
        run.assert_not_called()

    @patch.object(RELEASE, "command")
    def test_verifier_cryptographically_constrains_source_and_signer(self, run):
        run.return_value.stdout = json.dumps(attestation())
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            (directory / "package.zip").write_bytes(b"package")
            RELEASE.verify_artifacts(directory, REPOSITORY, COMMIT, SOURCE_REF, RUN_ID, ATTEMPT)
        arguments = run.call_args_list[-1].args[0]
        for option, value in (("--source-digest", COMMIT), ("--signer-digest", COMMIT),
                              ("--source-ref", SOURCE_REF), ("--repo", REPOSITORY),
                              ("--signer-workflow", f"{REPOSITORY}/{RELEASE.BUILD_WORKFLOW}")):
            self.assertEqual(arguments[arguments.index(option) + 1], value)
        self.assertIn("--deny-self-hosted-runners", arguments)

    def test_upload_failure_never_publishes_or_clobbers(self):
        args = argparse.Namespace(tag="v1.0", directory=Path("dist"), title="", platforms='["dos"]',
                                  draft=False, prerelease=False)
        environment = {"GITHUB_REPOSITORY": REPOSITORY, "GITHUB_SHA": COMMIT, "GITHUB_REF": SOURCE_REF,
                       "GITHUB_RUN_ID": RUN_ID, "GITHUB_RUN_ATTEMPT": ATTEMPT}
        with patch.object(RELEASE, "check_tag"), \
                patch.object(RELEASE, "verify_artifacts", return_value={"package.zip": "a" * 64}), \
                patch.object(RELEASE, "api", return_value={"id": 1}), \
                patch.object(RELEASE, "verify_draft") as verify, \
                patch.object(RELEASE, "command", side_effect=subprocess.CalledProcessError(1, "upload")) as run:
            with self.assertRaises(subprocess.CalledProcessError):
                RELEASE.publish(args, environment)
            verify.assert_not_called()
            self.assertEqual(run.call_count, 1)
            self.assertEqual(run.call_args.args[0][1:3], ["release", "upload"])
            self.assertNotIn("--clobber", run.call_args.args[0])

    def test_provenance_failure_cannot_create_a_draft(self):
        args = argparse.Namespace(tag="v1.0", directory=Path("dist"))
        environment = {"GITHUB_REPOSITORY": REPOSITORY, "GITHUB_SHA": COMMIT, "GITHUB_REF": SOURCE_REF,
                       "GITHUB_RUN_ID": RUN_ID, "GITHUB_RUN_ATTEMPT": ATTEMPT}
        with patch.object(RELEASE, "check_tag"), \
                patch.object(RELEASE, "verify_artifacts", side_effect=ValueError("bad signature")), \
                patch.object(RELEASE, "api") as api:
            with self.assertRaises(ValueError):
                RELEASE.publish(args, environment)
            api.assert_not_called()

    def test_failed_uploaded_asset_verification_leaves_draft_unpublished(self):
        args = argparse.Namespace(tag="v1.0", directory=Path("dist"), title="", platforms='["dos"]',
                                  draft=False, prerelease=False)
        environment = {"GITHUB_REPOSITORY": REPOSITORY, "GITHUB_SHA": COMMIT, "GITHUB_REF": SOURCE_REF,
                       "GITHUB_RUN_ID": RUN_ID, "GITHUB_RUN_ATTEMPT": ATTEMPT}
        with patch.object(RELEASE, "check_tag"), \
                patch.object(RELEASE, "verify_artifacts", return_value={"package.zip": "a" * 64}), \
                patch.object(RELEASE, "api", return_value={"id": 1}), \
                patch.object(RELEASE, "verify_draft", side_effect=ValueError("changed bytes")), \
                patch.object(RELEASE, "command") as run:
            with self.assertRaisesRegex(ValueError, "changed bytes"):
                RELEASE.publish(args, environment)
            self.assertEqual(run.call_count, 1)
            self.assertEqual(run.call_args.args[0][1:3], ["release", "upload"])


if __name__ == "__main__":
    unittest.main()
