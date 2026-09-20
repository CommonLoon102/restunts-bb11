#ifndef RESTUNTS_OWOOT_ROUTE_H
#define RESTUNTS_OWOOT_ROUTE_H

#include "legacy.h"

struct CARSTATE;

/* Call both checks every player tick, including ticks with a wheel over road.
 * Their progress lives in the car's reserved words, so checkpoints rewind it. */
legacy_s16 owoot_route_is_valid(struct CARSTATE *carstate, legacy_s16 allowed_jump);
legacy_s16 owoot_jump_is_valid(struct CARSTATE *carstate);

#endif
