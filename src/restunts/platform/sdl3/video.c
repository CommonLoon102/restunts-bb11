#include "sdl3.h"
#include "../../c/platform.h"
#include "../../c/fatal.h"
#include "../../c/hires.h"
#include <string.h>
#include <stdio.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_BYTES (SCREEN_WIDTH * SCREEN_HEIGHT)
#define VGA_MEMORY_SEGMENT 0xA000U
#define PRESENT_INTERVAL_MS 10U

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_Surface *frame_surface;
static legacy_s32 texture_width;
static legacy_s32 texture_height;
static legacy_u8 surface_output;
static legacy_u8 high_resolution_output;
static SDL_Rect surface_viewport;
static SDL_Color palette_colors[256];
static legacy_u32 palette_pixels[256];
static legacy_u8 previous_pixels[SCREEN_BYTES];
static legacy_u32 previous_generation;
static legacy_u64 last_present;
static legacy_u8 palette_changed = true;
static legacy_u8 drawing_frame;

static void video_fail(const char *operation)
{
	fprintf(stderr, "%s: %s\n", operation, SDL_GetError());
	dos_process_exit(1);
}

#ifdef __DJGPP__
static legacy_s32 mode_viewport_width(const SDL_DisplayMode *mode)
{
	/* The legacy VGA mode has nonsquare pixels; VESA modes use a 4:3 viewport. */
	if (mode->w == SCREEN_WIDTH && mode->h == SCREEN_HEIGHT) {
		return SCREEN_WIDTH;
	}
	return SDL_min(mode->w, mode->h * 4 / 3);
}

static legacy_u8 mode_is_better(const SDL_DisplayMode *candidate, const SDL_DisplayMode *current)
{
	if (current == NULL) {
		return true;
	}
	legacy_s32 candidate_width = mode_viewport_width(candidate);
	legacy_s32 current_width = mode_viewport_width(current);
	legacy_u8 candidate_fits = candidate_width >= HIRES_WIDTH;
	legacy_u8 current_fits = current_width >= HIRES_WIDTH;
	if (candidate_fits != current_fits) {
		return candidate_fits;
	}
	if (candidate_width != current_width) {
		return candidate_fits ? candidate_width < current_width : candidate_width > current_width;
	}
	legacy_s32 candidate_area = candidate->w * candidate->h;
	legacy_s32 current_area = current->w * current->h;
	if (candidate_area != current_area) {
		return candidate_area < current_area;
	}
	return SDL_BITSPERPIXEL(candidate->format) < SDL_BITSPERPIXEL(current->format);
}

static void select_dos_video_mode(legacy_u8 high_resolution)
{
	/* SDL writes a native int through this output pointer. */
	int mode_count;
	SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(SDL_GetPrimaryDisplay(), &mode_count);
	legacy_u8 selected = false;
	legacy_s32 selected_width = 0;
	if (modes != NULL) {
		for (legacy_s32 attempt = 0; attempt < mode_count && !selected; attempt++) {
			legacy_s32 best = -1;
			for (legacy_s32 index = 0; index < mode_count; index++) {
				const SDL_DisplayMode *mode = modes[index];
				if (mode == NULL || (SDL_ISPIXELFORMAT_INDEXED(mode->format) &&
									 mode->format != SDL_PIXELFORMAT_INDEX8)) {
					continue;
				}
				if (!high_resolution && (mode->w != SCREEN_WIDTH || mode->h != SCREEN_HEIGHT ||
										 mode->format != SDL_PIXELFORMAT_INDEX8)) {
					continue;
				}
				if (best < 0 || mode_is_better(mode, modes[best])) {
					best = index;
				}
			}
			if (best < 0) {
				break;
			}
			SDL_DestroyWindowSurface(window);
			selected = SDL_SetWindowFullscreenMode(window, modes[best]) && SDL_SyncWindow(window);
			selected_width = mode_viewport_width(modes[best]);
			modes[best] = NULL;
		}
		SDL_free(modes);
	}
	if (!selected) {
		video_fail("Select DOS video mode");
	}
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	if (surface == NULL) {
		video_fail("Create DOS framebuffer");
	}
	surface_viewport.x = 0;
	surface_viewport.y = 0;
	surface_viewport.w = surface->w;
	surface_viewport.h = surface->h;
	if (surface->w != SCREEN_WIDTH || surface->h != SCREEN_HEIGHT) {
		if (surface->w * 3 > surface->h * 4) {
			surface_viewport.w = surface->h * 4 / 3;
		} else {
			surface_viewport.h = surface->w * 3 / 4;
		}
		surface_viewport.x = (surface->w - surface_viewport.w) / 2;
		surface_viewport.y = (surface->h - surface_viewport.h) / 2;
	}
	if (high_resolution && selected_width < HIRES_WIDTH) {
		SDL_LogWarn(SDL_LOG_CATEGORY_VIDEO,
					"No VESA mode can display the full 1280x800 render at 4:3; scaling to %dx%d",
					surface_viewport.w, surface_viewport.h);
	}
	high_resolution_output = high_resolution;
	palette_changed = true;
}
#endif

