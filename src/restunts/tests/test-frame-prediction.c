#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "../c/frame_prediction.h"
#include "../c/crash_state.h"

static void test_motion_and_isolation(void)
{
	struct GAMESTATE previous = {0};
	struct GAMESTATE current = {0};
	struct GAMESTATE result;
	current.game_frame = 123;
	current.game_penalty = 50;
	current.playerstate.car_rev_speed = 600;
	previous.playerstate.car_position = (struct VECTORLONG){100, 200, 300};
	current.playerstate.car_position = (struct VECTORLONG){160, 180, 270};
	previous.playerstate.car_rotate = (struct VECTOR){1020, 4, 3};
	current.playerstate.car_rotate = (struct VECTOR){4, 1020, 1};
	previous.playerstate.car_suspension_deflection[2] = 40;
	current.playerstate.car_suspension_deflection[2] = 30;
	previous.game_follow_camera_position[0] = (struct VECTOR){100, 100, 100};
	current.game_follow_camera_position[0] = (struct VECTOR){90, 100, 110};
	struct GAMESTATE saved_previous = previous;
	struct GAMESTATE saved_current = current;
	frame_predict_state(&result, &current, &previous, FRAME_PREDICTION_ONE / 2U);
	assert(result.playerstate.car_position.lx == 190);
	assert(result.playerstate.car_position.ly == 170);
	assert(result.playerstate.car_position.lz == 255);
	assert(result.playerstate.car_rotate.x == 8);
	assert(result.playerstate.car_rotate.y == 1016);
	assert(result.playerstate.car_rotate.z == 0);
	assert(result.playerstate.car_suspension_deflection[2] == 25);
	assert(result.game_follow_camera_position[0].x == 85);
	assert(result.game_follow_camera_position[0].z == 115);
	assert(result.game_frame == current.game_frame);
	assert(result.game_penalty == current.game_penalty);
	assert(result.playerstate.car_rev_speed == current.playerstate.car_rev_speed);
	assert(memcmp(&previous, &saved_previous, sizeof(previous)) == 0);
	assert(memcmp(&current, &saved_current, sizeof(current)) == 0);
	/* Signed division must truncate tiny negative deltas toward zero. */
	frame_predict_state(&result, &current, &previous, 1);
	assert(result.playerstate.car_position.ly == 180);
	assert(result.playerstate.car_rotate.z == 1);
}

static void test_prediction_bounds(void)
{
	struct CARSTATE previous = {0};
	struct CARSTATE current = {0};
	struct CARSTATE result;
	previous.car_position = (struct VECTORLONG){INT32_MIN, INT32_MAX, 10};
	current.car_position = (struct VECTORLONG){INT32_MAX, INT32_MIN, 20};
	previous.car_steeringAngle = -32768;
	current.car_steeringAngle = 32767;
	frame_predict_car(&result, &current, &previous, UINT32_MAX);
	assert(result.car_position.lx == INT32_MAX);
	assert(result.car_position.ly == INT32_MIN);
	assert(result.car_position.lz == 30);
	assert(result.car_steeringAngle == 32767);
	frame_predict_car(&result, &current, &previous, 0);
	assert(memcmp(&result, &current, sizeof(current)) == 0);
}

static void test_discontinuities(void)
{
	struct GAMESTATE previous = {0};
	struct GAMESTATE current = {0};
	struct GAMESTATE result;
	previous.playerstate.car_position.lx = 100;
	current.playerstate.car_position.lx = 200;
	current.playerstate.car_crashBmpFlag = CRASH_EVENT_COLLISION;
	previous.game_follow_camera_position[0].x = 100;
	current.game_follow_camera_position[0].x = 400;
	current.game_trackside_camera_index[0] = 1;
	current.game_particles_active = 1;
	current.game_particle_forward_speed[0] = 20;
	current.game_particle_x[0] = 1000;
	frame_predict_state(&result, &current, &previous, FRAME_PREDICTION_ONE);
	assert(result.playerstate.car_position.lx == 200);
	assert(result.game_follow_camera_position[0].x == 400);
	assert(result.game_particle_x[0] == 1000);
	previous.game_particles_active = 1;
	previous.game_particle_forward_speed[0] = 20;
	previous.game_particle_x[0] = 900;
	frame_predict_state(&result, &current, &previous, FRAME_PREDICTION_ONE / 2U);
	assert(result.game_particle_x[0] == 1050);
	current.game_particle_owner[0] = 1;
	frame_predict_state(&result, &current, &previous, FRAME_PREDICTION_ONE / 2U);
	assert(result.game_particle_x[0] == 1000);
}

int main(void)
{
	test_motion_and_isolation();
	test_prediction_bounds();
	test_discontinuities();
	return 0;
}
