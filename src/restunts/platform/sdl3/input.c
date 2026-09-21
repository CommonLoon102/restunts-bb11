#include "sdl3.h"
#include "../../c/platform.h"
#include "../../c/keyboard.h"
#include "../../c/game_input.h"
#include "../../c/fatal.h"
#include <string.h>

#define KEY_BUFFER_CAPACITY 64U

static bool keys[SDL_SCANCODE_COUNT];
static bool consumed_keys[SDL_SCANCODE_COUNT];
static legacy_u16 key_buffer[KEY_BUFFER_CAPACITY];
static unsigned int key_read;
static unsigned int key_count;
static legacy_s16 mouse_x;
static legacy_s16 mouse_y;
static legacy_s16 mouse_buttons;
static legacy_s16 mouse_min_x;
static legacy_s16 mouse_min_y;
static legacy_s16 mouse_max_x = 319;
static legacy_s16 mouse_max_y = 199;
static bool mouse_available;
struct MOUSE_TRANSITION {
	legacy_s16 buttons;
	legacy_s16 x;
	legacy_s16 y;
};
static struct MOUSE_TRANSITION mouse_transitions[KEY_BUFFER_CAPACITY];
static unsigned int mouse_transition_read;
static unsigned int mouse_transition_count;
static bool joystick_initialized;
static legacy_u8 joystick_enabled;
static SDL_Joystick *joystick;
static bool pumping;
static Uint64 last_event_poll;

static const legacy_u8 dos_kb_keymap1[91] = {
	0,	 27,  49,  50,	51,	 52,  53,  54,	55,	 56,  57,  48,	45,	 61,  8,   9,	113, 119, 101,
	114, 116, 121, 117, 105, 111, 112, 91,	93,	 13,  0,   97,	115, 100, 102, 103, 104, 106, 107,
	108, 59,  39,  96,	0,	 92,  122, 120, 99,	 118, 98,  110, 109, 44,  46,  47,	0,	 42,  0,
	32,	 0,	  187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 0,	 0,	  199, 200, 201, 45,  203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	  0};

static const legacy_u8 dos_kb_keymap2[91] = {
	0,	 27,  33,  64,	35,	 36,  37,  94,	38,	 42,  40,  41,	95, 43, 8,	 143, 81,  87, 69,
	82,	 84,  89,  85,	73,	 79,  80,  123, 125, 13,  0,   65,	83, 68, 70,	 71,  72,  74, 75,
	76,	 58,  34,  126, 0,	 124, 90,  88,	67,	 86,  66,  78,	77, 60, 62,	 63,  0,   0,  0,
	32,	 0,	  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 0,	0,	199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	0,	0};

static const legacy_u8 dos_kb_keymap3[91] = {
	0,	 27,  49,  50,	51,	 52,  53,  54,	55,	 56,  57,  48,	45, 61, 8,	 143, 81,  87, 69,
	82,	 84,  89,  85,	73,	 79,  80,  91,	93,	 13,  0,   65,	83, 68, 70,	 71,  72,  74, 75,
	76,	 59,  39,  96,	0,	 92,  90,  88,	67,	 86,  66,  78,	77, 44, 46,	 47,  0,   0,  0,
	32,	 0,	  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 0,	0,	199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	0,	0};

static const legacy_u8 dos_kb_keymap4[91] = {
	0,	 27,  33,  0,	35,	 36,  37,  30,	38,	 42,  40,  41,	31,	 43, 127, 9,   17,	23, 5,
	18,	 20,  25,  21,	9,	 15,  16,  27,	29,	 13,  0,   1,	19,	 4,	 6,	  7,   8,	10, 11,
	12,	 59,  44,  96,	0,	 28,  26,  24,	3,	 22,  2,   14,	178, 60, 62,  63,  0,	0,	0,
	32,	 0,	  222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 0,	 0,	 199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	 0};