SDL_Window *sdl3_video_window(void)
{
	return window;
}

void sdl3_video_toggle_fullscreen(void)
{
	/* DOS already uses native fullscreen modes. SDL saves and restores the
	 * desktop window's size and position; the logical 4:3 viewport persists. */
	if (window == NULL || surface_output) {
		return;
	}
	legacy_u8 fullscreen = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) != 0;
	if (!SDL_SetWindowFullscreen(window, !fullscreen)) {
		SDL_LogWarn(SDL_LOG_CATEGORY_VIDEO, "Cannot change fullscreen mode: %s", SDL_GetError());
	}
}

void sdl3_video_window_to_game(legacy_f32 window_x, legacy_f32 window_y, legacy_f32 *x,
							   legacy_f32 *y)
{
	*x = window_x;
	*y = window_y;
	if (renderer != NULL) {
		SDL_RenderCoordinatesFromWindow(renderer, window_x, window_y, x, y);
		*y *= 200.0f / 240.0f;
	} else if (surface_output) {
		*x = (window_x - surface_viewport.x) * SCREEN_WIDTH / surface_viewport.w;
		*y = (window_y - surface_viewport.y) * SCREEN_HEIGHT / surface_viewport.h;
	}
}

void sdl3_video_game_to_window(legacy_f32 x, legacy_f32 y, legacy_f32 *window_x,
							   legacy_f32 *window_y)
{
	*window_x = x;
	*window_y = y;
	if (renderer != NULL) {
		SDL_RenderCoordinatesToWindow(renderer, x, y * (240.0f / 200.0f), window_x, window_y);
	} else if (surface_output) {
		*window_x = surface_viewport.x + x * surface_viewport.w / SCREEN_WIDTH;
		*window_y = surface_viewport.y + y * surface_viewport.h / SCREEN_HEIGHT;
	}
}

