#ifndef RESTUNTS_SKYBOX_HIRES_H
#define RESTUNTS_SKYBOX_HIRES_H

#include "legacy.h"

struct SPRITE;

/* Capture the original six-bit VGA palette, independently of later fades. */
void skybox_hires_set_palette(const legacy_u8 *palette);
void skybox_hires_unload(void);
void skybox_hires_draw(const struct SPRITE *target, legacy_s16 theme, legacy_s16 image,
					   legacy_s32 width, legacy_s32 height, legacy_s32 x, legacy_s32 y);

#endif
