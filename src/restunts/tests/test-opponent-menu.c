#include <string.h>

/* Reuse the car-menu drawing fixture, replacing input and ownership hooks
 * so this test drives the real opponent menu and checks each refresh. */
#define main car_snapshot_main
#define input_checking car_fixture_input_checking
#define mouse_multi_hittest car_fixture_mouse_multi_hittest
#define locate_text_res car_fixture_locate_text_res
#define file_load_resfile car_fixture_file_load_resfile
#define unload_resource car_fixture_unload_resource
#define mmgr_free car_fixture_mmgr_free
#define sprite_make_wnd car_fixture_sprite_make_wnd
#define sprite_free_wnd car_fixture_sprite_free_wnd
#define draw_button car_fixture_draw_button
#define font_draw_text car_fixture_font_draw_text
#include "test-car-menu.c"
#undef main
#undef input_checking
#undef mouse_multi_hittest
#undef locate_text_res
#undef file_load_resfile
#undef unload_resource
#undef mmgr_free
#undef sprite_make_wnd
#undef sprite_free_wnd
#undef draw_button
#undef font_draw_text
#include "../c/ghost.h"
#ifdef RESTUNTS_SDL3
#include "../c/opponent_portrait.h"
#include "../c/shape2d_internal.h"
static legacy_u32 portrait_draws, enhanced_portrait_draws, portrait_unloads;

void opponent_portrait_draw(const struct SPRITE *target, const struct SHAPE2D *original,
							legacy_u8 opponent)
{
	assert(target == &drawing_sprite && original == &fixture_shapes[1]);
	assert(opponent == (legacy_u8)gameconfig.game_opponenttype);
	portrait_draws++;
	if (supersight_enabled != 0 && opponent != 0) {
		enhanced_portrait_draws++;
	}
}

void opponent_portrait_unload(void)
{
	portrait_unloads++;
}
#endif
#undef memcpy

#define OPPONENT_TEST_EVENT_CAPACITY 32U
#define OPPONENT_TEST_RESOURCE_COUNT 3U

static const legacy_u8 previous_opponent[7] = {6, 6, 1, 2, 3, 4, 5};
static const legacy_u8 next_opponent[7] = {1, 2, 3, 4, 5, 6, 1};
static legacy_u16 opponent_keys[OPPONENT_TEST_EVENT_CAPACITY];
static legacy_s16 opponent_hits[OPPONENT_TEST_EVENT_CAPACITY];
static legacy_u8 expected_opponents[OPPONENT_TEST_EVENT_CAPACITY];
static legacy_u8 expected_loads[OPPONENT_TEST_EVENT_CAPACITY];
static legacy_u8 resource_live[OPPONENT_TEST_RESOURCE_COUNT];
static legacy_u32 event_count, event_index, expected_load_count, load_count;
static legacy_u32 resource_allocations, resource_releases, window_allocations, window_releases;
static legacy_u32 case_count, transition_count;
static legacy_u8 window_live;
static legacy_s16 ghost_selected, ghost_selection_result;
static legacy_s8 file_dialog_result;
static legacy_u32 ghost_dialogs, ghost_selections, ghost_button_draws, ghost_descriptions;
static legacy_u32 clock_descriptions, car_menu_calls, error_dialogs;

legacy_s16 ghost_is_selected(void)
{
	return ghost_selected;
}

void ghost_clear(void)
{
	ghost_selected = 0;
}

legacy_s16 ghost_select_replay(const legacy_s8 *directory, const legacy_s8 *name)
{
	assert(directory == replay_directory);
	assert(_strcmp(name, (legacy_s8 *)"GHOST") == 0);
	assert(gameconfig.game_opponenttype == 0);
	ghost_selections++;
	if (ghost_selection_result == 0) {
		ghost_selected = 1;
	}
	return ghost_selection_result;
}

legacy_s8 do_fileselect_dialog(legacy_s8 *directory, legacy_s8 *name, legacy_s8 *extension,
							   legacy_s8 *prompt)
{
	assert(directory == replay_directory);
	assert(_strcmp(extension, (legacy_s8 *)".rpl") == 0);
	assert(_strcmp(prompt, (legacy_s8 *)"Load Replay") == 0);
	assert(gameconfig.game_opponenttype == 0);
	assert(window_live != 0);
	ghost_dialogs++;
	if (file_dialog_result != 0) {
		_strcpy(name, (legacy_s8 *)"GHOST");
	}
	return file_dialog_result;
}

