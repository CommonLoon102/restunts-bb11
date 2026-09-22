#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../platform/sdl3/sdl3.h"
#include "../c/platform.h"
#include "../c/keyboard.h"
#include "../c/hires.h"

legacy_s32 sdl3_batch_mode;
static legacy_u8 framebuffer[65536];
static legacy_u8 high_resolution_framebuffer[1280 * 800];
static legacy_u8 high_resolution_active;
static legacy_u32 frame_generation;
static legacy_u32 first_callbacks;
static legacy_u32 second_callbacks;
static legacy_u32 audio_ticks;
static legacy_u8 quit_cleaned_up;

const legacy_u8 *hires_framebuffer(const legacy_u8 *legacy, legacy_s32 *width, legacy_s32 *height)
{
	*width = high_resolution_active ? 1280 : 320;
	*height = high_resolution_active ? 800 : 200;
	return high_resolution_active ? high_resolution_framebuffer : legacy;
}

legacy_u32 hires_generation(void)
{
	return frame_generation;
}

legacy_s32 hires_enabled(void)
{
	return high_resolution_active;
}

void hires_forget(const void *base)
{
	assert(base == framebuffer);
}

void hires_shutdown(void)
{
	high_resolution_active = false;
}

void *dos_memory_make_pointer(legacy_u16 segment, legacy_u16 offset)
{
	assert(segment == 0xA000U);
	return framebuffer + offset;
}

void dos_process_exit(legacy_s16 status)
{
	exit(status);
}

legacy_s16 kb_parse_key(legacy_s16 key)
{
	return key;
}

void dos_audio_shutdown(void)
{
}

void call_exitlist2(void)
{
	quit_cleaned_up = true;
}

void sdl3_audio_update(void)
{
	/* A sample block must observe both sequence callbacks for the current tick. */
	assert(first_callbacks == second_callbacks);
	audio_ticks++;
}

static void first_callback(void)
{
	assert(first_callbacks == second_callbacks);
	first_callbacks++;
}

static void second_callback(void)
{
	assert(first_callbacks == second_callbacks + 1U);
	second_callbacks++;
}

static void send_key(SDL_Scancode scan, SDL_Keymod modifiers, legacy_u8 down, legacy_u8 repeat)
{
	SDL_Event event;
	SDL_zero(event);
	event.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
	event.key.scancode = scan;
	event.key.mod = modifiers;
	event.key.down = down;
	event.key.repeat = repeat;
	assert(SDL_PushEvent(&event));
}

static void test_keyboard(void)
{
	kb_init_interrupt();
	send_key(SDL_SCANCODE_Q, SDL_KMOD_NONE, true, false);
	assert(kb_get_key_state(16) == 1);
	assert(dos_kb_get_char() == 'q');
	assert(kb_read_char() == 0);
	send_key(SDL_SCANCODE_Q, SDL_KMOD_NONE, false, false);
	assert(kb_get_key_state(16) == 0);
	send_key(SDL_SCANCODE_KP_8, SDL_KMOD_NUM, true, false);
	assert(kb_get_key_state(72) == 1);
	assert(kb_read_char() == KEY_UP);
	send_key(SDL_SCANCODE_KP_8, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_F1, SDL_KMOD_SHIFT, true, false);
	assert(kb_read_char() == KEY_SHIFT_F1);
	send_key(SDL_SCANCODE_F1, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_F11, SDL_KMOD_NONE, true, false);
	send_key(SDL_SCANCODE_F11, SDL_KMOD_NONE, true, true);
	assert(kb_read_char() == KEY_F11);
	assert(kb_read_char() == 0);
	send_key(SDL_SCANCODE_F11, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_F12, SDL_KMOD_CTRL, true, false);
	assert(kb_read_char() == 0);
	send_key(SDL_SCANCODE_F12, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_F12, SDL_KMOD_NONE, true, false);
	assert(kb_read_char() == KEY_F12);
	SDL_Event event;
	SDL_zero(event);
	event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
	assert(SDL_PushEvent(&event));
	assert(kb_get_key_state(88) == 0);
	assert(kb_get_key_state(-1) == 0);
	assert(kb_get_key_state(500) == 0);
}

