/* Exercise browser PCM delivery with real SDL, Vorbis and OPL synthesis. */
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include <emscripten.h>
#include "../platform/sdl3/sdl3.h"

static legacy_s32 test_timer_clock_active;
static legacy_u64 test_timer_clock;

static legacy_u64 test_timer_ticks(void)
{
	return test_timer_clock_active ? test_timer_clock : SDL_GetTicks();
}

#define SDL_GetTicks test_timer_ticks
#include "../platform/sdl3/timer.c"
#undef SDL_GetTicks

static SDL_AudioStreamCallback test_supply_callback;
static legacy_s32 test_demand_active;
static legacy_u32 test_demand_ticks;
static void SDLCALL test_supply_observer(void *userdata, SDL_AudioStream *stream,
										 legacy_int additional_amount, legacy_int total_amount);

static SDL_AudioStream *test_open_adlib(SDL_AudioDeviceID device, const SDL_AudioSpec *spec,
										SDL_AudioStreamCallback callback, void *userdata)
{
	test_supply_callback = callback;
	return SDL_OpenAudioDeviceStream(device, spec, callback ? test_supply_observer : NULL,
									 userdata);
}

#define SDL_OpenAudioDeviceStream test_open_adlib
#include "../platform/sdl3/audio.c"
#undef SDL_OpenAudioDeviceStream
#include "../platform/sdl3/music.c"

enum {
	TEST_MUSIC_DEVICE_FRAMES = 4096,
	TEST_HINT_CAPACITY = 16,
	TEST_PHASE_READS = 512,
	TEST_PHASE_OFFSETS_MS = 10,
	TEST_WARMUP_MS = 1000,
	TEST_PLAYBACK_MS = 3000,
	TEST_PAUSED_MS = 500,
	TEST_POLL_DELAY_MS = 1,
	TEST_STALL_INTERVAL_MS = 150,
	TEST_SHORT_STALL_MS = 30,
	TEST_LONG_STALL_MS = 60,
	TEST_RENDER_STALL_MS = 1000,
	TEST_NOTE = 72,
	TEST_VOLUME = 127
};

enum { TEST_STALL_NONE, TEST_STALL_BLOCKING, TEST_STALL_RENDER, TEST_STALL_DEMAND };

legacy_s32 sdl3_batch_mode;
legacy_u8 audio_music_active = AUDIO_STATE_ENABLED;
legacy_u8 audio_music_rate = AUDIO_ENGINE_MAX_VOLUME;
legacy_u8 audio_suspended;

static legacy_u8 test_instrument[ADLIB_RESOURCE_SIZE];
static legacy_u8 test_song_resource;
static legacy_s32 test_actual_device_frames;
static legacy_s32 test_actual_device_rate;
static legacy_s32 test_minimum_prime_frames = LEGACY_S32_MAX;
static legacy_u32 test_consumer_phases;

static void SDLCALL test_supply_observer(void *userdata, SDL_AudioStream *stream,
										 legacy_int additional_amount, legacy_int total_amount)
{
	legacy_u32 before = realtime_counter;
	test_supply_callback(userdata, stream, additional_amount, total_amount);
	if (test_demand_active && realtime_counter != before) {
		test_demand_ticks += realtime_counter - before;
	}
}

const legacy_char *asset_path_base(void)
{
	return "";
}

void sdl3_platform_pump(void)
{
	sdl3_timer_pump();
}

void audio_sequence_timer(void)
{
	if (audio_music_active && audio_music_enabled && !audio_suspended) {
		sdl3_music_tick();
	}
}

void timer_remove_callback(void (*callback)(void))
{
	dos_timer_unregister_callback(callback);
}

legacy_u32 resource_read_u32le(const legacy_u8 *source)
{
	return LEGACY_READ_U32_LE(source);
}

void audio_write_far_pointer(legacy_u8 *destination, const void *value)
{
	(void)destination;
	(void)value;
	assert(0 && "Unexpected sequencer resource rebinding");
}

EM_JS(void, test_phase, (const legacy_char *name),
	  { globalThis.audioFixture.phase = UTF8ToString(name); });

