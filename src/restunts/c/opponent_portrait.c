#include "opponent_portrait.h"
#include "hires.h"
#include "shape2d.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define PORTRAIT_WIDTH 80
#define PORTRAIT_HEIGHT 83
#define PORTRAIT_LEFT 2
#define PORTRAIT_TOP 2
#define PORTRAIT_INNER_WIDTH 74
#define PORTRAIT_INNER_HEIGHT 79
#define PORTRAIT_DIGIT_COLOR 39
#define PORTRAIT_ORIGINAL_SCALE 2
#define PORTRAIT_PATH_SIZE 1024
#define PORTRAIT_MAX_DIMENSION 4096U

enum PORTRAIT_ARTWORK { PORTRAIT_MASTER = 0, PORTRAIT_PREPARED = 1, PORTRAIT_ORIGINAL_UPSCALE = 2 };

static SDL_Surface *portrait;
static legacy_u8 attempted_opponent;

void opponent_portrait_unload(void)
{
	SDL_DestroySurface(portrait);
	portrait = NULL;
	attempted_opponent = 0;
}

static legacy_u32 portrait_read_be32(const legacy_u8 *bytes)
{
	return ((legacy_u32)bytes[0] << 24) | ((legacy_u32)bytes[1] << 16) |
		   ((legacy_u32)bytes[2] << 8) | bytes[3];
}

static SDL_Surface *portrait_load(const char *directory, legacy_u8 opponent, legacy_u8 artwork)
{
	static const char *subdirectories[] = {"", "game/", "game2/"};
	char path[PORTRAIT_PATH_SIZE];
	legacy_s32 length =
		snprintf(path, sizeof(path), "%s%sopp%u.png", directory, subdirectories[artwork], opponent);
	if (length < 0 || (size_t)length >= sizeof(path)) {
		return NULL;
	}
	/* Check dimensions before decoding optional artwork, so a malformed or
	 * oversized replacement cannot trigger a large image allocation. */
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}
	legacy_u8 header[24];
	size_t read = fread(header, 1, sizeof(header), file);
	fclose(file);
	static const legacy_u8 signature[8] = {137, 'P', 'N', 'G', 13, 10, 26, 10};
	if (read != sizeof(header) || memcmp(header, signature, sizeof(signature)) != 0 ||
		portrait_read_be32(header + 8) != 13 || memcmp(header + 12, "IHDR", 4) != 0) {
		return NULL;
	}
	legacy_u32 width = portrait_read_be32(header + 16);
	legacy_u32 height = portrait_read_be32(header + 20);
	if (width == 0 || height == 0 || width > PORTRAIT_MAX_DIMENSION ||
		height > PORTRAIT_MAX_DIMENSION) {
		return NULL;
	}
	if (artwork == PORTRAIT_PREPARED && (width != PORTRAIT_INNER_WIDTH * HIRES_SCALE ||
										 height != PORTRAIT_INNER_HEIGHT * HIRES_SCALE)) {
		return NULL;
	}
	if (artwork == PORTRAIT_ORIGINAL_UPSCALE &&
		(width != PORTRAIT_WIDTH * PORTRAIT_ORIGINAL_SCALE ||
		 height != PORTRAIT_HEIGHT * PORTRAIT_ORIGINAL_SCALE)) {
		return NULL;
	}
	return SDL_LoadPNG(path);
}

static SDL_Surface *portrait_find(legacy_u8 opponent, legacy_u8 artwork)
{
	SDL_Surface *source = portrait_load("opponents/", opponent, artwork);
	if (source == NULL) {
		const char *base = SDL_GetBasePath();
		char directory[PORTRAIT_PATH_SIZE];
		if (base != NULL) {
			legacy_s32 length = snprintf(directory, sizeof(directory), "%sopponents/", base);
			if (length >= 0 && (size_t)length < sizeof(directory)) {
				source = portrait_load(directory, opponent, artwork);
			}
		}
	}
#ifdef RESTUNTS_OPPONENT_DIRECTORY
	if (source == NULL) {
		source = portrait_load(RESTUNTS_OPPONENT_DIRECTORY "/", opponent, artwork);
	}
#endif
	return source;
}

