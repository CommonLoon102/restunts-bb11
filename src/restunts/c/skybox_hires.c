#include "skybox_hires.h"
#include "hires.h"
#include "shape2d.h"
#include "skybox.h"
#include "projection.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define SKYBOX_THEME_COUNT 5
#define SKYBOX_PATH_SIZE 1024
#define SKYBOX_PALETTE_COLOR_COUNT (LEGACY_U8_MAX + 1)
#define SKYBOX_PALETTE_CHANNEL_COUNT 3
#define SKYBOX_PALETTE_CHANNEL_MAX 63U
#define SKYBOX_NATIVE_SAMPLE_SCALE 1
#define SKYBOX_ORIGINAL_SAMPLE_SCALE HIRES_SCALE
#define SKYBOX_SAMPLE_ROUNDING_MARGIN 1

struct SKYBOX_HIRES_IMAGE {
	SDL_Surface *surface;
	legacy_s32 attempted;
};

static const legacy_char *theme_names[SKYBOX_THEME_COUNT] = {"desert", "tropical", "alpine", "city",
															 "country"};
static const legacy_char *image_names[SKYBOX_IMAGE_COUNT] = {"scen", "sce2", "sce3", "sce4"};
static struct SKYBOX_HIRES_IMAGE images[SKYBOX_IMAGE_COUNT];
static SDL_Color original_palette[SKYBOX_PALETTE_COLOR_COUNT];
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
	for (legacy_s32 index = 0; index < (legacy_s32)SKYBOX_PALETTE_COLOR_COUNT; index++) {
		original_palette[index].r =
			(palette[index * SKYBOX_PALETTE_CHANNEL_COUNT] & SKYBOX_PALETTE_CHANNEL_MAX) *
			SDL_ALPHA_OPAQUE / SKYBOX_PALETTE_CHANNEL_MAX;
		original_palette[index].g =
			(palette[index * SKYBOX_PALETTE_CHANNEL_COUNT + 1] & SKYBOX_PALETTE_CHANNEL_MAX) *
			SDL_ALPHA_OPAQUE / SKYBOX_PALETTE_CHANNEL_MAX;
		original_palette[index].b =
			(palette[index * SKYBOX_PALETTE_CHANNEL_COUNT + 2] & SKYBOX_PALETTE_CHANNEL_MAX) *
			SDL_ALPHA_OPAQUE / SKYBOX_PALETTE_CHANNEL_MAX;
		original_palette[index].a = SDL_ALPHA_OPAQUE;
	}
	palette_ready = 1;
}

