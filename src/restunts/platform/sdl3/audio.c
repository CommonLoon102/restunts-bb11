/* Native implementation of the AdLib driver interface. Resource offsets and
 * register arithmetic follow the supplied AD15.DRV; the portable sequencer
 * continues to own channel allocation, note timing, envelopes and modulation.
 * Only the main thread accesses this state. SDL consumes queued PCM separately. */
#include <SDL3/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../../c/audio.h"
#include "../../c/audio_internal.h"
#include "../../c/platform.h"
#include "../../c/timing.h"
#include "../../c/resource_bytes.h"
#ifdef __DJGPP__
#include <inlines/pc.h>
#else
#include "audio_trace.h"
#include "opl2.h"
#define ADLIB_EMULATOR "nuked"
#endif

extern legacy_s32 sdl3_batch_mode;

#define ADLIB_VOICES 9U
/* The original context mask reserves bit zero for its sampled-sound path.
 * FM contexts 1..9 correspond to OPL channels 0..8. */
#define ADLIB_CONTEXTS 10U
#define ADLIB_SAMPLE_RATE 44100U
#define ADLIB_TICK_SAMPLES (ADLIB_SAMPLE_RATE / 100U)
#define ADLIB_RESOURCE_SIZE 94U
#define ADLIB_OPERATOR_SIZE 12U
#define ADLIB_OPERATOR_OFFSET 70U

struct AUDIO_TIMER audio_timers[AUDIO_TIMER_COUNT];
struct AUDIO_CHANNEL audio_channels[AUDIO_CHANNEL_COUNT];
struct AUDIO_CHANNEL *audio_sfx_channels = audio_channels + AUDIO_EFFECT_CHANNEL_FIRST;
struct AUDIO_CONTEXT dos_audio_contexts[AUDIO_CONTEXT_COUNT];
legacy_u8 dos_audio_master_state[3] = {16, 0, 22};
legacy_u8 dos_audio_driver_data[256];
void *dos_audio_driver_binary;
legacy_s16 audio_update_lock = AUDIO_UPDATE_LOCKED;
legacy_s8 audio_music_enabled = AUDIO_STATE_ENABLED;
legacy_s8 audio_effects_enabled = AUDIO_STATE_ENABLED;
legacy_u8 dos_audio_uses_direct_channels;
legacy_u8 dos_audio_special_mode;
legacy_u8 dos_audio_master_volume;
legacy_u8 dos_audio_context_count;

struct ADLIB_VOICE {
	const legacy_u8 *resource;
	struct AUDIO_CHANNEL *channel;
	legacy_u16 base_pitch;
	legacy_u16 current_pitch;
	legacy_u8 note;
	legacy_u8 velocity;
	legacy_u8 doubled_multipliers;
};

static struct ADLIB_VOICE adlib_voices[ADLIB_VOICES];
static legacy_u8 adlib_registers[256];
static legacy_u8 adlib_register_valid[256];
static legacy_s32 adlib_ready;
static const legacy_u8 adlib_slots[ADLIB_VOICES] = {0, 1, 2, 8, 9, 10, 16, 17, 18};
static const legacy_u16 adlib_frequencies[60] = {
	21,	 23,  24,  25,	27,	 29,  30,  32,	34,	 36,  38,  40,	43,	 45,  48,
	51,	 54,  57,  61,	64,	 68,  72,  76,	81,	 86,  91,  96,	102, 108, 114,
	121, 128, 136, 144, 153, 162, 171, 182, 192, 204, 216, 229, 242, 257, 272,
	288, 306, 324, 343, 363, 385, 408, 432, 458, 485, 514, 544, 577, 611, 647};
#ifndef __DJGPP__
static opl2_chip *adlib_chip;
static legacy_s32 adlib_initializing;
static SDL_AudioStream *adlib_stream;

static void adlib_delete_chip(void)
{
	free(adlib_chip);
	adlib_chip = NULL;
}

static void adlib_generate_samples(legacy_s16 *samples, legacy_u32 count)
{
	OPL2_GenerateStream(adlib_chip, samples, count);
	adlib_trace_advance(count);
}
#endif

