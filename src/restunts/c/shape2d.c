#ifdef RESTUNTS_DOS

#include <dos.h>
#include <mem.h>
#include <stddef.h>
#include "externs.h"
#include "memmgr.h"
#include "fileio.h"

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

extern char aWindowdefOutOfRowTableSpa[];
extern char aMcgaWindow[];
extern char aWindowReleased[];
extern struct SPRITE far* wndsprite;

extern unsigned char* far wnd_defs; // a reserved memory chunk of 0xE10 bytes in seg012. contents are SPRITE structs followed by lineoffsets. cast to a far pointer for access to the contents in other segments.
extern char* far next_wnd_def; // near pointer relative to seg012 to the current SPRITE in wnd_defs. cast to a far pointer for access to the contents in other segments
extern struct SPRITE far sprite1; // seg012
extern struct SPRITE far sprite2; // seg012

extern void sprite_clear_1_color(unsigned char);	

struct SPRITE far* sprite_make_wnd(unsigned int width, unsigned int height, unsigned int unk) {
	int pages, i;
	char* wnd;
	char* nextwnd;
	struct SPRITE far * farwnd;
	char far* shapebuf;
	struct SHAPE2D far* hdr;
	unsigned int lineofs;
	unsigned int* lineofsptr;
	unsigned int far* farlineofsptr;
	unsigned short wnddefseg;

	wnddefseg = FP_SEG(&wnd_defs);

	pages = ((width * height + sizeof(struct SHAPE2D)) >> 4) + 1;
	shapebuf = mmgr_alloc_pages("MCGA WINDOW", pages);
	
	hdr = (struct SHAPE2D far*)MK_FP(FP_SEG(shapebuf), 0);
	hdr->s2d_width = width;
	hdr->s2d_height = height;
	hdr->s2d_pos_x = 0;
	hdr->s2d_pos_y = 0;
	hdr->s2d_unk1 = 0;
	hdr->s2d_unk2 = 0;

	// it is safe to read/write the pointers to next_wnd_def/wnd_defs, but not the contents
	wnd = next_wnd_def;
	nextwnd = next_wnd_def + sizeof(struct SPRITE) + height * sizeof(unsigned int);
	if (FP_OFF(nextwnd) >= FP_OFF(&wnd_defs) + 0xE10) {
		fatal_error(aWindowdefOutOfRowTableSpa);
	}
	next_wnd_def = nextwnd;

	// get a writable far pointer to the wndsprite
	farwnd = MK_FP(wnddefseg, FP_OFF(wnd));

	lineofsptr = (unsigned int*)(wnd + sizeof(struct SPRITE));
	farwnd->sprite_bitmapptr = hdr;
	farwnd->sprite_lineofs = lineofsptr;
	farwnd->sprite_left = 0;
	farwnd->sprite_left2 = 0;
	farwnd->sprite_right = width;
	farwnd->sprite_pitch = width;	// ??
	farwnd->sprite_top = 0;
	farwnd->sprite_height = height;
	farwnd->sprite_width2 = width;
	farwnd->sprite_widthsum = width;
	
	// create a writable far pointer to the line offsets
	farlineofsptr = MK_FP(wnddefseg, FP_OFF(lineofsptr));
	lineofs = sizeof(struct SHAPE2D);
	for (i = 0; i < height; i++) {
		*farlineofsptr = lineofs;
		farlineofsptr++;
		lineofs += width;
	}

	return farwnd;
}

void sprite_free_wnd(struct SPRITE far* wndsprite) {
	unsigned short spritesize;
	spritesize = sizeof(struct SPRITE) + wndsprite->sprite_height * sizeof(unsigned short);
	if (FP_OFF(wndsprite) + spritesize != FP_OFF(next_wnd_def)) {
		fatal_error(aWindowReleased);
	}
	next_wnd_def = next_wnd_def - spritesize;
	mmgr_release((void far*)wndsprite->sprite_bitmapptr);
}

void sprite_set_1_from_argptr(struct SPRITE far* argsprite) {
	fmemcpy(&sprite1, argsprite, sizeof(struct SPRITE));
}

void sprite_copy_2_to_1() {
	sprite_set_1_from_argptr(&sprite2);
}