EM_JS(void, test_finished, (), { globalThis.audioFixture.finished = true; });

EM_JS(legacy_s32, test_mobile, (), {
	Module['mobileDevice'] = globalThis.audioFixture.mobile;
	return Module['mobileDevice'] ? 1 : 0;
});

EM_JS(legacy_u32, test_device_hint, (), { return globalThis.audioFixture.deviceHintFrames; });

EM_JS(void, test_configuration,
	  (legacy_s32 reserve, legacy_s32 source_rate, legacy_s32 device_frames, legacy_s32 device_rate,
	   legacy_s32 callback, legacy_s32 queue_ms, legacy_u32 demand_ticks,
	   legacy_s32 minimum_prime_frames, legacy_u32 consumer_phases),
	  {
		  globalThis.audioFixture.configuration = {
			  reserveFrames : reserve,
			  sourceSampleRate : source_rate,
			  deviceFrames : device_frames,
			  deviceSampleRate : device_rate,
			  supplyCallback : !!callback,
			  musicQueueMilliseconds : queue_ms,
			  demandTicks : demand_ticks,
			  minimumPrimeFrames : minimum_prime_frames,
			  consumerPhases : consumer_phases
		  };
	  });

static void test_run(const legacy_char *phase, legacy_u32 duration, legacy_s32 stalls)
{
	test_phase(phase);
	legacy_u64 started = SDL_GetTicks();
	legacy_u64 next_stall = started + TEST_STALL_INTERVAL_MS;
	legacy_u32 stall_count = 0;
	while (SDL_GetTicks() - started < duration) {
		sdl3_timer_pump();
		if (stalls && SDL_GetTicks() >= next_stall) {
			/* Model a render/resource operation that occupies the browser thread.
			 * Audio keeps consuming PCM independently during this bounded pause. */
			legacy_u32 delay = TEST_RENDER_STALL_MS;
			if (stalls == TEST_STALL_BLOCKING) {
				delay = ++stall_count % 2U ? TEST_SHORT_STALL_MS : TEST_LONG_STALL_MS;
			}
			legacy_u64 end = SDL_GetTicks() + delay;
			while (SDL_GetTicks() < end) {
				if (stalls == TEST_STALL_RENDER) {
					sdl3_browser_render_yield_if_due();
				} else if (stalls == TEST_STALL_DEMAND) {
					/* Yield without pumping: the SDL consumer must service overdue ticks. */
					sdl3_platform_delay(0);
				}
			}
			next_stall = SDL_GetTicks() + TEST_STALL_INTERVAL_MS;
		}
		sdl3_platform_delay(TEST_POLL_DELAY_MS);
	}
}

static void test_device_frames(legacy_u32 sample_frames)
{
	legacy_char frames[TEST_HINT_CAPACITY];
	assert(SDL_snprintf(frames, sizeof(frames), "%u", sample_frames) > 0);
	assert(SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, frames));
}

static void test_adlib_device_configuration(void)
{
	SDL_AudioSpec spec;
	legacy_int frames;
	assert(SDL_GetAudioDeviceFormat(SDL_GetAudioStreamDevice(adlib_stream), &spec, &frames));
	assert(spec.freq > 0 && frames > 0);
	test_actual_device_frames = frames;
	test_actual_device_rate = spec.freq;
}