static void adlib_write(legacy_u32 reg, legacy_u32 value)
{
	legacy_u8 byte = (legacy_u8)value;
	if (!adlib_ready || reg >= sizeof(adlib_registers)) {
		return;
	}
	if (adlib_register_valid[reg] && adlib_registers[reg] == byte) {
		return;
	}
	adlib_registers[reg] = byte;
	adlib_register_valid[reg] = 1;
#ifdef __DJGPP__
	outportb(0x388, reg);
	for (legacy_s32 delay = 0; delay < 6; ++delay) {
		(void)inportb(0x388);
	}
	outportb(0x389, byte);
	for (legacy_s32 delay = 0; delay < 35; ++delay) {
		(void)inportb(0x388);
	}
#else
	/* Buffer runtime writes so same-tick key-off/key-on transitions survive. */
	legacy_s32 buffered = !adlib_initializing;
	if (buffered) {
		OPL2_WriteRegBuffered(adlib_chip, (legacy_u8)reg, byte);
	} else {
		OPL2_WriteReg(adlib_chip, (legacy_u8)reg, byte);
	}
	adlib_trace_write(reg, byte, buffered);
#endif
}

static legacy_s32 adlib_voice_index(legacy_s16 driver_channel)
{
	return driver_channel > 0 && (legacy_u16)driver_channel <= ADLIB_VOICES ? driver_channel - 1
																			: -1;
}

static legacy_u16 adlib_note_pitch(legacy_u32 note)
{
	note &= 255U;
	return (legacy_u16)((((note / 12U) & 7U) << 10U) | adlib_frequencies[24U + note % 12U]);
}

/* AD15 bends within an extended F-number table while retaining the note's
 * block. Interpolating packed pitches across octaves also changes the block. */
static legacy_u16 adlib_bend_target(legacy_u32 note, legacy_s32 semitones)
{
	note &= 255U;
	legacy_s32 index = 24 + (legacy_s32)(note % 12U) + semitones;
	if (index < 0) {
		index = 0;
	} else if (index >= (legacy_s32)(sizeof(adlib_frequencies) / sizeof(adlib_frequencies[0]))) {
		index = (legacy_s32)(sizeof(adlib_frequencies) / sizeof(adlib_frequencies[0])) - 1;
	}
	return (legacy_u16)((((note / 12U) & 7U) << 10U) | adlib_frequencies[index]);
}

/* A half-rate carrier rounds its phase increment independently from the
 * modulator. Low pitches can therefore leave a different relative phase
 * after returning to the same RPM. Integer multipliers retain the ratio.
 * Only normalize sustained, continuously pitched FM voices whose steady
 * level and key scaling permit lowering the pitch block. */
static legacy_u32 adlib_continuous_multiplier(const struct ADLIB_VOICE *state)
{
	static const legacy_u8 doubled[7] = {0, 2, 4, 6, 8, 10, 12};
	static const legacy_u8 selectors[6] = {22, 23, 24, 25, 40, 53};
	const legacy_u8 *resource = state->resource;
	if (state->note != 255U || !resource || resource[68] != 0 || resource[53] == 0x91U) {
		return 0;
	}
	const legacy_u8 *modulator = resource + ADLIB_OPERATOR_OFFSET;
	const legacy_u8 *carrier = modulator + ADLIB_OPERATOR_SIZE;
	if (carrier[6] != 0 || modulator[6] == 0 || modulator[6] >= sizeof(doubled)) {
		return 0;
	}
	for (legacy_u32 index = 0; index < sizeof(selectors); ++index) {
		legacy_u32 selector = resource[selectors[index]];
		if (selector == 0x81U || selector == 0x82U) {
			return 0;
		}
	}
	for (legacy_u32 index = 0; index < 2; ++index) {
		const legacy_u8 *op = modulator + index * ADLIB_OPERATOR_SIZE;
		if (op[0] != 15 || op[2] != 0 || op[3] != 15 || op[5] != 0 || op[7] != 0 || op[8] != 1 ||
			op[9] != 0) {
			return 0;
		}
	}
	return doubled[modulator[6]];
}

