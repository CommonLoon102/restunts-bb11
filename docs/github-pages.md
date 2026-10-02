# Chocolate Stunts on GitHub Pages

The landing page lives in `site/`. It presents the game's features, placeholders
for all 18 release packages, and the browser game in a same-origin iframe. Its
relative URLs work under a repository path such as `/restunts-bb11/`.

The linked `site/quick-guide.html` shares the landing page styling and gives
returning players a short guide to added hotkeys and launch parameters. Keep
it in the builder's `WEBSITE_FILES` list so it is included in the published site.

## Publish

1. Add the website and workflow changes to the repository through the usual
   review process.
2. Open **Settings > Pages** in GitHub and choose **GitHub Actions** as the build
   and deployment source.
3. Open **Actions > GitHub Pages > Run workflow**, select the reviewed branch,
   and run it. Allow that branch in the `github-pages` environment if needed.
4. The deployment job reports the published URL.

Publication is manual. Pushes, pull requests, and game releases do not publish
the site automatically. GitHub must have the workflow on the default branch
before its manual run button is available.

The workflow uses the same pinned Emscripten toolchain and browser packaging
script as release builds. It validates the archive's target, source commit,
checksum, file inventory, licenses, and relink materials before assembling the
site. It does not run the full game replay validation suite; publish a reviewed
commit whose normal project checks have passed.

No original Stunts game resources are copied to the website. Visitors choose
their own Broderbund Stunts 1.1 folder locally. The browser's own folder picker
and save controls work inside the same-origin frame; the page also provides a
link to open the game separately and allows fullscreen.

## Build and preview locally

Use the complete `restunts-browser.zip` and its matching
`restunts-browser.zip.sha256` from the package workflow. The assembly command
checks that its source commit matches the current repository HEAD:

```sh
python3 tools/scripts/build-pages.py \
    --browser-package /path/to/restunts-browser.zip \
    --output out/github-pages
python3 -m http.server 8000 --directory out/github-pages
```

Open `http://localhost:8000/`. For another known package revision, pass its full
source SHA with `--commit`; this is an explicit choice to preview website text
and a game from different revisions. Keep the checksum sidecar beside the ZIP.

The output directory must be new or empty. For another preview, select a fresh
`--output` directory or remove only the previous generated output yourself.
The builder copies only the named landing files and permitted files beneath
`site/assets/`; it rejects asset symlinks and unexpected formats.

To build a fresh package on a clean checkout in the Debian 12 build environment,
install the base tools listed in `.github/workflows/github-pages.yml`, then run:

```sh
bash tools/scripts/install-package-dependencies.sh browser
GITHUB_SHA=$(git rev-parse HEAD) bash tools/scripts/build-package.sh browser
python3 tools/scripts/build-pages.py \
    --browser-package dist/packages/restunts-browser.zip \
    --output out/github-pages
```

Installing the base Debian build tools requires root access. The browser
toolchain installer downloads its verified tools separately. See
[the browser build guide](wasm.md) for a direct local Emscripten build;
the Pages builder requires the complete release ZIP, not a standalone HTML file.

The hosted game is `game/restunts.html`. Its matching, unmodified browser ZIP is
available as `game/browser-runtime.zip`, with a checksum sidecar. That archive
includes the dependency notices, licenses, Nuked source, and complete relink kit.
Keep the archive and notice links with the hosted game. The 18 download cards
remain placeholders independently of this required runtime-materials link.

## Add release download links

Edit the 18 package cards in `site/index.html` when their release URLs are ready.
Add the matching release URL as each card anchor's `href`, remove
`aria-disabled="true"`, and replace its `Coming soon` status text with `Download`.
Update the section's packages-coming-soon notice once downloads are available.
Match each card's `data-target` to the
archive names in [the release package list](releases.md); the SSE2 and no-SSE2
variants are separate archives. Keep the CPU labels and bitness badges intact.

The website uses plain HTML, CSS, and JavaScript; no npm installation or separate
frontend compilation is needed. HTML and Markdown use CRLF working-tree line
endings, while the workflow and Python builder use LF.

## Check the builder

```sh
python3 -m unittest discover -s tools/scripts/tests -p test_build_pages.py
```

These checks cover complete browser distribution, source revision and checksum
failures, forbidden game data and archive paths, asset symlinks, and protecting
an existing output directory. The workflow also runs the shared release-package
validation tests.
