// Original Broderbund Stunts 1.1 EGA 3D materials; bitmap !eg0 tables are separate.
// See README.TXT for table provenance and the renderer's color domains.
#define TEMPLE_MATERIAL_COUNT (sizeof(material_color_list)/sizeof(legacy_s16))

legacy_s16 temple_material_primary_colors[TEMPLE_MATERIAL_COUNT]={0,1,2,3,4,5,6,7,8,9,10,
    11,12,13,14,15,2,11,15,8,0,14,8,0,14,6,6,6,15,15,15,7,15,3,11,4,12,1,
    0,8,7,3,11,8,0,4,4,12,1,1,9,9,4,4,12,12,0,8,7,7,7,14,14,15,3,3,11,
    11,2,2,10,10,5,5,13,5,7,7,15,15,14,14,14,14,4,4,4,4,5,5,5,5,8,8,8,
    10,15,7,6,4,9,10,2,2,10,10,15,15,6,6,4,6,4,8,13,5,13,0,0,8,0,15,7,
    14,7,4,12,12,15};

legacy_s16 temple_material_pattern_types[TEMPLE_MATERIAL_COUNT]={0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,2,0,2,2,0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,2,0,2,2,0,2,0,2,0,2,0,0,2,0,2,0,0,2,2,0,2,0,2,0,
    0,0,2,0,0,2,2,2,0,2,2,0,2,2,2,0,2,2,2,1,0,0,0,0,0,0,0,2,2,2,2,0,
    2,0,2,2,0,0,2,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0};

legacy_s16 temple_material_pattern_bits[TEMPLE_MATERIAL_COUNT]={-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,30685,-8841,30685,-1,-21931,-30686,-1,
    -21931,-30686,-1,-1,-1,30685,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -21931,-1,-21931,-1,-21931,-21931,-1,-21931,-21846,-21931,-21931,21930,-21931,
    -21931,-21931,0,-21931,-21931,-21931,-21931,-21931,-21931,-21931,0,-21931,0,0,-1,
    -21931,-1,0,-21931,-21931,-21931,0,-21931,-21931,-21931,-21931,-21931,-21931,0,
    -21931,-21931,-21931,30685,-1,-1,-1,-1,-1,-1,-1,-21931,-21931,-21931,-21931,0,
    -21931,0,-21931,-21931,0,0,-21931,0,0,0,0,0,-13261,13260,-18835,18834,-18835,
    18834,-18835,18834,0,0};

legacy_s16 temple_material_secondary_colors[TEMPLE_MATERIAL_COUNT]={0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,11,3,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,9,0,3,4,6,4,12,8,0,8,8,14,7,7,14,0,11,7,9,3,10,15,15,
    0,13,0,0,7,15,7,0,6,8,7,0,0,8,6,6,0,8,0,13,8,0,0,0,0,0,0,0,0,0,10,
    7,3,8,0,7,0,8,8,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

legacy_void TempleMaterialInit()
{
  /* Keep these arrays' identities: extended custom material indices use the
   * shared renderer's explicit adjacency rules for colors and patterns. */
  MemCpy(material_color_list,temple_material_primary_colors,sizeof(material_color_list));
  MemCpy(material_pattern_list,temple_material_pattern_types,sizeof(material_pattern_list));
  MemCpy(material_pattern2_list,temple_material_pattern_bits,sizeof(material_pattern2_list));
  material_clrlist_ptr=material_color_list;
  material_clrlist2_ptr=temple_material_secondary_colors;
  material_patlist_ptr=material_pattern_list;
  material_patlist2_ptr=material_pattern2_list;
}