static void test_timer(void)
{
	dos_timer_setup_interrupt();
	assert(dos_timer_register_callback(first_callback));
	assert(dos_timer_register_callback(second_callback));
	SDL_Delay(35);
	legacy_u32 current = timer_get_counter();
	assert(current >= 3);
	assert(first_callbacks == current);
	assert(audio_ticks == current);
	dos_timer_set_callbacks_suspended(1);
	legacy_u32 paused = timer_get_counter();
	legacy_u32 realtime = dos_timer_get_realtime_counter();
	legacy_u32 callbacks_at_pause = first_callbacks;
	SDL_Delay(25);
	assert(timer_get_counter() == paused);
	assert(first_callbacks == callbacks_at_pause);
	assert(dos_timer_get_realtime_counter() >= realtime + 2);
	realtime = dos_timer_get_realtime_counter();
	assert(audio_ticks == realtime);
	dos_timer_set_callbacks_suspended(0);
	SDL_Delay(15);
	assert(timer_get_counter() > paused);
	legacy_u32 slow = timer_get_slow_counter();
	realtime = dos_timer_get_realtime_counter();
	assert(slow <= realtime / 5U && slow >= current / 5U);
	dos_timer_unregister_callback(first_callback);
	dos_timer_unregister_callback(second_callback);
	dos_timer_set_callbacks_suspended(1);
	dos_timer_reset_counter();
	assert(timer_get_counter() == 0);
	dos_timer_shutdown();
}

static void assert_coordinate(legacy_f32 actual, legacy_f32 expected)
{
	assert(actual > expected - 0.1f && actual < expected + 0.1f);
}

static void check_video_aspect(legacy_s32 width, legacy_s32 height, legacy_f32 left, legacy_f32 top)
{
	assert(SDL_SetWindowSize(sdl3_video_window(), width, height));
	assert(SDL_SyncWindow(sdl3_video_window()));
	SDL_PumpEvents();
	sdl3_video_present();
	SDL_FRect bounds;
	assert(SDL_GetRenderLogicalPresentationRect(SDL_GetRenderer(sdl3_video_window()), &bounds));
	assert_coordinate(bounds.x, left);
	assert_coordinate(bounds.y, top);
	assert_coordinate(bounds.w, 960.0f);
	assert_coordinate(bounds.h, 720.0f);
	legacy_f32 x;
	legacy_f32 y;
	sdl3_video_game_to_window(0, 0, &x, &y);
	assert_coordinate(x, left);
	assert_coordinate(y, top);
	sdl3_video_game_to_window(320, 200, &x, &y);
	assert_coordinate(x, left + 960.0f);
	assert_coordinate(y, top + 720.0f);
	/* Ten original pixels occupy 30 horizontally and 36 vertically: VGA 6:5. */
	sdl3_video_game_to_window(10, 10, &x, &y);
	assert_coordinate(x, left + 30.0f);
	assert_coordinate(y, top + 36.0f);
}

