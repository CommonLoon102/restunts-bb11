/* Exercises the native driver against a shipped AdLib instrument and real
 * YM3812 synthesis without requiring an audio device. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include "../platform/sdl3/audio.c"

int sdl3_batch_mode;
static unsigned int removed_callbacks;

void audio_sequence_timer(void)
{
}

void timer_remove_callback(void (*callback)(void))
{
	assert(callback == audio_sequence_timer);
	++removed_callbacks;
}

legacy_u32 resource_read_u32le(const legacy_u8 *source)
{
	return LEGACY_READ_U32_LE(source);
}

/* The driver calls this sequencer helper only when rebinding a resource;
 * the tests below prepare voices through the driver API directly. */
void audio_write_far_pointer(legacy_u8 *destination, const void *value)
{
	(void)destination;
	(void)value;
	assert(0 && "Unexpected sequencer resource rebinding");
}

static void load_first_instrument(const char *path, legacy_u8 *instrument)
{
	FILE *file = fopen(path, "rb");
	assert(file != NULL);
	legacy_u8 header[6];
	assert(fread(header, 1, sizeof(header), file) == sizeof(header));
	unsigned int count = LEGACY_READ_U16_LE(header + 4);
	assert(count > 0 && count < 256);
	assert(fseek(file, 6L + count * 4L, SEEK_SET) == 0);
	legacy_u8 offset[4];
	assert(fread(offset, 1, sizeof(offset), file) == sizeof(offset));
	assert(fseek(file, 6L + count * 8L + LEGACY_READ_U32_LE(offset), SEEK_SET) == 0);
	assert(fread(instrument, 1, ADLIB_RESOURCE_SIZE, file) == ADLIB_RESOURCE_SIZE);
	fclose(file);
}

static unsigned long long pcm_energy(void)
{
	unsigned long long energy = 0;
	for (unsigned int frame = 0; frame < ADLIB_SAMPLE_RATE / 4; ++frame) {
		int sample = OPL_calc(adlib_chip);
		energy += (unsigned int)(sample * sample);
	}
	return energy;
}

int main(int argc, char **argv)
{
	assert(argc == 2);
	legacy_u8 instrument[ADLIB_RESOURCE_SIZE];
	load_first_instrument(argv[1], instrument);
	SDL_SetMainReady();
	SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "restunts-missing-audio-driver",
							SDL_HINT_OVERRIDE);
	assert(dos_audio_driver_initialize() == ADLIB_CONTEXTS);
	assert(!adlib_ready && adlib_chip == NULL && adlib_stream == NULL);
	/* Batch tools never acquire a device, even when an audio driver exists. */
	SDL_SetHintWithPriority(SDL_HINT_AUDIO_DRIVER, "dummy", SDL_HINT_OVERRIDE);
	sdl3_batch_mode = 1;
	assert(dos_audio_driver_initialize() == ADLIB_CONTEXTS);
	assert(!adlib_ready && adlib_chip == NULL && adlib_stream == NULL);
	sdl3_batch_mode = 0;
	assert(dos_audio_driver_initialize() == ADLIB_CONTEXTS);
	assert(adlib_ready && adlib_chip != NULL && adlib_stream != NULL);
	assert(pcm_energy() == 0);

	struct AUDIO_CHANNEL *channel = &audio_channels[0];
	struct AUDIO_CONTEXT *context = &dos_audio_contexts[1];
	channel->volume = 127;
	context->channel = 0;
	context->state = AUDIO_CONTEXT_STATE_PLAYING;
	context->driver_channel = 1;
	dos_audio_driver_prepare_context(1, context, (legacy_u8 *)channel, instrument);
	dos_audio_driver_activate_context(1, context, (legacy_u8 *)channel, 72, 127, instrument);
	assert(adlib_registers[0xa0] == 86);
	assert(adlib_registers[0xb0] == (32 | 24));
	assert(adlib_registers[0xe0] == (instrument[81] & 3));
	assert(adlib_registers[0xe3] == (instrument[93] & 3));
	assert(pcm_energy() > 1000000);

	dos_audio_set_channel_volume(0, 0);
	assert((adlib_registers[0x43] & 63) == 63);
	if (instrument[68] == 1) {
		assert((adlib_registers[0x40] & 63) == 63);
	}
	dos_audio_set_channel_volume(0, 127);
	assert((adlib_registers[0x43] & 63) < 63);

	/* Contexts retain the original numbering: final voice maps to OPL 8. */
	dos_audio_driver_prepare_context(9, &dos_audio_contexts[9], (legacy_u8 *)channel, instrument);
	dos_audio_driver_activate_context(9, &dos_audio_contexts[9], (legacy_u8 *)channel, 60, 127,
									  instrument);
	assert((adlib_registers[0xb8] & 32) != 0);
	assert(adlib_voice_index(0) == -1);
	assert(adlib_voice_index(10) == -1);

	/* Continuous engine values are frequencies, not MIDI note indices. */
	dos_audio_driver_set_context_value(1, context, 1000);
	assert(adlib_voices[0].base_pitch == (2U << 10U | 327U));
	dos_audio_driver_activate_context(1, context, (legacy_u8 *)channel, 255, 127, instrument);
	assert(adlib_registers[0xa0] == 71);
	assert((adlib_registers[0xb0] & 31) == 9);

	instrument[40] = 0x90;
	context->modulation = 9;
	dos_audio_driver_suspend_context(1, context, 0, instrument);
	assert(adlib_registers[0xa0] == 80);
	dos_audio_driver_start_context(1, context);
	assert((adlib_registers[0xb0] & 32) == 0);
	dos_audio_driver_release_channel(1);
	dos_audio_driver_release_channel(9);
	assert(adlib_voices[0].resource == NULL);
	assert(adlib_voices[8].resource == NULL);
	assert((adlib_registers[0x43] & 63) == 63);
	sdl3_audio_update();
	dos_audio_shutdown();
	assert(!adlib_ready && adlib_chip == NULL && adlib_stream == NULL);
	assert(removed_callbacks == 1);
	SDL_Quit();
	puts("SDL3 AdLib: synthesis, device fallback, batch mode, and cleanup passed");
	return 0;
}
