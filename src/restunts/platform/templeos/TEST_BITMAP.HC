/* Shared HolyC/host checks for palette selection and composed EGA bitmaps. */
#define TEST_BITMAP_RESOURCE_ADDRESS 0x40000
#define TEST_BITMAP_RESOURCE_PARAGRAPHS 4096
#define TEST_BITMAP_RESOURCE_BYTES (TEMPLE_RASTER_PARAGRAPH_BYTES*TEST_BITMAP_RESOURCE_PARAGRAPHS)
#define TEST_BITMAP_RESOURCE_COUNT 4
#define TEST_BITMAP_TABLE_COUNT (TEST_BITMAP_RESOURCE_COUNT-1)
#define TEST_BITMAP_PORTRAIT_ENTRY 1
#define TEST_BITMAP_PAPER_ENTRY 2
#define TEST_BITMAP_DIRECTORY_BYTES                                                                \
  (TEMPLE_RASTER_RESOURCE_HEADER+TEST_BITMAP_RESOURCE_COUNT*TEMPLE_RASTER_RESOURCE_ENTRY_BYTES)
#define TEST_BITMAP_DEFAULT_TABLE 0
#define TEST_BITMAP_PORTRAIT_TABLE 3
#define TEST_BITMAP_PAPER_TABLE 7
#define TEST_BITMAP_MISSING_TABLE 6
#define TEST_BITMAP_PLAIN_INDEX 17
#define TEST_BITMAP_TREAD_INDEX 19
#define TEST_BITMAP_FACE_INDEX 33
#define TEST_BITMAP_PLAIN_PIXEL 0
#define TEST_BITMAP_TREAD_PIXEL 1
#define TEST_BITMAP_FACE_PIXEL 2
#define TEST_BITMAP_PIXEL_COUNT 3
#define TEST_BITMAP_IMAGE_ROWS 2
#define TEST_BITMAP_IMAGE_PIXELS (TEST_BITMAP_PIXEL_COUNT*TEST_BITMAP_IMAGE_ROWS)
#define TEST_BITMAP_POSITION_Y_OFFSET 10
#define TEST_BITMAP_NOMINAL_Y 6
#define TEST_BITMAP_GRAY_PAIR 0x77
#define TEST_BITMAP_DEFAULT_FACE_PAIR 0xCC
#define TEST_BITMAP_FACE_PAIR 0xCF
#define TEST_BITMAP_PAPER_PAIR 0xFF
#define TEST_BITMAP_TREAD_PAIR 0xF7
#define TEST_BITMAP_NATIVE_COLOR 1
#define TEST_BITMAP_RUN_LIMIT (TEMPLE_RASTER_BYTE_SIGN-1)
#define TEST_BITMAP_ROW 20
#define TEST_BITMAP_PAPER_X 10
#define TEST_BITMAP_PORTRAIT_X 21
#define TEST_BITMAP_FALLBACK_X 30
#define TEST_BITMAP_COPY_X 40
#define TEST_BITMAP_SAVED_X 50
#define TEST_BITMAP_ODD_Y_X 60
#define TEST_BITMAP_ODD_Y (TEST_BITMAP_ROW+1)
#define TEST_BITMAP_CLIPPED_Y (TEST_BITMAP_ODD_Y+1)
#define TEST_BITMAP_TREAD_X (TEST_BITMAP_PAPER_X+TEST_BITMAP_TREAD_PIXEL)
#define TEST_BITMAP_PARITY_MASK 1

legacy_void TestBitmapWrite16(legacy_u8 *bytes,legacy_u16 value)
{
  bytes[0]=value&TEMPLE_BYTE_MASK;
  bytes[1]=value>>TEMPLE_BYTE_BITS;
}

legacy_void TestBitmapWrite32(legacy_u8 *bytes,legacy_u32 value)
{
  TestBitmapWrite16(bytes,value&TEMPLE_WORD_MASK);
  TestBitmapWrite16(bytes+sizeof(legacy_u16),value>>(TEMPLE_BYTE_BITS*2));
}

legacy_s64 TestBitmapEncode(legacy_u8 *output,legacy_u8 *values,legacy_s64 count)
{
  legacy_s64 index=0,run,written=0;
  while(index<count) {
    run=1;
    while(index+run<count&&run<TEST_BITMAP_RUN_LIMIT&&
        values[index+run]==values[index]) {
      run++;
    }
    /* Singletons are literal runs; repeated values use positive run counts. */
    if(run==1) {
      output[written++]=TEMPLE_BYTE_MASK;
    } else {
      output[written++]=run;
    }
    output[written++]=values[index];
    index+=run;
  }
  output[written++]=0;
  return written;
}