static void test_video_and_mouse(void)
{
	dos_video_set_mode_13h();
	assert(sdl3_video_window() != NULL);
	legacy_u8 red[] = {63, 0, 0};
	dos_video_set_palette(3, 1, red);
	memset(framebuffer, 3, 64000);
	sdl3_video_present();
	assert(dos_video_enable_planar_pages() == 0);
	check_video_aspect(1100, 720, 70.0f, 0.0f);
	check_video_aspect(960, 800, 0.0f, 40.0f);
	check_video_aspect(960, 720, 0.0f, 0.0f);
	legacy_f32 x;
	legacy_f32 y;
	legacy_f32 window_x;
	legacy_f32 window_y;
	sdl3_video_game_to_window(160, 100, &window_x, &window_y);
	sdl3_video_window_to_game(window_x, window_y, &x, &y);
	assert(x > 159.9f && x < 160.1f);
	assert(y > 99.9f && y < 100.1f);
	assert(dos_mouse_init(320, 200) != 0);
	assert(dos_mouse_get_button_count() >= 2);
	dos_mouse_set_minmax(15, 10, 300, 180);
	dos_mouse_set_position(0, 250);
	legacy_s16 buttons;
	legacy_s16 mx;
	legacy_s16 my;
	dos_mouse_get_state(&buttons, &mx, &my);
	assert(mx == 15 && my == 180);
	SDL_Event event;
	SDL_zero(event);
	event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
	event.button.button = SDL_BUTTON_RIGHT;
	event.button.down = true;
	event.button.x = window_x;
	event.button.y = window_y;
	assert(SDL_PushEvent(&event));
	dos_mouse_get_state(&buttons, &mx, &my);
	assert(buttons == 2 && mx == 160 && my == 100);
	event.type = SDL_EVENT_MOUSE_BUTTON_UP;
	event.button.down = false;
	assert(SDL_PushEvent(&event));
	event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
	event.button.button = SDL_BUTTON_LEFT;
	event.button.down = true;
	assert(SDL_PushEvent(&event));
	event.type = SDL_EVENT_MOUSE_BUTTON_UP;
	event.button.down = false;
	assert(SDL_PushEvent(&event));
	sdl3_platform_pump();
	dos_mouse_get_state(&buttons, &mx, &my);
	assert(buttons == 0);
	dos_mouse_get_state(&buttons, &mx, &my);
	assert(buttons == 1);
	dos_mouse_get_state(&buttons, &mx, &my);
	assert(buttons == 0);
}

static void assert_presented_color(legacy_s32 x, legacy_s32 y, Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_Surface *surface = SDL_RenderReadPixels(SDL_GetRenderer(sdl3_video_window()), NULL);
	assert(surface != NULL);
	Uint8 actual_red;
	Uint8 actual_green;
	Uint8 actual_blue;
	assert(SDL_ReadSurfacePixel(surface, x, y, &actual_red, &actual_green, &actual_blue, NULL));
	assert(actual_red == red && actual_green == green && actual_blue == blue);
	SDL_DestroySurface(surface);
}

static void test_high_resolution_video(void)
{
	legacy_u8 colors[] = {63, 0, 0, 0, 0, 63, 0, 63, 0};
	dos_video_set_palette(3, 3, colors);
	memset(framebuffer, 3, 64000);
	memset(high_resolution_framebuffer, 3, sizeof(high_resolution_framebuffer));
	for (legacy_s32 row = 0; row < 800; row++) {
		high_resolution_framebuffer[row * 1280 + 101] = 4;
	}
	high_resolution_active = true;
	frame_generation++;
	check_video_aspect(1100, 720, 70.0f, 0.0f);
	check_video_aspect(960, 800, 0.0f, 40.0f);
	assert(SDL_SetWindowSize(sdl3_video_window(), 1280, 960));
	assert(SDL_SyncWindow(sdl3_video_window()));
	SDL_PumpEvents();
	sdl3_video_present();
	/* Adjacent output pixels must retain detail smaller than one legacy pixel. */
	assert_presented_color(100, 120, 255, 0, 0);
	assert_presented_color(101, 120, 0, 0, 255);
	assert_presented_color(102, 120, 255, 0, 0);
	for (legacy_s32 row = 0; row < 800; row++) {
		high_resolution_framebuffer[row * 1280 + 101] = 5;
	}
	frame_generation++;
	SDL_Delay(11);
	sdl3_video_refresh();
	assert_presented_color(101, 120, 0, 255, 0);
	sdl3_video_begin_frame();
	for (legacy_s32 row = 0; row < 800; row++) {
		high_resolution_framebuffer[row * 1280 + 102] = 4;
	}
	frame_generation++;
	SDL_Delay(11);
	sdl3_video_refresh();
	assert_presented_color(102, 120, 255, 0, 0);
	sdl3_video_end_frame();
	assert_presented_color(102, 120, 0, 0, 255);
	high_resolution_active = false;
	frame_generation++;
	SDL_Delay(11);
	sdl3_video_refresh();
	assert_presented_color(101, 120, 255, 0, 0);
	assert_presented_color(102, 120, 255, 0, 0);
	check_video_aspect(960, 720, 0.0f, 0.0f);
}

