#include <dos.h>
#include <stddef.h>
#include "externs.h"
#include "fileio.h"
#include "memmgr.h"
#include "shape3d.h"

/*

TODO:

  lines function (calls)
- ----- --------- -------
X   159 mat_rot_zxy (16)
X    60 mat_multiply (0)
X   122 mat_mul_vector (0)
X    48 mat_invert (0)
X   126 vector_op_unk2 (14)
     97 vector_to_point (0)
X    32 rect_compare_point (0)
     40 vector_op_unk (0)
     86 transformed_shape_op_helper3 (4)
X    33 rect_adjust_from_point(0)
X    47 polarRadius2D (6)
X    13 polarRadius3D (4)
      7 transformed_shape_op_helper2 (0)
     74 transformed_shape_op_helper (0)
X     - polarAngle (0)
X     - mat_rot_z (0)
X     - mat_rot_x (0)
X     - mat_rot_y (0)
      - set_projection (0)


*/

extern char far* game1ptr;
extern char far* game2ptr;
extern char far* curshapeptr;
extern struct SHAPE3D game3dshapes[130];

extern void shape3d_init_shape(char far* shapeptr, struct SHAPE3D* gameshape);
extern unsigned long mmgr_get_res_ofs_diff_scaled(void);

extern char aBarn[];

int shape3d_load_all() {
	int i;
	unsigned long mmgrofsdiff;
	char* shapename;

	game1ptr = 0;
	game2ptr = 0;
	
	mmgrofsdiff = mmgr_get_res_ofs_diff_scaled();
	
	if (mmgrofsdiff < 0xFDE8)	// ??
		return 1;

	game1ptr = file_load_3dres("game1");
	game2ptr = file_load_3dres("game2");
	
	for (i = 0; i < 0x74; i++) {
		shapename = &aBarn[i * 5];
		curshapeptr = locate_shape_nofatal(game1ptr, shapename);
		if (curshapeptr == 0)
			curshapeptr = locate_shape_fatal(game2ptr, shapename);
		shape3d_init_shape(curshapeptr, &game3dshapes[i]);
	}
	return 0;
}

void shape3d_free_all() {
	if (game1ptr != 0)
		mmgr_free(game1ptr);
	if (game2ptr != 0)
		mmgr_free(game2ptr);
}

void shape3d_init_shape(char far* shapeptr, struct SHAPE3D* gameshape) {
	struct SHAPE3DHEADER far* hdr = shapeptr;
	gameshape->shape3d_numverts = hdr->header_numverts;
	gameshape->shape3d_numprimitives = hdr->header_numprimitives;
	gameshape->shape3d_numpaints = hdr->header_numpaints;
	gameshape->shape3d_verts = shapeptr + 4;
	gameshape->shape3d_cull1 = shapeptr + hdr->header_numverts * 6 + 4;
	gameshape->shape3d_cull2 = shapeptr + hdr->header_numprimitives * 4 + hdr->header_numverts * 6 + 4;
	gameshape->shape3d_primitives = shapeptr + hdr->header_numprimitives * 8 + hdr->header_numverts * 6 + 4;
}

extern struct MATRIX* mat_rot_zxy(unsigned, unsigned, unsigned, unsigned);
extern void mat_multiply(struct MATRIX*, struct MATRIX*, struct MATRIX*);
extern void mat_mul_vector(struct VECTOR* vec, struct MATRIX*, struct VECTOR* result);
extern void mat_invert(struct MATRIX*, struct MATRIX*);
extern char vector_op_unk2(struct VECTOR*);
extern void vector_to_point(struct VECTOR*, struct POINT2D*);
extern unsigned rect_compare_point(struct POINT2D*);
extern char transformed_shape_op_helper3(struct POINT2D far*);
extern void vector_op_unk(struct VECTOR*, struct VECTOR*, struct VECTOR*, unsigned);
extern void rect_adjust_from_point(struct POINT2D*, struct RECTANGLE*);
extern unsigned polarRadius2D(unsigned, unsigned);
extern unsigned polarRadius3D(struct VECTOR*);
extern unsigned transformed_shape_op_helper(unsigned, unsigned);
extern unsigned transformed_shape_op_helper2(unsigned, int);
extern void __aFuldiv();

extern unsigned word_40ECE;
extern unsigned transshapenumverts;
extern unsigned char far* transshapeprimitives;
extern struct VECTOR far* transshapeverts;
extern unsigned transshapenumpaints;
extern unsigned char transshapematerial;
extern unsigned char transshapeflags;
extern struct RECTANGLE* transshaperectptr;
extern struct MATRIX mat_temp;
extern long invpow2tbl[32];
extern unsigned char byte_4393D;
extern unsigned word_4394E;
extern unsigned word_45D98;
extern unsigned word_4554A;
extern unsigned word_443F2;
extern unsigned char transshapenumvertscopy;
extern struct POINT2D* polyvertpointptrtab[];
extern unsigned select_rect_param;
extern unsigned char primidxcounttab[];
extern unsigned char primtypetab[];
extern char far* transshapeprimptr;
extern unsigned polyinfoptrnext;
extern char far* polyinfoptr;
extern char far* transshapepolyinfo;
struct POINT2D far* transshapepolyinfopts;
extern char far* polyinfoptrs[];
extern unsigned polyinfonumpolys;
extern char transprimitivepaintjob;
extern unsigned char far* transshapeprimindexptr;
extern char byte_45514;

// ASMVECTOR is used temporarily for the inline assembly since x and y are ambigous struct member names
struct ASMVECTOR {
	int vx, vy, vz;
};

