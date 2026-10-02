The two Ogg Vorbis fixtures are generated sine waves, not game recordings.

- `tone-mono.ogg`: 0.1 seconds, 22,050 Hz, mono, 440 Hz sine.
- `tone-stereo.ogg`: 0.1 seconds, 48,000 Hz, stereo, 440 Hz left and 660 Hz right.

Each channel has amplitude 0.25. They were encoded with libsndfile 1.2.2
using `SF_FORMAT_OGG | SF_FORMAT_VORBIS` and the default Vorbis quality.
They exercise sample-rate changes, channel layouts, and looping of tracks
shorter than the playback queue. These synthetic fixtures may be used
under the same license as the test source.
