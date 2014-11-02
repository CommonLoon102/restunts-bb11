#include "externs.h"
#include "math.h"
#include "shape3d.h"

extern struct RECTANGLE* rectptr_unk2;
extern struct RECTANGLE rect_array_unk[];
extern struct RECTANGLE rect_array_unk2[];
extern struct RECTANGLE rect_unk[];
extern struct RECTANGLE rect_unk2;
extern struct RECTANGLE rect_unk6;
extern struct RECTANGLE rect_unk9;
extern struct RECTANGLE rect_unk11;
extern struct RECTANGLE rect_unk12;
extern struct RECTANGLE rect_unk15;
extern struct RECTANGLE cliprect_unk;
extern struct VECTOR vec_unk2;
extern struct VECTOR vec_planerotopresult;
extern struct MATRIX mat_temp;
extern int word_3B8EC;
extern int word_3B8F0;
extern int word_3B8EE;
extern int word_44D20;
extern char byte_3C09C[];
extern char byte_3C0C6[];
extern char word_46468;
extern int word_3BE34[];
extern char* off_3C084[];
extern struct SHAPE3D* off_3BE44[];
extern int terrainHeight;
extern int planindex;
extern int planindex_copy;
extern char byte_4392C;
extern struct TRANSFORMEDSHAPE3D currenttransshape[29];
//extern struct TRANSFORMEDSHAPE3D transshapeunk;
extern struct TRANSFORMEDSHAPE3D* curtransshape_ptr;
extern struct TRACKOBJECT trkObjectList[215]; // 215 entries
extern unsigned char fence_TrkObjCodes[];
extern int pState_minusRotate_z_2, pState_minusRotate_x_2, pState_minusRotate_y_2, pState_f36Mminf40sar2;

extern char unk_3C0EE[];
extern char unk_3C0F0[];
extern char unk_3C0F8[];
extern char unk_3C0F4[];
extern int word_3C0D6[];
extern int unk_3C0A2[];
extern int unk_3C0A6[];
extern int unk_3C0AE[];
extern int unk_3C0B6[];
extern struct TRACKOBJECT sceneshapes2[];
extern struct TRACKOBJECT sceneshapes3[];
extern struct SHAPE3D game3dshapes[130];
extern struct VECTOR carshapevec;
extern struct VECTOR carshapevecs[6];
extern short word_443E8[];
extern struct VECTOR oppcarshapevec;
extern struct VECTOR oppcarshapevecs[6];
extern short word_4448A[];
extern char backlights_paint_override;
extern int word_449FC[];
extern int word_463D6;
extern int transformedshape_zarray[];
extern int transformedshape_indices[];
extern char transformedshape_arg2array[];
extern int sdgame2_widths[];
extern void far* sdgame2shapes[];
extern void far* fontledresptr;
extern int dialog_fnt_colour;
extern char transformedshape_counter;

void build_track_object(struct VECTOR* a, struct VECTOR* b);
int plane_origin_op(int planIndex, int b, int c, int d);
void transformed_shape_add_for_sort(int a, int b);
void plane_rotate_op(void);
unsigned char subst_hillroad_track(unsigned char a, unsigned char b);
int skybox_op(int a, struct RECTANGLE* rectptr, int c, struct MATRIX* matptr, int e, int f, int g);
struct RECTANGLE* draw_ingame_text(void);
struct RECTANGLE* init_crak(int frame, int top, int height);
struct RECTANGLE* do_sinking(int frame, int top, int height);
struct RECTANGLE* intro_draw_text(char* str, int a, int b, int c, int d);
void font_set_fontdef2(void far* data);
void format_frame_as_string(char* s, int time, int c);
void shape_op_explosion(int a, void far* shp, int x, int y);
void heapsort_by_order(int n, int* heap, int* data);

void update_frame(char arg_0, struct RECTANGLE* arg_cliprectptr) {
/*
    var_lastpos2lookup = dword ptr -340
    var_lastposlookupw = word ptr -336
    var_transformresult = word ptr -334
    var_14C = byte ptr -332
    var_posadjust = byte ptr -308
    var_132 = word ptr -306
    var_130 = byte ptr -304
    var_trkobject_ptr = word ptr -302
    var_poslookupadjust = byte ptr -300
    var_12A = word ptr -298
    var_vec7 = VECTOR ptr -296
    var_122 = byte ptr -290
    var_mat = MATRIX ptr -288
    var_10E = word ptr -270
    terr_map_value = byte ptr -268
    var_pos2adjust = byte ptr -266
    var_108 = dword ptr -264
    var_stateptr = word ptr -260
    var_poslookup = byte ptr -258
    var_pos2lookupadjust = byte ptr -256
    var_angZ = word ptr -254
    var_FC = byte ptr -252
    var_angX = word ptr -250
    var_pos2lookup = byte ptr -246
    var_angY2 = word ptr -244
    var_vec6 = VECTOR ptr -242
    var_vec5 = VECTOR ptr -236
    var_E4 = byte ptr -228
    var_E2 = byte ptr -226
    var_E0 = word ptr -224
    var_multitileflag = word ptr -222
    var_DC = byte ptr -220
    var_DB = byte ptr -219
    var_DA = word ptr -218
    var_D8 = byte ptr -216
    var_matptr = word ptr -214
    var_postab = byte ptr -212
    var_BC = byte ptr -188
    var_A4 = word ptr -164
    var_counter2 = word ptr -162
    var_mat2 = MATRIX ptr -160
    var_rect2 = RECTANGLE ptr -142
    var_pos2tab = byte ptr -134
    var_6E = byte ptr -110
    var_6C = word ptr -108
    var_rect = RECTANGLE ptr -106
    elem_map_value = byte ptr -98
    var_60 = byte ptr -96
    var_5E = word ptr -94
    var_angY = word ptr -92
    var_angZ2 = word ptr -90
    var_vec4 = VECTOR ptr -88
    var_52 = word ptr -82
    var_50 = word ptr -80
    var_4E = byte ptr -78
    var_4C = byte ptr -76
    var_4A = byte ptr -74
    var_hillheight = word ptr -72
    var_angX2 = word ptr -70
    var_vec8 = VECTOR ptr -68
    var_rectptr = word ptr -62
    var_3C = byte ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_counter = word ptr -54
    var_trkobjectptr = word ptr -52
    var_32 = byte ptr -50
    var_1A = byte ptr -26
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_cliprectptr = word ptr 8
*/
	int si;
	char var_122;
	char var_E4;
	char var_DC[2];
	struct RECTANGLE* var_rectptr;
	struct MATRIX var_mat, var_mat2;
	struct MATRIX* var_matptr;
	struct VECTOR var_vec4, var_vec5, var_vec6, var_vec7, var_vec8;
	int var_angX2, var_angY2, var_angZ2;
	int var_angX, var_angY, var_E0;
	int var_38, var_angZ;
	int var_transformresult;
	int var_52;
	char* var_50;
	int var_2;
	int var_counter, var_counter2;
	char var_posadjust, var_pos2adjust;
	char var_poslookup, var_pos2lookup;
	char var_poslookupadjust, var_pos2lookupadjust;
	char var_D8, var_E2;
	unsigned char var_1A[24];
	char var_32[24];
	char var_BC[24];
	char var_postab[24];
	char var_pos2tab[24];
	unsigned char var_14C[24];
	char var_130;
	char var_3C;
	char var_60;
	char var_6E;
	char var_4A;
	char var_4E;
	int var_6C;
	int var_A4;
	int var_hillheight;
	int var_multitileflag;
	struct TRACKOBJECT* var_trkobjectptr;
	struct TRACKOBJECT* var_trkobject_ptr; // NOTE: beware of similar names!!
	char var_FC;
	char* var_10E;
	int di;
	int var_132;
	int var_5E;
	int var_3A;
	int* var_DA;
	int var_12A;
	unsigned char var_4C;
	struct RECTANGLE var_rect, var_rect2;
	struct VECTOR far* var_108;
	struct CARSTATE* var_stateptr;
	unsigned char elem_map_value;
	unsigned char terr_map_value;
	int temp1;
	struct TRACKOBJECT* temp2;

	//return ported_update_frame_(arg_0, arg_cliprectptr);
	
	var_DC[0] = 0;
	var_DC[1] = 0;
	if (video_flag5_is0 == 0)
		goto loc_1A122;
	if (arg_0 == 0)
		goto loc_1A122;
	rectptr_unk2 = rect_array_unk;
	rectptr_unk = rect_array_unk2;
	goto loc_1A12E;
/*    push    bp
    mov     bp, sp
    sub     sp, 154h
    push    di
    push    si
    mov     [bp+var_DC], 0
    mov     [bp+var_DB], 0
    cmp     video_flag5_is0, 0
    jz      short loc_1A122
    cmp     [bp+arg_0], 0
    jz      short loc_1A122
    mov     rectptr_unk2, offset rect_array_unk
    mov     rectptr_unk, offset rect_array_unk2
    jmp     short loc_1A12E*/
loc_1A122:
	rectptr_unk = rect_array_unk;
	rectptr_unk2 = rect_array_unk2;
    //mov     rectptr_unk, offset rect_array_unk
    //mov     rectptr_unk2, offset rect_array_unk2
loc_1A12E:
	if (timertestflag_copy == 0)
		goto loc_1A162;
	var_122 = 8;
	var_rectptr = rect_unk;
	si = 0;
	goto loc_1A145;
/*
    cmp     timertestflag_copy, 0
    jz      short loc_1A162
    mov     [bp+var_122], 8
    mov     [bp+var_rectptr], offset rect_unk
    sub     si, si
    jmp     short loc_1A145
    ; align 2
    db 144*/
loc_1A144:
	si++;
    //inc     si
loc_1A145:
	if (si >= 0xF)
		goto loc_1A167;
	*var_rectptr = cliprect_unk;
	var_rectptr++;
	goto loc_1A144;
/*    cmp     si, 0Fh
    jge     short loc_1A167
    mov     ax, [bp+var_rectptr]
    add     [bp+var_rectptr], 8
    push    si
    push    di
    mov     di, ax
    mov     si, offset cliprect_unk
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    jmp     short loc_1A144*/
loc_1A162:
	var_122 = 0;
    //mov     [bp+var_122], 0
loc_1A167:
	if (followOpponentFlag != 0)
		goto loc_1A1BE;
	var_vec5.x = state.playerstate.car_posWorld1.lx >> 6;
	var_vec5.y = state.playerstate.car_posWorld1.ly >> 6;
	var_vec5.z = state.playerstate.car_posWorld1.lz >> 6;
/*
    cmp     followOpponentFlag, 0
    jnz     short loc_1A1BE
    mov     ax, word ptr state.playerstate.car_posWorld1.lx
    mov     dx, word ptr state.playerstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1A177:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A177
    mov     [bp+var_vec5.vx], ax
    mov     ax, word ptr state.playerstate.car_posWorld1.ly
    mov     dx, word ptr state.playerstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1A18C:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A18C
    mov     [bp+var_vec5.vy], ax
    mov     ax, word ptr state.playerstate.car_posWorld1.lz
    mov     dx, word ptr state.playerstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1A1A1:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1A1
    mov     [bp+var_vec5.vz], ax*/
	var_angX2 = state.playerstate.car_rotate.y;
	var_angZ2 = state.playerstate.car_rotate.z;
	var_angY2 = state.playerstate.car_rotate.x;
	goto loc_1A20C;
/*    mov     ax, state.playerstate.car_rotate.vy
    mov     [bp+var_angX2], ax
    mov     ax, state.playerstate.car_rotate.vz
    mov     [bp+var_angZ2], ax
    mov     ax, state.playerstate.car_rotate.vx
    jmp     short loc_1A20C*/
loc_1A1BE:
	var_vec5.x = state.opponentstate.car_posWorld1.lx >> 6;
	var_vec5.y = state.opponentstate.car_posWorld1.ly >> 6;
	var_vec5.z = state.opponentstate.car_posWorld1.lz >> 6;
/*
    mov     ax, word ptr state.opponentstate.car_posWorld1.lx
    mov     dx, word ptr state.opponentstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1A1C7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1C7
    mov     [bp+var_vec5.vx], ax
    mov     ax, word ptr state.opponentstate.car_posWorld1.ly
    mov     dx, word ptr state.opponentstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1A1DC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1DC
    mov     [bp+var_vec5.vy], ax
    mov     ax, word ptr state.opponentstate.car_posWorld1.lz
    mov     dx, word ptr state.opponentstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1A1F1:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1F1
    mov     [bp+var_vec5.vz], ax*/
	var_angX2 = state.opponentstate.car_rotate.y;
	var_angZ2 = state.opponentstate.car_rotate.z;
	var_angY2 = state.opponentstate.car_rotate.x;
/*
    mov     ax, state.opponentstate.car_rotate.vy
    mov     [bp+var_angX2], ax
    mov     ax, state.opponentstate.car_rotate.vz
    mov     [bp+var_angZ2], ax
    mov     ax, state.opponentstate.car_rotate.vx*/
loc_1A20C:
	var_angY = -1;
	var_E0 = 0;
	if (cameramode != 0)
		goto loc_1A226;
	goto loc_1A434;
/*    mov     [bp+var_angY2], ax
    mov     [bp+var_angY], 0FFFFh
    mov     [bp+var_E0], 0
    mov     al, cameramode
    cbw
    or      ax, ax
    jnz     short loc_1A226
    jmp     loc_1A434
*/
loc_1A226:
	if (cameramode != 1)
		goto loc_1A22E;
	goto loc_1A40A;
/*    cmp     ax, 1
    jnz     short loc_1A22E
    jmp     loc_1A40A*/
loc_1A22E:
	if (cameramode == 2)
		goto loc_1A23E;
	if (cameramode != 3)
		goto loc_1A23B;
	goto loc_1A48E;
/*    cmp     ax, 2
    jz      short loc_1A23E
    cmp     ax, 3
    jnz     short loc_1A23B
    jmp     loc_1A48E
*/
loc_1A23B:
	goto loc_1A303;
//    jmp     loc_1A303
loc_1A23E:
	var_vec6.x = 0;
	var_vec6.y = 0;
	var_vec6.z = 0x4000;
	var_matptr = mat_rot_zxy(-var_angZ2, -var_angX2, -var_angY2, 0);
	mat_mul_vector(&var_vec6, var_matptr, &var_vec7);

/*
    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vy], 0
    mov     [bp+var_vec6.vz], 4000h
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_angY2]
    neg     ax
    push    ax
    mov     ax, [bp+var_angX2]
    neg     ax
    push    ax
    mov     ax, [bp+var_angZ2]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_matptr], ax
    lea     ax, [bp+var_vec7]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6*/

	var_vec6.x = 0;
	var_vec6.y = 0;
	var_vec6.z = word_3B8EC;
	var_matptr = mat_rot_zxy(0, -word_3B8F0, polarAngle(var_vec7.x, var_vec7.z) - word_3B8EE, 0);
/*  push    [bp+var_vec7.vz]
    push    [bp+var_vec7.vx]
    call    polarAngle
    add     sp, 4
    mov     si, ax
    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vy], 0
    mov     ax, word_3B8EC
    mov     [bp+var_vec6.vz], ax
    sub     ax, ax
    push    ax
    mov     ax, si
    sub     ax, word_3B8EE
    push    ax
    mov     ax, word_3B8F0
    neg     ax
    push    ax
    sub     ax, ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_matptr], ax*/
loc_1A2CC:
	mat_mul_vector(&var_vec6, var_matptr, &var_vec7);
	var_vec4.x = var_vec5.x + var_vec7.x;
	var_vec4.y = var_vec5.y + var_vec7.y;
	var_vec4.z = var_vec5.z + var_vec7.z;
/*
    lea     ax, [bp+var_vec7]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_vec5.vx]
    add     ax, [bp+var_vec7.vx]
    mov     [bp+var_vec4.vx], ax
    mov     ax, [bp+var_vec5.vy]
    add     ax, [bp+var_vec7.vy]
    mov     [bp+var_vec4.vy], ax
    mov     ax, [bp+var_vec5.vz]
    add     ax, [bp+var_vec7.vz]*/
loc_1A300:
    //mov     [bp+var_vec4.vz], ax
loc_1A303:
	if (var_angY == -1)
		goto loc_1A30C;
	goto loc_1A3E9;
/*    cmp     [bp+var_angY], 0FFFFh
    jz      short loc_1A30C
    jmp     loc_1A3E9*/
loc_1A30C:
	build_track_object(&var_vec4, &var_vec4);
	if (var_vec4.y >= terrainHeight)
		goto loc_1A327;
	var_vec4.y = terrainHeight;
/*    lea     ax, [bp+var_vec4]
    push    ax
    lea     ax, [bp+var_vec4]
    push    ax
    call    build_track_object
    add     sp, 4
    mov     ax, terrainHeight
    cmp     [bp+var_vec4.vy], ax
    jge     short loc_1A327
    mov     [bp+var_vec4.vy], ax*/
loc_1A327:
	if (byte_4392C == 0)
		goto loc_1A393;
	si = plane_origin_op(planindex, var_vec4.x, var_vec4.y, var_vec4.z);
	if (si >= 0xC)
		goto loc_1A393;
	vec_unk2.x = 0;
	vec_unk2.y = 0xC - si;
	vec_unk2.z = 0;
	planindex_copy = planindex;
	pState_f36Mminf40sar2 = 0;
	pState_minusRotate_x_2 = 0;
	pState_minusRotate_z_2 = 0;
	pState_minusRotate_y_2 = 0;
	plane_rotate_op();
	var_vec4.x += vec_planerotopresult.x;
	var_vec4.y += vec_planerotopresult.y;
	var_vec4.z += vec_planerotopresult.z;
	/*
    cmp     byte_4392C, 0
    jz      short loc_1A393
    push    [bp+var_vec4.vz]
    push    [bp+var_vec4.vy]
    push    [bp+var_vec4.vx]
    push    planindex
    call    plane_origin_op
    add     sp, 8
    mov     si, ax
    cmp     si, 0Ch
    jge     short loc_1A393
    mov     vec_unk2.vx, 0
    mov     ax, 0Ch
    sub     ax, si
    mov     vec_unk2.vy, ax
    mov     vec_unk2.vz, 0
    mov     ax, planindex
    mov     planindex_copy, ax
    mov     pState_f36Mminf40sar2, 0
    mov     pState_minusRotate_x_2, 0
    mov     pState_minusRotate_z_2, 0
    mov     pState_minusRotate_y_2, 0
    call    plane_rotate_op
    mov     ax, vec_planerotopresult.vx
    add     [bp+var_vec4.vx], ax
    mov     ax, vec_planerotopresult.vy
    add     [bp+var_vec4.vy], ax
    mov     ax, vec_planerotopresult.vz
    add     [bp+var_vec4.vz], ax*/
loc_1A393:
	var_angY = (-polarAngle(var_vec5.x - var_vec4.x, var_vec5.z - var_vec4.z)) & 0x3FF;
	var_38 = polarRadius2D(var_vec5.x - var_vec4.x, var_vec5.z - var_vec4.z);
	var_angX = polarAngle(var_vec5.y - var_vec4.y + 0x32, var_38) & 0x3FF;
/*    mov     ax, [bp+var_vec5.vz]
    sub     ax, [bp+var_vec4.vz]
    push    ax
    mov     ax, [bp+var_vec5.vx]
    sub     ax, [bp+var_vec4.vx]
    push    ax
    call    polarAngle
    add     sp, 4
    neg     ax
smart
    and     ah, 3
nosmart
    mov     [bp+var_angY], ax
    mov     ax, [bp+var_vec5.vz]
    sub     ax, [bp+var_vec4.vz]
    push    ax
    mov     ax, [bp+var_vec5.vx]
    sub     ax, [bp+var_vec4.vx]
    push    ax
    call    polarRadius2D
    add     sp, 4
    mov     [bp+var_38], ax
    push    ax
    mov     ax, [bp+var_vec5.vy]
    sub     ax, [bp+var_vec4.vy]
    add     ax, 32h ; '2'
    push    ax
    call    polarAngle
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     [bp+var_angX], ax*/
loc_1A3E9:
	if (var_E0 > 1)
		goto loc_1A3F3;
	goto loc_1A4D0;
/*    cmp     [bp+var_E0], 1
    jg      short loc_1A3F3
    jmp     loc_1A4D0*/
loc_1A3F3:
	if (var_E0 < 0x3FF)
		goto loc_1A3FE;
	goto loc_1A4D0;
/*    cmp     [bp+var_E0], 3FFh
    jl      short loc_1A3FE
    jmp     loc_1A4D0*/
loc_1A3FE:
	var_angZ = var_E0;
	goto loc_1A4D6;
/*    mov     ax, [bp+var_E0]
    mov     [bp+var_angZ], ax
    jmp     loc_1A4D6
    ; align 2
    db 144*/
loc_1A40A:
	var_vec4.x = state.game_vec1[followOpponentFlag].x;
	var_vec4.z = state.game_vec1[followOpponentFlag].z;
	var_vec4.y = state.game_vec1[followOpponentFlag].y;
	goto loc_1A303;
/*    mov     al, followOpponentFlag
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     ax, state.game_vec1.vx[bx]
    mov     [bp+var_vec4.vx], ax
    mov     ax, state.game_vec1.vz[bx]
    mov     [bp+var_vec4.vz], ax
    mov     ax, state.game_vec1.vy[bx]
    mov     [bp+var_vec4.vy], ax
    jmp     loc_1A303*/
loc_1A434:
	var_angY = var_angY2 & 0x3ff;
	var_angX = var_angX2 & 0x3ff;
	var_E0   = var_angZ2 & 0x3ff;
	var_matptr = mat_rot_zxy(-var_angZ2, -var_angX2, -var_angY2, 0);
	var_vec6.x = 0;
	var_vec6.z = 0;
	var_vec6.y = simd_player.car_height - 6;
	goto loc_1A2CC;
/*    mov     ax, [bp+var_angY2]
smart
    and     ah, 3
nosmart
    mov     [bp+var_angY], ax
    mov     ax, [bp+var_angX2]
smart
    and     ah, 3
nosmart
    mov     [bp+var_angX], ax
    mov     ax, [bp+var_angZ2]
smart
    and     ah, 3
nosmart
    mov     [bp+var_E0], ax
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_angY2]
    neg     ax
    push    ax
    mov     ax, [bp+var_angX2]
    neg     ax
    push    ax
    mov     ax, [bp+var_angZ2]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_matptr], ax
    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vz], 0
    mov     ax, simd_player.car_height
    sub     ax, 6
    mov     [bp+var_vec6.vy], ax
    jmp     loc_1A2CC
    ; align 2
    db 144

*/
loc_1A48E:
	//var_lastpos2lookup = trackdata9[state.field_3F7[followOpponentFlag] * 3 + 0];
	var_vec4.x = trackdata9[state.field_3F7[followOpponentFlag] * 3 + 0];
	var_vec4.y = trackdata9[state.field_3F7[followOpponentFlag] * 3 + 1] + word_44D20 + 0x5A;
	var_vec4.z = trackdata9[state.field_3F7[followOpponentFlag] * 3 + 2];
	goto loc_1A300;
/*	
    mov     al, followOpponentFlag
    cbw
    mov     bx, ax
    mov     al, state.field_3F7[bx]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr trackdata9
    mov     dx, word ptr trackdata9+2
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     word ptr [bp+var_lastpos2lookup+2], dx
    les     bx, [bp+var_lastpos2lookup]
    mov     ax, es:[bx]
    mov     [bp+var_vec4.vx], ax
    mov     ax, es:[bx+2]
    add     ax, word_44D20
    add     ax, 5Ah ; 'Z'
    mov     [bp+var_vec4.vy], ax
    mov     ax, es:[bx+4]
    jmp     loc_1A300  ; obs! var_vec4.z = ax on next line*/
loc_1A4D0:
	var_angZ = 0;
    //mov     [bp+var_angZ], 0
loc_1A4D6:
	if (state.game_frame != 0)
		goto loc_1A4E4;
	var_E4 = byte_3C0C6[word_46468&0xF];
	goto loc_1A4E8;
/*    cmp     state.game_frame, 0
    jnz     short loc_1A4E4
    mov     bx, word_46468
    jmp     short loc_1A4E8
    ; align 2
    db 144*/
loc_1A4E4:
	var_E4 = byte_3C0C6[state.game_frame&0xF];
//    mov     bx, state.game_frame
loc_1A4E8:
/*
    and     bx, 0Fh
    mov     al, byte_3C0C6[bx]
    mov     [bp+var_E4], al*/

	var_52 = select_cliprect_rotate(var_angZ, var_angX, var_angY, arg_cliprectptr, 0);
	var_50 = off_3C084[(var_52 & 0x3FF) >> 7];
    /*sub     ax, ax
    push    ax
    push    [bp+arg_cliprectptr]
    push    [bp+var_angY]
    push    [bp+var_angX]
    push    [bp+var_angZ]
    call    select_cliprect_rotate
    add     sp, 0Ah
    mov     [bp+var_52], ax
    and     ah, 3
loc_1A512:
    mov     cl, 7
    shr     ax, cl
    mov     si, ax
    mov     bx, si
    shl     bx, 1
    mov     ax, off_3C084[bx]
    mov     [bp+var_50], ax*/
	var_mat = *mat_rot_zxy(var_angZ, var_angX, 0, 1);
	var_vec6.x = 0;
	var_vec6.y = 0;
	var_vec6.z = 0x3E8;
	mat_mul_vector(&var_vec6, &var_mat, &var_vec8);
	if (var_vec8.z <= 0)
		goto loc_1A580;
	var_2 = 1;
	goto loc_1A585;
/*    mov     ax, 1
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_angX]
    push    [bp+var_angZ]
    call    mat_rot_zxy
    add     sp, 8
    push    si
    push    di
    lea     di, [bp+var_mat]
    mov     si, ax
    push    ss
    pop     es
    mov     cx, 9
    repne movsw
    pop     di
    pop     si
    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vy], 0
    mov     [bp+var_vec6.vz], 3E8h
    lea     ax, [bp+var_vec8]
    push    ax
    lea     ax, [bp+var_mat]
    push    ax
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_vec8.vz], 0
    jle     short loc_1A580
    mov     [bp+var_2], 1
    jmp     short loc_1A585*/
loc_1A580:
	var_2 = -1;
    //mov     [bp+var_2], 0FFFFh
loc_1A585:
	if (timertestflag2 == 0)
		goto loc_1A58F;
	goto loc_1A66D;
/*    cmp     timertestflag2, 0
    jz      short loc_1A58F
    jmp     loc_1A66D*/

loc_1A58F:
	currenttransshape[0].rectptr = &rect_unk9;
	currenttransshape[0].ts_flags = var_122 | 7;
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].unk = 0x400;
	currenttransshape[0].material = 0;
	var_counter = 0;