legacy_u16 show_dialog(legacy_s16 dialog_type, legacy_s16 save_background, void *text, legacy_u16 x,
					   legacy_u16 y, legacy_s16 border_color, legacy_s16 *disabled_choices,
					   legacy_s16 initial_choice)
{
	assert(dialog_type == DIALOG_TYPE_ACKNOWLEDGEMENT);
	assert(save_background == DIALOG_SAVE_BACKGROUND);
	assert(_strcmp(text, (legacy_s8 *)"Unable to load ghost replay.]") == 0);
	assert(x == DIALOG_AUTO_POSITION && y == DIALOG_AUTO_POSITION);
	assert(border_color == dialog_border_color);
	assert(disabled_choices == 0 && initial_choice == 0);
	error_dialogs++;
	return 0;
}

void draw_button(legacy_s8 *text, legacy_s16 x, legacy_s16 y, legacy_s16 width, legacy_s16 height,
				 legacy_s16 top_color, legacy_s16 bottom_color, legacy_s16 fill_color,
				 legacy_s16 font_color)
{
	if (x == 21 + 3 * 56) {
		assert(_strcmp(text, gameconfig.game_opponenttype == 0 ? (legacy_s8 *)"Ghost"
															   : opponent_car_button_id) == 0);
		if (gameconfig.game_opponenttype == 0) {
			ghost_button_draws++;
		}
	}
	car_fixture_draw_button(text, x, y, width, height, top_color, bottom_color, fill_color,
							font_color);
}

void font_draw_text(const legacy_s8 *text, legacy_s16 x, legacy_s16 y)
{
	if (gameconfig.game_opponenttype == 0) {
		assert(_strcmp(text, ghost_selected != 0 ? (legacy_s8 *)"Race against a Ghost."
												 : (legacy_s8 *)"Race against the Clock.") == 0);
		if (ghost_selected != 0) {
			ghost_descriptions++;
		} else {
			clock_descriptions++;
		}
	}
	car_fixture_font_draw_text(text, x, y);
}

legacy_s16 input_checking(legacy_s16 elapsed)
{
	(void)elapsed;
	assert(event_index < event_count);
	assert((legacy_u8)gameconfig.game_opponenttype == expected_opponents[event_index]);
	return (legacy_s16)opponent_keys[event_index];
}

legacy_s16 mouse_multi_hittest(legacy_s16 count, const struct BUTTON_AREA *buttons)
{
	assert(count == 5 && buttons == opponentmenu_buttons);
	assert(event_index < event_count);
	return opponent_hits[event_index++];
}

static void *allocate_resource(legacy_u32 index)
{
	assert(index < OPPONENT_TEST_RESOURCE_COUNT && resource_live[index] == 0);
	resource_live[index] = 1;
	resource_allocations++;
	return resource_bytes[index];
}

static void release_resource(void *resource)
{
	for (legacy_u32 index = 0; index < OPPONENT_TEST_RESOURCE_COUNT; index++) {
		if (resource == resource_bytes[index]) {
			assert(resource_live[index] != 0);
			resource_live[index] = 0;
			resource_releases++;
			return;
		}
	}
	assert(!"Opponent menu released an unknown resource");
}

void *file_load_resfile(const legacy_s8 *filename)
{
	if (_strcmp(filename, opponent_misc_resource_name) == 0) {
		return allocate_resource(0);
	}
	/* A wrap failure used to request opp/ rather than the sixth opponent. */
	if (filename[0] != 'o' || filename[1] != 'p' || filename[2] != 'p' || filename[3] < '1' ||
		filename[3] > '6' || filename[4] != 0) {
		fprintf(stderr, "Invalid opponent resource requested: %s\n", (const char *)filename);
		assert(!"Opponent resource must be opp1 through opp6");
	}
	assert(load_count < expected_load_count);
	assert((legacy_u8)(filename[3] - '0') == expected_loads[load_count++]);
	assert(filename[3] - '0' == gameconfig.game_opponenttype);
	return allocate_resource(2);
}