static void check_fullscreen(legacy_u8 expected)
{
	assert(SDL_SyncWindow(sdl3_video_window()));
	sdl3_platform_pump();
	assert(((SDL_GetWindowFlags(sdl3_video_window()) & SDL_WINDOW_FULLSCREEN) != 0) == expected);
	SDL_Renderer *renderer = SDL_GetRenderer(sdl3_video_window());
	/* SDL output pointers require the library's native int type. */
	int width;
	int height;
	assert(SDL_GetRenderOutputSize(renderer, &width, &height));
	legacy_f32 expected_width = (legacy_f32)width;
	legacy_f32 expected_height = expected_width * 3.0f / 4.0f;
	if (expected_height > height) {
		expected_height = (legacy_f32)height;
		expected_width = expected_height * 4.0f / 3.0f;
	}
	SDL_FRect bounds;
	assert(SDL_GetRenderLogicalPresentationRect(renderer, &bounds));
	assert_coordinate(bounds.x, ((legacy_f32)width - expected_width) / 2.0f);
	assert_coordinate(bounds.y, ((legacy_f32)height - expected_height) / 2.0f);
	assert_coordinate(bounds.w, expected_width);
	assert_coordinate(bounds.h, expected_height);
	legacy_f32 x;
	legacy_f32 y;
	sdl3_video_game_to_window(160, 100, &x, &y);
	assert_coordinate(x, width / 2.0f);
	assert_coordinate(y, height / 2.0f);
	sdl3_video_window_to_game(x, y, &x, &y);
	assert_coordinate(x, 160.0f);
	assert_coordinate(y, 100.0f);
}

static void test_fullscreen_shortcut(void)
{
	kb_init_interrupt();
	check_video_aspect(1100, 720, 70.0f, 0.0f);
	/* SDL reads and writes window coordinates through native int pointers. */
	int original_x;
	int original_y;
	assert(SDL_GetWindowPosition(sdl3_video_window(), &original_x, &original_y));
	send_key(SDL_SCANCODE_LALT, SDL_KMOD_LALT, true, false);
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_LALT, true, false);
	assert(kb_read_char() == 0);
	check_fullscreen(true);
	assert(kb_get_key_state(28) == 0);
	assert(kb_get_key_state(56) == 1);
	/* Neither auto-repeat nor duplicate key-down events toggle a held shortcut. */
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_LALT, true, true);
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_LALT, true, false);
	assert(kb_read_char() == 0);
	check_fullscreen(true);
	SDL_Event focus_lost;
	SDL_zero(focus_lost);
	focus_lost.type = SDL_EVENT_WINDOW_FOCUS_LOST;
	assert(SDL_PushEvent(&focus_lost));
	send_key(SDL_SCANCODE_LALT, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_NONE, true, true);
	assert(kb_read_char() == 0);
	assert(kb_get_key_state(28) == 0);
	assert(kb_get_key_state(56) == 0);
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_RALT, SDL_KMOD_RALT, true, false);
	send_key(SDL_SCANCODE_KP_ENTER, SDL_KMOD_RALT, true, false);
	assert(kb_read_char() == 0);
	check_fullscreen(false);
	assert(kb_get_key_state(28) == 0);
	send_key(SDL_SCANCODE_KP_ENTER, SDL_KMOD_NONE, false, false);
	send_key(SDL_SCANCODE_RALT, SDL_KMOD_NONE, false, false);
	assert(kb_read_char() == 0);
	int width;
	int height;
	int restored_x;
	int restored_y;
	assert(SDL_GetWindowSize(sdl3_video_window(), &width, &height));
	assert(width == 1100 && height == 720);
	assert(SDL_GetWindowPosition(sdl3_video_window(), &restored_x, &restored_y));
	assert(restored_x == original_x && restored_y == original_y);
	/* Consuming the shortcut must not suppress the next ordinary Enter. */
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_NONE, true, false);
	assert(kb_read_char() == KEY_ENTER);
	assert(kb_get_key_state(28) == 1);
	send_key(SDL_SCANCODE_RETURN, SDL_KMOD_NONE, false, false);
	assert(kb_get_key_state(28) == 0);
	send_key(SDL_SCANCODE_KP_ENTER, SDL_KMOD_NONE, true, false);
	assert(kb_read_char() == KEY_ENTER);
	send_key(SDL_SCANCODE_KP_ENTER, SDL_KMOD_NONE, false, false);
	assert(kb_get_key_state(28) == 0);
}