/*    mov     currenttransshape[0].ts_rectptr, offset rect_unk9
    mov     al, [bp+var_122]
    or      al, 7
    mov     currenttransshape[0].ts_flags, al
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     currenttransshape[0].ts_unk, 400h
    mov     currenttransshape[0].ts_material, 0
    mov     [bp+var_counter], 0*/
loc_1A5BA:
	si = (word_3BE34[var_counter] + var_angY + run_game_random) & 0x3ff;
	if (si < 0x87)
		goto loc_1A5DD;
	if (si > 0x379)
		goto loc_1A5DD;
	goto loc_1A661;
/*    mov     bx, [bp+var_counter]
    shl     bx, 1
    mov     si, word_3BE34[bx]
    add     si, [bp+var_angY]
    add     si, run_game_random
    and     si, 3FFh
    cmp     si, 87h ; '‡'
    jl      short loc_1A5DD
    cmp     si, 379h
    jg      short loc_1A5DD
    jmp     loc_1A661*/
loc_1A5DD:
	mat_rot_y(&var_mat2, si);
	var_vec6.x = 0;
	var_vec6.y = 0xAE6 - var_vec4.y;
	var_vec6.z = 0x3A98; //15000
	mat_mul_vector(&var_vec6, &var_mat2, &var_vec7);
	var_vec7.z = 0x3A98; //15000
	mat_mul_vector(&var_vec7, &var_mat, &currenttransshape[0].pos);
	if (currenttransshape[0].pos.z <= 0xC8)
		goto loc_1A661;
	/*
    push    si
    lea     ax, [bp+var_mat2]
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     [bp+var_vec6.vx], 0
    mov     ax, 0AE6h
    sub     ax, [bp+var_vec4.vy]
    mov     [bp+var_vec6.vy], ax
    mov     [bp+var_vec6.vz], 3A98h; 15000
    lea     ax, [bp+var_vec7]
    push    ax
    lea     ax, [bp+var_mat2]
    push    ax
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     [bp+var_vec7.vz], 3A98h; 15000
    mov     ax, offset currenttransshape
    push    ax
    lea     ax, [bp+var_mat]
    push    ax
    lea     ax, [bp+var_vec7]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     currenttransshape[0].ts_pos.vz, 0C8h ; 'È'
    jle     short loc_1A661*/
	currenttransshape[0].shapeptr = off_3BE44[var_counter];
	currenttransshape[0].rotvec.z = -var_angY;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
/*    mov     bx, [bp+var_counter]
    shl     bx, 1
    mov     ax, off_3BE44[bx]
    mov     currenttransshape[0].ts_shapeptr, ax
    mov     ax, [bp+var_angY]
    neg     ax
    mov     currenttransshape[0].ts_rotvec.vz, ax
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax*/
loc_1A661:
	var_counter++;
	if (var_counter >= 8)
		goto loc_1A66D;
	goto loc_1A5BA;
/*
    inc     [bp+var_counter]
    cmp     [bp+var_counter], 8
    jge     short loc_1A66D
    jmp     loc_1A5BA

; -----------------------------------------------------------------------------------------------
*/
loc_1A66D:
	var_pos2adjust = var_vec4.x >> 0xA;
	var_posadjust = -((var_vec4.z >> 0xA) - 0x1D);
	if (timertestflag2 == 0)
		goto loc_1A69D;
	var_D8 = state.playerstate.car_posWorld1.lx >> 16;
	var_E2 = 0x1D - (state.playerstate.car_posWorld1.lz >> 16);
/*    mov     ax, [bp+var_vec4.vx]
    mov     cl, 0Ah
    sar     ax, cl
    mov     [bp+var_pos2adjust], al
    mov     ax, [bp+var_vec4.vz]
    sar     ax, cl
    sub     al, 1Dh
    neg     al
    mov     [bp+var_posadjust], al
    cmp     timertestflag2, 0
    jz      short loc_1A69D
    mov     al, byte ptr state.playerstate.car_posWorld1.lx+2
    mov     [bp+var_D8], al
    mov     al, 1Dh
    sub     al, byte ptr state.playerstate.car_posWorld1.lz+2
    mov     [bp+var_E2], al*/
loc_1A69D:
	si = 0;
    //sub     si, si
loc_1A69F:
	var_32[si] = 0;
	si++;
	if (si < 0x17)
		goto loc_1A69F;
	var_130 = byte_3C09C[timertestflag2];
	si = 0x16;
	goto loc_1A6FF;
/*    mov     [bp+si+var_32], 0
    inc     si
    cmp     si, 17h
    jl      short loc_1A69F
    mov     bl, timertestflag2
    sub     bh, bh
    mov     al, byte_3C09C[bx]
    mov     [bp+var_130], al
    mov     si, 16h
    jmp     short loc_1A6FF*/

loc_1A6BC:
	// this lastpos2lookup looks like its used for completely different things
	var_pos2lookup = var_50[si * 3] + var_pos2adjust;
	var_poslookup = var_50[si * 3 + 1] + var_posadjust;
	if (var_pos2lookup < 0)
		goto loc_1A6FA;
	if (var_pos2lookup > 0x1D)
		goto loc_1A6FA;
	if (var_poslookup < 0)
		goto loc_1A6FA;
	if (var_poslookup <= 0x1D)
		goto loc_1A724;
/*    mov     ax, [bp+var_50]
    mov     cx, si ; cx = si
    mov     dx, cx
    shl     cx, 1 ; cx *= 2
    add     cx, dx ; cx += si
    add     ax, cx ; ax = var_50 + si * 3
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    mov     al, [bx]
    add     al, [bp+var_pos2adjust]
    mov     [bp+var_pos2lookup], al
    mov     al, [bx+1]
    add     al, [bp+var_posadjust]
    mov     [bp+var_poslookup], al
    cmp     [bp+var_pos2lookup], 0
    jl      short loc_1A6FA
    cmp     [bp+var_pos2lookup], 1Dh
    jg      short loc_1A6FA
    or      al, al
    jl      short loc_1A6FA
    cmp     al, 1Dh
    jle     short loc_1A724*/
loc_1A6FA:
	var_32[si] = 2;
    //mov     [bp+si+var_32], 2
loc_1A6FE:
	si--;
    //dec     si
loc_1A6FF:
	if (si >= 0)
		goto loc_1A706;
	goto loc_1A9C8;
/*    or      si, si
    jge     short loc_1A706
    jmp     loc_1A9C8
*/
loc_1A706:
	if (var_32[si] != 0)
		goto loc_1A6FE;
	if (var_50[si * 3 + 2] <= var_130)
		goto loc_1A6BC;
	goto loc_1A6FA;
/*    cmp     [bp+si+var_32], 0
    jnz     short loc_1A6FE
    mov     bx, [bp+var_50]
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bp+var_130]
    cmp     [bx+2], al
    jle     short loc_1A6BC
    jmp     short loc_1A6FA
*/
	
loc_1A724:
	elem_map_value = td14_elem_map_main[var_pos2lookup + trackrows[var_poslookup]];
	terr_map_value = td15_terr_map_main[var_pos2lookup + terrainrows[var_poslookup]];
	
	if (elem_map_value != 0)
		goto loc_1A774;
	goto loc_1A7FF;
/*    mov     al, [bp+var_pos2lookup]
    cbw
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     al, [bp+var_poslookup]
    cbw
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, word ptr td14_elem_map_main
    mov     es, word ptr td14_elem_map_main+2
    mov     al, es:[bx]
    mov     [bp+elem_map_value], al
    mov     bx, [bp+var_lastposlookupw]
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, word ptr td15_terr_map_main
    mov     es, word ptr td15_terr_map_main+2
    mov     al, es:[bx]
    mov     [bp+terr_map_value], al
    cmp     [bp+elem_map_value], 0
    jnz     short loc_1A774
    jmp     loc_1A7FF
	*/
loc_1A774:
	if (terr_map_value < 7)
		goto loc_1A797;
	if (terr_map_value >= 0xB)
		goto loc_1A797;
	elem_map_value = subst_hillroad_track(terr_map_value, elem_map_value);
	terr_map_value = 0;
/*    cmp     al, 7
    jb      short loc_1A797
    cmp     al, 0Bh
    jnb     short loc_1A797
    mov     al, [bp+elem_map_value]
    sub     ah, ah
    push    ax
    mov     al, [bp+terr_map_value]
    push    ax
    call    subst_hillroad_track
    add     sp, 4
    mov     [bp+elem_map_value], al
    mov     [bp+terr_map_value], 0*/
loc_1A797:
	if (elem_map_value == 0xFD)
		goto loc_1A7B0;
	if (elem_map_value == 0xFE)
		goto loc_1A7B4;
	if (elem_map_value != 0xFF)
		goto loc_1A7AE;
	goto loc_1A8CA;
/*    mov     al, [bp+elem_map_value]
    sub     ah, ah
    cmp     ax, 0FDh ; 'ý'
    jz      short loc_1A7B0
    cmp     ax, 0FEh ; 'þ'
    jz      short loc_1A7B4
    cmp     ax, 0FFh
    jnz     short loc_1A7AE
    jmp     loc_1A8CA
*/
loc_1A7AE:
	goto loc_1A7FF;
loc_1A7B0:
	var_pos2lookup--;
loc_1A7B4:
	var_poslookup--;
loc_1A7B8:
	elem_map_value = td14_elem_map_main[var_pos2lookup + trackrows[var_poslookup]];
	terr_map_value = td15_terr_map_main[var_pos2lookup + terrainrows[var_poslookup]];