void *file_load_resource(legacy_s16 type, const legacy_s8 *name)
{
	assert(type == FILE_RESOURCE_SHAPE2D_ALTERNATE);
	assert(_strcmp(name, opponent_menu_shapes_name) == 0);
	return allocate_resource(1);
}

void unload_resource(void *resource)
{
	release_resource(resource);
}

void *mmgr_free(legacy_s8 *resource)
{
	assert(resource == (legacy_s8 *)resource_bytes[1]);
	release_resource(resource);
	return 0;
}

struct SPRITE *sprite_make_wnd(legacy_u16 width, legacy_u16 height, legacy_u16 color)
{
	assert(width == 320 && height == 200 && color == 15);
	assert(window_live == 0);
	window_live = 1;
	window_allocations++;
	fixture_sprites[0].sprite_bitmapptr = &fixture_shapes[0];
	return &fixture_sprites[0];
}

void sprite_free_wnd(struct SPRITE *window)
{
	assert(window == &fixture_sprites[0] && window_live != 0);
	window_live = 0;
	window_releases++;
}

legacy_s8 *locate_text_res(legacy_s8 *resource, const legacy_s8 *name)
{
	if (_strcmp(name, (legacy_s8 *)"rep") == 0) {
		assert(resource == mainresptr);
		return (legacy_s8 *)"Load Replay";
	}
	if (_strcmp(name, opponent_description_id) == 0) {
		assert(resource == (legacy_s8 *)resource_bytes[2] && resource_live[2] != 0);
		return (legacy_s8 *)"Opponent]";
	}
	assert(resource == (legacy_s8 *)resource_bytes[0] && resource_live[0] != 0);
	return _strcmp(name, opponent_racing_car_label_id) == 0
			   ? (legacy_s8 *)"Race against the Clock.]"
			   : (legacy_s8 *)name;
}

void locate_many_resources(legacy_s8 *resource, const legacy_s8 *names, legacy_s8 **pointers)
{
	assert(resource == (legacy_s8 *)resource_bytes[1]);
	assert(names == opponent_portrait_shape_ids);
	for (legacy_u32 index = 0; index < 7; index++) {
		pointers[index] = (legacy_s8 *)&fixture_shapes[1];
	}
}

void sprite_draw_palette_mapped(struct SHAPE2D *shape)
{
	assert(shape == &fixture_shapes[0] || shape == &fixture_shapes[1]);
	assert((legacy_u8)gameconfig.game_opponenttype <= 6);
}

void check_input(void)
{
}

void show_waiting(void)
{
}

void run_car_menu(legacy_s8 *id, legacy_s8 *material, legacy_s8 *transmission, legacy_u16 opponent)
{
	assert(id == gameconfig.game_opponentcarid);
	assert(material == &gameconfig.game_opponentmaterial);
	assert(transmission == &gameconfig.game_opponenttransmission);
	assert(opponent != 0 && opponent == (legacy_u8)gameconfig.game_opponenttype);
	assert(window_live == 0 && resource_live[2] == 0);
	memcpy(id, "VETT", 4);
	*material = 2;
	*transmission = TRANSMISSION_AUTOMATIC;
	car_menu_calls++;
}

static void expect_load(legacy_u8 opponent)
{
	if (opponent != 0) {
		assert(expected_load_count < OPPONENT_TEST_EVENT_CAPACITY);
		expected_loads[expected_load_count++] = opponent;
	}
}

static void add_event(legacy_u16 key, legacy_u8 opponent)
{
	assert(event_count < OPPONENT_TEST_EVENT_CAPACITY);
	opponent_keys[event_count] = key;
	opponent_hits[event_count] = -1;
	expected_opponents[event_count++] = opponent;
}

