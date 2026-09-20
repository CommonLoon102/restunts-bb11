#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../c/ghost.h"
#include "../c/externs.h"
#include "../c/platform.h"
#include "../c/residue.h"
#include "../c/trackdata_layout.h"

#undef memcpy
#undef memset
#undef strcpy
#undef strlen

#define TEST_FILE_COUNT 12U
#define TEST_RECORDING_FRAMES 8U

struct GAMEINFO gameconfig;
struct GAMESTATE state;
struct SIMD simd_player;
struct LEGACY_EXECUTION_RESIDUE legacy_execution_residue;
legacy_s16 legacy_render_player_headings_active;
legacy_u16 framespersec;
legacy_u8 game_replay_mode;
legacy_s8 is_in_replay;
legacy_s8 replay_recording_flags;
legacy_u16 elapsed_time1;
legacy_u16 elapsed_time2;
legacy_s16 checkpoint_frame_interval;
legacy_s16 timer_ticks_per_frame;
legacy_s8 *steerWhlRespTable_ptr;
legacy_u8 frame_callback_countdown;
legacy_u8 slow_replay_countdown;
legacy_u8 race_exit_request;
legacy_u8 race_start_sequence_state;
legacy_s16 start_flag_animation;
static legacy_u8 test_track[REPLAY_TRACK_SIZE];
static legacy_s8 test_inputs[TRACKDATA_REPLAY_INPUT_BUFFER_SIZE];
legacy_u8 *track_element_map = test_track;
legacy_s8 *replay_input_buffer = test_inputs;
static legacy_s8 random_seed[GAMESTATE_RANDOM_SEED_SIZE];
static void (*cleanup_handler)(void);
static int simulation_loads;
static int simulation_steps;
static int simulation_frees;
static int fail_write;
static int fail_read;

struct TEST_FILE {
	legacy_s8 name[REPLAY_FILENAME_SIZE];
	FILE *stream;
	legacy_s16 exists;
};

static struct TEST_FILE files[TEST_FILE_COUNT];

legacy_u16 dos_file_open(const legacy_s8 *name, legacy_s16 create)
{
	for (legacy_u16 index = 1; index < TEST_FILE_COUNT; index++) {
		if (files[index].exists && strcmp((char *)files[index].name, (const char *)name) == 0) {
			assert(create == DOS_FILE_OPEN_EXISTING);
			rewind(files[index].stream);
			return index;
		}
	}
	if (create == DOS_FILE_OPEN_EXISTING) {
		return 0;
	}
	for (legacy_u16 index = 1; index < TEST_FILE_COUNT; index++) {
		if (!files[index].exists) {
			strcpy((char *)files[index].name, (const char *)name);
			files[index].stream = tmpfile();
			assert(files[index].stream != 0);
			files[index].exists = 1;
			return index;
		}
	}
	return 0;
}

legacy_s16 dos_file_close(legacy_u16 file)
{
	assert(file > 0 && file < TEST_FILE_COUNT && files[file].exists);
	return 0;
}

legacy_s16 dos_file_remove(const legacy_s8 *name)
{
	for (legacy_u16 index = 1; index < TEST_FILE_COUNT; index++) {
		if (files[index].exists && strcmp((char *)files[index].name, (const char *)name) == 0) {
			fclose(files[index].stream);
			files[index].exists = 0;
			return 0;
		}
	}
	return -1;
}

legacy_u16 dos_file_read(legacy_u16 file, void *destination, legacy_u16 length)
{
	if (fail_read) {
		return 0;
	}
	return (legacy_u16)fread(destination, 1, length, files[file].stream);
}

legacy_u16 dos_file_write(legacy_u16 file, const void *source, legacy_u16 length)
{
	if (fail_write) {
		return 0;
	}
	return (legacy_u16)fwrite(source, 1, length, files[file].stream);
}

legacy_s16 dos_file_seek(legacy_u16 file, legacy_s32 offset, legacy_s16 origin)
{
	return (legacy_s16)fseek(files[file].stream, offset, origin);
}

