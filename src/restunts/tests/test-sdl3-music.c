/* Decode generated Vorbis fixtures through SDL's dummy device and exercise
 * replacement lookup, lifecycle, and fallback without original game assets. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "../c/legacy.h"

#define TEST_CONFIGURED_DIRECTORY "configured-music"
#define TEST_EXECUTABLE_DIRECTORY "exe/"
#define TEST_PATH_CAPACITY 1024
#define TEST_READ_BYTES 8192
#define TEST_DECODE_ROUNDS 64
#define TEST_MONO_RATE 22050
#define TEST_STEREO_RATE 48000
#define TEST_FIXTURE_DURATION_DIVISOR 10
#define TEST_MIN_PCM_ENERGY 1000000U
#define TEST_INITIAL_VOLUME_DIVISOR 2
#define TEST_EXPECTED_ARGUMENTS 2

static legacy_s32 fail_queue_write;

static legacy_s32 test_put_audio_stream_data(SDL_AudioStream *stream, const void *buffer,
											 legacy_int length)
{
	if (fail_queue_write) {
		return false;
	}
	return SDL_PutAudioStreamData(stream, buffer, length);
}

#ifdef RESTUNTS_MUSIC_DIRECTORY
#undef RESTUNTS_MUSIC_DIRECTORY
#endif
#define RESTUNTS_MUSIC_DIRECTORY TEST_CONFIGURED_DIRECTORY
#define SDL_PutAudioStreamData test_put_audio_stream_data
#include "../platform/sdl3/music.c"
#undef SDL_PutAudioStreamData

legacy_s32 sdl3_batch_mode;
legacy_s8 audio_music_enabled = AUDIO_STATE_DISABLED;
legacy_u8 audio_suspended;
legacy_u8 audio_music_rate = AUDIO_ENGINE_MAX_VOLUME / TEST_INITIAL_VOLUME_DIVISOR;

static const legacy_char *test_base_path = TEST_EXECUTABLE_DIRECTORY;
static legacy_u8 resources[MUSIC_TRACK_COUNT];
static const void *fallback_resource;

const legacy_char *asset_path_base(void)
{
	return test_base_path;
}

void load_audio_finalize(void *resource)
{
	assert(fallback_resource == NULL);
	assert(music_stream == NULL && music_decoder == NULL);
	fallback_resource = resource;
	/* Finalizing the original resource must not restart a failed replacement. */
	assert(!sdl3_music_start(resource));
}

static const struct {
	const legacy_char *path;
	const legacy_char *fixture;
	legacy_s32 channels;
	legacy_s32 rate;
} tracks[] = {
	{"assets/music/titl.ogg", "tone-mono.ogg", MUSIC_CHANNELS_MONO, TEST_MONO_RATE},
	{"exe/assets/music/slct.ogg", "tone-stereo.ogg", MUSIC_CHANNELS_STEREO, TEST_STEREO_RATE},
	{"music/over.ogg", "tone-mono.ogg", MUSIC_CHANNELS_MONO, TEST_MONO_RATE},
	{"exe/music/vict.ogg", "tone-stereo.ogg", MUSIC_CHANNELS_STEREO, TEST_STEREO_RATE}};

static void copy_fixture(const legacy_char *directory, const legacy_char *name,
						 const legacy_char *destination)
{
	legacy_char path[TEST_PATH_CAPACITY];
	legacy_s32 length = snprintf(path, sizeof(path), "%s/%s", directory, name);
	assert(length > 0 && length < (legacy_s32)sizeof(path));
	assert(SDL_CopyFile(path, destination));
}

static void check_stopped(void)
{
	assert(music_stream == NULL && music_decoder == NULL);
	assert(music_track == MUSIC_NO_TRACK && !music_audio_initialized);
}