static void test_consumer_phase(void)
{
	SDL_AudioSpec spec;
	assert(SDL_GetAudioStreamFormat(adlib_stream, NULL, &spec));
	legacy_s32 frame_bytes = spec.channels * SDL_AUDIO_BYTESIZE(spec.format);
	legacy_s32 bytes = test_actual_device_frames * frame_bytes;
	void *samples = SDL_malloc(bytes);
	assert(samples != NULL);
	/* Sweep every millisecond of the 100 Hz producer phase. Repeated reads
	 * also cover the changing fractional relationship between device periods
	 * and synthesis ticks, including equal-rate devices without resampling. */
	for (legacy_u32 phase = 0; phase < TEST_PHASE_OFFSETS_MS; phase++) {
		test_timer_clock = SDL_GetTicks();
		test_timer_clock_active = 1;
		sdl3_timer_rebase();
		legacy_u64 started = last_tick + phase;
		assert(SDL_ClearAudioStream(adlib_stream));
		assert(adlib_prime_stream());
		legacy_s32 prime_frames = SDL_GetAudioStreamAvailable(adlib_stream) / frame_bytes;
		if (prime_frames < test_minimum_prime_frames) {
			test_minimum_prime_frames = prime_frames;
		}
		assert(prime_frames >= test_actual_device_frames);
		for (legacy_u32 index = 0; index < TEST_PHASE_READS; index++) {
			test_timer_clock = started + (legacy_u64)index * test_actual_device_frames *
											 SDL3_AUDIO_QUEUE_MILLISECONDS_PER_SECOND /
											 test_actual_device_rate;
			legacy_s32 received = SDL_GetAudioStreamData(adlib_stream, samples, bytes);
			if (received != bytes) {
				fprintf(stderr, "Consumer phase %u ms, read %u: %d of %d bytes\n", phase, index,
						received, bytes);
			}
			assert(received == bytes);
		}
		test_consumer_phases++;
		test_timer_clock_active = 0;
		sdl3_timer_rebase();
	}
	SDL_free(samples);
	assert(SDL_ClearAudioStream(adlib_stream));
	assert(adlib_prime_stream());
}

static void test_adlib(void)
{
	/* Startup time belongs to the stopped driver, before priming its new stream. */
	legacy_u32 startup_ticks = realtime_counter;
	legacy_u64 startup_end = SDL_GetTicks() + TEST_LONG_STALL_MS;
	while (SDL_GetTicks() < startup_end) {
	}
	assert(dos_audio_driver_initialize() == ADLIB_CONTEXTS);
	assert(adlib_ready && adlib_stream != NULL);
	test_adlib_device_configuration();
	assert(test_supply_callback != NULL && adlib_reserve_frames > 0);
	assert(realtime_counter > startup_ticks);
	assert(SDL_GetAudioStreamQueued(adlib_stream) ==
		   (legacy_s32)(adlib_reserve_frames * sizeof(legacy_s16)));
	test_consumer_phase();
	/* A sustained sine carrier has no rests that could mask an underrun. */
	LEGACY_WRITE_U32_LE(test_instrument, ADLIB_RESOURCE_SIZE);
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_LEVEL] = ADLIB_LEVEL_MAX;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_MULTIPLIER] = 1;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_SUSTAIN_TYPE] = 1;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_SIZE + ADLIB_OPERATOR_ATTACK] =
		ADLIB_ENVELOPE_RATE_MAX;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_SIZE + ADLIB_OPERATOR_RELEASE] =
		ADLIB_ENVELOPE_RATE_MAX;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_SIZE + ADLIB_OPERATOR_MULTIPLIER] = 1;
	test_instrument[ADLIB_OPERATOR_OFFSET + ADLIB_OPERATOR_SIZE + ADLIB_OPERATOR_SUSTAIN_TYPE] = 1;
	struct AUDIO_CHANNEL *channel = &audio_channels[AUDIO_EFFECT_CHANNEL_FIRST];
	struct AUDIO_CONTEXT *context = &dos_audio_contexts[AUDIO_DRIVER_CHANNEL_BASE];
	channel->channel = AUDIO_EFFECT_CHANNEL_FIRST;
	channel->volume = TEST_VOLUME;
	dos_audio_driver_prepare_context(AUDIO_DRIVER_CHANNEL_BASE, context, (legacy_u8 *)channel,
									 test_instrument);
	dos_audio_driver_activate_context(AUDIO_DRIVER_CHANNEL_BASE, context, (legacy_u8 *)channel,
									  TEST_NOTE, TEST_VOLUME, test_instrument);
	test_run("warmup", TEST_WARMUP_MS, TEST_STALL_NONE);
	test_run("adlib", TEST_PLAYBACK_MS, TEST_STALL_BLOCKING);
	test_run("render", TEST_PLAYBACK_MS, TEST_STALL_RENDER);
	test_demand_active = 1;
	test_run("demand", TEST_PLAYBACK_MS, TEST_STALL_DEMAND);
	test_demand_active = 0;
	assert(test_demand_ticks > 0);
	dos_audio_shutdown();
}

