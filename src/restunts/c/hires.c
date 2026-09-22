#include "hires.h"
#include "platform.h"
#include "shape2d.h"
#include "shape2d_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>

#define HIRES_ADDRESS_COUNT 65536UL
#define HIRES_CELL_PIXELS (HIRES_SCALE * HIRES_SCALE)

/* Each legacy pixel may carry sixteen independently rasterized samples.
 * Keeping these attached to byte offsets lets ordinary clipped sprite copies,
 * saved backgrounds, and dashboard masks carry their detail with them. */
struct HIRES_SURFACE {
	const legacy_u8 *base;
	legacy_u8 *pixels;
	legacy_u8 valid[HIRES_ADDRESS_COUNT];
	struct HIRES_SURFACE *next;
};

static struct HIRES_SURFACE *surfaces;
static struct HIRES_SURFACE *active;
static struct SPRITE active_sprite;
static legacy_s32 enabled;
static legacy_u32 generation;
static legacy_u8 *framebuffer;
static legacy_f32 *inverse_depth;
static legacy_u16 *depth_family;
static legacy_s32 depth_left, depth_right, depth_top, depth_bottom;

static void *hires_allocate(size_t size)
{
	void *result = calloc(1, size);
	if (result == NULL) {
		fputs("Cannot allocate high-resolution framebuffer\n", stderr);
		dos_process_exit(1);
	}
	return result;
}

static struct HIRES_SURFACE *hires_find(const legacy_u8 *base)
{
	for (struct HIRES_SURFACE *surface = surfaces; surface != NULL; surface = surface->next) {
		if (surface->base == base) {
			return surface;
		}
	}
	return NULL;
}

static struct HIRES_SURFACE *hires_create(const legacy_u8 *base)
{
	struct HIRES_SURFACE *surface = hires_find(base);
	if (surface == NULL) {
		surface = hires_allocate(sizeof(*surface));
		surface->base = base;
		surface->pixels = hires_allocate(HIRES_ADDRESS_COUNT * HIRES_CELL_PIXELS);
		surface->next = surfaces;
		surfaces = surface;
	}
	return surface;
}

static legacy_u8 *hires_cell(struct HIRES_SURFACE *surface, legacy_u16 offset)
{
	legacy_u8 *cell = surface->pixels + (size_t)offset * HIRES_CELL_PIXELS;
	if (!surface->valid[offset]) {
		memset(cell, surface->base[offset], HIRES_CELL_PIXELS);
		surface->valid[offset] = 1;
	}
	return cell;
}

static void hires_depth_reset(void)
{
	depth_left = depth_right = depth_top = depth_bottom = 0;
}

void hires_shutdown(void)
{
	while (surfaces != NULL) {
		struct HIRES_SURFACE *next = surfaces->next;
		free(surfaces->pixels);
		free(surfaces);
		surfaces = next;
	}
	free(framebuffer);
	framebuffer = NULL;
	free(inverse_depth);
	free(depth_family);
	inverse_depth = NULL;
	depth_family = NULL;
	hires_depth_reset();
	active = NULL;
	enabled = 0;
	generation++;
}

void hires_set_enabled(legacy_s32 value)
{
	value = value != 0;
	if (value == enabled) {
		return;
	}
	hires_shutdown();
	enabled = value;
}

legacy_s32 hires_enabled(void)
{
	return enabled;
}

legacy_u32 hires_generation(void)
{
	return generation;
}

legacy_s32 hires_begin(const struct SPRITE *target)
{
	if (!enabled || active != NULL) {
		return 0;
	}
	const legacy_u8 *base =
		dos_memory_make_pointer(dos_memory_pointer_segment(target->sprite_bitmapptr), 0);
	active = hires_create(base);
	active_sprite = *target;
	hires_depth_reset();
	/* Seed the background before the legacy 3D pass overwrites its pixels. */
	for (legacy_u32 y = target->sprite_top; y < target->sprite_bottom && y < 200; y++) {
		legacy_u16 row = LEGACY_READ_U16_LE(target->sprite_lineofs + y * 2);
		for (legacy_u32 x = target->sprite_raster_left; x < target->sprite_raster_right && x < 320;
			 x++) {
			hires_cell(active, (legacy_u16)(row + x));
		}
	}
	return 1;
}

void hires_end(void)
{
	hires_depth_reset();
	if (active != NULL) {
		active = NULL;
		generation++;
	}
}