static void begin_case(legacy_u8 opponent, legacy_u8 page_flipping)
{
	assert(resource_allocations == resource_releases && window_allocations == window_releases);
	memset(resource_live, 0, sizeof(resource_live));
	memset(&gameconfig, 0, sizeof(gameconfig));
	memcpy(gameconfig.game_playercarid, "COUN", 4);
	gameconfig.game_opponentcarid[0] = -1;
	gameconfig.game_opponenttype = (legacy_s8)opponent;
	gameconfig.game_playermaterial = 3;
	video_uses_page_flipping = page_flipping;
	fontnptr = (legacy_s8 *)resource_bytes[62];
	font_glyph_height = 8;
	ghost_selected = ghost_selection_result = 0;
	file_dialog_result = 1;
	ghost_dialogs = ghost_selections = ghost_button_draws = ghost_descriptions = 0;
	clock_descriptions = car_menu_calls = error_dialogs = 0;
	event_count = event_index = expected_load_count = load_count = 0;
	resource_allocations = resource_releases = window_allocations = window_releases = 0;
	expect_load(opponent);
#ifdef RESTUNTS_SDL3
	supersight_enabled = 0;
	portrait_draws = enhanced_portrait_draws = portrait_unloads = 0;
#endif
}

static void finish_case(legacy_u8 opponent, legacy_u32 refresh_count)
{
	run_opponent_menu();
	assert(event_index == event_count && load_count == expected_load_count);
	assert((legacy_u8)gameconfig.game_opponenttype == opponent);
	assert(resource_allocations == resource_releases && window_allocations == window_releases);
	assert(resource_allocations == expected_load_count + 2 && window_allocations == refresh_count);
	assert(window_live == 0);
	for (legacy_u32 index = 0; index < OPPONENT_TEST_RESOURCE_COUNT; index++) {
		assert(resource_live[index] == 0);
	}
	if (opponent != 0) {
		assert(memcmp(gameconfig.game_opponentcarid, car_menu_calls != 0 ? "VETT" : "COUN", 4) ==
			   0);
		assert(gameconfig.game_opponentmaterial == (car_menu_calls != 0 ? 2 : 0));
		assert(gameconfig.game_opponenttransmission ==
			   (car_menu_calls != 0 ? TRANSMISSION_AUTOMATIC : TRANSMISSION_MANUAL));
	} else {
		assert(gameconfig.game_opponentcarid[0] == -1);
	}
#ifdef RESTUNTS_SDL3
	assert(portrait_unloads == 1);
#endif
	case_count++;
}

static void test_direction(legacy_u8 initial, legacy_u8 direction, legacy_u32 repeats,
						   legacy_u8 page_flipping)
{
	legacy_u8 opponent = initial;
	begin_case(initial, page_flipping);
	if (direction != 0) {
		add_event(KEY_RIGHT, opponent);
	}
	for (legacy_u32 index = 0; index < repeats; index++) {
		add_event(KEY_ENTER, opponent);
		opponent = direction != 0 ? next_opponent[opponent] : previous_opponent[opponent];
		expect_load(opponent);
		transition_count++;
	}
	/* Navigate from Last/Next to Done, keeping hover disabled throughout. */
	add_event(KEY_LEFT, opponent);
	if (direction != 0) {
		add_event(KEY_LEFT, opponent);
	}
	add_event(KEY_ENTER, opponent);
	finish_case(opponent, repeats + 1);
}

static void test_return_to_clock(legacy_u8 page_flipping)
{
	begin_case(6, page_flipping);
	add_event(KEY_RIGHT, 6);
	add_event(KEY_RIGHT, 6);
	add_event(KEY_ENTER, 6); /* None unloads the existing opponent. */
	add_event(KEY_LEFT, 0);
	add_event(KEY_LEFT, 0);
	add_event(KEY_ENTER, 0); /* Last after None must select the sixth opponent. */
	expect_load(6);
	add_event(KEY_RIGHT, 6);
	add_event(KEY_RIGHT, 6);
	add_event(KEY_ENTER, 6);
	add_event(KEY_RIGHT, 0); /* Clock -> Ghost -> Done. */
	add_event(KEY_RIGHT, 0);
	add_event(KEY_ENTER, 0);
	finish_case(0, 4);
	transition_count += 3;
}

