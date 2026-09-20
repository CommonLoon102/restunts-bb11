#include "ghost.h"
#include "externs.h"
#include "fatal.h"
#include "fileio.h"
#include "platform.h"
#include "race_resources.h"
#include "residue.h"
#include "trackdata_layout.h"
#include "track_objects.h"

#define GHOST_TEMP_NAME_SIZE 13U
#define GHOST_TEMP_NAME_DIGITS 6U
#define GHOST_COPY_BUFFER_SIZE 256U
#define GHOST_MAX_OPPONENT_TYPE 6

static struct GAMEINFO ghost_config;
static struct CARSTATE ghost_state;
static struct SIMD ghost_simd;
static legacy_u16 ghost_file;
static legacy_s8 ghost_temp_name[GHOST_TEMP_NAME_SIZE];
static legacy_s16 ghost_active;
static legacy_s16 ghost_cleanup_registered;
static legacy_s16 ghost_pose_valid;
static legacy_u16 ghost_pose_frame;

static legacy_s16 ghost_seek(legacy_u16 file, legacy_s32 offset)
{
	(void)dos_file_error();
	(void)dos_file_seek(file, offset, DOS_FILE_SEEK_BEGIN);
	return dos_file_error() == 0;
}

void ghost_end_race(void)
{
	ghost_active = 0;
	ghost_pose_valid = 0;
}

void ghost_clear(void)
{
	ghost_end_race();
	if (ghost_file != 0) {
		(void)dos_file_close(ghost_file);
		ghost_file = 0;
	}
	if (ghost_temp_name[0] != 0) {
		(void)dos_file_remove(ghost_temp_name);
		ghost_temp_name[0] = 0;
	}
}

static void far ghost_cleanup(void)
{
	ghost_clear();
}

legacy_s16 ghost_is_selected(void)
{
	return ghost_file != 0;
}

legacy_s16 ghost_is_active(void)
{
	return ghost_active != 0 && gameconfig.game_opponenttype == 0;
}

struct CARSTATE *ghost_car_state(void)
{
	return ghost_is_active() && ghost_pose_valid ? &ghost_state : 0;
}

const struct SIMD *ghost_car_simd(void)
{
	return ghost_is_active() ? &ghost_simd : 0;
}

legacy_u8 ghost_car_material(void)
{
	return (legacy_u8)ghost_config.game_playermaterial;
}

const legacy_s8 *ghost_car_id(void)
{
	return ghost_config.game_playercarid;
}

/* The DOS create API truncates existing files, so only use an unused private
 * name. No persistent arena allocation may pin the opponent menu's resources. */
static legacy_u16 ghost_create_file(legacy_s8 *name)
{
	for (legacy_u32 candidate = 0; candidate < 1000000UL; candidate++) {
		legacy_u32 digits = candidate;
		name[0] = 'G';
		name[1] = 'H';
		for (legacy_u16 index = 0; index < GHOST_TEMP_NAME_DIGITS; index++) {
			name[7U - index] = (legacy_s8)('0' + digits % 10UL);
			digits /= 10UL;
		}
		name[8] = '.';
		name[9] = 'T';
		name[10] = 'M';
		name[11] = 'P';
		name[12] = 0;
		if (dos_file_find_first(name) == 0) {
			return dos_file_open(name, DOS_FILE_CREATE);
		}
	}
	return 0;
}

static legacy_s16 ghost_valid_car_id(const legacy_s8 *id)
{
	for (legacy_u16 index = 0; index < REPLAY_CAR_ID_SIZE; index++) {
		legacy_u8 character = (legacy_u8)id[index];
		if (character <= ' ' || character == '/' || character == '\\' || character == ':' ||
			character == '*' || character == '?' || character == '"' || character == '<' ||
			character == '>' || character == '|') {
			return 0;
		}
	}
	return 1;
}

static legacy_s16 ghost_valid_config(const struct GAMEINFO *config)
{
	if (config->game_framespersec != GAME_FRAME_RATE_LOW &&
		config->game_framespersec != GAME_FRAME_RATE_NORMAL) {
		return 0;
	}
	if (config->game_recordedframes > TRACKDATA_REPLAY_INPUT_BUFFER_SIZE ||
		config->game_opponenttype < 0 || config->game_opponenttype > GHOST_MAX_OPPONENT_TYPE ||
		config->game_playertransmission < TRANSMISSION_MANUAL ||
		config->game_playertransmission > TRANSMISSION_AUTOMATIC ||
		!ghost_valid_car_id(config->game_playercarid)) {
		return 0;
	}
	return config->game_opponenttype == 0 || ghost_valid_car_id(config->game_opponentcarid);
}

