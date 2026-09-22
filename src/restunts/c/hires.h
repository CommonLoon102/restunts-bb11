#ifndef RESTUNTS_HIRES_H
#define RESTUNTS_HIRES_H

#include "legacy.h"

#define HIRES_SCALE 4
#define HIRES_WIDTH 1280
#define HIRES_HEIGHT 800

struct SPRITE;

/* SDL3-only companion pixels. Legacy sprite offsets and resources stay 16-bit. */
void hires_set_enabled(int enabled);
int hires_enabled(void);
int hires_begin(const struct SPRITE *target);
void hires_end(void);
void hires_pixel(int x, int y, unsigned char color);
void hires_write(const unsigned char *base, legacy_u16 offset, unsigned char color);
void hires_raster(const unsigned char *destination, legacy_u16 destination_offset,
				  const unsigned char *source, legacy_u16 source_offset, legacy_u16 count,
				  legacy_s16 operation, const unsigned char *palette);
void hires_forget(const void *base);
void hires_forget_range(const void *base, legacy_u32 size);
const unsigned char *hires_framebuffer(const unsigned char *legacy, int *width, int *height);
unsigned long hires_generation(void);
void hires_shutdown(void);

#endif