static const legacy_u8 dos_kb_keymap5[92] = {
	0,	 27,  33,  64,	35,	 36,  37,  94,	38,	 42,  40,  41,	95,	 43,  8,   143, 144, 145, 146,
	147, 148, 149, 150, 151, 152, 153, 123, 125, 13,  0,   158, 159, 160, 161, 162, 163, 164, 165,
	166, 58,  34,  126, 0,	 124, 172, 173, 174, 175, 176, 177, 178, 60,  62,  63,	0,	 0,	  0,
	32,	 0,	  248, 249, 250, 251, 252, 253, 254, 255, 128, 129, 0,	 0,	  199, 200, 201, 45,  203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	  0,   0};

static const SDL_Scancode scancodes[] = {SDL_SCANCODE_UNKNOWN,
										 SDL_SCANCODE_ESCAPE,
										 SDL_SCANCODE_1,
										 SDL_SCANCODE_2,
										 SDL_SCANCODE_3,
										 SDL_SCANCODE_4,
										 SDL_SCANCODE_5,
										 SDL_SCANCODE_6,
										 SDL_SCANCODE_7,
										 SDL_SCANCODE_8,
										 SDL_SCANCODE_9,
										 SDL_SCANCODE_0,
										 SDL_SCANCODE_MINUS,
										 SDL_SCANCODE_EQUALS,
										 SDL_SCANCODE_BACKSPACE,
										 SDL_SCANCODE_TAB,
										 SDL_SCANCODE_Q,
										 SDL_SCANCODE_W,
										 SDL_SCANCODE_E,
										 SDL_SCANCODE_R,
										 SDL_SCANCODE_T,
										 SDL_SCANCODE_Y,
										 SDL_SCANCODE_U,
										 SDL_SCANCODE_I,
										 SDL_SCANCODE_O,
										 SDL_SCANCODE_P,
										 SDL_SCANCODE_LEFTBRACKET,
										 SDL_SCANCODE_RIGHTBRACKET,
										 SDL_SCANCODE_RETURN,
										 SDL_SCANCODE_LCTRL,
										 SDL_SCANCODE_A,
										 SDL_SCANCODE_S,
										 SDL_SCANCODE_D,
										 SDL_SCANCODE_F,
										 SDL_SCANCODE_G,
										 SDL_SCANCODE_H,
										 SDL_SCANCODE_J,
										 SDL_SCANCODE_K,
										 SDL_SCANCODE_L,
										 SDL_SCANCODE_SEMICOLON,
										 SDL_SCANCODE_APOSTROPHE,
										 SDL_SCANCODE_GRAVE,
										 SDL_SCANCODE_LSHIFT,
										 SDL_SCANCODE_BACKSLASH,
										 SDL_SCANCODE_Z,
										 SDL_SCANCODE_X,
										 SDL_SCANCODE_C,
										 SDL_SCANCODE_V,
										 SDL_SCANCODE_B,
										 SDL_SCANCODE_N,
										 SDL_SCANCODE_M,
										 SDL_SCANCODE_COMMA,
										 SDL_SCANCODE_PERIOD,
										 SDL_SCANCODE_SLASH,
										 SDL_SCANCODE_RSHIFT,
										 SDL_SCANCODE_KP_MULTIPLY,
										 SDL_SCANCODE_LALT,
										 SDL_SCANCODE_SPACE,
										 SDL_SCANCODE_CAPSLOCK,
										 SDL_SCANCODE_F1,
										 SDL_SCANCODE_F2,
										 SDL_SCANCODE_F3,
										 SDL_SCANCODE_F4,
										 SDL_SCANCODE_F5,
										 SDL_SCANCODE_F6,
										 SDL_SCANCODE_F7,
										 SDL_SCANCODE_F8,
										 SDL_SCANCODE_F9,
										 SDL_SCANCODE_F10,
										 SDL_SCANCODE_NUMLOCKCLEAR,
										 SDL_SCANCODE_SCROLLLOCK,
										 SDL_SCANCODE_HOME,
										 SDL_SCANCODE_UP,
										 SDL_SCANCODE_PAGEUP,
										 SDL_SCANCODE_KP_MINUS,
										 SDL_SCANCODE_LEFT,
										 SDL_SCANCODE_KP_5,
										 SDL_SCANCODE_RIGHT,
										 SDL_SCANCODE_KP_PLUS,
										 SDL_SCANCODE_END,
										 SDL_SCANCODE_DOWN,
										 SDL_SCANCODE_PAGEDOWN,
										 SDL_SCANCODE_INSERT,
										 SDL_SCANCODE_DELETE,
										 SDL_SCANCODE_UNKNOWN,
										 SDL_SCANCODE_UNKNOWN,
										 SDL_SCANCODE_NONUSBACKSLASH,
										 SDL_SCANCODE_F11,
										 SDL_SCANCODE_F12};

