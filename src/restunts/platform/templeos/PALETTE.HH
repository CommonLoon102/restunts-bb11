#ifndef RESTUNTS_TEMPLEOS_PALETTE_HH
#define RESTUNTS_TEMPLEOS_PALETTE_HH

#include "LEGACY.HH"

#define TEMPLE_PALETTE_SOURCE_COUNT 256
#define TEMPLE_PALETTE_OUTPUT_COUNT 16
#define TEMPLE_PALETTE_CHANNEL_COUNT 3
#define TEMPLE_PALETTE_CHANNEL_MAX 63
#define TEMPLE_PALETTE_NIBBLE_BITS 4
#define TEMPLE_PALETTE_NIBBLE_MASK 15

extern legacy_void TemplePaletteInit();
extern legacy_void TemplePaletteShutdown();
extern legacy_void TemplePaletteSet(legacy_u16 start,legacy_u16 count,legacy_u8 *rgb6);
extern legacy_u8 TemplePaletteNearest(legacy_u8 red,legacy_u8 green,legacy_u8 blue);
extern legacy_u8 TemplePaletteColor(legacy_u8 source_index);
extern legacy_u64 TemplePaletteGeneration();

/* A complete !eg0 resource has 256 packed pairs after its 16-byte shape header.
 * These functions accept that payload, not the header. The caller must apply
 * sprite transparency/AND/OR operations before converting a resulting color.
 */
extern legacy_u8 TemplePaletteEgaColor(
    legacy_u8 *pairs,legacy_u8 source_index,legacy_s64 x,legacy_s64 y);

#endif