/*	
	var_lastpos2lookup = var_pos2lookup;
    mov     al, [bp+var_pos2lookup]
    cbw
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     al, [bp+var_poslookup]
    cbw
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, word ptr td14_elem_map_main
    mov     es, word ptr td14_elem_map_main+2
    mov     al, es:[bx]
    mov     [bp+elem_map_value], al
    mov     bx, [bp+var_lastposlookupw]
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, word ptr td15_terr_map_main
    mov     es, word ptr td15_terr_map_main+2
    mov     al, es:[bx]
    mov     [bp+terr_map_value], al*/
loc_1A7FF:
	var_1A[si] = terr_map_value;
	var_BC[si] = var_50[si * 3 + 2];
	if (elem_map_value == 0)
		goto loc_1A857;
	if (timertestflag2 == 0)
		goto loc_1A857;
	if (trkObjectList[elem_map_value].ss_physicalModel < 0x40)
		goto loc_1A857;
	if (var_pos2lookup != var_D8)
		goto loc_1A853;
	if (var_poslookup == var_E2)
		goto loc_1A857;
/*
    mov     al, [bp+terr_map_value]
    mov     [bp+si+var_1A], al
    mov     bx, [bp+var_50]
    mov     ax, si
    mov     cx, ax   ; cx = si
    shl     ax, 1    ; ax = si * 2
    add     ax, cx   ; ax = (si * 2) + si
    add     bx, ax   ; bx = var_50 + si*3
    mov     al, [bx+2]
    mov     [bp+si+var_BC], al
    cmp     [bp+elem_map_value], 0
    jz      short loc_1A857
    cmp     timertestflag2, 0
    jz      short loc_1A857
    mov     al, [bp+elem_map_value]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    cmp     trkObjectList.ss_physicalModel[bx], 40h ; '@'
    jl      short loc_1A857
    mov     al, [bp+var_D8]
    cmp     [bp+var_pos2lookup], al
    jnz     short loc_1A853
    mov     al, [bp+var_E2]
    cmp     [bp+var_poslookup], al
    jz      short loc_1A857*/
loc_1A853:
	elem_map_value = 0;
loc_1A857:
	var_pos2tab[si] = var_pos2lookup;
	var_postab[si] = var_poslookup;
	var_14C[si] = elem_map_value;
	if (elem_map_value != 0)
		goto loc_1A877;
	goto loc_1A6FE;
/*    mov     al, [bp+var_pos2lookup]
    mov     [bp+si+var_pos2tab], al
    mov     al, [bp+var_poslookup]
    mov     [bp+si+var_postab], al
    mov     al, [bp+elem_map_value]
    mov     [bp+si+var_14C], al
    cmp     [bp+elem_map_value], 0
    jnz     short loc_1A877
    jmp     loc_1A6FE*/
	
loc_1A877:
	temp1 = sizeof(struct TRACKOBJECT);
	temp2 = &trkObjectList[elem_map_value];
	var_multitileflag = trkObjectList[elem_map_value].ss_multiTileFlag;
	if (var_multitileflag != 0)
		goto loc_1A898;
	goto loc_1A6FE;
/*    mov     al, [bp+elem_map_value]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, trkObjectList.ss_multiTileFlag[bx]
    cbw
    mov     [bp+var_multitileflag], ax
    or      ax, ax
    jnz     short loc_1A898
    jmp     loc_1A6FE*/
loc_1A898:
	var_pos2lookupadjust = var_pos2lookup - var_pos2adjust;
	var_poslookupadjust = var_poslookup - var_posadjust;
	if (var_multitileflag == 1)
		goto loc_1A8D2;
	if (var_multitileflag == 2)
		goto loc_1A914;
	if (var_multitileflag != 3)
		goto loc_1A8C6;
	goto loc_1A960;
/*
    mov     al, [bp+var_pos2lookup]
    sub     al, [bp+var_pos2adjust]
    mov     [bp+var_pos2lookupadjust], al
    mov     al, [bp+var_poslookup]
    sub     al, [bp+var_posadjust]
    mov     [bp+var_poslookupadjust], al
    mov     ax, [bp+var_multitileflag]
    cmp     ax, 1
    jz      short loc_1A8D2
    cmp     ax, 2
    jz      short loc_1A914
    cmp     ax, 3
    jnz     short loc_1A8C6
    jmp     loc_1A960*/
loc_1A8C6:
    goto loc_1A6FE;
	
loc_1A8CA:
	var_pos2lookup--;
	goto loc_1A7B8;
loc_1A8D2:
	di = 0;
	goto loc_1A8D7;
loc_1A8D6:
	di++;
loc_1A8D7:
	if (di < si)
		goto loc_1A8DE;
	goto loc_1A6FE;
loc_1A8DE:
	if (var_50[di * 3] != var_pos2lookupadjust)
		goto loc_1A8D6;
	if (var_50[di * 3 + 1] == var_poslookupadjust)
		goto loc_1A90E;
	if (var_50[di * 3 + 1] != var_poslookupadjust + 1)
		goto loc_1A8D6;
/*    mov     ax, [bp+var_50]
    mov     cx, di
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    mov     al, [bp+var_pos2lookupadjust]
    cmp     [bx], al
    jnz     short loc_1A8D6
    mov     al, [bp+var_poslookupadjust]
    cmp     [bx+1], al
    jz      short loc_1A90E
    cbw
    inc     ax
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    cmp     ax, cx
    jnz     short loc_1A8D6*/
loc_1A90E:
	var_32[di] = 1;
	goto loc_1A8D6;
loc_1A914:
	di = 0;
	goto loc_1A919;
loc_1A918:
	di++;
loc_1A919:
	if (di < si)
		goto loc_1A920;
	goto loc_1A6FE;
loc_1A920:
	if (var_50[si * 3 + 1] != var_poslookupadjust)
		goto loc_1A918;
	if (var_50[si * 3] == var_pos2lookupadjust)
		goto loc_1A959;
	if (var_50[si * 3] != var_pos2lookupadjust + 1)
		goto loc_1A918;
/*    mov     ax, [bp+var_50]
    mov     cx, di
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    mov     al, [bp+var_poslookupadjust]
    cmp     [bx+1], al
    jnz     short loc_1A918
    mov     al, [bx]
    mov     byte ptr [bp+var_lastposlookupw], al
    mov     al, [bp+var_pos2lookupadjust]
    cmp     byte ptr [bp+var_lastposlookupw], al
    jz      short loc_1A959
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_lastposlookupw]
    cbw
    cmp     cx, ax
    jnz     short loc_1A918*/
loc_1A959:
	var_32[di] = 1;
	goto loc_1A918;
	
loc_1A960:
	di = 0;
	goto loc_1A965;

loc_1A964:
	di++;
loc_1A965:
	if (di < si)
		goto loc_1A96C;
    goto loc_1A6FE;
loc_1A96C:
	if (var_50[di * 3] == var_pos2lookupadjust)
		goto loc_1A996;
	if (var_50[di * 3] != var_pos2lookupadjust + 1)
		goto loc_1A964;
/*    mov     bx, [bp+var_50]
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bx]
    mov     byte ptr [bp+var_lastpos2lookup], al
    mov     al, [bp+var_pos2lookupadjust]
    cmp     byte ptr [bp+var_lastpos2lookup], al
    jz      short loc_1A996
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_lastpos2lookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1A964*/
loc_1A996:
	if (var_50[di * 3 + 1] == var_poslookupadjust)
		goto loc_1A9C1;
	if (var_50[di * 3 + 1] != var_poslookupadjust + 1)
		goto loc_1A964;
/*    mov     bx, [bp+var_50]
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bx+1]
    mov     byte ptr [bp+var_lastpos2lookup], al
    mov     al, [bp+var_poslookupadjust]
    cmp     byte ptr [bp+var_lastpos2lookup], al
    jz      short loc_1A9C1
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_lastpos2lookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1A964*/
loc_1A9C1:
	var_32[di] = 1;
	goto loc_1A964;
	
//; -----------------------------------------------------------------------------
	
loc_1A9C8:
	var_3C = -1;
	var_6C = 0;
	if (cameramode != 0)
		goto loc_1A9E2;
	if (followOpponentFlag != 0)
		goto loc_1A9E2;
	goto loc_1AB65;
/*  mov     [bp+var_3C], 0FFh
    mov     [bp+var_6C], 0
    cmp     cameramode, 0
    jnz     short loc_1A9E2
    cmp     followOpponentFlag, 0
    jnz     short loc_1A9E2
    jmp     loc_1AB65*/
loc_1A9E2:
	if (state.playerstate.car_crashBmpFlag != 2)
		goto loc_1A9EC;
	goto loc_1AB65;
/*    cmp     state.playerstate.car_crashBmpFlag, 2
    jnz     short loc_1A9EC
    jmp     loc_1AB65*/
loc_1A9EC:
	var_matptr = mat_rot_zxy(-state.playerstate.car_rotate.z, -state.playerstate.car_rotate.y, -state.playerstate.car_rotate.x, 0);
	var_multitileflag = -1;
	di = -1;
	var_counter2 = 0;
	goto loc_1AA8C;
/*    sub     ax, ax
    push    ax
    mov     ax, state.playerstate.car_rotate.vx
    neg     ax
    push    ax
    mov     ax, state.playerstate.car_rotate.vy
    neg     ax
    push    ax
    mov     ax, state.playerstate.car_rotate.vz
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_matptr], ax
    mov     [bp+var_multitileflag], 0FFFFh
    mov     di, 0FFFFh
    mov     [bp+var_counter2], 0
    jmp     short loc_1AA8C*/

loc_1AA1E:
	si--;
loc_1AA1F:
	if (var_multitileflag >= si)
		goto loc_1AA88;
	if (var_32[si] == 2)
		goto loc_1AA1E;
	// ???:
	if (var_50[si * 3] + var_pos2adjust != var_pos2lookup)
		goto loc_1AA1E;
	if (var_50[si * 3 + 1] + var_posadjust != var_poslookup)
		goto loc_1AA1E;
	var_3C = var_pos2lookup;
	var_60 = var_poslookup;
	var_multitileflag = var_poslookup;
	di = var_counter2;
	goto loc_1AA1E;
/*    cmp     [bp+var_multitileflag], si
    jge     short loc_1AA88
    cmp     [bp+si+var_32], 2
    jz      short loc_1AA1E
    mov     ax, [bp+var_50]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     al, [bp+var_pos2adjust]
    cbw
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     cx, ax
    mov     al, [bx]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_pos2lookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1AA1E
    mov     al, [bp+var_posadjust]
    cbw
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_poslookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1AA1E
    mov     al, [bp+var_pos2lookup]
    mov     [bp+var_3C], al
    mov     al, [bp+var_poslookup]
    mov     [bp+var_60], al
    mov     [bp+var_multitileflag], si
    mov     di, [bp+var_counter2]
    jmp     short loc_1AA1E
    ; align 2
    db 144
*/
loc_1AA88:
	var_counter2++;
    //inc     [bp+var_counter2]
loc_1AA8C:
	if (var_counter2 >= 4)
		goto loc_1AAF4;
	var_vec6 = simd_player.wheel_coords[var_counter2];
/*    cmp     [bp+var_counter2], 4
    jge     short loc_1AAF4
    mov     bx, [bp+var_counter2]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    push    si
    push    di
    lea     di, [bp+var_vec6]
    lea     si, simd_player.wheel_coords.vx[bx]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si*/
	mat_mul_vector(&var_vec6, var_matptr, &var_vec8); //; rotating car wheels, maybe?
	var_pos2lookup = (var_vec8.x + state.playerstate.car_posWorld1.lx) >> 16; // bits 16-24
	var_poslookup = -(((var_vec8.z + state.playerstate.car_posWorld1.lz) >> 16) - 0x1D);
	si = 0x16;
	goto loc_1AA1F;
/*    lea     ax, [bp+var_vec8]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector  ; rotating car wheels, maybe?
    add     sp, 6
    mov     ax, [bp+var_vec8.vx]
    cwd
    add     ax, word ptr state.playerstate.car_posWorld1.lx
    adc     dx, word ptr state.playerstate.car_posWorld1.lx+2
    mov     ax, dx
    mov     [bp+var_pos2lookup], al
    mov     ax, [bp+var_vec8.vz]
    cwd
    add     ax, word ptr state.playerstate.car_posWorld1.lz
    adc     dx, word ptr state.playerstate.car_posWorld1.lz+2
    mov     ax, dx
    sub     al, 1Dh
    neg     al
    mov     [bp+var_poslookup], al
    mov     si, 16h
    jmp     loc_1AA1F
    ; align 2
    db 144*/
	

loc_1AAF4:
	if (di == -1)//0xFFFF)
		goto loc_1AB65;
	if (state.playerstate.car_surfaceWhl[0] != 4)
		goto loc_1AB15;
	if (state.playerstate.car_surfaceWhl[1] != 4)
		goto loc_1AB15;
	if (state.playerstate.car_surfaceWhl[2] != 4)
		goto loc_1AB15;
	if (state.playerstate.car_surfaceWhl[3] == 4)
		goto loc_1AB65;
/*
    cmp     di, 0FFFFh
    jz      short loc_1AB65
    cmp     state.playerstate.car_surfaceWhl1, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfaceWhl2, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfaceWhl3, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfaceWhl4, 4
    jz      short loc_1AB65*/
loc_1AB15:
	var_vec6.x = 0;
	var_vec6.z = 0;
	var_vec6.y = 0x7530;
	mat_mul_vector(&var_vec6, var_matptr, &var_vec8);
	mat_mul_vector(&var_vec8, &mat_temp, &var_vec6);
	if (var_vec6.z > 0)
		goto loc_1AB60;
	var_6C = -0x800 /*0xF800*/;
	goto loc_1AB65;
/*    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vz], 0
    mov     [bp+var_vec6.vy], 7530h
    lea     ax, [bp+var_vec8]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_vec6]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    lea     ax, [bp+var_vec8]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_vec6.vz], 0
    jg      short loc_1AB60
    mov     [bp+var_6C], 0F800h
    jmp     short loc_1AB65
    ; align 2
    db 144
*/
loc_1AB60:
	var_6C = 0x800;
    //mov     [bp+var_6C], 800h
loc_1AB65:
	var_4A = -1;//0xFF;
	var_A4 = 0;
	if (gameconfig.game_opponenttype != 0)
		goto loc_1AB79;
	goto loc_1AD0E;
/*    mov     [bp+var_4A], 0FFh
    mov     [bp+var_A4], 0
    cmp     gameconfig.game_opponenttype, 0
    jnz     short loc_1AB79
    jmp     loc_1AD0E*/
	
loc_1AB79:
	if (cameramode != 0)
		goto loc_1AB8A;
	if (followOpponentFlag == 0)
		goto loc_1AB8A;
	goto loc_1AD0E;
/*    cmp     cameramode, 0
    jnz     short loc_1AB8A
    cmp     followOpponentFlag, 0
    jz      short loc_1AB8A
    jmp     loc_1AD0E*/
loc_1AB8A:
	if (state.opponentstate.car_crashBmpFlag != 2)
		goto loc_1AB94;
	goto loc_1AD0E;
/*    cmp     state.opponentstate.car_crashBmpFlag, 2
    jnz     short loc_1AB94
    jmp     loc_1AD0E*/
loc_1AB94:
	var_matptr = mat_rot_zxy(-state.opponentstate.car_rotate.z, -state.opponentstate.car_rotate.y, -state.opponentstate.car_rotate.x, 0);
	var_multitileflag = -1;//0xFFFF;
	di = -1;//0xFFFF;
	var_counter2 = 0;
	goto loc_1AC34;
/*    sub     ax, ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.vx
    neg     ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.vy
    neg     ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.vz
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_matptr], ax
    mov     [bp+var_multitileflag], 0FFFFh
    mov     di, 0FFFFh
    mov     [bp+var_counter2], 0
    jmp     short loc_1AC34*/

//; this chunk of code here is used below, before loc_1AC9C
loc_1ABC6:
	si--;
    //dec     si
loc_1ABC7:
	if (var_multitileflag >= si)
		goto loc_1AC30;
	if (var_32[si] == 2)
		goto loc_1ABC6;
	if (var_50[si * 3] + var_pos2adjust != var_pos2lookup)
		goto loc_1ABC6;
	if (var_50[si * 3 + 1] + var_posadjust != var_poslookup)
		goto loc_1ABC6;
	var_4A = var_pos2lookup;
	var_6E = var_poslookup;
	var_multitileflag = si;//var_poslookup;
	di = var_counter2;
	goto loc_1ABC6;
/*    cmp     [bp+var_multitileflag], si
    jge     short loc_1AC30
    cmp     [bp+si+var_32], 2
    jz      short loc_1ABC6
    mov     ax, [bp+var_50]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     al, [bp+var_pos2adjust]
    cbw
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     cx, ax
    mov     al, [bx]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_pos2lookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1ABC6
    mov     al, [bp+var_posadjust]
    cbw
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_poslookup]
    cbw
    cmp     cx, ax
    jnz     short loc_1ABC6
    mov     al, [bp+var_pos2lookup]
    mov     [bp+var_4A], al
    mov     al, [bp+var_poslookup]
    mov     [bp+var_6E], al
    mov     [bp+var_multitileflag], si
    mov     di, [bp+var_counter2]
    jmp     short loc_1ABC6
    ; align 2
    db 144
	*/	
loc_1AC30:
	var_counter2++;
    //inc     [bp+var_counter2]
loc_1AC34:
	if (var_counter2 >= 4)
		goto loc_1AC9C;
	var_vec6 = simd_opponent.wheel_coords[var_counter2];
	/*
    cmp     [bp+var_counter2], 4
    jge     short loc_1AC9C
    mov     bx, [bp+var_counter2]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    push    si
    push    di
    lea     di, [bp+var_vec6]
    lea     si, simd_opponent.wheel_coords.vx[bx]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si*/
	mat_mul_vector(&var_vec6, var_matptr, &var_vec8);
	var_pos2lookup = (var_vec8.x + state.opponentstate.car_posWorld1.lx) >> 16;
	var_poslookup = -(((var_vec8.z + state.opponentstate.car_posWorld1.lz) >> 16) - 0x1D);
	si = 0x16;
	goto loc_1ABC7;