/* Every shape keeps the existing scene painter order, but resolves its own
 * overlapping surfaces by depth. Clear only the current projected bounds;
 * family zero invalidates an old depth without clearing another float array. */
void hires_depth_begin(legacy_s32 left, legacy_s32 right, legacy_s32 top, legacy_s32 bottom)
{
	hires_depth_reset();
	if (active == NULL) {
		return;
	}
	legacy_s32 clip_left = active_sprite.sprite_raster_left * HIRES_SCALE;
	legacy_s32 clip_right = active_sprite.sprite_raster_right * HIRES_SCALE;
	legacy_s32 clip_top = active_sprite.sprite_top * HIRES_SCALE;
	legacy_s32 clip_bottom = active_sprite.sprite_bottom * HIRES_SCALE;
	if (clip_left < 0) {
		clip_left = 0;
	}
	if (clip_right > HIRES_WIDTH) {
		clip_right = HIRES_WIDTH;
	}
	if (clip_top < 0) {
		clip_top = 0;
	}
	if (clip_bottom > HIRES_HEIGHT) {
		clip_bottom = HIRES_HEIGHT;
	}
	if (left < clip_left) {
		left = clip_left;
	}
	if (right > clip_right) {
		right = clip_right;
	}
	if (top < clip_top) {
		top = clip_top;
	}
	if (bottom > clip_bottom) {
		bottom = clip_bottom;
	}
	if (left >= right || top >= bottom) {
		return;
	}
	if (inverse_depth == NULL) {
		inverse_depth = hires_allocate((size_t)HIRES_WIDTH * HIRES_HEIGHT * sizeof(*inverse_depth));
		depth_family = hires_allocate((size_t)HIRES_WIDTH * HIRES_HEIGHT * sizeof(*depth_family));
	}
	depth_left = left;
	depth_right = right;
	depth_top = top;
	depth_bottom = bottom;
	for (legacy_s32 y = top; y < bottom; y++) {
		memset(depth_family + (size_t)y * HIRES_WIDTH + left, 0,
			   (size_t)(right - left) * sizeof(*depth_family));
	}
}

legacy_s32 hires_depth_test(legacy_s32 x, legacy_s32 y, legacy_f64 inverse_z, legacy_u16 family,
							legacy_s32 attached)
{
	if (active == NULL || x < depth_left || x >= depth_right || y < depth_top ||
		y >= depth_bottom || !(inverse_z > 0) || inverse_z > FLT_MAX || family == 0) {
		return 0;
	}
	size_t index = (size_t)y * HIRES_WIDTH + x;
	if (attached && depth_family[index] == family) {
		/* Resource overlays may sit slightly behind their parent. Keep the
		 * parent's occlusion depth while allowing its authored paint order. */
		return 1;
	}
	legacy_f32 depth = (legacy_f32)inverse_z;
	if (depth_family[index] != 0 &&
		depth + 4 * FLT_EPSILON * inverse_depth[index] < inverse_depth[index]) {
		return 0;
	}
	inverse_depth[index] = depth;
	depth_family[index] = family;
	return 1;
}

void hires_pixel(legacy_s32 x, legacy_s32 y, legacy_u8 color)
{
	if (active == NULL || x < 0 || y < 0 || x >= HIRES_WIDTH || y >= HIRES_HEIGHT ||
		x < active_sprite.sprite_raster_left * HIRES_SCALE ||
		x >= active_sprite.sprite_raster_right * HIRES_SCALE ||
		y < active_sprite.sprite_top * HIRES_SCALE ||
		y >= active_sprite.sprite_bottom * HIRES_SCALE) {
		return;
	}
	legacy_u16 row = LEGACY_READ_U16_LE(active_sprite.sprite_lineofs + (y / HIRES_SCALE) * 2);
	legacy_u8 *cell = hires_cell(active, (legacy_u16)(row + x / HIRES_SCALE));
	cell[(y % HIRES_SCALE) * HIRES_SCALE + x % HIRES_SCALE] = color;
}

void hires_write(const legacy_u8 *base, legacy_u16 offset, legacy_u8 color)
{
	(void)color;
	if (!enabled || active != NULL) {
		return;
	}
	struct HIRES_SURFACE *surface = hires_find(base);
	if (surface != NULL && surface->valid[offset]) {
		/* A 2D write replaces the whole pixel even when its colour is unchanged. */
		surface->valid[offset] = 0;
		generation++;
	}
}

