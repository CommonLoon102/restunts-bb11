# Portable libvpx configuration

Restunts downloads libvpx 1.17.0 from the WebM Project and Mozilla nestegg at
revision `767aab25013acefbdcc6d68a2a7a2c9081303e3f`. The archives and SHA-256
checksums are pinned in `cmake/webm.cmake`. Upstream sources remain unmodified.

libvpx uses BSD-3-Clause terms, with an additional patent grant. The generated
libvpx files in this directory retain the upstream license. nestegg uses ISC
terms. Packages install both licenses and the libvpx patent grant.

Only VP8 video decoding is enabled. VP9, encoders, assembly, CPU dispatch,
postprocessing, threads, examples, tools, tests, libyuv and libwebm are disabled.
The decoder uses portable C; nestegg is a single C source and public header.
This avoids extra executable or shared-library dependencies, and lets CMake
cross-compile the same sources for desktop, 32-bit DOS and WebAssembly.
The libvpx target requires C11; the Restunts application keeps C99.

## Regenerating after an upstream update

Run the following once in an empty directory beside the unpacked libvpx source,
using a POSIX development host with GCC, GNU make and Perl:

```sh
../libvpx-1.17.0/configure --target=generic-gnu --disable-vp9 \
    --disable-vp8-encoder --disable-examples --disable-tools --disable-docs \
    --disable-unit-tests --disable-webm-io --disable-libyuv \
    --disable-multithread --disable-runtime-cpu-detect --disable-postproc \
    --disable-shared --enable-small
make
```

Copy `vp8_rtcd.h`, `vpx_dsp_rtcd.h`, `vpx_scale_rtcd.h`, `vpx_version.h`, and
`vpx_config.c` into this directory, preserving CRLF working-tree endings.
Copy `vpx_config.h` to `vpx_config.h.in`. Set `HAVE_PTHREAD_H` and `HAVE_UNISTD_H`
to zero (neither is needed with threading disabled), and replace the
`CONFIG_BIG_ENDIAN` value with `@restunts_vpx_big_endian@`, supplied by CMake's
target byte order. Set `CONFIG_OS_SUPPORT` to zero: optional decoder profiling
timers otherwise require `clock_gettime`, which DJGPP does not provide. Restunts
uses its own SDL clock for presentation. Remove trailing whitespace from the
template.

Update the explicit source list in `cmake/webm.cmake` from the `.c.o` entries in
the build, excluding `vpx_config.c.o` which uses the copy in this directory.
Retain the upstream generated C interfaces verbatim; do not apply the game's
`legacy_*` type conventions or formatter to third-party code. Copy updated
`LICENSE` and `PATENTS` alongside these files and update the notices.

Regular builds require only CMake and the target C compiler; they do not run
this regeneration step or execute target binaries on the build host.
