# Bundled LZ4 block codec

Source: https://github.com/lz4/lz4
Version: v1.10.0
License: BSD-2-Clause (see LICENSE and the source headers).
Copyright (c) 2011-2023, Yann Collet.

The files `lz4.c`, `lz4.h`, and `LICENSE` come from upstream's `lib/`
directory at tag `v1.10.0`:

- https://raw.githubusercontent.com/lz4/lz4/v1.10.0/lib/lz4.c
- https://raw.githubusercontent.com/lz4/lz4/v1.10.0/lib/lz4.h
- https://raw.githubusercontent.com/lz4/lz4/v1.10.0/lib/LICENSE

Local changes, 2026-09-24: normalize C/H line endings to CRLF according to
`.gitattributes`, and add this README. The codec's contents and formatting are
otherwise unchanged from upstream; the license is copied verbatim.

The parent CMake build compiles the codec as the static `restunts_lz4` library
for lossless lighting-cache compression. The game and shadow-cache regression
link it directly; no system LZ4 installation or runtime DLL is required. Both
Runtime and Tests packages include the BSD license as
`share/licenses/restunts/LZ4-LICENSE`.
