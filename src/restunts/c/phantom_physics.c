#include "phantom_physics.h"
#include "presentation.h"
#include "externs.h"
#include "state_internal.h"
#include "physics_internal.h"
#include "track_collision.h"
#include "wheel_transform.h"
#include "residue.h"
#include "game_input.h"
#include "crash_state.h"

#define PHANTOM_MAXIMUM_TICK_COUNT 2U

extern struct MATRIX wheel_heading_rotation;
extern struct MATRIX plane_heading_rotation;
extern legacy_s16 cached_plane_heading;
extern legacy_s16 cached_wheel_heading;

struct PHANTOM_SCRATCH {
	struct TRACK_COLLISION_SNAPSHOT collision;
	struct LEGACY_EXECUTION_RESIDUE residue;
	struct VECTORLONG position;
	struct VECTOR rotation;
	struct VECTOR initial_rotation;
	struct VECTOR forward;
	struct VECTOR world;
	struct MATRIX car_rotation;
	struct MATRIX plane_rotation;
	struct MATRIX wheel_rotation;
	legacy_s16 plane;
	legacy_s16 next_position;
	legacy_s16 heading;
	legacy_s16 plane_heading;
	legacy_s16 wheel_heading;
	legacy_s16 render_headings;
};

static void phantom_capture_scratch(struct PHANTOM_SCRATCH *saved)
{
	track_collision_capture(&saved->collision);
	saved->residue = legacy_execution_residue;
	saved->position = (struct VECTORLONG){car_working_x, car_working_y, car_working_z};
	saved->rotation = (struct VECTOR){car_working_yaw, car_working_pitch, car_working_roll};
	saved->initial_rotation = (struct VECTOR){car_initial_yaw, car_initial_pitch, car_initial_roll};
	saved->forward = wheel_forward_travel;
	saved->world = wheel_world_travel;
	saved->car_rotation = car_to_world_rotation;
	saved->plane_rotation = plane_heading_rotation;
	saved->wheel_rotation = wheel_heading_rotation;
	saved->plane = planindex_copy;
	saved->next_position = nextPosAndNormalIP;
	saved->heading = wheel_heading_offset;
	saved->plane_heading = cached_plane_heading;
	saved->wheel_heading = cached_wheel_heading;
	saved->render_headings = legacy_render_player_headings_active;
}

static void phantom_restore_scratch(const struct PHANTOM_SCRATCH *saved)
{
	track_collision_restore(&saved->collision);
	legacy_execution_residue = saved->residue;
	car_working_x = saved->position.lx;
	car_working_y = saved->position.ly;
	car_working_z = saved->position.lz;
	car_working_yaw = saved->rotation.x;
	car_working_pitch = saved->rotation.y;
	car_working_roll = saved->rotation.z;
	car_initial_yaw = saved->initial_rotation.x;
	car_initial_pitch = saved->initial_rotation.y;
	car_initial_roll = saved->initial_rotation.z;
	wheel_forward_travel = saved->forward;
	wheel_world_travel = saved->world;
	car_to_world_rotation = saved->car_rotation;
	plane_heading_rotation = saved->plane_rotation;
	wheel_heading_rotation = saved->wheel_rotation;
	planindex_copy = saved->plane;
	nextPosAndNormalIP = saved->next_position;
	wheel_heading_offset = saved->heading;
	cached_plane_heading = saved->plane_heading;
	cached_wheel_heading = saved->wheel_heading;
	legacy_render_player_headings_active = saved->render_headings;
}

static legacy_s32 phantom_blend(legacy_s32 previous, legacy_s32 updated, legacy_u32 fraction)
{
	return previous +
		   (legacy_s32)((legacy_s64)(updated - previous) * fraction / PHANTOM_PHYSICS_ONE);
}

static void phantom_grip(struct CARSTATE *car, struct SIMD *simd, legacy_s16 behavior,
						 legacy_u32 fraction20)
{
	struct CARSTATE before = *car;
	update_grip(car, simd, behavior);
	if (framespersec == GAME_FRAME_RATE_LOW) {
		fraction20 /= GAME_FRAME_RATE_NORMAL / GAME_FRAME_RATE_LOW;
	}
	/* Grip's contact classification and wheel direction are instantaneous.
	 * Its drag, heading decay and slide response evolve over the short step. */
	car->car_actual_speed =
		phantom_blend(before.car_actual_speed, car->car_actual_speed, fraction20);
	car->car_rev_speed = phantom_blend(before.car_rev_speed, car->car_rev_speed, fraction20);
	car->car_slide_yaw_delta =
		phantom_blend(before.car_slide_yaw_delta, car->car_slide_yaw_delta, fraction20);
	car->car_velocity_heading_offset = phantom_blend(before.car_velocity_heading_offset,
													 car->car_velocity_heading_offset, fraction20);
	car->car_rotate.x = phantom_blend(before.car_rotate.x, car->car_rotate.x, fraction20);
}

