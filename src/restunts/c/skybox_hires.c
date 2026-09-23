#include "skybox_hires.h"
#include "hires.h"
#include "shape2d.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define SKYBOX_THEME_COUNT 5
#define SKYBOX_IMAGE_COUNT 4
#define SKYBOX_PATH_SIZE 1024

struct SKYBOX_HIRES_IMAGE {
	SDL_Surface *surface;
	legacy_s32 attempted;
};

static const char *theme_names[SKYBOX_THEME_COUNT] = {"desert", "tropical", "alpine", "city",
													  "country"};
static const char *image_names[SKYBOX_IMAGE_COUNT] = {"scen", "sce2", "sce3", "sce4"};
static struct SKYBOX_HIRES_IMAGE images[SKYBOX_IMAGE_COUNT];
static SDL_Color original_palette[256];
static legacy_s32 palette_ready;
static legacy_s16 loaded_theme = -1;

void skybox_hires_unload(void)
{
	for (legacy_s32 index = 0; index < SKYBOX_IMAGE_COUNT; index++) {
		SDL_DestroySurface(images[index].surface);
	}
	memset(images, 0, sizeof(images));
	loaded_theme = -1;
}

void skybox_hires_set_palette(const legacy_u8 *palette)
{
	skybox_hires_unload();
	for (legacy_s32 index = 0; index < 256; index++) {
		original_palette[index].r = (palette[index * 3] & 63U) * 255U / 63U;
		original_palette[index].g = (palette[index * 3 + 1] & 63U) * 255U / 63U;
		original_palette[index].b = (palette[index * 3 + 2] & 63U) * 255U / 63U;
		original_palette[index].a = 255;
	}
	palette_ready = 1;
}

static SDL_Surface *skybox_hires_load(const char *directory, legacy_s16 theme, legacy_s16 image)
{
	char path[SKYBOX_PATH_SIZE];
	legacy_s32 length = snprintf(path, sizeof(path), "%s%s-%s.png", directory, theme_names[theme],
								 image_names[image]);
	if (length < 0 || (size_t)length >= sizeof(path)) {
		return NULL;
	}
	SDL_Surface *source = SDL_LoadPNG(path);
	if (source == NULL) {
		/* Packaged names also work on DOS filesystems without long names. */
		length = snprintf(path, sizeof(path), "%ssky%d-%d.png", directory, theme, image);
		if (length >= 0 && (size_t)length < sizeof(path)) {
			source = SDL_LoadPNG(path);
		}
	}
	return source;
}

static SDL_Surface *skybox_hires_image(legacy_s16 theme, legacy_s16 image, legacy_s32 width,
									   legacy_s32 height)
{
	if (loaded_theme != theme) {
		skybox_hires_unload();
		loaded_theme = theme;
	}
	struct SKYBOX_HIRES_IMAGE *entry = &images[image];
	if (entry->attempted) {
		return entry->surface;
	}
	entry->attempted = 1;
	/* A data-directory override is useful for custom scenery. Installed games
	 * find the packaged images beside the executable, even with --data-dir. */
	SDL_Surface *source = skybox_hires_load("skyboxes/", theme, image);
	if (source == NULL) {
		const char *base = SDL_GetBasePath();
		char directory[SKYBOX_PATH_SIZE];
		if (base != NULL) {
			legacy_s32 length = snprintf(directory, sizeof(directory), "%sskyboxes/", base);
			if (length >= 0 && (size_t)length < sizeof(directory)) {
				source = skybox_hires_load(directory, theme, image);
			}
		}
	}
#ifdef RESTUNTS_SKYBOX_DIRECTORY
	if (source == NULL) {
		source = skybox_hires_load(RESTUNTS_SKYBOX_DIRECTORY "/", theme, image);
	}
#endif
	if (source == NULL) {
		return NULL;
	}
	if (source->w != width * HIRES_SCALE || source->h != height * HIRES_SCALE) {
		SDL_DestroySurface(source);
		return NULL;
	}
	SDL_Palette *palette = SDL_CreatePalette(256);
	if (palette != NULL && SDL_SetPaletteColors(palette, original_palette, 0, 256)) {
		entry->surface = SDL_ConvertSurfaceAndColorspace(source, SDL_PIXELFORMAT_INDEX8, palette,
														 SDL_COLORSPACE_SRGB, 0);
	}
	SDL_DestroyPalette(palette);
	SDL_DestroySurface(source);
	return entry->surface;
}

void skybox_hires_draw(const struct SPRITE *target, legacy_s16 theme, legacy_s16 image,
					   legacy_s32 width, legacy_s32 height, legacy_s32 x, legacy_s32 y)
{
	if (!hires_enabled() || !palette_ready || theme < 0 || theme >= SKYBOX_THEME_COUNT ||
		image < 0 || image >= SKYBOX_IMAGE_COUNT || width <= 0 || width > 320 || height <= 0 ||
		height > 200) {
		return;
	}
	legacy_s32 left = SDL_max(SDL_max(x, target->sprite_raster_left), 0);
	legacy_s32 right = SDL_min(SDL_min(x + width, target->sprite_raster_right), 320);
	legacy_s32 top = SDL_max(SDL_max(y, target->sprite_top), 0);
	legacy_s32 bottom = SDL_min(SDL_min(y + height, target->sprite_bottom), 200);
	if (left >= right || top >= bottom) {
		return;
	}
	struct SPRITE clip = *target;
	clip.sprite_raster_left = left;
	clip.sprite_raster_right = right;
	clip.sprite_top = top;
	clip.sprite_bottom = bottom;
	SDL_Surface *source = skybox_hires_image(theme, image, width, height);
	if (source == NULL || source->w != width * HIRES_SCALE || source->h != height * HIRES_SCALE ||
		!hires_begin(&clip)) {
		return;
	}
	left *= HIRES_SCALE;
	right *= HIRES_SCALE;
	top *= HIRES_SCALE;
	bottom *= HIRES_SCALE;
	for (legacy_s32 row = top; row < bottom; row++) {
		const legacy_u8 *pixels =
			(const legacy_u8 *)source->pixels + (row - y * HIRES_SCALE) * source->pitch;
		for (legacy_s32 column = left; column < right; column++) {
			hires_pixel(column, row, pixels[column - x * HIRES_SCALE]);
		}
	}
	hires_end();
}
