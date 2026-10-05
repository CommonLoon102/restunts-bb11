#include "PALETTE.HH"

#define TEMPLE_PALETTE_RED_CHANNEL 0
#define TEMPLE_PALETTE_GREEN_CHANNEL 1
#define TEMPLE_PALETTE_BLUE_CHANNEL 2
#define TEMPLE_PALETTE_DAC16_MAX 65535
#define TEMPLE_PALETTE_RED_WEIGHT 30
#define TEMPLE_PALETTE_GREEN_WEIGHT 59
#define TEMPLE_PALETTE_BLUE_WEIGHT 11
#define TEMPLE_PALETTE_DISTANCE_LIMIT                                                              \
  ((TEMPLE_PALETTE_RED_WEIGHT+TEMPLE_PALETTE_GREEN_WEIGHT+TEMPLE_PALETTE_BLUE_WEIGHT)*\
          TEMPLE_PALETTE_CHANNEL_MAX*TEMPLE_PALETTE_CHANNEL_MAX+\
      1)

/* RGB6 values match TempleOS gr_palette_std and the game's first 16 !pal
 * entries. The OS programs its DAC from CBGR48; its draw surfaces store one
 * byte per pixel, of which only the low four bits select an opaque color.
 */
legacy_u8 temple_palette_standard[TEMPLE_PALETTE_OUTPUT_COUNT*TEMPLE_PALETTE_CHANNEL_COUNT]={0,
    0,0,0,0,42,0,42,0,0,42,42,42,0,0,42,0,42,42,21,0,42,42,42,21,21,21,21,
    21,63,21,63,21,21,63,63,63,21,21,63,21,63,63,63,21,63,63,63};
legacy_u8 temple_palette_source[TEMPLE_PALETTE_SOURCE_COUNT*TEMPLE_PALETTE_CHANNEL_COUNT];
legacy_u8 temple_palette_map[TEMPLE_PALETTE_SOURCE_COUNT];
CBGR48 temple_palette_saved[TEMPLE_PALETTE_OUTPUT_COUNT];
legacy_u8 temple_palette_active=FALSE;
legacy_u64 temple_palette_generation=0;

legacy_u64 TemplePaletteGeneration()
{
  return temple_palette_generation;
}

legacy_u8 TemplePaletteNearest(legacy_u8 red,legacy_u8 green,legacy_u8 blue)
{
  legacy_s64 best_distance=TEMPLE_PALETTE_DISTANCE_LIMIT;
  legacy_u8 noreg best=BLACK;
  legacy_s64 index;
  legacy_s64 offset;
  legacy_s64 red_delta;
  legacy_s64 green_delta;
  legacy_s64 blue_delta;
  legacy_s64 distance;

  /* Weighted squared RGB is a deterministic approximation, not a perceptually
   * uniform color space. No square root is needed to order the distances.
   * Strict comparison chooses the lowest palette index on equal distances.
   */
  for(index=0;index<TEMPLE_PALETTE_OUTPUT_COUNT;index++) {
    offset=index*TEMPLE_PALETTE_CHANNEL_COUNT;
    red_delta=(red&TEMPLE_PALETTE_CHANNEL_MAX)-
        temple_palette_source[offset+TEMPLE_PALETTE_RED_CHANNEL];
    green_delta=(green&TEMPLE_PALETTE_CHANNEL_MAX)-
        temple_palette_source[offset+TEMPLE_PALETTE_GREEN_CHANNEL];
    blue_delta=(blue&TEMPLE_PALETTE_CHANNEL_MAX)-
        temple_palette_source[offset+TEMPLE_PALETTE_BLUE_CHANNEL];
    distance=TEMPLE_PALETTE_RED_WEIGHT*red_delta*red_delta+
        TEMPLE_PALETTE_GREEN_WEIGHT*green_delta*green_delta+
        TEMPLE_PALETTE_BLUE_WEIGHT*blue_delta*blue_delta;
    if(distance<best_distance) {
      best=index;
      best_distance=distance;
    }
  }
  return best;
}

legacy_void TemplePaletteRebuild()
{
  legacy_s64 index;
  legacy_s64 offset;
  for(index=0;index<TEMPLE_PALETTE_SOURCE_COUNT;index++) {
    offset=index*TEMPLE_PALETTE_CHANNEL_COUNT;
    /* Keep the EGA entries stable even when a fade makes colors identical. */
    if(index<TEMPLE_PALETTE_OUTPUT_COUNT) {
      temple_palette_map[index]=index;
    } else {
      temple_palette_map[index]=
          TemplePaletteNearest(temple_palette_source[offset+TEMPLE_PALETTE_RED_CHANNEL],
              temple_palette_source[offset+TEMPLE_PALETTE_GREEN_CHANNEL],
              temple_palette_source[offset+TEMPLE_PALETTE_BLUE_CHANNEL]);
    }
  }
  temple_palette_generation++;
}

