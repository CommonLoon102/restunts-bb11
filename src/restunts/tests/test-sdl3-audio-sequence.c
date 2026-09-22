/* Exercise optional voice-bank entries through the real native resource mapper,
 * sound-effect sequencer, context allocator and SDL dummy audio device. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include "../c/audio_control.h"
#include "../c/audio_internal.h"
#include "../c/platform.h"
#include "../platform/sdl3/sdl3.h"

extern legacy_s16 audio_play_effect(void *resource, legacy_s16 channel, legacy_u8 priority);

#define SEQUENCE_COMMAND(command) (AUDIO_SEQUENCE_COMMAND_BASE + AUDIO_SEQUENCE_COMMAND_##command)
#define TEST_CHANNEL AUDIO_EFFECT_CHANNEL_FIRST
#define TEST_INSTRUMENT_BYTES 94U
#define TEST_BANK_BYTES 512U
#define TEST_INSTRUMENT_COUNT 3U
#define TEST_HEADER_BYTES (8U + TEST_INSTRUMENT_COUNT * AUDIO_FAR_POINTER_SIZE + 5U)

enum missing_note_kind { MISSING_INSTRUMENT, UNBOUND_INSTRUMENT, MISSING_PERCUSSION };

struct sequence_fixture {
	legacy_u8 *header;
	legacy_u8 *events;
	legacy_u8 *instrument;
};

static void make_instrument(legacy_u8 *resource)
{
	LEGACY_WRITE_U32_LE(resource, TEST_INSTRUMENT_BYTES);
	/* Context 1 is the first playable AdLib voice; direct mode also uses
	 * driver channel 1. The remaining fields describe a sustained sine. */
	LEGACY_WRITE_U16_LE(resource + 12, 2);
	resource[67] = 1;
	resource[74] = 63;
	resource[76] = 1;
	resource[78] = 1;
	resource[82] = 15;
	resource[85] = 15;
	resource[88] = 1;
	resource[90] = 1;
}

static struct sequence_fixture make_fixture(legacy_u8 kind, legacy_u8 volume)
{
	legacy_u16 segment = dos_memory_allocate(TEST_BANK_BYTES * 2U / 16U);
	assert(segment != 0);
	legacy_u8 *effects = dos_memory_make_pointer(segment, 0);
	legacy_u8 *voices = effects + TEST_BANK_BYTES;
	memset(effects, 0, TEST_BANK_BYTES * 2U);

	/* MISS is deliberately absent. PERC selects a percussion bank whose
	 * individual drum instruments are all absent as well. */
	LEGACY_WRITE_U32_LE(voices, 22U + TEST_INSTRUMENT_BYTES * 2U);
	LEGACY_WRITE_U16_LE(voices + 4, 2);
	memcpy(voices + 6, "TONEPERC", 8);
	LEGACY_WRITE_U32_LE(voices + 14, 0);
	LEGACY_WRITE_U32_LE(voices + 18, TEST_INSTRUMENT_BYTES);
	legacy_u8 *instrument = voices + 22;
	make_instrument(instrument);
	make_instrument(instrument + TEST_INSTRUMENT_BYTES);
	instrument[TEST_INSTRUMENT_BYTES + 5] = 5;

	legacy_u8 events[] = {
		0, SEQUENCE_COMMAND(SET_INSTRUMENT), 0, 0, SEQUENCE_COMMAND(SET_VOLUME), volume, 0, 60, 5,
		2, SEQUENCE_COMMAND(SET_INSTRUMENT), 1, 0, SEQUENCE_COMMAND(SET_VOLUME), 127,	 0, 64, 5,
		2, SEQUENCE_COMMAND(RETURN)};
	if (kind == MISSING_PERCUSSION) {
		events[2] = 2;
		events[7] = 24;
	}
	size_t skipped = kind == UNBOUND_INSTRUMENT ? 3U : 0U;
	size_t event_bytes = sizeof(events) - skipped;

	/* An SFX bank contains a nested song with hdr1 and one sequence. Use
	 * real relocation instead of injecting already-resolved channel pointers. */
	legacy_u8 *song = effects + 14;
	legacy_u8 *header = song + 22;
	legacy_u8 *sequence = header + TEST_HEADER_BYTES;
	legacy_u32 song_bytes = 22U + TEST_HEADER_BYTES + 4U + event_bytes;
	LEGACY_WRITE_U32_LE(effects, 14U + song_bytes);
	LEGACY_WRITE_U16_LE(effects + 4, 1);
	memcpy(effects + 6, "TEST", 4);
	LEGACY_WRITE_U32_LE(effects + 10, 0);
	LEGACY_WRITE_U32_LE(song, song_bytes);
	LEGACY_WRITE_U16_LE(song + 4, 2);
	memcpy(song + 6, "hdr1seq1", 8);
	LEGACY_WRITE_U32_LE(song + 14, 0);
	LEGACY_WRITE_U32_LE(song + 18, TEST_HEADER_BYTES);
	LEGACY_WRITE_U32_LE(header, TEST_HEADER_BYTES);
	header[4] = AUDIO_RESOURCE_TYPE_EFFECT;
	header[6] = TEST_INSTRUMENT_COUNT;
	memcpy(header + 7, "MISSTONEPERC", TEST_INSTRUMENT_COUNT * AUDIO_FAR_POINTER_SIZE);
	header[7 + TEST_INSTRUMENT_COUNT * AUDIO_FAR_POINTER_SIZE] = 1;
	memcpy(header + 8 + TEST_INSTRUMENT_COUNT * AUDIO_FAR_POINTER_SIZE, "seq1", 4);
	LEGACY_WRITE_U32_LE(sequence, 4U + event_bytes);
	memcpy(sequence + 4, events + skipped, event_bytes);

