#!/usr/bin/env python3
"""Publish verified build artifacts without rebuilding or repackaging them."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
from urllib.parse import quote


BUILD_WORKFLOW = ".github/workflows/build-packages.yml"
RELEASE_WORKFLOW = ".github/workflows/release.yml"
API_VERSION = "2026-03-10"
HASH_BLOCK_SIZE = 1024 * 1024
TAG_PATTERN = re.compile(r"[A-Za-z0-9][A-Za-z0-9._/-]*")


def command(arguments, **kwargs):
    return subprocess.run(arguments, check=True, text=True, **kwargs)


def api(repository, suffix, payload=None, paginate=False):
    arguments = ["gh", "api", f"repos/{repository}/{suffix}",
                 "--header", f"X-GitHub-Api-Version: {API_VERSION}"]
    if paginate:
        arguments.extend(["--paginate", "--slurp"])
    if payload is not None:
        arguments.extend(["--method", "POST", "--input", "-"])
    result = command(arguments, input=json.dumps(payload) if payload is not None else None,
                     stdout=subprocess.PIPE)
    return json.loads(result.stdout)


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(HASH_BLOCK_SIZE), b""):
            digest.update(block)
    return digest.hexdigest()


def check_tag(repository, tag):
    if TAG_PATTERN.fullmatch(tag) is None:
        raise ValueError("Tag contains unsupported characters")
    command(["git", "check-ref-format", f"refs/tags/{tag}"])
    refs = command(["git", "ls-remote", "--tags", "origin", f"refs/tags/{tag}"],
                   stdout=subprocess.PIPE).stdout
    if refs.strip():
        raise ValueError(f"Tag already exists: {tag}")
    releases = api(repository, "releases?per_page=100", paginate=True)
    if any(release["tag_name"] == tag for page in releases for release in page):
        raise ValueError(f"Release or draft already exists: {tag}")


def check_attestation_run(results, repository, source_ref, commit, run_id, attempt):
    """Enforce signed certificate claims, not workflow-supplied predicate values."""
    # Fulcio certificate extensions 1.3.6.1.4.1.57264.1.{18,19,20,21}.
    # For reusable workflows, buildConfig identifies the top-level caller.
    expected = {
        "runInvocationURI": f"https://github.com/{repository}/actions/runs/{run_id}/attempts/{attempt}",
        "buildConfigURI": f"https://github.com/{repository}/{RELEASE_WORKFLOW}@{source_ref}",
        "buildConfigDigest": commit,
        "buildTrigger": "workflow_dispatch",
    }
    for result in results:
        certificate = result.get("verificationResult", {}).get("signature", {}).get("certificate", {})
        if all(certificate.get(key) == value for key, value in expected.items()):
            return
    raise ValueError("No verified attestation belongs to this release workflow run and attempt")


def check_release_tag(refs, tag, commit):
    """Allow a service-created tag only when it points directly to this build."""
    expected = f"{commit}\trefs/tags/{tag}"
    if refs.strip() not in ("", expected):
        raise ValueError("The release tag does not point directly to the verified source commit")


def verify_artifacts(directory, repository, commit, source_ref, run_id, attempt):
    command([sys.executable, str(Path(__file__).with_name("release-packages.py")), "verify",
             "--directory", str(directory), "--commit", commit, "--all",
             "--run-id", run_id, "--run-attempt", attempt])
    files = sorted(directory.iterdir())
    for path in files:
        result = command([
            "gh", "attestation", "verify", str(path), "--repo", repository,
            "--signer-workflow", f"{repository}/{BUILD_WORKFLOW}",
            "--source-digest", commit, "--signer-digest", commit,
            "--source-ref", source_ref, "--deny-self-hosted-runners", "--format", "json",
        ], stdout=subprocess.PIPE)
        check_attestation_run(json.loads(result.stdout), repository, source_ref, commit, run_id, attempt)
    return {path.name: sha256(path) for path in files}


def check_assets(assets, expected, directory=None):
    names = [asset["name"] for asset in assets]
    if len(names) != len(set(names)) or set(names) != set(expected):
        raise ValueError("Release asset inventory differs from the verified build artifacts")
    for asset in assets:
        name = asset["name"]
        if asset.get("state") != "uploaded" or asset.get("digest") != f"sha256:{expected[name]}":
            raise ValueError(f"Release asset digest/state mismatch: {name}")
        if directory is not None:
            local = directory / name
            if not local.is_file() or local.is_symlink() or sha256(local) != expected[name]:
                raise ValueError(f"Downloaded release asset differs from build artifact: {name}")


def get_assets(repository, release_id):
    return [asset for page in api(repository, f"releases/{release_id}/assets?per_page=100", paginate=True)
            for asset in page]


def verify_draft(repository, release_id, tag, commit, expected):
    release = api(repository, f"releases/{release_id}")
    if not release["draft"] or release["tag_name"] != tag or release["target_commitish"] != commit:
        raise ValueError("Release draft tag, target commit, or state changed during upload")
    assets = get_assets(repository, release_id)
    check_assets(assets, expected)
    with tempfile.TemporaryDirectory(prefix="release-check-") as temporary:
        directory = Path(temporary)
        # Download by immutable asset ID, including private draft assets.
        for asset in assets:
            with (directory / asset["name"]).open("wb") as destination:
                subprocess.run(["gh", "api", f"repos/{repository}/releases/assets/{asset['id']}",
                                "--header", "Accept: application/octet-stream"],
                               check=True, stdout=destination)
        check_assets(get_assets(repository, release_id), expected, directory)


def release_notes(repository, tag, commit, source_ref, run_id, platforms, draft):
    lines = [
        f"Built from `{source_ref}` at commit `{commit}`.", "",
        f"[Build and validation run](https://github.com/{repository}/actions/runs/{run_id}).",
        f"Golden replay validation passed for: {', '.join(json.loads(platforms))}.", "",
        "All platform packages and their SHA-256 files are the original build artifacts.",
        "Original Stunts game content is not included; supply your own game files.", "",
        "Verify a downloaded package's build provenance using a current GitHub CLI:", "",
        "```sh",
        f"gh attestation verify PACKAGE --repo {repository} \\",
        f"  --signer-workflow {repository}/{BUILD_WORKFLOW} \\",
        f"  --source-digest {commit} --signer-digest {commit} --deny-self-hosted-runners",
        f"gh release verify-asset {tag} PACKAGE --repo {repository}",
        "```", "",
        "The first command checks its signed build provenance; the second checks the published immutable release.",
    ]
    if draft:
        lines.extend(["", "This draft was verified on upload. Draft assets and tags remain mutable until publication.",
                      "Manual publication does not rerun this workflow's checks."])
    return "\n".join(lines) + "\n"


def publish(args, environment):
    repository = environment["GITHUB_REPOSITORY"]
    commit = environment["GITHUB_SHA"]
    source_ref = environment["GITHUB_REF"]
    run_id = environment["GITHUB_RUN_ID"]
    attempt = environment["GITHUB_RUN_ATTEMPT"]
    check_tag(repository, args.tag)
    expected = verify_artifacts(args.directory, repository, commit, source_ref, run_id, attempt)
    body = release_notes(repository, args.tag, commit, source_ref, run_id, args.platforms, args.draft)
    # JSON stdin preserves titles/notes verbatim without shell interpolation.
    release = api(repository, "releases", {
        "tag_name": args.tag, "target_commitish": commit, "name": args.title or args.tag,
        "body": body, "draft": True, "prerelease": args.prerelease,
    })
    release_id = release["id"]
    command(["gh", "release", "upload", args.tag, "--repo", repository,
             *[str(args.directory / name) for name in expected]])
    verify_draft(repository, release_id, args.tag, commit, expected)
    if args.draft:
        print(f"Verified draft: {release['html_url']} (mutable until published)")
        return
    # Never move an existing tag, even if one appeared while preparing the draft.
    refs = command(["git", "ls-remote", "--tags", "origin", f"refs/tags/{args.tag}"],
                   stdout=subprocess.PIPE).stdout
    check_release_tag(refs, args.tag, commit)
    command(["gh", "release", "edit", args.tag, "--repo", repository, "--draft=false"])
    published = api(repository, f"releases/{release_id}")
    if published.get("immutable") is not True or published.get("draft") is not False:
        raise ValueError("Published release is not immutable; check the repository release settings")
    tag_ref = api(repository, f"git/ref/tags/{quote(args.tag, safe='')}")
    if tag_ref["object"].get("type") != "commit" or tag_ref["object"].get("sha") != commit:
        raise ValueError("Published tag does not identify the verified source commit")
    check_assets(get_assets(repository, release_id), expected)
    print(f"Verified immutable release: {published['html_url']}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("check-tag", "publish"))
    parser.add_argument("--tag", required=True)
    parser.add_argument("--directory", type=Path, default=Path("dist"))
    parser.add_argument("--title", default="")
    parser.add_argument("--platforms", default='["dos","sdl3"]')
    parser.add_argument("--draft", action="store_true")
    parser.add_argument("--prerelease", action="store_true")
    args = parser.parse_args()
    try:
        if args.command == "check-tag":
            check_tag(os.environ["GITHUB_REPOSITORY"], args.tag)
        else:
            publish(args, os.environ)
    except (ValueError, KeyError, OSError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"Release verification failed: {error}\n")


if __name__ == "__main__":
    main()
