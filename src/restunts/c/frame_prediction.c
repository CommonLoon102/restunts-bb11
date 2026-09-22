#include "frame_prediction.h"

static legacy_u32 prediction_fraction(legacy_u32 fraction)
{
	return fraction > FRAME_PREDICTION_ONE ? FRAME_PREDICTION_ONE : fraction;
}

static legacy_s32 predict_position(legacy_s32 current, legacy_s32 previous, legacy_u32 fraction)
{
	legacy_s64 predicted =
		current + ((legacy_s64)current - previous) * fraction / (legacy_s64)FRAME_PREDICTION_ONE;
	if (predicted > 2147483647LL) {
		return 2147483647L;
	}
	if (predicted < -2147483647LL - 1) {
		return (-2147483647L - 1);
	}
	return (legacy_s32)predicted;
}

static legacy_s16 predict_word(legacy_s16 current, legacy_s16 previous, legacy_u32 fraction)
{
	legacy_s32 predicted = predict_position(current, previous, fraction);
	if (predicted > 32767) {
		return 32767;
	}
	if (predicted < -32768L) {
		return -32768L;
	}
	return (legacy_s16)predicted;
}

static legacy_s16 predict_angle(legacy_s16 current, legacy_s16 previous, legacy_u32 fraction)
{
	legacy_s32 delta = ((legacy_u16)(current - previous + ANGLE_HALF_TURN) & ANGLE_MASK) -
					   (legacy_s32)ANGLE_HALF_TURN;
	return (
		legacy_s16)((current + delta * (legacy_s32)fraction / (legacy_s32)FRAME_PREDICTION_ONE) &
					ANGLE_MASK);
}

void frame_predict_vector(struct VECTOR *result, const struct VECTOR *current,
						  const struct VECTOR *previous, legacy_u32 fraction)
{
	fraction = prediction_fraction(fraction);
	result->x = predict_word(current->x, previous->x, fraction);
	result->y = predict_word(current->y, previous->y, fraction);
	result->z = predict_word(current->z, previous->z, fraction);
}

void frame_predict_car(struct CARSTATE *result, const struct CARSTATE *current,
					   const struct CARSTATE *previous, legacy_u32 fraction)
{
	*result = *current;
	fraction = prediction_fraction(fraction);
	/* A crash is an authoritative discontinuity, never a velocity estimate. */
	if (fraction == 0 || current->car_crashBmpFlag != previous->car_crashBmpFlag) {
		return;
	}
	result->car_position.lx =
		predict_position(current->car_position.lx, previous->car_position.lx, fraction);
	result->car_position.ly =
		predict_position(current->car_position.ly, previous->car_position.ly, fraction);
	result->car_position.lz =
		predict_position(current->car_position.lz, previous->car_position.lz, fraction);
	result->car_rotate.x = predict_angle(current->car_rotate.x, previous->car_rotate.x, fraction);
	result->car_rotate.y = predict_angle(current->car_rotate.y, previous->car_rotate.y, fraction);
	result->car_rotate.z = predict_angle(current->car_rotate.z, previous->car_rotate.z, fraction);
	result->car_steeringAngle =
		predict_word(current->car_steeringAngle, previous->car_steeringAngle, fraction);
	for (legacy_u16 wheel = 0; wheel < CARSTATE_WHEEL_COUNT; wheel++) {
		result->car_suspension_deflection[wheel] =
			predict_word(current->car_suspension_deflection[wheel],
						 previous->car_suspension_deflection[wheel], fraction);
	}
}

void frame_predict_state(struct GAMESTATE *result, const struct GAMESTATE *current,
						 const struct GAMESTATE *previous, legacy_u32 fraction)
{
	*result = *current;
	fraction = prediction_fraction(fraction);
	frame_predict_car(&result->playerstate, &current->playerstate, &previous->playerstate,
					  fraction);
	frame_predict_car(&result->opponentstate, &current->opponentstate, &previous->opponentstate,
					  fraction);
	for (legacy_u16 car = 0; car < GAMESTATE_CAR_VECTOR_COUNT; car++) {
		if (current->game_trackside_camera_index[car] ==
			previous->game_trackside_camera_index[car]) {
			frame_predict_vector(&result->game_follow_camera_position[car],
								 &current->game_follow_camera_position[car],
								 &previous->game_follow_camera_position[car], fraction);
		}
	}
	if (current->game_particles_active == 0 || previous->game_particles_active == 0) {
		return;
	}
	for (legacy_u16 part = 0; part < GAMESTATE_PARTICLE_SLOT_COUNT; part++) {
		if (current->game_particle_forward_speed[part] == 0 ||
			previous->game_particle_forward_speed[part] == 0 ||
			current->game_particle_owner[part] != previous->game_particle_owner[part] ||
			current->game_particle_shape_index[part] != previous->game_particle_shape_index[part]) {
			continue;
		}
		result->game_particle_x[part] = predict_position(current->game_particle_x[part],
														 previous->game_particle_x[part], fraction);
		result->game_particle_y[part] = predict_position(current->game_particle_y[part],
														 previous->game_particle_y[part], fraction);
		result->game_particle_z[part] = predict_position(current->game_particle_z[part],
														 previous->game_particle_z[part], fraction);
		result->game_particle_rotation_x[part] =
			predict_angle(current->game_particle_rotation_x[part],
						  previous->game_particle_rotation_x[part], fraction);
		result->game_particle_rotation_y[part] =
			predict_angle(current->game_particle_rotation_y[part],
						  previous->game_particle_rotation_y[part], fraction);
		result->game_particle_heading[part] = predict_angle(
			current->game_particle_heading[part], previous->game_particle_heading[part], fraction);
	}
}