legacy_u8 *TestBitmapResource(legacy_u8 *resource,legacy_u8 parsed)
{
  legacy_u8 pairs[TEMPLE_PALETTE_SOURCE_COUNT];
  legacy_u8 pixels[TEST_BITMAP_IMAGE_PIXELS];
  legacy_s64 index,offset=TEST_BITMAP_DIRECTORY_BYTES,count;
  legacy_u8 *shape=NULL;
  MemSet(resource,0,TEST_BITMAP_RESOURCE_BYTES);
  TestBitmapWrite16(resource+sizeof(legacy_u32),TEST_BITMAP_RESOURCE_COUNT);
  MemCpy(resource+TEMPLE_RASTER_RESOURCE_HEADER,"!eg0!eg3!eg7test",
      TEST_BITMAP_RESOURCE_COUNT*TEMPLE_RASTER_RESOURCE_ID_BYTES);
  pixels[TEST_BITMAP_PLAIN_PIXEL]=TEST_BITMAP_PLAIN_INDEX;
  pixels[TEST_BITMAP_TREAD_PIXEL]=TEST_BITMAP_TREAD_INDEX;
  pixels[TEST_BITMAP_FACE_PIXEL]=TEST_BITMAP_FACE_INDEX;
  MemCpy(pixels+TEST_BITMAP_PIXEL_COUNT,pixels,TEST_BITMAP_PIXEL_COUNT);
  for(index=0;index<TEST_BITMAP_RESOURCE_COUNT;index++) {
    TestBitmapWrite32(resource+TEMPLE_RASTER_RESOURCE_HEADER+
            TEST_BITMAP_RESOURCE_COUNT*TEMPLE_RASTER_RESOURCE_ID_BYTES+
            index*sizeof(legacy_u32),
        offset-TEST_BITMAP_DIRECTORY_BYTES);
    shape=resource+offset;
    count=TEMPLE_PALETTE_SOURCE_COUNT;
    if(index==TEST_BITMAP_TABLE_COUNT) {
      count=TEST_BITMAP_IMAGE_PIXELS;
    }
    TestBitmapWrite16(shape,count);
    TestBitmapWrite16(shape+sizeof(legacy_u16),1);
    if(index==TEST_BITMAP_TABLE_COUNT) {
      TestBitmapWrite16(shape,TEST_BITMAP_PIXEL_COUNT);
      TestBitmapWrite16(shape+sizeof(legacy_u16),TEST_BITMAP_IMAGE_ROWS);
      TestBitmapWrite16(shape+TEST_BITMAP_POSITION_Y_OFFSET,TEST_BITMAP_NOMINAL_Y);
    }
    offset+=TEMPLE_RASTER_SHAPE_HEADER;
    MemSet(pairs,TEST_BITMAP_GRAY_PAIR,sizeof(pairs));
    pairs[TEST_BITMAP_FACE_INDEX]=TEST_BITMAP_DEFAULT_FACE_PAIR;
    if(index==TEST_BITMAP_PORTRAIT_ENTRY) {
      pairs[TEST_BITMAP_FACE_INDEX]=TEST_BITMAP_FACE_PAIR;
    }
    if(index==TEST_BITMAP_PAPER_ENTRY) {
      pairs[TEST_BITMAP_PLAIN_INDEX]=TEST_BITMAP_PAPER_PAIR;
      pairs[TEST_BITMAP_TREAD_INDEX]=TEST_BITMAP_TREAD_PAIR;
    }
    if(index==TEST_BITMAP_TABLE_COUNT) {
      MemCpy(pairs,pixels,sizeof(pixels));
    }
    if(parsed) {
      offset+=TestBitmapEncode(resource+offset,pairs,count);
    } else {
      MemCpy(resource+offset,pairs,count);
      offset+=count;
    }
  }
  TestBitmapWrite32(resource,offset);
  if(parsed) {
    TempleRasterResourceParsed(resource);
  } else {
    TempleRasterResource(resource);
  }
  return shape;
}

legacy_u16 TestBitmapOffsetAt(legacy_s64 x,legacy_s64 y)
{
  return TempleRasterRead16(drawing_sprite.sprite_lineofs+y*sizeof(legacy_u16))+x;
}

legacy_u16 TestBitmapOffset(legacy_s64 x)
{
  return TestBitmapOffsetAt(x,TEST_BITMAP_ROW);
}

legacy_void TestBitmapCellAt(CDC *dc,legacy_s64 x,legacy_s64 y,legacy_u8 color)
{
  legacy_s64 native_x,native_y;
  for(native_y=0;native_y<TEMPLE_RENDER_SCALE;native_y++) {
    for(native_x=0;native_x<TEMPLE_RENDER_SCALE;native_x++) {
      TestAssert(TestRasterPixel(dc,x*TEMPLE_RENDER_SCALE+native_x,
                    y*TEMPLE_RENDER_SCALE+native_y)==color);
    }
  }
}