static legacy_s16 ghost_copy_input(legacy_u16 source, legacy_u16 destination, legacy_u16 length)
{
	legacy_u8 buffer[GHOST_COPY_BUFFER_SIZE];
	while (length != 0) {
		legacy_u16 count = length > sizeof(buffer) ? sizeof(buffer) : length;
		if (dos_file_read(source, buffer, count) != count ||
			dos_file_write(destination, buffer, count) != count) {
			return 0;
		}
		length -= count;
	}
	return 1;
}

legacy_s16 ghost_select_replay(const legacy_s8 *directory, const legacy_s8 *name)
{
	legacy_s8 path[REPLAY_FILENAME_SIZE];
	legacy_u16 directory_length = directory == 0 ? 0 : strlen(directory);
	if ((legacy_u32)directory_length + strlen(name) + 6UL > sizeof(path)) {
		return 1;
	}
	file_build_path(directory, name, ".rpl", path);
	legacy_u16 source = dos_file_open(path, DOS_FILE_OPEN_EXISTING);
	if (source == 0) {
		return 1;
	}
	legacy_u8 header[REPLAY_INPUT_OFFSET];
	struct GAMEINFO config;
	if (dos_file_read(source, header, sizeof(header)) != sizeof(header)) {
		(void)dos_file_close(source);
		return 1;
	}
	replay_gameinfo_decode(&config, header);
	if (!ghost_valid_config(&config)) {
		(void)dos_file_close(source);
		return 1;
	}
	config.game_trackname[REPLAY_TRACK_NAME_SIZE - 1U] = 0;
	legacy_s8 temp_name[GHOST_TEMP_NAME_SIZE];
	legacy_u16 destination = ghost_create_file(temp_name);
	if (destination == 0) {
		(void)dos_file_close(source);
		return 1;
	}
	legacy_s16 copied = dos_file_write(destination, header, sizeof(header)) == sizeof(header) &&
						ghost_copy_input(source, destination, config.game_recordedframes);
	(void)dos_file_close(source);
	if (!copied) {
		(void)dos_file_close(destination);
		(void)dos_file_remove(temp_name);
		return 1;
	}

	/* Preserve the previous track in the header scratch space until setup and
	 * storage both succeed. Track setup canonicalizes some large tile IDs. */
	for (legacy_u16 index = 0; index < REPLAY_TRACK_SIZE; index++) {
		legacy_u8 previous = track_element_map[index];
		track_element_map[index] = header[REPLAY_GAMEINFO_SIZE + index];
		header[REPLAY_GAMEINFO_SIZE + index] = previous;
	}
	if (track_setup() != 0 || !ghost_seek(destination, REPLAY_GAMEINFO_SIZE) ||
		dos_file_write(destination, track_element_map, REPLAY_TRACK_SIZE) != REPLAY_TRACK_SIZE) {
		for (legacy_u16 index = 0; index < REPLAY_TRACK_SIZE; index++) {
			track_element_map[index] = header[REPLAY_GAMEINFO_SIZE + index];
		}
		(void)track_setup();
		(void)dos_file_close(destination);
		(void)dos_file_remove(temp_name);
		return 1;
	}

	ghost_clear();
	ghost_file = destination;
	strcpy(ghost_temp_name, temp_name);
	ghost_config = config;
	if (!ghost_cleanup_registered) {
		add_exit_handler(ghost_cleanup);
		ghost_cleanup_registered = 1;
	}
	for (legacy_u16 index = 0; index < REPLAY_TRACK_NAME_SIZE; index++) {
		gameconfig.game_trackname[index] = ghost_config.game_trackname[index];
	}
	return 0;
}

void ghost_check_track(void)
{
	if (!ghost_is_selected()) {
		return;
	}
	for (legacy_u16 index = 0; index < REPLAY_TRACK_NAME_SIZE; index++) {
		if (gameconfig.game_trackname[index] != ghost_config.game_trackname[index]) {
			ghost_clear();
			return;
		}
	}
	if (!ghost_seek(ghost_file, REPLAY_GAMEINFO_SIZE)) {
		ghost_clear();
		return;
	}
	legacy_u8 buffer[GHOST_COPY_BUFFER_SIZE];
	legacy_u16 offset = 0;
	while (offset < REPLAY_TRACK_SIZE) {
		legacy_u16 remaining = REPLAY_TRACK_SIZE - offset;
		legacy_u16 count = remaining > sizeof(buffer) ? sizeof(buffer) : remaining;
		if (dos_file_read(ghost_file, buffer, count) != count) {
			ghost_clear();
			return;
		}
		for (legacy_u16 index = 0; index < count; index++) {
			if (buffer[index] != track_element_map[offset + index]) {
				ghost_clear();
				return;
			}
		}
		offset += count;
	}
}

/* A replay stores controls rather than positions. Reconstruct its original
 * race (including its AI) before starting the independent live race. Poses are
 * kept on disk so the full recording buffer fits conventional DOS memory. */
