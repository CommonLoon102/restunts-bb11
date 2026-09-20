#include "sdl3.h"
#include "../../c/platform.h"
#include "../../c/fatal.h"
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
static bool indexed_output;
static SDL_Color palette_colors[256];
static Uint32 palette_pixels[256];
static Uint32 converted_pixels[SCREEN_BYTES];
static unsigned char previous_pixels[SCREEN_BYTES];
static Uint64 last_present;
static bool palette_changed = true;
static bool drawing_frame;

static void video_fail(const char *operation)
{
	fprintf(stderr, "%s: %s\n", operation, SDL_GetError());
	dos_process_exit(1);
}

SDL_Window *sdl3_video_window(void)
{
	return window;
}

void sdl3_video_window_to_game(float window_x, float window_y, float *x, float *y)
{
	*x = window_x;
	*y = window_y;
	if (renderer != NULL) {
		SDL_RenderCoordinatesFromWindow(renderer, window_x, window_y, x, y);
		*y *= 200.0f / 240.0f;
	}
}

void sdl3_video_game_to_window(float x, float y, float *window_x, float *window_y)
{
	*window_x = x;
	*window_y = y;
	if (renderer != NULL) {
		SDL_RenderCoordinatesToWindow(renderer, x, y * (240.0f / 200.0f), window_x, window_y);
	}
}

void sdl3_video_present(void)
{
	if (window == NULL || drawing_frame) {
		return;
	}
	const unsigned char *pixels = dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0);
	if (indexed_output) {
		SDL_Surface *surface = SDL_GetWindowSurface(window);
		if (surface == NULL || surface->format != SDL_PIXELFORMAT_INDEX8) {
			video_fail("Get indexed video surface");
		}
		if (palette_changed &&
			!SDL_SetPaletteColors(SDL_GetSurfacePalette(surface), palette_colors, 0, 256)) {
			video_fail("Set video palette");
		}
		if (SDL_MUSTLOCK(surface) && !SDL_LockSurface(surface)) {
			video_fail("Lock video surface");
		}
		for (unsigned int row = 0; row < SCREEN_HEIGHT; row++) {
			memcpy((unsigned char *)surface->pixels + row * surface->pitch,
				   pixels + row * SCREEN_WIDTH, SCREEN_WIDTH);
		}
		if (SDL_MUSTLOCK(surface)) {
			SDL_UnlockSurface(surface);
		}
		if (!SDL_UpdateWindowSurface(window)) {
			video_fail("Present indexed video");
		}
	} else {
		for (unsigned int index = 0; index < SCREEN_BYTES; index++) {
			converted_pixels[index] = palette_pixels[pixels[index]];
		}
		if (!SDL_UpdateTexture(texture, NULL, converted_pixels, SCREEN_WIDTH * sizeof(Uint32)) ||
			!SDL_RenderClear(renderer) || !SDL_RenderTexture(renderer, texture, NULL, NULL) ||
			!SDL_RenderPresent(renderer)) {
			video_fail("Present video");
		}
	}
	memcpy(previous_pixels, pixels, SCREEN_BYTES);
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
		const unsigned char *pixels = dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0);
		if (palette_changed || memcmp(pixels, previous_pixels, SCREEN_BYTES) != 0) {
			sdl3_video_present();
		}
		/* Also throttle unchanged screens, including idle menus. */
		last_present = SDL_GetTicks();
	}
}

void sdl3_video_shutdown(void)
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	indexed_output = false;
	drawing_frame = false;
	texture = NULL;
	renderer = NULL;
	window = NULL;
}

void dos_video_set_mode_13h(void)
{
	if (sdl3_batch_mode) {
		memset(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0), 0, SCREEN_BYTES);
		return;
	}
	sdl3_video_shutdown();
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0 && !SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		video_fail("Initialize video");
	}
#ifdef __DJGPP__
	/* DOS Mode 13h displays 320x200 with the VGA's native 4:3 pixel aspect. */
	SDL_SetHint(SDL_HINT_DOS_ALLOW_DIRECT_FRAMEBUFFER, "1");
	window =
		SDL_CreateWindow("Chocolate Stunts", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
#else
	window = SDL_CreateWindow("Chocolate Stunts", 960, 720, SDL_WINDOW_RESIZABLE);
#endif
	if (window == NULL) {
		video_fail("Create game window");
	}
	memset(dos_memory_make_pointer(VGA_MEMORY_SEGMENT, 0), 0, SCREEN_BYTES);
#ifdef __DJGPP__
	/* The DOS surface format follows the selected display mode. Pin indexed
	 * Mode 13h explicitly; the default desktop mode can be true colour. */
	int mode_count;
	SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(SDL_GetPrimaryDisplay(), &mode_count);
	bool selected = false;
	if (modes != NULL) {
		for (int index = 0; index < mode_count; index++) {
			if (modes[index]->w == SCREEN_WIDTH && modes[index]->h == SCREEN_HEIGHT &&
				modes[index]->format == SDL_PIXELFORMAT_INDEX8) {
				selected = SDL_SetWindowFullscreenMode(window, modes[index]);
				break;
			}
		}
		SDL_free(modes);
	}
	if (!selected || !SDL_SyncWindow(window)) {
		video_fail("Select VGA Mode 13h");
	}
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	if (surface == NULL || surface->format != SDL_PIXELFORMAT_INDEX8 ||
		(SDL_GetSurfacePalette(surface) == NULL && SDL_CreateSurfacePalette(surface) == NULL)) {
		video_fail("Create indexed framebuffer");
	}
	indexed_output = true;
#else
	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		video_fail("Create renderer");
	}
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
								SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == NULL || !SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST) ||
		!SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, 240,
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
	for (unsigned int index = start; index < 256U && index < (unsigned int)start + count; index++) {
		SDL_Color *color = &palette_colors[index];
		color->r = (Uint8)((palette[0] & 63U) * 255U / 63U);
		color->g = (Uint8)((palette[1] & 63U) * 255U / 63U);
		color->b = (Uint8)((palette[2] & 63U) * 255U / 63U);
		color->a = 255;
		palette_pixels[index] =
			0xFF000000U | ((Uint32)color->r << 16) | ((Uint32)color->g << 8) | color->b;
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