static void check_pcm_and_looping(legacy_s32 channels, legacy_s32 sample_rate)
{
	SDL_AudioSpec source;
	SDL_AudioSpec output;
	assert(SDL_GetAudioStreamFormat(music_stream, &source, &output));
	assert(source.format == SDL_AUDIO_S16);
	assert(source.channels == channels && source.freq == sample_rate);
	assert(SDL_AudioStreamDevicePaused(music_stream));
	assert(stb_vorbis_stream_length_in_samples(music_decoder) ==
		   (legacy_u32)(sample_rate / TEST_FIXTURE_DURATION_DIVISOR));
	legacy_u64 energy = 0;
	legacy_u32 frames = 0;
	legacy_s32 distinct_channels = 0;
	for (legacy_s32 round = 0; round < TEST_DECODE_ROUNDS; ++round) {
		assert(music_fill());
		assert(SDL_GetAudioStreamQueued(music_stream) <=
			   music_queue_bytes + MUSIC_DECODE_FRAMES * channels * (legacy_s32)sizeof(legacy_s16));
		legacy_u8 bytes[TEST_READ_BYTES];
		legacy_s32 read = SDL_GetAudioStreamData(music_stream, bytes, sizeof(bytes));
		assert(read > 0);
		legacy_u8 *converted = NULL;
		legacy_s32 converted_bytes = 0;
		assert(
			SDL_ConvertAudioSamples(&output, bytes, read, &source, &converted, &converted_bytes));
		legacy_s16 *samples = (legacy_s16 *)converted;
		legacy_s32 count = converted_bytes / (legacy_s32)sizeof(*samples);
		for (legacy_s32 index = 0; index < count; ++index) {
			legacy_s32 value = samples[index];
			energy += (legacy_u32)(value * value);
			if (channels == MUSIC_CHANNELS_STEREO && index % channels != 0 &&
				samples[index] != samples[index - 1]) {
				distinct_channels = 1;
			}
		}
		frames += (legacy_u32)(count / channels);
		SDL_free(converted);
	}
	/* The fixtures last a tenth of a second: draining over a second proves
	 * that EOF seeks continue producing PCM rather than stopping or spinning. */
	assert(frames > (legacy_u32)sample_rate);
	assert(energy > TEST_MIN_PCM_ENERGY);
	assert(channels == MUSIC_CHANNELS_MONO || distinct_channels);
}

static void check_volume_and_pause(void)
{
	assert(SDL_GetAudioStreamGain(music_stream) ==
		   (legacy_f32)audio_music_rate / AUDIO_ENGINE_MAX_VOLUME);
	sdl3_music_set_volume(-1);
	assert(SDL_GetAudioStreamGain(music_stream) == 0.0f);
	sdl3_music_set_volume(AUDIO_ENGINE_MAX_VOLUME + 1);
	assert(SDL_GetAudioStreamGain(music_stream) == 1.0f);
	sdl3_music_set_volume(audio_music_rate);
	audio_music_enabled = AUDIO_STATE_ENABLED;
	sdl3_music_sync();
	assert(!SDL_AudioStreamDevicePaused(music_stream));
	audio_suspended = AUDIO_STATE_ENABLED;
	sdl3_music_sync();
	assert(SDL_AudioStreamDevicePaused(music_stream));
	audio_music_enabled = AUDIO_STATE_DISABLED;
	audio_suspended = AUDIO_STATE_DISABLED;
	sdl3_music_sync();
	assert(SDL_AudioStreamDevicePaused(music_stream));
	audio_music_enabled = AUDIO_STATE_ENABLED;
	sdl3_music_sync();
	assert(!SDL_AudioStreamDevicePaused(music_stream));
	audio_music_enabled = AUDIO_STATE_DISABLED;
	sdl3_music_sync();
	assert(SDL_AudioStreamDevicePaused(music_stream));
}

static void check_all_tracks(const legacy_char *fixtures)
{
	for (legacy_u32 index = 0; index < MUSIC_TRACK_COUNT; ++index) {
		sdl3_music_register(&resources[index], (const legacy_s8 *)music_names[index]);
		assert(!sdl3_music_start(&resources[index]));
		check_stopped();
		copy_fixture(fixtures, tracks[index].fixture, tracks[index].path);
		assert(sdl3_music_start(&resources[index]));
		assert(music_track == (legacy_s32)index);
		check_pcm_and_looping(tracks[index].channels, tracks[index].rate);
		check_volume_and_pause();
		sdl3_music_stop();
		check_stopped();
		assert(sdl3_music_start(&resources[index]));
		check_pcm_and_looping(tracks[index].channels, tracks[index].rate);
		sdl3_music_stop();
		assert(SDL_RemovePath(tracks[index].path));
	}
}