static void phantom_car_step(struct PHANTOM_PHYSICS *phantom, legacy_s16 index,
							 legacy_u32 fraction20)
{
	struct CARSTATE *car =
		index == PLAYER_CAR_INDEX ? &phantom->state.playerstate : &phantom->state.opponentstate;
	struct SIMD *simd = index == PLAYER_CAR_INDEX ? &simd_player : &simd_opponent;
	legacy_s8 input = phantom->input_flags;
	if (index == OPPONENT_CAR_INDEX) {
		input = car->car_is_accelerating ? INPUT_ACCELERATE_FLAG
										 : (car->car_is_braking ? INPUT_BRAKE_FLAG : INPUT_NONE);
	}
	if (car->car_crashBmpFlag != CRASH_EVENT_NONE) {
		input = INPUT_BRAKE_FLAG;
		if (car->car_actual_speed == CAR_SPEED_STOPPED && car->car_rev_speed == CAR_SPEED_STOPPED &&
			car->car_wheel_vertical_speed[0] == 0 && car->car_wheel_vertical_speed[1] == 0 &&
			car->car_wheel_vertical_speed[2] == 0 && car->car_wheel_vertical_speed[3] == 0) {
			return;
		}
	}
	update_car_speed_fraction(input, index, car, simd, fraction20);
	if (index == PLAYER_CAR_INDEX) {
		update_player_steering_fraction(
			car, ((legacy_u8)input >> INPUT_STEERING_SHIFT) & INPUT_PEDAL_MASK, fraction20);
	}
	phantom_grip(car, simd,
				 index == PLAYER_CAR_INDEX ? GRIP_BEHAVIOR_PLAYER : GRIP_BEHAVIOR_OPPONENT,
				 fraction20);
	update_player_state_fraction(&phantom->state, index, fraction20);
}

void phantom_physics_reset(struct PHANTOM_PHYSICS *phantom, const struct GAMESTATE *keyframe,
						   legacy_s8 input_flags)
{
	phantom->state = *keyframe;
	phantom->residue = legacy_execution_residue;
	phantom->elapsed20 = 0;
	phantom->input_flags = input_flags;
}

void phantom_physics_advance(struct PHANTOM_PHYSICS *phantom, legacy_u32 elapsed20)
{
	/* At most the next pair of 10 Hz ticks (fast replay). A delayed renderer
	 * must never run an unbounded speculative simulation. */
	if (elapsed20 > PHANTOM_MAXIMUM_TICK_COUNT * PHANTOM_PHYSICS_LOW_RATE_TICK) {
		elapsed20 = PHANTOM_MAXIMUM_TICK_COUNT * PHANTOM_PHYSICS_LOW_RATE_TICK;
	}
	if (elapsed20 <= phantom->elapsed20) {
		return;
	}
	struct PHANTOM_SCRATCH saved;
	phantom_capture_scratch(&saved);
	legacy_execution_residue = phantom->residue;
	legacy_render_player_headings_active = 0;
	const legacy_u32 maximum_step =
		PHANTOM_PHYSICS_ONE * GAME_FRAME_RATE_NORMAL / PRESENTATION_RATE;
	while (phantom->elapsed20 < elapsed20) {
		legacy_u32 step = elapsed20 - phantom->elapsed20;
		/* Never integrate more than one presentation interval at once, including skipped
		 * presentations and faster replay playback. The next step consumes
		 * this one's contact, suspension and velocity state. */
		if (step > maximum_step) {
			step = maximum_step;
		}
		phantom_car_step(phantom, PLAYER_CAR_INDEX, step);
		if (gameconfig.game_opponenttype != 0) {
			phantom_car_step(phantom, OPPONENT_CAR_INDEX, step);
		}
		update_follow_cameras_fraction(&phantom->state, step);
		phantom->elapsed20 += step;
	}
	phantom->residue = legacy_execution_residue;
	phantom_restore_scratch(&saved);
}