void sprite_copy_2_to_1_2() {
	sprite_set_1_from_argptr(&sprite2);
}

void sprite_copy_2_to_1_clear() {
	sprite_set_1_from_argptr(&sprite2);
	sprite_clear_1_color(0);
}

void sprite_copy_wnd_to_1() {
	sprite_set_1_from_argptr(wndsprite);
}

void sprite_copy_wnd_to_1_clear() {
	sprite_set_1_from_argptr(wndsprite);
	sprite_clear_1_color(0);
}

void sprite_copy_both_to_arg(struct SPRITE* argsprite) {
	fmemcpy(argsprite, &sprite1, sizeof(struct SPRITE) * 2);
}

void sprite_copy_arg_to_both(struct SPRITE* argsprite) {
	fmemcpy(&sprite1, argsprite, sizeof(struct SPRITE) * 2);
}

void sprite_clear_1_color(unsigned char color) {
	
	int height, top, left, right, pitch, lines, width, widthdiff, i, j;
	unsigned int ofs;
	unsigned char far* bitmapptr;
	unsigned int far* lineofs;

	top = sprite1.sprite_top;
	left = sprite1.sprite_left;
	right = sprite1.sprite_right;
	pitch = sprite1.sprite_pitch;
	bitmapptr = (unsigned char far*)sprite1.sprite_bitmapptr;
	lineofs = MK_FP(FP_SEG(&sprite1), FP_OFF(sprite1.sprite_lineofs));

	lines = sprite1.sprite_height - top;
	if (lines <= 0) return;

	ofs = lineofs[top] + left;

	width = right - left;
	if (width <= 0) return ;
	
	widthdiff = pitch - width;

	for (i = 0; i < lines; i++) {
		for (j = 0; j < width; j++) {
			bitmapptr[ofs ++] = color;
		}
		ofs += widthdiff;
	}
}

void sprite_putimage(struct SHAPE2D far* shape) {
	//ported_sprite_putimage_(shape);

	int lines, widthdiff, i, j;
	unsigned int ofs;
	unsigned char far* destbitmapptr;
	unsigned int far* destlineofs;
	unsigned char far* srcbitmapptr;

	destbitmapptr = (unsigned char far*)sprite1.sprite_bitmapptr;
	destlineofs = MK_FP(FP_SEG(&sprite1), FP_OFF(sprite1.sprite_lineofs));
	srcbitmapptr = ((unsigned char far*)shape) + sizeof(struct SHAPE2D);

	if (shape->s2d_pos_y + shape->s2d_height > sprite1.sprite_height) {
		lines = sprite1.sprite_height - shape->s2d_pos_y;
	} else {
		lines = shape->s2d_height;
	}

	ofs = destlineofs[shape->s2d_pos_y] + shape->s2d_pos_x;
	widthdiff = sprite1.sprite_pitch - shape->s2d_width;

	for (i = 0; i < lines; i++) {
		for (j = 0; j < shape->s2d_width; j++) {
			destbitmapptr[ofs ++] = *srcbitmapptr++;
		}
		ofs += widthdiff;
	}
	
}

void sprite_putimage_and(struct SHAPE2D far* shape, unsigned short a, unsigned short b) {
	ported_sprite_putimage_and_(shape, a, b);
}

void sprite_putimage_or(struct SHAPE2D far* shape, unsigned short a, unsigned short b) {
	ported_sprite_putimage_or_(shape, a, b);
}

extern void far* sub_3265B(unsigned char far* memchunk, int arg_4); // file_shape2d_res_find()

