#include <assert.h>
#include <stdio.h>

#include "../c/stateply.c"
#include "../c/math.c"

struct GAMESTATE state;
struct MATRIX car_to_world_rotation;
legacy_s16 car_working_pitch, car_working_roll, car_working_yaw;
legacy_u16 framespersec;
legacy_s16 planindex;
struct PLANE *planptr;

static void test_inherited_contact_reconciliation(void)
{
	/* New impacts retain their whole correction, even for a very short step. */
	assert(retained_wheel_plane_residual(1, -100, 1) == 0);
	assert(retained_wheel_plane_residual(0, -100, 21845UL) == 0);
	assert(retained_wheel_plane_residual(-100, 1, 21845UL) == 0);

	/* Existing compression relaxes with time; extra penetration is removed now. */
	assert(retained_wheel_plane_residual(-12, -30, 0) == -12);
	assert(retained_wheel_plane_residual(-12, -30, 21845UL) == -8);
	assert(retained_wheel_plane_residual(-12, -30, PHANTOM_PHYSICS_ONE) == 0);
	assert(retained_wheel_plane_residual(-12, -2, 21845UL) == -2);

	for (legacy_s16 previous = -384; previous <= 32; previous++) {
		for (legacy_s16 current = -384; current <= 32; current++) {
			legacy_s16 residual = retained_wheel_plane_residual(previous, current, 21845UL);
			if (previous >= 0 || current >= 0) {
				assert(residual == 0);
			} else {
				/* Correcting a short step never makes an overlap deeper, and
				 * never retains penetration created after its starting pose. */
				assert(residual >= current);
				assert(residual >= previous);
				assert(residual <= 0);
			}
		}
	}
}

static void test_fractional_inverted_attachment(void)
{
	static const legacy_u32 fractions[] = {0, 1, 21845UL, PHANTOM_PHYSICS_ONE};
	static const legacy_u16 rates[] = {GAME_FRAME_RATE_NORMAL, GAME_FRAME_RATE_LOW};
	static const legacy_s16 expected[2][4] = {{192, 0, 64, 192}, {192, 0, 32, 96}};
	for (legacy_u16 rate = 0; rate < sizeof(rates) / sizeof(rates[0]); rate++) {
		struct CARSTATE car = {0};
		struct PLAYER_WHEEL_MOTION motion = {0};
		framespersec = rates[rate];
		car.car_sumSurfAllWheels = 4;
		car.car_actual_speed = PLAYER_PHYSICS_LOW_SPEED_LIMIT + 1;
		car_working_pitch = car_working_yaw = 0;
		car_working_roll = ANGLE_HALF_TURN;
		for (legacy_u16 i = 0; i < sizeof(fractions) / sizeof(fractions[0]); i++) {
			player_motion_fraction = fractions[i];
			prepare_wheel_rotation(&car, &motion);
			assert(motion.inverted_adjustment == expected[rate][i]);
			assert(motion.inverted_offset.x == 0);
			assert(motion.inverted_offset.y == expected[rate][i]);
			assert(motion.inverted_offset.z == 0);
		}
		car.car_actual_speed = PLAYER_PHYSICS_LOW_SPEED_LIMIT;
		player_motion_fraction = 21845UL;
		prepare_wheel_rotation(&car, &motion);
		assert(motion.inverted_adjustment == -expected[rate][2]);
		car_working_roll = 0;
		prepare_wheel_rotation(&car, &motion);
		assert(motion.inverted_adjustment == 0);
		car_working_roll = ANGLE_HALF_TURN;
		car.car_sumSurfAllWheels = CAR_WHEEL_CONTACT_NONE;
		prepare_wheel_rotation(&car, &motion);
		assert(motion.inverted_adjustment == 0);
	}
	player_motion_fraction = 0;
}

static void test_contact_residual_applies_to_wheel(void)
{
	struct PLANE plane = {0};
	struct PLAYER_WHEEL_MOTION motion = {0};
	mat_rot_y(&plane.plane_rotation, 0);
	planptr = &plane;
	planindex = 0;
	framespersec = GAME_FRAME_RATE_NORMAL;
	player_motion_fraction = 21845UL;
	motion.current[0].ly = 6400;
	motion.contact_distances[0] = -30;
	relax_wheel_plane_residual(&motion, 0, -12);
	assert(motion.current[0].ly == 6400 - 8 * 64);
	assert(motion.contact_distances[0] == -22);
	motion.current[0].ly = 6400;
	motion.contact_distances[0] = -30;
	relax_wheel_plane_residual(&motion, 0, 1);
	assert(motion.current[0].ly == 6400);
	assert(motion.contact_distances[0] == -30);
	motion.contact_distances[0] = 12;
	relax_wheel_plane_residual(&motion, 0, 12);
	assert(motion.current[0].ly == 6400);
	assert(motion.contact_distances[0] == 4);
	player_motion_fraction = 0;
}

legacy_int main(void)
{
	test_inherited_contact_reconciliation();
	test_fractional_inverted_attachment();
	test_contact_residual_applies_to_wheel();
	puts("phantom motion regression tests passed");
	return 0;
}