void hires_raster(const legacy_u8 *destination, legacy_u16 destination_offset,
				  const legacy_u8 *source, legacy_u16 source_offset, legacy_u16 count,
				  legacy_s16 operation, const legacy_u8 *palette)
{
	if (!enabled || active != NULL || count == 0) {
		return;
	}
	struct HIRES_SURFACE *src = hires_find(source);
	struct HIRES_SURFACE *dst = hires_find(destination);
	if (src == NULL && dst == NULL) {
		return;
	}
	if (dst == NULL) {
		dst = hires_create(destination);
	}
	for (legacy_u32 index = 0; index < count; index++) {
		legacy_u16 so = (legacy_u16)(source_offset + index);
		legacy_u16 dest = (legacy_u16)(destination_offset + index);
		legacy_u8 source_color = source[so];
		legacy_u8 *cell = hires_cell(dst, dest);
		/* Match the game's forward traversal, including overlapping copies. */
		legacy_u8 samples[HIRES_CELL_PIXELS];
		if (src != NULL && src->valid[so]) {
			memcpy(samples, src->pixels + (size_t)so * HIRES_CELL_PIXELS, sizeof(samples));
		} else {
			memset(samples, source_color, sizeof(samples));
		}
		for (legacy_s32 sample = 0; sample < HIRES_CELL_PIXELS; sample++) {
			legacy_u8 value = samples[sample];
			if (operation == SHAPE2D_RASTER_AND) {
				cell[sample] &= value;
			} else if (operation == SHAPE2D_RASTER_OR) {
				cell[sample] |= value;
			} else if (operation == SHAPE2D_RASTER_MAP) {
				value = palette[value];
				if (value != 255) {
					cell[sample] = value;
				}
			} else {
				cell[sample] = value;
			}
		}
	}
	generation++;
}

void hires_forget(const void *base)
{
	struct HIRES_SURFACE **link = &surfaces;
	while (*link != NULL) {
		struct HIRES_SURFACE *surface = *link;
		if (surface->base == base) {
			if (surface == active) {
				active = NULL;
			}
			*link = surface->next;
			free(surface->pixels);
			free(surface);
			generation++;
			return;
		}
		link = &surface->next;
	}
}

void hires_forget_range(const void *base, legacy_u32 size)
{
	if (!enabled || size == 0) {
		return;
	}
	uintptr_t start = (uintptr_t)base;
	uintptr_t end = start + size;
	struct HIRES_SURFACE **link = &surfaces;
	while (*link != NULL) {
		struct HIRES_SURFACE *surface = *link;
		uintptr_t address = (uintptr_t)surface->base;
		if (address >= start && address < end) {
			if (surface == active) {
				active = NULL;
			}
			*link = surface->next;
			free(surface->pixels);
			free(surface);
			generation++;
			continue;
		}
		if (address < start && start - address < HIRES_ADDRESS_COUNT) {
			/* Paragraph aliases may start before the released bytes. Do not
			 * discard neighbouring live sprites that share the 64 KiB window. */
			size_t first = start - address;
			size_t last = end - address;
			if (last > HIRES_ADDRESS_COUNT) {
				last = HIRES_ADDRESS_COUNT;
			}
			memset(surface->valid + first, 0, last - first);
			generation++;
		}
		link = &surface->next;
	}
}

const legacy_u8 *hires_framebuffer(const legacy_u8 *legacy, legacy_s32 *width, legacy_s32 *height)
{
	*width = enabled ? HIRES_WIDTH : 320;
	*height = enabled ? HIRES_HEIGHT : 200;
	if (!enabled) {
		return legacy;
	}
	if (framebuffer == NULL) {
		framebuffer = hires_allocate(HIRES_WIDTH * HIRES_HEIGHT);
	}
	struct HIRES_SURFACE *surface = hires_find(legacy);
	for (legacy_s32 y = 0; y < 200; y++) {
		for (legacy_s32 x = 0; x < 320; x++) {
			legacy_u32 offset = y * 320 + x;
			const legacy_u8 *cell = surface != NULL && surface->valid[offset]
										? surface->pixels + offset * HIRES_CELL_PIXELS
										: NULL;
			for (legacy_s32 row = 0; row < HIRES_SCALE; row++) {
				legacy_u8 *out =
					framebuffer + (y * HIRES_SCALE + row) * HIRES_WIDTH + x * HIRES_SCALE;
				if (cell != NULL) {
					memcpy(out, cell + row * HIRES_SCALE, HIRES_SCALE);
				} else {
					memset(out, legacy[offset], HIRES_SCALE);
				}
			}
		}
	}
	return framebuffer;
}