unsigned transformed_shape_op(struct TRANSFORMEDSHAPE3D* arg_transshapeptr) {
	long far* var_cull1;
	long far* var_cull2;
		
	unsigned char var_vertflagtbl[256];
	struct MATRIX* var_rotmatptr;
	struct MATRIX var_mat;
	struct MATRIX var_mat2;
	struct VECTOR var_vec;
	struct VECTOR var_vec2;
	struct VECTOR var_vec3;
	struct VECTOR var_vec4;
	long var_45C;
	long var_A;
	unsigned var_45E, var_460, var_1A;
	unsigned char var_ptrectflag, var_primtype;
	struct VECTOR var_vecarr[255];
	unsigned var_primitiveflags, var_fileprimtype, var_4, var_polyvertcounter, var_C, var_448, var_B7C, var_462;
	int var_polyvertX, var_polyvertY;
	struct POINT2D far* var_transshapepolyinfoptptr;
	long var_18;
	struct POINT2D var_574, var_450;
	struct POINT2D var_vecarr2[255];
	struct POINT2D** var_polyvertunktabptr;
	struct POINT2D far* var_polyvertsptr;

	/*
    var_B7C = word ptr -2940
    var_polyvertunktabptr = word ptr -2938
    var_cull1 = dword ptr -2936
    var_vec4 = VECTOR ptr -2932
    var_vecarr = VECTOR ptr -2926
    var_574 = POINT2D ptr -1396
    var_polyvertY = word ptr -1392
    var_polyvertsptr = dword ptr -1390
    var_vec3 = VECTOR ptr -1386
    var_polyvertX = word ptr -1380
    var_vertflagtbl = byte ptr -1378
    var_462 = word ptr -1122
    var_460 = word ptr -1120
    var_45E = word ptr -1118
    var_45C = dword ptr -1116
    var_vec2 = VECTOR ptr -1112
    var_450 = POINT2D ptr -1104
    var_ptrectflag = byte ptr -1098
    var_448 = word ptr -1096
    var_mat = MATRIX ptr -1094
    var_cull2 = dword ptr -1076
    var_transshapepolyinfoptptr = dword ptr -1072
    var_rotmatptr = word ptr -1068
    var_mat2 = MATRIX ptr -1066
    var_fileprimtype = word ptr -1048
    var_vecarr2 = VECTOR ptr -1046
    var_1A = word ptr -26
    var_18 = dword ptr -24
    var_vec = VECTOR ptr -20
    var_polyvertcounter = word ptr -14
    var_C = word ptr -12
    var_A = dword ptr -10
    var_primtype = byte ptr -6
    var_4 = word ptr -4
    var_primitiveflags = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_transshapeptr = word ptr 6
*/

	unsigned result, i;
	unsigned temp, temp0, temp1;
	const char* errorstr = "errorstr %i";
	const char* errorstr2 = "errorstr2 %i %i";

	//result = ported_transformed_shape_op_(arg_transshapeptr);
	//return result;

	if (word_40ECE != 0) return 1;
	transshapenumverts = arg_transshapeptr->shapeptr->shape3d_numverts;
	transshapeprimitives = arg_transshapeptr->shapeptr->shape3d_primitives;
	transshapeverts = arg_transshapeptr->shapeptr->shape3d_verts;
	transshapenumpaints = arg_transshapeptr->shapeptr->shape3d_numpaints;
	var_cull1 = arg_transshapeptr->shapeptr->shape3d_cull1;
	var_cull2 = arg_transshapeptr->shapeptr->shape3d_cull2;
	transshapematerial = arg_transshapeptr->material;
	if (transshapematerial >= transshapenumpaints)
		transshapematerial = 0;
	transshapeflags = arg_transshapeptr->ts_flags;
	
	if ((transshapeflags & 8) != 0) {
		transshaperectptr = arg_transshapeptr->rectptr;
	}
	
	for (i = 0; i < transshapenumverts; i++) {
		var_vertflagtbl[i] = 0xff;
	}
	
	if ((transshapeflags & 2) == 0) {
		var_rotmatptr = mat_rot_zxy(arg_transshapeptr->rotvec.x, arg_transshapeptr->rotvec.y, arg_transshapeptr->rotvec.z, 0);
		mat_mul_vector(&arg_transshapeptr->pos, &mat_temp, &var_vec);
		mat_multiply(var_rotmatptr, &mat_temp, &var_mat2);
		mat_invert(&var_mat2, &var_mat);
		var_vec2.x = 0;
		var_vec2.y = 0;
		var_vec2.z = 0x1000;
		mat_mul_vector(&var_vec2, &var_mat, &var_vec3);
		if (var_vec3.y <= 0 || arg_transshapeptr->pos.y >= 0 || arg_transshapeptr->unk * 2 <= abs(var_vec.x) || arg_transshapeptr->unk * 2 <= abs(var_vec.z)) {
			byte_4393D = vector_op_unk2(&var_vec3);
			var_45C = invpow2tbl[byte_4393D];
			var_A = invpow2tbl[byte_4393D];
		} else {
			var_45C = -1;
			var_A = 0;
		}
	} else {
		var_rotmatptr = mat_rot_zxy(arg_transshapeptr->rotvec.x, arg_transshapeptr->rotvec.y, arg_transshapeptr->rotvec.z, 0);
		mat_multiply(var_rotmatptr, &mat_temp, &var_mat2);
		var_vec = arg_transshapeptr->pos;
		var_45C = -1;
		var_A = 0;
	}
	
// loc_250A3:
	word_4394E = word_443F2;
	word_45D98 = word_443F2;
	word_4554A = 0;
	var_45E = 0;
	
	if (transshapenumverts <= 8) {
		transshapenumvertscopy = transshapenumverts;
	} else {
		transshapenumvertscopy = 8;
	}
	
	if (transshapenumvertscopy > 4 && transshapeverts[0].y == transshapeverts[4].y) {
		transshapenumvertscopy = 4;
	}

	goto loc_250E6;

loc_250E6:
	var_ptrectflag = 0x0f;
	var_460 = 1;
	var_1A = 0;
	i = 0;
	goto loc_2513F;

loc_2513E:
	i++;
loc_2513F:
	if (transshapenumvertscopy > i) goto loc_2514B;
	//goto loc_251F6;
	if (var_460 != 0 || var_1A == 0 || arg_transshapeptr->unk < abs(var_vec.x)) return -1;
	goto loc_25220;

loc_2514B:	

	polyvertpointptrtab[i] = &var_vecarr2[i];
	var_vec2 = transshapeverts[i];
	if (select_rect_param != 0) {
		var_vec2.x /= 2;
		var_vec2.y /= 2;
		var_vec2.z /= 2;
	}
	mat_mul_vector(&var_vec2, &var_mat2, &var_vec3);
	var_vec3.x += var_vec.x;
	var_vec3.y += var_vec.y;
	var_vec3.z += var_vec.z;
	var_vecarr[i] = var_vec3;
	if (var_vec3.z < 0xc) {
		var_vertflagtbl[i] = 1;
		var_1A = 1;
		goto loc_2513E;
	}
	goto loc_250FA;
	
loc_250FA:
	var_460 = 0;
	var_vertflagtbl[i] = 0;
	vector_to_point(&var_vec3, polyvertpointptrtab[i]);
	if (var_ptrectflag != 0)
		var_ptrectflag &= rect_compare_point(polyvertpointptrtab[i]);
	if (var_ptrectflag != 0) 
		goto loc_2513E;
	goto loc_25220;

	
	
/*
asm {
    cmp     word_40ECE, 0
    jz      short loc_24EB8
loc_24EAE:
    mov     ax, 1

    jmp the_end
    retf

loc_24EB8:
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx + .shapeptr]
    mov     ax, [bx + .shape3d_numverts]
    mov     transshapenumverts, ax

    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     ax, word ptr [bx+.shape3d_primitives]
    mov     dx, word ptr [bx+(.shape3d_primitives+2)]
    mov     word ptr transshapeprimitives, ax
    mov     word ptr transshapeprimitives+2, dx

loc_24ED6:
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     ax, word ptr [bx+.shape3d_verts]
    mov     dx, word ptr [bx+(.shape3d_verts+2)]
    mov     word ptr transshapeverts, ax
    mov     word ptr transshapeverts+2, dx
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     al, byte ptr [bx+.shape3d_numpaints]
    cbw
    mov     transshapenumpaints, ax
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     ax, word ptr [bx+.shape3d_cull1]
    mov     dx, word ptr [bx+(.shape3d_cull1+2)]
    mov     word ptr [var_cull1], ax
    mov     word ptr [var_cull1+2], dx
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     ax, word ptr [bx+.shape3d_cull2]
    mov     dx, word ptr [bx+(.shape3d_cull2+2)]
    mov     word ptr [var_cull2], ax
    mov     word ptr [var_cull2+2], dx
    mov     bx, [arg_transshapeptr]
    mov     al, [bx+.material]
    mov     transshapematerial, al
    cmp     byte ptr transshapenumpaints, al
    ja      short loc_24F32
    mov     transshapematerial, 0
loc_24F32:
    mov     al, [bx+.ts_flags]
    mov     transshapeflags, al
    test    transshapeflags, 8
    jz      short loc_24F45
    mov     ax, [bx+.rectptr]
    mov     transshaperectptr, ax

loc_24F45:
    sub     si, si
    jmp     short loc_24F50

loc_24F4A:
    mov     byte ptr [si+var_vertflagtbl], 0FFh
    inc     si

loc_24F50:

    mov     ax, si
    cmp     ax, transshapenumverts
    jb      short loc_24F4A

    test    byte ptr transshapeflags, 2
    jz      short loc_24FB6
    sub     ax, ax
    push    ax
    mov     bx, [arg_transshapeptr]
    push    word ptr [bx+.rotvec.z]
    push    word ptr [bx+.rotvec.y]
    push    word ptr [bx+.rotvec.x]
    //push    cs

    call far ptr mat_rot_zxy
    add     sp, 8
    mov     [var_rotmatptr], ax
    lea     ax, [var_mat2]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    push    word ptr [var_rotmatptr]
    call    far ptr mat_multiply
    add     sp, 6
    mov     ax, [arg_transshapeptr]
    push    si
    push    di
    lea     di, [var_vec]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
loc_24F9F:
    mov     word ptr [var_45C], 0FFFFh
    mov     word ptr [var_45C+2], 0FFFFh
    sub     ax, ax
    mov     word ptr [var_A+2], ax
    mov     word ptr [var_A], ax
    jmp     loc_250A3
loc_24FB6:
    sub     ax, ax
    push    ax
    mov     bx, [arg_transshapeptr]
    push    word ptr [bx+.rotvec.z]
    push    word ptr [bx+.rotvec.y]
    push    word ptr [bx+.rotvec.x]
    //push    cs
    call far ptr mat_rot_zxy
    add     sp, 8

    mov     [var_rotmatptr], ax
    lea     ax, [var_vec]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    push    word ptr [arg_transshapeptr]
    call    far ptr mat_mul_vector
    add     sp, 6
    lea     ax, [var_mat2]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    push    word ptr [var_rotmatptr]
    call    far ptr mat_multiply
    add     sp, 6
    lea     ax, [var_mat]
    push    ax
    lea     ax, [var_mat2]
    push    ax
    call    far ptr mat_invert
    add     sp, 4
    mov     word ptr [var_vec2.vx], 0
    mov     word ptr [var_vec2.vy], 0
    mov     word ptr [var_vec2.vz], 1000h
    lea     ax, [var_vec3]
    push    ax
    lea     ax, [var_mat]
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr mat_mul_vector
    add     sp, 6
    cmp     word ptr [var_vec3.y], 0
    jle     short loc_25046
    mov     bx, [arg_transshapeptr]
    cmp     word ptr [bx+.pos.y], 0
    jge     short loc_25046
    jmp     loc_24F9F
loc_25046:
    push    word ptr [var_vec.vx] // int
    call    far ptr abs
    add     sp, 2
    mov     bx, [arg_transshapeptr]
    mov     cx, [bx+.unk]
    shl     cx, 1
    cmp     cx, ax
    jle     short loc_25077
    push    word ptr [var_vec.z] // int
    call    far ptr abs
    add     sp, 2
    mov     bx, [arg_transshapeptr]
    mov     cx, [bx+.unk]
    shl     cx, 1
    cmp     cx, ax
    jle     short loc_25077
    jmp     loc_24F9F
loc_25077:
    lea     ax, [var_vec3]
    push    ax
    //push    cs
    call far ptr vector_op_unk2
    add     sp, 2
    mov     byte_4393D, al
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    mov     ax, word ptr invpow2tbl[bx]
    mov     dx, word ptr (invpow2tbl+2)[bx]
    mov     word ptr [var_45C], ax
    mov     word ptr [var_45C+2], dx
    mov     word ptr [var_A], ax
    mov     word ptr [var_A+2], dx

loc_250A3:
    mov     ax, word_443F2
    mov     word_4394E, ax
    mov     word_45D98, ax
    mov     word_4554A, 0
    mov     word ptr [var_45E], 0
    cmp     transshapenumverts, 8
    jbe     short loc_250C6
    mov     transshapenumvertscopy, 8
    jmp     short loc_250CC

loc_250C6:
    mov     al, byte ptr transshapenumverts
    mov     transshapenumvertscopy, al
loc_250CC:
    cmp     transshapenumvertscopy, 4
    jbe     short loc_250E6
    les     bx, transshapeverts
    mov     ax, es:[bx+1Ah]
    cmp     es:[bx+2], ax
    jnz     short loc_250E6
    mov     transshapenumvertscopy, 4

}

loc_250E6:
	asm {
    mov     byte ptr [var_ptrectflag], 0Fh
    mov     word ptr [var_460], 1
    mov     word ptr [var_1A], 0
    sub     si, si
    jmp     short loc_2513F


loc_2513E:
    inc     si
loc_2513F:
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     ax, si
    ja      short loc_2514B
    jmp     loc_251F6
loc_2514B:

    mov     bx, si
    shl     bx, 1 // * 2 = sizeof ptr

    mov     ax, si
    shl     ax, 1
    shl     ax, 1 // * 4? (sizeof(POINT2D)) ??
    
    add ax, bp
    add ax, offset var_vecarr2 // LOOKS STRANGE IN ASSEMBEL - this is maybe similar to sub ax, 0xyxy
    //add     ax, bp
    //sub     ax, 416h        // array access in var_416, but dunno how to make IDA show this

	mov     polyvertpointptrtab[bx], ax
    

    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1 // * 6 = sizeof(VECTOR)
    add     ax, word ptr transshapeverts
    mov     dx, word ptr transshapeverts+2
    push    si
    push    di
    lea     di, [var_vec2]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    cmp     select_rect_param, 0
    jz      short loc_25196
    sar     word ptr [var_vec2.vx], 1
    sar     word ptr [var_vec2.vy], 1
    sar     word ptr [var_vec2.vz], 1
loc_25196:

    lea     ax, [var_vec3]
    push    ax
    lea     ax, [var_mat2]
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr mat_mul_vector
    add     sp, 6
    mov     ax, word ptr [var_vec.vx]  // DOES NOT APPEAR IN ASSEMBLE WO word ptr
    add     word ptr [var_vec3.vx], ax
    mov     ax, word ptr [var_vec.vy]
    add     word ptr [var_vec3.vy], ax
    mov     ax, word ptr [var_vec.vz]
    add     word ptr [var_vec3.vz], ax
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1           // bx = vertex index * 6
    add     bx, bp
    push    si
    push    di
    lea     di, [bx+offset var_vecarr] // DOES NOT APPEAR IN ASSEMBEL
    lea     si, [var_vec3]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    cmp     word ptr [var_vec3.vz], 0Ch
    jl      short loc_251E9
//    jmp     loc_250FA

//loc_250FA:

    mov     word ptr [var_460], 0
    mov     byte ptr [si+var_vertflagtbl], 0
    mov     bx, si
    shl     bx, 1
    push    word ptr polyvertpointptrtab[bx]
    lea     ax, [var_vec3]
    push    ax
    call    far ptr vector_to_point
    add     sp, 4
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_25134
    mov     bx, si
    shl     bx, 1
    push    word ptr polyvertpointptrtab[bx]
    //push    cs
    call far ptr rect_compare_point
    add     sp, 2
    and     byte ptr [var_ptrectflag], al
loc_25134:
    cmp     byte ptr [var_ptrectflag], 0
    jnz     short loc_2513E
    jmp     loc_25220

}

loc_251E9:
asm {

    mov     byte ptr [si+var_vertflagtbl], 1
    mov     word ptr [var_1A], 1
    jmp     loc_2513E // continue
}
    
// end of loop, for i = 0 .. transshapenumvertscopy

loc_251F6:
	asm {

    cmp     word ptr [var_460], 0
    jnz     short _done_ret_neg1
    cmp     word ptr [var_1A], 0
    jz      short _done_ret_neg1
    push    word ptr [var_vec.vx] // int
    call    far ptr abs
    add     sp, 2
    mov     bx, [arg_transshapeptr]
    cmp     [bx+.unk], ax
    jge     short loc_25220

asm {
_done_ret_neg1:
    mov     ax, 0FFFFh
    //pop     si
    //pop     di
    jmp the_end
    retf
	}
*/

loc_25220: // // in the loop, for i = 0 .. transshapenumvertscopy
	
	transshapeprimitives = arg_transshapeptr->shapeptr->shape3d_primitives;
	
/*		
	asm {
    mov     bx, [arg_transshapeptr]
    mov     bx, [bx+.shapeptr]
    mov     ax, word ptr [bx+.shape3d_primitives]
    mov     dx, word ptr [bx+(.shape3d_primitives+2)]
    mov     word ptr transshapeprimitives, ax
    mov     word ptr transshapeprimitives+2, dx*/

loc_25233:
	transshapeprimptr = transshapeprimitives + primidxcounttab[transshapeprimitives[0]] + transshapenumpaints + 2;
	var_primitiveflags = transshapeprimitives[1];
	var_4 = 0;
	if ((var_cull1[0] & var_45C) != 0) {
		goto loc_25282;
	}
	goto loc_25801;
/*
asm {
	les     bx, transshapeprimitives
    mov     bl, es:[bx]     // primitives+0 = primitive type
    sub     bh, bh

    mov     al, primidxcounttab[bx] // look up maybe indexcount from a table? FELL OUT OF ASSEMBY
    sub     ah, ah
    add     ax, transshapenumpaints
    add     ax, word ptr transshapeprimitives
    mov     dx, es
    add     ax, 2
    mov     word ptr transshapeprimptr, ax
    mov     word ptr transshapeprimptr+2, dx
    mov     bx, word ptr transshapeprimitives
    mov     al, es:[bx+1]   // primitives+1 = primitive flags
    sub     ah, ah
    mov     [var_primitiveflags], ax
    mov     word ptr [var_4], 0
    les     bx, [var_cull1]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    and     ax, word ptr [var_45C]
    and     dx, word ptr [var_45C+2]
    or      dx, ax
    jnz     short loc_25282

    jmp     loc_25801
}*/
loc_25282:

	var_fileprimtype = transshapeprimitives[0];
	transshapenumvertscopy = primidxcounttab[var_fileprimtype];
	var_primtype = primtypetab[var_fileprimtype];

	transshapepolyinfo = polyinfoptr + polyinfoptrnext;
	polyinfoptrs[polyinfonumpolys] = transshapepolyinfo;

	transprimitivepaintjob = transshapeprimitives[2 + transshapematerial];
	transshapeprimitives += 2 + transshapenumpaints; // <- skip header and materials, -> point at indices

	var_ptrectflag = 0x0f;
	var_460 = 1;
	var_1A = 0;
	transshapeprimindexptr = transshapeprimitives;
	var_polyvertcounter = 0;
	goto loc_25328;

/*
asm {
    les     bx, transshapeprimitives
    mov     al, es:[bx]     // primitives+0 = type
    sub     ah, ah
    mov     [var_fileprimtype], ax
    mov     bx, ax
    mov     al, primidxcounttab[bx]
    mov     transshapenumvertscopy, al
    mov     al, primtypetab[bx]
    mov     [var_primtype], al   // primunktab maps from file-based primitive type to internal type:

    mov     ax, polyinfoptrnext
    add     ax, word ptr polyinfoptr
    mov     dx, word ptr polyinfoptr+2
    mov     word ptr transshapepolyinfo, ax
    mov     word ptr transshapepolyinfo+2, dx

    mov     bx, polyinfonumpolys
    shl     bx, 1
    shl     bx, 1
    mov     word ptr polyinfoptrs[bx], ax
    mov     word ptr (polyinfoptrs+2)[bx], dx

    mov     bl, transshapematerial
    sub     bh, bh
    add     bx, word ptr transshapeprimitives
    mov     es, word ptr transshapeprimitives+2
    mov     al, es:[bx+2]   // primitives+2+X = paint job color, X in [0..numpaints]
    mov     transprimitivepaintjob, al

    mov     ax, transshapenumpaints
    add     ax, 2
    add     word ptr transshapeprimitives, ax // <- skip header and materials, -> point at indices

    mov     byte ptr [var_ptrectflag], 0Fh
    mov     word ptr [var_460], 1
    mov     word ptr [var_1A], 0
    mov     ax, word ptr transshapeprimitives
    mov     dx, es
    mov     word ptr transshapeprimindexptr, ax
    mov     word ptr transshapeprimindexptr+2, dx
    mov     word ptr [var_polyvertcounter], 0
    jmp     short loc_25328
*/
loc_25304:
	var_460 = 0;
/*asm {
    mov     word ptr [var_460], 0
}*/
loc_2530A:
	if (var_ptrectflag != 0) {
		var_ptrectflag &= rect_compare_point(polyvertpointptrtab[var_polyvertcounter]);
	}
/*asm {
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_25325
    mov     bx, [var_polyvertcounter]
    shl     bx, 1
    push    word ptr polyvertpointptrtab[bx]
    //push    cs
    call far ptr rect_compare_point
    add     sp, 2
    and     [var_ptrectflag], al
}*/
loc_25325:
	var_polyvertcounter++;
/*asm {
    inc     word ptr [var_polyvertcounter]
}*/
loc_25328:
	if (var_polyvertcounter >= transshapenumvertscopy) goto loc_2542A;
/*asm {
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     [var_polyvertcounter], ax
    jb      short loc_25335

    jmp     loc_2542A
}*/

loc_25335:
	temp = transshapeprimindexptr[0];
	//fatal_error("%i", temp);
	transshapeprimindexptr++;
	polyvertpointptrtab[var_polyvertcounter] = &var_vecarr2[temp];
	
	if (var_vertflagtbl[temp] == 0xff) { 
		goto loc_25370; 
	}
	if (var_vertflagtbl[temp] == 0) { 
		goto loc_25304;
	}
	if (var_vertflagtbl[temp] == 1) { 
		var_1A = 1;
		goto loc_25325;
	}
	//goto loc_2536E; ->
	goto loc_25325;


/*asm {
	//mov si, temp
	//sub ah,ah
    mov     bx, word ptr transshapeprimindexptr
    inc     word ptr transshapeprimindexptr
    mov     es, word ptr transshapeprimindexptr+2
    mov     al, es:[bx]
    mov     si, ax
    mov     bx, [var_polyvertcounter]
    shl     bx, 1
    shl     ax, 1
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr2
    //add     ax, bp
    //sub     ax, 416h
    mov     polyvertpointptrtab[bx], ax
    mov     al, [si+var_vertflagtbl]
    cbw
    cmp     ax, 0FFFFh
    jz      short loc_25370
    or      ax, ax
    jz      short loc_25304
    cmp     ax, 1
    jnz     short loc_2536E
    jmp     loc_253FD
loc_2536E:
    jmp     short loc_25325
}
*/
loc_25370:

	var_vec2 = transshapeverts[temp];
	if (select_rect_param != 0) {
		var_vec2.x /= 2;
		var_vec2.y /= 2;
		var_vec2.z /= 2;
	}
	mat_mul_vector(&var_vec2, &var_mat2, &var_vec3);
	var_vec3.x += var_vec.x;
	var_vec3.y += var_vec.y;
	var_vec3.z += var_vec.z;
	var_vecarr[temp] = var_vec3;

	if (var_vec3.z >= 0xc) {
		// goto loc_25406; ->
		var_460 = 0;
		var_vertflagtbl[temp] = 0;
		vector_to_point(&var_vec3, polyvertpointptrtab[var_polyvertcounter]);
		goto loc_2530A;

	}
	var_vertflagtbl[temp] = 1;
	var_1A = 1;
	goto loc_25325;

/*	
asm {
	mov si, temp
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr transshapeverts
    mov     dx, word ptr transshapeverts+2
    push    si
    push    di
    lea     di, [var_vec2]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    cmp     select_rect_param, 0
    jz      short loc_253A8
    sar     word ptr [var_vec2.vx], 1
    sar     word ptr [var_vec2.vy], 1
    sar     word ptr [var_vec2.vz], 1

loc_253A8:
    lea     ax, [var_vec3]
    push    ax
    lea     ax, [var_mat2]
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr mat_mul_vector
    add     sp, 6

    mov     ax, word ptr [var_vec.vx]
    add     word ptr [var_vec3.vx], ax
    mov     ax, word ptr [var_vec.vy]
    add     word ptr [var_vec3.vy], ax
    mov     ax, word ptr [var_vec.vz]
    add     word ptr [var_vec3.vz], ax
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    push    si
    push    di
    lea     di, [bx+offset var_vecarr]
    lea     si, [var_vec3]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si

    cmp     word ptr [var_vec3.z], 0Ch
    jge     short loc_25406
    mov     byte ptr [si+var_vertflagtbl], 1
}

loc_253FD:
asm {
    mov     word ptr [var_1A], 1
    jmp     loc_25325
}

loc_25406:
asm {
	mov si, temp
    mov     word ptr [var_460], 0
    mov     byte ptr [si+var_vertflagtbl], 0
    mov     bx, [var_polyvertcounter]
    shl     bx, 1
    push    word ptr polyvertpointptrtab[bx]
    lea     ax, [var_vec3]
    push    ax
    call    far ptr vector_to_point
    add     sp, 4
    jmp     loc_2530A
}*/

// end of 2nd loop


loc_2542A:

	if (var_460 != 0) goto loc_25801;
	if (var_ptrectflag != 0 && var_1A != 0) goto loc_25801;
	if (var_primtype == 0) goto _primtype_poly;
	if (var_primtype == 1) goto _primtype_line;
	if (var_primtype == 2) goto _primtype_sphere;
	if (var_primtype == 3) goto _primtype_wheel;
	if (var_primtype == 5) goto loc_25CE0;
	goto loc_25801;

/*
asm {
    cmp     word ptr [var_460], 0
    jz      short loc_25434
    jmp     loc_25801
loc_25434:
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_25444
    cmp     word ptr [var_1A], 0
    jnz     short loc_25444
    jmp     loc_25801
loc_25444:
    mov     al, [var_primtype]
    sub     ah, ah
    or      ax, ax
    jz      short _primtype_poly  // al = 0 for polygons,
    cmp     ax, 1           // 1 = lines
    jnz     short loc_25455
    jmp     _primtype_line
loc_25455:
    cmp     ax, 2
    jnz     short loc_2545D
    jmp     _primtype_sphere // 2 = sphere
loc_2545D:
    cmp     ax, 3
    jnz     short loc_25465
    jmp     _primtype_wheel // 3 = wheel
loc_25465:
    cmp     ax, 5
    jnz     short loc_2546D
    jmp     loc_25CE0       // 5 = particle
loc_2546D:
    jmp     loc_25801       // everything else? (4?)

// ------------------------------------ primtype_poly ------------------------------------
}
*/

_primtype_poly:
	var_transshapepolyinfoptptr = transshapepolyinfo + 6;
	transshapeprimindexptr = transshapeprimitives;

	var_18 = 0;
	var_ptrectflag = 0x0f;

	if (var_1A != 0) goto loc_25518;
	i = 0;
	goto loc_254A7;
loc_254A6:
	i++;
loc_254A7:
	if (transshapenumvertscopy <= i) goto loc_2571A;
	var_C = transshapeprimindexptr[0];
	transshapeprimindexptr++;
	var_18 += var_vecarr[var_C].z;
	var_polyvertunktabptr = &polyvertpointptrtab[i];
	*var_transshapepolyinfoptptr = **var_polyvertunktabptr;
	if (var_ptrectflag != 0) {
		var_ptrectflag &= rect_compare_point(*var_polyvertunktabptr);
	}
	
	var_transshapepolyinfoptptr++;
	goto loc_254A6;

	/*
asm {
    mov     ax, word ptr transshapepolyinfo
    mov     dx, word ptr transshapepolyinfo+2
    add     ax, 6
    mov     word ptr [var_transshapepolyinfoptptr], ax
    mov     word ptr [var_transshapepolyinfoptptr+2], dx
    mov     ax, word ptr transshapeprimitives
    mov     dx, word ptr transshapeprimitives+2
    mov     word ptr transshapeprimindexptr, ax
    mov     word ptr transshapeprimindexptr+2, dx
    sub     ax, ax
    mov     word ptr [var_18+2], ax
    mov     word ptr [var_18], ax
    mov     byte ptr [var_ptrectflag], 0Fh
    cmp     [var_1A], ax
    jnz     short loc_25518
    sub     si, si
    jmp     short loc_254A7
loc_254A6:
    inc     si
loc_254A7:
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     ax, si
    ja      short loc_254B3
    jmp     loc_2571A

loc_254B3:
//	mov ax, [var_polyvertunktabptr]
    mov     bx, word ptr transshapeprimindexptr
    inc     word ptr transshapeprimindexptr
    mov     es, word ptr transshapeprimindexptr+2
    mov     al, es:[bx]
    mov     [var_C], ax
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx+offset var_vecarr.z]
    cwd
    add     word ptr [var_18], ax
    adc     word ptr [var_18+2], dx
    mov     ax, si
    shl     ax, 1
    add     ax, offset polyvertpointptrtab
    mov     [var_polyvertunktabptr], ax // 
    mov     bx, ax // ax = bx = POINT2D**
    mov     bx, [bx] // bx = POINT2D*
    mov     ax, [bx] // ax = x
    mov     dx, [bx+2] // dx = y
    les     bx, [var_transshapepolyinfoptptr]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_25511
    mov     bx, [var_polyvertunktabptr]
    push    word ptr [bx]
    call far ptr rect_compare_point
    add     sp, 2
    and     [var_ptrectflag], al
loc_25511:
    add     word ptr [var_transshapepolyinfoptptr], 4
    jmp     short loc_254A6
}
*/
loc_25518:

	var_polyvertcounter = 0;
	var_448 = transshapeprimitives[transshapenumvertscopy - 1];
	i = 0;
	goto loc_255EE;
/*
asm {
    mov     word ptr [var_polyvertcounter], 0
    mov     bl, transshapenumvertscopy
    sub     bh, bh
    add     bx, word ptr transshapeprimitives
    mov     es, word ptr transshapeprimitives+2
    mov     al, es:[bx-1]
    sub     ah, ah
    mov     [var_448], ax
    sub     si, si
    jmp     loc_255EE
}*/
loc_2553A:


	if (var_vertflagtbl[var_448] != 0) goto loc_255E6;

	vector_op_unk(&var_vecarr[var_448], &var_vecarr[var_C], &var_vec2, 0x0C);
	vector_to_point(&var_vec2, &var_574);

	if (var_574.px == var_vecarr2[var_448].px && var_574.py == var_vecarr2[var_448].py) goto loc_255E6;

	if (var_ptrectflag != 0) {
		var_ptrectflag &= rect_compare_point(&var_574);
	}
	
	*var_transshapepolyinfoptptr = var_574;
	
loc_255DE:
	var_transshapepolyinfoptptr++;
	var_polyvertcounter++;
/*	
	asm {
    mov     bx, [var_448]
    add     bx, bp

    add bx, offset var_vertflagtbl
    mov al, [bx]
    cmp     al, 0  // does not compile!!!
    jz      short loc_2554A
    jmp     loc_255E6
loc_2554A:
    mov     ax, 0Ch
    push    ax
    lea     ax, [var_vec2]
    push    ax
    mov     ax, [var_C]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    mov     ax, [var_448]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    call    far ptr vector_op_unk
    add     sp, 8
    lea     ax, [var_574]
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr vector_to_point
    add     sp, 4
    mov     ax, [var_448]
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    mov     [var_B7C], ax
mov ax, var_B7C
    mov     bx, ax
    mov     ax, word ptr [var_574.px]
    cmp     [bx+offset var_vecarr2.vx], ax
    jnz     short loc_255B4
    mov     ax, word ptr [var_574.py]
    cmp     [bx+offset var_vecarr2.y], ax
    jz      short loc_255E6
loc_255B4:
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_255CB
    lea     ax, [var_574]
    push    ax
    ;push    cs
    call far ptr rect_compare_point
    add     sp, 2
    and     [var_ptrectflag], al

loc_255CB:
    les     bx, [var_transshapepolyinfoptptr]
    mov     ax, word ptr [var_574.px]
    mov     dx, word ptr [var_574.py]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
loc_255DE:
    add     word ptr [var_transshapepolyinfoptptr], 4
    inc     word ptr [var_polyvertcounter]
}
*/

loc_255E6:
	var_448 = var_C;
	i++;
/*asm {
    mov     ax, [var_C]
    mov     [var_448], ax
    inc     si
}*/
loc_255EE:

	if (transshapenumvertscopy <= i) goto loc_25714;
	var_C = transshapeprimindexptr[0];
	transshapeprimindexptr++;

	var_18 += var_vecarr[var_C].z;

	if (var_vertflagtbl[var_C] != 0) goto loc_2553A;

	if (var_vertflagtbl[var_448] == 0) goto loc_256D7;
/*asm {
	
mov si, i
	
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     ax, si
    ja      short loc_255FA
    jmp     loc_25714
loc_255FA:
    mov     bx, word ptr transshapeprimindexptr
    inc     word ptr transshapeprimindexptr
    mov     es, word ptr transshapeprimindexptr+2
    mov     al, es:[bx]
    mov     [var_C], ax

//mov ax, var_C
//mov si, i
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    mov     [var_polyvertunktabptr], ax

mov cx, var_C
    mov     ax, [var_polyvertunktabptr]
    mov     bx, ax
    mov     ax, [bx+offset var_vecarr.z]
    cwd
    add     word ptr [var_18], ax
    adc     word ptr [var_18+2], dx

mov cx, var_C
    mov     bx, cx
    add     bx, bp
    add bx, offset var_vertflagtbl
    mov al, [bx]
    cmp     al, 0 // issue
    jz      short loc_25635
    jmp     loc_2553A

loc_25635:
    mov     bx, [var_448]
    add     bx, bp
    add bx, offset var_vertflagtbl
    mov al, [bx]
    cmp     al, 0 // issue
    jnz     short loc_25645
    jmp     loc_256D7
loc_25645:

}
*/
// &var_vecarr[var_C] is equivalent to var_polyvertunktabptr-stuff+var_vecarr... becoz var_polyvertunktabptr is an array-indexer relative to the stack frame
	vector_op_unk(&var_vecarr[var_C], &var_vecarr[var_448], &var_vec2, 0x0C);
	vector_to_point(&var_vec2, &var_574);

	if (var_574.px == var_vecarr2[var_C].px && var_574.py == var_vecarr2[var_C].py) goto loc_256D7;

	if (var_ptrectflag != 0) {
		var_ptrectflag &= rect_compare_point(&var_574);
	}
	
	*var_transshapepolyinfoptptr = var_574;
	var_transshapepolyinfoptptr++;
	var_polyvertcounter++;
/*
asm {
    mov     ax, 0Ch
    push    ax
    lea     ax, [var_vec2]
    push    ax
    mov     ax, [var_448]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    mov     ax, [var_polyvertunktabptr]
    ;sub     ax, 0B6Eh
    add ax, offset var_vecarr
    push    ax
    call    far ptr vector_op_unk
    add     sp, 8
    lea     ax, [var_574]
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr vector_to_point
    add     sp, 4
    mov     ax, [var_C]
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    mov     [var_B7C], ax // another wicked bp-relative indexer thingy
    
mov ax, var_B7C
    mov     bx, ax
    mov     ax, word ptr [var_574.px]
    cmp     [bx+offset var_vecarr2.vx], ax
    jnz     short loc_256A5
    mov     ax, word ptr [var_574.py]
    cmp     [bx+offset var_vecarr2.y], ax
    jz      short loc_256D7
loc_256A5:
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_256BC
    lea     ax, [var_574]
    push    ax
    ;push    cs
    call far ptr rect_compare_point
    add     sp, 2
    and     [var_ptrectflag], al
loc_256BC:
    les     bx, [var_transshapepolyinfoptptr]
    mov     ax, word ptr [var_574.px]
    mov     dx, word ptr [var_574.py]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    add     word ptr [var_transshapepolyinfoptptr], 4
    inc     word ptr [var_polyvertcounter]

}
*/
loc_256D7:
	*var_transshapepolyinfoptptr = *polyvertpointptrtab[i];
	if (var_ptrectflag != 0) {
		var_ptrectflag &= rect_compare_point(polyvertpointptrtab[i]);
	}
	goto loc_255DE;
/*asm {
mov si, i
    mov     ax, si
    shl     ax, 1
    add     ax, offset polyvertpointptrtab
    mov     [var_B7C], ax
    mov     bx, ax
    mov     bx, [bx]
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, [var_transshapepolyinfoptptr]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    cmp     byte ptr [var_ptrectflag], 0
    jnz     short loc_25700
    jmp     loc_255DE
loc_25700:
    mov     bx, [var_B7C]
    push    word ptr [bx]
    ;push    cs
    call far ptr rect_compare_point
    add     sp, 2
    and     [var_ptrectflag], al
    jmp     loc_255DE
}*/
loc_25714:
	transshapenumvertscopy = var_polyvertcounter;
	
/*asm {
	mov si, i
	
    mov     al, byte ptr [var_polyvertcounter]
    mov     transshapenumvertscopy, al
}*/
loc_2571A:

	if (transshapenumvertscopy == 0) goto loc_25801;
	if (var_ptrectflag != 0) goto loc_25801;
	if ((var_primitiveflags & 1) != 0) goto loc_25760;
	if ((var_A & *var_cull2) != 0) goto loc_25760;
	
	if (transformed_shape_op_helper3(transshapepolyinfo + 6) == 0) goto loc_25763;

/*asm {
    cmp     transshapenumvertscopy, 0
    jnz     short loc_25724
    jmp     loc_25801
loc_25724:
    cmp     byte ptr [var_ptrectflag], 0
    jz      short loc_2572E
    jmp     loc_25801
loc_2572E:
    test    byte ptr [var_primitiveflags], 1
    jnz     short loc_25760
    les     bx, [var_cull2]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    and     ax, word ptr [var_A]
    and     dx, word ptr [var_A+2]
    or      dx, ax
    jnz     short loc_25760
    mov     ax, word ptr transshapepolyinfo
    mov     dx, word ptr transshapepolyinfo+2
    add     ax, 6
    push    dx
    push    ax
    ;push    cs
    call far ptr transformed_shape_op_helper3
    add     sp, 4
    or      al, al
    jz      short loc_25763
}*/
loc_25760:
	var_4++;
/*asm {
    inc     word ptr [var_4]
}*/
loc_25763:

	if (var_4 == 0) goto loc_25801;
	if ((transshapeflags & 8) == 0) goto loc_25801;
	
	var_polyvertsptr = transshapepolyinfo + 6;
	var_polyvertcounter = 0;
	goto loc_257F7;

/*asm {
    cmp     word ptr [var_4], 0
    jnz     short loc_2576C
    jmp     loc_25801
loc_2576C:
    test    transshapeflags, 8
    jnz     short loc_25776
    jmp     loc_25801
loc_25776:
    mov     ax, word ptr transshapepolyinfo
    mov     dx, word ptr transshapepolyinfo+2
    add     ax, 6
    mov     word ptr [var_polyvertsptr], ax
    mov     word ptr [var_polyvertsptr+2], dx
    mov     word ptr [var_polyvertcounter], 0
    jmp     short loc_257F7
}*/

loc_25790:

	var_polyvertX = var_polyvertsptr->px;
	var_polyvertY = var_polyvertsptr->py;
	var_polyvertsptr++;
	
	if (var_polyvertX < transshaperectptr->x1) {
		transshaperectptr->x1 = var_polyvertX;
	}
	
	if (transshaperectptr->y1 < var_polyvertX + 1) {
		transshaperectptr->y1 = var_polyvertX + 1;
	}
	
	if (transshaperectptr->x2 > var_polyvertY) {
		transshaperectptr->x2 = var_polyvertY;
	}
	
	if (transshaperectptr->y2 < var_polyvertY + 1) {
		transshaperectptr->y2 = var_polyvertY + 1;
	}
	
	var_polyvertcounter++;

/*asm {
    les     bx, [var_polyvertsptr]
    mov     ax, es:[bx+.px] // x2 in point2d
    mov     [var_polyvertX], ax
    mov     ax, es:[bx+.py]
    mov     [var_polyvertY], ax
    add     word ptr [var_polyvertsptr], 4
    mov     bx, transshaperectptr
    mov     ax, [bx+.x1]
    cmp     [var_polyvertX], ax
    jge     short loc_257BA
    mov     ax, [var_polyvertX]
    mov     [bx+.x1], ax
loc_257BA:
    mov     ax, [var_polyvertX]
    inc     ax
    mov     [var_B7C], ax
    mov     bx, transshaperectptr
    cmp     [bx+.y1], ax
    jge     short loc_257CF
    mov     [bx+.y1], ax
loc_257CF:
    mov     bx, transshaperectptr
    mov     ax, [var_polyvertY]
    cmp     [bx+.x2], ax
    jle     short loc_257DF
    mov     [bx+.x2], ax
loc_257DF:
    mov     ax, [var_polyvertY]
    inc     ax
    mov     [var_B7C], ax
    mov     bx, transshaperectptr
    cmp     [bx+.y2], ax
    jge     short loc_257F4
    mov     [bx+.y2], ax
loc_257F4:
    inc     word ptr [var_polyvertcounter]
}*/
loc_257F7:
	if (var_polyvertcounter < transshapenumvertscopy) goto loc_25790;

/*asm {
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     [var_polyvertcounter], ax
    jb      short loc_25790
}*/

// ------------------------------------ no primtype or unknown- skip this ------------------------------------

loc_25801:

	transshapeprimitives = transshapeprimptr;
	var_cull1++;
	var_cull2++;
	if (var_4 != 0) goto loc_25D3C;
	if ((var_primitiveflags & 2) != 0) goto loc_25E04;
loc_2582B:

	if ((transshapeprimitives[1] & 2) == 0) goto loc_25E04;
	transshapeprimitives += primidxcounttab[transshapeprimitives[0]] + transshapenumpaints + 2;
	var_cull1++;
	var_cull2++;
	goto loc_2582B;

/*asm {
    mov     ax, word ptr transshapeprimptr
    mov     dx, word ptr transshapeprimptr+2

    mov     word ptr transshapeprimitives, ax
    mov     word ptr transshapeprimitives+2, dx
    add     word ptr [var_cull2], 4
    add     word ptr [var_cull1], 4
    cmp     word ptr [var_4], 0
    jz      short loc_25822
    jmp     loc_25D3C
loc_25822:
    test    byte ptr [var_primitiveflags], 2
    jz      short loc_2582B
    jmp     loc_25E04
loc_2582B:

    les     bx, transshapeprimitives
    test    byte ptr es:[bx+1], 2
    jnz     short loc_25839

    jmp     loc_25E04

loc_25839:

    mov     bl, es:[bx]
    sub     bh, bh
    mov     al, primidxcounttab[bx]
    sub     ah, ah
    add     ax, transshapenumpaints
    add     ax, 2
    add     word ptr transshapeprimitives, ax
    add     word ptr [var_cull1], 4
    add     word ptr [var_cull2], 4
    jmp     short loc_2582B

// ------------------------------------ primtype_line ------------------------------------
}*/
_primtype_line:

	temp0 = transshapeprimitives[0];
	temp1 = transshapeprimitives[1];
	if (var_vertflagtbl[temp0] +var_vertflagtbl[temp1] == 2) {
		goto loc_25801;
	}
	if (var_vertflagtbl[temp0] == 0) {
		goto loc_258BC;
	}
	vector_op_unk(&var_vecarr[temp1], &var_vecarr[temp0], &var_vec2, 0x0C);
	temp = temp0;
	goto loc_258F6;

/*
asm {
    les     bx, transshapeprimitives
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    mov     al, es:[bx+1]
    mov     di, ax
    mov     al, [di+var_vertflagtbl]
    cbw
    mov     cx, ax
    mov     al, [si+var_vertflagtbl]
    cbw
    add     ax, cx
    cmp     ax, 2
    jz      short loc_25801
    cmp     byte ptr [si+var_vertflagtbl], 0
    jz      short loc_258BC
    mov     ax, 0Ch
    push    ax
    lea     ax, [var_vec2]
    push    ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    call    far ptr vector_op_unk
    add     sp, 8
    mov     ax, si
    jmp     short loc_258F6
}
*/
loc_258BC:

	if (var_vertflagtbl[temp1] == 0) goto loc_2590D;
	vector_op_unk(&var_vecarr[temp0], &var_vecarr[temp1], &var_vec2, 0x0C);

	temp = temp1;
/*	
asm {
	
    les     bx, transshapeprimitives
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    mov     al, es:[bx+1]
    mov     di, ax

	
	
    cmp     byte ptr [di+var_vertflagtbl], 0
    jz      short loc_2590D
    mov     ax, 0Ch
    push    ax
    lea     ax, [var_vec2]
    push    ax
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr
    ;add     ax, bp
    ;sub     ax, 0B6Eh
    push    ax
    call    far ptr vector_op_unk
    add     sp, 8
    mov     ax, di
}
*/
loc_258F6:

	vector_to_point(&var_vec2, &var_vecarr2[temp]);
/*
asm {
    shl     ax, 1
    shl     ax, 1
    add ax, bp
    add ax, offset var_vecarr2
    ;add     ax, bp
    ;sub     ax, 416h
    push    ax
    lea     ax, [var_vec2]
    push    ax
    call    far ptr vector_to_point
    add     sp, 4
}
*/
loc_2590D:
	
	// NOTE: when temp0 and temp1 were negative (ie bogus var_18), there was a sorting error with some of the wheels on the lamborghini LM-002
	var_18 = var_vecarr[temp0].z + var_vecarr[temp1].z;
	transshapepolyinfopts = transshapepolyinfo +6;
	transshapepolyinfopts[0] = *polyvertpointptrtab[0];
	transshapepolyinfopts[1] = *polyvertpointptrtab[1];

	if ((transshapeflags & 8) == 0) goto loc_25983;
	rect_adjust_from_point(polyvertpointptrtab[0], transshaperectptr);
	rect_adjust_from_point(polyvertpointptrtab[1], transshaperectptr);

/*asm {
	
	
    les     bx, transshapeprimitives
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    mov     al, es:[bx+1]
    mov     di, ax


    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx+offset var_vecarr.z]
    mov     bx, di
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
    add     bx, bp
    add     ax, [bx+offset var_vecarr.z]
    cwd
    mov     word ptr [var_18], ax
    mov     word ptr [var_18+2], dx

    mov     bx, polyvertpointptrtab
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, polyvertpointptrtab+2
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    test    transshapeflags, 8
    jz      short loc_25983
    push    word ptr transshaperectptr
    push    word ptr polyvertpointptrtab
    call far ptr rect_adjust_from_point
    add     sp, 4
    push    word ptr transshaperectptr
    push    word ptr polyvertpointptrtab+2
// injected start
    call far ptr rect_adjust_from_point
    add     sp, 4
// injected end

loc_2597C:
    //call far ptr rect_adjust_from_point // been injected in respective calls
    //add     sp, 4
}*/

loc_25983:
	transshapenumvertscopy = 2;
/*asm {
    mov     transshapenumvertscopy, 2
}*/

loc_25988:
	var_4++;
	goto loc_25801;
/*asm {
    inc     word ptr [var_4]
    jmp     loc_25801
}*/
// ------------------------------------ primtype_wheel ------------------------------------

_primtype_wheel:

	if (var_1A != 0) goto loc_25801;
	
	transshapepolyinfopts = transshapepolyinfo +6;
	transshapepolyinfopts[0] = *polyvertpointptrtab[0];
	transshapepolyinfopts[1] = *polyvertpointptrtab[1];
	transshapepolyinfopts[2] = *polyvertpointptrtab[2];
	transshapepolyinfopts[3] = *polyvertpointptrtab[3];

	if (transformed_shape_op_helper3(transshapepolyinfopts) != 0) goto loc_25A7C;

	transshapepolyinfopts[0] = *polyvertpointptrtab[3];
	transshapepolyinfopts[1] = *polyvertpointptrtab[4];
	transshapepolyinfopts[2] = *polyvertpointptrtab[5];
	transshapepolyinfopts[3] = *polyvertpointptrtab[0];

	var_18 = var_vecarr[transshapeprimitives[3]].z << 2;
	goto loc_25A9E;
/*	
asm {
    cmp     word ptr [var_1A], 0
    jz      short loc_25997
    jmp     loc_25801
loc_25997:
    mov     bx, polyvertpointptrtab
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, polyvertpointptrtab+2
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    mov     bx, polyvertpointptrtab+4
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+0Eh], ax
    mov     es:[bx+10h], dx
    mov     bx, polyvertpointptrtab+6
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+12h], ax
    mov     es:[bx+14h], dx
    mov     ax, word ptr transshapepolyinfo
    mov     dx, word ptr transshapepolyinfo+2
    add     ax, 6
    push    dx
    push    ax
    call far ptr transformed_shape_op_helper3
    add     sp, 4
    or      al, al
    jnz     short loc_25A7C
    mov     bx, polyvertpointptrtab+6
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, polyvertpointptrtab+8
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    mov     bx, polyvertpointptrtab+0Ah
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+0Eh], ax
    mov     es:[bx+10h], dx
    mov     bx, polyvertpointptrtab
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+12h], ax
    mov     es:[bx+14h], dx
    les     bx, transshapeprimitives
    mov     al, es:[bx+3]   // primitives+3 = paintjob 1? [0..x]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx+offset var_vecarr.z]
    cwd
    mov     cl, 2
loc_25A71:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_25A71
    jmp     short loc_25A9E
}*/
loc_25A7C:
	var_18 = var_vecarr[transshapeprimitives[0]].z << 2;
/*asm {
    les     bx, transshapeprimitives
    mov     al, es:[bx]     // primitives+0 = primitivetype
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx+offset var_vecarr.z]
    cwd
    mov     cl, 2
loc_25A96:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_25A96
}*/
loc_25A9E:

	transshapepolyinfopts = transshapepolyinfo +6;
	temp = polarRadius2D(transshapepolyinfopts[0].px - transshapepolyinfopts[1].px, transshapepolyinfopts[0].py - transshapepolyinfopts[1].py);
	temp1 = polarRadius2D(transshapepolyinfopts[0].px - transshapepolyinfopts[2].px, transshapepolyinfopts[0].py - transshapepolyinfopts[2].py);
	
	if (temp1 > temp) temp = temp1;

	if ((transshapeflags & 8) == 0) goto loc_25B9C;
	
	var_450.px = transshapepolyinfopts[0].px - temp - 1;
	var_450.py = transshapepolyinfopts[0].py - temp - 1;
	rect_adjust_from_point(&var_450, transshaperectptr);

	var_450.px = transshapepolyinfopts[0].px + temp + 1;
	var_450.py = transshapepolyinfopts[0].py + temp + 1;
	rect_adjust_from_point(&var_450, transshaperectptr);
	
	var_450.px = transshapepolyinfopts[3].px - temp - 1;
	var_450.py = transshapepolyinfopts[3].py - temp - 1;
	rect_adjust_from_point(&var_450, transshaperectptr);

	var_450.px = transshapepolyinfopts[3].px + temp + 1;
	var_450.py = transshapepolyinfopts[3].py + temp + 1;
	rect_adjust_from_point(&var_450, transshaperectptr);

/*
asm {
    mov     word ptr [var_18], ax
    mov     word ptr [var_18+2], dx
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+8]
    sub     ax, es:[bx+0Ch]
    push    ax
    mov     ax, es:[bx+6]
    sub     ax, es:[bx+0Ah]
    push    ax
    call    far ptr polarRadius2D
    add     sp, 4
    mov     si, ax
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+8]
    sub     ax, es:[bx+10h]
    push    ax
    mov     ax, es:[bx+6]
    sub     ax, es:[bx+0Eh]
    push    ax
    call    far ptr polarRadius2D
    add     sp, 4
    mov     di, ax
    cmp     di, si
    jle     short loc_25AEA
    mov     si, di
loc_25AEA:
    test    transshapeflags, 8
    jnz     short loc_25AF4
    jmp     loc_25B9C
loc_25AF4:
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+6]
    sub     ax, si
    dec     ax
    mov     word ptr [var_450+.vx], ax
    mov     ax, es:[bx+8]
    sub     ax, si
    dec     ax
    mov     word ptr [var_450+.vy], ax
    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
    call far ptr rect_adjust_from_point
    add     sp, 4
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+8]
    add     ax, si
    inc     ax
    mov     word ptr [var_450+.vy], ax
    mov     ax, es:[bx+6]
    add     ax, si
    inc     ax
    mov     word ptr [var_450+.vx], ax
    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
    call far ptr rect_adjust_from_point
    add     sp, 4
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+12h]
    sub     ax, si
    dec     ax
    mov     word ptr [var_450+.vx], ax
    mov     ax, es:[bx+14h]
    sub     ax, si
    dec     ax
    mov     word ptr [var_450+.vy], ax
    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
    call far ptr rect_adjust_from_point
    add     sp, 4
    les     bx, transshapepolyinfo
    mov     ax, es:[bx+14h]
    add     ax, si
    inc     ax
    mov     word ptr [var_450+.vy], ax
    mov     ax, es:[bx+12h]
    add     ax, si
    inc     ax
    mov     word ptr [var_450+.vx], ax

    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
    ;push    cs
    call far ptr rect_adjust_from_point   // denne fucker opppppp litt inni
    add     sp, 4
}*/
loc_25B9C:
	transshapenumvertscopy = 4;
	var_4 = 1;
	goto loc_25801;

/*asm {
    mov     transshapenumvertscopy, 4
    mov     word ptr [var_4], 1
    jmp     loc_25801

}*/

// ------------------------------------ primtype_sphere ------------------------------------
_primtype_sphere:

	temp0 = transshapeprimitives[0];
	temp1 = transshapeprimitives[1];
//fatal_error("anders: %i %i", temp0, temp1);
	var_18 = var_vecarr[temp0].z + var_vecarr[temp1].z;
	if (var_vertflagtbl[temp0] + var_vertflagtbl[temp1] != 0) goto loc_25801;

	transshapepolyinfopts = transshapepolyinfo +6;
	transshapepolyinfopts[0] = *polyvertpointptrtab[0];
	var_vec3 = var_vecarr[temp0];
	var_vec4 = var_vecarr[temp1];

	var_vec2.x = var_vec3.x - var_vec4.x;
	var_vec2.y = var_vec3.y - var_vec4.y;
	var_vec2.z = var_vec3.z - var_vec4.z;
	var_462 = transformed_shape_op_helper2(polarRadius3D(&var_vec2), var_vec3.z);
	transshapepolyinfopts[1].px = var_462;
	if ((transshapeflags & 8) == 0) goto loc_25983;

	var_450.py = polyvertpointptrtab[0]->py - var_462;
	var_450.px = polyvertpointptrtab[0]->px - var_462;

	rect_adjust_from_point(&var_450, transshaperectptr);

	var_450.py = polyvertpointptrtab[0]->py + var_462;
	var_450.px = polyvertpointptrtab[0]->px + var_462;

	rect_adjust_from_point(&var_450, transshaperectptr);
	goto loc_25983;
/*	
asm {
    les     bx, transshapeprimitives
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    mov     al, es:[bx+1]
    mov     di, ax
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    mov     [var_B7C], ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    mov     [var_polyvertunktabptr], ax
    mov     bx, ax
    mov     ax, [bx+offset var_vecarr.z]
    mov     bx, [var_B7C]
    add     ax, [bx+offset var_vecarr.z]
    cwd
    mov     word ptr [var_18], ax
    mov     word ptr [var_18+2], dx
    mov     al, [di+var_vertflagtbl]
    cbw
    mov     cx, ax
    mov     al, [si+var_vertflagtbl]
    cbw
    add     ax, cx
    jz      short loc_25C01
    jmp     loc_25801
loc_25C01:
    mov     bx, polyvertpointptrtab
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, [var_polyvertunktabptr]
    push    si
    push    di
    lea     di, [var_vec3]
    lea     si, [bx+offset var_vecarr]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [var_B7C]
    push    si
    push    di
    lea     di, [var_vec4]
    lea     si, [bx + offset var_vecarr]
    //lea     si, [bx-0B6Eh]
    movsw
    movsw
    movsw
    pop     di
    pop     si

    mov     ax, word ptr [var_vec3.vx]
    sub     ax, word ptr [var_vec4.vx]
    mov     word ptr [var_vec2.vx], ax

    mov     ax, word ptr [var_vec3.y]
    sub     ax, word ptr [var_vec4.y]
    mov     word ptr [var_vec2.y], ax

    mov     ax, word ptr [var_vec3.z]
    sub     ax, word ptr [var_vec4.z]
    mov     word ptr [var_vec2.z], ax

    push    word ptr [var_vec3.z]
    lea     ax, [var_vec2]
    push    ax
    call    far ptr polarRadius3D
    add     sp, 2
    push    ax
    call    far ptr transformed_shape_op_helper2
    add     sp, 4
    mov     [var_462], ax
    
    les     bx, transshapepolyinfo
    mov     es:[bx+0Ah], ax
    test    transshapeflags, 8
    jnz     short loc_25C92
    jmp     loc_25983
loc_25C92:

    mov     bx, polyvertpointptrtab
    mov     ax, [bx+2]
    sub     ax, [var_462]
    mov     word ptr [var_450+.vy], ax
    mov     ax, [bx]
    sub     ax, [var_462]
    mov     word ptr [var_450+.vx], ax
    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
    ;push    cs
    call far ptr rect_adjust_from_point
    add     sp, 4
    mov     ax, [var_462]
    mov     bx, polyvertpointptrtab
    add     ax, [bx]
    mov     word ptr [var_450+.vx], ax
    mov     ax, [bx+2]
    add     ax, [var_462]
    mov     word ptr [var_450+.vx], ax
    push    word ptr transshaperectptr
    lea     ax, [var_450]
    push    ax
// injected start
    call far ptr rect_adjust_from_point
    add     sp, 4
// injected end
    jmp     loc_25983
}
*/
// ------------------------------------ primtype 5 - unknown / particle? ------------------------------------

loc_25CE0:
	fatal_error("unhandled primitive type 5");
	goto loc_25988; // var4++ | goto 25801
/*asm {
    les     bx, transshapeprimitives
    mov     al, es:[bx]     // primitives+ 0 = type
    sub     ah, ah
    mov     si, ax
    cmp     [si+var_vertflagtbl], ah
    jz      short loc_25CF4
    jmp     loc_25801
loc_25CF4:
    mov     bx, si
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx+offset var_vecarr.z]
    cwd
    mov     word ptr [var_18], ax
    mov     word ptr [var_18+2], dx
    mov     bx, polyvertpointptrtab
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, transshapepolyinfo
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    test    transshapeflags, 8
    jz      short loc_25D34
    push    word ptr transshaperectptr
    push    word ptr polyvertpointptrtab
    ;push    cs
    call far ptr rect_adjust_from_point
    add     sp, 4
loc_25D34:
    mov     transshapenumvertscopy, 1
    jmp     loc_25988
}    */


// ------------------------------------ jumps here from inside the loc_25801-case if var_4 != 0------------------------------------

loc_25D3C:
	var_45E++;
	transshapepolyinfo[3] = transshapenumvertscopy;
	transshapepolyinfo[4] = var_primtype;
	if (transprimitivepaintjob == 0x2D) {
		transshapepolyinfo[2] = byte_45514;
	} else {
		transshapepolyinfo[2] = transprimitivepaintjob;
	}
	
	if (transshapenumvertscopy == 1) {
		// goto loc_25D9C;
		temp0 = var_18;
	} else 
	if (transshapenumvertscopy == 2) {
		//goto loc_25DB8;
		temp0 = var_18 >> 1;
	} else
	if (transshapenumvertscopy == 4) {
		//goto loc_25DC6;
		temp0 = var_18 >> 2;
	} else
	if (transshapenumvertscopy == 8) {
		// goto loc_25DDA;
		temp0 = var_18 >> 3;
	} else {
		temp0 = var_18 / transshapenumvertscopy;
	}
	
	((unsigned int far*)transshapepolyinfo)[0] = temp0;
	
	
	if ((transshapeflags & 1) != 0 || (var_primitiveflags & 2) != 0) {
		temp = 0;
	} else
		temp = 1;

	word_40ECE = transformed_shape_op_helper(temp0, temp);
	
	if (word_40ECE == 0) goto loc_25E04;
	return 1;

/*	
asm {
    inc     word ptr [var_45E]
    les     bx, transshapepolyinfo
    mov     al, transshapenumvertscopy
    mov     es:[bx+3], al   // polyinfo+3 = numverts
    les     bx, transshapepolyinfo
    mov     al, [var_primtype]
    mov     es:[bx+4], al   // polyinfo+4 = primtype
    cmp     transprimitivepaintjob, 2Dh // '-'
    jnz     short loc_25D66
    les     bx, transshapepolyinfo
    mov     al, byte_45514
    jmp     short loc_25D6D
loc_25D66:
    les     bx, transshapepolyinfo
    mov     al, transprimitivepaintjob
loc_25D6D:
    mov     es:[bx+2], al   // polyinfo+2 = paintjob
    mov     al, transshapenumvertscopy
    sub     ah, ah
    cmp     ax, 1
    jz      short loc_25D9C
    cmp     ax, 2
    jz      short loc_25DB8
    cmp     ax, 4
    jz      short loc_25DC6
    cmp     ax, 8
    jz      short loc_25DDA
    
    mov     al, transshapenumvertscopy
    sub     ah, ah

    sub     cx, cx
    push    cx
    push    ax
    push    word ptr [var_18+2]
    push    word ptr [var_18]
    call    far ptr __aFuldiv
    jmp     short loc_25DC2
}
loc_25D9C:
asm {
    mov     si, word ptr [var_18]

loc_25D9F:

    les     bx, transshapepolyinfo
    mov     es:[bx], si     // polyinfo+0 = ???

    test    transshapeflags, 1
    jnz     short loc_25DB3
    test    byte ptr [var_primitiveflags], 2
    jz      short loc_25DEE
loc_25DB3:
    sub     ax, ax
    jmp     short loc_25DF1
}

loc_25DB8:
asm {
    mov     ax, word ptr [var_18]
    mov     dx, word ptr [var_18+2]
    sar     dx, 1
    rcr     ax, 1
loc_25DC2:
    mov     si, ax
    jmp     short loc_25D9F
}
loc_25DC6:
asm {
    mov     ax, word ptr [var_18]
    mov     dx, word ptr [var_18+2]
    mov     cl, 2
loc_25DCE:
    or      cl, cl
    jz      short loc_25DC2
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jmp     short loc_25DCE
}
loc_25DDA:
asm {
    mov     ax, word ptr [var_18]
    mov     dx, word ptr [var_18+2]
    mov     cl, 3
loc_25DE2:
    or      cl, cl
    jz      short loc_25DC2
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jmp     short loc_25DE2
    
loc_25DEE:
    mov     ax, 1
loc_25DF1:
    push    ax
    push    si
    call far ptr transformed_shape_op_helper
    add     sp, 4
    mov     word_40ECE, ax
    or      ax, ax
    jz      short loc_25E04
    
    //jmp     loc_24EAE // inject:
//loc_24EAE:
    mov     ax, 1

    jmp the_end
}
*/

loc_25E04:

	if (transshapeprimitives[0] != 0) goto loc_25233;

	if (var_45E != 0) return 0;
	return -1;
/*	
asm {
    les     bx, transshapeprimitives
    cmp     byte ptr es:[bx], 0
    jz      short _transform_done
    jmp     loc_25233


_transform_done:
    cmp     word ptr [var_45E], 0
    jnz     short _done_ret_0

    //jmp     _done_ret_neg1
    mov     ax, 0FFFFh
    jmp the_end

_done_ret_0:
    sub     ax, ax
    //pop     si
    //pop     di
    jmp the_end
    retf

}

the_end:
asm {
	mov result, ax
}
//fatal_error("result %i", result);
	//fatal_error("%i %i %i %i", transshaperectptr->x1, transshaperectptr->y1, transshaperectptr->x2, transshaperectptr->y2);
	//return result;
return result;
*/
}