static void adlib_write_pitch(legacy_s32 voice, legacy_u16 pitch, legacy_s32 key_on)
{
	struct ADLIB_VOICE *state = &adlib_voices[voice];
	state->current_pitch = pitch;
	legacy_u32 multiplier = adlib_continuous_multiplier(state);
	if (multiplier != 0 || state->doubled_multipliers) {
		legacy_u32 slot = adlib_slots[voice];
		const legacy_u8 *modulator = state->resource + ADLIB_OPERATOR_OFFSET;
		const legacy_u8 *carrier = modulator + ADLIB_OPERATOR_SIZE;
		adlib_write(0x20U + slot, (adlib_registers[0x20U + slot] & 0xf0U) |
									  (multiplier != 0 ? multiplier : modulator[6]));
		adlib_write(0x23U + slot,
					(adlib_registers[0x23U + slot] & 0xf0U) | (multiplier != 0 ? 1U : carrier[6]));
	}
	state->doubled_multipliers = multiplier != 0;
	if (multiplier != 0) {
		/* Doubling both multipliers and halving the base pitch preserves the
		 * carrier's exact phase increment, including its low-pitch rounding.
		 * Every intermediate A0/B0 write also retains the integer ratio. */
		pitch &= 0x1fffU;
		pitch = (pitch & 0x1c00U) != 0 ? pitch - 0x400U : pitch >> 1U;
	}
	adlib_write(0xa0U + voice, pitch & 255U);
	adlib_write(0xb0U + voice, ((pitch >> 8U) & 31U) | (key_on ? 32U : 0U));
}

static void adlib_program_operator(legacy_u32 slot, const legacy_u8 *op, legacy_u32 multiplier)
{
	adlib_write(0x20U + slot,
				(op[10] << 7U) | (op[9] << 6U) | (op[8] << 5U) | (op[7] << 4U) | multiplier);
	adlib_write(0x40U + slot, (op[5] << 6U) | op[4]);
	adlib_write(0x60U + slot, (op[0] << 4U) | op[1]);
	adlib_write(0x80U + slot, (op[2] << 4U) | op[3]);
	adlib_write(0xe0U + slot, op[11] & 3U);
}

static void adlib_volume(legacy_s32 voice, legacy_u32 volume)
{
	const struct ADLIB_VOICE *state = &adlib_voices[voice];
	const legacy_u8 *resource = state->resource;
	if (!resource) {
		return;
	}
	if (volume > 127U) {
		volume = 127U;
	}
	/* AD15 rounds two products independently, with a denominator of 128. */
	legacy_u32 scale = (((volume * state->velocity) >> 6U) + 1U) >> 1U;
	for (legacy_u32 op_index = 0; op_index < 2; ++op_index) {
		const legacy_u8 *op = resource + ADLIB_OPERATOR_OFFSET + op_index * ADLIB_OPERATOR_SIZE;
		legacy_u32 level = op[4] & 63U;
		if (op_index != 0 || resource[68] == 1) {
			legacy_u32 gain = ((((63U - level) * scale) >> 6U) + 1U) >> 1U;
			level = 63U - (gain & 63U);
		}
		adlib_write(0x40U + adlib_slots[voice] + op_index * 3U, (op[5] << 6U) | level);
	}
}

/* The voice resource selects which OPL property a controller/envelope drives. */
static void adlib_control(legacy_s32 voice, legacy_u32 selector, legacy_u32 value)
{
	const legacy_u8 *resource = adlib_voices[voice].resource;
	if (!resource || selector < 0x81U || selector > 0x85U) {
		return;
	}
	if (selector == 0x85U) {
		adlib_write(0xc0U + voice, (((value >> 4U) & 7U) << 1U) | resource[68]);
		return;
	}
	legacy_u32 operator_index = (selector - 0x81U) & 1U;
	legacy_u32 slot = adlib_slots[voice] + operator_index * 3U;
	const legacy_u8 *op = resource + ADLIB_OPERATOR_OFFSET + operator_index * ADLIB_OPERATOR_SIZE;
	if (selector < 0x83U) {
		adlib_write(0x20U + slot, (op[10] << 7U) | (op[9] << 6U) | (op[8] << 5U) | (op[7] << 4U) |
									  ((value >> 3U) & 7U));
	} else {
		adlib_write(0x40U + slot, (op[5] << 6U) | (((127U - value) >> 1U) & 63U));
	}
}

