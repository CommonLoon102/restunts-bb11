#include "sdl3.h"
#include "../../c/platform.h"
#include "../../c/keyboard.h"
#include "../../c/game_input.h"
#include "../../c/fatal.h"
#include "../../c/hires.h"
#ifndef __DJGPP__
#include "../../c/render_vulkan.h"
#endif
#include <string.h>

#define KEY_BUFFER_CAPACITY 64U
#define KEY_WAIT_POLL_MS 1U
#define DOS_KB_PRIMARY_KEYMAP_SIZE 91U
#define DOS_KB_ALT_KEYMAP_SIZE 92U
#define DOS_KB_EXTENDED_KEY_FLAG LEGACY_U8_SIGN_BIT
#define DOS_KB_SCANCODE_MASK LEGACY_S8_MAX
#define DOS_KB_EXTENDED_KEY_NORMALIZE_MIN 133U
#define MOUSE_SCREEN_WIDTH 320
#define MOUSE_SCREEN_HEIGHT 200
#define MOUSE_LEFT_BUTTON 1
#define MOUSE_RIGHT_BUTTON 2
#define MOUSE_MIDDLE_BUTTON 4
#define MOUSE_BUTTON_COUNT 3
#define JOYSTICK_AXIS_X 0
#define JOYSTICK_AXIS_Y 1
#define JOYSTICK_AXIS_COUNT 2U
#define JOYSTICK_PRIMARY_HAT 0
#define JOYSTICK_PRIMARY_BUTTON 0
#define JOYSTICK_SECONDARY_BUTTON 1
#define JOYSTICK_GAME_AXIS_SCALE 32
#define JOYSTICK_DIRECTION_THRESHOLD 16384

/* XT make codes used by the legacy key-state API, distinct from SDL scancodes
 * and the BIOS key words in keyboard.h. */
enum DOS_SCANCODE {
	DOS_SCANCODE_NONE = 0,
	DOS_SCANCODE_ENTER = 28,
	DOS_SCANCODE_CONTROL = 29,
	DOS_SCANCODE_ALT = 56,
	DOS_SCANCODE_HOME = 71,
	DOS_SCANCODE_UP = 72,
	DOS_SCANCODE_PAGE_UP = 73,
	DOS_SCANCODE_LEFT = 75,
	DOS_SCANCODE_RIGHT = 77,
	DOS_SCANCODE_END = 79,
	DOS_SCANCODE_DOWN = 80,
	DOS_SCANCODE_PAGE_DOWN = 81,
	DOS_SCANCODE_INSERT = 82,
	DOS_SCANCODE_DELETE = 83
};

static legacy_u8 keys[SDL_SCANCODE_COUNT];
static legacy_u8 consumed_keys[SDL_SCANCODE_COUNT];
static legacy_u16 key_buffer[KEY_BUFFER_CAPACITY];
static legacy_u32 key_read;
static legacy_u32 key_count;
static legacy_s16 mouse_x;
static legacy_s16 mouse_y;
static legacy_s16 mouse_buttons;
static legacy_s16 mouse_min_x;
static legacy_s16 mouse_min_y;
static legacy_s16 mouse_max_x = MOUSE_SCREEN_WIDTH - 1;
static legacy_s16 mouse_max_y = MOUSE_SCREEN_HEIGHT - 1;
static legacy_u8 mouse_available;
struct MOUSE_TRANSITION {
	legacy_s16 buttons;
	legacy_s16 x;
	legacy_s16 y;
};
static struct MOUSE_TRANSITION mouse_transitions[KEY_BUFFER_CAPACITY];
static legacy_u32 mouse_transition_read;
static legacy_u32 mouse_transition_count;
static legacy_u8 joystick_initialized;
static legacy_u8 joystick_enabled;
static SDL_Joystick *joystick;
static legacy_u8 pumping;
static legacy_u64 last_event_poll;

/* Original DOS translation data, indexed by XT make code. Values below the
 * high-bit flag are character bytes; flagged values encode BIOS scan bytes.
 * The normalization threshold intentionally preserves high BIOS scans used by
 * Alt+F9 and Alt+F10. Modifier precedence and historical table quirks matter. */
