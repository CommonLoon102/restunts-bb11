#include "hires.h"
#include "platform.h"
#include "shape2d.h"
#include "shape2d_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define HIRES_ADDRESS_COUNT 65536UL
#define HIRES_CELL_PIXELS (HIRES_SCALE * HIRES_SCALE)

/* Each legacy pixel may carry sixteen independently rasterized samples.
 * Keeping these attached to byte offsets lets ordinary clipped sprite copies,
 * saved backgrounds, and dashboard masks carry their detail with them. */
struct HIRES_SURFACE {
	const unsigned char *base;
	unsigned char *pixels;
	unsigned char valid[HIRES_ADDRESS_COUNT];
	struct HIRES_SURFACE *next;
};

static struct HIRES_SURFACE *surfaces;
static struct HIRES_SURFACE *active;
static struct SPRITE active_sprite;
static int enabled;
static unsigned long generation;
static unsigned char *framebuffer;

static void *hires_allocate(size_t size)
{
	void *result = calloc(1, size);
	if (result == NULL) {
		fputs("Cannot allocate high-resolution framebuffer\n", stderr);
		dos_process_exit(1);
	}
	return result;
}

static struct HIRES_SURFACE *hires_find(const unsigned char *base)
{
	for (struct HIRES_SURFACE *surface = surfaces; surface != NULL; surface = surface->next) {
		if (surface->base == base) {
			return surface;
		}
	}
	return NULL;
}

static struct HIRES_SURFACE *hires_create(const unsigned char *base)
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

static unsigned char *hires_cell(struct HIRES_SURFACE *surface, legacy_u16 offset)
{
	unsigned char *cell = surface->pixels + (size_t)offset * HIRES_CELL_PIXELS;
	if (!surface->valid[offset]) {
		memset(cell, surface->base[offset], HIRES_CELL_PIXELS);
		surface->valid[offset] = 1;
	}
	return cell;
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
	active = NULL;
	enabled = 0;
	generation++;
}

void hires_set_enabled(int value)
{
	value = value != 0;
	if (value == enabled) {
		return;
	}
	hires_shutdown();
	enabled = value;
}

int hires_enabled(void)
{
	return enabled;
}

unsigned long hires_generation(void)
{
	return generation;
}

int hires_begin(const struct SPRITE *target)
{
	if (!enabled || active != NULL) {
		return 0;
	}
	const unsigned char *base =
		dos_memory_make_pointer(dos_memory_pointer_segment(target->sprite_bitmapptr), 0);
	active = hires_create(base);
	active_sprite = *target;
	/* Seed the background before the legacy 3D pass overwrites its pixels. */
	for (unsigned int y = target->sprite_top; y < target->sprite_bottom && y < 200; y++) {
		legacy_u16 row = LEGACY_READ_U16_LE(target->sprite_lineofs + y * 2);
		for (unsigned int x = target->sprite_raster_left;
			 x < target->sprite_raster_right && x < 320; x++) {
			hires_cell(active, (legacy_u16)(row + x));
		}
	}
	return 1;
}

void hires_end(void)
{
	if (active != NULL) {
		active = NULL;
		generation++;
	}
}

void hires_pixel(int x, int y, unsigned char color)
{
	if (active == NULL || x < 0 || y < 0 || x >= HIRES_WIDTH || y >= HIRES_HEIGHT ||
		x < active_sprite.sprite_raster_left * HIRES_SCALE ||
		x >= active_sprite.sprite_raster_right * HIRES_SCALE ||
		y < active_sprite.sprite_top * HIRES_SCALE ||
		y >= active_sprite.sprite_bottom * HIRES_SCALE) {
		return;
	}
	legacy_u16 row = LEGACY_READ_U16_LE(active_sprite.sprite_lineofs + (y / HIRES_SCALE) * 2);
	unsigned char *cell = hires_cell(active, (legacy_u16)(row + x / HIRES_SCALE));
	cell[(y % HIRES_SCALE) * HIRES_SCALE + x % HIRES_SCALE] = color;
}

void hires_write(const unsigned char *base, legacy_u16 offset, unsigned char color)
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

void hires_raster(const unsigned char *destination, legacy_u16 destination_offset,
				  const unsigned char *source, legacy_u16 source_offset, legacy_u16 count,
				  legacy_s16 operation, const unsigned char *palette)
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
	for (unsigned int index = 0; index < count; index++) {
		legacy_u16 so = (legacy_u16)(source_offset + index);
		legacy_u16 dest = (legacy_u16)(destination_offset + index);
		unsigned char source_color = source[so];
		unsigned char *cell = hires_cell(dst, dest);
		/* Match the game's forward traversal, including overlapping copies. */
		unsigned char samples[HIRES_CELL_PIXELS];
		if (src != NULL && src->valid[so]) {
			memcpy(samples, src->pixels + (size_t)so * HIRES_CELL_PIXELS, sizeof(samples));
		} else {
			memset(samples, source_color, sizeof(samples));
		}
		for (int sample = 0; sample < HIRES_CELL_PIXELS; sample++) {
			unsigned char value = samples[sample];
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

const unsigned char *hires_framebuffer(const unsigned char *legacy, int *width, int *height)
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
	for (int y = 0; y < 200; y++) {
		for (int x = 0; x < 320; x++) {
			unsigned int offset = y * 320 + x;
			const unsigned char *cell = surface != NULL && surface->valid[offset]
											? surface->pixels + offset * HIRES_CELL_PIXELS
											: NULL;
			for (int row = 0; row < HIRES_SCALE; row++) {
				unsigned char *out =
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