static void test_ghost_selection(legacy_u8 page_flipping, legacy_u8 already_selected,
								 legacy_u8 use_mouse, legacy_u8 result)
{
	begin_case(0, page_flipping);
	ghost_selected = already_selected;
	file_dialog_result = result != 1;
	ghost_selection_result = result == 2;
	if (use_mouse != 0) {
		add_event(KEY_ENTER, 0);
		opponent_hits[event_count - 1] = 3;
	} else {
		add_event(KEY_LEFT, 0); /* Last -> Done -> Ghost. */
		add_event(KEY_LEFT, 0);
		add_event(KEY_ENTER, 0);
	}
	add_event(KEY_RIGHT, 0);
	add_event(KEY_ENTER, 0);
	finish_case(0, already_selected == 0 && result == 0 ? 2 : 1);
	assert(ghost_dialogs == 1 && ghost_selections == (result != 1));
	assert(error_dialogs == (result == 2));
	assert(ghost_selected == (already_selected != 0 || result == 0));
	assert(ghost_descriptions == (already_selected != 0 || result == 0));
	assert(clock_descriptions == (already_selected == 0));
	assert(ghost_button_draws == window_allocations);
	assert(car_menu_calls == 0);
	assert(memcmp(gameconfig.game_playercarid, "COUN", 4) == 0);
	assert(gameconfig.game_playermaterial == 3);
}

static void test_clear_ghost(legacy_u8 page_flipping, legacy_u8 selection)
{
	begin_case(0, page_flipping);
	ghost_selected = 1;
	add_event(KEY_ENTER, 0);
	opponent_hits[event_count - 1] = selection;
	legacy_u8 opponent = selection == 0 ? 6 : (selection == 1 ? 1 : 0);
	expect_load(opponent);
	add_event(KEY_ENTER, opponent);
	opponent_hits[event_count - 1] = 4;
	finish_case(opponent, 2);
	assert(ghost_selected == 0 && ghost_descriptions == 1);
	assert(clock_descriptions == (selection == 2));
	assert(ghost_dialogs == 0);
}

static void test_opponent_car(legacy_u8 page_flipping)
{
	begin_case(3, page_flipping);
	add_event(KEY_ENTER, 3);
	opponent_hits[event_count - 1] = 3;
	expect_load(3);
	add_event(KEY_RIGHT, 3);
	add_event(KEY_ENTER, 3);
	finish_case(3, 2);
	assert(car_menu_calls == 1 && ghost_dialogs == 0 && ghost_button_draws == 0);
}

#ifdef RESTUNTS_SDL3
static void test_portrait_toggle(legacy_u8 page_flipping, legacy_s16 key, legacy_s32 available)
{
	vulkan_fixture_available = available;
	begin_case(3, page_flipping);
	add_event((legacy_u16)key, 3);
	add_event((legacy_u16)key, 3);
	add_event(KEY_ENTER, 3);
	opponent_hits[event_count - 1] = 4;
	finish_case(3, 1);
	assert(supersight_enabled == 0);
	assert(portrait_draws == (available ? 3U : 1U));
	assert(enhanced_portrait_draws == (available ? 1U : 0U));
	if (available) {
		assert(display_last_renderer_key == key);
	}
	vulkan_fixture_available = 1;
}
#endif

int main(void)
{
	for (legacy_u8 page_flipping = 0; page_flipping < 2; page_flipping++) {
		for (legacy_u8 initial = 0; initial < 7; initial++) {
			for (legacy_u8 direction = 0; direction < 2; direction++) {
				test_direction(initial, direction, 1, page_flipping);
				test_direction(initial, direction, 19, page_flipping);
			}
		}
		test_return_to_clock(page_flipping);
		for (legacy_u8 already_selected = 0; already_selected < 2; already_selected++) {
			for (legacy_u8 use_mouse = 0; use_mouse < 2; use_mouse++) {
				for (legacy_u8 result = 0; result < 3; result++) {
					test_ghost_selection(page_flipping, already_selected, use_mouse, result);
				}
			}
		}
		for (legacy_u8 selection = 0; selection < 3; selection++) {
			test_clear_ghost(page_flipping, selection);
		}
		test_opponent_car(page_flipping);
#ifdef RESTUNTS_SDL3
		test_portrait_toggle(page_flipping, KEY_F10, 1);
		test_portrait_toggle(page_flipping, KEY_F10, 0);
		test_portrait_toggle(page_flipping, KEY_F12, 1);
		test_portrait_toggle(page_flipping, KEY_SHIFT_F12, 1);
#endif
	}
	printf("test-opponent-menu: passed %" LEGACY_PRIu32 " sessions, %" LEGACY_PRIu32
		   " transitions\n",
		   case_count, transition_count);
	return 0;
}