/*    lea     ax, [bp+var_vec8]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_vec8.vx]
    cwd
    add     ax, word ptr state.opponentstate.car_posWorld1.lx
    adc     dx, word ptr state.opponentstate.car_posWorld1.lx+2
    mov     ax, dx
    mov     [bp+var_pos2lookup], al
    mov     ax, [bp+var_vec8.vz]
    cwd
    add     ax, word ptr state.opponentstate.car_posWorld1.lz
    adc     dx, word ptr state.opponentstate.car_posWorld1.lz+2
    mov     ax, dx
    sub     al, 1Dh
    neg     al
    mov     [bp+var_poslookup], al
    mov     si, 16h
    jmp     loc_1ABC7
    ; align 2
    db 144
	*/
loc_1AC9C:
	if (di == -1)//0xFFFF)
		goto loc_1AD0E;
	if (state.opponentstate.car_surfaceWhl[0] != 4)
		goto loc_1ACBD;
	if (state.opponentstate.car_surfaceWhl[1] != 4)
		goto loc_1ACBD;
	if (state.opponentstate.car_surfaceWhl[2] != 4)
		goto loc_1ACBD;
	if (state.opponentstate.car_surfaceWhl[3] == 4)
		goto loc_1AD0E;

/*	cmp     di, 0FFFFh
    jz      short loc_1AD0E
    cmp     state.opponentstate.car_surfaceWhl1, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfaceWhl2, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfaceWhl3, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfaceWhl4, 4
    jz      short loc_1AD0E*/
loc_1ACBD:
	var_vec6.x = 0;
	var_vec6.y = 0;
	var_vec6.z = 0x7530;
	mat_mul_vector(&var_vec6, var_matptr, &var_vec8);
	mat_mul_vector(&var_vec8, &mat_temp, &var_vec6);
	if (var_vec6.z > 0)
		goto loc_1AD08;
	var_A4 = -0x800; //0xF800; // signed number!
	goto loc_1AD0E;
/*
	
    mov     [bp+var_vec6.vx], 0
    mov     [bp+var_vec6.vz], 0
    mov     [bp+var_vec6.vy], 7530h
    lea     ax, [bp+var_vec8]
    push    ax
    push    [bp+var_matptr]
    lea     ax, [bp+var_vec6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_vec6]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    lea     ax, [bp+var_vec8]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_vec6.vz], 0
    jg      short loc_1AD08
    mov     [bp+var_A4], 0F800h
    jmp     short loc_1AD0E*/
loc_1AD08:
	var_A4 = 0x800;
    //mov     [bp+var_A4], 800h
	
//; -----------------------------------------------------------------------------

loc_1AD0E:
	var_4E = 0;
	si = 0;
	goto loc_1BF6D;
/*    mov     [bp+var_4E], 0
    sub     si, si
    jmp     loc_1BF6D

    ; align 2
    db 144

	
; this is called from almost at the end just before loc_1BFD5*/
loc_1AD18:
	var_counter = 1;
	var_10E = unk_3C0EE;
	goto loc_1AD55;
/*    mov     [bp+var_counter], 1
    mov     [bp+var_10E], offset unk_3C0EE
    jmp     short loc_1AD55
    ; align 2
    db 144*/
loc_1AD26:
	var_counter = 2;
	var_10E = unk_3C0F0;
	goto loc_1AD55;
/*    mov     [bp+var_counter], 2
    mov     [bp+var_10E], offset unk_3C0F0
    jmp     short loc_1AD55
    ; align 2
    db 144*/
loc_1AD34:
	var_counter = 3;
	goto loc_1AD4F;
/*    mov     [bp+var_counter], 3
    jmp     short loc_1AD4F
    ; align 2
    db 144*/
loc_1AD3C:
	var_counter = 4;
	var_10E = unk_3C0F8;
	goto loc_1AD55;
/*    mov     [bp+var_counter], 4
    mov     [bp+var_10E], offset unk_3C0F8
    jmp     short loc_1AD55
    ; align 2
    db 144*/
loc_1AD4A:
	var_counter = 1;
    //mov     [bp+var_counter], 1
loc_1AD4F:
	var_10E = unk_3C0F4;
    //mov     [bp+var_10E], offset unk_3C0F4
loc_1AD55:
	var_multitileflag = 0;
	goto loc_1AE7A;
/*    mov     [bp+var_multitileflag], 0
    jmp     loc_1AE7A*/
loc_1AD5E:
	if (var_pos2lookupadjust == 0)
		goto loc_1AD80;
	if (var_pos2lookupadjust == 0x1D)
		goto loc_1ADCC;

	if (var_poslookupadjust == 0)
		goto loc_1ADEC;
	if (var_poslookupadjust == 0x1D)
		goto loc_1ADF0;

	di = -1;//0xFFFF;
	goto loc_1AD97;
/*    mov     al, [bp+var_pos2lookupadjust]
    cbw
    or      ax, ax
    jz      short loc_1AD80
    cmp     ax, 1Dh
    jz      short loc_1ADCC
    mov     al, [bp+var_poslookupadjust]
    cbw
    or      ax, ax
    jz      short loc_1ADEC
    cmp     ax, 1Dh
    jz      short loc_1ADF0
    mov     di, 0FFFFh
    jmp     short loc_1AD97
    ; align 2
    db 144*/
loc_1AD80:
	if (var_poslookupadjust == 0)
		goto loc_1AD94;
	if (var_poslookupadjust == 0x1D)
		goto loc_1ADC6;
	di = 6;
	goto loc_1AD97;
/*    mov     al, [bp+var_poslookupadjust]
    cbw
    or      ax, ax
    jz      short loc_1AD94
    cmp     ax, 1Dh
    jz      short loc_1ADC6
    mov     di, 6
    jmp     short loc_1AD97
    ; align 2
    db 144*/
loc_1AD94:
	di = 7;
    //mov     di, 7
loc_1AD97:
	if (di != -1)//0xFFFF)
		goto loc_1AD9F;
	goto loc_1AE76;
/*    cmp     di, 0FFFFh
    jnz     short loc_1AD9F
    jmp     loc_1AE76*/
loc_1AD9F:
	var_trkobjectptr = &trkObjectList[fence_TrkObjCodes[di]];
	if (var_FC != 0)
		goto loc_1ADF6;
    currenttransshape[0].shapeptr = var_trkobjectptr->ss_shapePtr;
	goto loc_1ADFC;
/*    mov     al, fence_TrkObjCodes[di]; indices to corner shapes?
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1	
    add     ax, offset trkObjectList
    mov     [bp+var_trkobjectptr], ax
    cmp     [bp+var_FC], 0
    jnz     short loc_1ADF6
    mov     bx, ax
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    jmp     short loc_1ADFC
    ; align 2
    db 144*/
loc_1ADC6:
	di = 5;
	goto loc_1AD97;
loc_1ADCC:
	if (var_poslookupadjust == 0)
		goto loc_1ADE0;
	if (var_poslookupadjust == 0x1D)
		goto loc_1ADE6;
	di = 2;
	goto loc_1AD97;
/*    mov     al, [bp+var_poslookupadjust]
    cbw
    or      ax, ax
    jz      short loc_1ADE0
    cmp     ax, 1Dh
    jz      short loc_1ADE6
    mov     di, 2
    jmp     short loc_1AD97
    ; align 2
    db 144*/
loc_1ADE0:
	di = 1;
	goto loc_1AD97;
loc_1ADE6:
	di = 3;
	goto loc_1AD97;
loc_1ADEC:
	di = 0;
	goto loc_1AD97;
loc_1ADF0:
	di = 4;
	goto loc_1AD97;
loc_1ADF6:
	// also see above goto loc_1ADFC
	currenttransshape[0].shapeptr = var_trkobjectptr->ss_loShapePtr;
    //mov     bx, [bp+var_trkobjectptr]
    //mov     ax, [bx+TRACKOBJECT.ss_loShapePtr]
loc_1ADFC:
	currenttransshape[0].pos.x = trackcenterpos2[var_pos2lookupadjust] - var_vec4.x;
	currenttransshape[0].pos.y = -var_vec4.y;
	currenttransshape[0].pos.z = trackcenterpos[var_poslookupadjust] - var_vec4.z;
	currenttransshape[0].rectptr = &rect_unk2;
	currenttransshape[0].ts_flags = var_122 | 5;
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].rotvec.z = word_3C0D6[di];
	currenttransshape[0].unk = 0x400;
	currenttransshape[0].material = 0;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
	if (var_transformresult <= 0)
		goto loc_1AE76;
	goto loc_1B03C;
/*    mov     currenttransshape[0].ts_shapeptr, ax
    mov     al, [bp+var_pos2lookupadjust]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_vec4.vx]
    mov     currenttransshape[0].ts_pos.vx, ax
    mov     ax, [bp+var_vec4.vy]
    neg     ax
    mov     currenttransshape[0].ts_pos.vy, ax
    mov     al, [bp+var_poslookupadjust]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_vec4.vz]
    mov     currenttransshape[0].ts_pos.vz, ax
    mov     currenttransshape[0].ts_rectptr, offset rect_unk2
    mov     al, [bp+var_122]
    or      al, 5
    mov     currenttransshape[0].ts_flags, al
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     bx, di
    shl     bx, 1
    mov     ax, word_3C0D6[bx]
    mov     currenttransshape[0].ts_rotvec.vz, ax
    mov     currenttransshape[0].ts_unk, 400h
    mov     currenttransshape[0].ts_material, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1AE76
    jmp     loc_1B03C
*/
loc_1AE76:
	var_multitileflag++;
    //inc     [bp+var_multitileflag]
loc_1AE7A:
	if (var_multitileflag >= var_counter)
		goto loc_1AECC;
	var_pos2lookupadjust = var_10E[var_multitileflag * 2] + var_pos2lookup;
	var_poslookupadjust = var_10E[var_multitileflag * 2 + 1] + var_poslookup;
	if (timertestflag2 != 0)
		goto loc_1AEB2;
	goto loc_1AD5E;
/*    mov     ax, [bp+var_counter]
    cmp     [bp+var_multitileflag], ax
    jge     short loc_1AECC
    mov     ax, [bp+var_multitileflag]
    shl     ax, 1
    add     ax, [bp+var_10E]
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    mov     al, [bx]
    add     al, [bp+var_pos2lookup]
    mov     [bp+var_pos2lookupadjust], al
    mov     al, [bx+1]
    add     al, [bp+var_poslookup]
    mov     [bp+var_poslookupadjust], al
    cmp     timertestflag2, 0
    jnz     short loc_1AEB2
    jmp     loc_1AD5E*/
loc_1AEB2:
	if (var_pos2lookupadjust != var_D8)
		goto loc_1AE76;
	if (var_poslookupadjust != var_E2)
		goto loc_1AEC9;
	goto loc_1AD5E;
/*    mov     al, [bp+var_D8]
    cmp     [bp+var_pos2lookupadjust], al
    jnz     short loc_1AE76
    mov     al, [bp+var_E2]
    cmp     [bp+var_poslookupadjust], al
    jnz     short loc_1AEC9
    jmp     loc_1AD5E*/
loc_1AEC9:
	goto loc_1AE76;
loc_1AECC:
	// terrain type 0x06: a flat piece of land at an elevated level  
	if (terr_map_value != 6)
		goto loc_1AF50;

	var_hillheight = hillHeightConsts[1];
	if (elem_map_value == 0)
		goto loc_1AEE4;
/*    cmp     [bp+terr_map_value], 6
    jnz     short loc_1AF50
    mov     ax, hillHeightConsts+2
    mov     [bp+var_hillheight], ax
    cmp     [bp+elem_map_value], 0
    jz      short loc_1AEE4*/
loc_1AEDF:
	terr_map_value = 0;
    //mov     [bp+terr_map_value], 0
loc_1AEE4:
	if (terr_map_value != 0)
		goto loc_1AEEE;
	goto loc_1B11C;
/*    cmp     [bp+terr_map_value], 0
    jnz     short loc_1AEEE
    jmp     loc_1B11C*/
loc_1AEEE:
	var_trkobject_ptr = &sceneshapes2[terr_map_value];
	currenttransshape[0].shapeptr = var_trkobject_ptr->ss_shapePtr;
	currenttransshape[0].pos.x = trackcenterpos2[var_pos2lookup] - var_vec4.x;
	currenttransshape[0].pos.y = var_hillheight - var_vec4.y;
	currenttransshape[0].pos.z = trackcenterpos[var_poslookup] - var_vec4.z;
	if (var_hillheight == 0)
		goto loc_1AF47;
	goto loc_1B0D4;
/*    mov     al, [bp+terr_map_value]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes2
    mov     [bp+var_trkobject_ptr], ax
    mov     bx, ax
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     currenttransshape[0].ts_shapeptr, ax
    mov     al, [bp+var_pos2lookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_vec4.vx]
    mov     currenttransshape[0].ts_pos.vx, ax
    mov     ax, [bp+var_hillheight]
    sub     ax, [bp+var_vec4.vy]
    mov     currenttransshape[0].ts_pos.vy, ax
    mov     al, [bp+var_poslookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_vec4.vz]
    mov     currenttransshape[0].ts_pos.vz, ax
    cmp     [bp+var_hillheight], 0
    jz      short loc_1AF47
    jmp     loc_1B0D4*/
loc_1AF47:
	currenttransshape[0].rectptr = &rect_unk2;
	goto loc_1B0DA;
    //mov     currenttransshape[0].ts_rectptr, offset rect_unk2
    //jmp     loc_1B0DA
loc_1AF50:
	var_hillheight = 0;
	if (elem_map_value < 0x69)
		goto loc_1AEE4;
	if (elem_map_value <= 0x6C)
		goto loc_1AF67;
	goto loc_1AEE4;
/*    mov     [bp+var_hillheight], 0
    mov     al, [bp+elem_map_value]
    sub     ah, ah
    cmp     ax, 69h ; 'i'
    jb      short loc_1AEE4
    cmp     ax, 6Ch ; 'l'
    jbe     short loc_1AF67
    jmp     loc_1AEE4*/
loc_1AF67:
	var_multitileflag = 0;
	goto loc_1B0AC;
/*    mov     [bp+var_multitileflag], 0
    jmp     loc_1B0AC*/
loc_1AF70:
	var_pos2lookupadjust = var_pos2lookup;
    //mov     al, [bp+var_pos2lookup]
loc_1AF74:
	var_poslookupadjust = var_poslookup;
/*    mov     [bp+var_pos2lookupadjust], al
    mov     al, [bp+var_poslookup]*/
loc_1AF7C:
    //mov     [bp+var_poslookupadjust], al
loc_1AF80:
	terr_map_value = td15_terr_map_main[var_pos2lookupadjust + terrainrows[var_poslookupadjust]];
	if (terr_map_value != 0)
		goto loc_1AFB4;
	goto loc_1B0A8;
/*
    mov     al, [bp+var_pos2lookupadjust]
    cbw
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     al, [bp+var_poslookupadjust]
    cbw
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, word ptr td15_terr_map_main
    mov     es, word ptr td15_terr_map_main+2
    mov     al, es:[bx]
    mov     [bp+terr_map_value], al
    or      al, al
    jnz     short loc_1AFB4
    jmp     loc_1B0A8*/
loc_1AFB4:
	var_trkobject_ptr = &sceneshapes2[terr_map_value];
	currenttransshape[0].shapeptr = var_trkobject_ptr->ss_shapePtr;
	currenttransshape[0].pos.x = trackcenterpos2[var_pos2lookupadjust] - var_vec4.x;
	currenttransshape[0].pos.y = -var_vec4.y;
	currenttransshape[0].pos.z = trackcenterpos[var_poslookupadjust] - var_vec4.z;
	currenttransshape[0].rectptr = &rect_unk2;
	currenttransshape[0].ts_flags = var_122 | 5;
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].rotvec.z = var_trkobject_ptr->ss_rotY;
	currenttransshape[0].unk = 0x400;
	currenttransshape[0].material = 0;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
	if (var_transformresult <= 0)
		goto loc_1B0A8;
/*    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes2
    mov     [bp+var_trkobject_ptr], ax
    mov     bx, ax
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     currenttransshape[0].ts_shapeptr, ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_vec4.vx]
    mov     currenttransshape[0].ts_pos.vx, ax
    mov     ax, [bp+var_vec4.vy]
    neg     ax
    mov     currenttransshape[0].ts_pos.vy, ax
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_vec4.vz]
    mov     currenttransshape[0].ts_pos.vz, ax
    mov     currenttransshape[0].ts_rectptr, offset rect_unk2
    mov     al, [bp+var_122]
    or      al, 5
    mov     currenttransshape[0].ts_flags, al
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_rotY]
    mov     currenttransshape[0].ts_rotvec.vz, ax
    mov     currenttransshape[0].ts_unk, 400h
    mov     currenttransshape[0].ts_material, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1B0A8*/

loc_1B03C:
	var_132 = skybox_op(arg_0, arg_cliprectptr, var_2, &var_mat, var_angZ, var_angY, var_vec4.y);
	sprite_set_1_size(0, 0x140, arg_cliprectptr->top, arg_cliprectptr->bottom);
	get_a_poly_info();
	si = 0;
	goto loc_1C0C2;
/*    push    [bp+var_vec4.vy]
    push    [bp+var_angY]
    push    [bp+var_angZ]
    lea     ax, [bp+var_mat]
    push    ax
    push    [bp+var_2]
    push    [bp+arg_cliprectptr]
    mov     al, [bp+arg_0]
    cbw
    push    ax
    call skybox_op
    add     sp, 0Eh
    mov     [bp+var_132], ax
    mov     bx, [bp+arg_cliprectptr]
    push    [bx+RECTANGLE.rc_bottom]
    push    [bx+RECTANGLE.rc_top]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    sprite_set_1_size
    add     sp, 8
    call    get_a_poly_info
    sub     si, si
    jmp     loc_1C0C2

    ; align 2
    db 144*/
loc_1B084:
	var_pos2lookupadjust = var_pos2lookup + 1;
	goto loc_1AF74;
/*    mov     al, [bp+var_pos2lookup]
    inc     al
    jmp     loc_1AF74
    ; align 2
    db 144*/
loc_1B08E:	
	var_pos2lookupadjust = var_pos2lookup;
    //mov     al, [bp+var_pos2lookup]
loc_1B092:
	var_poslookupadjust = var_poslookup + 1;
	goto loc_1AF7C;
/*    mov     [bp+var_pos2lookupadjust], al
    mov     al, [bp+var_poslookup]
    inc     al
    jmp     loc_1AF7C
    ; align 2
    db 144*/