void file_unflip_shape2d(unsigned char far* memchunk, char far* mempages) {

	int var_2, var_counter;
	unsigned short var_6, var_8, var_A, var_C, var_E, src_height;
	unsigned char far* subres;
	unsigned char subchar;
	int i, j;
	unsigned short arg_0 = FP_OFF(memchunk);
	unsigned short arg_2 = FP_SEG(memchunk);
	unsigned short arg_4 = FP_OFF(mempages);
	unsigned short arg_6 = FP_SEG(mempages);
	unsigned int dseg = FP_SEG(wndsprite); // any object on the dseg will do

	var_2 = *(unsigned short far*)&memchunk[4];
	var_counter = 0;
	do {
		subres = sub_3265B(memchunk, var_counter); // find_shape2d??
		
		subchar = subres[15];
		if ((subchar & 0xF0) == 0) {
			subchar = subres[14] >> 4;
			if (subchar != 0) {
				if (subchar < 4) {
					var_C = *(unsigned short far*)(&subres[0]); // SHAPE2D?
					var_E = *(unsigned short far*)(&subres[2]); // SHAPE2D?
					switch (subchar - 1) {
						case 0:
							// regular flip
							for (j = 0; j < var_E; j++) { // height
								for (i = 0; i < var_C; i++) { // width
									mempages[i + j * var_C] = subres[0x10 + j + i * var_E];
								}
							}
							break;
						case 1:
							// interlaced flip?
							for (j = 0; j < var_E; j += 2) { // height
								for (i = 0; i < var_C; i++) { // width
									mempages[i + j * var_C] = subres[0x10 + (j / 2) + i * var_E];
								}
							}
							for (j = 0; j < var_E; j += 2) { // height
								for (i = 0; i < var_C; i++) { // width
									mempages[var_C + i + j * var_C] = subres[0x10 + ((var_E + j + 1) / 2) + i * var_E];
								}
							}
							break;
						case 2:
							// refer to loc_32BDE in the original function
							fatal_error("unhandled flip type 2");
							break;
					}
					
					// copy flipped bits from mempages -> subres
					for (j = 0; j < var_E; j++) { // height
						for (i = 0; i < var_C; i++) { // width
							subres[0x10 + i + j * var_C] = mempages[i + j * var_C];
						}
					}
				}
			}
		}
		var_counter++;
		var_2--;
	} while (var_2 > 0);
	
/*    asm {

	this is the unimplemented unflip case 2 above:

// switch 2
loc_32BDE:
    mov     bx, dx // dx = row counter
    shr     bx, 1
    add     bx, 10h
    add     bx, [var_6]
    mov     cx, [var_C] // width
    mov     si, [var_E]  // height
    shr     si, 1
    adc     si, 0		// si = (height + 1) / 2

loc_32BF3:
    mov     al, [bx]
    stosb
    add     bx, si
    loop    loc_32BF3

    inc     dx
    cmp     dx, [var_E]
    jz      short loc_32C15 // done

    mov     cx, [var_C]
    mov     si, [var_E]
    shr     si, 1
loc_32C08:
    mov     al, [bx]
    stosb
    add     bx, si
    loop    loc_32C08
    inc     dx
    cmp     dx, [var_E]
    jnz     short loc_32BDE
    */

}

void far* file_load_shape2d(char* shapename, int fatal) {
	char str[100];
	char* strptr;
	char strchar;
	int counter;
	void far* memchunk;
	void far* mempages;
	int unflipsize;

	strcpy(str, shapename);
	strptr = str;
	
	while (*strptr != '.' && *strptr) {
		strptr++;
	}
	
	if (*strptr != 0) {
		fatal_error("unhandled - load_2dshape has dot in the name");
	}

	counter = 0;
	for (counter = 0; shapeexts[counter] != 0; counter++) {

		strcpy(strptr, &shapeexts[counter]);
		memchunk = mmgr_get_chunk_by_name(str);
		if (memchunk) return memchunk; // return existing chunk with same name

		if (file_find(str)) {
			if (strcmp(strptr, ".PVS") == 0) {
				memchunk = file_decomp(str, fatal);
				if (!memchunk) return MK_FP(0, 0);
				
				unflipsize = get_unflip_size(memchunk);
				mempages = mmgr_alloc_pages("UNFLIP", unflipsize);
				file_unflip_shape2d(memchunk, mempages);
				//ported_file_unflip_2dshape_(memchunk, mempages);
				mmgr_release(mempages);
				
				return memchunk;
			}
			// TODO: cases for XVS, PES, ESH
		}
	}
	fatal_error("unhandled - cannot load %s", str);
}

void far* file_load_shape2d_fatal(char* shapename) {
	return file_load_shape2d(shapename, 1);
}

void far* file_load_shape2d_nofatal(char* shapename) {
	return file_load_shape2d(shapename, 0);
}

#endif