legacy_void TemplePaletteUpload()
{
  legacy_s64 index;
  legacy_s64 offset;
  CBGR48 output[TEMPLE_PALETTE_OUTPUT_COUNT];
  for(index=0;index<TEMPLE_PALETTE_OUTPUT_COUNT;index++) {
    offset=index*TEMPLE_PALETTE_CHANNEL_COUNT;
    output[index]=0;
    output[index].r=temple_palette_source[offset+TEMPLE_PALETTE_RED_CHANNEL]*
        TEMPLE_PALETTE_DAC16_MAX/TEMPLE_PALETTE_CHANNEL_MAX;
    output[index].g=temple_palette_source[offset+TEMPLE_PALETTE_GREEN_CHANNEL]*
        TEMPLE_PALETTE_DAC16_MAX/TEMPLE_PALETTE_CHANNEL_MAX;
    output[index].b=temple_palette_source[offset+TEMPLE_PALETTE_BLUE_CHANNEL]*
        TEMPLE_PALETTE_DAC16_MAX/TEMPLE_PALETTE_CHANNEL_MAX;
  }
  GrPaletteSet(output);
}

legacy_void TemplePaletteInit()
{
  if(temple_palette_active) {
    return;
  }
  GrPaletteGet(temple_palette_saved);
  MemSet(temple_palette_source,0,sizeof(temple_palette_source));
  MemCpy(temple_palette_source,temple_palette_standard,sizeof(temple_palette_standard));
  TemplePaletteRebuild();
  TemplePaletteUpload();
  temple_palette_active=TRUE;
}

legacy_void TemplePaletteShutdown()
{
  if(temple_palette_active) {
    GrPaletteSet(temple_palette_saved);
    temple_palette_active=FALSE;
  }
}

legacy_void TemplePaletteSet(legacy_u16 start,legacy_u16 count,legacy_u8 *rgb6)
{
  legacy_s64 index;
  legacy_s64 channel;
  legacy_s64 end;
  legacy_s64 offset;
  if(!rgb6||!count||start>=TEMPLE_PALETTE_SOURCE_COUNT) {
    return;
  }
  TemplePaletteInit();
  end=start+count;
  if(end>TEMPLE_PALETTE_SOURCE_COUNT) {
    end=TEMPLE_PALETTE_SOURCE_COUNT;
  }
  for(index=start;index<end;index++) {
    offset=index*TEMPLE_PALETTE_CHANNEL_COUNT;
    for(channel=0;channel<TEMPLE_PALETTE_CHANNEL_COUNT;channel++) {
      temple_palette_source[offset+channel]=*rgb6++&TEMPLE_PALETTE_CHANNEL_MAX;
    }
  }
  TemplePaletteRebuild();
  /* The physical sixteen colors follow the original palette's EGA entries.
   * Updating their DAC levels preserves smooth fades with only sixteen colors.
   */
  if(start<TEMPLE_PALETTE_OUTPUT_COUNT) {
    TemplePaletteUpload();
  }
}

legacy_u8 TemplePaletteColor(legacy_u8 source_index)
{
  /* Index 255 can be a real framebuffer color. It is only transparent in
   * specific sprite operations; never reserve it in the final color map.
   */
  return temple_palette_map[source_index&TEMPLE_BYTE_MASK];
}

/* A complete !eg0 resource has 256 packed pairs after its 16-byte shape header.
 * This function accepts that payload, not the header. The caller must apply
 * sprite transparency/AND/OR operations before converting a resulting color.
 */
legacy_u8 TemplePaletteEgaColor(
    legacy_u8 *pairs,legacy_u8 source_index,legacy_s64 x,legacy_s64 y)
{
  legacy_u8 noreg pair;
  if(!pairs) {
    return TemplePaletteColor(source_index);
  }
  pair=pairs[source_index&TEMPLE_BYTE_MASK];
  /* DOS EGA uses the high nibble on even logical (320x200) pixels. */
  if(!((x^y)&TEMPLE_PALETTE_CHECKER_MASK)) {
    pair>>=TEMPLE_PALETTE_NIBBLE_BITS;
  }
  return pair&TEMPLE_PALETTE_NIBBLE_MASK;
}