loc_1B0A0:
	var_pos2lookupadjust = var_pos2lookup + 1;
	goto loc_1B092;
/*    mov     al, [bp+var_pos2lookup]
    inc     al
    jmp     short loc_1B092*/
loc_1B0A8:
	var_multitileflag++;
    //inc     [bp+var_multitileflag]
loc_1B0AC:
	if (var_multitileflag < 4)
		goto loc_1B0B6;
	goto loc_1AEDF;
/*    cmp     [bp+var_multitileflag], 4
    jl      short loc_1B0B6
    jmp     loc_1AEDF*/
loc_1B0B6:
	if (var_multitileflag != 0)
		goto loc_1B0C1;
	goto loc_1AF70;
/*    mov     ax, [bp+var_multitileflag]
    or      ax, ax
    jnz     short loc_1B0C1
    jmp     loc_1AF70*/
loc_1B0C1:
	if (var_multitileflag == 1)
		goto loc_1B084;
	if (var_multitileflag == 2)
		goto loc_1B08E;
	if (var_multitileflag == 3)
		goto loc_1B0A0;
	goto loc_1AF80;
/*
    cmp     ax, 1
    jz      short loc_1B084
    cmp     ax, 2
    jz      short loc_1B08E
    cmp     ax, 3
    jz      short loc_1B0A0
    jmp     loc_1AF80
    ; align 2
    db 144*/
loc_1B0D4:
	currenttransshape[0].rectptr = &rect_unk6;
    //mov     currenttransshape[0].ts_rectptr, offset rect_unk6
loc_1B0DA:
	currenttransshape[0].ts_flags = var_122 | 5;
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].rotvec.z = var_trkobject_ptr->ss_rotY;
	currenttransshape[0].unk = 0x400;
	currenttransshape[0].material = 0;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
	if (var_transformresult <= 0)
		goto loc_1B11C;
	goto loc_1B03C;
/*    mov     al, [bp+var_122]
    or      al, 5
    mov     currenttransshape[0].ts_flags, al
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_rotY]
    mov     currenttransshape[0].ts_rotvec.vz, ax
    mov     currenttransshape[0].ts_unk, 400h
    mov     currenttransshape[0].ts_material, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1B11C
    jmp     loc_1B03C
*/
loc_1B11C:
	transformedshape_counter = 0;
	curtransshape_ptr = currenttransshape;
	if (elem_map_value != 0)
		goto loc_1B130;
	goto loc_1B71E;	
/*    mov     transformedshape_counter, 0
    mov     curtransshape_ptr, offset currenttransshape
    cmp     [bp+elem_map_value], 0
    jnz     short loc_1B130
    jmp     loc_1B71E*/
loc_1B130:
	var_trkobject_ptr = &trkObjectList[elem_map_value];
	if ((var_trkobject_ptr->ss_multiTileFlag & 1) == 0)
		goto loc_1B168;
	var_5E = trackpos[var_poslookup];
	var_poslookupadjust = var_poslookup + 1;
	goto loc_1B17C;
/*    mov     al, [bp+elem_map_value]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset trkObjectList
    mov     [bp+var_trkobject_ptr], ax
    mov     bx, ax
    test    [bx+TRACKOBJECT.ss_multiTileFlag], 1
    jz      short loc_1B168
    mov     al, [bp+var_poslookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackpos[bx]
    mov     [bp+var_5E], ax
    mov     al, [bp+var_poslookup]
    inc     al
    jmp     short loc_1B17C*/
loc_1B168:
	var_5E = trackcenterpos[var_poslookup];
	var_poslookupadjust = var_poslookup;
/*    mov     al, [bp+var_poslookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    mov     [bp+var_5E], ax
    mov     al, [bp+var_poslookup]*/
loc_1B17C:
	if ((var_trkobject_ptr->ss_multiTileFlag & 2) == 0)
		goto loc_1B1A2;
	var_3A = trackpos2[1 + var_pos2lookup];
	var_pos2lookupadjust = var_pos2lookup + 1;
	goto loc_1B1B6;
/*    mov     [bp+var_poslookupadjust], al
    mov     bx, [bp+var_trkobject_ptr]
    test    [bx+TRACKOBJECT.ss_multiTileFlag], 2
    jz      short loc_1B1A2
    mov     al, [bp+var_pos2lookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, (trackpos2+2)[bx]
    mov     [bp+var_3A], ax
    mov     al, [bp+var_pos2lookup]
    inc     al
    jmp     short loc_1B1B6*/
loc_1B1A2:
	var_3A = trackcenterpos2[var_pos2lookup];
	var_pos2lookupadjust = var_pos2lookup;
/*    mov     al, [bp+var_pos2lookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    mov     [bp+var_3A], ax
    mov     al, [bp+var_pos2lookup]*/
loc_1B1B6:
	var_vec8.x = var_3A - var_vec4.x;
	var_vec8.y = var_hillheight - var_vec4.y;
	var_vec8.z = var_5E - var_vec4.z;
	if (var_hillheight != 0)
		goto loc_1B1DE;
	goto loc_1B2AE;
/*    mov     [bp+var_pos2lookupadjust], al
    mov     ax, [bp+var_3A]
    sub     ax, [bp+var_vec4.vx]
    mov     [bp+var_vec8.vx], ax
    mov     ax, [bp+var_hillheight]
    sub     ax, [bp+var_vec4.vy]
    mov     [bp+var_vec8.vy], ax
    mov     ax, [bp+var_5E]
    sub     ax, [bp+var_vec4.vz]
    mov     [bp+var_vec8.vz], ax
    cmp     [bp+var_hillheight], 0
    jnz     short loc_1B1DE
    jmp     loc_1B2AE*/
loc_1B1DE:
	if (var_trkobject_ptr->ss_multiTileFlag == 0)
		goto loc_1B1FC;
	if (var_trkobject_ptr->ss_multiTileFlag == 1)
		goto loc_1B20E;
	if (var_trkobject_ptr->ss_multiTileFlag == 2)
		goto loc_1B21A;
	if (var_trkobject_ptr->ss_multiTileFlag == 3)
		goto loc_1B226;
	goto loc_1B205;
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     al, [bx+TRACKOBJECT.ss_multiTileFlag]
    cbw
    or      ax, ax
    jz      short loc_1B1FC
    cmp     ax, 1
    jz      short loc_1B20E
    cmp     ax, 2
    jz      short loc_1B21A
    cmp     ax, 3
    jz      short loc_1B226
    jmp     short loc_1B205
    ; align 2
    db 144*/
loc_1B1FC:
	di = 1;
	var_DA = unk_3C0A2;
    /*mov     di, 1
    mov     [bp+var_DA], offset unk_3C0A2*/
loc_1B205:
	var_multitileflag = 0;
	goto loc_1B236;
/*    mov     [bp+var_multitileflag], 0
    jmp     short loc_1B236
    ; align 2
    db 144*/
loc_1B20E:
	di = 2;
	var_DA = unk_3C0A6;
	goto loc_1B205;
/*    mov     di, 2
    mov     [bp+var_DA], offset unk_3C0A6
    jmp     short loc_1B205
    ; align 2
    db 144*/
loc_1B21A:
	di = 2;
	var_DA = unk_3C0AE;
	goto loc_1B205;
/*    mov     di, 2
    mov     [bp+var_DA], offset unk_3C0AE
    jmp     short loc_1B205
    ; align 2
    db 144*/
loc_1B226:
	di = 4;
	var_DA = unk_3C0B6;
	goto loc_1B205;
/*    mov     di, 4
    mov     [bp+var_DA], offset unk_3C0B6
    jmp     short loc_1B205
    ; align 2
    db 144*/
loc_1B232:
	var_multitileflag++;
    //inc     [bp+var_multitileflag]
loc_1B236:
	if (var_multitileflag>= di)
		goto loc_1B2AE;
	currenttransshape[0].pos.x = *var_DA + var_vec8.x;
	var_DA++;
	currenttransshape[0].pos.y = var_vec8.y;
	currenttransshape[0].pos.z = *var_DA + var_vec8.z;
	var_DA++;
	currenttransshape[0].shapeptr = &game3dshapes[0x3B2 / sizeof(struct SHAPE3D)]; // whatevs
	currenttransshape[0].rectptr = &rect_unk6;
	currenttransshape[0].ts_flags = var_122 | 5;
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].rotvec.z = 0;
	currenttransshape[0].unk = 0x800;
	currenttransshape[0].material = 0;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
	if (var_transformresult <= 0)
		goto loc_1B232;
	goto loc_1B03C;
/*    cmp     [bp+var_multitileflag], di
    jge     short loc_1B2AE
    mov     bx, [bp+var_DA]
    add     [bp+var_DA], 2
    mov     ax, [bx]
    add     ax, [bp+var_vec8.vx]
    mov     currenttransshape[0].ts_pos.vx, ax
    mov     ax, [bp+var_vec8.vy]
    mov     currenttransshape[0].ts_pos.vy, ax
    mov     bx, [bp+var_DA]
    add     [bp+var_DA], 2
    mov     ax, [bx]
    add     ax, [bp+var_vec8.vz]
    mov     currenttransshape[0].ts_pos.vz, ax
    mov     currenttransshape[0].ts_shapeptr, (offset game3dshapes.shape3d_numverts+3B2h)
    mov     currenttransshape[0].ts_rectptr, offset rect_unk6
    mov     al, [bp+var_122]
    or      al, 5
    mov     currenttransshape[0].ts_flags, al
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     currenttransshape[0].ts_rotvec.vz, 0
    mov     currenttransshape[0].ts_unk, 800h
    mov     currenttransshape[0].ts_material, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1B232
    jmp     loc_1B03C*/

loc_1B2AE:
	if (var_trkobject_ptr->ss_ssOvelay != 0)
		goto loc_1B2BB;
	goto loc_1B374;
/*    mov     bx, [bp+var_trkobject_ptr]
    cmp     [bx+TRACKOBJECT.ss_ssOvelay], 0
    jnz     short loc_1B2BB
    jmp     loc_1B374*/
loc_1B2BB:
	var_trkobjectptr = &trkObjectList[var_trkobject_ptr->ss_ssOvelay];
	if (var_FC == 0)
		goto loc_1B2E0;
	currenttransshape[1].shapeptr = var_trkobjectptr->ss_loShapePtr;
	goto loc_1B2E6;
/*    mov     al, [bx+TRACKOBJECT.ss_ssOvelay]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset trkObjectList
    mov     [bp+var_trkobjectptr], ax
    cmp     [bp+var_FC], 0
    jz      short loc_1B2E0
    mov     bx, ax
    mov     ax, [bx+TRACKOBJECT.ss_loShapePtr]
    jmp     short loc_1B2E6*/
loc_1B2E0:
	currenttransshape[1].shapeptr = var_trkobjectptr->ss_shapePtr;
/*  mov     bx, [bp+var_trkobjectptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]*/
loc_1B2E6:
	if (currenttransshape[1].shapeptr != 0)
		goto loc_1B2F0;
	goto loc_1B374;
/*    mov     currenttransshape[1].ts_shapeptr, ax
    or      ax, ax
    jnz     short loc_1B2F0
    jmp     loc_1B374*/
loc_1B2F0:
	currenttransshape[1].pos = var_vec8; // ?? which member?
	currenttransshape[1].rotvec.x = 0;
	currenttransshape[1].rotvec.y = 0;
	currenttransshape[1].rotvec.z = var_trkobjectptr->ss_rotY;
	if (var_trkobjectptr->ss_multiTileFlag == 0)
		goto loc_1B320;
	currenttransshape[1].unk = 0x400;
	goto loc_1B326;
/*    push    si
    push    di
    mov     di, offset transshapeunk
    lea     si, [bp+var_vec8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     currenttransshape[1].ts_rotvec.vx, 0
    mov     currenttransshape[1].ts_rotvec.vy, 0
    mov     ax, [bx+TRACKOBJECT.ss_rotY]
    mov     currenttransshape[1].ts_rotvec.vz, ax
    cmp     [bx+TRACKOBJECT.ss_multiTileFlag], 0
    jz      short loc_1B320
    mov     currenttransshape[1].ts_unk, 400h
    jmp     short loc_1B326
    ; align 2
    db 144*/
loc_1B320:
	currenttransshape[1].unk = 0x800;
    //mov     currenttransshape[1].ts_unk, 800h
loc_1B326:
	if (var_trkobjectptr->ss_surfaceType < 0)
		goto loc_1B332;
    currenttransshape[1].material = var_trkobjectptr->ss_surfaceType;
	goto loc_1B336;
/*  cmp     [bx+TRACKOBJECT.ss_surfaceType], 0
    jl      short loc_1B332
    mov     al, [bx+TRACKOBJECT.ss_surfaceType]
    jmp     short loc_1B336
    ; align 2
    db 144*/
loc_1B332:
	currenttransshape[1].material = var_E4;
    //mov     al, [bp+var_E4]
loc_1B336:
	currenttransshape[1].ts_flags = var_trkobjectptr->ss_ignoreZBias | var_122 | 4;
	if ((currenttransshape[1].ts_flags & 1) == 0)
		goto loc_1B36A;
	currenttransshape[1].rectptr = &rect_unk2;
	var_transformresult = transformed_shape_op(&currenttransshape[1]);
	if (var_transformresult <= 0)
		goto loc_1B374;
	goto loc_1B03C;
/*    mov     currenttransshape[1].ts_material, al
    mov     al, [bx+TRACKOBJECT.ss_ignoreZBias]
    or      al, [bp+var_122]
    or      al, 4
    mov     currenttransshape[1].ts_flags, al
    test    currenttransshape[1].ts_flags, 1
    jz      short loc_1B36A
    mov     currenttransshape[1].ts_rectptr, offset rect_unk2
    mov     ax, offset transshapeunk
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1B374
    jmp     loc_1B03C*/
loc_1B36A:
	currenttransshape[1].rectptr = &rect_unk6;
	var_4E = 1;
    //mov     currenttransshape[1].ts_rectptr, offset rect_unk6 ; 92A4h
    //mov     [bp+var_4E], 1
loc_1B374:
	if (var_FC == 0)
		goto loc_1B384;
	currenttransshape[0].shapeptr = var_trkobject_ptr->ss_loShapePtr;
	goto loc_1B38B;
/*    cmp     [bp+var_FC], 0
    jz      short loc_1B384
    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_loShapePtr]
    jmp     short loc_1B38B*/
loc_1B384:
	currenttransshape[0].shapeptr = var_trkobject_ptr->ss_shapePtr;
    //mov     bx, [bp+var_trkobject_ptr]
    //mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
loc_1B38B:
	currenttransshape[0].pos = var_vec8; // whatever
	currenttransshape[0].rotvec.x = 0;
	currenttransshape[0].rotvec.y = 0;
	currenttransshape[0].rotvec.z = var_trkobject_ptr->ss_rotY;
	if (var_trkobject_ptr->ss_multiTileFlag == 0)
		goto loc_1B3BE;
	currenttransshape[0].unk = 0x400;
	goto loc_1B3C4;
/*    mov     currenttransshape[0].ts_shapeptr, ax
    push    si
    push    di
    mov     di, offset currenttransshape
    lea     si, [bp+var_vec8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     currenttransshape[0].ts_rotvec.vx, 0
    mov     currenttransshape[0].ts_rotvec.vy, 0
    mov     ax, [bx+TRACKOBJECT.ss_rotY]
    mov     currenttransshape[0].ts_rotvec.vz, ax
    cmp     [bx+TRACKOBJECT.ss_multiTileFlag], 0
    jz      short loc_1B3BE
    mov     currenttransshape[0].ts_unk, 400h
    jmp     short loc_1B3C4
    ; align 2
    db 144*/
loc_1B3BE:
	currenttransshape[0].unk = 0x800;
    //mov     currenttransshape[0].ts_unk, 800h
loc_1B3C4:
	currenttransshape[0].ts_flags = var_trkobject_ptr->ss_ignoreZBias | var_122 | 4;
	if (var_trkobject_ptr->ss_surfaceType < 0)
		goto loc_1B3DC;
	currenttransshape[0].material = var_trkobject_ptr->ss_surfaceType;
	goto loc_1B3E0;
/*    mov     al, [bx+TRACKOBJECT.ss_ignoreZBias]
    or      al, [bp+var_122]
    or      al, 4
    mov     currenttransshape[0].ts_flags, al
    cmp     [bx+TRACKOBJECT.ss_surfaceType], 0
    jl      short loc_1B3DC
    mov     al, [bx+TRACKOBJECT.ss_surfaceType]
    jmp     short loc_1B3E0
    ; align 2
    db 144*/
loc_1B3DC:
	currenttransshape[0].material = var_E4;
    //mov     al, [bp+var_E4]
loc_1B3E0:
	if ((var_trkobject_ptr->ss_ignoreZBias & 1) == 0)
		goto loc_1B408;
	currenttransshape[0].rectptr = &rect_unk2;
	var_transformresult = transformed_shape_op(&currenttransshape[0]);
	if (var_transformresult <= 0)
		goto loc_1B46A;
	goto loc_1B03C;
/*    mov     currenttransshape[0].ts_material, al
    test    [bx+TRACKOBJECT.ss_ignoreZBias], 1
    jz      short loc_1B408
    mov     currenttransshape[0].ts_rectptr, offset rect_unk2
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1B46A
    jmp     loc_1B03C

    ; align 2
    db 144*/
loc_1B408:
	currenttransshape[0].rectptr = &rect_unk6;
	transformed_shape_add_for_sort(0, 0);
	if (var_4E == 0)
		goto loc_1B449;
	var_4E = 0;
	transformed_shape_add_for_sort(-0x800 /*0xF800*/, 0);
	if (var_6C == 0)
		goto loc_1B43C;
	var_6C = -0x400;//0xFC00;
/*    mov     currenttransshape[0].ts_rectptr, offset rect_unk6
    sub     ax, ax
    push    ax
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4
    cmp     [bp+var_4E], 0
    jz      short loc_1B449
    mov     [bp+var_4E], 0
    sub     ax, ax
    push    ax
    mov     ax, 0F800h
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4
    cmp     [bp+var_6C], 0
    jz      short loc_1B43C
    mov     [bp+var_6C], 0FC00h*/
loc_1B43C:
	if (var_A4 == 0)
		goto loc_1B449;
	var_A4 -= 0x400;
