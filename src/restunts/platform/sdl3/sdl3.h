#ifndef RESTUNTS_SDL3_H
#define RESTUNTS_SDL3_H

#include <SDL3/SDL.h>
#include "../../c/legacy.h"

extern legacy_s32 sdl3_batch_mode;

void sdl3_platform_pump(void);
void sdl3_platform_shutdown(void);
void sdl3_timer_pump(void);
void sdl3_audio_update(void);
void sdl3_input_shutdown(void);
void sdl3_video_shutdown(void);
void sdl3_video_present(void);
void sdl3_video_present_immediate(void);
void sdl3_video_toggle_fullscreen(void);
void sdl3_video_begin_frame(void);
void sdl3_video_end_frame(void);
void sdl3_video_refresh(void);
SDL_Window *sdl3_video_window(void);
void sdl3_video_window_to_game(legacy_f32 window_x, legacy_f32 window_y, legacy_f32 *x,
							   legacy_f32 *y);
void sdl3_video_game_to_window(legacy_f32 x, legacy_f32 y, legacy_f32 *window_x,
							   legacy_f32 *window_y);

#endif
