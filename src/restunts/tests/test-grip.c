#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../c/externs.h"
#include "../c/state_internal.h"
#include "../c/physics_internal.h"
#include "../c/residue.h"
#include "../c/crash_state.h"

legacy_s16 grassDecelDivTab[5] = {255, 256, 192, 128, 64};
legacy_s16 terrainrows[30];
legacy_u8 far *track_element_map;

static struct CARSTATE car;
static struct SIMD simd;
static legacy_u8 elements[901];
static legacy_u32 random_state = 1;

#ifdef GRIP_DIFFERENTIAL
void reference_update_grip(struct CARSTATE *, struct SIMD *, legacy_s16);
void reference_update_legacy_grip_stack_words(struct CARSTATE *, struct SIMD *, legacy_u16,
											  legacy_u16);
#endif

static legacy_u16 random_word(void)
{
	random_state = random_state * 1664525UL + 1013904223UL;
	return (legacy_u16)(random_state >> 16);
}

static void reset_car(void)
{
	memset(&car, 0, sizeof(car));
	memset(&simd, 0, sizeof(simd));
	memset(&legacy_execution_residue, 0, sizeof(legacy_execution_residue));
	memset(elements, 0, sizeof(elements));
	for (legacy_s32 index = 0; index < 30; index++) {
		terrainrows[index] = index * 30;
	}
	track_element_map = elements;
	car.car_sumSurfAllWheels = 4;
	for (legacy_s32 index = 0; index < 4; index++) {
		car.car_surfaceWhl[index] = CAR_SURFACE_PAVED;
	}
	car.car_actual_speed = 16000;
	car.car_rev_speed = 16000;
	car.car_position.lx = 10L << 16;
	car.car_position.lz = 10L << 16;
	simd.grip = 500;
	simd.sliding = 256;
	for (legacy_s32 index = 0; index < SIMD_SURFACE_GRIP_COUNT; index++) {
		simd.surface_grip[index] = 256;
	}
}

static void run_grip(legacy_s16 behavior)
{
	legacy_u16 speed_before = car.car_rev_speed;
	legacy_u16 actual_before = car.car_actual_speed;
#ifdef GRIP_DIFFERENTIAL
	struct CARSTATE expected = car;
	struct LEGACY_EXECUTION_RESIDUE before = legacy_execution_residue;
	reference_update_grip(&expected, &simd, behavior);
	reference_update_legacy_grip_stack_words(&expected, &simd, speed_before, actual_before);
	struct LEGACY_EXECUTION_RESIDUE expected_residue = legacy_execution_residue;
	legacy_execution_residue = before;
#endif
	update_grip(&car, &simd, behavior);
	update_legacy_grip_stack_words(&car, &simd, speed_before, actual_before,
								   LEGACY_DEFAULT_PLAYER_TICK_SI);
#ifdef GRIP_DIFFERENTIAL
	assert(memcmp(&expected, &car, sizeof(car)) == 0);
	assert(memcmp(&expected_residue, &legacy_execution_residue, sizeof(expected_residue)) == 0);
#endif
}

static void test_contact_and_grass(void)
{
	reset_car();
	car.car_sumSurfAllWheels = 0;
	car.car_front_wheel_response_angle = 32;
	car.car_slidingFlag = CAR_SLIDING_ACTIVE;
	car.car_slip_angle = 100;
	run_grip(GRIP_BEHAVIOR_PLAYER);
	assert(car.car_front_wheel_response_angle == 0);
	assert(car.car_slidingFlag == CAR_SLIDING_INACTIVE);
	assert(car.car_actual_speed == 16000);
	assert(car.car_slip_angle == 100);
	assert(legacy_execution_residue.grip_stack_words[3] == 80);
	for (legacy_s32 grass_count = 1; grass_count <= 4; grass_count++) {
		reset_car();
		for (legacy_s32 index = 0; index < grass_count; index++) {
			car.car_surfaceWhl[index] = CAR_SURFACE_GRASS;
		}
		run_grip(GRIP_BEHAVIOR_PLAYER);
		assert(car.car_actual_speed == 16000 - 16000 / grassDecelDivTab[grass_count]);
		assert(car.car_rev_speed == car.car_actual_speed);
		assert(car.car_demandedGrip == 0);
		assert(car.car_surfacegrip_sum == 1000);
	}
}