static unsigned int legacy_scancode(SDL_Scancode code)
{
	switch (code) {
		case SDL_SCANCODE_RCTRL:
			return 29;
		case SDL_SCANCODE_RALT:
			return 56;
		case SDL_SCANCODE_KP_ENTER:
			return 28;
		case SDL_SCANCODE_KP_7:
			return 71;
		case SDL_SCANCODE_KP_8:
			return 72;
		case SDL_SCANCODE_KP_9:
			return 73;
		case SDL_SCANCODE_KP_4:
			return 75;
		case SDL_SCANCODE_KP_6:
			return 77;
		case SDL_SCANCODE_KP_1:
			return 79;
		case SDL_SCANCODE_KP_2:
			return 80;
		case SDL_SCANCODE_KP_3:
			return 81;
		case SDL_SCANCODE_KP_0:
			return 82;
		case SDL_SCANCODE_KP_PERIOD:
			return 83;
		default:
			break;
	}
	for (unsigned int index = 1; index < SDL_arraysize(scancodes); index++) {
		if (scancodes[index] == code) {
			return index;
		}
	}
	return 0;
}

static void input_key(const SDL_KeyboardEvent *event)
{
	if ((unsigned int)event->scancode >= SDL_SCANCODE_COUNT) {
		return;
	}
	bool was_pressed = keys[event->scancode];
	keys[event->scancode] = event->down;
	if (!event->down) {
		consumed_keys[event->scancode] = false;
		return;
	}
	if (!was_pressed && !event->repeat) {
		consumed_keys[event->scancode] = false;
	}
	if (consumed_keys[event->scancode]) {
		return;
	}
	if ((event->scancode == SDL_SCANCODE_RETURN || event->scancode == SDL_SCANCODE_KP_ENTER) &&
		(event->mod & SDL_KMOD_ALT) != 0) {
		/* Keep Enter consumed until release, even if Alt is released first. */
		consumed_keys[event->scancode] = true;
		if (!was_pressed && !event->repeat) {
			sdl3_video_toggle_fullscreen();
		}
		return;
	}
	unsigned int scan = legacy_scancode(event->scancode);
	if (scan == 0) {
		return;
	}
	legacy_u16 value;
	if (scan == 87 || scan == 88) {
		if (was_pressed || event->repeat ||
			(event->mod & (SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT))) {
			return;
		}
		value = (legacy_u16)(scan == 87 ? KEY_F11 : KEY_F12);
	} else {
		if ((event->mod & SDL_KMOD_ALT) != 0) {
			value = dos_kb_keymap5[scan];
		} else if ((event->mod & SDL_KMOD_CTRL) != 0) {
			value = dos_kb_keymap4[scan];
		} else if ((event->mod & SDL_KMOD_SHIFT) != 0) {
			value = dos_kb_keymap2[scan];
		} else if ((event->mod & SDL_KMOD_CAPS) != 0) {
			value = dos_kb_keymap3[scan];
		} else {
			value = dos_kb_keymap1[scan];
		}
		if ((value & 128U) != 0) {
			if (value >= 133U) {
				value &= 127U;
			}
			value <<= 8;
		}
	}
	if (value == 0) {
		return;
	}
	if (key_count == KEY_BUFFER_CAPACITY) {
		key_read = (key_read + 1U) % KEY_BUFFER_CAPACITY;
		key_count--;
	}
	key_buffer[(key_read + key_count++) % KEY_BUFFER_CAPACITY] = value;
}