legacy_u8 dos_audio_driver_initialize(void)
{
	adlib_ready = 0;
	memset(adlib_register_valid, 0, sizeof(adlib_register_valid));
	memset(adlib_voices, 0, sizeof(adlib_voices));
	if (sdl3_batch_mode) {
		/* Dump tools still resolve instruments and maintain sequencer state,
		 * but do not need an audio device or access to DOS hardware ports. */
		adlib_ready = 0;
		return ADLIB_CONTEXTS;
	}
#ifdef __DJGPP__
	adlib_ready = 1;
	adlib_write(4, 0x60);
	adlib_write(4, 0x80);
	legacy_u32 initial = inportb(0x388);
	adlib_write(2, 0xff);
	adlib_write(4, 0x21);
	for (legacy_s32 delay = 0; delay < 400; ++delay) {
		(void)inportb(0x388);
	}
	legacy_u32 running = inportb(0x388);
	adlib_write(4, 0x60);
	adlib_write(4, 0x80);
	if ((initial & 0xe0U) != 0 || (running & 0xe0U) != 0xc0U) {
		adlib_ready = 0;
		fputs("Audio unavailable: no AdLib-compatible chip at port 388h; continuing silently.\n",
			  stderr);
		return ADLIB_CONTEXTS;
	}
#else
	adlib_chip = calloc(1, sizeof(*adlib_chip));
	if (!adlib_chip) {
		fputs("Audio unavailable: cannot allocate OPL synthesizer; continuing silently.\n", stderr);
		return ADLIB_CONTEXTS;
	}
	OPL2_Reset(adlib_chip, ADLIB_SAMPLE_RATE);
	if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
		fprintf(stderr, "Audio unavailable: %s; continuing silently.\n", SDL_GetError());
		adlib_delete_chip();
		return ADLIB_CONTEXTS;
	}
	SDL_AudioSpec spec = {SDL_AUDIO_S16, 1, ADLIB_SAMPLE_RATE};
	adlib_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	if (!adlib_stream || !SDL_ResumeAudioStreamDevice(adlib_stream)) {
		fprintf(stderr, "Audio unavailable: %s; continuing silently.\n", SDL_GetError());
		SDL_DestroyAudioStream(adlib_stream);
		adlib_stream = NULL;
		adlib_delete_chip();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return ADLIB_CONTEXTS;
	}
	adlib_ready = 1;
	adlib_initializing = 1;
	adlib_trace_open(ADLIB_EMULATOR, 3579545, ADLIB_SAMPLE_RATE);
#endif
	for (legacy_u32 reg = 0; reg < 256; ++reg) {
		adlib_write(reg, 0);
	}
	adlib_write(1, 0x20);
	adlib_write(8, 0x40);
#ifndef __DJGPP__
	adlib_initializing = 0;
#endif
	return ADLIB_CONTEXTS;
}

/* Called once after each 100 Hz main-thread timer tick. Keeping the synthesizer
 * here avoids racing the sequencer or resource frees in SDL's audio thread. */
void sdl3_audio_update(void)
{
#ifndef __DJGPP__
	if (!adlib_ready || !adlib_stream) {
		return;
	}
	/* Bound latency after a debugger stop or a catch-up burst. */
	if (SDL_GetAudioStreamQueued(adlib_stream) > (legacy_s32)(ADLIB_SAMPLE_RATE / 5U)) {
		adlib_trace_clear();
		SDL_ClearAudioStream(adlib_stream);
	}
	legacy_s16 samples[ADLIB_TICK_SAMPLES];
	adlib_generate_samples(samples, ADLIB_TICK_SAMPLES);
	SDL_PutAudioStreamData(adlib_stream, samples, sizeof(samples));
#endif
}

void dos_audio_driver_prepare_context(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context,
									  legacy_u8 *timer, void *resource)
{
	(void)context;
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice < 0 || !resource || resource_read_u32le(resource) < ADLIB_RESOURCE_SIZE) {
		return;
	}
	struct ADLIB_VOICE *state = &adlib_voices[voice];
	legacy_s32 was_doubled = state->doubled_multipliers;
	state->resource = resource;
	state->channel = (struct AUDIO_CHANNEL *)timer;
	legacy_u32 multiplier = adlib_continuous_multiplier(state);
	state->doubled_multipliers = multiplier != 0;
	const legacy_u8 *bytes = resource;
	const legacy_u8 *modulator = bytes + ADLIB_OPERATOR_OFFSET;
	const legacy_u8 *carrier = modulator + ADLIB_OPERATOR_SIZE;
	adlib_write(0xc0U + voice, (bytes[69] << 1U) | bytes[68]);
	adlib_program_operator(adlib_slots[voice], modulator,
						   multiplier != 0 ? multiplier : modulator[6]);
	adlib_program_operator(adlib_slots[voice] + 3U, carrier, multiplier != 0 ? 1U : carrier[6]);
	if (was_doubled != state->doubled_multipliers) {
		/* Resource rebinding may happen while a voice is sounding. Keep its
		 * last pitch in the same units as the newly programmed multipliers. */
		adlib_write_pitch(voice, state->current_pitch, adlib_registers[0xb0U + voice] & 32U);
	}
	if (state->channel) {
		adlib_control(voice, bytes[22], state->channel->driver_private_state[0]);
		adlib_control(voice, bytes[23], state->channel->driver_private_state[2]);
		adlib_control(voice, bytes[24], state->channel->driver_private_state[3]);
	}
}