legacy_s16 dos_file_error(void)
{
	return 0;
}

const legacy_s8 *dos_file_find_first(const legacy_s8 *name)
{
	for (legacy_u16 index = 1; index < TEST_FILE_COUNT; index++) {
		if (files[index].exists && strcmp((char *)files[index].name, (const char *)name) == 0) {
			return files[index].name;
		}
	}
	return 0;
}

void file_build_path(const legacy_s8 *directory, const legacy_s8 *name, const legacy_s8 *extension,
					 legacy_s8 *destination)
{
	assert(directory == 0 || directory[0] == 0);
	sprintf((char *)destination, "%s%s", name, extension);
}

void add_exit_handler(void (*handler)(void))
{
	cleanup_handler = handler;
}

legacy_s16 track_setup(void)
{
	/* Exercise the canonicalization performed by the real track compiler. */
	test_track[182] = 4;
	return 0;
}

void get_kevinrandom_seed(legacy_s8 *seed)
{
	memcpy(seed, random_seed, sizeof(random_seed));
}

void init_kevinrandom(const legacy_s8 *seed)
{
	memcpy(random_seed, seed, sizeof(random_seed));
}

void ghost_load_simulation_resources(void)
{
	assert(gameconfig.game_opponenttype == 3);
	assert(memcmp(gameconfig.game_playercarid, "ANSX", REPLAY_CAR_ID_SIZE) == 0);
	simulation_loads++;
	simd_player.car_height = 47;
}

void ghost_free_simulation_resources(void)
{
	simulation_frees++;
}

void init_game_state(legacy_s16 mode)
{
	assert(mode == GAMESTATE_INIT_RESET_CHECKPOINTS);
	elapsed_time1 = 0;
	elapsed_time2 = 0;
	checkpoint_frame_interval = (legacy_s16)(framespersec * 30);
	timer_ticks_per_frame = (legacy_s16)(100 / framespersec);
	memset(&state, 0, sizeof(state));
}

void update_gamestate_silent(void)
{
	assert(game_replay_mode == REPLAY_MODE_PLAYBACK);
	assert(is_in_replay == 1);
	assert(replay_recording_flags == REPLAY_RECORDING_RESTARTABLE_FLAG);
	assert(gameconfig.game_opponenttype == 3);
	state.playerstate.car_position.lx += replay_input_buffer[state.game_frame];
	state.playerstate.car_position.lz += gameconfig.game_opponenttype;
	state.playerstate.car_steeringAngle = state.game_frame;
	state.playerstate.car_sound_flags = CAR_SOUND_ENGINE_ACTIVE_FLAG;
	state.game_frame++;
	legacy_execution_residue.penalty_route_word++;
	legacy_render_player_headings_active = 1;
	random_seed[0]++;
	simulation_steps++;
}

static void make_replay(const char *name, legacy_u16 fps, legacy_u16 frame_count,
						legacy_s16 truncated)
{
	struct GAMEINFO config = {0};
	memcpy(config.game_playercarid, "ANSX", REPLAY_CAR_ID_SIZE);
	memcpy(config.game_opponentcarid, "P964", REPLAY_CAR_ID_SIZE);
	memcpy(config.game_trackname, "GHOSTTRK", 9);
	config.game_opponenttype = 3;
	config.game_framespersec = fps;
	config.game_recordedframes = frame_count;
	legacy_u8 header[REPLAY_GAMEINFO_SIZE];
	replay_gameinfo_encode(header, &config);
	legacy_u16 file = dos_file_open((const legacy_s8 *)name, DOS_FILE_CREATE);
	dos_file_write(file, header, sizeof(header));
	legacy_u8 track[REPLAY_TRACK_SIZE];
	for (legacy_u16 index = 0; index < sizeof(track); index++) {
		track[index] = (legacy_u8)(index * 7U);
	}
	dos_file_write(file, track, sizeof(track));
	for (legacy_u16 frame = 0; frame < frame_count && !truncated; frame++) {
		legacy_u8 input = (legacy_u8)(frame + 1U);
		dos_file_write(file, &input, 1);
	}
}