legacy_s16 ghost_prepare_race(void)
{
	ghost_end_race();
	ghost_check_track();
	if (!ghost_is_selected() || gameconfig.game_opponenttype != 0) {
		return 0;
	}
	if (!ghost_seek(ghost_file, REPLAY_INPUT_OFFSET) ||
		dos_file_read(ghost_file, replay_input_buffer, ghost_config.game_recordedframes) !=
			ghost_config.game_recordedframes) {
		return 1;
	}
	struct GAMEINFO saved_config = gameconfig;
	struct GAMESTATE saved_state = state;
	struct LEGACY_EXECUTION_RESIDUE saved_residue = legacy_execution_residue;
	legacy_s16 saved_render_headings = legacy_render_player_headings_active;
	legacy_u16 saved_frame_rate = framespersec;
	legacy_u8 saved_mode = game_replay_mode;
	legacy_s8 saved_is_in_replay = is_in_replay;
	legacy_s8 saved_recording_flags = replay_recording_flags;
	legacy_u16 saved_elapsed_time1 = elapsed_time1;
	legacy_u16 saved_elapsed_time2 = elapsed_time2;
	legacy_s16 saved_checkpoint_interval = checkpoint_frame_interval;
	legacy_s16 saved_timer_ticks = timer_ticks_per_frame;
	legacy_s8 *saved_steering_table = steerWhlRespTable_ptr;
	legacy_u8 saved_frame_countdown = frame_callback_countdown;
	legacy_u8 saved_slow_countdown = slow_replay_countdown;
	legacy_u8 saved_exit_request = race_exit_request;
	legacy_u8 saved_start_sequence = race_start_sequence_state;
	legacy_s16 saved_flag_animation = start_flag_animation;
	legacy_s8 seed[GAMESTATE_RANDOM_SEED_SIZE];
	get_kevinrandom_seed(seed);
	gameconfig = ghost_config;
	game_replay_mode = REPLAY_MODE_PLAYBACK;
	/* Direct impact sounds and result totals have separate replay guards. */
	is_in_replay = 1;
	replay_recording_flags = REPLAY_RECORDING_RESTARTABLE_FLAG;
	framespersec = ghost_config.game_framespersec;
	struct LEGACY_EXECUTION_RESIDUE empty_residue = {0};
	legacy_execution_residue = empty_residue;
	legacy_render_player_headings_active = 0;
	ghost_load_simulation_resources();
	ghost_simd = simd_player;
	init_game_state(GAMESTATE_INIT_RESET_CHECKPOINTS);
	legacy_s16 succeeded = 1;
	for (legacy_u16 frame = 0; frame <= ghost_config.game_recordedframes; frame++) {
		if (frame != 0) {
			update_gamestate_silent();
		}
		if (dos_file_write(ghost_file, &state.playerstate, sizeof(state.playerstate)) !=
			sizeof(state.playerstate)) {
			succeeded = 0;
			break;
		}
	}
	ghost_free_simulation_resources();
	gameconfig = saved_config;
	state = saved_state;
	framespersec = saved_frame_rate;
	game_replay_mode = saved_mode;
	is_in_replay = saved_is_in_replay;
	replay_recording_flags = saved_recording_flags;
	elapsed_time1 = saved_elapsed_time1;
	elapsed_time2 = saved_elapsed_time2;
	checkpoint_frame_interval = saved_checkpoint_interval;
	timer_ticks_per_frame = saved_timer_ticks;
	steerWhlRespTable_ptr = saved_steering_table;
	frame_callback_countdown = saved_frame_countdown;
	slow_replay_countdown = saved_slow_countdown;
	race_exit_request = saved_exit_request;
	race_start_sequence_state = saved_start_sequence;
	start_flag_animation = saved_flag_animation;
	legacy_execution_residue = saved_residue;
	legacy_render_player_headings_active = saved_render_headings;
	init_kevinrandom(seed);
	if (!succeeded) {
		return 1;
	}
	ghost_active = 1;
	ghost_update(0, framespersec);
	return ghost_pose_valid ? 0 : 1;
}

void ghost_update(legacy_u32 frame, legacy_u16 live_frame_rate)
{
	if (!ghost_is_active() || live_frame_rate == 0) {
		return;
	}
	legacy_u32 target = (legacy_u32)frame * ghost_config.game_framespersec / live_frame_rate;
	if (target > ghost_config.game_recordedframes) {
		target = ghost_config.game_recordedframes;
	}
	if (ghost_pose_valid && target == ghost_pose_frame) {
		return;
	}
	legacy_u32 offset =
		replay_file_size(ghost_config.game_recordedframes) + target * sizeof(struct CARSTATE);
	if (!ghost_seek(ghost_file, (legacy_s32)offset) ||
		dos_file_read(ghost_file, &ghost_state, sizeof(ghost_state)) != sizeof(ghost_state)) {
		ghost_end_race();
		return;
	}
	ghost_state.car_sound_flags = CAR_SOUND_NONE;
	ghost_pose_frame = (legacy_u16)target;
	ghost_pose_valid = 1;
}