void dos_audio_driver_set_context_value(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context,
										legacy_u16 value)
{
	(void)context;
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice < 0) {
		return;
	}
	/* The engine supplies Hz. AD15 normalizes to its 50 kHz OPL clock. */
	legacy_u32 numerator = (legacy_u32)value << 16U;
	legacy_u32 block = 0;
	while (numerator > 25550000UL && block < 7U) {
		numerator >>= 1U;
		++block;
	}
	adlib_voices[voice].base_pitch = (legacy_u16)((numerator / 50000UL) | (block << 10U));
}

void dos_audio_driver_activate_context(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context,
									   legacy_u8 *timer, legacy_s16 pitch, legacy_u16 parameter,
									   void *resource)
{
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice < 0 || !resource) {
		return;
	}
	struct ADLIB_VOICE *state = &adlib_voices[voice];
	if (state->resource != resource) {
		dos_audio_driver_prepare_context(driver_channel, context, timer, resource);
	}
	if (state->resource != resource) {
		return;
	}
	state->channel = (struct AUDIO_CHANNEL *)timer;
	state->note = (legacy_u8)pitch;
	if (state->note != 255U) {
		state->base_pitch = adlib_note_pitch(state->note);
	}
	state->velocity = state->resource[21] ? (legacy_u8)(parameter & 127U) : 127U;
	adlib_write_pitch(voice, state->base_pitch + (legacy_s8)state->resource[17], 0);
	adlib_write_pitch(voice, state->base_pitch + (legacy_s8)state->resource[17], 1);
	adlib_volume(voice, state->channel->volume);
}

void dos_audio_driver_start_context(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context)
{
	(void)context;
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice >= 0) {
		adlib_write(0xb0U + voice, adlib_registers[0xb0U + voice] & ~32U);
	}
}

void dos_audio_driver_end_context(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context)
{
	dos_audio_driver_start_context(driver_channel, context);
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice >= 0) {
		adlib_write(0x40U + adlib_slots[voice], 63U);
		adlib_write(0x43U + adlib_slots[voice], 63U);
	}
}

void dos_audio_driver_release_channel(legacy_s16 driver_channel)
{
	dos_audio_driver_end_context(driver_channel, NULL);
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice >= 0) {
		memset(&adlib_voices[voice], 0, sizeof(adlib_voices[voice]));
	}
}

void dos_audio_driver_start(void)
{
	for (legacy_s16 channel = 1; (legacy_u16)channel <= ADLIB_VOICES; ++channel) {
		dos_audio_driver_release_channel(channel);
	}
}

void dos_audio_driver_suspend_context(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context,
									  legacy_u16 value, void *resource)
{
	(void)value;
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice < 0 || !context || !context->state || !resource) {
		return;
	}
	struct ADLIB_VOICE *state = &adlib_voices[voice];
	const legacy_u8 *bytes = resource;
	if (state->resource != bytes) {
		return;
	}
	legacy_u16 pitch = state->base_pitch;
	if (bytes[53] == 0x91U) {
		pitch = adlib_note_pitch(state->note + context->sequence_value);
	}
	if (bytes[40] == 0x90U) {
		pitch = LEGACY_U16_WRAP_ADD(pitch, context->modulation);
	}
	if (bytes[25] == 0x90U) {
		pitch = LEGACY_U16_WRAP_ADD(pitch, context->level);
	}
	legacy_s16 bend = state->channel ? LEGACY_S16_FROM_BITS(state->channel->pitch) : 0;
	if (bend != 0) {
		legacy_s32 semitones = bend > 0 ? bytes[18] : -(legacy_s32)bytes[18];
		legacy_s32 difference = (legacy_s32)adlib_bend_target(state->note, semitones) - pitch;
		/* AD15 shifts signed products, then subtracts for downward bends. */
		legacy_s32 delta = LEGACY_S32_SAR(difference * bend, 13U);
		pitch = LEGACY_U16_WRAP_ADD(pitch, bend > 0 ? delta : -delta);
	}
	pitch = LEGACY_U16_WRAP_ADD(pitch, (legacy_s8)bytes[17]);
	adlib_write_pitch(voice, pitch, context->state == AUDIO_CONTEXT_STATE_PLAYING);
	adlib_control(voice, bytes[53], context->sequence_value);
	adlib_control(voice, bytes[40], (legacy_u16)context->modulation);
	adlib_control(voice, bytes[25], (legacy_u16)context->level);
}

