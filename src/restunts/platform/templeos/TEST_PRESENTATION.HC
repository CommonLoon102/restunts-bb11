/* Shared HolyC/host checks for the full 4:3 display and EGA pixel aspect. */
#define TEST_PRESENT_FIRST_COLOR 1
#define TEST_PRESENT_COLOR_COUNT (TEMPLE_PALETTE_OUTPUT_COUNT-TEST_PRESENT_FIRST_COLOR)
#define TEST_PRESENT_PAIR_FIRST 4
#define TEST_PRESENT_PAIR_SECOND 2
#define TEST_PRESENT_PAIR                                                                          \
  (TEST_PRESENT_PAIR_FIRST|(TEST_PRESENT_PAIR_SECOND<<TEMPLE_PALETTE_NIBBLE_BITS))

legacy_s64 TestPresentBoundary(legacy_s64 pixel,legacy_s64 source_size,legacy_s64 screen_size)
{
  return (pixel*screen_size+source_size-1)/source_size;
}

legacy_u8 TestRasterPixel(CDC *dc,legacy_s64 x,legacy_s64 y)
{
  x=TestPresentBoundary(x,TEMPLE_RENDER_WIDTH,TEMPLE_SCREEN_WIDTH);
  y=TestPresentBoundary(y,TEMPLE_RENDER_HEIGHT,TEMPLE_SCREEN_HEIGHT);
  return dc->body[y*dc->width_internal+x];
}

legacy_void TestPresentation(CDC *dc)
{
  legacy_u8 saved_pairs[TEMPLE_PALETTE_SOURCE_COUNT];
  legacy_u8 noreg saved_have_default=temple_raster_have_default;
  legacy_s64 paired,x,y,screen_x,screen_y,right,bottom;
  legacy_u8 noreg expected;
  MemCpy(saved_pairs,temple_raster_default_pairs,sizeof(saved_pairs));
  for(paired=FALSE;paired<=TRUE;paired++) {
    temple_raster_have_default=paired;
    MemSet(temple_raster_default_pairs,TEST_PRESENT_PAIR,sizeof(temple_raster_default_pairs));
    for(y=0;y<TEMPLE_RENDER_HEIGHT;y++) {
      for(x=0;x<TEMPLE_RENDER_WIDTH;x++) {
        TempleRasterPlot(x,y,TEST_PRESENT_FIRST_COLOR+(x+y)%TEST_PRESENT_COLOR_COUNT);
      }
    }
    MemSet(dc->body,TEMPLE_BYTE_MASK,TEMPLE_SCREEN_PIXELS);
    TempleRasterPresent(dc);
    /* Check each native pixel's entire physical footprint. Independent ceiling
     * boundaries cover duplicated rows as well as the first and last screen row.
     * Nonblack colors expose padding left behind by a letterboxed presentation.
     */
    for(y=0;y<TEMPLE_RENDER_HEIGHT;y++) {
      bottom=TestPresentBoundary(y+1,TEMPLE_RENDER_HEIGHT,TEMPLE_SCREEN_HEIGHT);
      for(x=0;x<TEMPLE_RENDER_WIDTH;x++) {
        right=TestPresentBoundary(x+1,TEMPLE_RENDER_WIDTH,TEMPLE_SCREEN_WIDTH);
        expected=TEST_PRESENT_FIRST_COLOR+(x+y)%TEST_PRESENT_COLOR_COUNT;
        if(paired) {
          expected=TEST_PRESENT_PAIR_SECOND;
          if(((x/TEMPLE_RENDER_SCALE)^(y/TEMPLE_RENDER_SCALE))&
              TEMPLE_PALETTE_CHECKER_MASK) {
            expected=TEST_PRESENT_PAIR_FIRST;
          }
        }
        for(screen_y=TestPresentBoundary(y,TEMPLE_RENDER_HEIGHT,TEMPLE_SCREEN_HEIGHT);
            screen_y<bottom;screen_y++) {
          for(screen_x=TestPresentBoundary(x,TEMPLE_RENDER_WIDTH,TEMPLE_SCREEN_WIDTH);
              screen_x<right;screen_x++) {
            TestAssert(dc->body[screen_y*dc->width_internal+screen_x]==expected);
          }
        }
      }
    }
  }
  temple_raster_have_default=FALSE;
  RsVideoPagesFillSpan(
      drawing_sprite.sprite_bitmapptr,0,TEMPLE_LOGICAL_WIDTH*TEMPLE_LOGICAL_HEIGHT,BLACK);
  MemCpy(temple_raster_default_pairs,saved_pairs,sizeof(saved_pairs));
  temple_raster_have_default=saved_have_default;
}