static void check_search_and_fallback(const legacy_char *fixtures)
{
	const legacy_char *configured = TEST_CONFIGURED_DIRECTORY "/titl.ogg";
	copy_fixture(fixtures, "tone-stereo.ogg", configured);
	copy_fixture(fixtures, "tone-mono.ogg", tracks[0].path);
	assert(sdl3_music_start(&resources[0]));
	check_pcm_and_looping(MUSIC_CHANNELS_MONO, TEST_MONO_RATE);
	sdl3_music_stop();
	assert(SDL_RemovePath(tracks[0].path));
	test_base_path = NULL;
	assert(sdl3_music_start(&resources[0]));
	check_pcm_and_looping(MUSIC_CHANNELS_STEREO, TEST_STEREO_RATE);
	sdl3_music_stop();
	test_base_path = TEST_EXECUTABLE_DIRECTORY;
	assert(SDL_RemovePath(configured));

	FILE *invalid = fopen(tracks[0].path, "wb");
	assert(invalid != NULL);
	assert(fputs("not Ogg Vorbis", invalid) >= 0);
	assert(fclose(invalid) == 0);
	assert(!sdl3_music_start(&resources[0]));
	check_stopped();
	/* A broken title replacement does not suppress the other three songs. */
	copy_fixture(fixtures, tracks[1].fixture, tracks[1].path);
	assert(sdl3_music_start(&resources[1]));
	check_pcm_and_looping(tracks[1].channels, tracks[1].rate);
	sdl3_music_stop();
	assert(SDL_RemovePath(tracks[1].path));
	assert(SDL_RemovePath(tracks[0].path));
	assert(SDL_CreateDirectory(tracks[0].path));
	assert(!sdl3_music_start(&resources[0]));
	check_stopped();
	assert(SDL_RemovePath(tracks[0].path));
}

static void check_device_batch_and_recovery(const legacy_char *fixtures)
{
	copy_fixture(fixtures, tracks[0].fixture, tracks[0].path);
	SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "restunts-missing-audio-driver",
							SDL_HINT_OVERRIDE);
	assert(!sdl3_music_start(&resources[0]));
	check_stopped();
	SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "dummy", SDL_HINT_OVERRIDE);
	sdl3_batch_mode = 1;
	assert(!sdl3_music_start(&resources[0]));
	check_stopped();
	assert(SDL_WasInit(SDL_INIT_AUDIO) == 0);
	sdl3_batch_mode = 0;
	assert(sdl3_music_start(&resources[0]));
	assert(SDL_ClearAudioStream(music_stream));
	fail_queue_write = 1;
	audio_music_enabled = AUDIO_STATE_ENABLED;
	sdl3_music_update();
	check_stopped();
	assert(fallback_resource == &resources[0]);
	assert(!sdl3_music_start(&resources[0]));
	fail_queue_write = 0;
	audio_music_enabled = AUDIO_STATE_DISABLED;
	sdl3_music_register(&resources[0], (const legacy_s8 *)"TITL");
	assert(sdl3_music_start(&resources[0]));
	check_pcm_and_looping(MUSIC_CHANNELS_MONO, TEST_MONO_RATE);
	sdl3_music_stop();
	/* Resource addresses can be reused for non-music effects after unloading. */
	sdl3_music_register(&resources[0], (const legacy_s8 *)"engine");
	assert(!sdl3_music_start(&resources[0]));
	check_stopped();
	sdl3_music_register(&resources[0], (const legacy_s8 *)"titl");
	assert(sdl3_music_start(&resources[0]));
	sdl3_music_shutdown();
	check_stopped();
	assert(!sdl3_music_start(&resources[0]));
	assert(SDL_RemovePath(tracks[0].path));
}

legacy_int main(legacy_int argc, legacy_char **argv)
{
	assert(argc == TEST_EXPECTED_ARGUMENTS);
	SDL_SetMainReady();
	SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "dummy", SDL_HINT_OVERRIDE);
	static const legacy_char *const directories[] = {"assets/music", "exe/assets/music", "music",
													 "exe/music", TEST_CONFIGURED_DIRECTORY};
	for (legacy_u32 index = 0; index < sizeof(directories) / sizeof(directories[0]); ++index) {
		assert(SDL_CreateDirectory(directories[index]));
	}
	assert(!sdl3_music_start(NULL));
	check_all_tracks(argv[1]);
	check_search_and_fallback(argv[1]);
	check_device_batch_and_recovery(argv[1]);
	sdl3_music_shutdown();
	sdl3_music_update();
	sdl3_music_sync();
	sdl3_music_set_volume(AUDIO_ENGINE_MAX_VOLUME);
	SDL_Quit();
	puts("SDL3 music: Vorbis PCM, looping, paths, fallback, gain, pause, and cleanup passed");
	return 0;
}