static legacy_s16 clamp_mouse(float coordinate, legacy_s16 minimum, legacy_s16 maximum)
{
	if (coordinate < minimum) {
		return minimum;
	}
	if (coordinate > maximum) {
		return maximum;
	}
	return (legacy_s16)coordinate;
}

static void input_mouse_position(float window_x, float window_y)
{
	float x;
	float y;
	sdl3_video_window_to_game(window_x, window_y, &x, &y);
	mouse_x = clamp_mouse(x, mouse_min_x, mouse_max_x);
	mouse_y = clamp_mouse(y, mouse_min_y, mouse_max_y);
}

static void open_joystick(void)
{
	if (sdl3_batch_mode || joystick != NULL) {
		return;
	}
	if (!joystick_initialized) {
		joystick_initialized = SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		if (!joystick_initialized) {
			return;
		}
	}
	int count;
	SDL_JoystickID *ids = SDL_GetJoysticks(&count);
	for (int index = 0; index < count && joystick == NULL; index++) {
		joystick = SDL_OpenJoystick(ids[index]);
	}
	SDL_free(ids);
}

void sdl3_platform_pump(void)
{
	if (sdl3_batch_mode) {
		sdl3_timer_pump();
		return;
	}
	if (pumping) {
		return;
	}
	pumping = true;
	SDL_Event event;
	bool poll_events =
		SDL_GetTicks() != last_event_poll || SDL_HasEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
	while (poll_events && SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT:
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				pumping = false;
				call_exitlist2();
				return;
			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
				input_key(&event.key);
				break;
			case SDL_EVENT_WINDOW_FOCUS_LOST:
				/* A fullscreen transition can change focus while Enter is held.
				 * Keep consumed shortcuts latched until release or a fresh press. */
				memset(keys, 0, sizeof(keys));
				key_count = 0;
				mouse_buttons = 0;
				mouse_transition_count = 0;
				break;
			case SDL_EVENT_WINDOW_EXPOSED:
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				sdl3_video_present();
				break;
			case SDL_EVENT_MOUSE_MOTION:
				input_mouse_position(event.motion.x, event.motion.y);
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP: {
				legacy_s16 flag = event.button.button == SDL_BUTTON_LEFT	 ? 1
								  : event.button.button == SDL_BUTTON_RIGHT	 ? 2
								  : event.button.button == SDL_BUTTON_MIDDLE ? 4
																			 : 0;
				if (event.button.down) {
					mouse_buttons |= flag;
				} else {
					mouse_buttons &= ~flag;
				}
				input_mouse_position(event.button.x, event.button.y);
				if (flag != 0) {
					if (mouse_transition_count == KEY_BUFFER_CAPACITY) {
						mouse_transition_read = (mouse_transition_read + 1U) % KEY_BUFFER_CAPACITY;
						mouse_transition_count--;
					}
					struct MOUSE_TRANSITION *transition =
						&mouse_transitions[(mouse_transition_read + mouse_transition_count++) %
										   KEY_BUFFER_CAPACITY];
					transition->buttons = mouse_buttons;
					transition->x = mouse_x;
					transition->y = mouse_y;
				}
				break;
			}
			case SDL_EVENT_JOYSTICK_REMOVED:
				if (joystick != NULL && event.jdevice.which == SDL_GetJoystickID(joystick)) {
					SDL_CloseJoystick(joystick);
					joystick = NULL;
					open_joystick();
				}
				break;
			case SDL_EVENT_JOYSTICK_ADDED:
				if (joystick_enabled != 0) {
					open_joystick();
				}
				break;
			default:
				break;
		}
	}
	if (poll_events) {
		last_event_poll = SDL_GetTicks();
	}
	sdl3_timer_pump();
	sdl3_video_refresh();
	pumping = false;
}

