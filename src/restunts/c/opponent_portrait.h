#ifndef RESTUNTS_OPPONENT_PORTRAIT_H
#define RESTUNTS_OPPONENT_PORTRAIT_H

#include "legacy.h"

struct SPRITE;
struct SHAPE2D;

void opponent_portrait_draw(const struct SPRITE *target, const struct SHAPE2D *original,
							legacy_u8 opponent);
void opponent_portrait_unload(void);

#endif