static void test_selection_is_separate(void)
{
	memcpy(gameconfig.game_playercarid, "COUN", REPLAY_CAR_ID_SIZE);
	gameconfig.game_playertransmission = TRANSMISSION_AUTOMATIC;
	gameconfig.game_recordedframes = 17;
	framespersec = GAME_FRAME_RATE_NORMAL;
	make_replay("source.rpl", GAME_FRAME_RATE_LOW, TEST_RECORDING_FRAMES, 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"source") == 0);
	assert(ghost_is_selected());
	assert(!ghost_is_active());
	assert(ghost_car_state() == 0);
	assert(memcmp(gameconfig.game_playercarid, "COUN", REPLAY_CAR_ID_SIZE) == 0);
	assert(gameconfig.game_playertransmission == TRANSMISSION_AUTOMATIC);
	assert(gameconfig.game_recordedframes == 17);
	assert(strcmp((char *)gameconfig.game_trackname, "GHOSTTRK") == 0);
	assert(test_track[42] == (legacy_u8)(42U * 7U));
	assert(cleanup_handler != 0);
	ghost_check_track();
	assert(ghost_is_selected());
}

static void test_invalid_selection_preserves_previous(void)
{
	make_replay("invalid.rpl", 0, 0, 0);
	make_replay("short.rpl", GAME_FRAME_RATE_NORMAL, 100, 1);
	make_replay("long.rpl", GAME_FRAME_RATE_NORMAL, 12001, 1);
	assert(ghost_select_replay(0, (const legacy_s8 *)"invalid") != 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"short") != 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"long") != 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"missing") != 0);
	fail_write = 1;
	assert(ghost_select_replay(0, (const legacy_s8 *)"source") != 0);
	fail_write = 0;
	assert(ghost_is_selected());
	assert(memcmp(ghost_car_id(), "ANSX", REPLAY_CAR_ID_SIZE) == 0);
}

static void test_playback_timing_and_isolation(void)
{
	elapsed_time1 = 111;
	elapsed_time2 = 222;
	checkpoint_frame_interval = 999;
	timer_ticks_per_frame = 11;
	state.playerstate.car_position.lx = 987654;
	state.game_frame = 21;
	game_replay_mode = REPLAY_MODE_PAUSED;
	is_in_replay = 0;
	replay_recording_flags = REPLAY_RECORDING_ACTIVE_FLAG;
	random_seed[0] = 99;
	legacy_execution_residue.penalty_route_word = 77;
	legacy_render_player_headings_active = 5;
	struct GAMESTATE before = state;
	struct GAMEINFO config = gameconfig;
	assert(ghost_prepare_race() == 0);
	assert(memcmp(&state, &before, sizeof(state)) == 0);
	assert(memcmp(&gameconfig, &config, sizeof(config)) == 0);
	assert(random_seed[0] == 99);
	assert(legacy_execution_residue.penalty_route_word == 77);
	assert(legacy_render_player_headings_active == 5);
	assert(game_replay_mode == REPLAY_MODE_PAUSED);
	assert(is_in_replay == 0);
	assert(replay_recording_flags == REPLAY_RECORDING_ACTIVE_FLAG);
	assert(framespersec == GAME_FRAME_RATE_NORMAL);
	assert(elapsed_time1 == 111 && elapsed_time2 == 222);
	assert(checkpoint_frame_interval == 999 && timer_ticks_per_frame == 11);
	assert(simulation_loads == 1 && simulation_frees == 1);
	assert(simulation_steps == TEST_RECORDING_FRAMES);
	assert(ghost_car_simd()->car_height == 47);
	assert(ghost_car_state()->car_position.lx == 0);
	ghost_update(6, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lx == 6);
	assert(ghost_car_state()->car_position.lz == 9);
	assert(ghost_car_state()->car_sound_flags == CAR_SOUND_NONE);
	ghost_update(16, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lx == 36);
	ghost_update(200000UL, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lx == 36);
	ghost_update(2, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lx == 1);
	assert(memcmp(&state, &before, sizeof(state)) == 0);
	ghost_end_race();
	assert(ghost_is_selected() && !ghost_is_active());
	assert(ghost_prepare_race() == 0);
	assert(ghost_car_state()->car_position.lx == 0);
	gameconfig.game_opponenttype = 1;
	assert(!ghost_is_active() && ghost_car_state() == 0);
	assert(ghost_prepare_race() == 0);
	assert(simulation_loads == 2);
	gameconfig.game_opponenttype = 0;
	assert(ghost_prepare_race() == 0);
	fail_read = 1;
	ghost_update(4, GAME_FRAME_RATE_NORMAL);
	fail_read = 0;
	assert(!ghost_is_active());
	assert(ghost_is_selected());
}

