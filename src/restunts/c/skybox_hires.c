#include "skybox_hires.h"
#include "hires.h"
#include "shape2d.h"
#include "skybox.h"
#include "projection.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define SKYBOX_THEME_COUNT 5
#define SKYBOX_IMAGE_COUNT 4
#define SKYBOX_PATH_SIZE 1024
#define SKYBOX_PANORAMA_WIDTH 1024
#define SKYBOX_HORIZON_DISTANCE 15000.0
#define SKYBOX_LOWEST_DETAIL_LEVEL 4

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

struct SKYBOX_HIRES_STRIP {
	const legacy_u8 *pixels;
	legacy_s32 width, height, pitch, scale;
};

static legacy_s32 skybox_hires_prepare_strips(struct SKYBOX_HIRES_STRIP strips[4],
											  struct SHAPE2D *const shapes[4], legacy_s16 theme)
{
	legacy_s32 maximum_height = 0;
	for (legacy_s32 index = 0; index < SKYBOX_IMAGE_COUNT; index++) {
		const struct SHAPE2D *shape = shapes[index];
		if (shape == NULL || shape->width == 0 || shape->width > 320 || shape->height == 0 ||
			shape->height > 200) {
			continue;
		}
		struct SKYBOX_HIRES_STRIP *strip = &strips[index];
		strip->pixels = (const legacy_u8 *)shape + SHAPE2D_HEADER_SIZE;
		strip->width = shape->width * HIRES_SCALE;
		strip->height = shape->height * HIRES_SCALE;
		strip->pitch = shape->width;
		strip->scale = HIRES_SCALE;
		SDL_Surface *source = NULL;
		if (palette_ready && theme >= 0 && theme < SKYBOX_THEME_COUNT) {
			source = skybox_hires_image(theme, index, shape->width, shape->height);
		}
		if (source != NULL && source->w == strip->width && source->h == strip->height) {
			strip->pixels = source->pixels;
			strip->pitch = source->pitch;
			strip->scale = 1;
		}
		maximum_height = SDL_max(maximum_height, strip->height);
	}
	return maximum_height;
}

static legacy_u8 skybox_hires_sample(const struct SKYBOX_HIRES_STRIP strips[4], legacy_f64 along,
									 legacy_f64 above, legacy_u8 sky_color)
{
	static const legacy_s32 offsets[SKYBOX_IMAGE_COUNT] = {0, 320, 512, 832};
	legacy_s32 column =
		(legacy_u32)(legacy_s32)SDL_floor(along) & (SKYBOX_PANORAMA_WIDTH * HIRES_SCALE - 1);
	legacy_s32 index = column < 512 * HIRES_SCALE ? (column < 320 * HIRES_SCALE ? 0 : 1)
												  : (column < 832 * HIRES_SCALE ? 2 : 3);
	const struct SKYBOX_HIRES_STRIP *strip = &strips[index];
	column -= offsets[index] * HIRES_SCALE;
	if (strip->pixels == NULL || column >= strip->width || above > strip->height) {
		return sky_color;
	}
	legacy_s32 row = (legacy_s32)(strip->height - above);
	return strip->pixels[(row / strip->scale) * strip->pitch + column / strip->scale];
}