/*    cmp     [bp+var_A4], 0
    jz      short loc_1B449
    sub     [bp+var_A4], 400h*/
loc_1B449:
	if (var_pos2lookup != startcol2)
		goto loc_1B464;
	if (var_poslookup != startrow2)
		goto loc_1B464;
	var_12A = 0;
	goto loc_1B46A;
/*    mov     al, startcol2
    cmp     [bp+var_pos2lookup], al
    jnz     short loc_1B464
    mov     al, startrow2
    cmp     [bp+var_poslookup], al
    jnz     short loc_1B464
    mov     [bp+var_12A], 0
    jmp     short loc_1B46A
    ; align 2
    db 144*/
loc_1B464:
	var_12A = -1;
    //mov     [bp+var_12A], 0FFFFh
loc_1B46A:
	var_4C = trackdata19[var_pos2lookup + trackrows[var_poslookup]];
	if (var_4C != 0xFF)
		goto loc_1B493;
	goto loc_1B72E;
/*    mov     al, [bp+var_poslookup]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_pos2lookup]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata19
    mov     es, word ptr trackdata19+2
    mov     al, es:[bx]
    mov     [bp+var_4C], al
    cmp     al, 0FFh
    jnz     short loc_1B493
    jmp     loc_1B72E*/
loc_1B493:
	if (state.field_3FA[var_4C] != 0)
		goto loc_1B4A0;
	goto loc_1B626;
/*    cbw
    mov     bx, ax
    cmp     state.field_3FA[bx], 0
    jnz     short loc_1B4A0
    jmp     loc_1B626*/
loc_1B4A0:
	if (state.field_42A != 0)
		goto loc_1B4AA;
	goto loc_1B72E;
/*    cmp     state.field_42A, 0
    jnz     short loc_1B4AA
    jmp     loc_1B72E*/
loc_1B4AA:
	di = 0;
    //sub     di, di
loc_1B4AC:
	if (state.field_38E[di] != 0)
		goto loc_1B4C0;
	goto loc_1B61A;
/*    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    cmp     word ptr state.field_38E[bx], 0
    jnz     short loc_1B4C0
    jmp     loc_1B61A*/
loc_1B4C0:
	if (var_4C + 2 == state.field_443[di])
		goto loc_1B4D4;
	goto loc_1B61A;
/*    mov     al, [bp+var_4C]
    cbw
    add     ax, 2
    mov     cl, state.field_443[di]
    sub     ch, ch
    cmp     ax, cx
    jz      short loc_1B4D4
    jmp     loc_1B61A*/
loc_1B4D4:
	var_trkobject_ptr = &sceneshapes3[state.field_42B[di]];
	// NOTE var_4C multiplier
	curtransshape_ptr->pos.x = (state.game_longs1[di] >> 6) + td10_track_check_rel[var_4C * 3 + 0] - var_vec4.x;
/*    mov     al, state.field_42B[di]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_trkobject_ptr], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     ax, word ptr state.game_longvecs1.lx[bx]
    mov     dx, word ptr (state.game_longvecs1.lx+2)[bx]
    mov     cl, 6
loc_1B503:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B503
    mov     cx, ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    add     cx, es:[bx]
    sub     cx, [bp+var_vec4.vx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], cx*/
	curtransshape_ptr->pos.y = (state.game_longs2[di] >> 6) + td10_track_check_rel[var_4C * 3 + 1] - var_vec4.y;
/*    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs2.lx[bx]
    mov     dx, word ptr (state.game_longvecs2.lx+2)[bx]
    mov     cl, 6
loc_1B53B:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B53B
    mov     cx, ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    add     cx, es:[bx+2]
    sub     cx, [bp+var_vec4.vy]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], cx*/
	curtransshape_ptr->pos.z = (state.game_longs3[di] >> 6) + td10_track_check_rel[var_4C * 3 + 2] - var_vec4.z;
/*	
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs3.lx[bx]
    mov     dx, word ptr (state.game_longvecs3.lx+2)[bx]
    mov     cl, 6
loc_1B575:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B575
    mov     cx, ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    add     cx, es:[bx+4]
    sub     cx, [bp+var_vec4.vz]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], cx*/
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	curtransshape_ptr->rectptr = &rect_unk6;
	curtransshape_ptr->ts_flags = var_122 | 5;
	curtransshape_ptr->rotvec.x = -state.field_2FE[di];
	curtransshape_ptr->rotvec.y = -state.field_32E[di];
	curtransshape_ptr->rotvec.z = -state.field_35E[di];
	curtransshape_ptr->unk = 0x400;
	curtransshape_ptr->material = 0;
    //mov     bx, [bp+var_trkobject_ptr]
	transformed_shape_add_for_sort(0, 0);
/*    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk6
    mov     bx, curtransshape_ptr
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], al
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_2FE[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], ax
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    mov     ax, state.field_32E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_35E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 400h
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_material], 0
    sub     ax, ax
    push    ax
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4*/
loc_1B61A:
	di++;
	if (di >= 0x18)
		goto loc_1B623;
	goto loc_1B4AC;
/*    inc     di
    cmp     di, 18h
    jge     short loc_1B623
    jmp     loc_1B4AC*/
loc_1B623:
	goto loc_1B72E;
loc_1B626:
	var_trkobject_ptr = &trkObjectList[212 + trackdata23[var_4C]];
	//var_trkobject_ptr = &trkObjectList[0x0B98 / sizeof(struct TRACKOBJECT) + trackdata23[var_4C]];
	curtransshape_ptr->pos.x = td10_track_check_rel[var_4C * 3 + 0] - var_vec4.x;
	curtransshape_ptr->pos.y = td10_track_check_rel[var_4C * 3 + 1] - var_vec4.y;
	curtransshape_ptr->pos.z = td10_track_check_rel[var_4C * 3 + 2] - var_vec4.z;
/*  mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    add     bx, word ptr trackdata23
    mov     es, word ptr trackdata23+2
    mov     al, es:[bx]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, (offset trkObjectList.ss_trkObjInfoPtr+0B98h)
    mov     [bp+var_trkobject_ptr], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    mov     ax, es:[bx]
    sub     ax, [bp+var_vec4.vx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    mov     ax, es:[bx+2]
    sub     ax, [bp+var_vec4.vy]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr td10_track_check_rel
    mov     es, word ptr td10_track_check_rel+2
    mov     ax, es:[bx+4]
    sub     ax, [bp+var_vec4.vz]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], ax*/
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	curtransshape_ptr->rectptr = &rect_unk6;
	curtransshape_ptr->ts_flags = var_122 | 4;
	curtransshape_ptr->rotvec.x = 0;
	curtransshape_ptr->rotvec.y = 0;
	curtransshape_ptr->rotvec.z = td08_direction_related[var_4C];
	curtransshape_ptr->unk = 0x64;
	curtransshape_ptr->material = 0;
	transformed_shape_add_for_sort(0, 0);
	goto loc_1B72E;
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk6
    mov     bx, curtransshape_ptr
    mov     al, [bp+var_122]
    or      al, 4
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], al
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], 0
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], 0
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, word ptr td08_direction_related
    mov     es, word ptr td08_direction_related+2
    mov     ax, es:[bx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 64h ; 'd'
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_material], 0
    sub     ax, ax
    push    ax
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4
    jmp     short loc_1B72E*/
loc_1B71E:
	var_pos2lookupadjust = var_pos2lookup;
	var_poslookupadjust = var_poslookup;
/*    mov     al, [bp+var_pos2lookup]
    mov     [bp+var_pos2lookupadjust], al
    mov     al, [bp+var_poslookup]
    mov     [bp+var_poslookupadjust], al*/
loc_1B72E:
	if (var_3C == var_pos2lookup)
		goto loc_1B743;
	if (var_3C == var_pos2lookupadjust)
		goto loc_1B743;
	goto loc_1B9DA;
/*    mov     al, [bp+var_pos2lookup]
    cmp     [bp+var_3C], al
    jz      short loc_1B743
    mov     al, [bp+var_pos2lookupadjust]
    cmp     [bp+var_3C], al
    jz      short loc_1B743
    jmp     loc_1B9DA*/
loc_1B743:
	if (var_60 == var_poslookup)
		goto loc_1B758;
	if (var_60 == var_poslookupadjust)
		goto loc_1B758;
	goto loc_1B9DA;
/*    mov     al, [bp+var_poslookup]
    cmp     [bp+var_60], al
    jz      short loc_1B758
    mov     al, [bp+var_poslookupadjust]
    cmp     [bp+var_60], al
    jz      short loc_1B758
    jmp     loc_1B9DA*/
loc_1B758:
	if (state.field_42A != 0)
		goto loc_1B762;
	goto loc_1B89F;
/*    cmp     state.field_42A, 0
    jnz     short loc_1B762
    jmp     loc_1B89F*/
loc_1B762:
	di = 0;
loc_1B764:
	if (state.field_38E[di] != 0)
		goto loc_1B778;
	goto loc_1B896;
/*    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    cmp     word ptr state.field_38E[bx], 0
    jnz     short loc_1B778
    jmp     loc_1B896*/
loc_1B778:
	if (state.field_443[di] == 0)
		goto loc_1B782;
	goto loc_1B896;
/*    cmp     state.field_443[di], 0
    jz      short loc_1B782
    jmp     loc_1B896*/
loc_1B782:
	var_trkobject_ptr = &sceneshapes3[state.field_42B[di]];
	curtransshape_ptr->pos.x = (state.game_longs1[di] + state.playerstate.car_posWorld1.lx >> 6) - var_vec4.x;
	curtransshape_ptr->pos.y = (state.game_longs2[di] + state.playerstate.car_posWorld1.ly >> 6) - var_vec4.y;
	curtransshape_ptr->pos.z = (state.game_longs3[di] + state.playerstate.car_posWorld1.lz >> 6) - var_vec4.z;
/*    mov     al, state.field_42B[di]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_trkobject_ptr], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     ax, word ptr state.game_longvecs1.lx[bx]
    mov     dx, word ptr (state.game_longvecs1.lx+2)[bx]
    add     ax, word ptr state.playerstate.car_posWorld1.lx
    adc     dx, word ptr state.playerstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1B7B9:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B7B9
    sub     ax, [bp+var_vec4.vx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], ax
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs2.lx[bx]
    mov     dx, word ptr (state.game_longvecs2.lx+2)[bx]
    add     ax, word ptr state.playerstate.car_posWorld1.ly
    adc     dx, word ptr state.playerstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1B7E0:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B7E0
    sub     ax, [bp+var_vec4.vy]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs3.lx[bx]
    mov     dx, word ptr (state.game_longvecs3.lx+2)[bx]
    add     ax, word ptr state.playerstate.car_posWorld1.lz
    adc     dx, word ptr state.playerstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1B808:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B808
    sub     ax, [bp+var_vec4.vz]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], ax*/
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	curtransshape_ptr->rectptr = &rect_unk6;
	curtransshape_ptr->ts_flags = var_122 | 5;
	curtransshape_ptr->rotvec.x = -state.field_2FE[di];
	curtransshape_ptr->rotvec.y = -state.field_32E[di];
	curtransshape_ptr->rotvec.z = -state.field_35E[di];
	curtransshape_ptr->unk = 0x400;
	curtransshape_ptr->material = gameconfig.game_playermaterial;
	transformed_shape_add_for_sort(var_6C & var_12A, 0);
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk6
    mov     bx, curtransshape_ptr
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], al
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_2FE[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_32E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_35E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 400h
    mov     bx, curtransshape_ptr
    mov     al, gameconfig.game_playermaterial
    mov     [bx+TRANSFORMEDSHAPE.ts_material], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_6C]
    and     ax, [bp+var_12A]
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4*/
loc_1B896:
	di++;
	if (di >= 0x18)
		goto loc_1B89F;
	goto loc_1B764;
/*    inc     di
    cmp     di, 18h
    jge     short loc_1B89F
    jmp     loc_1B764*/
loc_1B89F:
	var_trkobject_ptr = &trkObjectList[2];//0x1C / sizeof(struct TRACKOBJECT)];
	curtransshape_ptr->pos.x = (state.playerstate.car_posWorld1.lx >> 6) - var_vec4.x;
	curtransshape_ptr->pos.y = (state.playerstate.car_posWorld1.ly >> 6) - var_vec4.y;
	curtransshape_ptr->pos.z = (state.playerstate.car_posWorld1.lz >> 6) - var_vec4.z;
/*
    mov     [bp+var_trkobject_ptr], (offset trkObjectList.ss_trkObjInfoPtr+1Ch)
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.playerstate.car_posWorld1.lx
    mov     dx, word ptr state.playerstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1B8B2:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8B2
    sub     ax, [bp+var_vec4.vx]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], ax
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.playerstate.car_posWorld1.ly
    mov     dx, word ptr state.playerstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1B8CC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8CC
    sub     ax, [bp+var_vec4.vy]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.playerstate.car_posWorld1.lz
    mov     dx, word ptr state.playerstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1B8E7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8E7
    sub     ax, [bp+var_vec4.vz]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], ax*/
	if (var_FC != 0)
		goto loc_1B903;
	if (timertestflag2 <= 2)
		goto loc_1B914;
/*    cmp     [bp+var_FC], 0
    jnz     short loc_1B903
    cmp     timertestflag2, 2
    jbe     short loc_1B914*/
loc_1B903:
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_loShapePtr;
	goto loc_1B94A;
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_loShapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    jmp     short loc_1B94A
    ; align 2
    db 144*/
loc_1B914:
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	sub_204AE(&game3dshapes[0x0AD4 / sizeof(struct SHAPE3D)].shape3d_verts[8], state.playerstate.car_steeringAngle, &state.playerstate.car_rc2, word_443E8, carshapevecs, &carshapevec);
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     ax, offset carshapevec
    push    ax
    mov     ax, offset carshapevecs
    push    ax
    mov     ax, offset word_443E8
    push    ax
    mov     ax, offset state.playerstate.car_rc2
    push    ax
    push    state.playerstate.car_steeringAngle
    mov     ax, word ptr game3dshapes.shape3d_verts+0AD4h
    mov     dx, word ptr game3dshapes.shape3d_verts+0AD6h
    add     ax, 30h ; '0'
    push    dx
    push    ax
    call    sub_204AE
    add     sp, 0Eh*/
loc_1B94A:
	if (timertestflag_copy == 0)
		goto loc_1B964;
	curtransshape_ptr->rectptr = &rect_unk12;
/*    cmp     timertestflag_copy, 0
    jz      short loc_1B964
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk12*/
loc_1B95A:
	curtransshape_ptr->ts_flags = 0xC;
	goto loc_1B990;
/*    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], 0Ch
    jmp     short loc_1B990*/
loc_1B964:
	if (state.playerstate.car_crashBmpFlag != 1)
		goto loc_1B988;
	var_rect = cliprect_unk;
	curtransshape_ptr->rectptr = &var_rect;
	goto loc_1B95A;
/*    cmp     state.playerstate.car_crashBmpFlag, 1
    jnz     short loc_1B988
    push    si
    push    di
    lea     di, [bp+var_rect]
    mov     si, offset cliprect_unk
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, curtransshape_ptr
    lea     ax, [bp+var_rect]
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], ax
    jmp     short loc_1B95A
    ; align 2
    db 144*/
loc_1B988:
	curtransshape_ptr->ts_flags = 4;
//    mov     bx, curtransshape_ptr
//    mov     [bx+TRANSFORMEDSHAPE.ts_flags], 4
loc_1B990:
	curtransshape_ptr->rotvec.x = -state.playerstate.car_rotate.z;
	curtransshape_ptr->rotvec.y = -state.playerstate.car_rotate.y;
	curtransshape_ptr->rotvec.z = -state.playerstate.car_rotate.x;
	curtransshape_ptr->unk = 0x12C;
	curtransshape_ptr->material = gameconfig.game_playermaterial;
	transformed_shape_add_for_sort(var_6C & var_12A, 2);
/*    mov     bx, curtransshape_ptr
    mov     ax, state.playerstate.car_rotate.vz
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], ax
    mov     bx, curtransshape_ptr
    mov     ax, state.playerstate.car_rotate.vy
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], ax
    mov     bx, curtransshape_ptr
    mov     ax, state.playerstate.car_rotate.vx
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 12Ch
    mov     bx, curtransshape_ptr
    mov     al, gameconfig.game_playermaterial
    mov     [bx+TRANSFORMEDSHAPE.ts_material], al
    mov     ax, 2
    push    ax
    mov     ax, [bp+var_6C]
    and     ax, [bp+var_12A]
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4*/
loc_1B9DA:
	if (var_4A == var_pos2lookup)
		goto loc_1B9EF;
	if (var_4A == var_pos2lookupadjust)
		goto loc_1B9EF;
	goto loc_1BC89;
/*    mov     al, [bp+var_pos2lookup]
    cmp     [bp+var_4A], al
    jz      short loc_1B9EF
    mov     al, [bp+var_pos2lookupadjust]
    cmp     [bp+var_4A], al
    jz      short loc_1B9EF
    jmp     loc_1BC89*/
loc_1B9EF:
	if (var_6E == var_poslookup)
		goto loc_1BA04;
	if (var_6E == var_poslookupadjust)
		goto loc_1BA04;
	goto loc_1BC89;
/*    mov     al, [bp+var_poslookup]
    cmp     [bp+var_6E], al
    jz      short loc_1BA04
    mov     al, [bp+var_poslookupadjust]
    cmp     [bp+var_6E], al
    jz      short loc_1BA04
    jmp     loc_1BC89*/
loc_1BA04:
	if (state.field_42A != 0)
		goto loc_1BA0E;
	goto loc_1BB4C;
/*    cmp     state.field_42A, 0
    jnz     short loc_1BA0E
    jmp     loc_1BB4C*/
loc_1BA0E:
    di = 0;
loc_1BA10:
	if (state.field_38E[di] != 0)
		goto loc_1BA24;
	goto loc_1BB43;
/*    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    cmp     word ptr state.field_38E[bx], 0
    jnz     short loc_1BA24
    jmp     loc_1BB43*/
loc_1BA24:
	if (state.field_443[di] == 1)
		goto loc_1BA2E;
	goto loc_1BB43;
/*    cmp     state.field_443[di], 1
    jz      short loc_1BA2E
    jmp     loc_1BB43*/
