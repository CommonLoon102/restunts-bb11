/* Shared HolyC/host checks for native and logical polygon coverage. */
#define TEST_POLYGON_VERTEX_COUNT 4
#define TEST_POLYGON_TILE_COUNT 2
#define TEST_POLYGON_BASE_Y 100
#define TEST_POLYGON_LEFT 10
#define TEST_POLYGON_WIDTH 12
#define TEST_POLYGON_HEIGHT TEMPLE_RENDER_SCALE
#define TEST_POLYGON_NATIVE_PIXEL 1
#define TEST_SPHERE_CENTER 11
#define TEST_SPHERE_SIZE 4
#define TEST_POLYGON_UPPER_COLOR 4
#define TEST_POLYGON_LOWER_COLOR 2
#define TEST_POLYGON_ALTERNATE_COLOR 14
#define TEST_POLYGON_PATTERN 0xCC33
#define TEST_POLYGON_PATTERN_RUN 2

legacy_void TestPolygonQuad(
    CPoint2D *vertices,legacy_s64 left,legacy_s64 top,legacy_s64 right,legacy_s64 bottom)
{
  vertices[0].px=left;
  vertices[0].py=top;
  vertices[1].px=right;
  vertices[1].py=top;
  vertices[2].px=right;
  vertices[2].py=bottom;
  vertices[3].px=left;
  vertices[3].py=bottom;
}

legacy_void TestPolygonSharedEdges(CDC *dc)
{
  CSprite saved_sprite;
  CPoint2D upper[TEST_POLYGON_VERTEX_COUNT];
  CPoint2D lower[TEST_POLYGON_VERTEX_COUNT];
  legacy_s64 native;
  legacy_s64 scale;
  legacy_s64 render_y;
  legacy_s64 clipped;
  legacy_s64 phase;
  legacy_s64 mode;
  legacy_s64 reverse;
  legacy_s64 tile;
  legacy_s64 left;
  legacy_s64 right;
  legacy_s64 shared_y;
  legacy_s64 top;
  legacy_s64 bottom;
  legacy_s64 x;
  legacy_s64 y;
  legacy_s64 center_y;
  legacy_s64 clipped_top;
  legacy_s64 clipped_bottom;
  legacy_u8 noreg expected;
  MemCpy(&saved_sprite,&drawing_sprite,sizeof(CSprite));
  for(native=FALSE;native<=TRUE;native++) {
    TempleRasterNative(native);
    scale=TempleRasterCoordinateScale();
    for(clipped=FALSE;clipped<=TRUE;clipped++) {
      /* The second window also exercises negative polygon vertices and clipping. */
      left=TEST_POLYGON_LEFT;
      drawing_sprite.sprite_top=0;
      drawing_sprite.sprite_bottom=TEMPLE_LOGICAL_HEIGHT;
      drawing_sprite.sprite_raster_left=0;
      drawing_sprite.sprite_raster_right=TEMPLE_LOGICAL_WIDTH;
      if(clipped) {
        left=-TEST_POLYGON_WIDTH/2;
        drawing_sprite.sprite_top=1;
        drawing_sprite.sprite_bottom=TEST_POLYGON_HEIGHT;
        drawing_sprite.sprite_raster_left=1;
        drawing_sprite.sprite_raster_right=TEST_POLYGON_WIDTH/2-1;
      }
      right=left+TEST_POLYGON_WIDTH;
      clipped_top=drawing_sprite.sprite_top*TEMPLE_RASTER_Y_SCALE;
      clipped_bottom=drawing_sprite.sprite_bottom*TEMPLE_RASTER_Y_SCALE;
      for(phase=0;phase<TEMPLE_RENDER_SCALE;phase++) {
        shared_y=phase;
        if(!clipped) {
          shared_y+=TEST_POLYGON_BASE_Y;
        }
        top=shared_y-TEST_POLYGON_HEIGHT;
        bottom=shared_y+TEST_POLYGON_HEIGHT;
        TestPolygonQuad(upper,left,top,right,shared_y);
        TestPolygonQuad(lower,left,shared_y,right,bottom);
        for(mode=TEMPLE_RASTER_PAINT_SOLID;mode<=TEMPLE_RASTER_PAINT_TWO_COLOR;mode++) {
          for(reverse=FALSE;reverse<=TRUE;reverse++) {
            RsVideoPagesFillSpan(drawing_sprite.sprite_bitmapptr,0,
                TEMPLE_LOGICAL_WIDTH*TEMPLE_LOGICAL_HEIGHT,BLACK);
            /* Distinct colors expose overlaps as well as missing shared-edge rows. */
            for(tile=0;tile<TEST_POLYGON_TILE_COUNT;tile++) {
              if(tile==reverse) {
                TempleRasterPolygon(TEST_POLYGON_UPPER_COLOR,TEST_POLYGON_ALTERNATE_COLOR,
                    TEST_POLYGON_PATTERN,mode,TEST_POLYGON_VERTEX_COUNT,upper);
              } else {
                TempleRasterPolygon(TEST_POLYGON_LOWER_COLOR,TEST_POLYGON_ALTERNATE_COLOR,
                    TEST_POLYGON_PATTERN,mode,TEST_POLYGON_VERTEX_COUNT,lower);
              }
            }
            TempleRasterPresent(dc);
            for(y=0;y<TEMPLE_SCREEN_HEIGHT;y++) {
              /* Integer inequalities independently test pixel centers against edges. */
              render_y=y*TEMPLE_RENDER_HEIGHT/TEMPLE_SCREEN_HEIGHT;
              center_y=2*render_y+1;
              for(x=0;x<TEMPLE_SCREEN_WIDTH;x++) {
                expected=BLACK;
                if(x>=left*scale&&x<right*scale&&
                    x>=drawing_sprite.sprite_raster_left*TEMPLE_RASTER_X_SCALE&&
                    x<drawing_sprite.sprite_raster_right*TEMPLE_RASTER_X_SCALE&&
                    render_y>=clipped_top&&render_y<clipped_bottom&&
                    center_y>=2*top*scale&&center_y<2*bottom*scale) {
                  expected=TEST_POLYGON_LOWER_COLOR;
                  if(center_y<2*shared_y*scale) {
                    expected=TEST_POLYGON_UPPER_COLOR;
                  }
                  if(mode!=TEMPLE_RASTER_PAINT_SOLID) {
                    if(((x/TEST_POLYGON_PATTERN_RUN+render_y)&1)==0) {
                      if(mode==TEMPLE_RASTER_PAINT_TWO_COLOR) {
                        expected=TEST_POLYGON_ALTERNATE_COLOR;
                      }
                    } else if(mode==TEMPLE_RASTER_PAINT_MASKED) {
                      expected=BLACK;
                    }
                  }
                }
                TestAssert(dc->body[y*dc->width_internal+x]==expected);
              }
            }
          }
        }
      }
    }
  }
  TempleRasterNative(FALSE);
  MemCpy(&drawing_sprite,&saved_sprite,sizeof(CSprite));
}