static void present_surface(const legacy_u8 *pixels, const legacy_u32 *argb, legacy_s32 width,
							legacy_s32 height)
{
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	if (surface == NULL) {
		video_fail("Get video surface");
	}
	const void *frame_pixels = argb != NULL ? (const void *)argb : pixels;
	SDL_PixelFormat format = argb != NULL ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_INDEX8;
	legacy_u8 new_frame_surface = frame_surface == NULL || frame_surface->pixels != frame_pixels ||
								  frame_surface->format != format || frame_surface->w != width ||
								  frame_surface->h != height;
	if (new_frame_surface) {
		SDL_DestroySurface(frame_surface);
		frame_surface = SDL_CreateSurfaceFrom(width, height, format, (void *)frame_pixels,
											  width * SDL_BYTESPERPIXEL(format));
		if (frame_surface == NULL ||
			(argb == NULL && SDL_CreateSurfacePalette(frame_surface) == NULL)) {
			video_fail("Create presentation surface");
		}
	}
	SDL_Palette *palette = SDL_GetSurfacePalette(frame_surface);
	if (palette != NULL && (palette_changed || new_frame_surface) &&
		!SDL_SetPaletteColors(palette, palette_colors, 0, 256)) {
		video_fail("Set video palette");
	}
	if (surface->format == SDL_PIXELFORMAT_INDEX8) {
		if (palette != NULL) {
			if (SDL_GetSurfacePalette(surface) != palette &&
				!SDL_SetSurfacePalette(surface, palette)) {
				video_fail("Set framebuffer palette");
			}
		} else {
			SDL_Palette *output_palette = SDL_GetSurfacePalette(surface);
			if (output_palette == NULL) {
				output_palette = SDL_CreateSurfacePalette(surface);
			}
			if (output_palette == NULL || !SDL_SetPaletteColors(output_palette, palette_colors, 0,
																SDL_arraysize(palette_colors))) {
				video_fail("Set framebuffer palette");
			}
		}
	}
	if ((surface_viewport.w != surface->w || surface_viewport.h != surface->h) &&
		!SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGB(surface, 0, 0, 0))) {
		video_fail("Clear video borders");
	}
	if (!SDL_BlitSurfaceScaled(frame_surface, NULL, surface, &surface_viewport,
							   SDL_SCALEMODE_NEAREST) ||
		!SDL_UpdateWindowSurface(window)) {
		video_fail("Present video surface");
	}
}

static void present_texture(const legacy_u8 *pixels, const legacy_u32 *argb, legacy_s32 width,
							legacy_s32 height)
{
	if (texture == NULL || texture_width != width || texture_height != height) {
		SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
									width, height);
		if (texture == NULL || !SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
			video_fail("Create presentation texture");
		}
		texture_width = width;
		texture_height = height;
	}
	void *texture_pixels;
	/* SDL writes a native int through this output pointer. */
	int pitch;
	if (!SDL_LockTexture(texture, NULL, &texture_pixels, &pitch)) {
		video_fail("Lock video texture");
	}
	for (legacy_s32 row = 0; row < height; row++) {
		legacy_u32 *destination = (legacy_u32 *)((legacy_u8 *)texture_pixels + row * pitch);
		if (argb != NULL) {
			memcpy(destination, argb + row * width, (size_t)width * sizeof(*destination));
			continue;
		}
		const legacy_u8 *source = pixels + row * width;
		for (legacy_s32 column = 0; column < width; column++) {
			destination[column] = palette_pixels[source[column]];
		}
	}
	SDL_UnlockTexture(texture);
	if (!SDL_RenderClear(renderer) || !SDL_RenderTexture(renderer, texture, NULL, NULL) ||
		!SDL_RenderPresent(renderer)) {
		video_fail("Present video");
	}
}

void sdl3_video_present(void)
{
	if (window == NULL || drawing_frame) {
		return;
	}
#ifdef __DJGPP__
	if (high_resolution_output != (hires_enabled() != 0)) {
		select_dos_video_mode(hires_enabled() != 0);
	}
#endif
	const legacy_u8 *legacy_pixels = dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0);
	legacy_s32 width;
	legacy_s32 height;
	const legacy_u8 *pixels = hires_framebuffer(legacy_pixels, &width, &height);
	const legacy_u32 *argb = hires_framebuffer_argb(legacy_pixels, palette_pixels);
	if (surface_output) {
		present_surface(pixels, argb, width, height);
	} else {
		present_texture(pixels, argb, width, height);
	}
	memcpy(previous_pixels, legacy_pixels, SCREEN_BYTES);
	previous_generation = hires_generation();
	palette_changed = false;
	last_present = SDL_GetTicks();
}

void sdl3_video_begin_frame(void)
{
	drawing_frame = true;
}