static const legacy_u8 dos_kb_keymap_plain[DOS_KB_PRIMARY_KEYMAP_SIZE] = {
	0,	 27,  49,  50,	51,	 52,  53,  54,	55,	 56,  57,  48,	45,	 61,  8,   9,	113, 119, 101,
	114, 116, 121, 117, 105, 111, 112, 91,	93,	 13,  0,   97,	115, 100, 102, 103, 104, 106, 107,
	108, 59,  39,  96,	0,	 92,  122, 120, 99,	 118, 98,  110, 109, 44,  46,  47,	0,	 42,  0,
	32,	 0,	  187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 0,	 0,	  199, 200, 201, 45,  203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	  0};

static const legacy_u8 dos_kb_keymap_shift[DOS_KB_PRIMARY_KEYMAP_SIZE] = {
	0,	 27,  33,  64,	35,	 36,  37,  94,	38,	 42,  40,  41,	95, 43, 8,	 143, 81,  87, 69,
	82,	 84,  89,  85,	73,	 79,  80,  123, 125, 13,  0,   65,	83, 68, 70,	 71,  72,  74, 75,
	76,	 58,  34,  126, 0,	 124, 90,  88,	67,	 86,  66,  78,	77, 60, 62,	 63,  0,   0,  0,
	32,	 0,	  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 0,	0,	199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	0,	0};

static const legacy_u8 dos_kb_keymap_caps[DOS_KB_PRIMARY_KEYMAP_SIZE] = {
	0,	 27,  49,  50,	51,	 52,  53,  54,	55,	 56,  57,  48,	45, 61, 8,	 143, 81,  87, 69,
	82,	 84,  89,  85,	73,	 79,  80,  91,	93,	 13,  0,   65,	83, 68, 70,	 71,  72,  74, 75,
	76,	 59,  39,  96,	0,	 92,  90,  88,	67,	 86,  66,  78,	77, 44, 46,	 47,  0,   0,  0,
	32,	 0,	  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 0,	0,	199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	0,	0};

static const legacy_u8 dos_kb_keymap_control[DOS_KB_PRIMARY_KEYMAP_SIZE] = {
	0,	 27,  33,  0,	35,	 36,  37,  30,	38,	 42,  40,  41,	31,	 43, 127, 9,   17,	23, 5,
	18,	 20,  25,  21,	9,	 15,  16,  27,	29,	 13,  0,   1,	19,	 4,	 6,	  7,   8,	10, 11,
	12,	 59,  44,  96,	0,	 28,  26,  24,	3,	 22,  2,   14,	178, 60, 62,  63,  0,	0,	0,
	32,	 0,	  222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 0,	 0,	 199, 200, 201, 45, 203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	 0};

static const legacy_u8 dos_kb_keymap_alt[DOS_KB_ALT_KEYMAP_SIZE] = {
	0,	 27,  33,  64,	35,	 36,  37,  94,	38,	 42,  40,  41,	95,	 43,  8,   143, 144, 145, 146,
	147, 148, 149, 150, 151, 152, 153, 123, 125, 13,  0,   158, 159, 160, 161, 162, 163, 164, 165,
	166, 58,  34,  126, 0,	 124, 172, 173, 174, 175, 176, 177, 178, 60,  62,  63,	0,	 0,	  0,
	32,	 0,	  248, 249, 250, 251, 252, 253, 254, 255, 128, 129, 0,	 0,	  199, 200, 201, 45,  203,
	204, 205, 43,  207, 208, 209, 210, 211, 0,	 0,	  0,   0,	0,	 0,	  0,   0};