void dos_audio_set_channel_volume(legacy_s16 channel, legacy_s16 volume)
{
	if (channel < 0 || (legacy_u16)channel >= AUDIO_CHANNEL_COUNT) {
		return;
	}
	audio_channels[channel].volume = (legacy_u8)volume;
	for (legacy_u32 voice = 0; voice < ADLIB_VOICES; ++voice) {
		if (dos_audio_contexts[voice + 1U].channel == channel) {
			adlib_volume(voice, (legacy_u8)volume);
		}
	}
}

void dos_audio_bind_channel_context(legacy_s16 channel, void *resource)
{
	if (channel < 0 || (legacy_u16)channel >= AUDIO_CHANNEL_COUNT || !resource) {
		return;
	}
	struct AUDIO_CHANNEL *state = &audio_channels[channel];
	audio_write_far_pointer((legacy_u8 *)&state->resource, resource);
	const legacy_u8 *bytes = resource;
	state->driver_channel = bytes[67] < 16U ? bytes[67] : (legacy_u8)((channel & 15) + 1);
	for (legacy_s16 index = 1; (legacy_u16)index < ADLIB_CONTEXTS; ++index) {
		if (dos_audio_contexts[index].channel == channel) {
			dos_audio_driver_prepare_context(index, &dos_audio_contexts[index], (legacy_u8 *)state,
											 resource);
		}
	}
}

void dos_audio_set_context_pitch(legacy_s16 context_index, legacy_s16 pitch)
{
	if (context_index < 0 || (legacy_u16)context_index >= ADLIB_CONTEXTS) {
		return;
	}
	struct AUDIO_CONTEXT *context = &dos_audio_contexts[context_index];
	dos_audio_driver_set_context_value(context->driver_channel, context, (legacy_u16)pitch);
}

void dos_audio_driver_set_control(legacy_s16 driver_channel, struct AUDIO_CONTEXT *context,
								  legacy_u16 control, legacy_u16 value)
{
	legacy_s32 voice = adlib_voice_index(driver_channel);
	if (voice < 0 || !context || !context->state || !adlib_voices[voice].resource) {
		return;
	}
	const legacy_u8 *resource = adlib_voices[voice].resource;
	if (control == 7U) {
		adlib_volume(voice, value);
	} else if (control == 1U || control == 11U || control == 12U) {
		legacy_u32 offset = control == 1U ? 22U : control == 11U ? 23U : 24U;
		adlib_control(voice, resource[offset], value);
	}
}

/* These operations are also no-ops in AD15: banks/master SysEx belong to
 * MT-32, and pitch changes are applied by the per-context 100 Hz update. */
void dos_audio_driver_load_bank(void *bank)
{
	(void)bank;
}

void dos_audio_driver_set_pitch(legacy_u8 *timer, legacy_s16 pitch, legacy_s16 driver_channel)
{
	(void)timer;
	(void)pitch;
	(void)driver_channel;
}

void dos_audio_driver_send_data(legacy_u16 length, legacy_u8 *data)
{
	(void)length;
	(void)data;
}

void dos_audio_driver_reset(void)
{
}

void dos_audio_driver_suspend_all(struct AUDIO_CONTEXT *contexts)
{
	(void)contexts;
}

void dos_audio_driver_set_master_state(legacy_s16 operation, void *state)
{
	(void)operation;
	(void)state;
}

void dos_audio_shutdown(void)
{
	audio_update_lock = AUDIO_UPDATE_LOCKED;
	timer_remove_callback(audio_sequence_timer);
	dos_audio_driver_start();
	adlib_ready = 0;
#ifndef __DJGPP__
	adlib_trace_close();
	if (adlib_stream) {
		SDL_DestroyAudioStream(adlib_stream);
		adlib_stream = NULL;
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}
	if (adlib_chip) {
		adlib_delete_chip();
	}
#endif
	dos_audio_driver_binary = NULL;
	dos_audio_uses_direct_channels = 0;
	dos_audio_special_mode = 0;
	dos_audio_context_count = 0;
	audio_music_enabled = AUDIO_STATE_DISABLED;
	audio_effects_enabled = AUDIO_STATE_DISABLED;
	audio_update_lock = AUDIO_UPDATE_UNLOCKED;
}
