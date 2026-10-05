// Standalone guest regression checks; include from a fresh TempleOS task.
#include "LEGACY.HH"
#define SHAPE2D_RASTER_AND 0
#define SHAPE2D_RASTER_OR 1
#define SHAPE2D_RASTER_COPY 2
#define SHAPE2D_RASTER_MAP 3
#define SHAPE2D_TRANSPARENT_COLOR 255
class CPoint2D
{
  legacy_s16 px,py;
};
class CSprite
{
  legacy_u8 *sprite_bitmapptr,*sprite_lineofs;
  legacy_u16 sprite_buffer_width,sprite_top,sprite_bottom,sprite_raster_left,
      sprite_raster_right;
};
CSprite drawing_sprite,screen_sprite;
legacy_u8 *arena=NULL;
legacy_u8 *RsDosMemoryMakePointer(legacy_u16 segment,legacy_u16 offset)
{
  return arena+segment*16+offset;
}
legacy_u16 RsDosMemoryPointerSegment(legacy_u8 *pointer)
{
  return (pointer(legacy_u8 *)-arena)/16;
}
legacy_s16 RsMmgrGetChunkSize(legacy_u8 *pointer)
{
  no_warn pointer;
  return 4096;
}
#include "PALETTE.HC"
#include "RASTER.HC"
legacy_u8 video_page_count=1;
legacy_u8 video_uses_page_flipping=0;
#include "VIDEO.HC"
legacy_void TemplePump()
{
}
legacy_void TestAssert(legacy_s64 condition)
{
  if(!condition) {
    throw('Raster');
  }
}

#include "TEST_POLYGON.HC"
#include "TEST_LINE.HC"

legacy_void TestPresentChanged(legacy_u8 changed)
{
  CDC *previous=temple_video_front;
  /* Bypass rate limiting so these checks exercise invalidation itself. */
  temple_video_next_present=0;
  TempleVideoPresent();
  TestAssert((temple_video_front!=previous)==changed);
}

legacy_void TestVideoCache(CSprite *background)
{
  legacy_u8 rgb[TEMPLE_PALETTE_CHANNEL_COUNT];
  temple_raster_have_default=FALSE;
  TestPresentChanged(FALSE);
  TestPresentChanged(FALSE);
  RsVideoPagesWritePixel(background->sprite_bitmapptr,0,GREEN);
  TestPresentChanged(FALSE);
  RsVideoPagesWritePixel(screen_sprite.sprite_bitmapptr,0,BLUE);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==BLUE);
  RsVideoPagesCopySpan(screen_sprite.sprite_bitmapptr,0,background->sprite_bitmapptr,0,1);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==GREEN);
  TempleRasterXorPixel(screen_sprite.sprite_bitmapptr,0,BLUE^GREEN);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==BLUE);
  RsVideoPagesFillSpan(screen_sprite.sprite_bitmapptr,0,1,RED);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==RED);
  RsVideoPagesPatternSpan(
      screen_sprite.sprite_bitmapptr,0,1,TEMPLE_RASTER_BYTE_SIGN,GREEN,BLACK,FALSE);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==GREEN);
  RsPreRenderLine(0,0,0,0,RED);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==RED);
  MemCpy(rgb,temple_palette_standard+RED*TEMPLE_PALETTE_CHANNEL_COUNT,sizeof(rgb));
  TemplePaletteSet(TEMPLE_BYTE_MASK,1,rgb);
  RsVideoPagesWritePixel(screen_sprite.sprite_bitmapptr,0,TEMPLE_BYTE_MASK);
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==RED);
  TestAssert(!TempleRasterScreenDirty());
  MemCpy(rgb,temple_palette_standard+GREEN*TEMPLE_PALETTE_CHANNEL_COUNT,sizeof(rgb));
  TemplePaletteSet(TEMPLE_BYTE_MASK,1,rgb);
  TestAssert(!TempleRasterScreenDirty());
  TestPresentChanged(TRUE);
  TestAssert(TestRasterPixel(temple_video_front,0,0)==GREEN);
  TestPresentChanged(FALSE);
  /* Direct raster callers still get a complete conversion of a clean screen. */
  DCFill(temple_video_back,WHITE);
  TempleRasterPresent(temple_video_back);
  TestAssert(temple_video_back->body[0]==BLACK);
  TestAssert(
      temple_video_back->body[(TEMPLE_SCREEN_HEIGHT-1)*temple_video_back->width_internal]==
      BLACK);
  TestAssert(TestRasterPixel(temple_video_back,0,0)==GREEN);
  TestPresentChanged(FALSE);
}