static const legacy_u32 scancodes[] = {SDL_SCANCODE_UNKNOWN,
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

static legacy_u32 legacy_scancode(legacy_u32 code)
{
	switch (code) {
		case SDL_SCANCODE_RCTRL:
			return DOS_SCANCODE_CONTROL;
		case SDL_SCANCODE_RALT:
			return DOS_SCANCODE_ALT;
		case SDL_SCANCODE_KP_ENTER:
			return DOS_SCANCODE_ENTER;
		case SDL_SCANCODE_KP_7:
			return DOS_SCANCODE_HOME;
		case SDL_SCANCODE_KP_8:
			return DOS_SCANCODE_UP;
		case SDL_SCANCODE_KP_9:
			return DOS_SCANCODE_PAGE_UP;
		case SDL_SCANCODE_KP_4:
			return DOS_SCANCODE_LEFT;
		case SDL_SCANCODE_KP_6:
			return DOS_SCANCODE_RIGHT;
		case SDL_SCANCODE_KP_1:
			return DOS_SCANCODE_END;
		case SDL_SCANCODE_KP_2:
			return DOS_SCANCODE_DOWN;
		case SDL_SCANCODE_KP_3:
			return DOS_SCANCODE_PAGE_DOWN;
		case SDL_SCANCODE_KP_0:
			return DOS_SCANCODE_INSERT;
		case SDL_SCANCODE_KP_PERIOD:
			return DOS_SCANCODE_DELETE;
		default:
			break;
	}
	for (legacy_u32 index = 1; index < SDL_arraysize(scancodes); index++) {
		if (scancodes[index] == code) {
			return index;
		}
	}
	return DOS_SCANCODE_NONE;
}

static void input_key(const SDL_KeyboardEvent *event)
{
	if ((legacy_u32)event->scancode >= SDL_SCANCODE_COUNT) {
		return;
	}
	legacy_u8 was_pressed = keys[event->scancode];
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
	legacy_u32 scan = legacy_scancode(event->scancode);
	if (scan == DOS_SCANCODE_NONE) {
		return;
	}
#ifndef __DJGPP__
	/* F10 selects a renderer or an editor category once per physical press. */
	if (event->scancode == SDL_SCANCODE_F10 && (was_pressed || event->repeat)) {
		return;
	}
#endif
	legacy_u16 value;
	if (event->scancode == SDL_SCANCODE_F11 || event->scancode == SDL_SCANCODE_F12) {
		if (was_pressed || event->repeat || (event->mod & (SDL_KMOD_CTRL | SDL_KMOD_ALT)) ||
			(event->scancode == SDL_SCANCODE_F11 && (event->mod & SDL_KMOD_SHIFT))) {
			return;
		}
		value = (legacy_u16)(event->scancode == SDL_SCANCODE_F11 ? KEY_F11
							 : (event->mod & SDL_KMOD_SHIFT)	 ? KEY_SHIFT_F12
																 : KEY_F12);
	} else {
		if ((event->mod & SDL_KMOD_ALT) != 0) {
			value = dos_kb_keymap_alt[scan];
		} else if ((event->mod & SDL_KMOD_CTRL) != 0) {
			value = dos_kb_keymap_control[scan];
		} else if ((event->mod & SDL_KMOD_SHIFT) != 0) {
			value = dos_kb_keymap_shift[scan];
		} else if ((event->mod & SDL_KMOD_CAPS) != 0) {
			value = dos_kb_keymap_caps[scan];
		} else {
			value = dos_kb_keymap_plain[scan];
		}
		if ((value & DOS_KB_EXTENDED_KEY_FLAG) != 0) {
			if (value >= DOS_KB_EXTENDED_KEY_NORMALIZE_MIN) {
				value &= DOS_KB_SCANCODE_MASK;
			}
			value <<= LEGACY_BYTE_BITS;
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

static legacy_s16 clamp_mouse(legacy_f32 coordinate, legacy_s16 minimum, legacy_s16 maximum)
{
	if (coordinate < minimum) {
		return minimum;
	}
	if (coordinate > maximum) {
		return maximum;
	}
	return (legacy_s16)coordinate;
}

static void input_mouse_position(legacy_f32 window_x, legacy_f32 window_y)
{
	legacy_f32 x;
	legacy_f32 y;
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
	/* SDL writes a native int through this output pointer. */
	int count;
	SDL_JoystickID *ids = SDL_GetJoysticks(&count);
	for (legacy_s32 index = 0; index < count && joystick == NULL; index++) {
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
	legacy_u8 poll_events =
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
				legacy_s16 flag = event.button.button == SDL_BUTTON_LEFT	 ? MOUSE_LEFT_BUTTON
								  : event.button.button == SDL_BUTTON_RIGHT	 ? MOUSE_RIGHT_BUTTON
								  : event.button.button == SDL_BUTTON_MIDDLE ? MOUSE_MIDDLE_BUTTON
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
	hires_shutdown();

#ifndef __DJGPP__
	render_vulkan_shutdown();
#endif
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
	if (key <= DOS_SCANCODE_NONE || (legacy_u32)key >= SDL_arraysize(scancodes)) {
		return 0;
	}
	if (keys[scancodes[key]] && !consumed_keys[scancodes[key]]) {
		return 1;
	}
	/* Distinct SDL keys share the original XT scancode (keypad, right modifiers). */
	for (legacy_u32 code = 1; code < SDL_SCANCODE_COUNT; code++) {
		if (keys[code] && !consumed_keys[code] && legacy_scancode(code) == (legacy_u32)key) {
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
		SDL_Delay(KEY_WAIT_POLL_MS);
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
	mouse_max_x = maximum_x >= MOUSE_SCREEN_WIDTH ? MOUSE_SCREEN_WIDTH - 1 : maximum_x;
	mouse_max_y = maximum_y >= MOUSE_SCREEN_HEIGHT ? MOUSE_SCREEN_HEIGHT - 1 : maximum_y;
	mouse_x = clamp_mouse(mouse_x, mouse_min_x, mouse_max_x);
	mouse_y = clamp_mouse(mouse_y, mouse_min_y, mouse_max_y);
}

void dos_mouse_set_position(legacy_s16 x, legacy_s16 y)
{
	mouse_x = clamp_mouse(x, mouse_min_x, mouse_max_x);
	mouse_y = clamp_mouse(y, mouse_min_y, mouse_max_y);
	if (sdl3_video_window() != NULL) {
		legacy_f32 window_x;
		legacy_f32 window_y;
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
	return mouse_available ? MOUSE_BUTTON_COUNT : 0;
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
	if (joystick_enabled == 0 || joystick == NULL || axis_index >= JOYSTICK_AXIS_COUNT) {
		return 0;
	}
	/* Preserve the game's analogue range of -32 .. +31 and signed truncation. */
	return (legacy_s16)(((legacy_s32)SDL_GetJoystickAxis(joystick, axis_index) *
						 JOYSTICK_GAME_AXIS_SCALE) /
						(-SDL_JOYSTICK_AXIS_MIN));
}

legacy_s16 dos_get_joy_flags(void)
{
	sdl3_platform_pump();
	if (joystick_enabled == 0 || joystick == NULL) {
		return 0;
	}
	legacy_s16 flags = 0;
	legacy_s16 x = SDL_GetJoystickAxis(joystick, JOYSTICK_AXIS_X);
	legacy_s16 y = SDL_GetJoystickAxis(joystick, JOYSTICK_AXIS_Y);
	if (x < -JOYSTICK_DIRECTION_THRESHOLD) {
		flags |= INPUT_STEER_LEFT_FLAG;
	} else if (x >= JOYSTICK_DIRECTION_THRESHOLD) {
		flags |= INPUT_STEER_RIGHT_FLAG;
	}
	if (y < -JOYSTICK_DIRECTION_THRESHOLD) {
		flags |= INPUT_ACCELERATE_FLAG;
	} else if (y >= JOYSTICK_DIRECTION_THRESHOLD) {
		flags |= INPUT_BRAKE_FLAG;
	}
	if (SDL_GetNumJoystickHats(joystick) > 0) {
		legacy_u8 hat = SDL_GetJoystickHat(joystick, JOYSTICK_PRIMARY_HAT);
		if (hat & SDL_HAT_UP) {
			flags |= INPUT_ACCELERATE_FLAG;
		}
		if (hat & SDL_HAT_DOWN) {
			flags |= INPUT_BRAKE_FLAG;
		}
		if (hat & SDL_HAT_LEFT) {
			flags |= INPUT_STEER_LEFT_FLAG;
		}
		if (hat & SDL_HAT_RIGHT) {
			flags |= INPUT_STEER_RIGHT_FLAG;
		}
	}
	if (SDL_GetJoystickButton(joystick, JOYSTICK_PRIMARY_BUTTON)) {
		flags |= INPUT_PRIMARY_ACTION_FLAG;
	}
	if (SDL_GetJoystickButton(joystick, JOYSTICK_SECONDARY_BUTTON)) {
		flags |= INPUT_SECONDARY_ACTION_FLAG;
	}
	return flags;
}
