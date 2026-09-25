#ifndef RESTUNTS_PHANTOM_PHYSICS_H
#define RESTUNTS_PHANTOM_PHYSICS_H

#include "gamestate.h"
#include "residue.h"

struct SIMD;

/* Q16 time in units of one 20 Hz physics tick. */
#define PHANTOM_PHYSICS_ONE 65536UL
#define PHANTOM_PHYSICS_LOW_RATE_TICK                                                              \
	(PHANTOM_PHYSICS_ONE * GAME_FRAME_RATE_NORMAL / GAME_FRAME_RATE_LOW)

struct PHANTOM_PHYSICS {
	struct GAMESTATE state;
	struct LEGACY_EXECUTION_RESIDUE residue;
	legacy_u32 elapsed20;
	legacy_s8 input_flags;
};

/* Reset at each authoritative tick or timeline change. Only the last consumed
 * input is retained; advancing this disposable branch never polls controls. */
void phantom_physics_reset(struct PHANTOM_PHYSICS *phantom, const struct GAMESTATE *keyframe,
						   legacy_s8 input_flags);
void phantom_physics_advance(struct PHANTOM_PHYSICS *phantom, legacy_u32 elapsed20);

/* Internal short-step entry points. The original tick functions remain intact. */
void update_player_state_fraction(struct GAMESTATE *branch, legacy_s16 car_index,
								  legacy_u32 fraction20);
legacy_s16 update_wheel_suspension_fraction(struct CARSTATE *carstate, legacy_s16 contact_delta,
											legacy_s16 wheel_index, legacy_u32 fraction20);
void update_car_speed_fraction(legacy_s8 input_flags, legacy_s16 car_index,
							   struct CARSTATE *carstate, struct SIMD *simd, legacy_u32 fraction20);
void update_player_steering_fraction(struct CARSTATE *carstate, legacy_s8 steering_input,
									 legacy_u32 fraction20);
void update_follow_cameras_fraction(struct GAMESTATE *branch, legacy_u32 fraction20);

#endif