legacy_s32 skybox_hires_render(const struct SPRITE *target, const struct SKYBOX *scenery,
							   struct SHAPE2D *const shapes[4], legacy_s16 theme,
							   const struct MATRIX *rotation, legacy_s16 direction,
							   legacy_s16 angle, legacy_s16 camera_y, legacy_s16 detail)
{
	if (!hires_enabled() || projection_focal_length_x == 0 || projection_focal_length_y == 0) {
		return 0;
	}
	struct SPRITE clip = *target;
	clip.sprite_raster_left = SDL_min(clip.sprite_raster_left, 320);
	clip.sprite_raster_right = SDL_min(clip.sprite_raster_right, 320);
	clip.sprite_top = SDL_min(clip.sprite_top, 200);
	clip.sprite_bottom = SDL_min(clip.sprite_bottom, 200);
	if (clip.sprite_raster_left >= clip.sprite_raster_right ||
		clip.sprite_top >= clip.sprite_bottom) {
		return 0;
	}
	struct SKYBOX_HIRES_STRIP strips[SKYBOX_IMAGE_COUNT] = {0};
	legacy_s32 maximum_height = 0;
	if (detail != SKYBOX_LOWEST_DETAIL_LEVEL) {
		maximum_height = skybox_hires_prepare_strips(strips, shapes, theme);
	}
	if (!hires_begin(&clip)) {
		return 0;
	}

	/* The original horizon lies in y + direction * camera_y / 15000 * z = 0.
	 * Rotate its normal, then sample the panorama in coordinates along and
	 * above that line. Undoing projection's unequal X/Y scales keeps banking
	 * aligned with the road. Unlike vertical image strips this also works at
	 * 90 and 180 degrees, without changing the level panorama's size. */
	legacy_f64 altitude = direction * (legacy_f64)camera_y / SKYBOX_HORIZON_DISTANCE;
	legacy_f64 normal_x = rotation->m._12 + altitude * rotation->m._13;
	legacy_f64 normal_y = rotation->m._22 + altitude * rotation->m._23;
	legacy_f64 normal_z = rotation->m._32 + altitude * rotation->m._33;
	legacy_f64 length = SDL_sqrt(normal_x * normal_x + normal_y * normal_y);
	legacy_f64 focal_x = projection_focal_length_x;
	legacy_f64 focal_y = projection_focal_length_y;
	legacy_f64 center_x = (legacy_s16)projection_center_x;
	legacy_f64 center_y = (legacy_s16)projection_center_y;
	legacy_f64 phase = center_x - ((angle + ANGLE_HALF_TURN) & ANGLE_MASK);
	if (direction < 0) {
		phase += ANGLE_HALF_TURN;
	}
	if (length > 0) {
		normal_x /= length;
		normal_y /= length;
		normal_z /= length;
	}
	/* Work directly in companion-pixel coordinates. Dividing each sample by
	 * a focal length and multiplying back can round exact texel boundaries
	 * down to the preceding column, even for a simple quarter turn. */
	legacy_f64 along_y = normal_x * focal_x / focal_y;
	legacy_f64 above_x = normal_x * focal_y / focal_x;
	legacy_f64 horizon = normal_z * focal_y * HIRES_SCALE;
	/* A whole logical pixel can share one solid fill when all sixteen
	 * samples are outside the artwork band. Keep boundary cells in the
	 * sample loop, including a one-sample margin for floating-point rounding. */
	legacy_f64 cell_radius = (HIRES_SCALE - 1) * 0.5 * (SDL_fabs(above_x) + SDL_fabs(normal_y)) + 1;
	for (legacy_s32 y = clip.sprite_top; y < clip.sprite_bottom; y++) {
		legacy_f64 center_above =
			horizon - normal_y * ((y + 0.5) * HIRES_SCALE - center_y * HIRES_SCALE);
		for (legacy_s32 x = clip.sprite_raster_left; x < clip.sprite_raster_right; x++) {
			legacy_f64 above_center =
				center_above + above_x * ((x + 0.5) * HIRES_SCALE - center_x * HIRES_SCALE);
			if (length == 0 || above_center + cell_radius < 0 ||
				above_center - cell_radius > maximum_height) {
				hires_fill_pixel(x, y,
								 above_center > 0 ? scenery->sky_color : scenery->ground_color);
				continue;
			}
			for (legacy_s32 sample_y = y * HIRES_SCALE; sample_y < (y + 1) * HIRES_SCALE;
				 sample_y++) {
				legacy_f64 relative_y = sample_y + 0.5 - center_y * HIRES_SCALE;
				legacy_f64 row_above = horizon - normal_y * relative_y;
				legacy_f64 row_along = phase * HIRES_SCALE + along_y * relative_y;
				for (legacy_s32 sample_x = x * HIRES_SCALE; sample_x < (x + 1) * HIRES_SCALE;
					 sample_x++) {
					legacy_f64 relative_x = sample_x + 0.5 - center_x * HIRES_SCALE;
					legacy_f64 above = above_x * relative_x + row_above;
					legacy_u8 color = above > 0 ? scenery->sky_color : scenery->ground_color;
					/* Check height before integer conversion so near-pole
					 * coordinates never reach the texture sampler. */
					if (above > 0 && above <= maximum_height) {
						legacy_f64 along = normal_y * relative_x + row_along;
						color = skybox_hires_sample(strips, along, above, color);
					}
					hires_pixel(sample_x, sample_y, color);
				}
			}
		}
	}
	hires_end();
	return 1;
}
