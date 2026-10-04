/* Shared HolyC/host regression checks for the original EGA 3D materials. */
#define TEST_MATERIAL_BLUE_FIRST 48
#define TEST_MATERIAL_BLUE_LAST 51
#define TEST_MATERIAL_DARK_BLUE 1
#define TEST_MATERIAL_LIGHT_BLUE 9
#define TEST_MATERIAL_CYAN 3
#define TEST_MATERIAL_BLUE_MIXED 49
#define TEST_MATERIAL_CYAN_MIXED 51
#define TEST_MATERIAL_SOLID 0
#define TEST_MATERIAL_CHECKER 0xAA55
#define TEST_MATERIAL_SIDE 8
#define TEST_MATERIAL_VERTEX_COUNT 4
#define TEST_MATERIAL_REINIT_COUNT 2
#define TEST_MATERIAL_SENTINEL 0xA5
#define TEST_MATERIAL_HASH_OFFSET 2166136261
#define TEST_MATERIAL_HASH_PRIME 16777619
#define TEST_MATERIAL_HASH_MASK 0xFFFFFFFF
#define TEST_MATERIAL_PRIMARY_HASH 0x3C28D83A
#define TEST_MATERIAL_TYPE_HASH 0x761895E7
#define TEST_MATERIAL_BITS_HASH 0x56C5ADC7
#define TEST_MATERIAL_SECONDARY_HASH 0xFD250415

legacy_u32 TestMaterialHash(legacy_s16 *table)
{
  legacy_u64 hash=TEST_MATERIAL_HASH_OFFSET;
  legacy_s64 index;
  legacy_s64 byte;
  legacy_u16 word;
  /* FNV-1a of the DOS little-endian words, independently extracted from the
   * original EGA driver. Hash all entries, including invisible materials. */
  for(index=0;index<TEMPLE_MATERIAL_COUNT;index++) {
    word=table[index]&TEMPLE_WORD_MASK;
    for(byte=0;byte<sizeof(legacy_u16);byte++) {
      hash^=(word>>(byte*TEMPLE_BYTE_BITS))&TEMPLE_BYTE_MASK;
      hash=hash*TEST_MATERIAL_HASH_PRIME&TEST_MATERIAL_HASH_MASK;
    }
  }
  return hash;
}

legacy_void TestMaterialInitialization()
{
  legacy_u8 saved_pairs[TEMPLE_PALETTE_SOURCE_COUNT];
  legacy_u8 saved_have_default=temple_raster_have_default;
  legacy_s64 iteration;
  legacy_s64 index;
  MemCpy(saved_pairs,temple_raster_default_pairs,sizeof(saved_pairs));
  for(iteration=0;iteration<TEST_MATERIAL_REINIT_COUNT;iteration++) {
    /* Exercise both absent and loaded bitmap tables across initialization. */
    temple_raster_have_default=iteration;
    for(index=0;index<TEMPLE_PALETTE_SOURCE_COUNT;index++) {
      temple_raster_default_pairs[index]=index^TEST_MATERIAL_SENTINEL;
    }
    MemSet(material_color_list,TEST_MATERIAL_SENTINEL,sizeof(material_color_list));
    MemSet(material_pattern_list,TEST_MATERIAL_SENTINEL,sizeof(material_pattern_list));
    MemSet(material_pattern2_list,TEST_MATERIAL_SENTINEL,sizeof(material_pattern2_list));
    material_clrlist_ptr=NULL;
    material_clrlist2_ptr=NULL;
    material_patlist_ptr=NULL;
    material_patlist2_ptr=NULL;
    TempleMaterialInit();
    TestMaterialAssert(material_clrlist_ptr==material_color_list);
    TestMaterialAssert(material_clrlist2_ptr==temple_material_secondary_colors);
    TestMaterialAssert(material_patlist_ptr==material_pattern_list);
    TestMaterialAssert(material_patlist2_ptr==material_pattern2_list);
    TestMaterialAssert(TestMaterialHash(material_clrlist_ptr)==TEST_MATERIAL_PRIMARY_HASH);
    TestMaterialAssert(TestMaterialHash(material_patlist_ptr)==TEST_MATERIAL_TYPE_HASH);
    TestMaterialAssert(TestMaterialHash(material_patlist2_ptr)==TEST_MATERIAL_BITS_HASH);
    TestMaterialAssert(TestMaterialHash(material_clrlist2_ptr)==TEST_MATERIAL_SECONDARY_HASH);
    TestMaterialAssert(temple_raster_have_default==iteration);
    for(index=0;index<TEMPLE_PALETTE_SOURCE_COUNT;index++) {
      TestMaterialAssert(temple_raster_default_pairs[index]==(index^TEST_MATERIAL_SENTINEL));
    }
  }
  MemCpy(temple_raster_default_pairs,saved_pairs,sizeof(saved_pairs));
  temple_raster_have_default=saved_have_default;
}