loc_1BA2E:
	var_trkobject_ptr = &sceneshapes3[state.field_42B[di]];
	curtransshape_ptr->pos.x = (state.game_longs1[di] + state.opponentstate.car_posWorld1.lx >> 6) - var_vec4.x;
	curtransshape_ptr->pos.y = (state.game_longs2[di] + state.opponentstate.car_posWorld1.ly >> 6) - var_vec4.y;
	curtransshape_ptr->pos.z = (state.game_longs3[di] + state.opponentstate.car_posWorld1.lz >> 6) - var_vec4.z;
/*
    mov     al, state.field_42B[di]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_trkobject_ptr], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_lastposlookupw], ax
    mov     bx, ax
    mov     ax, word ptr state.game_longvecs1.lx[bx]
    mov     dx, word ptr (state.game_longvecs1.lx+2)[bx]
    add     ax, word ptr state.opponentstate.car_posWorld1.lx
    adc     dx, word ptr state.opponentstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1BA65:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BA65
    sub     ax, [bp+var_vec4.vx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], ax
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs2.lx[bx]
    mov     dx, word ptr (state.game_longvecs2.lx+2)[bx]
    add     ax, word ptr state.opponentstate.car_posWorld1.ly
    adc     dx, word ptr state.opponentstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1BA8C:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BA8C
    sub     ax, [bp+var_vec4.vy]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax
    mov     bx, [bp+var_lastposlookupw]
    mov     ax, word ptr state.game_longvecs3.lx[bx]
    mov     dx, word ptr (state.game_longvecs3.lx+2)[bx]
    add     ax, word ptr state.opponentstate.car_posWorld1.lz
    adc     dx, word ptr state.opponentstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1BAB4:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BAB4
    sub     ax, [bp+var_vec4.vz]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], ax*/
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	curtransshape_ptr->rectptr = &rect_unk6;
	curtransshape_ptr->ts_flags = var_122 | 5;
	curtransshape_ptr->rotvec.x = -state.field_2FE[di];
	curtransshape_ptr->rotvec.y = -state.field_32E[di];
	curtransshape_ptr->rotvec.z = -state.field_35E[di];
	curtransshape_ptr->unk = 0x400;
	curtransshape_ptr->material = gameconfig.game_opponentmaterial;
	transformed_shape_add_for_sort(var_A4 & var_12A, 0);
/*
    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk6
    mov     bx, curtransshape_ptr
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], al
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_2FE[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_32E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], ax
    mov     bx, word ptr [bp+var_lastpos2lookup]
    mov     ax, state.field_35E[bx]
    neg     ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 400h
    mov     bx, curtransshape_ptr
    mov     al, gameconfig.game_opponentmaterial
    mov     [bx+TRANSFORMEDSHAPE.ts_material], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_A4]
    and     ax, [bp+var_12A]
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4*/
loc_1BB43:
	di++;
	if (di >= 0x18)
		goto loc_1BB4C;
	goto loc_1BA10;
/*    inc     di
    cmp     di, 18h
    jge     short loc_1BB4C
    jmp     loc_1BA10*/
loc_1BB4C:
	var_trkobject_ptr = &trkObjectList[3];//0x2A / sizeof(struct TRACKOBJECT)];
	curtransshape_ptr->pos.x = (state.opponentstate.car_posWorld1.lx >> 6) - var_vec4.x;
	curtransshape_ptr->pos.y = (state.opponentstate.car_posWorld1.ly >> 6) - var_vec4.y;
	curtransshape_ptr->pos.z = (state.opponentstate.car_posWorld1.lz >> 6) - var_vec4.z;
/*
    mov     [bp+var_trkobject_ptr], (offset trkObjectList.ss_trkObjInfoPtr+2Ah) ; 20C2h
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.opponentstate.car_posWorld1.lx
    mov     dx, word ptr state.opponentstate.car_posWorld1.lx+2
    mov     cl, 6
loc_1BB5F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB5F
    sub     ax, [bp+var_vec4.vx]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], ax
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.opponentstate.car_posWorld1.ly
    mov     dx, word ptr state.opponentstate.car_posWorld1.ly+2
    mov     cl, 6
loc_1BB79:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB79
    sub     ax, [bp+var_vec4.vy]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax
    mov     bx, curtransshape_ptr
    mov     ax, word ptr state.opponentstate.car_posWorld1.lz
    mov     dx, word ptr state.opponentstate.car_posWorld1.lz+2
    mov     cl, 6
loc_1BB94:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB94
    sub     ax, [bp+var_vec4.vz]
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], ax*/
	if (var_FC != 0)
		goto loc_1BBB0;
	if (timertestflag2 <= 2)
		goto loc_1BBC0;
/*    cmp     [bp+var_FC], 0
    jnz     short loc_1BBB0
    cmp     timertestflag2, 2
    jbe     short loc_1BBC0*/
loc_1BBB0:
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_loShapePtr;
	goto loc_1BBF6;
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_loShapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    jmp     short loc_1BBF6*/
loc_1BBC0:
	curtransshape_ptr->shapeptr = var_trkobject_ptr->ss_shapePtr;
	sub_204AE(&game3dshapes[0x0AEA / sizeof(struct SHAPE3D)].shape3d_verts[8], state.opponentstate.car_steeringAngle, &state.opponentstate.car_rc2, word_4448A, oppcarshapevecs, &oppcarshapevec);
/*    mov     bx, [bp+var_trkobject_ptr]
    mov     ax, [bx+TRACKOBJECT.ss_shapePtr]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], ax
    mov     ax, offset oppcarshapevec
    push    ax
    mov     ax, offset oppcarshapevecs
    push    ax
    mov     ax, offset word_4448A
    push    ax
    mov     ax, offset state.opponentstate.car_rc2
    push    ax
    push    state.opponentstate.car_steeringAngle
    mov     ax, word ptr game3dshapes.shape3d_verts+0AEAh
    mov     dx, word ptr game3dshapes.shape3d_verts+0AECh
    add     ax, 30h ; '0'
    push    dx
    push    ax
    call    sub_204AE
    add     sp, 0Eh*/
loc_1BBF6:
	if (timertestflag_copy == 0)
		goto loc_1BC10;
	curtransshape_ptr->rectptr = &rect_unk15;
/*    cmp     timertestflag_copy, 0
    jz      short loc_1BC10
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk15*/
loc_1BC06:
	curtransshape_ptr->ts_flags = 0xC;
	goto loc_1BC3E;
/*    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], 0Ch
    jmp     short loc_1BC3E*/
loc_1BC10:
	if (state.opponentstate.car_crashBmpFlag != 1)
		goto loc_1BC36;
	var_rect2 = cliprect_unk;
	curtransshape_ptr->rectptr = &var_rect2;
	goto loc_1BC06;
/*    cmp     state.opponentstate.car_crashBmpFlag, 1
    jnz     short loc_1BC36
    push    si
    push    di
    lea     di, [bp+var_rect2]
    mov     si, offset cliprect_unk
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, curtransshape_ptr
    lea     ax, [bp+var_rect2]
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], ax
    jmp     short loc_1BC06
    ; align 2
    db 144*/
loc_1BC36:
	curtransshape_ptr->ts_flags = 4;
    //mov     bx, curtransshape_ptr
    //mov     [bx+TRANSFORMEDSHAPE.ts_flags], 4
loc_1BC3E:
	curtransshape_ptr->rotvec.x = -state.opponentstate.car_rotate.z;
	curtransshape_ptr->rotvec.y = -state.opponentstate.car_rotate.y;
	curtransshape_ptr->rotvec.z = -state.opponentstate.car_rotate.x;
	curtransshape_ptr->unk = 0x12C;
	curtransshape_ptr->material = gameconfig.game_opponentmaterial;
	transformed_shape_add_for_sort(var_4A & var_12A, 3);
/*    mov     bx, curtransshape_ptr
    mov     ax, state.opponentstate.car_rotate.vz
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], ax
    mov     bx, curtransshape_ptr
    mov     ax, state.opponentstate.car_rotate.vy
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], ax
    mov     bx, curtransshape_ptr
    mov     ax, state.opponentstate.car_rotate.vx
    neg     ax
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 12Ch
    mov     bx, curtransshape_ptr
    mov     al, gameconfig.game_opponentmaterial
    mov     [bx+TRANSFORMEDSHAPE.ts_material], al
    mov     ax, 3
    push    ax
    mov     ax, [bp+var_A4]
    and     ax, [bp+var_12A]
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4*/
loc_1BC89:
	if (state.game_inputmode == 0)
		goto loc_1BC93;
	goto loc_1BEAA;
/*    cmp     state.game_inputmode, 0
    jz      short loc_1BC93
    jmp     loc_1BEAA*/
loc_1BC93:
	if (var_pos2lookup == startcol2)
		goto loc_1BCA5;
	if (var_pos2lookupadjust == startcol2)
		goto loc_1BCA5;
	goto loc_1BEAA;
/*    mov     al, startcol2
    cmp     [bp+var_pos2lookup], al
    jz      short loc_1BCA5
    cmp     [bp+var_pos2lookupadjust], al
    jz      short loc_1BCA5
    jmp     loc_1BEAA*/
loc_1BCA5:
	if (var_poslookup == startrow2)
		goto loc_1BCB7;
	if (var_poslookupadjust == startrow2)
		goto loc_1BCB7;
	goto loc_1BEAA;
/*    mov     al, startrow2
    cmp     [bp+var_poslookup], al
    jz      short loc_1BCB7
    cmp     [bp+var_poslookupadjust], al
    jz      short loc_1BCB7
    jmp     loc_1BEAA*/
	
	//; these 100 lines can go in a separate function
loc_1BCB7:

	var_multitileflag = multiply_and_scale(cos_fast(word_44DCA), 0x24);
	var_counter = multiply_and_scale(sin_fast(word_44DCA), 0x24) + 0x38;
/*  mov     ax, 24h ; '$'
    push    ax
    push    word_44DCA
    call    cos_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     [bp+var_multitileflag], ax
    mov     ax, 24h ; '$'
    push    ax
    push    word_44DCA
    call    sin_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    add     ax, 38h ; '8'
    mov     [bp+var_counter], ax*/
	var_108 = &game3dshapes[0x98A / sizeof(struct SHAPE3D)].shape3d_verts[8];
	var_108[0].x = var_multitileflag - 0x24;
	var_108[1].x = var_multitileflag - 0x24;
	var_108[2].x = 0x24 - var_multitileflag;
	var_108[3].x = 0x24 - var_multitileflag;

	var_108[0].z = var_counter;
	var_108[1].z = var_counter;
	var_108[2].z = var_counter;
	var_108[3].z = var_counter;

/*  mov     ax, word ptr game3dshapes.shape3d_verts+98Ah
    mov     dx, word ptr game3dshapes.shape3d_verts+98Ch
    add     ax, 30h ; '0'
    mov     word ptr [bp+var_108], ax
    mov     word ptr [bp+var_108+2], dx
    les     bx, [bp+var_108]
    mov     ax, [bp+var_multitileflag]
    sub     ax, 24h ; '$'
    mov     es:[bx], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_multitileflag]
    sub     ax, 24h ; '$'
    mov     es:[bx+6], ax
    les     bx, [bp+var_108]
    mov     ax, 24h ; '$'
    sub     ax, [bp+var_multitileflag]
    mov     es:[bx+0Ch], ax
    les     bx, [bp+var_108]
    mov     ax, 24h ; '$'
    sub     ax, [bp+var_multitileflag]
    mov     es:[bx+12h], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_counter]
    mov     es:[bx+4], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_counter]
    mov     es:[bx+0Ah], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_counter]
    mov     es:[bx+10h], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_counter]
    mov     es:[bx+16h], ax*/
	 
	curtransshape_ptr->pos.x =
		multiply_and_scale(sin_fast(track_angle + 0x100), 0x24) +
		multiply_and_scale(sin_fast(track_angle + 0x200), 0x1B6) + 
		trackcenterpos2[startcol2] - var_vec4.x;
	
    /*mov     ax, 24h ; '$'
    push    ax
    mov     ax, track_angle
    add     ah, 1
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     word ptr [bp+var_lastpos2lookup], ax

    mov     cx, 1B6h
    push    cx
    mov     cx, track_angle
    add     ch, 2
    push    cx
    call    sin_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     cx, ax
    mov     al, startcol2
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, trackcenterpos2[bx]
    add     cx, word ptr [bp+var_lastpos2lookup]
    sub     cx, [bp+var_vec4.vx]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vx], cx*/
	
	curtransshape_ptr->pos.y = hillHeightConsts[hillFlag] - var_vec4.y;

/*    mov     al, hillFlag
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, hillHeightConsts[bx]
    sub     ax, [bp+var_vec4.vy]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vy], ax*/
	curtransshape_ptr->pos.z =
		multiply_and_scale(cos_fast(track_angle + 0x100), 0x24) +
		multiply_and_scale(cos_fast(track_angle + 0x200), 0x1B6) + 
		trackcenterpos[startrow2] - var_vec4.z;
/*
    mov     ax, 24h ; '$'
    push    ax
    mov     ax, track_angle
    add     ah, 1
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     cx, 1B6h
    push    cx
    mov     cx, track_angle
    add     ch, 2
    push    cx
    mov     word ptr [bp+var_lastpos2lookup], ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     cx, ax
    mov     al, startrow2
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, trackcenterpos[bx]
    add     cx, word ptr [bp+var_lastpos2lookup]
    sub     cx, [bp+var_vec4.vz]
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_pos.vz], cx*/

	curtransshape_ptr->shapeptr = &game3dshapes[0x98A / sizeof(struct SHAPE3D)];
	curtransshape_ptr->rectptr = &rect_unk6;
	curtransshape_ptr->ts_flags = var_122 | 4;
	curtransshape_ptr->rotvec.x = 0;
	curtransshape_ptr->rotvec.y = 0;
	curtransshape_ptr->rotvec.z = track_angle;
	curtransshape_ptr->unk = 0x400;
	var_multitileflag = word_44DCA >> 6;
	if (var_multitileflag <= 3)
		goto loc_1BE8D;
	var_multitileflag = 3;

    /*mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_shapeptr], (offset game3dshapes.shape3d_numverts+98Ah)
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rectptr], offset rect_unk6
    mov     bx, curtransshape_ptr
    mov     al, [bp+var_122]
    or      al, 4
    mov     [bx+TRANSFORMEDSHAPE.ts_flags], al
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vx], 0
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vy], 0
    mov     bx, curtransshape_ptr
    mov     ax, track_angle
    mov     [bx+TRANSFORMEDSHAPE.ts_rotvec.vz], ax
    mov     bx, curtransshape_ptr
    mov     [bx+TRANSFORMEDSHAPE.ts_unk], 400h
    mov     ax, word_44DCA
    mov     cl, 6
    sar     ax, cl
    mov     [bp+var_multitileflag], ax
    cmp     ax, 3
    jle     short loc_1BE8D
    mov     [bp+var_multitileflag], 3*/
loc_1BE8D:
	curtransshape_ptr->material = var_multitileflag;	
	transformed_shape_add_for_sort(var_12A & -0x800 /*0xF800*/, 0);
/*    mov     bx, curtransshape_ptr
    mov     al, byte ptr [bp+var_multitileflag]
    mov     [bx+TRANSFORMEDSHAPE.ts_material], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_12A]
smart
    and     ax, 0F800h
nosmart
    push    ax
    call transformed_shape_add_for_sort
    add     sp, 4

*/

loc_1BEAA:
	if (transformedshape_counter != 0)
		goto loc_1BEB4;
	goto loc_1BF6C;
/*    cmp     transformedshape_counter, 0
    jnz     short loc_1BEB4
    jmp     loc_1BF6C*/
loc_1BEB4:
	if (transformedshape_counter <= 1)
		goto loc_1BED0;
	heapsort_by_order(transformedshape_counter, transformedshape_zarray, transformedshape_indices);
/*    cmp     transformedshape_counter, 1
    jle     short loc_1BED0
    mov     ax, offset transformedshape_indices
    push    ax
    mov     ax, offset transformedshape_zarray
    push    ax
    mov     al, transformedshape_counter
    cbw
    push    ax
    call    heapsort_by_order
    add     sp, 6*/
loc_1BED0:
	var_multitileflag = 0;
	goto loc_1BF38;
//    mov     [bp+var_multitileflag], 0
//    jmp     short loc_1BF38
loc_1BED8:
	if (state.playerstate.car_is_braking != 0)
		goto loc_1BF09;
//    cmp     state.playerstate.car_is_braking, 0
//    jnz     short loc_1BF09
loc_1BEDF:
	backlights_paint_override = 0x2E;
    //mov     backlights_paint_override, 2Eh ; '.'
loc_1BEE4:
	var_transformresult = transformed_shape_op(&currenttransshape[di]); // DI??
	if (var_transformresult <= 0)
		goto loc_1BF10;
	goto loc_1B03C;
/*    mov     ax, 14h // sizeof(transformedshape)
    imul    di
    add     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_transformresult], ax
    or      ax, ax
    jle     short loc_1BF10
    jmp     loc_1B03C

    ; align 2
    db 144*/
loc_1BF02:
	if (state.opponentstate.car_is_braking == 0)
		goto loc_1BEDF;
    //cmp     state.opponentstate.car_is_braking, 0
    //jz      short loc_1BEDF
loc_1BF09:
	backlights_paint_override = 0x2F;
	goto loc_1BEE4;
    //mov     backlights_paint_override, 2Fh ; '/'
    //jmp     short loc_1BEE4
loc_1BF10:
	if (var_transformresult != 0)
		goto loc_1BF34;
	if (transformedshape_arg2array[di] == 2)
		goto loc_1BF28;
	if (transformedshape_arg2array[di] == 3)
		goto loc_1BF5E;
	goto loc_1BF34;
/*    cmp     [bp+var_transformresult], 0
    jnz     short loc_1BF34
    mov     al, transformedshape_arg2array[di]
    cbw
    cmp     ax, 2
    jz      short loc_1BF28
    cmp     ax, 3
    jz      short loc_1BF5E
    jmp     short loc_1BF34*/
loc_1BF28:
	if (state.playerstate.car_crashBmpFlag != 1)
		goto loc_1BF34;
	var_DC[0] = 1;
/*    cmp     state.playerstate.car_crashBmpFlag, 1
    jnz     short loc_1BF34
    mov     [bp+var_DC], 1*/