	assert(init_audio_resources(effects, voices, "TEST") == header);
	assert(audio_read_far_pointer(header + 7) == NULL);
	assert(audio_read_far_pointer(header + 11) == instrument);
	assert(audio_bass_drum_resource == NULL);
	struct sequence_fixture fixture = {header, sequence + 4, instrument};
	return fixture;
}

static void check_missing_note(legacy_u8 kind, legacy_u8 volume, legacy_s32 direct)
{
	dos_audio_uses_direct_channels = (legacy_u8)direct;
	audio_reset_channels();
	struct sequence_fixture fixture = make_fixture(kind, volume);
	assert(audio_play_effect(fixture.header, TEST_CHANNEL, 64) == TEST_CHANNEL);
	struct AUDIO_CHANNEL *channel = &audio_channels[TEST_CHANNEL];
	assert(audio_read_far_pointer((legacy_u8 *)&channel->cursor) == fixture.events);
	assert(channel->active_notes == 0);
	struct AUDIO_CONTEXT before[AUDIO_CONTEXT_COUNT];
	memcpy(before, dos_audio_contexts, sizeof(before));

	/* Racing disables music, but effects still run through this timer. A
	 * missing note must be skipped without allocating or reclaiming a voice. */
	audio_sequence_timer();
	assert(channel->volume == volume);
	assert(channel->active_notes == 0);
	assert(memcmp(before, dos_audio_contexts, sizeof(before)) == 0);
	size_t consumed = kind == UNBOUND_INSTRUMENT ? 6U : 9U;
	assert(audio_read_far_pointer((legacy_u8 *)&channel->cursor) == fixture.events + consumed);
	assert(channel->delay == 1);
	if (kind != MISSING_PERCUSSION) {
		assert(audio_read_far_pointer((legacy_u8 *)&channel->resource) == NULL);
		assert(channel->driver_channel == 255);
	}

	audio_sequence_timer();
	assert(channel->delay == 0);
	assert(memcmp(before, dos_audio_contexts, sizeof(before)) == 0);
	audio_sequence_timer();
	assert(channel->active_notes == (direct ? 0 : 1));
	assert(channel->volume == 127);
	assert(audio_read_far_pointer((legacy_u8 *)&channel->resource) == fixture.instrument);
	legacy_u32 playing = 0;
	for (legacy_u32 index = 0; index < AUDIO_CONTEXT_COUNT; ++index) {
		struct AUDIO_CONTEXT *context = &dos_audio_contexts[index];
		if (context->state == AUDIO_CONTEXT_STATE_PLAYING) {
			assert(context->channel == TEST_CHANNEL);
			assert(context->driver_channel == 1);
			assert(audio_read_far_pointer((legacy_u8 *)&context->resource) == fixture.instrument);
			++playing;
		}
	}
	assert(playing == 1);

	audio_sequence_timer();
	audio_sequence_timer();
	assert(audio_read_far_pointer((legacy_u8 *)&channel->cursor) == NULL);
	assert(channel->active_notes == 0);
	for (legacy_u32 index = 0; index < AUDIO_CONTEXT_COUNT; ++index) {
		assert(dos_audio_contexts[index].state == AUDIO_CONTEXT_STATE_FREE);
	}
}

int main(void)
{
	SDL_SetMainReady();
	assert(SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "dummy", SDL_HINT_OVERRIDE));
	assert(audio_load_dos_driver("ad15.drv", 0, 0) == 0);
	assert(SDL_GetCurrentAudioDriver() != NULL);
	assert(strcmp(SDL_GetCurrentAudioDriver(), "dummy") == 0);
	audio_music_enabled = AUDIO_STATE_DISABLED;
	audio_music_active = AUDIO_STATE_DISABLED;
	for (legacy_s32 direct = 0; direct <= 1; ++direct) {
		check_missing_note(MISSING_INSTRUMENT, 0, direct);
		check_missing_note(MISSING_INSTRUMENT, 127, direct);
		check_missing_note(UNBOUND_INSTRUMENT, 127, direct);
		check_missing_note(MISSING_PERCUSSION, 127, direct);
	}
	dos_audio_shutdown();
	SDL_Quit();
	puts("SDL3 audio sequences: missing instruments skip notes and preserve later playback");
	return 0;
}
