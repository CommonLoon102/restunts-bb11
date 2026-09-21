#ifndef RESTUNTS_SDL3_H
#define RESTUNTS_SDL3_H

#include <SDL3/SDL.h>

extern int sdl3_batch_mode;

void sdl3_platform_pump(void);
void sdl3_platform_shutdown(void);
void sdl3_timer_pump(void);
void sdl3_audio_update(void);
void sdl3_input_shutdown(void);
void sdl3_video_shutdown(void);
void sdl3_video_present(void);
void sdl3_video_toggle_fullscreen(void);
void sdl3_video_begin_frame(void);
void sdl3_video_end_frame(void);
void sdl3_video_refresh(void);
SDL_Window *sdl3_video_window(void);
void sdl3_video_window_to_game(float window_x, float window_y, float *x, float *y);
void sdl3_video_game_to_window(float x, float y, float *window_x, float *window_y);

#endif