static void test_recenter_and_banking(void)
{
	static const legacy_s16 expected[] = {-8, -6, 0, 0, 0, 6, 8};
	static const legacy_s16 input[] = {-8, -7, -1, 0, 1, 7, 8};
	for (legacy_s32 rotation = 0; rotation < 7; rotation++) {
		reset_car();
		car.car_rotate.x = input[rotation];
		run_grip(GRIP_BEHAVIOR_PLAYER);
		assert(car.car_rotate.x == expected[rotation]);
	}
	reset_car();
	car.car_rotate.z = 25;
	elements[terrainrows[10] + 10] = 253;
	elements[terrainrows[11] + 9] = 52;
	run_grip(GRIP_BEHAVIOR_PLAYER);
	assert(car.car_front_wheel_response_angle == 5);
	reset_car();
	car.car_steeringAngle = 12;
	car.car_slide_yaw_delta = 16;
	run_grip(GRIP_BEHAVIOR_OPPONENT);
	assert(car.car_front_wheel_response_angle == 48);
	assert(car.car_slide_yaw_delta == 15);
	assert(car.car_velocity_heading_offset == -15);
}

static void configure_collision_option(const char *option)
{
	legacy_s8 *argv[] = {(legacy_s8 *)"restunts", (legacy_s8 *)option};
	configure_legacy_collision(option ? 2 : 1, argv);
}

static void test_legacy_collision_recovery(void)
{
	static const char *options[] = {NULL, "/lc:on"};
	for (legacy_u32 index = 0; index < sizeof(options) / sizeof(options[0]); index++) {
		configure_collision_option(options[index]);
		reset_car();
		car.car_velocity_heading_offset = -46;
		for (legacy_s32 frame = 0; frame < 128; frame++) {
			run_grip(GRIP_BEHAVIOR_PLAYER);
		}
		assert(car.car_steeringAngle == CAR_STEERING_CENTERED);
		assert(car.car_velocity_heading_offset == -15);
		assert(car.car_front_wheel_response_angle == -15);
		reset_car();
		car.car_slide_yaw_delta = -15;
		run_grip(GRIP_BEHAVIOR_OPPONENT);
		assert(car.car_slide_yaw_delta == -15);
		assert(car.car_velocity_heading_offset == 15);
	}
}

static void assert_collision_recovery(legacy_s16 angle, legacy_s16 behavior, legacy_u16 speed)
{
	reset_car();
	car.car_actual_speed = speed;
	car.car_rev_speed = speed;
	if (behavior == GRIP_BEHAVIOR_PLAYER) {
		car.car_velocity_heading_offset = angle;
	} else {
		car.car_slide_yaw_delta = angle;
	}
	for (legacy_s32 frame = 0; frame < 512; frame++) {
		legacy_s16 before = behavior == GRIP_BEHAVIOR_PLAYER ? car.car_velocity_heading_offset
															 : car.car_slide_yaw_delta;
		update_grip(&car, &simd, behavior);
		legacy_s16 after = behavior == GRIP_BEHAVIOR_PLAYER ? car.car_velocity_heading_offset
															: car.car_slide_yaw_delta;
		if (before < 0) {
			assert(after > before && after <= 0);
		} else if (before > 0) {
			assert(after < before && after >= 0);
		} else {
			assert(after == 0);
		}
		assert(car.car_steeringAngle == CAR_STEERING_CENTERED);
	}
	assert(car.car_velocity_heading_offset == 0);
	assert(car.car_slide_yaw_delta == 0);
	assert(car.car_front_wheel_response_angle == 0);
	assert(car.car_actual_speed == speed);
	assert(car.car_rev_speed == speed);
}