void sdl3_input_shutdown(void)
{
	SDL_CloseJoystick(joystick);
	joystick = NULL;
	joystick_initialized = false;
	memset(keys, 0, sizeof(keys));
	memset(consumed_keys, 0, sizeof(consumed_keys));
	key_count = 0;
	mouse_available = false;
	mouse_transition_count = 0;
	mouse_transition_read = 0;
}

void sdl3_platform_shutdown(void)
{
	dos_timer_shutdown();
	dos_audio_shutdown();
	sdl3_input_shutdown();
	sdl3_video_shutdown();
	SDL_Quit();
}

void kb_init_interrupt(void)
{
	memset(keys, 0, sizeof(keys));
	memset(consumed_keys, 0, sizeof(consumed_keys));
	key_count = 0;
	key_read = 0;
}

void kb_exit_handler(void)
{
	memset(keys, 0, sizeof(keys));
	memset(consumed_keys, 0, sizeof(consumed_keys));
	key_count = 0;
}

legacy_s16 kb_get_key_state(legacy_s16 key)
{
	sdl3_platform_pump();
	if (key <= 0 || (unsigned int)key >= SDL_arraysize(scancodes)) {
		return 0;
	}
	if (keys[scancodes[key]] && !consumed_keys[scancodes[key]]) {
		return 1;
	}
	/* Distinct SDL keys share the original XT scancode (keypad, right modifiers). */
	for (unsigned int code = 1; code < SDL_SCANCODE_COUNT; code++) {
		if (keys[code] && !consumed_keys[code] &&
			legacy_scancode((SDL_Scancode)code) == (unsigned int)key) {
			return 1;
		}
	}
	return 0;
}

legacy_s16 kb_read_char(void)
{
	sdl3_platform_pump();
	if (key_count == 0) {
		return 0;
	}
	legacy_u16 result = key_buffer[key_read];
	key_read = (key_read + 1U) % KEY_BUFFER_CAPACITY;
	key_count--;
	return LEGACY_S16_FROM_BITS(result);
}

legacy_s16 dos_kb_get_char(void)
{
	return kb_parse_key(kb_read_char());
}

legacy_s16 kb_call_readchar_callback(void)
{
	return kb_read_char();
}

legacy_s16 kb_checking(void)
{
	sdl3_platform_pump();
	return key_count != 0 ? LEGACY_S16_FROM_BITS(key_buffer[key_read]) : 0;
}

legacy_s16 kb_check(void)
{
	sdl3_platform_pump();
	key_count = 0;
	return 0;
}

void flush_stdin(void)
{
	while (kb_read_char() == 0) {
		SDL_Delay(1);
	}
}

void dos_kb_set_numlock(void)
{
	/* SDL keypad events are explicitly mapped to the game's navigation keys. */
}

void dos_kb_clear_numlock(void)
{
}

legacy_s16 dos_mouse_init(legacy_s16 width, legacy_s16 height)
{
	mouse_available = sdl3_video_window() != NULL;
	dos_mouse_set_minmax(0, 0, width - 1, height - 1);
	dos_mouse_set_position(width / 2, height / 2);
	return mouse_available ? -1 : 0;
}

void dos_mouse_set_minmax(legacy_s16 minimum_x, legacy_s16 minimum_y, legacy_s16 maximum_x,
						  legacy_s16 maximum_y)
{
	mouse_min_x = minimum_x;
	mouse_min_y = minimum_y;
	mouse_max_x = maximum_x > 319 ? 319 : maximum_x;
	mouse_max_y = maximum_y > 199 ? 199 : maximum_y;
	mouse_x = clamp_mouse(mouse_x, mouse_min_x, mouse_max_x);
	mouse_y = clamp_mouse(mouse_y, mouse_min_y, mouse_max_y);
}