legacy_void TestBitmapCell(CDC *dc,legacy_s64 x,legacy_u8 color)
{
  TestBitmapCellAt(dc,x,TEST_BITMAP_ROW,color);
}

legacy_u8 TestBitmapPairColorAt(legacy_u8 pair,legacy_s64 x,legacy_s64 y)
{
  if(!((x^y)&TEST_BITMAP_PARITY_MASK)) {
    pair>>=TEMPLE_PALETTE_NIBBLE_BITS;
  }
  return pair&TEMPLE_PALETTE_NIBBLE_MASK;
}

legacy_u8 TestBitmapPairColor(legacy_u8 pair,legacy_s64 x)
{
  return TestBitmapPairColorAt(pair,x,TEST_BITMAP_ROW);
}

legacy_void TestBitmapOddY(
    CDC *dc,legacy_u8 *resource,legacy_u8 *shape,legacy_u8 *source,legacy_u8 *map)
{
  legacy_s64 tread_x=TEST_BITMAP_ODD_Y_X+TEST_BITMAP_TREAD_PIXEL;
  legacy_u8 noreg color=
      TestBitmapPairColorAt(TEST_BITMAP_TREAD_PAIR,tread_x,TEST_BITMAP_NOMINAL_Y);
  RsShape2dSelectEgaPalette(resource,TEST_BITMAP_PAPER_TABLE);
  TempleRasterSourceAt(shape,TEST_BITMAP_ODD_Y);
  RsVideoPagesRasterSpan(drawing_sprite.sprite_bitmapptr,
      TestBitmapOffsetAt(TEST_BITMAP_ODD_Y_X,TEST_BITMAP_ODD_Y),source,0,
      TEST_BITMAP_PIXEL_COUNT,SHAPE2D_RASTER_MAP,map);
  TempleRasterSource(NULL);
  /* A clipped draw still receives the original draw Y, then advances its source
   * and destination by the skipped row. The nominal shape phase advances too.
   */
  TempleRasterSourceAt(shape,TEST_BITMAP_ODD_Y);
  RsVideoPagesRasterSpan(drawing_sprite.sprite_bitmapptr,
      TestBitmapOffsetAt(TEST_BITMAP_ODD_Y_X,TEST_BITMAP_CLIPPED_Y),source,
      TEST_BITMAP_PIXEL_COUNT,TEST_BITMAP_PIXEL_COUNT,SHAPE2D_RASTER_MAP,map);
  TempleRasterSource(NULL);
  RsVideoPagesCopySpan(drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_COPY_X),
      drawing_sprite.sprite_bitmapptr,TestBitmapOffsetAt(tread_x,TEST_BITMAP_ODD_Y),1);
  TempleRasterPresent(dc);
  TestBitmapCellAt(dc,tread_x,TEST_BITMAP_ODD_Y,color);
  TestBitmapCellAt(dc,tread_x,TEST_BITMAP_CLIPPED_Y,
      TestBitmapPairColorAt(TEST_BITMAP_TREAD_PAIR,tread_x,TEST_BITMAP_NOMINAL_Y+1));
  TestBitmapCell(dc,TEST_BITMAP_COPY_X,color);
}