static void test_corrected_collision_recovery(void)
{
	static const struct {
		legacy_s16 angle;
		legacy_s16 damped;
	} cases[] = {
		{-32768, -30720}, {-4096, -3840}, {-46, -43}, {-16, -15},	{-15, -14},		{0, 0},
		{15, 14},		  {16, 15},		  {46, 43},	  {4096, 3840}, {32767, 30719},
	};
	configure_collision_option("/lc:off");
	/* Include every formerly stuck negative remainder and the replay's -46 offset. */
	for (legacy_s16 angle = -64; angle <= 64; angle++) {
		assert_collision_recovery(angle, GRIP_BEHAVIOR_PLAYER, 16000);
		assert_collision_recovery(angle, GRIP_BEHAVIOR_OPPONENT, 16000);
		assert(damp_collision_angle(angle) == -damp_collision_angle(-angle));
	}
	for (legacy_u32 index = 0; index < sizeof(cases) / sizeof(cases[0]); index++) {
		assert(damp_collision_angle(cases[index].angle) == cases[index].damped);
		/* Low speed keeps large heading offsets within the available tire grip. */
		assert_collision_recovery(cases[index].angle, GRIP_BEHAVIOR_PLAYER, 256);
		assert_collision_recovery(cases[index].angle, GRIP_BEHAVIOR_OPPONENT, 256);
	}
}

/* Inputs stay inside the track and use the six valid contact coefficients.
 * Signed edge values stress the explicitly wrapped 16-bit grip arithmetic. */
static void test_wrapped_grip_sweep(void)
{
	static const legacy_s16 angles[] = {-32768, -16384, -1024, -256, -16,	-1,	  0,
										1,		16,		256,   1024, 16384, 32767};
	static const legacy_s16 grips[] = {-32768, -1, 0, 1, 256, 500, 16384, 32767};
	static const legacy_u16 speeds[] = {0, 1, 255, 256, 257, 32767, 32768, 65535};
	legacy_u32 hash = 2166136261UL;
	for (legacy_u32 sample = 0; sample < 200000; sample++) {
		reset_car();
		car.car_steeringAngle = angles[random_word() % 13];
		car.car_velocity_heading_offset = angles[random_word() % 13];
		car.car_rotate.x = angles[random_word() % 13];
		car.car_rotate.z = angles[random_word() % 13];
		car.car_slide_yaw_delta = angles[random_word() % 13];
		car.car_slip_angle = angles[random_word() % 13];
		car.car_actual_speed = speeds[random_word() % 8];
		car.car_rev_speed = speeds[random_word() % 8];
		car.car_slidingFlag = random_word() % 2;
		car.car_crashBmpFlag = random_word() % 3;
		car.car_sumSurfAllWheels = random_word() % 5;
		for (legacy_u32 index = 0; index < 4; index++) {
			car.car_surfaceWhl[index] = random_word() % 6;
		}
		simd.grip = grips[random_word() % 8];
		simd.sliding = grips[random_word() % 8];
		for (legacy_u32 index = 0; index < SIMD_SURFACE_GRIP_COUNT; index++) {
			simd.surface_grip[index] = grips[random_word() % 8];
		}
		elements[terrainrows[10] + 10] = 51 + random_word() % 6;
		run_grip((legacy_s16)(sample % 3));
		const legacy_u8 *bytes = (const legacy_u8 *)&car;
		for (legacy_u32 index = 0; index < sizeof(car); index++) {
			hash = (hash ^ bytes[index]) * 16777619UL;
		}
		bytes = (const legacy_u8 *)&legacy_execution_residue;
		for (legacy_u32 index = 0; index < sizeof(legacy_execution_residue); index++) {
			hash = (hash ^ bytes[index]) * 16777619UL;
		}
	}
#ifdef GRIP_RECORD_BASELINE
	fprintf(stdout, "%08" LEGACY_PRIx32 "\n", hash);
#else
	/* Captured from the original update_grip and explicit stack-residue model. */
	assert(hash == 0x216e96a0UL);
#endif
}

int main(void)
{
	test_contact_and_grass();
	test_recenter_and_banking();
	test_wrapped_grip_sweep();
	test_legacy_collision_recovery();
	test_corrected_collision_recovery();
	return 0;
}