static void test_ogg(void)
{
	/* Require a period larger than the former fixed 100 ms replacement queue. */
	test_device_frames(TEST_MUSIC_DEVICE_FRAMES);
	audio_music_enabled = AUDIO_STATE_ENABLED;
	sdl3_music_register(&test_song_resource, (const legacy_s8 *)"slct");
	sdl3_music_set_enabled(1);
	assert(sdl3_music_start(&test_song_resource));
	test_run("warmup", TEST_WARMUP_MS, TEST_STALL_NONE);
	test_run("ogg", TEST_PLAYBACK_MS, TEST_STALL_BLOCKING);
	audio_suspended = AUDIO_STATE_ENABLED;
	sdl3_music_sync();
	test_run("warmup", TEST_WARMUP_MS, TEST_STALL_NONE);
	test_run("paused", TEST_PAUSED_MS, TEST_STALL_NONE);
	audio_suspended = AUDIO_STATE_DISABLED;
	sdl3_music_sync();
	test_run("warmup", TEST_WARMUP_MS, TEST_STALL_NONE);
	test_run("resumed", TEST_PLAYBACK_MS, TEST_STALL_BLOCKING);
	test_configuration(adlib_reserve_frames, ADLIB_SAMPLE_RATE, test_actual_device_frames,
					   test_actual_device_rate, test_supply_callback != NULL,
					   music_queue_bytes * MUSIC_MILLISECONDS_PER_SECOND /
						   ((legacy_s32)stb_vorbis_get_info(music_decoder).sample_rate *
							music_channels * sizeof(legacy_s16)),
					   test_demand_ticks, test_minimum_prime_frames, test_consumer_phases);
	sdl3_music_shutdown();
}

static void test_desktop(void)
{
	assert(dos_audio_driver_initialize() == ADLIB_CONTEXTS);
	assert(adlib_ready && adlib_stream != NULL);
	test_adlib_device_configuration();
	assert(test_supply_callback == NULL);
	assert(SDL_GetAudioStreamQueued(adlib_stream) == 0);
	dos_audio_shutdown();
	test_device_frames(TEST_MUSIC_DEVICE_FRAMES);
	audio_music_enabled = AUDIO_STATE_ENABLED;
	sdl3_music_register(&test_song_resource, (const legacy_s8 *)"slct");
	sdl3_music_set_enabled(1);
	assert(sdl3_music_start(&test_song_resource));
	stb_vorbis_info info = stb_vorbis_get_info(music_decoder);
	legacy_s32 queue_ms = music_queue_bytes * MUSIC_MILLISECONDS_PER_SECOND /
						  ((legacy_s32)info.sample_rate * music_channels * sizeof(legacy_s16));
	assert(queue_ms == MUSIC_QUEUE_MILLISECONDS);
	test_configuration(0, ADLIB_SAMPLE_RATE, test_actual_device_frames, test_actual_device_rate,
					   test_supply_callback != NULL, queue_ms, test_demand_ticks, 0,
					   test_consumer_phases);
	sdl3_music_shutdown();
}

legacy_int main(void)
{
	legacy_s32 mobile = test_mobile();
	assert(sdl3_browser_is_mobile() == mobile);
	test_device_frames(test_device_hint());
	dos_timer_setup_interrupt();
	assert(dos_timer_register_callback(audio_sequence_timer) ==
		   DOS_TIMER_CALLBACK_REGISTRATION_SUCCEEDED);
	if (mobile) {
		test_adlib();
		test_ogg();
	} else {
		test_desktop();
	}
	dos_timer_shutdown();
	SDL_Quit();
	test_finished();
	puts("WebAssembly audio regression tests passed");
	return 0;
}