legacy_u8 TestMaterialExpectedColor(legacy_s64 material,legacy_s64 x,legacy_s64 y)
{
  /* DOS blue paint progresses from dark blue to light blue and then cyan.
   * Its checkerboard selects the primary color on even x+y, at native scale. */
  if(material==TEST_MATERIAL_BLUE_FIRST||
      (material==TEST_MATERIAL_BLUE_MIXED&&((x+y)&1)==0)) {
    return TEST_MATERIAL_DARK_BLUE;
  }
  if(material==TEST_MATERIAL_CYAN_MIXED&&((x+y)&1)!=0) {
    return TEST_MATERIAL_CYAN;
  }
  return TEST_MATERIAL_LIGHT_BLUE;
}

legacy_void TestMaterialRaster(CDC *dc)
{
  CPoint2D vertices[TEST_MATERIAL_VERTEX_COUNT];
  legacy_s64 material;
  legacy_s64 x;
  legacy_s64 y;
  legacy_u8 expected;
  vertices[0].px=0;
  vertices[0].py=0;
  vertices[1].px=TEST_MATERIAL_SIDE;
  vertices[1].py=0;
  vertices[2].px=TEST_MATERIAL_SIDE;
  vertices[2].py=TEST_MATERIAL_SIDE;
  vertices[3].px=0;
  vertices[3].py=TEST_MATERIAL_SIDE;
  TempleRasterNative(TRUE);
  for(material=TEST_MATERIAL_BLUE_FIRST;material<=TEST_MATERIAL_BLUE_LAST;material++) {
    RsVideoPagesFillSpan(
        drawing_sprite.sprite_bitmapptr,0,TEMPLE_LOGICAL_WIDTH*TEMPLE_LOGICAL_HEIGHT,BLACK);
    if(material_patlist_ptr[material]==TEST_MATERIAL_SOLID) {
      RsPreRenderDefault(material_clrlist_ptr[material],TEST_MATERIAL_VERTEX_COUNT,vertices);
    } else {
      TestMaterialAssert(
          (material_patlist2_ptr[material]&TEMPLE_WORD_MASK)==TEST_MATERIAL_CHECKER);
      RsPreRenderTwoColor(material_patlist2_ptr[material],material_clrlist2_ptr[material],
          material_clrlist_ptr[material],TEST_MATERIAL_VERTEX_COUNT,vertices);
    }
    TempleRasterPresent(dc);
    for(y=0;y<TEST_MATERIAL_SIDE;y++) {
      for(x=0;x<TEST_MATERIAL_SIDE;x++) {
        expected=TestMaterialExpectedColor(material,x,y);
        TestMaterialAssert(
            dc->body[(y+TEMPLE_RENDER_TOP)*dc->width_internal+x+TEMPLE_RENDER_LEFT]==
            expected);
      }
    }
  }
  TempleRasterNative(FALSE);
}