static SDL_Surface *skybox_hires_load(const legacy_char *directory, legacy_s16 theme,
									  legacy_s16 image)
{
	legacy_char path[SKYBOX_PATH_SIZE];
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
		const legacy_char *base = SDL_GetBasePath();
		legacy_char directory[SKYBOX_PATH_SIZE];
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
	SDL_Palette *palette = SDL_CreatePalette(SKYBOX_PALETTE_COLOR_COUNT);
	if (palette != NULL &&
		SDL_SetPaletteColors(palette, original_palette, 0, SKYBOX_PALETTE_COLOR_COUNT)) {
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
	legacy_s32 scale = hires_render_scale();
	/* The ordinary horizon has already been drawn into the legacy target. */
	if (scale == HIRES_MINIMUM_SCALE || !hires_enabled() || !palette_ready || theme < 0 ||
		theme >= SKYBOX_THEME_COUNT || image < 0 || image >= SKYBOX_IMAGE_COUNT || width <= 0 ||
		width > SKYBOX_SCREEN_WIDTH || height <= 0 || height > SKYBOX_SCREEN_BOTTOM) {
		return;
	}
	legacy_s32 left = SDL_max(SDL_max(x, target->sprite_raster_left), 0);
	legacy_s32 right =
		SDL_min(SDL_min(x + width, target->sprite_raster_right), SKYBOX_SCREEN_WIDTH);
	legacy_s32 top = SDL_max(SDL_max(y, target->sprite_top), 0);
	legacy_s32 bottom = SDL_min(SDL_min(y + height, target->sprite_bottom), SKYBOX_SCREEN_BOTTOM);
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
	/* Retain authored artwork; sample destination-pixel centers at every scale. */
	legacy_s32 source_step = HIRES_SCALE / scale;
	for (legacy_s32 row = top; row < bottom; row++) {
		const legacy_u8 *pixels = (const legacy_u8 *)source->pixels +
								  (row - y) * HIRES_SCALE * source->pitch +
								  (left - x) * HIRES_SCALE;
		for (legacy_s32 column = left; column < right; column++) {
			legacy_u8 samples[HIRES_SCALE * HIRES_SCALE];
			for (legacy_s32 sample_y = 0; sample_y < scale; sample_y++) {
				const legacy_u8 *source_row =
					pixels + (sample_y * source_step + source_step / 2) * source->pitch;
				for (legacy_s32 sample_x = 0; sample_x < scale; sample_x++) {
					samples[sample_y * scale + sample_x] =
						source_row[sample_x * source_step + source_step / 2];
				}
			}
			hires_write_pixel(column, row, samples);
			pixels += HIRES_SCALE;
		}
	}
	hires_end();
}

struct SKYBOX_HIRES_STRIP {
	const legacy_u8 *pixels;
	legacy_s32 width, height, pitch, scale;
};

static legacy_s32 skybox_hires_prepare_strips(struct SKYBOX_HIRES_STRIP strips[SKYBOX_IMAGE_COUNT],
											  struct SHAPE2D *const shapes[SKYBOX_IMAGE_COUNT],
											  legacy_s16 theme, legacy_s32 scale)
{
	legacy_s32 maximum_height = 0;
	for (legacy_s32 index = 0; index < SKYBOX_IMAGE_COUNT; index++) {
		const struct SHAPE2D *shape = shapes[index];
		if (shape == NULL || shape->width == 0 || shape->width > SKYBOX_SCREEN_WIDTH ||
			shape->height == 0 || shape->height > SKYBOX_SCREEN_BOTTOM) {
			continue;
		}
		struct SKYBOX_HIRES_STRIP *strip = &strips[index];
		strip->pixels = (const legacy_u8 *)shape + SHAPE2D_HEADER_SIZE;
		strip->width = shape->width * HIRES_SCALE;
		strip->height = shape->height * HIRES_SCALE;
		strip->pitch = shape->width;
		strip->scale = SKYBOX_ORIGINAL_SAMPLE_SCALE;
		SDL_Surface *source = NULL;
		/* Keep cached enhanced artwork available for recovery; the smallest
		 * renderer uses the original pixels with the same modern projection. */
		if (scale != HIRES_MINIMUM_SCALE && palette_ready && theme >= 0 &&
			theme < SKYBOX_THEME_COUNT) {
			source = skybox_hires_image(theme, index, shape->width, shape->height);
		}
		if (source != NULL && source->w == strip->width && source->h == strip->height) {
			strip->pixels = source->pixels;
			strip->pitch = source->pitch;
			strip->scale = SKYBOX_NATIVE_SAMPLE_SCALE;
		}
		maximum_height = SDL_max(maximum_height, strip->height);
	}
	return maximum_height;
}

static legacy_u8 skybox_hires_sample(const struct SKYBOX_HIRES_STRIP strips[SKYBOX_IMAGE_COUNT],
									 legacy_f64 along, legacy_f64 above, legacy_u8 sky_color)
{
	static const legacy_s32 offsets[SKYBOX_IMAGE_COUNT] = {
		0, SKYBOX_IMAGE_WIDTH, SKYBOX_IMAGE_HALF_WRAP, SKYBOX_IMAGE_ONE_AND_HALF_WIDTH};
	/* Conversion truncates toward zero; correcting negative fractions gives
	 * floor without a library call for every panorama sample. */
	legacy_s32 column = (legacy_s32)along;
	column -= along < column;
	column = (legacy_u32)column & (SKYBOX_IMAGE_FULL_WRAP * HIRES_SCALE - 1);
	legacy_s32 index =
		column < (legacy_s32)SKYBOX_IMAGE_HALF_WRAP * HIRES_SCALE
			? (column < (legacy_s32)SKYBOX_IMAGE_WIDTH * HIRES_SCALE ? 0 : 1)
			: (column < (legacy_s32)SKYBOX_IMAGE_ONE_AND_HALF_WIDTH * HIRES_SCALE ? 2 : 3);
	const struct SKYBOX_HIRES_STRIP *strip = &strips[index];
	column -= offsets[index] * HIRES_SCALE;
	if (strip->pixels == NULL || column >= strip->width || above > strip->height) {
		return sky_color;
	}
	legacy_s32 row = (legacy_s32)(strip->height - above);
	if (strip->scale != SKYBOX_NATIVE_SAMPLE_SCALE) {
		row /= SKYBOX_ORIGINAL_SAMPLE_SCALE;
		column /= SKYBOX_ORIGINAL_SAMPLE_SCALE;
	}
	return strip->pixels[row * strip->pitch + column];
}

/* With an exactly level horizon, source columns do not change between rows
 * and source rows do not change between columns. Keep the original sums and
 * conversions so half-pixel boundaries select precisely the same texels. */
static void skybox_hires_render_level(const struct SPRITE *clip, const struct SKYBOX *scenery,
									  const struct SKYBOX_HIRES_STRIP strips[SKYBOX_IMAGE_COUNT],
									  legacy_s32 maximum_height, legacy_f64 normal_y,
									  legacy_f64 along_y, legacy_f64 above_x, legacy_f64 horizon,
									  legacy_f64 phase, legacy_f64 center_x, legacy_f64 center_y,
									  legacy_f64 cell_radius, legacy_s32 scale)
{
	legacy_s32 source_step = HIRES_SCALE / scale;
	enum { MISSING_STRIP = SKYBOX_IMAGE_COUNT, ROW_STRIP_COUNT = SKYBOX_IMAGE_COUNT + 1 };
	static const legacy_s32 offsets[SKYBOX_IMAGE_COUNT] = {
		0, SKYBOX_IMAGE_WIDTH, SKYBOX_IMAGE_HALF_WRAP, SKYBOX_IMAGE_ONE_AND_HALF_WIDTH};
	legacy_u8 column_strips[HIRES_WIDTH];
	legacy_u16 columns[HIRES_WIDTH];
	legacy_f64 relative_y = clip->sprite_top * HIRES_SCALE +
							HIRES_SAMPLE_CENTER_OFFSET * source_step - center_y * HIRES_SCALE;
	legacy_f64 row_along = phase * HIRES_SCALE + along_y * relative_y;
	legacy_f64 zero_above =
		above_x * (clip->sprite_raster_left * HIRES_SCALE +
				   HIRES_SAMPLE_CENTER_OFFSET * source_step - center_x * HIRES_SCALE);
	for (legacy_s32 x = clip->sprite_raster_left * scale; x < clip->sprite_raster_right * scale;
		 x++) {
		legacy_f64 relative_x =
			(x + HIRES_SAMPLE_CENTER_OFFSET) * source_step - center_x * HIRES_SCALE;
		legacy_f64 column_along = normal_y * relative_x;
		legacy_f64 along = column_along + row_along;
		legacy_s32 column = (legacy_s32)along;
		column -= along < column;
		column = (legacy_u32)column & (SKYBOX_IMAGE_FULL_WRAP * HIRES_SCALE - 1);
		legacy_s32 index =
			column < (legacy_s32)SKYBOX_IMAGE_HALF_WRAP * HIRES_SCALE
				? (column < (legacy_s32)SKYBOX_IMAGE_WIDTH * HIRES_SCALE ? 0 : 1)
				: (column < (legacy_s32)SKYBOX_IMAGE_ONE_AND_HALF_WIDTH * HIRES_SCALE ? 2 : 3);
		const struct SKYBOX_HIRES_STRIP *strip = &strips[index];
		column -= offsets[index] * HIRES_SCALE;
		if (strip->pixels == NULL || column >= strip->width) {
			column_strips[x] = MISSING_STRIP;
			columns[x] = 0;
			continue;
		}
		column_strips[x] = index;
		if (strip->scale != SKYBOX_NATIVE_SAMPLE_SCALE) {
			column /= SKYBOX_ORIGINAL_SAMPLE_SCALE;
		}
		columns[x] = column;
	}
	for (legacy_s32 y = clip->sprite_top; y < clip->sprite_bottom; y++) {
		legacy_f64 center_above =
			horizon -
			normal_y * ((y + HIRES_SAMPLE_CENTER_OFFSET) * HIRES_SCALE - center_y * HIRES_SCALE);
		legacy_f64 above_center =
			center_above +
			above_x * ((clip->sprite_raster_left + HIRES_SAMPLE_CENTER_OFFSET) * HIRES_SCALE -
					   center_x * HIRES_SCALE);
		if (above_center + cell_radius < 0 || above_center - cell_radius > maximum_height) {
			legacy_u8 color = above_center > 0 ? scenery->sky_color : scenery->ground_color;
			for (legacy_s32 x = clip->sprite_raster_left; x < clip->sprite_raster_right; x++) {
				hires_fill_pixel(x, y, color);
			}
			continue;
		}
		const legacy_u8 *sample_rows[HIRES_SCALE][ROW_STRIP_COUNT] = {0};
		legacy_u8 row_colors[HIRES_SCALE];
		for (legacy_s32 sample_y = 0; sample_y < scale; sample_y++) {
			legacy_f64 relative_y = y * HIRES_SCALE +
									(sample_y + HIRES_SAMPLE_CENTER_OFFSET) * source_step -
									center_y * HIRES_SCALE;
			legacy_f64 row_above = horizon - normal_y * relative_y;
			legacy_f64 above = zero_above + row_above;
			row_colors[sample_y] = above > 0 ? scenery->sky_color : scenery->ground_color;
			if (above <= 0 || above > maximum_height) {
				continue;
			}
			for (legacy_s32 index = 0; index < SKYBOX_IMAGE_COUNT; index++) {
				const struct SKYBOX_HIRES_STRIP *strip = &strips[index];
				if (strip->pixels == NULL || above > strip->height) {
					continue;
				}
				legacy_s32 row = (legacy_s32)(strip->height - above);
				if (strip->scale != SKYBOX_NATIVE_SAMPLE_SCALE) {
					row /= SKYBOX_ORIGINAL_SAMPLE_SCALE;
				}
				sample_rows[sample_y][index] = strip->pixels + row * strip->pitch;
			}
		}
		for (legacy_s32 x = clip->sprite_raster_left; x < clip->sprite_raster_right; x++) {
			legacy_u8 samples[HIRES_SCALE * HIRES_SCALE];
			for (legacy_s32 sample_y = 0; sample_y < scale; sample_y++) {
				for (legacy_s32 sample_x = 0; sample_x < scale; sample_x++) {
					legacy_s32 column = x * scale + sample_x;
					const legacy_u8 *row = sample_rows[sample_y][column_strips[column]];
					samples[sample_y * scale + sample_x] =
						row != NULL ? row[columns[column]] : row_colors[sample_y];
				}
			}
			hires_write_pixel(x, y, samples);
		}
	}
}

legacy_s32 skybox_hires_render(const struct SPRITE *target, const struct SKYBOX *scenery,
							   struct SHAPE2D *const shapes[SKYBOX_IMAGE_COUNT], legacy_s16 theme,
							   const struct MATRIX *rotation, legacy_s16 direction,
							   legacy_s16 angle, legacy_s16 camera_y, legacy_s16 detail)
{
	if (!hires_enabled() || projection_focal_length_x == 0 || projection_focal_length_y == 0) {
		return 0;
	}
	legacy_s32 scale = hires_render_scale();
	legacy_s32 source_step = HIRES_SCALE / scale;
	struct SPRITE clip = *target;
	clip.sprite_raster_left = SDL_min(clip.sprite_raster_left, SKYBOX_SCREEN_WIDTH);
	clip.sprite_raster_right = SDL_min(clip.sprite_raster_right, SKYBOX_SCREEN_WIDTH);
	clip.sprite_top = SDL_min(clip.sprite_top, SKYBOX_SCREEN_BOTTOM);
	clip.sprite_bottom = SDL_min(clip.sprite_bottom, SKYBOX_SCREEN_BOTTOM);
	if (clip.sprite_raster_left >= clip.sprite_raster_right ||
		clip.sprite_top >= clip.sprite_bottom) {
		return 0;
	}
	struct SKYBOX_HIRES_STRIP strips[SKYBOX_IMAGE_COUNT] = {0};
	legacy_s32 maximum_height = 0;
	if (detail != SKYBOX_LOWEST_DETAIL_LEVEL) {
		maximum_height = skybox_hires_prepare_strips(strips, shapes, theme, scale);
	}
	if (!hires_begin(&clip)) {
		return 0;
	}

	/* The original horizon lies in y + direction * camera_y / 15000 * z = 0.
	 * Rotate its normal, then sample the panorama in coordinates along and
	 * above that line. Undoing projection's unequal X/Y scales keeps banking
	 * aligned with the road. Unlike vertical image strips this also works at
	 * 90 and 180 degrees, without changing the level panorama's size. */
	legacy_f64 altitude = direction * (legacy_f64)camera_y / SKYBOX_ROLL_VECTOR_Z;
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
	/* A whole logical pixel can share one solid fill when all active
	 * samples are outside the artwork band. Keep boundary cells in the
	 * sample loop, including a one-sample margin for floating-point rounding. */
	legacy_f64 cell_radius = (HIRES_SCALE - source_step) * HIRES_SAMPLE_CENTER_OFFSET *
								 (SDL_fabs(above_x) + SDL_fabs(normal_y)) +
							 SKYBOX_SAMPLE_ROUNDING_MARGIN;
	if (normal_x == 0 && length > 0) {
		skybox_hires_render_level(&clip, scenery, strips, maximum_height, normal_y, along_y,
								  above_x, horizon, phase, center_x, center_y, cell_radius, scale);
		hires_end();
		return 1;
	}
	/* Projection is affine. Reuse each column's products across the whole
	 * viewport and each row's products across all cells, keeping the original
	 * sample arithmetic order at texel boundaries. */
	legacy_f64 column_above[HIRES_WIDTH];
	legacy_f64 column_along[HIRES_WIDTH];
	for (legacy_s32 x = clip.sprite_raster_left * scale; x < clip.sprite_raster_right * scale;
		 x++) {
		legacy_f64 relative_x =
			(x + HIRES_SAMPLE_CENTER_OFFSET) * source_step - center_x * HIRES_SCALE;
		column_above[x] = above_x * relative_x;
		column_along[x] = normal_y * relative_x;
	}
	for (legacy_s32 y = clip.sprite_top; y < clip.sprite_bottom; y++) {
		legacy_f64 center_above =
			horizon -
			normal_y * ((y + HIRES_SAMPLE_CENTER_OFFSET) * HIRES_SCALE - center_y * HIRES_SCALE);
		legacy_f64 row_above[HIRES_SCALE];
		legacy_f64 row_along[HIRES_SCALE];
		for (legacy_s32 sample_y = 0; sample_y < scale; sample_y++) {
			legacy_f64 relative_y = y * HIRES_SCALE +
									(sample_y + HIRES_SAMPLE_CENTER_OFFSET) * source_step -
									center_y * HIRES_SCALE;
			row_above[sample_y] = horizon - normal_y * relative_y;
			row_along[sample_y] = phase * HIRES_SCALE + along_y * relative_y;
		}
		for (legacy_s32 x = clip.sprite_raster_left; x < clip.sprite_raster_right; x++) {
			legacy_f64 above_center =
				center_above +
				above_x * ((x + HIRES_SAMPLE_CENTER_OFFSET) * HIRES_SCALE - center_x * HIRES_SCALE);
			if (length == 0 || above_center + cell_radius < 0 ||
				above_center - cell_radius > maximum_height) {
				hires_fill_pixel(x, y,
								 above_center > 0 ? scenery->sky_color : scenery->ground_color);
				continue;
			}
			legacy_u8 samples[HIRES_SCALE * HIRES_SCALE];
			for (legacy_s32 sample_y = 0; sample_y < scale; sample_y++) {
				for (legacy_s32 sample_x = 0; sample_x < scale; sample_x++) {
					legacy_s32 column = x * scale + sample_x;
					legacy_f64 above = column_above[column] + row_above[sample_y];
					legacy_u8 color = above > 0 ? scenery->sky_color : scenery->ground_color;
					/* Check height before integer conversion so near-pole
					 * coordinates never reach the texture sampler. */
					if (above > 0 && above <= maximum_height) {
						legacy_f64 along = column_along[column] + row_along[sample_y];
						color = skybox_hires_sample(strips, along, above, color);
					}
					samples[sample_y * scale + sample_x] = color;
				}
			}
			hires_write_pixel(x, y, samples);
		}
	}
	hires_end();
	return 1;
}