legacy_void RasterTests()
{
  legacy_s64 i,j,y,ly,saw_mixed=0;
  legacy_u8 rows[400],rgb[3],map[256],noreg neutral;
  legacy_u32 *cell,*p,saved[TEMPLE_RASTER_CELL_PIXELS];
  CPoint2D triangle[3];
  CSprite background;
  CDC *dc;
  legacy_u8 *res;
  legacy_u8 noreg mouse_was_shown=ms.show;
  TemplePaletteInit();
  TestAssert(TempleRasterSample(257,NULL)==1);
  TestAssert(TemplePaletteColor(257)==TemplePaletteColor(1));
  for(i=0;i<16;i++) {
    TestAssert(TemplePaletteNearest(temple_palette_standard[i*3],
                  temple_palette_standard[i*3+1],temple_palette_standard[i*3+2])==i);
  }
  rgb[0]=63;
  rgb[1]=0;
  rgb[2]=0;
  TemplePaletteSet(255,65535,rgb);
  TestAssert(TemplePaletteColor(255)==4);
  arena=CAlloc(0x100000);
  for(y=0;y<200;y++) {
    rows[2*y]=(y*320)&255;
    rows[2*y+1]=(y*320)>>8;
  }
  MemSet(&drawing_sprite,0,sizeof(CSprite));
  drawing_sprite.sprite_bitmapptr=arena+0x10000;
  drawing_sprite.sprite_lineofs=rows;
  drawing_sprite.sprite_buffer_width=320;
  drawing_sprite.sprite_bottom=200;
  drawing_sprite.sprite_raster_right=320;
  MemCpy(&screen_sprite,&drawing_sprite,sizeof(CSprite));
  TempleRasterSelect(&drawing_sprite);
  triangle[0].px=0;
  triangle[0].py=0;
  triangle[1].px=7;
  triangle[1].py=0;
  triangle[2].px=0;
  triangle[2].py=7;
  RsPreRenderDefault(12,3,triangle);
  cell=temple_raster_target->samples+5*TEMPLE_RASTER_CELL_PIXELS;
  MemCpy(saved,cell,sizeof(saved));
  for(i=0;i<100;i++) {
    p=temple_raster_target->samples+i*TEMPLE_RASTER_CELL_PIXELS;
    for(j=1;j<TEMPLE_RASTER_CELL_PIXELS;j++) {
      if(p[0]!=p[j]) {
        saw_mixed=1;
      }
    }
  }
  TestAssert(saw_mixed);
  TempleRasterXorPixel(drawing_sprite.sprite_bitmapptr,5,15);
  TempleRasterXorPixel(drawing_sprite.sprite_bitmapptr,5,15);
  TestAssert(!MemCmp(saved,cell,sizeof(saved)));
  neutral=255;
  RsVideoPagesRasterSpan(
      drawing_sprite.sprite_bitmapptr,5,&neutral,0,1,SHAPE2D_RASTER_AND,NULL);
  TestAssert(!MemCmp(saved,cell,sizeof(saved)));
  neutral=0;
  RsVideoPagesRasterSpan(
      drawing_sprite.sprite_bitmapptr,5,&neutral,0,1,SHAPE2D_RASTER_OR,NULL);
  TestAssert(!MemCmp(saved,cell,sizeof(saved)));
  MemSet(map,255,sizeof(map));
  neutral=3;
  RsVideoPagesRasterSpan(
      drawing_sprite.sprite_bitmapptr,5,&neutral,0,1,SHAPE2D_RASTER_MAP,map);
  TestAssert(!MemCmp(saved,cell,sizeof(saved)));
  MemCpy(&background,&drawing_sprite,sizeof(CSprite));
  background.sprite_bitmapptr=arena+0x20000;
  TempleRasterRegisterSprite(&background);
  RsVideoPagesCopySpan(background.sprite_bitmapptr,9,drawing_sprite.sprite_bitmapptr,5,1);
  TestAssert(!MemCmp(saved,
      TempleRasterFind(background.sprite_bitmapptr)->samples+9*TEMPLE_RASTER_CELL_PIXELS,
      sizeof(saved)));
  RsVideoPagesWritePixel(drawing_sprite.sprite_bitmapptr,5,2);
  RsVideoPagesCopySpan(drawing_sprite.sprite_bitmapptr,5,background.sprite_bitmapptr,9,1);
  TestAssert(!MemCmp(saved,cell,sizeof(saved)));
  dc=DCNew(TEMPLE_SCREEN_WIDTH,TEMPLE_SCREEN_HEIGHT);
  TempleRasterPresent(dc);
  for(i=0;i<TEMPLE_SCREEN_PIXELS;i++) {
    TestAssert(dc->body[i]<16);
  }
  TestPolygonSharedEdges(dc);
  TestNativeGeometry(dc);
  TestLineCoverage(dc);
  DCDel(dc);
  for(y=0;y<TEMPLE_RENDER_HEIGHT;y++) {
    ly=y/TEMPLE_RASTER_Y_SCALE;
    TestAssert(TempleRasterY(ly)<=y&&TempleRasterY(ly+1)>y);
  }
  res=arena+0x30000;
  res[4]=1;
  MemCpy(res+6,"!eg0",4);
  res[14]=0;
  res[15]=1;
  res[16]=1;
  for(i=0;i<256;i++) {
    res[30+i]=i;
  }
  TempleRasterResource(res);
  TestAssert(TempleRasterPairs(res+30)[127]==127);
  TempleRasterForget(res);
  TestAssert(!TempleRasterPairs(res+30));
  res[30]=128;
  for(i=0;i<128;i++) {
    res[31+i]=i;
  }
  res[159]=128;
  for(i=0;i<128;i++) {
    res[160+i]=128+i;
  }
  TempleRasterResourceParsed(res);
  TestAssert(TempleRasterPairs(res+30)[255]==255);
  MemCpy(res+6,"test",4);
  TempleRasterResource(res);
  TestAssert(!TempleRasterPairs(res+30));
  temple_raster_have_default=TRUE;
  MemSet(temple_raster_default_pairs,0xCC,sizeof(temple_raster_default_pairs));
  res[30]=255;
  RsVideoPagesRasterSpan(
      drawing_sprite.sprite_bitmapptr,10,res,30,1,SHAPE2D_RASTER_COPY,NULL);
  TestAssert(
      !(temple_raster_target->samples[10*TEMPLE_RASTER_CELL_PIXELS]&TEMPLE_RASTER_PAIR_FLAG));
  TempleRasterSource(res+30);
  RsVideoPagesWritePixel(drawing_sprite.sprite_bitmapptr,11,255);
  TempleRasterSource(NULL);
  TestAssert(
      !(temple_raster_target->samples[11*TEMPLE_RASTER_CELL_PIXELS]&TEMPLE_RASTER_PAIR_FLAG));
  TempleRasterForgetRange(res+31,1);
  TestAssert(!TempleRasterResourceAt(res+30));
  RsDosVideoSetMode13h();
  TestAssert(!ms.show);
  TestAssert(temple_video_front->width==TEMPLE_SCREEN_WIDTH);
  TestAssert(temple_video_front->height==TEMPLE_SCREEN_HEIGHT);
  TestAssert(temple_video_front->flags&DCF_NO_TRANSPARENTS);
  TestAssert(temple_video_back->flags&DCF_NO_TRANSPARENTS);
  TempleVideoPresent();
  TestVideoCache(&background);
  TempleRasterForgetRange(arena+0x1F000,16);
  TestAssert(!TempleRasterFind(drawing_sprite.sprite_bitmapptr));
  TestAssert(TempleRasterFind(background.sprite_bitmapptr)!=NULL);
  TempleVideoShutdown();
  TestAssert(ms.show==mouse_was_shown);
  TestAssert(!temple_video_active&&!temple_video_front&&!temple_raster_surfaces);
  TempleRasterShutdown();
  TemplePaletteShutdown();
  Free(arena);
  "Palette, native raster, and video guest checks passed.\n";
}
RasterTests;
