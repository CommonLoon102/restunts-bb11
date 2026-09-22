#ifndef RESTUNTS_FRAME_PREDICTION_H
#define RESTUNTS_FRAME_PREDICTION_H

#include "gamestate.h"

#define FRAME_PREDICTION_ONE 65536UL

/* Visual extrapolation from consecutive authoritative snapshots. No input,
 * timers, physics, or global state are accessed. Fractions are limited to one
 * simulation step; callers reset history after timeline discontinuities. */
void frame_predict_vector(struct VECTOR *result, const struct VECTOR *current,
						  const struct VECTOR *previous, legacy_u32 fraction);
void frame_predict_car(struct CARSTATE *result, const struct CARSTATE *current,
					   const struct CARSTATE *previous, legacy_u32 fraction);
void frame_predict_state(struct GAMESTATE *result, const struct GAMESTATE *current,
						 const struct GAMESTATE *previous, legacy_u32 fraction);

#endif
