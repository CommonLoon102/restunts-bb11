Vendored emu8950
================

Source: https://github.com/digital-sound-antiques/emu8950
Commit: c27078c654de8f9cf37e9f02f040cc251aba33d9
License: MIT (see LICENSE).

Local changes beyond CRLF line endings:

- Use OPL_ADPCM_delete when changing away from Y8950, releasing both sample buffers.
- Zero-initialize the ADPCM object so allocation-failure cleanup only frees valid pointers.
- Multiply signed exponential-table output by two instead of left-shifting a negative value.

AddressSanitizer and UndefinedBehaviorSanitizer exposed the leak and signed-shift issue
while the native AdLib regression synthesized a shipped instrument.
The SDL3 desktop backend uses YM3812 (OPL2) emulation. The DOS target writes
the hardware OPL2 chip and does not compile this dependency.
