# Release packages and verification

The **PR validation** and manual **Release** workflows call the same
`build-packages.yml` workflow. Every successful run produces these 13 archives
and a SHA-256 sidecar for each archive:

| Archive | Target baseline |
| --- | --- |
| `restunts-dos16.zip` | 16-bit DOS, Open Watcom |
| `restunts-dos32.zip` | 32-bit DOS, DJGPP/SDL3, bundled CWSDPMI |
| `restunts-linux-arm32.tar.gz` | ARMv7 hard-float, Debian/Raspbian 12+; Pi 2 and newer |
| `restunts-linux-arm64.tar.gz` | AArch64, Debian/Raspbian 12+ |
| `restunts-linux-x86.tar.gz` | 32-bit x86 with SSE2, Debian 12+ |
| `restunts-linux-x86-no-sse2.tar.gz` | 32-bit x86 without SSE2, Debian 12+ |
| `restunts-linux-x64.tar.gz` | x86_64, Debian 12+ |
| `restunts-windows-arm64.zip` | ARM64, Windows 10+ |
| `restunts-windows-x86.zip` | 32-bit x86 with SSE2, Windows XP SP2+ |
| `restunts-windows-x86-no-sse2.zip` | 32-bit x86 without SSE2, Windows XP SP2+ |
| `restunts-windows-x64.zip` | x86_64, Windows 7+ |
| `restunts-macos-universal.tar.gz` | Intel and Apple Silicon, macOS 11.0+ |
| `restunts-browser.zip` | Offline HTML/WebAssembly |

These are build baselines, not a claim that CI exercises every OS version and
CPU. The game and replay tests run only on native Linux SDL3 and/or 16-bit DOS,
as selected by the `platforms` input. That selection never reduces the package
matrix. ARM32 targets ARMv7; it does not cover the original Pi 1 or Pi Zero.

## Package contents

Supply your own original Broderbund Stunts 1.1 game files. None of those files
are distributed in these archives. Enhanced opponent portraits and skyboxes
are included for the SDL3 game; the browser embeds them in `restunts.html`.
Packages retain their directory layout so the game can locate those assets.

Desktop packages contain the game, physics dumper, renderer dumper, statically
linked SDL, its license, and the replaceable Nuked OPL2 shared library. The
exact Nuked source, license, standalone build files and build context are under
`share/restunts/nuked-opl2-lite/`. Keep the complete package together.

The browser package contains the single playable `restunts.html`, notices,
licenses, and `share/restunts/wasm-relink/`. The relinking kit contains Nuked's
source and the object/archive files needed to rebuild the HTML with a modified
library. Only the HTML is needed to play; distribute the complete archive.

DOS uses hardware AdLib/OPL and includes neither Nuked binaries nor its sources.
DOS32 includes `bin/CWSDPMI.EXE` and the accompanying CWSDPMI redistribution
material. Copy the DOS32 executables, CWSDPMI and enhanced artwork directories
into the writable directory containing your original game files. DOS16 keeps its executables at the archive root; copy them into the original
game directory before running.

Each package also contains `PACKAGE-MANIFEST.json`, recording the source commit,
build run and hashes of its packaged files. The corresponding `<archive>.sha256`
file hashes the finished archive. Missing required artwork, runtime libraries,
source files or licenses fail packaging; unexpected files also fail validation.

## Creating a release

Keep **Settings > General > Releases > Enable release immutability** enabled.
It applies to newly published releases. GitHub documents the setting in
[Preventing changes to your releases](https://docs.github.com/en/code-security/how-tos/secure-your-supply-chain/establish-provenance-and-integrity/prevent-release-changes).

Run the **Release** workflow on the source branch/commit you want to release,
providing a new tag and optional title. Choose `dos`, `sdl3`, or both for replay
validation. Existing formatting, host and regression checks remain required.

The workflow builds and archives all targets, attests the finished archives,
and uploads them as `packages-<target>` Actions artifacts. The publishing job
downloads those artifacts from the same run. It verifies the complete target
set, archive checksums and signed provenance for the expected source commit,
builder workflow and run attempt. It never rebuilds or repacks the binaries.

Publication creates a draft, attaches the verified archives and checksums,
and downloads the attachments again to check their bytes before publishing.
The published release is checked for immutability and the expected source tag.
A missing package, failed test or integrity mismatch prevents successful
publication. Existing release assets are never overwritten.

The optional `draft` input leaves the verified release unpublished. Drafts
remain mutable until publication; publishing a draft later through GitHub's UI
does not repeat the workflow's prepublication checks. Use the normal publishing
path for verification immediately before publication. If a release attempt
fails, start a fresh workflow run: immutable artifact names cannot be overwritten,
and publication requires all attestations to belong to one run attempt.
A failed run may leave a draft for inspection; resolve that draft/tag before
attempting another release with the same name, or choose a new unused tag.

## Verifying a download

Use a current [GitHub CLI](https://cli.github.com/) on a supported machine.
Verification can happen on a modern computer before copying a package to DOS
or an older Windows installation. Replace the example tag and archive below
with your release and platform:

```sh
repo=CommonLoon102/restunts-bb11
tag=v1.2.3
archive=restunts-linux-x64.tar.gz
gh release download "$tag" --repo "$repo" \
    --pattern "$archive" --pattern "$archive.sha256"
sha256sum --check "$archive.sha256"
gh release verify "$tag" --repo "$repo"
gh release verify-asset "$tag" "$archive" --repo "$repo"
gh attestation verify "$archive" --repo "$repo" \
    --signer-workflow "$repo/.github/workflows/build-packages.yml" \
    --deny-self-hosted-runners
```

The checksum checks the download. Release verification checks GitHub's signed
release attestation. Build verification checks the signed association with the
repository and packaging workflow; its output identifies the source commit and
build invocation. For an exact source requirement, add
`--source-digest FULL_COMMIT_SHA --signer-digest FULL_COMMIT_SHA` to the last
command, using the commit associated with the release tag.

See GitHub's documentation for [immutable releases](https://docs.github.com/en/code-security/concepts/supply-chain-security/immutable-releases)
and [attestation verification](https://cli.github.com/manual/gh_attestation_verify).
These mechanisms establish origin and detect substitution; they do not prove
that reviewed source, dependencies or the build environment are malware-free.