static void test_joystick(void)
{
	assert(SDL_InitSubSystem(SDL_INIT_JOYSTICK));
	SDL_VirtualJoystickDesc descriptor;
	SDL_INIT_INTERFACE(&descriptor);
	descriptor.type = SDL_JOYSTICK_TYPE_GAMEPAD;
	descriptor.naxes = 2;
	descriptor.nbuttons = 2;
	descriptor.nhats = 1;
	descriptor.name = "Restunts platform test";
	SDL_JoystickID id = SDL_AttachVirtualJoystick(&descriptor);
	assert(id != 0);
	SDL_Joystick *virtual_joystick = SDL_OpenJoystick(id);
	assert(virtual_joystick != NULL);
	dos_joystick_reset_calibration();
	assert(dos_joystick_is_enabled());
	assert(SDL_SetJoystickVirtualAxis(virtual_joystick, 0, -32768));
	assert(SDL_SetJoystickVirtualAxis(virtual_joystick, 1, 32767));
	assert(SDL_SetJoystickVirtualButton(virtual_joystick, 0, true));
	SDL_UpdateJoysticks();
	assert(dos_get_joy_flags() == (8 | 2 | 16));
	assert(dos_joystick_get_scaled_axis(0) == -32);
	assert(dos_joystick_get_scaled_axis(1) == 31);
	assert(SDL_SetJoystickVirtualAxis(virtual_joystick, 0, 0));
	assert(SDL_SetJoystickVirtualAxis(virtual_joystick, 1, 0));
	assert(SDL_SetJoystickVirtualButton(virtual_joystick, 0, false));
	assert(SDL_SetJoystickVirtualHat(virtual_joystick, 0, SDL_HAT_RIGHTUP));
	SDL_UpdateJoysticks();
	assert(dos_get_joy_flags() == (4 | 1));
	dos_joystick_set_enabled(0);
	assert(dos_get_joy_flags() == 0);
	assert(dos_joystick_get_scaled_axis(0) == 0);
	SDL_CloseJoystick(virtual_joystick);
	assert(SDL_DetachVirtualJoystick(id));
}

int main(void)
{
	SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
	assert(SDL_Init(SDL_INIT_EVENTS));
	sdl3_batch_mode = 1;
	dos_video_set_mode_13h();
	dos_joystick_set_enabled(1);
	assert(dos_mouse_init(320, 200) == 0);
	sdl3_platform_pump();
	assert(sdl3_video_window() == NULL);
	assert(SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0);
	dos_joystick_set_enabled(0);
	sdl3_batch_mode = 0;
	test_keyboard();
	test_timer();
	test_video_and_mouse();
	test_high_resolution_video();
	test_fullscreen_shortcut();
	test_joystick();
	SDL_Event quit;
	SDL_zero(quit);
	quit.type = SDL_EVENT_QUIT;
	assert(SDL_PushEvent(&quit));
	sdl3_platform_pump();
	assert(quit_cleaned_up);
	sdl3_platform_shutdown();
	puts("SDL3 keyboard, timer, video coordinates, mouse and joystick passed.");
	return 0;
}
