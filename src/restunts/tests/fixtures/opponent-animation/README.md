# Opponent animation test clips

These synthetic clips (16 x 16 unless otherwise noted) were created for the Restunts regression suite.
Their image and audio content is dedicated to the public domain under CC0-1.0.
No original game artwork or third-party recordings are included.

- `colors.webm`: VP8, red/green/blue frames at four frames per second (750 ms).
- `reverse.webm`: VP8, blue/green/red frames at the same rate.
- `odd.webm`: VP8 colors at 17 x 19 pixels, testing odd chroma plane strides.
- `vp9.webm`: VP9 version, to verify unsupported codecs fall back safely.
- `with-audio.webm`: reverse VP8 clip with a synthetic 440 Hz Opus track, to
  verify audio tracks can be skipped while video is decoded.

The VP8 clips use one keyframe followed by two interframes. Generate the raw
RGB24 input by repeating each RGB triplet 256 times, then concatenating the
three frames. The following FFmpeg 7.0.2 options produce the video fixtures:

```sh
ffmpeg -f rawvideo -pixel_format rgb24 -video_size 16x16 -framerate 4 \
    -i colors.rgb -an -c:v libvpx -pix_fmt yuv420p -g 999 -keyint_min 999 \
    -auto-alt-ref 0 -threads 1 colors.webm
```

For VP9, replace `-c:v libvpx` with `-c:v libvpx-vp9` and omit the GOP and
alternate-reference options. For the audio fixture, use the reverse input,
replace `-an` with a second input
`-f lavfi -i sine=frequency=440:sample_rate=48000:duration=0.75`, and add
`-c:a libopus`. Tests create corrupt and truncated inputs from these fixtures
at runtime.