legacy_void TestBitmapPalettes(CDC *dc,CSprite *background)
{
  legacy_u8 *resource=arena+TEST_BITMAP_RESOURCE_ADDRESS;
  legacy_u8 *shape,*source;
  legacy_u8 pixels[TEST_BITMAP_IMAGE_PIXELS],map[TEMPLE_PALETTE_SOURCE_COUNT];
  legacy_u8 noreg saved_have_default=temple_raster_have_default;
  legacy_u8 noreg parsed,color;
  legacy_s64 index,slot;
  legacy_u32 saved[TEMPLE_RASTER_CELL_PIXELS];
  legacy_u32 *cell=temple_raster_target->samples+
      TestBitmapOffset(TEST_BITMAP_TREAD_X)*TEMPLE_RASTER_CELL_PIXELS;
  temple_raster_have_default=FALSE;
  pixels[TEST_BITMAP_PLAIN_PIXEL]=TEST_BITMAP_PLAIN_INDEX;
  pixels[TEST_BITMAP_TREAD_PIXEL]=TEST_BITMAP_TREAD_INDEX;
  pixels[TEST_BITMAP_FACE_PIXEL]=TEST_BITMAP_FACE_INDEX;
  MemCpy(pixels+TEST_BITMAP_PIXEL_COUNT,pixels,TEST_BITMAP_PIXEL_COUNT);
  for(index=0;index<TEMPLE_PALETTE_SOURCE_COUNT;index++) {
    map[index]=index;
  }
  for(parsed=FALSE;parsed<=TRUE;parsed++) {
    shape=TestBitmapResource(resource,parsed);
    source=shape+TEMPLE_RASTER_SHAPE_HEADER;
    if(parsed) {
      /* RLE image decoding passes temporary pixels under the shape's context. */
      source=pixels;
    }
    RsShape2dSelectEgaPalette(resource,TEST_BITMAP_PAPER_TABLE);
    TempleRasterSource(shape);
    RsVideoPagesRasterSpan(drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_PAPER_X),
        source,0,TEST_BITMAP_PIXEL_COUNT,SHAPE2D_RASTER_MAP,map);
    RsShape2dSelectEgaPalette(resource,TEST_BITMAP_PORTRAIT_TABLE);
    RsVideoPagesRasterSpan(drawing_sprite.sprite_bitmapptr,
        TestBitmapOffset(TEST_BITMAP_PORTRAIT_X),source,0,TEST_BITMAP_PIXEL_COUNT,
        SHAPE2D_RASTER_MAP,map);
    RsShape2dSelectEgaPalette(resource,TEST_BITMAP_MISSING_TABLE);
    RsVideoPagesRasterSpan(drawing_sprite.sprite_bitmapptr,
        TestBitmapOffset(TEST_BITMAP_FALLBACK_X),source,0,TEST_BITMAP_PIXEL_COUNT,
        SHAPE2D_RASTER_MAP,map);
    TempleRasterSource(NULL);
    /* Palette switches must not change samples already composed on screen. */
    RsShape2dSelectEgaPalette(resource,TEST_BITMAP_DEFAULT_TABLE);
    TempleRasterPresent(dc);
    TestBitmapCell(
        dc,TEST_BITMAP_PAPER_X,TestBitmapPairColor(TEST_BITMAP_PAPER_PAIR,TEST_BITMAP_PAPER_X));
    TestBitmapCell(
        dc,TEST_BITMAP_TREAD_X,TestBitmapPairColor(TEST_BITMAP_TREAD_PAIR,TEST_BITMAP_TREAD_X));
    TestBitmapCell(dc,TEST_BITMAP_PORTRAIT_X+TEST_BITMAP_FACE_PIXEL,
        TestBitmapPairColor(
            TEST_BITMAP_FACE_PAIR,TEST_BITMAP_PORTRAIT_X+TEST_BITMAP_FACE_PIXEL));
    TestBitmapCell(dc,TEST_BITMAP_FALLBACK_X,
        TestBitmapPairColor(TEST_BITMAP_GRAY_PAIR,TEST_BITMAP_FALLBACK_X));
    TestBitmapCell(dc,TEST_BITMAP_FALLBACK_X+TEST_BITMAP_TREAD_PIXEL,
        TestBitmapPairColor(
            TEST_BITMAP_GRAY_PAIR,TEST_BITMAP_FALLBACK_X+TEST_BITMAP_TREAD_PIXEL));
    TestBitmapCell(dc,TEST_BITMAP_FALLBACK_X+TEST_BITMAP_FACE_PIXEL,
        TestBitmapPairColor(
            TEST_BITMAP_DEFAULT_FACE_PAIR,TEST_BITMAP_FALLBACK_X+TEST_BITMAP_FACE_PIXEL));
    /* Moving an authored pixel by an odd logical distance preserves its phase. */
    RsVideoPagesCopySpan(drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_COPY_X),
        drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_TREAD_X),1);
    TempleRasterPresent(dc);
    color=TestBitmapPairColor(TEST_BITMAP_TREAD_PAIR,TEST_BITMAP_TREAD_X);
    TestBitmapCell(dc,TEST_BITMAP_COPY_X,color);
    /* Save and restore a cell containing both bitmap and native geometry. */
    TempleRasterPlot(TEST_BITMAP_TREAD_X*TEMPLE_RENDER_SCALE,
        TEST_BITMAP_ROW*TEMPLE_RENDER_SCALE,TEST_BITMAP_NATIVE_COLOR);
    MemCpy(saved,cell,sizeof(saved));
    TestAssert(saved[0]!=saved[TEMPLE_RASTER_CELL_PIXELS-1]);
    RsVideoPagesCopySpan(background->sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_SAVED_X),
        drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_TREAD_X),1);
    RsVideoPagesWritePixel(
        drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_TREAD_X),BLACK);
    RsVideoPagesCopySpan(drawing_sprite.sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_TREAD_X),
        background->sprite_bitmapptr,TestBitmapOffset(TEST_BITMAP_SAVED_X),1);
    for(slot=0;slot<TEMPLE_RASTER_CELL_PIXELS;slot++) {
      TestAssert(cell[slot]==saved[slot]);
    }
    TestBitmapOddY(dc,resource,shape,source,map);
    TempleRasterForget(resource);
  }
  RsVideoPagesFillSpan(
      drawing_sprite.sprite_bitmapptr,0,TEMPLE_LOGICAL_WIDTH*TEMPLE_LOGICAL_HEIGHT,BLACK);
  temple_raster_have_default=saved_have_default;
}