static SDL_Surface *portrait_convert(SDL_Surface *source, legacy_u8 artwork)
{
	if (artwork == PORTRAIT_PREPARED) {
		/* Preserve the prepared palette colors and pixel blocks exactly. */
		return SDL_ConvertSurface(source, SDL_PIXELFORMAT_ARGB8888);
	}
	SDL_Surface *result =
		SDL_CreateSurface(PORTRAIT_INNER_WIDTH * HIRES_SCALE, PORTRAIT_INNER_HEIGHT * HIRES_SCALE,
						  SDL_PIXELFORMAT_ARGB8888);
	SDL_Rect interior = {PORTRAIT_LEFT * PORTRAIT_ORIGINAL_SCALE,
						 PORTRAIT_TOP * PORTRAIT_ORIGINAL_SCALE,
						 PORTRAIT_INNER_WIDTH * PORTRAIT_ORIGINAL_SCALE,
						 PORTRAIT_INNER_HEIGHT * PORTRAIT_ORIGINAL_SCALE};
	legacy_u8 original_upscale = artwork == PORTRAIT_ORIGINAL_UPSCALE;
	/* game2 contains the complete doubled tile. Its interior keeps each pixel
	 * as a solid 2x2 block; the original frame and number are drawn by the menu. */
	if (result != NULL &&
		(!SDL_SetSurfaceBlendMode(source, SDL_BLENDMODE_NONE) ||
		 !SDL_BlitSurfaceScaled(source, original_upscale ? &interior : NULL, result, NULL,
								original_upscale ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR))) {
		SDL_DestroySurface(result);
		return NULL;
	}
	return result;
}

static SDL_Surface *portrait_image(legacy_u8 opponent)
{
	if (attempted_opponent == opponent) {
		return portrait;
	}
	opponent_portrait_unload();
	attempted_opponent = opponent;
	/* Search all roots for each tier: game2, game, then the master portrait. */
	for (legacy_s32 artwork = PORTRAIT_ORIGINAL_UPSCALE; artwork >= PORTRAIT_MASTER; artwork--) {
		SDL_Surface *source = portrait_find(opponent, (legacy_u8)artwork);
		if (source != NULL) {
			portrait = portrait_convert(source, (legacy_u8)artwork);
			SDL_DestroySurface(source);
			if (portrait != NULL) {
				return portrait;
			}
		}
	}
	return NULL;
}

void opponent_portrait_draw(const struct SPRITE *target, const struct SHAPE2D *original,
							legacy_u8 opponent)
{
	if (!hires_enabled() || opponent < 1 || opponent > 6 ||
		shape2d_get_width(original) != PORTRAIT_WIDTH ||
		shape2d_get_height(original) != PORTRAIT_HEIGHT) {
		return;
	}
	SDL_Surface *source = portrait_image(opponent);
	if (source == NULL || !hires_begin_argb(target)) {
		return;
	}
	legacy_s32 x = shape2d_get_pos_x(original) + PORTRAIT_LEFT;
	legacy_s32 y = shape2d_get_pos_y(original) + PORTRAIT_TOP;
	const legacy_u8 *indexed = (const legacy_u8 *)original + SHAPE2D_HEADER_SIZE;
	for (legacy_s32 row = 0; row < source->h; row++) {
		const legacy_u32 *pixels =
			(const legacy_u32 *)((const legacy_u8 *)source->pixels + row * source->pitch);
		for (legacy_s32 column = 0; column < source->w; column++) {
			legacy_s32 original_x = column / HIRES_SCALE + PORTRAIT_LEFT;
			legacy_s32 original_y = row / HIRES_SCALE + PORTRAIT_TOP;
			/* Keep the authored red number, including its exact pixel contours.
			 * Its surrounding photograph remains continuous through the mask. */
			if (original_x >= 66 && original_x <= 71 && original_y >= 4 && original_y <= 12 &&
				indexed[original_y * PORTRAIT_WIDTH + original_x] == PORTRAIT_DIGIT_COLOR) {
				continue;
			}
			hires_argb_pixel(x * HIRES_SCALE + column, y * HIRES_SCALE + row, pixels[column]);
		}
	}
	hires_end();
}