static void test_long_recording_and_preparation_failure(void)
{
	make_replay("maximum.rpl", GAME_FRAME_RATE_NORMAL, TRACKDATA_REPLAY_INPUT_BUFFER_SIZE, 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"maximum") == 0);
	struct GAMESTATE before = state;
	struct GAMEINFO config = gameconfig;
	fail_write = 1;
	assert(ghost_prepare_race() != 0);
	fail_write = 0;
	assert(ghost_is_selected() && !ghost_is_active());
	assert(memcmp(&state, &before, sizeof(state)) == 0);
	assert(memcmp(&gameconfig, &config, sizeof(config)) == 0);
	assert(ghost_prepare_race() == 0);
	/* This seek crosses the DOS 64 KiB boundary and converts 10 fps live
	 * timing into a 20 fps recording's timeline. */
	ghost_update(500, GAME_FRAME_RATE_LOW);
	legacy_s32 expected = 0;
	for (legacy_u16 frame = 0; frame < 1000; frame++) {
		expected += test_inputs[frame];
	}
	assert(ghost_car_state()->car_position.lx == expected);
	assert(ghost_car_state()->car_position.lz == 3000);
	ghost_update(6000, GAME_FRAME_RATE_LOW);
	assert(ghost_car_state()->car_position.lz == 36000);
	ghost_update(65536UL, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lz == 36000);
	ghost_update(0, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lx == 0);
	assert(ghost_car_state()->car_position.lz == 0);
	make_replay("slowmax.rpl", GAME_FRAME_RATE_LOW, TRACKDATA_REPLAY_INPUT_BUFFER_SIZE, 0);
	assert(ghost_select_replay(0, (const legacy_s8 *)"slowmax") == 0);
	assert(ghost_prepare_race() == 0);
	ghost_update(24000UL, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lz == 36000);
	ghost_update(1, GAME_FRAME_RATE_NORMAL);
	assert(ghost_car_state()->car_position.lz == 0);
}

static void test_track_changes_and_cleanup(void)
{
	test_track[100]++;
	ghost_check_track();
	assert(!ghost_is_selected());
	assert(ghost_select_replay(0, (const legacy_s8 *)"source") == 0);
	gameconfig.game_trackname[0] = 'X';
	ghost_check_track();
	assert(!ghost_is_selected());
	assert(ghost_select_replay(0, (const legacy_s8 *)"source") == 0);
	cleanup_handler();
	assert(!ghost_is_selected());
	for (legacy_u16 index = 1; index < TEST_FILE_COUNT; index++) {
		if (files[index].exists) {
			assert(files[index].name[0] != 'G');
			fclose(files[index].stream);
		}
	}
}

int main(void)
{
	test_selection_is_separate();
	test_invalid_selection_preserves_previous();
	test_playback_timing_and_isolation();
	test_long_recording_and_preparation_failure();
	test_track_changes_and_cleanup();
	return 0;
}