loc_1BF34:
	var_multitileflag++;
    //inc     [bp+var_multitileflag]
loc_1BF38:
	if (var_multitileflag >= transformedshape_counter)
		goto loc_1BF6C;
	// di is used for index into currenttransshape elsewhere
	di = transformedshape_indices[var_multitileflag];
	if (transformedshape_arg2array[di] == 2)
		goto loc_1BED8;
	if (transformedshape_arg2array[di] == 3)
		goto loc_1BF02;
	goto loc_1BEE4;
/*    mov     al, transformedshape_counter
    cbw
    cmp     [bp+var_multitileflag], ax
    jge     short loc_1BF6C
    mov     bx, [bp+var_multitileflag]
    shl     bx, 1
    mov     di, transformedshape_indices[bx]
    mov     al, transformedshape_arg2array[di]
    cbw
    cmp     ax, 2
    jz      short loc_1BED8
    cmp     ax, 3
    jz      short loc_1BF02
    jmp     short loc_1BEE4
    ; align 2
    db 144*/
loc_1BF5E:
	if (state.opponentstate.car_crashBmpFlag != 1)
		goto loc_1BF34;
	var_DC[1] = 1;
	goto loc_1BF34;
/*    cmp     state.opponentstate.car_crashBmpFlag, 1
    jnz     short loc_1BF34
    mov     [bp+var_DB], 1
    jmp     short loc_1BF34
; ---- looks like a natural separation
*/

loc_1BF6C:
	si++;
loc_1BF6D:
	if (si < 0x17)
		goto loc_1BF75;
	goto loc_1B03C;
/*    cmp     si, 17h
    jl      short loc_1BF75
    jmp     loc_1B03C*/
	
loc_1BF75:
	if (var_32[si] != 0)
		goto loc_1BF6C;
	var_pos2lookup = var_pos2tab[si];
	var_poslookup = var_postab[si];
	elem_map_value = var_14C[si];
	terr_map_value = var_1A[si];
	var_FC = var_BC[si];
	var_12A = 0;
	if (elem_map_value != 0)
		goto loc_1BFB0;
	goto loc_1AD4A;
/*    cmp     [bp+si+var_32], 0
    jnz     short loc_1BF6C
    mov     al, [bp+si+var_pos2tab]
    mov     [bp+var_pos2lookup], al
    mov     al, [bp+si+var_postab]
    mov     [bp+var_poslookup], al
    mov     al, [bp+si+var_14C]
    mov     [bp+elem_map_value], al
    mov     al, [bp+si+var_1A]
    mov     [bp+terr_map_value], al
    mov     al, [bp+si+var_BC]
    mov     [bp+var_FC], al
    mov     [bp+var_12A], 0
    cmp     [bp+elem_map_value], 0
    jnz     short loc_1BFB0
    jmp     loc_1AD4A*/
loc_1BFB0:
	var_trkobject_ptr = &trkObjectList[elem_map_value];
	if (var_trkobject_ptr->ss_multiTileFlag != 0)
		goto loc_1BFD5;
	goto loc_1AD18;
	
/*    mov     al, [bp+elem_map_value]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset trkObjectList
    mov     [bp+var_trkobject_ptr], ax
    mov     bx, ax
    mov     al, [bx+TRACKOBJECT.ss_multiTileFlag]
    cbw
    or      ax, ax
    jnz     short loc_1BFD5
    jmp     loc_1AD18*/
loc_1BFD5:
	if (var_trkobject_ptr->ss_multiTileFlag != 1)
		goto loc_1BFDD;
	goto loc_1AD26;
/*    cmp     ax, 1
    jnz     short loc_1BFDD
    jmp     loc_1AD26*/
loc_1BFDD:
	if (var_trkobject_ptr->ss_multiTileFlag != 2)
		goto loc_1BFE5;
	goto loc_1AD34;
/*    cmp     ax, 2
    jnz     short loc_1BFE5
    jmp     loc_1AD34*/
loc_1BFE5:
	if (var_trkobject_ptr->ss_multiTileFlag != 3)
		goto loc_1BFED;
	goto loc_1AD3C;
/*    cmp     ax, 3
    jnz     short loc_1BFED
    jmp     loc_1AD3C*/
loc_1BFED:
	goto loc_1AD55;
	

loc_1BFF0:
	var_rectptr = &rect_unk15;
	goto loc_1C009;
/*    mov     [bp+var_rectptr], offset rect_unk15
    jmp     short loc_1C009
    ; align 2
    db 144*/
loc_1BFF8:
	if (si != 0)
		goto loc_1C002;
	var_rectptr = &var_rect;
	goto loc_1C006;
/*    or      si, si
    jnz     short loc_1C002
    lea     ax, [bp+var_rect]
    jmp     short loc_1C006
    ; align 2
    db 144*/
loc_1C002:
	var_rectptr = &var_rect2;
    //lea     ax, [bp+var_rect2]
loc_1C006:
    //mov     [bp+var_rectptr], ax
loc_1C009:
	if (rect_intersect(var_rectptr, arg_cliprectptr) == 0)
		goto loc_1C01E;
	goto loc_1C0C1;
/*    push    [bp+arg_cliprectptr]
    push    [bp+var_rectptr]
    call    rect_intersect
    add     sp, 4
    or      al, al
    jz      short loc_1C01E
    jmp     loc_1C0C1*/
loc_1C01E:
	sprite_set_1_size(var_rectptr->left, var_rectptr->right, var_rectptr->top, var_rectptr->bottom);
	var_vec6.x = (var_rectptr->right + var_rectptr->left) >> 1;
	var_vec6.y = (var_rectptr->top + var_rectptr->bottom) >> 1;
	var_multitileflag = var_rectptr->right - var_rectptr->left;
	var_counter = var_rectptr->bottom - var_rectptr->top;
	if (var_counter <= var_multitileflag)
		goto loc_1C070;
	var_multitileflag = var_counter;
/*    mov     bx, [bp+var_rectptr]
    push    [bx+RECTANGLE.rc_bottom]
    push    [bx+RECTANGLE.rc_top]
    push    [bx+RECTANGLE.rc_right]
    push    [bx+RECTANGLE.rc_left]
    call    sprite_set_1_size
    add     sp, 8
    mov     bx, [bp+var_rectptr]
    mov     ax, [bx+RECTANGLE.rc_right]
    add     ax, [bx+RECTANGLE.rc_left]
    sar     ax, 1
    mov     [bp+var_vec6.vx], ax
    mov     ax, [bx+RECTANGLE.rc_top]
    add     ax, [bx+RECTANGLE.rc_bottom]
    sar     ax, 1
    mov     [bp+var_vec6.vy], ax
    mov     ax, [bx+RECTANGLE.rc_right]
    sub     ax, [bx+RECTANGLE.rc_left]
    mov     [bp+var_multitileflag], ax
    mov     ax, [bx+RECTANGLE.rc_bottom]
    sub     ax, [bx+RECTANGLE.rc_top]
    mov     [bp+var_counter], ax
    mov     ax, [bp+var_multitileflag]
    cmp     [bp+var_counter], ax
    jle     short loc_1C070
    mov     ax, [bp+var_counter]
    mov     [bp+var_multitileflag], ax*/
loc_1C070:
	di = (state.game_frame >> 2) % 3 ;
	var_counter = ((long)var_multitileflag << 8) / (long)sdgame2_widths[di];
	shape_op_explosion(var_counter, sdgame2shapes[di], var_vec6.x, var_vec6.y);
/*    mov     ax, state.game_frame
    shr     ax, 1
    shr     ax, 1
    sub     dx, dx
    mov     cx, 3
    div     cx
    mov     di, dx
    mov     bx, di
    shl     bx, 1
    mov     ax, sdgame2_widths[bx]; lookup width of explotion shape
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_multitileflag]
    cwd
    mov     dh, dl
    mov     dl, ah
    mov     ah, al
    sub     al, al
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_counter], ax
    push    [bp+var_vec6.vy]
    push    [bp+var_vec6.vx]
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    push    word ptr (sdgame2shapes+2)[bx]
    push    word ptr sdgame2shapes[bx]; loops up one of the three explotion anim shapes
    push    ax
    call    shape_op_explosion
    add     sp, 0Ah*/
loc_1C0C1:
	si++;
    //inc     si
loc_1C0C2:
	if (si >= 2)
		goto loc_1C0E8;
	if (var_DC[si] == 0)
		goto loc_1C0C1;
	if (timertestflag_copy != 0)
		goto loc_1C0D8;
	goto loc_1BFF8;
/*    cmp     si, 2
    jge     short loc_1C0E8
    cmp     [bp+si+var_DC], 0
    jz      short loc_1C0C1
    cmp     timertestflag_copy, 0
    jnz     short loc_1C0D8
    jmp     loc_1BFF8*/
loc_1C0D8:
	if (si == 0)
		goto loc_1C0DF;
	goto loc_1BFF0;
/*    or      si, si
    jz      short loc_1C0DF
    jmp     loc_1BFF0*/
loc_1C0DF:
	var_rectptr = &rect_unk12;
	goto loc_1C009;
/*    mov     [bp+var_rectptr], offset rect_unk12
    jmp     loc_1C009
    ; align 2
    db 144
; --------------------------------------------------------
*/

loc_1C0E8:
	sprite_set_1_size(0, 0x140, arg_cliprectptr->top, arg_cliprectptr->bottom);
	if (cameramode == 0)
		goto loc_1C10A;
	goto loc_1C1C6;
/*    mov     bx, [bp+arg_cliprectptr]
    push    [bx+RECTANGLE.rc_bottom]
    push    [bx+RECTANGLE.rc_top]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    sprite_set_1_size
    add     sp, 8
    cmp     cameramode, 0
    jz      short loc_1C10A
    jmp     loc_1C1C6*/
loc_1C10A:
	if (followOpponentFlag == 0)
		goto loc_1C11E;
	var_stateptr = &state.opponentstate;
	si = state.game_oEndFrame;
	goto loc_1C128;
/*    cmp     followOpponentFlag, 0
    jz      short loc_1C11E
    mov     [bp+var_stateptr], offset state.opponentstate
    mov     si, state.game_oEndFrame
    jmp     short loc_1C128
    ; align 2
    db 144*/
loc_1C11E:
	var_stateptr = &state.playerstate;
	si = state.game_pEndFrame;
/*    mov     [bp+var_stateptr], offset state.playerstate
    mov     si, state.game_pEndFrame*/
loc_1C128:
	if (var_stateptr->car_crashBmpFlag != 1)
		goto loc_1C17C;
	if (timertestflag_copy == 0)
		goto loc_1C162;
	rect_union(init_crak(state.game_frame - si, arg_cliprectptr->top, arg_cliprectptr->bottom - arg_cliprectptr->top), rect_unk, rect_unk);
/*    mov     bx, [bp+var_stateptr]
    cmp     [bx+CARSTATE.car_crashBmpFlag], 1
    jnz     short loc_1C17C
    cmp     timertestflag_copy, 0
    jz      short loc_1C162
    mov     ax, offset rect_unk
    push    ax
    push    ax
    mov     bx, [bp+arg_cliprectptr]
    mov     ax, [bx+RECTANGLE.rc_bottom]
    sub     ax, [bx+RECTANGLE.rc_top]
    push    ax
    push    [bx+RECTANGLE.rc_top]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    call init_crak*/
loc_1C156:
	goto loc_1C1C3;
/*    add     sp, 6
    push    ax
    call    rect_union
    jmp     short loc_1C1C3
    ; align 2
    db 144*/
loc_1C162:
	init_crak(state.game_frame - si, arg_cliprectptr->top, arg_cliprectptr->bottom - arg_cliprectptr->top);
	goto loc_1C1C3;
/*    mov     bx, [bp+arg_cliprectptr]
    mov     ax, [bx+RECTANGLE.rc_bottom]
    sub     ax, [bx+RECTANGLE.rc_top]
    push    ax
    push    [bx+RECTANGLE.rc_top]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    call init_crak
    jmp     short loc_1C1C3
    ; align 2
    db 144*/
loc_1C17C:
	if (var_stateptr->car_crashBmpFlag != 2)
		goto loc_1C1C6;
	if (timertestflag_copy == 0)
		goto loc_1C1AC;
	rect_union(do_sinking(state.game_frame - si, arg_cliprectptr->top, arg_cliprectptr->bottom - arg_cliprectptr->top), rect_unk, rect_unk);
	goto loc_1C156;
/*    mov     bx, [bp+var_stateptr]
    cmp     [bx+CARSTATE.car_crashBmpFlag], 2
    jnz     short loc_1C1C6
    cmp     timertestflag_copy, 0
    jz      short loc_1C1AC
    mov     ax, offset rect_unk
    push    ax
    push    ax
    mov     bx, [bp+arg_cliprectptr]
    mov     ax, [bx+RECTANGLE.rc_bottom]
    sub     ax, [bx+RECTANGLE.rc_top]
    push    ax
    push    [bx+RECTANGLE.rc_top]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    call do_sinking
    jmp     short loc_1C156*/
loc_1C1AC:
	do_sinking(state.game_frame - si, arg_cliprectptr->top, arg_cliprectptr->bottom - arg_cliprectptr->top);
/*    mov     bx, [bp+arg_cliprectptr]
    mov     ax, [bx+RECTANGLE.rc_bottom]
    sub     ax, [bx+RECTANGLE.rc_top]
    push    ax
    push    [bx+RECTANGLE.rc_top]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    call do_sinking*/
loc_1C1C3:
    //add     sp, 6
loc_1C1C6:
	if (game_replay_mode == 0)
		goto loc_1C1D0;
	goto loc_1C25B;
/*    cmp     game_replay_mode, 0
    jz      short loc_1C1D0
    jmp     loc_1C25B*/
loc_1C1D0:
	if (state.game_inputmode != 0)
		goto loc_1C1DA;
	goto loc_1C25B;
/*    cmp     state.game_inputmode, 0
    jnz     short loc_1C1DA
    jmp     loc_1C25B*/
loc_1C1DA:
	format_frame_as_string(&resID_byte1, elapsed_time1 + elapsed_time2, 0);
	font_set_fontdef2(fontledresptr);
	if (timertestflag_copy == 0)
		goto loc_1C238;
	rect_union(intro_draw_text(&resID_byte1, 0x8C, roofbmpheight + 2, dialog_fnt_colour, 0), &rect_unk11, &rect_unk11);
	goto loc_1C256;
/*    sub     ax, ax
    push    ax              ; int
    mov     ax, elapsed_time1
    add     ax, elapsed_time2
    push    ax
    mov     ax, offset resID_byte1
    push    ax              ; char *
    call    format_frame_as_string; prints elapsed time (unless in pause/replay, or some unknown state)
    add     sp, 6
    push    word ptr fontledresptr+2
    push    word ptr fontledresptr
    call    font_set_fontdef2
    add     sp, 4
    cmp     timertestflag_copy, 0
    jz      short loc_1C238
    mov     ax, offset rect_unk11
    push    ax
    push    ax
    sub     ax, ax
    push    ax
    push    dialog_fnt_colour
    mov     ax, roofbmpheight
    add     ax, 2
    push    ax
    mov     ax, 8Ch ; 'Œ'
    push    ax
    mov     ax, offset resID_byte1
    push    ax
    call    intro_draw_text
    add     sp, 0Ah
    push    ax
    call    rect_union
    add     sp, 6
    jmp     short loc_1C256
    ; align 4
    db 144
    db 144*/
loc_1C238:
	intro_draw_text(&resID_byte1, 0x8C, roofbmpheight + 2, dialog_fnt_colour, 0);
/*    sub     ax, ax
    push    ax
    push    dialog_fnt_colour
    mov     ax, roofbmpheight
    add     ax, 2
    push    ax
    mov     ax, 8Ch ; 'Œ'
    push    ax
    mov     ax, offset resID_byte1
    push    ax
    call    intro_draw_text
    add     sp, 0Ah*/
loc_1C256:
	font_set_fontdef();
    //call    font_set_fontdef
loc_1C25B:
	if (timertestflag_copy != 0)
		goto loc_1C265;
	goto loc_1C2F8;
/*    cmp     timertestflag_copy, 0
    jnz     short loc_1C265
    jmp     loc_1C2F8*/
loc_1C265:
	rect_union(draw_ingame_text(), rect_unk, rect_unk);
	if (var_132 == 0)
		goto loc_1C2AE;
	rect_unk[0] = *arg_cliprectptr;
	si = 1;
/*    mov     ax, offset rect_unk
    push    ax
    push    ax
    call draw_ingame_text
    push    ax
    call    rect_union
    add     sp, 6
    cmp     [bp+var_132], 0
    jz      short loc_1C2AE
    mov     ax, [bp+arg_cliprectptr]
    push    si
    push    di
    mov     di, offset rect_unk
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     si, 1*/
loc_1C293:
	rect_unk[si] = cliprect_unk;
	si++;
	if (si < 0xF)
		goto loc_1C293;
/*    mov     bx, si
    mov     cl, 3
    shl     bx, cl
    push    si
    push    di
    lea     di, rect_unk.rc_left[bx]
    mov     si, offset cliprect_unk
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     si
    cmp     si, 0Fh
    jl      short loc_1C293*/
loc_1C2AE:
	si = 0;
    //sub     si, si
loc_1C2B0:
	rectptr_unk[si] = rect_unk[si];
	si++;
	if (si < 0xF)
		goto loc_1C2B0;
	word_449FC[arg_0] = var_angY;
	word_463D6 = var_angY;
	return;
/*    mov     ax, si
    mov     cl, 3
    shl     ax, cl
    mov     word ptr [bp+var_lastpos2lookup], ax
    mov     bx, ax
    lea     ax, rect_unk.rc_left[bx]
    mov     bx, word ptr [bp+var_lastpos2lookup]
    add     bx, rectptr_unk
    push    si
    push    di
    mov     di, bx
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     si
    cmp     si, 0Fh
    jl      short loc_1C2B0
    mov     al, [bp+arg_0]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bp+var_angY]
    mov     word_449FC[bx], ax
    mov     ax, [bp+var_angY]
    mov     word_463D6, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144*/
loc_1C2F8:
	draw_ingame_text();
	return;
/*    call draw_ingame_text
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
update_frame endp*/
}
