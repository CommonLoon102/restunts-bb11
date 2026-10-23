#ifndef RESTUNTS_SDL3_AUDIO_QUEUE_H
#define RESTUNTS_SDL3_AUDIO_QUEUE_H

#include "sdl3.h"

/* The mobile consumer catches up overdue ticks before reading this period. */
#define SDL3_AUDIO_QUEUE_PERIODS 1U
/* Allow for deferred browser tasks without resetting otherwise healthy PCM. */
#define SDL3_AUDIO_BROWSER_STALL_MILLISECONDS 500U
#define SDL3_AUDIO_QUEUE_FALLBACK_MILLISECONDS 100U
#define SDL3_AUDIO_QUEUE_MILLISECONDS_PER_SECOND 1000U

/* SDL consumes complete device periods, which may be larger than a game tick.
 * Express the reserve in the stream's input rate, rounding upward so a rate
 * conversion cannot leave the queue a fraction of a device period short. */
static inline legacy_s32 sdl3_audio_queue_frames(SDL_AudioStream *stream,
												 legacy_s32 source_sample_rate)
{
	if (source_sample_rate <= 0) {
		return 0;
	}
	SDL_AudioSpec device_spec;
	legacy_int device_frames;
	legacy_u64 frames;
	if (stream != NULL &&
		SDL_GetAudioDeviceFormat(SDL_GetAudioStreamDevice(stream), &device_spec, &device_frames) &&
		device_spec.freq > 0 && device_frames > 0) {
		frames = ((legacy_u64)device_frames * SDL3_AUDIO_QUEUE_PERIODS * source_sample_rate +
				  device_spec.freq - 1U) /
				 device_spec.freq;
	} else {
		frames = ((legacy_u64)source_sample_rate * SDL3_AUDIO_QUEUE_FALLBACK_MILLISECONDS +
				  SDL3_AUDIO_QUEUE_MILLISECONDS_PER_SECOND - 1U) /
				 SDL3_AUDIO_QUEUE_MILLISECONDS_PER_SECOND;
	}
	return frames > LEGACY_S32_MAX ? (legacy_s32)LEGACY_S32_MAX : (legacy_s32)frames;
}

#endif
