#ifndef RESTUNTS_GHOST_H
#define RESTUNTS_GHOST_H

#include "gamestate.h"

struct SIMD;

/* Camera state follows the source player at the cached replay frame. */
struct GHOST_CAMERA_STATE {
	struct VECTOR follow_position;
	struct VECTOR previous_position;
	legacy_s16 trackside_index;
	legacy_s16 frame;
	legacy_s16 crash_frame;
};

/* Selection never installs the replay as the player's current recording. */
legacy_s16 ghost_select_replay(const legacy_s8 *directory, const legacy_s8 *name);
legacy_s16 ghost_is_selected(void);
void ghost_clear(void);
void ghost_check_track(void);

/* Prepare before loading live race resources or installing the frame callback. */
legacy_s16 ghost_prepare_race(void);
void ghost_end_race(void);
legacy_s16 ghost_is_active(void);
void ghost_update(legacy_u32 frame, legacy_u16 live_frame_rate);
struct CARSTATE *ghost_car_state(void);
const struct GHOST_CAMERA_STATE *ghost_camera_state(void);
/* Convert recorded listener movement to one live audio tick, without file I/O. */
void ghost_adjust_camera_motion(struct VECTOR *previous, const struct VECTOR *current);
const struct SIMD *ghost_car_simd(void);
const legacy_s8 *ghost_car_id(void);
legacy_u8 ghost_car_material(void);

#endif