legacy_void TestNativeGeometry(CDC *dc)
{
  CPoint2D vertices[TEST_POLYGON_VERTEX_COUNT];
  legacy_s64 x;
  legacy_s64 y;
  legacy_u8 noreg expected;
  RsVideoPagesFillSpan(
      drawing_sprite.sprite_bitmapptr,0,TEMPLE_LOGICAL_WIDTH*TEMPLE_LOGICAL_HEIGHT,BLACK);
  TempleRasterNative(TRUE);
  /* A one-pixel polygon at odd coordinates cannot survive DOS quantization. */
  TestPolygonQuad(vertices,TEST_POLYGON_NATIVE_PIXEL,TEST_POLYGON_NATIVE_PIXEL,
      TEST_POLYGON_NATIVE_PIXEL+1,TEST_POLYGON_NATIVE_PIXEL+1);
  RsPreRenderDefault(TEST_POLYGON_UPPER_COLOR,TEST_POLYGON_VERTEX_COUNT,vertices);
  TempleRasterPresent(dc);
  for(y=0;y<TEMPLE_RENDER_SCALE;y++) {
    for(x=0;x<TEMPLE_RENDER_SCALE;x++) {
      expected=BLACK;
      if(x==TEST_POLYGON_NATIVE_PIXEL&&y==TEST_POLYGON_NATIVE_PIXEL) {
        expected=TEST_POLYGON_UPPER_COLOR;
      }
      TestAssert(TestRasterPixel(dc,x,y)==expected);
    }
  }
  RsPreRenderLine(TEST_POLYGON_NATIVE_PIXEL,TEST_POLYGON_NATIVE_PIXEL,TEST_POLYGON_NATIVE_PIXEL,
      TEST_POLYGON_NATIVE_PIXEL,TEST_POLYGON_LOWER_COLOR);
  TempleRasterPresent(dc);
  TestAssert(TestRasterPixel(dc,TEST_POLYGON_NATIVE_PIXEL,TEST_POLYGON_NATIVE_PIXEL)==
      TEST_POLYGON_LOWER_COLOR);
  TestAssert(TestRasterPixel(dc,TEST_POLYGON_NATIVE_PIXEL*TEMPLE_RENDER_SCALE,
                TEST_POLYGON_NATIVE_PIXEL*TEMPLE_RENDER_SCALE)==BLACK);
  RsPreRenderSphere(
      TEST_SPHERE_CENTER,TEST_SPHERE_CENTER,TEST_SPHERE_SIZE,TEST_POLYGON_LOWER_COLOR);
  TempleRasterPresent(dc);
  TestAssert(
      TestRasterPixel(dc,TEST_SPHERE_CENTER,TEST_SPHERE_CENTER)==TEST_POLYGON_LOWER_COLOR);
  TestAssert(
      TestRasterPixel(dc,TEST_SPHERE_CENTER+TEST_SPHERE_SIZE/2,TEST_SPHERE_CENTER)==BLACK);
  TestAssert(TestRasterPixel(dc,TEST_SPHERE_CENTER*TEMPLE_RENDER_SCALE,
                TEST_SPHERE_CENTER*TEMPLE_RENDER_SCALE)==BLACK);
  /* Returning to UI drawing restores logical coordinates exactly once. */
  TempleRasterNative(FALSE);
  RsPreRenderDefault(TEST_POLYGON_UPPER_COLOR,TEST_POLYGON_VERTEX_COUNT,vertices);
  TempleRasterPresent(dc);
  for(y=0;y<TEMPLE_RENDER_SCALE;y++) {
    for(x=0;x<TEMPLE_RENDER_SCALE;x++) {
      TestAssert(
          TestRasterPixel(dc,TEST_POLYGON_NATIVE_PIXEL*TEMPLE_RENDER_SCALE+x,
              TEST_POLYGON_NATIVE_PIXEL*TEMPLE_RENDER_SCALE+y)==TEST_POLYGON_UPPER_COLOR);
    }
  }
}
