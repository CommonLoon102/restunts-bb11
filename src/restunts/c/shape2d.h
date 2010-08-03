#ifndef RESTUNTS_SHAPE2D_H
#define RESTUNTS_SHAPE2D_H

#pragma pack (push, 1)
struct SHAPE2D {
	unsigned short s2d_width;
	unsigned short s2d_height;
	unsigned short s2d_unk1;
	unsigned short s2d_unk2;
	unsigned short s2d_pos_x;
	unsigned short s2d_pos_y;
	unsigned char s2d_unk3;
	unsigned char s2d_unk4;
	unsigned char s2d_unk5;
	unsigned char s2d_unk6;
};

struct SPRITE {
	struct SHAPE2D far* sprite_bitmapptr;
	unsigned short sprite_unk1;
	unsigned short sprite_unk2;
	unsigned short sprite_unk3;
	unsigned int* sprite_lineofs;
	unsigned short sprite_left;
	unsigned short sprite_right;
	unsigned short sprite_top;
	unsigned short sprite_height;
	unsigned short sprite_pitch;
	unsigned short sprite_unk4;
	unsigned short sprite_width2;
	unsigned short sprite_left2;
	unsigned short sprite_widthsum;
};
#pragma pack (pop)

#endif
