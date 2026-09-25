#include <assert.h>
#include <string.h>

#include "../c/externs.h"
#include "../c/crash_state.h"

struct GAMESTATE state;
legacy_u16 framespersec;
legacy_u16 elapsed_time1;

static legacy_u32 random_calls;

legacy_s16 get_kevinrandom(void)
{
	random_calls++;
	return 3;
}

static void reset_state(void)
{
	memset(&state, 0, sizeof(state));
	state.game_frame = 123;
	state.playerstate.car_actual_speed = 12000;
	state.playerstate.car_rev_speed = 11000;
	state.opponentstate.car_actual_speed = 8000;
	state.opponentstate.car_rev_speed = 7000;
	framespersec = GAME_FRAME_RATE_NORMAL;
	elapsed_time1 = 7;
	random_calls = 0;
}

static void test_invalid_car_indices(void)
{
	static const legacy_s16 invalid_indices[] = {-32768, -1, 2, 32767};

	reset_state();
	struct GAMESTATE before = state;
	for (legacy_u16 index = 0; index < sizeof(invalid_indices) / sizeof(invalid_indices[0]);
		 index++) {
		for (legacy_s16 event = CRASH_EVENT_NONE; event <= CRASH_EVENT_IMMEDIATE_STOP; event++) {
			update_crash_state(event, invalid_indices[index]);
			assert(memcmp(&state, &before, sizeof(state)) == 0);
			assert(random_calls == 0);
		}
	}
}

static void test_player_and_opponent_selection(void)
{
	reset_state();
	struct CARSTATE opponent_before = state.opponentstate;
	update_crash_state(CRASH_EVENT_WATER, PLAYER_CAR_INDEX);
	assert(state.playerstate.car_crashBmpFlag == CRASH_EVENT_WATER);
	assert(state.playerstate.car_actual_speed == 0);
	assert(state.playerstate.car_rev_speed == 0);
	assert(memcmp(&state.opponentstate, &opponent_before, sizeof(opponent_before)) == 0);
	assert(state.game_pEndFrame == 123);
	assert(state.game_impactSpeed == 12000);
	assert(state.game_end_event == CRASH_EVENT_WATER);

	reset_state();
	struct CARSTATE player_before = state.playerstate;
	update_crash_state(CRASH_EVENT_FINISH, OPPONENT_CAR_INDEX);
	assert(state.opponentstate.car_crashBmpFlag == CRASH_EVENT_FINISH);
	assert(state.opponentstate.car_actual_speed == 8000);
	assert(state.opponentstate.car_rev_speed == 7000);
	assert(memcmp(&state.playerstate, &player_before, sizeof(player_before)) == 0);
	assert(state.game_oEndFrame == 123);
	assert(state.game_opponent_finish_time == 130);
	assert(state.game_end_event == CRASH_EVENT_NONE);
}

static void test_crash_event_is_applied_once(void)
{
	reset_state();
	update_crash_state(CRASH_EVENT_IMMEDIATE_STOP, PLAYER_CAR_INDEX);
	assert(state.playerstate.car_crashBmpFlag == CRASH_EVENT_COLLISION);
	assert(state.playerstate.car_actual_speed == 0);
	assert(state.playerstate.car_rev_speed == 0);
	assert(random_calls > 0);
	struct GAMESTATE before = state;
	legacy_u32 random_calls_before = random_calls;
	update_crash_state(CRASH_EVENT_WATER, PLAYER_CAR_INDEX);
	assert(memcmp(&state, &before, sizeof(state)) == 0);
	assert(random_calls == random_calls_before);
}

legacy_int main(void)
{
	test_invalid_car_indices();
	test_player_and_opponent_selection();
	test_crash_event_is_applied_once();
	return 0;
}