void sdl3_video_end_frame(void)
{
	drawing_frame = false;
	sdl3_video_present();
}

void sdl3_video_refresh(void)
{
	if (window != NULL && !drawing_frame && SDL_GetTicks() - last_present >= PRESENT_INTERVAL_MS) {
		const legacy_u8 *pixels = dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0);
		if (palette_changed || hires_generation() != previous_generation ||
			memcmp(pixels, previous_pixels, SCREEN_BYTES) != 0) {
			sdl3_video_present();
		}
		/* Also throttle unchanged screens, including idle menus. */
		last_present = SDL_GetTicks();
	}
}

void sdl3_video_shutdown(void)
{
	SDL_DestroySurface(frame_surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	surface_output = false;
	high_resolution_output = false;
	drawing_frame = false;
	frame_surface = NULL;
	texture = NULL;
	texture_width = 0;
	texture_height = 0;
	renderer = NULL;
	window = NULL;
}

void dos_video_set_mode_13h(void)
{
	if (sdl3_batch_mode) {
		hires_forget(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0));
		memset(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0), 0, SCREEN_BYTES);
		return;
	}
	sdl3_video_shutdown();
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0 && !SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		video_fail("Initialize video");
	}
#ifdef __DJGPP__
	/* The direct framebuffer supports indexed and truecolour VESA modes. */
	SDL_SetHint(SDL_HINT_DOS_ALLOW_DIRECT_FRAMEBUFFER, "1");
	window =
		SDL_CreateWindow("Chocolate Stunts", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
#else
	window = SDL_CreateWindow("Chocolate Stunts", 960, 720, SDL_WINDOW_RESIZABLE);
#endif
	if (window == NULL) {
		video_fail("Create game window");
	}
	hires_forget(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0));
	memset(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0), 0, SCREEN_BYTES);
#ifdef __DJGPP__
	select_dos_video_mode(hires_enabled() != 0);
	surface_output = true;
#else
	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		video_fail("Create renderer");
	}
	if (!SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, 240,
										  SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
		video_fail("Configure framebuffer scaling");
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
#endif
	SDL_HideCursor();
	palette_changed = true;
	sdl3_video_present();
}

legacy_s16 dos_video_get_status(void)
{
	/* Preserve polling loops that wait for both phases of VGA retrace. */
	sdl3_platform_pump();
	return (SDL_GetTicks() % 14U) < 2U ? 8 : 0;
}

legacy_s16 video_get_status(void)
{
	return dos_video_get_status();
}

void dos_video_set_palette(legacy_u16 start, legacy_u16 count, legacy_u8 *palette)
{
	for (legacy_u32 index = start; index < 256U && index < (legacy_u32)start + count; index++) {
		SDL_Color *color = &palette_colors[index];
		color->r = (legacy_u8)((palette[0] & 63U) * 255U / 63U);
		color->g = (legacy_u8)((palette[1] & 63U) * 255U / 63U);
		color->b = (legacy_u8)((palette[2] & 63U) * 255U / 63U);
		color->a = 255;
		palette_pixels[index] =
			0xFF000000U | ((legacy_u32)color->r << 16) | ((legacy_u32)color->g << 8) | color->b;
		palette += 3;
	}
	palette_changed = true;
}

legacy_u8 dos_video_enable_planar_pages(void)
{
	/* SDL consumes the game's packed framebuffer; VGA planes are unnecessary. */
	return 0;
}

void dos_video_set_write_planes(legacy_u8 mask)
{
	(void)mask;
}

void dos_video_set_read_plane(legacy_u8 plane)
{
	(void)plane;
}

void dos_video_show_page(legacy_u16 address)
{
	(void)address;
	sdl3_video_present();
}

void dos_video_set_mode4(void)
{
	dos_video_set_mode_13h();
}

void dos_video_set_mode7(void)
{
	sdl3_video_shutdown();
}