void dos_mouse_set_position(legacy_s16 x, legacy_s16 y)
{
	mouse_x = clamp_mouse(x, mouse_min_x, mouse_max_x);
	mouse_y = clamp_mouse(y, mouse_min_y, mouse_max_y);
	if (sdl3_video_window() != NULL) {
		float window_x;
		float window_y;
		sdl3_video_game_to_window(mouse_x, mouse_y, &window_x, &window_y);
		SDL_WarpMouseInWindow(sdl3_video_window(), window_x, window_y);
	}
}

void dos_mouse_get_state(legacy_s16 *buttons, legacy_s16 *x, legacy_s16 *y)
{
	sdl3_platform_pump();
	/* A press and release can arrive between game polls. Preserve both edges,
	 * including their click coordinates, instead of losing the complete click. */
	if (mouse_transition_count != 0) {
		const struct MOUSE_TRANSITION *transition = &mouse_transitions[mouse_transition_read];
		*buttons = transition->buttons;
		*x = clamp_mouse(transition->x, mouse_min_x, mouse_max_x);
		*y = clamp_mouse(transition->y, mouse_min_y, mouse_max_y);
		mouse_transition_read = (mouse_transition_read + 1U) % KEY_BUFFER_CAPACITY;
		mouse_transition_count--;
	} else {
		*buttons = mouse_buttons;
		*x = mouse_x;
		*y = mouse_y;
	}
}

legacy_u16 dos_mouse_get_button_count(void)
{
	return mouse_available ? 3 : 0;
}

void dos_joystick_reset_calibration(void)
{
	/* SDL supplies calibrated signed axes; their centre and range are stable. */
	dos_joystick_set_enabled(1);
}

void dos_joystick_set_enabled(legacy_u8 enabled)
{
	joystick_enabled = enabled;
	if (enabled != 0) {
		open_joystick();
	}
}

legacy_u8 dos_joystick_is_enabled(void)
{
	return joystick_enabled;
}

legacy_s16 dos_joystick_get_scaled_axis(legacy_u16 axis_index)
{
	sdl3_platform_pump();
	if (joystick_enabled == 0 || joystick == NULL || axis_index >= 2) {
		return 0;
	}
	/* The game's analogue steering expects approximately -31 .. +32. */
	return (legacy_s16)(((legacy_s32)SDL_GetJoystickAxis(joystick, axis_index) * 32) / 32768);
}

legacy_s16 dos_get_joy_flags(void)
{
	sdl3_platform_pump();
	if (joystick_enabled == 0 || joystick == NULL) {
		return 0;
	}
	legacy_s16 flags = 0;
	Sint16 x = SDL_GetJoystickAxis(joystick, 0);
	Sint16 y = SDL_GetJoystickAxis(joystick, 1);
	if (x < -16384) {
		flags |= 8;
	} else if (x >= 16384) {
		flags |= 4;
	}
	if (y < -16384) {
		flags |= 1;
	} else if (y >= 16384) {
		flags |= 2;
	}
	if (SDL_GetNumJoystickHats(joystick) > 0) {
		Uint8 hat = SDL_GetJoystickHat(joystick, 0);
		if (hat & SDL_HAT_UP) {
			flags |= 1;
		}
		if (hat & SDL_HAT_DOWN) {
			flags |= 2;
		}
		if (hat & SDL_HAT_LEFT) {
			flags |= 8;
		}
		if (hat & SDL_HAT_RIGHT) {
			flags |= 4;
		}
	}
	if (SDL_GetJoystickButton(joystick, 0)) {
		flags |= 16;
	}
	if (SDL_GetJoystickButton(joystick, 1)) {
		flags |= 32;
	}
	return flags;
}
