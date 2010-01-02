.model medium
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
    include seg001.inc
    include seg002.inc
    include seg004.inc
    include seg005.inc
    include seg006.inc
    include seg007.inc
    include seg008.inc
    include seg009.inc
    include seg010.inc
    include seg011.inc
    include seg012.inc
    include seg013.inc
    include seg014.inc
    include seg015.inc
    include seg016.inc
    include seg017.inc
    include seg018.inc
    include seg019.inc
    include seg020.inc
    include seg021.inc
    include seg022.inc
    include seg023.inc
    include seg024.inc
    include seg025.inc
    include seg026.inc
    include seg027.inc
    include seg028.inc
    include seg029.inc
    include seg030.inc
    include seg031.inc
    include seg032.inc
    include seg033.inc
    include seg034.inc
    include seg035.inc
    include seg036.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg003 segment byte public 'STUNTSC' use16
    assume cs:seg003
    assume es:nothing, ss:nothing, ds:dseg
    public sub_19F14
    public sub_1A096
    public update_frame
    public sub_1C302
    public skybox_op
    public sub_1CB80
    public sub_1CBDC
    public draw_ingame_text
    public sub_1D534
    public sub_1D5B4
    public load_skybox
    public unload_skybox
    public load_sdgame2_shapes
    public sub_1D92A
    public setup_intro
    public sub_1DEA6
    pop     ds
    retf
sub_19F14 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    cmp     byte_46436, 0
    jz      short loc_19F26
    jmp     loc_1A090
loc_19F26:
    call    sprite_copy_2_to_1_2
    cmp     byte_454A4, 0
    jz      short loc_19F35
    jmp     loc_1A046
loc_19F35:
    cmp     word_44984, 0
    jnz     short loc_19F3F
    jmp     loc_1A030
loc_19F3F:
    sub     si, si
loc_19F41:
    mov     byte_45520[si], 3
    inc     si
    cmp     si, 0Fh
    jl      short loc_19F41
    cmp     byte_3B8FA, 4
    jnz     short loc_19F59
    mov     ax, word_463D6
    mov     word_449FE, ax
loc_19F59:
    mov     ax, word_463D6
    cmp     word_449FE, ax
    jnz     short loc_19F8B
    mov     ax, word_45982
    cmp     word_45574, ax
    jnz     short loc_19F8B
    mov     ax, word_45984
    cmp     word_45576, ax
    jnz     short loc_19F8B
    mov     ax, word_45986
    cmp     word_45578, ax
    jnz     short loc_19F8B
    mov     ax, word_45988
    cmp     word_4557A, ax
    jnz     short loc_19F8B
    mov     byte_45525, 0
loc_19F8B:
    mov     byte_46166, 0
    mov     ax, offset unk_44DCE
    push    ax
    mov     ax, offset byte_46166
    push    ax
    push    [bp+arg_0]
    mov     ax, offset rect_array_unk2
    push    ax
    mov     ax, offset rect_array_unk
    push    ax
    mov     ax, offset byte_45520
    push    ax
    mov     ax, 0Fh
    push    ax
    call    sub_26A52
    add     sp, 0Eh
    cmp     byte_46166, 0
    jz      short loc_1A01E
    mov     ax, offset word_455D4
    push    ax
    mov     ax, offset unk_44DCE
    push    ax
    mov     al, byte_46166
    cbw
    push    ax
    call    sub_26B4A
    add     sp, 6
    call    sub_28DB6
    sub     si, si
    jmp     short loc_1A013
loc_19FD8:
    mov     bx, si
    shl     bx, 1
    mov     ax, word_455D4[bx]
    mov     cl, 3
    shl     ax, cl
    add     ax, 965Eh
    mov     [bp+var_2], ax
    mov     bx, ax
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    inc     si
loc_1A013:
    mov     al, byte_46166
    cbw
    cmp     ax, si
    jg      short loc_19FD8
    jmp     short loc_1A05E
    ; align 2
    db 144
loc_1A01E:
    mov     bx, [bp+arg_0]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    jmp     short loc_1A03E
loc_1A030:
    mov     bx, [bp+arg_0]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
loc_1A03E:
    call    set_sprite1_size
    add     sp, 8
loc_1A046:
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
loc_1A05E:
    call    sub_28D9E
    cmp     word_44984, 0
    jz      short loc_1A090
    mov     ax, word_463D6
    mov     word_449FE, ax
    sub     si, si
loc_1A072:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    push    si
    push    di
    lea     si, rect_array_unk[di]
    lea     di, rect_array_unk2[di]
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
    jl      short loc_1A072
loc_1A090:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_19F14 endp
sub_1A096 proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    cmp     word_44984, 0
    jz      short loc_1A0EE
    push    si
    mov     di, offset rect_array_unk
    mov     si, offset rect_unk5
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     si
    push    si
    mov     di, offset rect_array_unk2
    mov     si, offset rect_unk5
    movsw
    movsw
    movsw
    movsw
    pop     si
    mov     si, 1
loc_1A0C2:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    push    si
    push    di
    lea     di, rect_array_unk[di]
    mov     si, offset rect_unk4
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    push    si
    push    di
    lea     di, rect_array_unk2[di]
    mov     si, offset rect_unk4
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     si
    cmp     si, 0Fh
    jl      short loc_1A0C2
loc_1A0EE:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1A096 endp
update_frame proc far
    var_154 = dword ptr -340
    var_150 = word ptr -336
    var_14E = word ptr -334
    var_14C = byte ptr -332
    var_134 = byte ptr -308
    var_132 = word ptr -306
    var_130 = byte ptr -304
    var_12E = word ptr -302
    var_12C = byte ptr -300
    var_12A = word ptr -298
    var_128 = word ptr -296
    var_126 = word ptr -294
    var_124 = word ptr -292
    var_122 = byte ptr -290
    var_120 = byte ptr -288
    var_10E = word ptr -270
    var_10C = byte ptr -268
    var_10A = byte ptr -266
    var_108 = dword ptr -264
    var_104 = word ptr -260
    var_102 = byte ptr -258
    var_100 = byte ptr -256
    var_FE = word ptr -254
    var_FC = byte ptr -252
    var_FA = word ptr -250
    var_F6 = byte ptr -246
    var_F4 = word ptr -244
    var_F2 = word ptr -242
    var_F0 = word ptr -240
    var_EE = word ptr -238
    var_EC = word ptr -236
    var_EA = word ptr -234
    var_E8 = word ptr -232
    var_E4 = byte ptr -228
    var_E2 = byte ptr -226
    var_E0 = word ptr -224
    var_DE = word ptr -222
    var_DC = byte ptr -220
    var_DB = byte ptr -219
    var_DA = word ptr -218
    var_D8 = byte ptr -216
    var_D6 = word ptr -214
    var_D4 = byte ptr -212
    var_BC = byte ptr -188
    var_A4 = word ptr -164
    var_A2 = word ptr -162
    var_A0 = byte ptr -160
    var_8E = byte ptr -142
    var_86 = byte ptr -134
    var_6E = byte ptr -110
    var_6C = word ptr -108
    var_6A = byte ptr -106
    var_62 = byte ptr -98
    var_60 = byte ptr -96
    var_5E = word ptr -94
    var_5C = word ptr -92
    var_5A = word ptr -90
    var_58 = word ptr -88
    var_56 = word ptr -86
    var_54 = word ptr -84
    var_52 = word ptr -82
    var_50 = word ptr -80
    var_4E = byte ptr -78
    var_4C = byte ptr -76
    var_4A = byte ptr -74
    var_48 = word ptr -72
    var_46 = word ptr -70
    var_44 = word ptr -68
    var_42 = word ptr -66
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3C = byte ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_36 = word ptr -54
    var_34 = word ptr -52
    var_32 = byte ptr -50
    var_1A = byte ptr -26
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 154h
    push    di
    push    si
    mov     [bp+var_DC], 0
    mov     [bp+var_DB], 0
    cmp     byte_46436, 0
    jz      short loc_1A122
    cmp     [bp+arg_0], 0
    jz      short loc_1A122
    mov     word_3B808, offset rect_array_unk
    mov     word_3B80A, offset rect_array_unk2
    jmp     short loc_1A12E
loc_1A122:
    mov     word_3B80A, offset rect_array_unk
    mov     word_3B808, offset rect_array_unk2
loc_1A12E:
    cmp     word_44984, 0
    jz      short loc_1A162
    mov     [bp+var_122], 8
    mov     [bp+var_3E], offset rect_unk
    sub     si, si
    jmp     short loc_1A145
    ; align 2
    db 144
loc_1A144:
    inc     si
loc_1A145:
    cmp     si, 0Fh
    jge     short loc_1A167
    mov     ax, [bp+var_3E]
    add     [bp+var_3E], 8
    push    si
    push    di
    mov     di, ax
    mov     si, offset rect_unk4
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    jmp     short loc_1A144
loc_1A162:
    mov     [bp+var_122], 0
loc_1A167:
    cmp     byte_463E0, 0
    jnz     short loc_1A1BE
    mov     ax, word ptr state.playerstate.car_longvec1.long_x
    mov     dx, word ptr state.playerstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1A177:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A177
    mov     [bp+var_EC], ax
    mov     ax, word ptr state.playerstate.car_longvec1.long_y
    mov     dx, word ptr state.playerstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1A18C:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A18C
    mov     [bp+var_EA], ax
    mov     ax, word ptr state.playerstate.car_longvec1.long_z
    mov     dx, word ptr state.playerstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1A1A1:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1A1
    mov     [bp+var_E8], ax
    mov     ax, state.playerstate.car_rotate.y
    mov     [bp+var_46], ax
    mov     ax, state.playerstate.car_rotate.z
    mov     [bp+var_5A], ax
    mov     ax, state.playerstate.car_rotate.x
    jmp     short loc_1A20C
loc_1A1BE:
    mov     ax, word ptr state.opponentstate.car_longvec1.long_x
    mov     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1A1C7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1C7
    mov     [bp+var_EC], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_y
    mov     dx, word ptr state.opponentstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1A1DC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1DC
    mov     [bp+var_EA], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_z
    mov     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1A1F1:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1A1F1
    mov     [bp+var_E8], ax
    mov     ax, state.opponentstate.car_rotate.y
    mov     [bp+var_46], ax
    mov     ax, state.opponentstate.car_rotate.z
    mov     [bp+var_5A], ax
    mov     ax, state.opponentstate.car_rotate.x
loc_1A20C:
    mov     [bp+var_F4], ax
    mov     [bp+var_5C], 0FFFFh
    mov     [bp+var_E0], 0
    mov     al, byte_3B8F5
    cbw
    or      ax, ax
    jnz     short loc_1A226
    jmp     loc_1A434
loc_1A226:
    cmp     ax, 1
    jnz     short loc_1A22E
    jmp     loc_1A40A
loc_1A22E:
    cmp     ax, 2
    jz      short loc_1A23E
    cmp     ax, 3
    jnz     short loc_1A23B
    jmp     loc_1A48E
loc_1A23B:
    jmp     loc_1A303
loc_1A23E:
    mov     [bp+var_F2], 0
    mov     [bp+var_F0], 0
    mov     [bp+var_EE], 4000h
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_F4]
    neg     ax
    push    ax
    mov     ax, [bp+var_46]
    neg     ax
    push    ax
    mov     ax, [bp+var_5A]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_D6], ax
    lea     ax, [bp+var_128]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_124]
    push    [bp+var_128]
    call    sub_2EA4E
    add     sp, 4
    mov     si, ax
    mov     [bp+var_F2], 0
    mov     [bp+var_F0], 0
    mov     ax, word_3B8EC
    mov     [bp+var_EE], ax
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
    mov     [bp+var_D6], ax
loc_1A2CC:
    lea     ax, [bp+var_128]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_EC]
    add     ax, [bp+var_128]
    mov     [bp+var_58], ax
    mov     ax, [bp+var_EA]
    add     ax, [bp+var_126]
    mov     [bp+var_56], ax
    mov     ax, [bp+var_E8]
    add     ax, [bp+var_124]
loc_1A300:
    mov     [bp+var_54], ax
loc_1A303:
    cmp     [bp+var_5C], 0FFFFh
    jz      short loc_1A30C
    jmp     loc_1A3E9
loc_1A30C:
    lea     ax, [bp+var_58]
    push    ax
    lea     ax, [bp+var_58]
    push    ax
    call    sub_1E1A0
    add     sp, 4
    mov     ax, word_449EC
    cmp     [bp+var_56], ax
    jge     short loc_1A327
    mov     [bp+var_56], ax
loc_1A327:
    cmp     byte_4392C, 0
    jz      short loc_1A393
    push    [bp+var_54]
    push    [bp+var_56]
    push    [bp+var_58]
    push    word_44DC6
    call    plane_origin_op
    add     sp, 8
    mov     si, ax
    cmp     si, 0Ch
    jge     short loc_1A393
    mov     vec_unk2.x, 0
    mov     ax, 0Ch
    sub     ax, si
    mov     vec_unk2.y, ax
    mov     vec_unk2.z, 0
    mov     ax, word_44DC6
    mov     word_44F4A, ax
    mov     word_44F48, 0
    mov     word_44F44, 0
    mov     word_44F42, 0
    mov     word_44F46, 0
    call    plane_rotate_op
    mov     ax, vec_planerotopresult.x
    add     [bp+var_58], ax
    mov     ax, vec_planerotopresult.y
    add     [bp+var_56], ax
    mov     ax, vec_planerotopresult.z
    add     [bp+var_54], ax
loc_1A393:
    mov     ax, [bp+var_E8]
    sub     ax, [bp+var_54]
    push    ax
    mov     ax, [bp+var_EC]
    sub     ax, [bp+var_58]
    push    ax
    call    sub_2EA4E
    add     sp, 4
    neg     ax
smart
    and     ah, 3
nosmart
    mov     [bp+var_5C], ax
    mov     ax, [bp+var_E8]
    sub     ax, [bp+var_54]
    push    ax
    mov     ax, [bp+var_EC]
    sub     ax, [bp+var_58]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_38], ax
    push    ax
    mov     ax, [bp+var_EA]
    sub     ax, [bp+var_56]
    add     ax, 32h ; '2'
    push    ax
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     [bp+var_FA], ax
loc_1A3E9:
    cmp     [bp+var_E0], 1
    jg      short loc_1A3F3
    jmp     loc_1A4D0
loc_1A3F3:
    cmp     [bp+var_E0], 3FFh
    jl      short loc_1A3FE
    jmp     loc_1A4D0
loc_1A3FE:
    mov     ax, [bp+var_E0]
    mov     [bp+var_FE], ax
    jmp     loc_1A4D6
    ; align 2
    db 144
loc_1A40A:
    mov     al, byte_463E0
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     ax, state.game_vec1.x[bx]
    mov     [bp+var_58], ax
    mov     ax, state.game_vec1.z[bx]
    mov     [bp+var_54], ax
    mov     ax, state.game_vec1.y[bx]
    mov     [bp+var_56], ax
    jmp     loc_1A303
loc_1A434:
    mov     ax, [bp+var_F4]
smart
    and     ah, 3
nosmart
    mov     [bp+var_5C], ax
    mov     ax, [bp+var_46]
smart
    and     ah, 3
nosmart
    mov     [bp+var_FA], ax
    mov     ax, [bp+var_5A]
smart
    and     ah, 3
nosmart
    mov     [bp+var_E0], ax
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_F4]
    neg     ax
    push    ax
    mov     ax, [bp+var_46]
    neg     ax
    push    ax
    mov     ax, [bp+var_5A]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_D6], ax
    mov     [bp+var_F2], 0
    mov     [bp+var_EE], 0
    mov     ax, simd_copy.car_height
    sub     ax, 6
    mov     [bp+var_F0], ax
    jmp     loc_1A2CC
    ; align 2
    db 144
loc_1A48E:
    mov     al, byte_463E0
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
    mov     word ptr [bp+var_154], ax
    mov     word ptr [bp+var_154+2], dx
    les     bx, [bp+var_154]
    mov     ax, es:[bx]
    mov     [bp+var_58], ax
    mov     ax, es:[bx+2]
    add     ax, word_44D20
    add     ax, 5Ah ; 'Z'
    mov     [bp+var_56], ax
    mov     ax, es:[bx+4]
    jmp     loc_1A300
loc_1A4D0:
    mov     [bp+var_FE], 0
loc_1A4D6:
    cmp     state.game_frame, 0
    jnz     short loc_1A4E4
    mov     bx, word_46468
    jmp     short loc_1A4E8
    ; align 2
    db 144
loc_1A4E4:
    mov     bx, state.game_frame
loc_1A4E8:
smart
    and     bx, 0Fh
nosmart
    mov     al, byte_3C0C6[bx]
    mov     [bp+var_E4], al
    sub     ax, ax
    push    ax
    push    [bp+arg_2]
    push    [bp+var_5C]
    push    [bp+var_FA]
    push    [bp+var_FE]
    call    select_rect_rotate
    add     sp, 0Ah
    mov     [bp+var_52], ax
smart
    and     ah, 3
nosmart
    mov     cl, 7
    shr     ax, cl
    mov     si, ax
    mov     bx, si
    shl     bx, 1
    mov     ax, off_3C084[bx]
    mov     [bp+var_50], ax
    mov     ax, 1
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_FA]
    push    [bp+var_FE]
    call    mat_rot_zxy
    add     sp, 8
    push    si
    push    di
    lea     di, [bp+var_120]
    mov     si, ax
    push    ss
    pop     es
    mov     cx, 9
    repne movsw
    pop     di
    pop     si
    mov     [bp+var_F2], 0
    mov     [bp+var_F0], 0
    mov     [bp+var_EE], 3E8h
    lea     ax, [bp+var_44]
    push    ax
    lea     ax, [bp+var_120]
    push    ax
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_40], 0
    jle     short loc_1A580
    mov     [bp+var_2], 1
    jmp     short loc_1A585
loc_1A580:
    mov     [bp+var_2], 0FFFFh
loc_1A585:
    cmp     byte_3B8FA, 0
    jz      short loc_1A58F
    jmp     loc_1A66D
loc_1A58F:
    mov     word_42A42, 92CCh
    mov     al, [bp+var_122]
    or      al, 7
    mov     byte_42A4C, al
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     word_42A4A, 400h
    mov     byte_42A4D, 0
    mov     [bp+var_36], 0
loc_1A5BA:
    mov     bx, [bp+var_36]
    shl     bx, 1
    mov     si, word_3BE34[bx]
    add     si, [bp+var_5C]
    add     si, word_42D28
smart
    and     si, 3FFh
nosmart
    cmp     si, 87h ; '‡'
    jl      short loc_1A5DD
    cmp     si, 379h
    jg      short loc_1A5DD
    jmp     loc_1A661
loc_1A5DD:
    push    si
    lea     ax, [bp+var_A0]
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     [bp+var_F2], 0
    mov     ax, 0AE6h
    sub     ax, [bp+var_56]
    mov     [bp+var_F0], ax
    mov     [bp+var_EE], 3A98h; 15000
    lea     ax, [bp+var_128]
    push    ax
    lea     ax, [bp+var_A0]
    push    ax
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     [bp+var_124], 3A98h; 15000
    mov     ax, offset currenttransshape
    push    ax
    lea     ax, [bp+var_120]
    push    ax
    lea     ax, [bp+var_128]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     word_42A3E, 0C8h ; 'È'
    jle     short loc_1A661
    mov     bx, [bp+var_36]
    shl     bx, 1
    mov     ax, off_3BE44[bx]
    mov     word_42A40, ax
    mov     ax, [bp+var_5C]
    neg     ax
    mov     word_42A48, ax
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
loc_1A661:
    inc     [bp+var_36]
    cmp     [bp+var_36], 8
    jge     short loc_1A66D
    jmp     loc_1A5BA
loc_1A66D:
    mov     ax, [bp+var_58]
    mov     cl, 0Ah
    sar     ax, cl
    mov     [bp+var_10A], al
    mov     ax, [bp+var_54]
    sar     ax, cl
    sub     al, 1Dh
    neg     al
    mov     [bp+var_134], al
    cmp     byte_3B8FA, 0
    jz      short loc_1A69D
    mov     al, byte ptr state.playerstate.car_longvec1.long_x+2
    mov     [bp+var_D8], al
    mov     al, 1Dh
    sub     al, byte ptr state.playerstate.car_longvec1.long_z+2
    mov     [bp+var_E2], al
loc_1A69D:
    sub     si, si
loc_1A69F:
    mov     [bp+si+var_32], 0
    inc     si
    cmp     si, 17h
    jl      short loc_1A69F
    mov     bl, byte_3B8FA
    sub     bh, bh
    mov     al, byte_3C09C[bx]
    mov     [bp+var_130], al
    mov     si, 16h
    jmp     short loc_1A6FF
loc_1A6BC:
    mov     ax, [bp+var_50]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    mov     al, [bx]
    add     al, [bp+var_10A]
    mov     [bp+var_F6], al
    mov     al, [bx+1]
    add     al, [bp+var_134]
    mov     [bp+var_102], al
    cmp     [bp+var_F6], 0
    jl      short loc_1A6FA
    cmp     [bp+var_F6], 1Dh
    jg      short loc_1A6FA
    or      al, al
    jl      short loc_1A6FA
    cmp     al, 1Dh
    jle     short loc_1A724
loc_1A6FA:
    mov     [bp+si+var_32], 2
loc_1A6FE:
    dec     si
loc_1A6FF:
    or      si, si
    jge     short loc_1A706
    jmp     loc_1A9C8
loc_1A706:
    cmp     [bp+si+var_32], 0
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
loc_1A724:
    mov     al, [bp+var_F6]
    cbw
    mov     word ptr [bp+var_154], ax
    mov     al, [bp+var_102]
    cbw
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, word ptr [bp+var_154]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_62], al
    mov     bx, [bp+var_150]
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_154]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_10C], al
    cmp     [bp+var_62], 0
    jnz     short loc_1A774
    jmp     loc_1A7FF
loc_1A774:
    cmp     al, 7
    jb      short loc_1A797
    cmp     al, 0Bh
    jnb     short loc_1A797
    mov     al, [bp+var_62]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_10C]
    push    ax
    call    map_terrain_track
    add     sp, 4
    mov     [bp+var_62], al
    mov     [bp+var_10C], 0
loc_1A797:
    mov     al, [bp+var_62]
    sub     ah, ah
    cmp     ax, 0FDh ; 'ý'
    jz      short loc_1A7B0
    cmp     ax, 0FEh ; 'þ'
    jz      short loc_1A7B4
    cmp     ax, 0FFh
    jnz     short loc_1A7AE
    jmp     loc_1A8CA
loc_1A7AE:
    jmp     short loc_1A7FF
loc_1A7B0:
    dec     [bp+var_F6]
loc_1A7B4:
    dec     [bp+var_102]
loc_1A7B8:
    mov     al, [bp+var_F6]
    cbw
    mov     word ptr [bp+var_154], ax
    mov     al, [bp+var_102]
    cbw
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, word ptr [bp+var_154]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_62], al
    mov     bx, [bp+var_150]
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_154]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_10C], al
loc_1A7FF:
    mov     al, [bp+var_10C]
    mov     [bp+si+var_1A], al
    mov     bx, [bp+var_50]
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bx+2]
    mov     [bp+si+var_BC], al
    cmp     [bp+var_62], 0
    jz      short loc_1A857
    cmp     byte_3B8FA, 0
    jz      short loc_1A857
    mov     al, [bp+var_62]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    cmp     sceneshapes.scene_unk4[bx], 40h ; '@'
    jl      short loc_1A857
    mov     al, [bp+var_D8]
    cmp     [bp+var_F6], al
    jnz     short loc_1A853
    mov     al, [bp+var_E2]
    cmp     [bp+var_102], al
    jz      short loc_1A857
loc_1A853:
    mov     [bp+var_62], 0
loc_1A857:
    mov     al, [bp+var_F6]
    mov     [bp+si+var_86], al
    mov     al, [bp+var_102]
    mov     [bp+si+var_D4], al
    mov     al, [bp+var_62]
    mov     [bp+si+var_14C], al
    cmp     [bp+var_62], 0
    jnz     short loc_1A877
    jmp     loc_1A6FE
loc_1A877:
    mov     al, [bp+var_62]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, sceneshapes.scene_translateflag[bx]
    cbw
    mov     [bp+var_DE], ax
    or      ax, ax
    jnz     short loc_1A898
    jmp     loc_1A6FE
loc_1A898:
    mov     al, [bp+var_F6]
    sub     al, [bp+var_10A]
    mov     [bp+var_100], al
    mov     al, [bp+var_102]
    sub     al, [bp+var_134]
    mov     [bp+var_12C], al
    mov     ax, [bp+var_DE]
    cmp     ax, 1
    jz      short loc_1A8D2
    cmp     ax, 2
    jz      short loc_1A914
    cmp     ax, 3
    jnz     short loc_1A8C6
    jmp     loc_1A960
loc_1A8C6:
    jmp     loc_1A6FE
    ; align 2
    db 144
loc_1A8CA:
    dec     [bp+var_F6]
    jmp     loc_1A7B8
    ; align 2
    db 144
loc_1A8D2:
    sub     di, di
    jmp     short loc_1A8D7
loc_1A8D6:
    inc     di
loc_1A8D7:
    cmp     di, si
    jl      short loc_1A8DE
    jmp     loc_1A6FE
loc_1A8DE:
    mov     ax, [bp+var_50]
    mov     cx, di
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    mov     al, [bp+var_100]
    cmp     [bx], al
    jnz     short loc_1A8D6
    mov     al, [bp+var_12C]
    cmp     [bx+1], al
    jz      short loc_1A90E
    cbw
    inc     ax
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    cmp     ax, cx
    jnz     short loc_1A8D6
loc_1A90E:
    mov     [bp+di+var_32], 1
    jmp     short loc_1A8D6
loc_1A914:
    sub     di, di
    jmp     short loc_1A919
loc_1A918:
    inc     di
loc_1A919:
    cmp     di, si
    jl      short loc_1A920
    jmp     loc_1A6FE
loc_1A920:
    mov     ax, [bp+var_50]
    mov     cx, di
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    mov     al, [bp+var_12C]
    cmp     [bx+1], al
    jnz     short loc_1A918
    mov     al, [bx]
    mov     byte ptr [bp+var_150], al
    mov     al, [bp+var_100]
    cmp     byte ptr [bp+var_150], al
    jz      short loc_1A959
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_150]
    cbw
    cmp     cx, ax
    jnz     short loc_1A918
loc_1A959:
    mov     [bp+di+var_32], 1
    jmp     short loc_1A918
    ; align 2
    db 144
loc_1A960:
    sub     di, di
    jmp     short loc_1A965
loc_1A964:
    inc     di
loc_1A965:
    cmp     di, si
    jl      short loc_1A96C
    jmp     loc_1A6FE
loc_1A96C:
    mov     bx, [bp+var_50]
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bx]
    mov     byte ptr [bp+var_154], al
    mov     al, [bp+var_100]
    cmp     byte ptr [bp+var_154], al
    jz      short loc_1A996
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_154]
    cbw
    cmp     cx, ax
    jnz     short loc_1A964
loc_1A996:
    mov     bx, [bp+var_50]
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     bx, ax
    mov     al, [bx+1]
    mov     byte ptr [bp+var_154], al
    mov     al, [bp+var_12C]
    cmp     byte ptr [bp+var_154], al
    jz      short loc_1A9C1
    cbw
    inc     ax
    mov     cx, ax
    mov     al, byte ptr [bp+var_154]
    cbw
    cmp     cx, ax
    jnz     short loc_1A964
loc_1A9C1:
    mov     [bp+di+var_32], 1
    jmp     short loc_1A964
    ; align 2
    db 144
loc_1A9C8:
    mov     [bp+var_3C], 0FFh
    mov     [bp+var_6C], 0
    cmp     byte_3B8F5, 0
    jnz     short loc_1A9E2
    cmp     byte_463E0, 0
    jnz     short loc_1A9E2
    jmp     loc_1AB65
loc_1A9E2:
    cmp     state.playerstate.field_C9, 2
    jnz     short loc_1A9EC
    jmp     loc_1AB65
loc_1A9EC:
    sub     ax, ax
    push    ax
    mov     ax, state.playerstate.car_rotate.x
    neg     ax
    push    ax
    mov     ax, state.playerstate.car_rotate.y
    neg     ax
    push    ax
    mov     ax, state.playerstate.car_rotate.z
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_D6], ax
    mov     [bp+var_DE], 0FFFFh
    mov     di, 0FFFFh
    mov     [bp+var_A2], 0
    jmp     short loc_1AA8C
loc_1AA1E:
    dec     si
loc_1AA1F:
    cmp     [bp+var_DE], si
    jge     short loc_1AA88
    cmp     [bp+si+var_32], 2
    jz      short loc_1AA1E
    mov     ax, [bp+var_50]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_154], ax
    mov     al, [bp+var_10A]
    cbw
    mov     bx, word ptr [bp+var_154]
    mov     cx, ax
    mov     al, [bx]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_F6]
    cbw
    cmp     cx, ax
    jnz     short loc_1AA1E
    mov     al, [bp+var_134]
    cbw
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_102]
    cbw
    cmp     cx, ax
    jnz     short loc_1AA1E
    mov     al, [bp+var_F6]
    mov     [bp+var_3C], al
    mov     al, [bp+var_102]
    mov     [bp+var_60], al
    mov     [bp+var_DE], si
    mov     di, [bp+var_A2]
    jmp     short loc_1AA1E
    ; align 2
    db 144
loc_1AA88:
    inc     [bp+var_A2]
loc_1AA8C:
    cmp     [bp+var_A2], 4
    jge     short loc_1AAF4
    mov     bx, [bp+var_A2]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    push    si
    push    di
    lea     di, [bp+var_F2]
    lea     si, simd_copy.wheel_coords.x[bx]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    lea     ax, [bp+var_44]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_44]
    cwd
    add     ax, word ptr state.playerstate.car_longvec1.long_x
    adc     dx, word ptr state.playerstate.car_longvec1.long_x+2
    mov     ax, dx
    mov     [bp+var_F6], al
    mov     ax, [bp+var_40]
    cwd
    add     ax, word ptr state.playerstate.car_longvec1.long_z
    adc     dx, word ptr state.playerstate.car_longvec1.long_z+2
    mov     ax, dx
    sub     al, 1Dh
    neg     al
    mov     [bp+var_102], al
    mov     si, 16h
    jmp     loc_1AA1F
    ; align 2
    db 144
loc_1AAF4:
    cmp     di, 0FFFFh
    jz      short loc_1AB65
    cmp     state.playerstate.car_surfacegrip_idx1, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfacegrip_idx2, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfacegrip_idx3, 4
    jnz     short loc_1AB15
    cmp     state.playerstate.car_surfacegrip_idx4, 4
    jz      short loc_1AB65
loc_1AB15:
    mov     [bp+var_F2], 0
    mov     [bp+var_EE], 0
    mov     [bp+var_F0], 7530h
    lea     ax, [bp+var_44]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_F2]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    lea     ax, [bp+var_44]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_EE], 0
    jg      short loc_1AB60
    mov     [bp+var_6C], 0F800h
    jmp     short loc_1AB65
    ; align 2
    db 144
loc_1AB60:
    mov     [bp+var_6C], 800h
loc_1AB65:
    mov     [bp+var_4A], 0FFh
    mov     [bp+var_A4], 0
    cmp     gameconfig.game_opponenttype, 0
    jnz     short loc_1AB79
    jmp     loc_1AD0E
loc_1AB79:
    cmp     byte_3B8F5, 0
    jnz     short loc_1AB8A
    cmp     byte_463E0, 0
    jz      short loc_1AB8A
    jmp     loc_1AD0E
loc_1AB8A:
    cmp     state.opponentstate.field_C9, 2
    jnz     short loc_1AB94
    jmp     loc_1AD0E
loc_1AB94:
    sub     ax, ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.x
    neg     ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.y
    neg     ax
    push    ax
    mov     ax, state.opponentstate.car_rotate.z
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_D6], ax
    mov     [bp+var_DE], 0FFFFh
    mov     di, 0FFFFh
    mov     [bp+var_A2], 0
    jmp     short loc_1AC34
loc_1ABC6:
    dec     si
loc_1ABC7:
    cmp     [bp+var_DE], si
    jge     short loc_1AC30
    cmp     [bp+si+var_32], 2
    jz      short loc_1ABC6
    mov     ax, [bp+var_50]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    add     ax, cx
    mov     word ptr [bp+var_154], ax
    mov     al, [bp+var_10A]
    cbw
    mov     bx, word ptr [bp+var_154]
    mov     cx, ax
    mov     al, [bx]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_F6]
    cbw
    cmp     cx, ax
    jnz     short loc_1ABC6
    mov     al, [bp+var_134]
    cbw
    mov     cx, ax
    mov     al, [bx+1]
    cbw
    add     ax, cx
    mov     cx, ax
    mov     al, [bp+var_102]
    cbw
    cmp     cx, ax
    jnz     short loc_1ABC6
    mov     al, [bp+var_F6]
    mov     [bp+var_4A], al
    mov     al, [bp+var_102]
    mov     [bp+var_6E], al
    mov     [bp+var_DE], si
    mov     di, [bp+var_A2]
    jmp     short loc_1ABC6
    ; align 2
    db 144
loc_1AC30:
    inc     [bp+var_A2]
loc_1AC34:
    cmp     [bp+var_A2], 4
    jge     short loc_1AC9C
    mov     bx, [bp+var_A2]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    push    si
    push    di
    lea     di, [bp+var_F2]
    lea     si, simd2.wheel_coords.x[bx]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    lea     ax, [bp+var_44]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_44]
    cwd
    add     ax, word ptr state.opponentstate.car_longvec1.long_x
    adc     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     ax, dx
    mov     [bp+var_F6], al
    mov     ax, [bp+var_40]
    cwd
    add     ax, word ptr state.opponentstate.car_longvec1.long_z
    adc     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     ax, dx
    sub     al, 1Dh
    neg     al
    mov     [bp+var_102], al
    mov     si, 16h
    jmp     loc_1ABC7
    ; align 2
    db 144
loc_1AC9C:
    cmp     di, 0FFFFh
    jz      short loc_1AD0E
    cmp     state.opponentstate.car_surfacegrip_idx1, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfacegrip_idx2, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfacegrip_idx3, 4
    jnz     short loc_1ACBD
    cmp     state.opponentstate.car_surfacegrip_idx4, 4
    jz      short loc_1AD0E
loc_1ACBD:
    mov     [bp+var_F2], 0
    mov     [bp+var_EE], 0
    mov     [bp+var_F0], 7530h
    lea     ax, [bp+var_44]
    push    ax
    push    [bp+var_D6]
    lea     ax, [bp+var_F2]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_F2]
    push    ax
    mov     ax, offset mat_temp
    push    ax
    lea     ax, [bp+var_44]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_EE], 0
    jg      short loc_1AD08
    mov     [bp+var_A4], 0F800h
    jmp     short loc_1AD0E
loc_1AD08:
    mov     [bp+var_A4], 800h
loc_1AD0E:
    mov     [bp+var_4E], 0
    sub     si, si
    jmp     loc_1BF6D
    ; align 2
    db 144
loc_1AD18:
    mov     [bp+var_36], 1
    mov     [bp+var_10E], 97Eh
    jmp     short loc_1AD55
    ; align 2
    db 144
loc_1AD26:
    mov     [bp+var_36], 2
    mov     [bp+var_10E], 980h
    jmp     short loc_1AD55
    ; align 2
    db 144
loc_1AD34:
    mov     [bp+var_36], 3
    jmp     short loc_1AD4F
    ; align 2
    db 144
loc_1AD3C:
    mov     [bp+var_36], 4
    mov     [bp+var_10E], 988h
    jmp     short loc_1AD55
    ; align 2
    db 144
loc_1AD4A:
    mov     [bp+var_36], 1
loc_1AD4F:
    mov     [bp+var_10E], 984h
loc_1AD55:
    mov     [bp+var_DE], 0
    jmp     loc_1AE7A
loc_1AD5E:
    mov     al, [bp+var_100]
    cbw
    or      ax, ax
    jz      short loc_1AD80
    cmp     ax, 1Dh
    jz      short loc_1ADCC
    mov     al, [bp+var_12C]
    cbw
    or      ax, ax
    jz      short loc_1ADEC
    cmp     ax, 1Dh
    jz      short loc_1ADF0
    mov     di, 0FFFFh
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1AD80:
    mov     al, [bp+var_12C]
    cbw
    or      ax, ax
    jz      short loc_1AD94
    cmp     ax, 1Dh
    jz      short loc_1ADC6
    mov     di, 6
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1AD94:
    mov     di, 7
loc_1AD97:
    cmp     di, 0FFFFh
    jnz     short loc_1AD9F
    jmp     loc_1AE76
loc_1AD9F:
    mov     al, byte_3C0E6[di]; indices to corner shapes?
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes
    mov     [bp+var_34], ax
    cmp     [bp+var_FC], 0
    jnz     short loc_1ADF6
    mov     bx, ax
    mov     ax, [bx+4]
    jmp     short loc_1ADFC
    ; align 2
    db 144
loc_1ADC6:
    mov     di, 5
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1ADCC:
    mov     al, [bp+var_12C]
    cbw
    or      ax, ax
    jz      short loc_1ADE0
    cmp     ax, 1Dh
    jz      short loc_1ADE6
    mov     di, 2
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1ADE0:
    mov     di, 1
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1ADE6:
    mov     di, 3
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1ADEC:
    sub     di, di
    jmp     short loc_1AD97
loc_1ADF0:
    mov     di, 4
    jmp     short loc_1AD97
    ; align 2
    db 144
loc_1ADF6:
    mov     bx, [bp+var_34]
    mov     ax, [bx+6]
loc_1ADFC:
    mov     word_42A40, ax
    mov     al, [bp+var_100]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_58]
    mov     currenttransshape, ax
    mov     ax, [bp+var_56]
    neg     ax
    mov     word_42A3C, ax
    mov     al, [bp+var_12C]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_54]
    mov     word_42A3E, ax
    mov     word_42A42, 929Ch
    mov     al, [bp+var_122]
    or      al, 5
    mov     byte_42A4C, al
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     bx, di
    shl     bx, 1
    mov     ax, word_3C0D6[bx]
    mov     word_42A48, ax
    mov     word_42A4A, 400h
    mov     byte_42A4D, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1AE76
    jmp     loc_1B03C
loc_1AE76:
    inc     [bp+var_DE]
loc_1AE7A:
    mov     ax, [bp+var_36]
    cmp     [bp+var_DE], ax
    jge     short loc_1AECC
    mov     ax, [bp+var_DE]
    shl     ax, 1
    add     ax, [bp+var_10E]
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    mov     al, [bx]
    add     al, [bp+var_F6]
    mov     [bp+var_100], al
    mov     al, [bx+1]
    add     al, [bp+var_102]
    mov     [bp+var_12C], al
    cmp     byte_3B8FA, 0
    jnz     short loc_1AEB2
    jmp     loc_1AD5E
loc_1AEB2:
    mov     al, [bp+var_D8]
    cmp     [bp+var_100], al
    jnz     short loc_1AE76
    mov     al, [bp+var_E2]
    cmp     [bp+var_12C], al
    jnz     short loc_1AEC9
    jmp     loc_1AD5E
loc_1AEC9:
    jmp     short loc_1AE76
    ; align 2
    db 144
loc_1AECC:
    cmp     [bp+var_10C], 6
    jnz     short loc_1AF50
    mov     ax, word_3B8EA
    mov     [bp+var_48], ax
    cmp     [bp+var_62], 0
    jz      short loc_1AEE4
loc_1AEDF:
    mov     [bp+var_10C], 0
loc_1AEE4:
    cmp     [bp+var_10C], 0
    jnz     short loc_1AEEE
    jmp     loc_1B11C
loc_1AEEE:
    mov     al, [bp+var_10C]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes2
    mov     [bp+var_12E], ax
    mov     bx, ax
    mov     ax, [bx+4]
    mov     word_42A40, ax
    mov     al, [bp+var_F6]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_58]
    mov     currenttransshape, ax
    mov     ax, [bp+var_48]
    sub     ax, [bp+var_56]
    mov     word_42A3C, ax
    mov     al, [bp+var_102]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_54]
    mov     word_42A3E, ax
    cmp     [bp+var_48], 0
    jz      short loc_1AF47
    jmp     loc_1B0D4
loc_1AF47:
    mov     word_42A42, offset rect_unk2
    jmp     loc_1B0DA
loc_1AF50:
    mov     [bp+var_48], 0
    mov     al, [bp+var_62]
    sub     ah, ah
    cmp     ax, 69h ; 'i'
    jb      short loc_1AEE4
    cmp     ax, 6Ch ; 'l'
    jbe     short loc_1AF67
    jmp     loc_1AEE4
loc_1AF67:
    mov     [bp+var_DE], 0
    jmp     loc_1B0AC
loc_1AF70:
    mov     al, [bp+var_F6]
loc_1AF74:
    mov     [bp+var_100], al
    mov     al, [bp+var_102]
loc_1AF7C:
    mov     [bp+var_12C], al
loc_1AF80:
    mov     al, [bp+var_100]
    cbw
    mov     word ptr [bp+var_154], ax
    mov     al, [bp+var_12C]
    cbw
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     bx, terrainrows[bx]
    add     bx, word ptr [bp+var_154]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_10C], al
    or      al, al
    jnz     short loc_1AFB4
    jmp     loc_1B0A8
loc_1AFB4:
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes2
    mov     [bp+var_12E], ax
    mov     bx, ax
    mov     ax, [bx+4]
    mov     word_42A40, ax
    mov     bx, word ptr [bp+var_154]
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    sub     ax, [bp+var_58]
    mov     currenttransshape, ax
    mov     ax, [bp+var_56]
    neg     ax
    mov     word_42A3C, ax
    mov     bx, [bp+var_150]
    mov     ax, trackcenterpos[bx]
    sub     ax, [bp+var_54]
    mov     word_42A3E, ax
    mov     word_42A42, 929Ch
    mov     al, [bp+var_122]
    or      al, 5
    mov     byte_42A4C, al
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     bx, [bp+var_12E]
    mov     ax, [bx+2]
    mov     word_42A48, ax
    mov     word_42A4A, 400h
    mov     byte_42A4D, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1B0A8
loc_1B03C:
    push    [bp+var_56]
    push    [bp+var_5C]
    push    [bp+var_FE]
    lea     ax, [bp+var_120]
    push    ax
    push    [bp+var_2]
    push    [bp+arg_2]
    mov     al, [bp+arg_0]
    cbw
    push    ax
    push    cs
    call near ptr skybox_op
    add     sp, 0Eh
    mov     [bp+var_132], ax
    mov     bx, [bp+arg_2]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    call    sub_25FF6
    sub     si, si
    jmp     loc_1C0C2
    ; align 2
    db 144
loc_1B084:
    mov     al, [bp+var_F6]
    inc     al
    jmp     loc_1AF74
    ; align 2
    db 144
loc_1B08E:
    mov     al, [bp+var_F6]
loc_1B092:
    mov     [bp+var_100], al
    mov     al, [bp+var_102]
    inc     al
    jmp     loc_1AF7C
    ; align 2
    db 144
loc_1B0A0:
    mov     al, [bp+var_F6]
    inc     al
    jmp     short loc_1B092
loc_1B0A8:
    inc     [bp+var_DE]
loc_1B0AC:
    cmp     [bp+var_DE], 4
    jl      short loc_1B0B6
    jmp     loc_1AEDF
loc_1B0B6:
    mov     ax, [bp+var_DE]
    or      ax, ax
    jnz     short loc_1B0C1
    jmp     loc_1AF70
loc_1B0C1:
    cmp     ax, 1
    jz      short loc_1B084
    cmp     ax, 2
    jz      short loc_1B08E
    cmp     ax, 3
    jz      short loc_1B0A0
    jmp     loc_1AF80
    ; align 2
    db 144
loc_1B0D4:
    mov     word_42A42, offset rect_unk6
loc_1B0DA:
    mov     al, [bp+var_122]
    or      al, 5
    mov     byte_42A4C, al
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     bx, [bp+var_12E]
    mov     ax, [bx+2]
    mov     word_42A48, ax
    mov     word_42A4A, 400h
    mov     byte_42A4D, 0
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1B11C
    jmp     loc_1B03C
loc_1B11C:
    mov     byte_45D7E, 0
    mov     word_46434, offset currenttransshape
    cmp     [bp+var_62], 0
    jnz     short loc_1B130
    jmp     loc_1B71E
loc_1B130:
    mov     al, [bp+var_62]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes
    mov     [bp+var_12E], ax
    mov     bx, ax
    test    byte ptr [bx+0Bh], 1
    jz      short loc_1B168
    mov     al, [bp+var_102]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackpos[bx]
    mov     [bp+var_5E], ax
    mov     al, [bp+var_102]
    inc     al
    jmp     short loc_1B17C
loc_1B168:
    mov     al, [bp+var_102]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    mov     [bp+var_5E], ax
    mov     al, [bp+var_102]
loc_1B17C:
    mov     [bp+var_12C], al
    mov     bx, [bp+var_12E]
    test    byte ptr [bx+0Bh], 2
    jz      short loc_1B1A2
    mov     al, [bp+var_F6]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+7558h]
    mov     [bp+var_3A], ax
    mov     al, [bp+var_F6]
    inc     al
    jmp     short loc_1B1B6
loc_1B1A2:
    mov     al, [bp+var_F6]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    mov     [bp+var_3A], ax
    mov     al, [bp+var_F6]
loc_1B1B6:
    mov     [bp+var_100], al
    mov     ax, [bp+var_3A]
    sub     ax, [bp+var_58]
    mov     [bp+var_44], ax
    mov     ax, [bp+var_48]
    sub     ax, [bp+var_56]
    mov     [bp+var_42], ax
    mov     ax, [bp+var_5E]
    sub     ax, [bp+var_54]
    mov     [bp+var_40], ax
    cmp     [bp+var_48], 0
    jnz     short loc_1B1DE
    jmp     loc_1B2AE
loc_1B1DE:
    mov     bx, [bp+var_12E]
    mov     al, [bx+0Bh]
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
    db 144
loc_1B1FC:
    mov     di, 1
    mov     [bp+var_DA], 932h
loc_1B205:
    mov     [bp+var_DE], 0
    jmp     short loc_1B236
    ; align 2
    db 144
loc_1B20E:
    mov     di, 2
    mov     [bp+var_DA], 936h
    jmp     short loc_1B205
    ; align 2
    db 144
loc_1B21A:
    mov     di, 2
    mov     [bp+var_DA], 93Eh
    jmp     short loc_1B205
    ; align 2
    db 144
loc_1B226:
    mov     di, 4
    mov     [bp+var_DA], 946h
    jmp     short loc_1B205
    ; align 2
    db 144
loc_1B232:
    inc     [bp+var_DE]
loc_1B236:
    cmp     [bp+var_DE], di
    jge     short loc_1B2AE
    mov     bx, [bp+var_DA]
    add     [bp+var_DA], 2
    mov     ax, [bx]
    add     ax, [bp+var_44]
    mov     currenttransshape, ax
    mov     ax, [bp+var_42]
    mov     word_42A3C, ax
    mov     bx, [bp+var_DA]
    add     [bp+var_DA], 2
    mov     ax, [bx]
    add     ax, [bp+var_40]
    mov     word_42A3E, ax
    mov     word_42A40, 79FEh
    mov     word_42A42, 92A4h
    mov     al, [bp+var_122]
    or      al, 5
    mov     byte_42A4C, al
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     word_42A48, 0
    mov     word_42A4A, 800h
    mov     byte_42A4D, 0
    mov     ax, 72CAh
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1B232
    jmp     loc_1B03C
loc_1B2AE:
    mov     bx, [bp+var_12E]
    cmp     byte ptr [bx+8], 0
    jnz     short loc_1B2BB
    jmp     loc_1B374
loc_1B2BB:
    mov     al, [bx+8]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_34], ax
    cmp     [bp+var_FC], 0
    jz      short loc_1B2E0
    mov     bx, ax
    mov     ax, [bx+6]
    jmp     short loc_1B2E6
loc_1B2E0:
    mov     bx, [bp+var_34]
    mov     ax, [bx+4]
loc_1B2E6:
    mov     word_42A54, ax
    or      ax, ax
    jnz     short loc_1B2F0
    jmp     loc_1B374
loc_1B2F0:
    push    si
    push    di
    mov     di, 72DEh
    lea     si, [bp+var_44]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     word_42A58, 0
    mov     word_42A5A, 0
    mov     ax, [bx+2]
    mov     word_42A5C, ax
    cmp     byte ptr [bx+0Bh], 0
    jz      short loc_1B320
    mov     word_42A5E, 400h
    jmp     short loc_1B326
    ; align 2
    db 144
loc_1B320:
    mov     word_42A5E, 800h
loc_1B326:
    cmp     byte ptr [bx+9], 0
    jl      short loc_1B332
    mov     al, [bx+9]
    jmp     short loc_1B336
    ; align 2
    db 144
loc_1B332:
    mov     al, [bp+var_E4]
loc_1B336:
    mov     byte_42A61, al
    mov     al, [bx+0Ah]
    or      al, [bp+var_122]
    or      al, 4
    mov     byte_42A60, al
    test    byte_42A60, 1
    jz      short loc_1B36A
    mov     word_42A56, 929Ch
    mov     ax, 72DEh
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1B374
    jmp     loc_1B03C
loc_1B36A:
    mov     word_42A56, 92A4h
    mov     [bp+var_4E], 1
loc_1B374:
    cmp     [bp+var_FC], 0
    jz      short loc_1B384
    mov     bx, [bp+var_12E]
    mov     ax, [bx+6]
    jmp     short loc_1B38B
loc_1B384:
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
loc_1B38B:
    mov     word_42A40, ax
    push    si
    push    di
    mov     di, offset currenttransshape
    lea     si, [bp+var_44]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     word_42A44, 0
    mov     word_42A46, 0
    mov     ax, [bx+2]
    mov     word_42A48, ax
    cmp     byte ptr [bx+0Bh], 0
    jz      short loc_1B3BE
    mov     word_42A4A, 400h
    jmp     short loc_1B3C4
    ; align 2
    db 144
loc_1B3BE:
    mov     word_42A4A, 800h
loc_1B3C4:
    mov     al, [bx+0Ah]
    or      al, [bp+var_122]
    or      al, 4
    mov     byte_42A4C, al
    cmp     byte ptr [bx+9], 0
    jl      short loc_1B3DC
    mov     al, [bx+9]
    jmp     short loc_1B3E0
    ; align 2
    db 144
loc_1B3DC:
    mov     al, [bp+var_E4]
loc_1B3E0:
    mov     byte_42A4D, al
    test    byte ptr [bx+0Ah], 1
    jz      short loc_1B408
    mov     word_42A42, 929Ch
    mov     ax, offset currenttransshape
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1B46A
    jmp     loc_1B03C
    ; align 2
    db 144
loc_1B408:
    mov     word_42A42, 92A4h
    sub     ax, ax
    push    ax
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
    cmp     [bp+var_4E], 0
    jz      short loc_1B449
    mov     [bp+var_4E], 0
    sub     ax, ax
    push    ax
    mov     ax, 0F800h
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
    cmp     [bp+var_6C], 0
    jz      short loc_1B43C
    mov     [bp+var_6C], 0FC00h
loc_1B43C:
    cmp     [bp+var_A4], 0
    jz      short loc_1B449
    sub     [bp+var_A4], 400h
loc_1B449:
    mov     al, startcol2
    cmp     [bp+var_F6], al
    jnz     short loc_1B464
    mov     al, startrow2
    cmp     [bp+var_102], al
    jnz     short loc_1B464
    mov     [bp+var_12A], 0
    jmp     short loc_1B46A
    ; align 2
    db 144
loc_1B464:
    mov     [bp+var_12A], 0FFFFh
loc_1B46A:
    mov     al, [bp+var_102]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_F6]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata19
    mov     es, word ptr trackdata19+2
    mov     al, es:[bx]
    mov     [bp+var_4C], al
    cmp     al, 0FFh
    jnz     short loc_1B493
    jmp     loc_1B72E
loc_1B493:
    cbw
    mov     bx, ax
    cmp     state.field_3FA[bx], 0
    jnz     short loc_1B4A0
    jmp     loc_1B626
loc_1B4A0:
    cmp     state.field_42A, 0
    jnz     short loc_1B4AA
    jmp     loc_1B72E
loc_1B4AA:
    sub     di, di
loc_1B4AC:
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    cmp     word ptr state.field_38E[bx], 0
    jnz     short loc_1B4C0
    jmp     loc_1B61A
loc_1B4C0:
    mov     al, [bp+var_4C]
    cbw
    add     ax, 2
    mov     cl, state.field_443[di]
    sub     ch, ch
    cmp     ax, cx
    jz      short loc_1B4D4
    jmp     loc_1B61A
loc_1B4D4:
    mov     al, state.field_42B[di]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_12E], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     ax, word ptr state.game_longvecs1.long_x[bx]
    mov     dx, word ptr (state.game_longvecs1.long_x+2)[bx]
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
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    add     cx, es:[bx]
    sub     cx, [bp+var_58]
    mov     bx, word_46434
    mov     [bx], cx
    mov     bx, [bp+var_150]
    mov     ax, word ptr state.game_longvecs2.long_x[bx]
    mov     dx, word ptr (state.game_longvecs2.long_x+2)[bx]
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
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    add     cx, es:[bx+2]
    sub     cx, [bp+var_56]
    mov     bx, word_46434
    mov     [bx+2], cx
    mov     bx, [bp+var_150]
    mov     ax, [bx-721Ch]
    mov     dx, [bx-721Ah]
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
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    add     cx, es:[bx+4]
    sub     cx, [bp+var_54]
    mov     bx, word_46434
    mov     [bx+4], cx
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     bx, word_46434
    mov     word ptr [bx+8], 92A4h
    mov     bx, word_46434
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+12h], al
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6FDEh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ah], ax
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    mov     ax, [bx-6FAEh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ch], ax
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6F7Eh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 400h
    mov     bx, word_46434
    mov     byte ptr [bx+13h], 0
    sub     ax, ax
    push    ax
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1B61A:
    inc     di
    cmp     di, 18h
    jge     short loc_1B623
    jmp     loc_1B4AC
loc_1B623:
    jmp     loc_1B72E
loc_1B626:
    mov     al, [bp+var_4C]
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
    add     ax, 2C30h
    mov     [bp+var_12E], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     ax, es:[bx]
    sub     ax, [bp+var_58]
    mov     bx, word_46434
    mov     [bx], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     ax, es:[bx+2]
    sub     ax, [bp+var_56]
    mov     bx, word_46434
    mov     [bx+2], ax
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     ax, es:[bx+4]
    sub     ax, [bp+var_54]
    mov     bx, word_46434
    mov     [bx+4], ax
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     bx, word_46434
    mov     word ptr [bx+8], 92A4h
    mov     bx, word_46434
    mov     al, [bp+var_122]
    or      al, 4
    mov     [bx+12h], al
    mov     bx, word_46434
    mov     word ptr [bx+0Ah], 0
    mov     bx, word_46434
    mov     word ptr [bx+0Ch], 0
    mov     al, [bp+var_4C]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata8
    mov     es, word ptr trackdata8+2
    mov     ax, es:[bx]
    mov     bx, word_46434
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 64h ; 'd'
    mov     bx, word_46434
    mov     byte ptr [bx+13h], 0
    sub     ax, ax
    push    ax
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
    jmp     short loc_1B72E
loc_1B71E:
    mov     al, [bp+var_F6]
    mov     [bp+var_100], al
    mov     al, [bp+var_102]
    mov     [bp+var_12C], al
loc_1B72E:
    mov     al, [bp+var_F6]
    cmp     [bp+var_3C], al
    jz      short loc_1B743
    mov     al, [bp+var_100]
    cmp     [bp+var_3C], al
    jz      short loc_1B743
    jmp     loc_1B9DA
loc_1B743:
    mov     al, [bp+var_102]
    cmp     [bp+var_60], al
    jz      short loc_1B758
    mov     al, [bp+var_12C]
    cmp     [bp+var_60], al
    jz      short loc_1B758
    jmp     loc_1B9DA
loc_1B758:
    cmp     state.field_42A, 0
    jnz     short loc_1B762
    jmp     loc_1B89F
loc_1B762:
    sub     di, di
loc_1B764:
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    cmp     word ptr [bx-6F4Eh], 0
    jnz     short loc_1B778
    jmp     loc_1B896
loc_1B778:
    cmp     byte ptr [di-6E99h], 0
    jz      short loc_1B782
    jmp     loc_1B896
loc_1B782:
    mov     al, [di-6EB1h]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_12E], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     ax, [bx-72DCh]
    mov     dx, [bx-72DAh]
    add     ax, word ptr state.playerstate.car_longvec1.long_x
    adc     dx, word ptr state.playerstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1B7B9:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B7B9
    sub     ax, [bp+var_58]
    mov     bx, word_46434
    mov     [bx], ax
    mov     bx, [bp+var_150]
    mov     ax, [bx-727Ch]
    mov     dx, [bx-727Ah]
    add     ax, word ptr state.playerstate.car_longvec1.long_y
    adc     dx, word ptr state.playerstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1B7E0:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B7E0
    sub     ax, [bp+var_56]
    mov     bx, word_46434
    mov     [bx+2], ax
    mov     bx, [bp+var_150]
    mov     ax, [bx-721Ch]
    mov     dx, [bx-721Ah]
    add     ax, word ptr state.playerstate.car_longvec1.long_z
    adc     dx, word ptr state.playerstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1B808:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B808
    sub     ax, [bp+var_54]
    mov     bx, word_46434
    mov     [bx+4], ax
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     bx, word_46434
    mov     word ptr [bx+8], 92A4h
    mov     bx, word_46434
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+12h], al
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6FDEh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ah], ax
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6FAEh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ch], ax
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6F7Eh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 400h
    mov     bx, word_46434
    mov     al, gameconfig.game_playermaterial
    mov     [bx+13h], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_6C]
    and     ax, [bp+var_12A]
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1B896:
    inc     di
    cmp     di, 18h
    jge     short loc_1B89F
    jmp     loc_1B764
loc_1B89F:
    mov     [bp+var_12E], 20B4h
    mov     bx, word_46434
    mov     ax, word ptr state.playerstate.car_longvec1.long_x
    mov     dx, word ptr state.playerstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1B8B2:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8B2
    sub     ax, [bp+var_58]
    mov     [bx], ax
    mov     bx, word_46434
    mov     ax, word ptr state.playerstate.car_longvec1.long_y
    mov     dx, word ptr state.playerstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1B8CC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8CC
    sub     ax, [bp+var_56]
    mov     [bx+2], ax
    mov     bx, word_46434
    mov     ax, word ptr state.playerstate.car_longvec1.long_z
    mov     dx, word ptr state.playerstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1B8E7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1B8E7
    sub     ax, [bp+var_54]
    mov     [bx+4], ax
    cmp     [bp+var_FC], 0
    jnz     short loc_1B903
    cmp     byte_3B8FA, 2
    jbe     short loc_1B914
loc_1B903:
    mov     bx, [bp+var_12E]
    mov     ax, [bx+6]
    mov     bx, word_46434
    mov     [bx+6], ax
    jmp     short loc_1B94A
    ; align 2
    db 144
loc_1B914:
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     ax, 754Ah
    push    ax
    mov     ax, 8C8Ah
    push    ax
    mov     ax, 8C78h
    push    ax
    mov     ax, 8ECAh
    push    ax
    push    state.playerstate.field_20
    mov     ax, word ptr game3dshapes.shape3d_verts+0AD4h
    mov     dx, word ptr game3dshapes.shape3d_verts+0AD6h
    add     ax, 30h ; '0'
    push    dx
    push    ax
    call    sub_204AE
    add     sp, 0Eh
loc_1B94A:
    cmp     word_44984, 0
    jz      short loc_1B964
    mov     bx, word_46434
    mov     word ptr [bx+8], 92ACh
loc_1B95A:
    mov     bx, word_46434
    mov     byte ptr [bx+12h], 0Ch
    jmp     short loc_1B990
loc_1B964:
    cmp     state.playerstate.field_C9, 1
    jnz     short loc_1B988
    push    si
    push    di
    lea     di, [bp+var_6A]
    mov     si, 3394h
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, word_46434
    lea     ax, [bp+var_6A]
    mov     [bx+8], ax
    jmp     short loc_1B95A
    ; align 2
    db 144
loc_1B988:
    mov     bx, word_46434
    mov     byte ptr [bx+12h], 4
loc_1B990:
    mov     bx, word_46434
    mov     ax, state.playerstate.car_rotate.z
    neg     ax
    mov     [bx+0Ah], ax
    mov     bx, word_46434
    mov     ax, state.playerstate.car_rotate.y
    neg     ax
    mov     [bx+0Ch], ax
    mov     bx, word_46434
    mov     ax, state.playerstate.car_rotate.x
    neg     ax
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 12Ch
    mov     bx, word_46434
    mov     al, gameconfig.game_playermaterial
    mov     [bx+13h], al
    mov     ax, 2
    push    ax
    mov     ax, [bp+var_6C]
    and     ax, [bp+var_12A]
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1B9DA:
    mov     al, [bp+var_F6]
    cmp     [bp+var_4A], al
    jz      short loc_1B9EF
    mov     al, [bp+var_100]
    cmp     [bp+var_4A], al
    jz      short loc_1B9EF
    jmp     loc_1BC89
loc_1B9EF:
    mov     al, [bp+var_102]
    cmp     [bp+var_6E], al
    jz      short loc_1BA04
    mov     al, [bp+var_12C]
    cmp     [bp+var_6E], al
    jz      short loc_1BA04
    jmp     loc_1BC89
loc_1BA04:
    cmp     state.field_42A, 0
    jnz     short loc_1BA0E
    jmp     loc_1BB4C
loc_1BA0E:
    sub     di, di
loc_1BA10:
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    cmp     word ptr [bx-6F4Eh], 0
    jnz     short loc_1BA24
    jmp     loc_1BB43
loc_1BA24:
    cmp     byte ptr [di-6E99h], 1
    jz      short loc_1BA2E
    jmp     loc_1BB43
loc_1BA2E:
    mov     al, [di-6EB1h]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, offset sceneshapes3
    mov     [bp+var_12E], ax
    mov     ax, di
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_150], ax
    mov     bx, ax
    mov     ax, word ptr state.game_longvecs1.long_x[bx]
    mov     dx, word ptr (state.game_longvecs1.long_x+2)[bx]
    add     ax, word ptr state.opponentstate.car_longvec1.long_x
    adc     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1BA65:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BA65
    sub     ax, [bp+var_58]
    mov     bx, word_46434
    mov     [bx], ax
    mov     bx, [bp+var_150]
    mov     ax, word ptr state.game_longvecs2.long_x[bx]
    mov     dx, word ptr (state.game_longvecs2.long_x+2)[bx]
    add     ax, word ptr state.opponentstate.car_longvec1.long_y
    adc     dx, word ptr state.opponentstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1BA8C:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BA8C
    sub     ax, [bp+var_56]
    mov     bx, word_46434
    mov     [bx+2], ax
    mov     bx, [bp+var_150]
    mov     ax, word ptr state.game_longvecs3.long_x[bx]
    mov     dx, word ptr (state.game_longvecs3.long_x+2)[bx]
    add     ax, word ptr state.opponentstate.car_longvec1.long_z
    adc     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1BAB4:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BAB4
    sub     ax, [bp+var_54]
    mov     bx, word_46434
    mov     [bx+4], ax
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     bx, word_46434
    mov     word ptr [bx+8], 92A4h
    mov     bx, word_46434
    mov     al, [bp+var_122]
    or      al, 5
    mov     [bx+12h], al
    mov     bx, word ptr [bp+var_154]
    mov     ax, state.field_2FE[bx]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ah], ax
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6FAEh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Ch], ax
    mov     bx, word ptr [bp+var_154]
    mov     ax, [bx-6F7Eh]
    neg     ax
    mov     bx, word_46434
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 400h
    mov     bx, word_46434
    mov     al, gameconfig.game_opponentmaterial
    mov     [bx+13h], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_A4]
    and     ax, [bp+var_12A]
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1BB43:
    inc     di
    cmp     di, 18h
    jge     short loc_1BB4C
    jmp     loc_1BA10
loc_1BB4C:
    mov     [bp+var_12E], 20C2h
    mov     bx, word_46434
    mov     ax, word ptr state.opponentstate.car_longvec1.long_x
    mov     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1BB5F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB5F
    sub     ax, [bp+var_58]
    mov     [bx], ax
    mov     bx, word_46434
    mov     ax, word ptr state.opponentstate.car_longvec1.long_y
    mov     dx, word ptr state.opponentstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1BB79:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB79
    sub     ax, [bp+var_56]
    mov     [bx+2], ax
    mov     bx, word_46434
    mov     ax, word ptr state.opponentstate.car_longvec1.long_z
    mov     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1BB94:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1BB94
    sub     ax, [bp+var_54]
    mov     [bx+4], ax
    cmp     [bp+var_FC], 0
    jnz     short loc_1BBB0
    cmp     byte_3B8FA, 2
    jbe     short loc_1BBC0
loc_1BBB0:
    mov     bx, [bp+var_12E]
    mov     ax, [bx+6]
    mov     bx, word_46434
    mov     [bx+6], ax
    jmp     short loc_1BBF6
loc_1BBC0:
    mov     bx, [bp+var_12E]
    mov     ax, [bx+4]
    mov     bx, word_46434
    mov     [bx+6], ax
    mov     ax, 7594h
    push    ax
    mov     ax, 9184h
    push    ax
    mov     ax, 8D1Ah
    push    ax
    mov     ax, 8F9Ah
    push    ax
    push    state.opponentstate.field_20
    mov     ax, word ptr game3dshapes.shape3d_verts+0AEAh
    mov     dx, word ptr game3dshapes.shape3d_verts+0AECh
    add     ax, 30h ; '0'
    push    dx
    push    ax
    call    sub_204AE
    add     sp, 0Eh
loc_1BBF6:
    cmp     word_44984, 0
    jz      short loc_1BC10
    mov     bx, word_46434
    mov     word ptr [bx+8], 92B4h
loc_1BC06:
    mov     bx, word_46434
    mov     byte ptr [bx+12h], 0Ch
    jmp     short loc_1BC3E
loc_1BC10:
    cmp     state.opponentstate.field_C9, 1
    jnz     short loc_1BC36
    push    si
    push    di
    lea     di, [bp+var_8E]
    mov     si, 3394h
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, word_46434
    lea     ax, [bp+var_8E]
    mov     [bx+8], ax
    jmp     short loc_1BC06
    ; align 2
    db 144
loc_1BC36:
    mov     bx, word_46434
    mov     byte ptr [bx+12h], 4
loc_1BC3E:
    mov     bx, word_46434
    mov     ax, state.opponentstate.car_rotate.z
    neg     ax
    mov     [bx+0Ah], ax
    mov     bx, word_46434
    mov     ax, state.opponentstate.car_rotate.y
    neg     ax
    mov     [bx+0Ch], ax
    mov     bx, word_46434
    mov     ax, state.opponentstate.car_rotate.x
    neg     ax
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 12Ch
    mov     bx, word_46434
    mov     al, gameconfig.game_opponentmaterial
    mov     [bx+13h], al
    mov     ax, 3
    push    ax
    mov     ax, [bp+var_A4]
    and     ax, [bp+var_12A]
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1BC89:
    cmp     state.field_3F5, 0
    jz      short loc_1BC93
    jmp     loc_1BEAA
loc_1BC93:
    mov     al, startcol2
    cmp     [bp+var_F6], al
    jz      short loc_1BCA5
    cmp     [bp+var_100], al
    jz      short loc_1BCA5
    jmp     loc_1BEAA
loc_1BCA5:
    mov     al, startrow2
    cmp     [bp+var_102], al
    jz      short loc_1BCB7
    cmp     [bp+var_12C], al
    jz      short loc_1BCB7
    jmp     loc_1BEAA
loc_1BCB7:
    mov     ax, 24h ; '$'
    push    ax
    push    word_44DCA
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     [bp+var_DE], ax
    mov     ax, 24h ; '$'
    push    ax
    push    word_44DCA
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, 38h ; '8'
    mov     [bp+var_36], ax
    mov     ax, word ptr game3dshapes.shape3d_verts+98Ah
    mov     dx, word ptr game3dshapes.shape3d_verts+98Ch
    add     ax, 30h ; '0'
    mov     word ptr [bp+var_108], ax
    mov     word ptr [bp+var_108+2], dx
    les     bx, [bp+var_108]
    mov     ax, [bp+var_DE]
    sub     ax, 24h ; '$'
    mov     es:[bx], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_DE]
    sub     ax, 24h ; '$'
    mov     es:[bx+6], ax
    les     bx, [bp+var_108]
    mov     ax, 24h ; '$'
    sub     ax, [bp+var_DE]
    mov     es:[bx+0Ch], ax
    les     bx, [bp+var_108]
    mov     ax, 24h ; '$'
    sub     ax, [bp+var_DE]
    mov     es:[bx+12h], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_36]
    mov     es:[bx+4], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_36]
    mov     es:[bx+0Ah], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_36]
    mov     es:[bx+10h], ax
    les     bx, [bp+var_108]
    mov     ax, [bp+var_36]
    mov     es:[bx+16h], ax
    mov     ax, 24h ; '$'
    push    ax
    mov     ax, track_angle
    add     ah, 1
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     cx, 1B6h
    push    cx
    mov     cx, track_angle
    add     ch, 2
    push    cx
    mov     word ptr [bp+var_154], ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     cx, ax
    mov     al, startcol2
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-599Eh]
    add     cx, word ptr [bp+var_154]
    sub     cx, [bp+var_58]
    mov     bx, word_46434
    mov     [bx], cx
    mov     al, byte_4499B
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+178h]
    sub     ax, [bp+var_56]
    mov     bx, word_46434
    mov     [bx+2], ax
    mov     ax, 24h ; '$'
    push    ax
    mov     ax, track_angle
    add     ah, 1
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     cx, 1B6h
    push    cx
    mov     cx, track_angle
    add     ch, 2
    push    cx
    mov     word ptr [bp+var_154], ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     cx, ax
    mov     al, startrow2
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-55EAh]
    add     cx, word ptr [bp+var_154]
    sub     cx, [bp+var_54]
    mov     bx, word_46434
    mov     [bx+4], cx
    mov     bx, word_46434
    mov     word ptr [bx+6], 7FD6h
    mov     bx, word_46434
    mov     word ptr [bx+8], 92A4h
    mov     bx, word_46434
    mov     al, [bp+var_122]
    or      al, 4
    mov     [bx+12h], al
    mov     bx, word_46434
    mov     word ptr [bx+0Ah], 0
    mov     bx, word_46434
    mov     word ptr [bx+0Ch], 0
    mov     bx, word_46434
    mov     ax, track_angle
    mov     [bx+0Eh], ax
    mov     bx, word_46434
    mov     word ptr [bx+10h], 400h
    mov     ax, word_44DCA
    mov     cl, 6
    sar     ax, cl
    mov     [bp+var_DE], ax
    cmp     ax, 3
    jle     short loc_1BE8D
    mov     [bp+var_DE], 3
loc_1BE8D:
    mov     bx, word_46434
    mov     al, byte ptr [bp+var_DE]
    mov     [bx+13h], al
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_12A]
smart
    and     ax, 0F800h
nosmart
    push    ax
    push    cs
    call near ptr sub_1CB80
    add     sp, 4
loc_1BEAA:
    cmp     byte_45D7E, 0
    jnz     short loc_1BEB4
    jmp     loc_1BF6C
loc_1BEB4:
    cmp     byte_45D7E, 1
    jle     short loc_1BED0
    mov     ax, 0A6ACh
    push    ax
    mov     ax, 9D6Ah
    push    ax
    mov     al, byte_45D7E
    cbw
    push    ax
    call    sub_36BE8
    add     sp, 6
loc_1BED0:
    mov     [bp+var_DE], 0
    jmp     short loc_1BF38
loc_1BED8:
    cmp     state.playerstate.car_is_braking, 0
    jnz     short loc_1BF09
loc_1BEDF:
    mov     byte_45514, 2Eh ; '.'
loc_1BEE4:
    mov     ax, 14h
    imul    di
    add     ax, 72CAh
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cbw
    mov     [bp+var_14E], ax
    or      ax, ax
    jle     short loc_1BF10
    jmp     loc_1B03C
    ; align 2
    db 144
loc_1BF02:
    cmp     state.opponentstate.car_is_braking, 0
    jz      short loc_1BEDF
loc_1BF09:
    mov     byte_45514, 2Fh ; '/'
    jmp     short loc_1BEE4
loc_1BF10:
    cmp     [bp+var_14E], 0
    jnz     short loc_1BF34
    mov     al, [di-5D6Eh]
    cbw
    cmp     ax, 2
    jz      short loc_1BF28
    cmp     ax, 3
    jz      short loc_1BF5E
    jmp     short loc_1BF34
loc_1BF28:
    cmp     state.playerstate.field_C9, 1
    jnz     short loc_1BF34
    mov     [bp+var_DC], 1
loc_1BF34:
    inc     [bp+var_DE]
loc_1BF38:
    mov     al, byte_45D7E
    cbw
    cmp     [bp+var_DE], ax
    jge     short loc_1BF6C
    mov     bx, [bp+var_DE]
    shl     bx, 1
    mov     di, [bx-5954h]
    mov     al, [di-5D6Eh]
    cbw
    cmp     ax, 2
    jz      short loc_1BED8
    cmp     ax, 3
    jz      short loc_1BF02
    jmp     short loc_1BEE4
    ; align 2
    db 144
loc_1BF5E:
    cmp     state.opponentstate.field_C9, 1
    jnz     short loc_1BF34
    mov     [bp+var_DB], 1
    jmp     short loc_1BF34
loc_1BF6C:
    inc     si
loc_1BF6D:
    cmp     si, 17h
    jl      short loc_1BF75
    jmp     loc_1B03C
loc_1BF75:
    cmp     [bp+si+var_32], 0
    jnz     short loc_1BF6C
    mov     al, [bp+si+var_86]
    mov     [bp+var_F6], al
    mov     al, [bp+si+var_D4]
    mov     [bp+var_102], al
    mov     al, [bp+si+var_14C]
    mov     [bp+var_62], al
    mov     al, [bp+si+var_1A]
    mov     [bp+var_10C], al
    mov     al, [bp+si+var_BC]
    mov     [bp+var_FC], al
    mov     [bp+var_12A], 0
    cmp     [bp+var_62], 0
    jnz     short loc_1BFB0
    jmp     loc_1AD4A
loc_1BFB0:
    mov     al, [bp+var_62]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_12E], ax
    mov     bx, ax
    mov     al, [bx+0Bh]
    cbw
    or      ax, ax
    jnz     short loc_1BFD5
    jmp     loc_1AD18
loc_1BFD5:
    cmp     ax, 1
    jnz     short loc_1BFDD
    jmp     loc_1AD26
loc_1BFDD:
    cmp     ax, 2
    jnz     short loc_1BFE5
    jmp     loc_1AD34
loc_1BFE5:
    cmp     ax, 3
    jnz     short loc_1BFED
    jmp     loc_1AD3C
loc_1BFED:
    jmp     loc_1AD55
loc_1BFF0:
    mov     [bp+var_3E], 92B4h
    jmp     short loc_1C009
    ; align 2
    db 144
loc_1BFF8:
    or      si, si
    jnz     short loc_1C002
    lea     ax, [bp+var_6A]
    jmp     short loc_1C006
    ; align 2
    db 144
loc_1C002:
    lea     ax, [bp+var_8E]
loc_1C006:
    mov     [bp+var_3E], ax
loc_1C009:
    push    [bp+arg_2]
    push    [bp+var_3E]
    call    sub_265EC
    add     sp, 4
    or      al, al
    jz      short loc_1C01E
    jmp     loc_1C0C1
loc_1C01E:
    mov     bx, [bp+var_3E]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    mov     bx, [bp+var_3E]
    mov     ax, [bx+2]
    add     ax, [bx]
    sar     ax, 1
    mov     [bp+var_F2], ax
    mov     ax, [bx+4]
    add     ax, [bx+6]
    sar     ax, 1
    mov     [bp+var_F0], ax
    mov     ax, [bx+2]
    sub     ax, [bx]
    mov     [bp+var_DE], ax
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    mov     [bp+var_36], ax
    mov     ax, [bp+var_DE]
    cmp     [bp+var_36], ax
    jle     short loc_1C070
    mov     ax, [bp+var_36]
    mov     [bp+var_DE], ax
loc_1C070:
    mov     ax, state.game_frame
    shr     ax, 1
    shr     ax, 1
    sub     dx, dx
    mov     cx, 3
    div     cx
    mov     di, dx
    mov     bx, di
    shl     bx, 1
    mov     ax, [bx-613Ah]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_DE]
    cwd
    mov     dh, dl
    mov     dl, ah
    mov     ah, al
    sub     al, al
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_36], ax
    push    [bp+var_F0]
    push    [bp+var_F2]
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx-59B2h]
    push    word ptr [bx-59B4h]
    push    ax
    call    sub_347DC
    add     sp, 0Ah
loc_1C0C1:
    inc     si
loc_1C0C2:
    cmp     si, 2
    jge     short loc_1C0E8
    cmp     [bp+si+var_DC], 0
    jz      short loc_1C0C1
    cmp     word_44984, 0
    jnz     short loc_1C0D8
    jmp     loc_1BFF8
loc_1C0D8:
    or      si, si
    jz      short loc_1C0DF
    jmp     loc_1BFF0
loc_1C0DF:
    mov     [bp+var_3E], 92ACh
    jmp     loc_1C009
    ; align 2
    db 144
loc_1C0E8:
    mov     bx, [bp+arg_2]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    cmp     byte_3B8F5, 0
    jz      short loc_1C10A
    jmp     loc_1C1C6
loc_1C10A:
    cmp     byte_463E0, 0
    jz      short loc_1C11E
    mov     [bp+var_104], 8F46h
    mov     si, state.game_frame2
    jmp     short loc_1C128
    ; align 2
    db 144
loc_1C11E:
    mov     [bp+var_104], 8E76h
    mov     si, state.field_146
loc_1C128:
    mov     bx, [bp+var_104]
    cmp     byte ptr [bx+0C9h], 1
    jnz     short loc_1C17C
    cmp     word_44984, 0
    jz      short loc_1C162
    mov     ax, 9294h
    push    ax
    push    ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    push    ax
    push    word ptr [bx+4]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    push    cs
    call near ptr sub_1D5B4
loc_1C156:
    add     sp, 6
    push    ax
    call    sub_26572
    jmp     short loc_1C1C3
    ; align 2
    db 144
loc_1C162:
    mov     bx, [bp+arg_2]
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    push    ax
    push    word ptr [bx+4]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    push    cs
    call near ptr sub_1D5B4
    jmp     short loc_1C1C3
    ; align 2
    db 144
loc_1C17C:
    mov     bx, [bp+var_104]
    cmp     byte ptr [bx+0C9h], 2
    jnz     short loc_1C1C6
    cmp     word_44984, 0
    jz      short loc_1C1AC
    mov     ax, 9294h
    push    ax
    push    ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    push    ax
    push    word ptr [bx+4]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    push    cs
    call near ptr sub_1D534
    jmp     short loc_1C156
loc_1C1AC:
    mov     bx, [bp+arg_2]
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    push    ax
    push    word ptr [bx+4]
    mov     ax, state.game_frame
    sub     ax, si
    push    ax
    push    cs
    call near ptr sub_1D534
loc_1C1C3:
    add     sp, 6
loc_1C1C6:
    cmp     byte_45DB2, 0
    jz      short loc_1C1D0
    jmp     loc_1C25B
loc_1C1D0:
    cmp     state.field_3F5, 0
    jnz     short loc_1C1DA
    jmp     loc_1C25B
loc_1C1DA:
    sub     ax, ax
    push    ax              ; int
    mov     ax, word_45A24
    add     ax, word_42D02
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    push    word_459F6
    push    word_459F4
    call    set_fontdef2
    add     sp, 4
    cmp     word_44984, 0
    jz      short loc_1C238
    mov     ax, 92C4h
    push    ax
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, word_461CC
    add     ax, 2
    push    ax
    mov     ax, 8Ch ; 'Œ'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    ax
    call    sub_26572
    add     sp, 6
    jmp     short loc_1C256
    ; align 4
    db 144
    db 144
loc_1C238:
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, word_461CC
    add     ax, 2
    push    ax
    mov     ax, 8Ch ; 'Œ'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28F98
    add     sp, 0Ah
loc_1C256:
    call    set_fontdef
loc_1C25B:
    cmp     word_44984, 0
    jnz     short loc_1C265
    jmp     loc_1C2F8
loc_1C265:
    mov     ax, 9294h
    push    ax
    push    ax
    push    cs
    call near ptr draw_ingame_text
    push    ax
    call    sub_26572
    add     sp, 6
    cmp     [bp+var_132], 0
    jz      short loc_1C2AE
    mov     ax, [bp+arg_2]
    push    si
    push    di
    mov     di, 9294h
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     si, 1
loc_1C293:
    mov     bx, si
    mov     cl, 3
    shl     bx, cl
    push    si
    push    di
    lea     di, [bx-6D6Ch]
    mov     si, 3394h
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     si
    cmp     si, 0Fh
    jl      short loc_1C293
loc_1C2AE:
    sub     si, si
loc_1C2B0:
    mov     ax, si
    mov     cl, 3
    shl     ax, cl
    mov     word ptr [bp+var_154], ax
    mov     bx, ax
    lea     ax, [bx-6D6Ch]
    mov     bx, word ptr [bp+var_154]
    add     bx, word_3B80A
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
    mov     ax, [bp+var_5C]
    mov     [bx-6D74h], ax
    mov     ax, [bp+var_5C]
    mov     word_463D6, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_1C2F8:
    push    cs
    call near ptr draw_ingame_text
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
update_frame endp
sub_1C302 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    cmp     byte_3B8FA, 4
    jz      short loc_1C320
    mov     si, [bp+arg_4]
    mov     bx, [bp+arg_0]
    sub     si, [bx+4]
    sub     si, skybox_current
    jmp     short loc_1C329
loc_1C320:
    mov     si, [bp+arg_4]
    mov     bx, [bp+arg_0]
    sub     si, [bx+4]
loc_1C329:
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    cmp     ax, si
    jge     short loc_1C339
    mov     si, [bx+6]
    sub     si, [bx+4]
loc_1C339:
    or      si, si
    jle     short loc_1C35F
    mov     ax, [bx+4]
    add     ax, si
    push    ax
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk1
    call    clear_sprite1_color
    add     sp, 2
loc_1C35F:
    cmp     byte_3B8FA, 4
    jnz     short loc_1C369
    jmp     loc_1C432
loc_1C369:
    mov     si, [bp+arg_2]
    add     si, 200h
smart
    and     si, 3FFh
nosmart
    sub     si, 400h
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_4]
    cmp     [bx+4], ax
    jl      short loc_1C386
    jmp     loc_1C432
loc_1C386:
    sub     ax, word_454CE
    cmp     ax, [bx+6]
    jle     short loc_1C392
    jmp     loc_1C432
loc_1C392:
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    mov     ax, [bp+arg_4]
    sub     ax, skybox_ptr1
    push    ax
    push    si
    push    word ptr skyboxes+2
    push    word ptr skyboxes
    call    sub_33BBC
    add     sp, 8
    mov     ax, [bp+arg_4]
    sub     ax, skybox_ptr2
    push    ax
    lea     ax, [si+140h]
    push    ax
    push    word ptr skyboxes+6
    push    word ptr skyboxes+4
    call    sub_33BBC
    add     sp, 8
    mov     ax, [bp+arg_4]
    sub     ax, skybox_ptr3
    push    ax
    lea     ax, [si+200h]
    push    ax
    push    word ptr skyboxes+0Ah
    push    word ptr skyboxes+8
    call    sub_33BBC
    add     sp, 8
    mov     ax, [bp+arg_4]
    sub     ax, skybox_ptr4
    push    ax
    lea     ax, [si+340h]
    push    ax
    push    word ptr skyboxes+0Eh
    push    word ptr skyboxes+0Ch
    call    sub_33BBC
    add     sp, 8
    mov     ax, [bp+arg_4]
    sub     ax, skybox_ptr1
    push    ax
    lea     ax, [si+400h]
    push    ax
    push    word ptr skyboxes+2
    push    word ptr skyboxes
    call    sub_33BBC
    add     sp, 8
loc_1C432:
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_4]
    cmp     [bx+4], ax
    jle     short loc_1C442
    mov     di, [bx+4]
    jmp     short loc_1C445
loc_1C442:
    mov     di, [bp+arg_4]
loc_1C445:
    mov     si, [bx+6]
    sub     si, di
    or      si, si
    jle     short loc_1C46D
    mov     ax, di
    add     ax, si
    push    ax
    push    di
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk2
    call    clear_sprite1_color
    add     sp, 2
loc_1C46D:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_1C302 endp
skybox_op proc far
    var_78 = byte ptr -120
    var_76 = word ptr -118
    var_72 = word ptr -114
    var_6E = word ptr -110
    var_5C = word ptr -92
    var_5A = word ptr -90
    var_58 = word ptr -88
    var_56 = word ptr -86
    var_54 = word ptr -84
    var_50 = word ptr -80
    var_4E = word ptr -78
    var_4C = word ptr -76
    var_4A = word ptr -74
    var_48 = word ptr -72
    var_46 = word ptr -70
    var_44 = word ptr -68
    var_42 = word ptr -66
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_34 = word ptr -52
    var_32 = word ptr -50
    var_30 = word ptr -48
    var_2E = word ptr -46
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = byte ptr -36
    var_20 = word ptr -32
    var_1E = byte ptr -30
    var_1A = word ptr -26
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = word ptr 16
    arg_C = word ptr 18

    push    bp
    mov     bp, sp
    sub     sp, 78h
    push    di
    push    si
    mov     byte_46166, 0
    mov     [bp+var_5C], 0
    mov     bx, [bp+arg_2]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    cmp     [bp+arg_8], 0
    jnz     short loc_1C4A7
    jmp     loc_1C958
loc_1C4A7:
    mov     ax, 4650h
    imul    [bp+arg_4]
    mov     [bp+var_58], ax
    mov     ax, [bp+arg_C]
    neg     ax
    mov     [bp+var_56], ax
    mov     ax, 3A98h
    imul    [bp+arg_4]
    mov     [bp+var_54], ax
    lea     ax, [bp+var_24]
    push    ax
    push    [bp+arg_6]
    lea     ax, [bp+var_58]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, 0B9B0h
    imul    [bp+arg_4]
    mov     [bp+var_58], ax
    lea     ax, [bp+var_1E]
    push    ax
    push    [bp+arg_6]
    lea     ax, [bp+var_58]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_20], 0
    jl      short loc_1C4FC
    cmp     [bp+var_1A], 0
    jge     short loc_1C51C
loc_1C4FC:
    mov     di, skybox_unk1
loc_1C500:
    mov     bx, [bp+arg_2]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    di
    jmp     loc_1CB6A
loc_1C51C:
    lea     ax, [bp+var_4E]
    push    ax
    lea     ax, [bp+var_24]
    push    ax
    call    sub_323D9
    add     sp, 4
    lea     ax, [bp+var_4A]
    push    ax
    lea     ax, [bp+var_1E]
    push    ax
    call    sub_323D9
    add     sp, 4
    cmp     [bp+var_4E], 140h
    jle     short loc_1C558
    cmp     [bp+var_4A], 140h
    jle     short loc_1C558
    mov     ax, [bp+var_48]
    cmp     [bp+var_4C], ax
    jl      short loc_1C4FC
loc_1C552:
    mov     di, skybox_unk2
    jmp     short loc_1C500
loc_1C558:
    cmp     [bp+var_4E], 0
    jge     short loc_1C56E
    cmp     [bp+var_4A], 0
    jge     short loc_1C56E
    mov     ax, [bp+var_48]
    cmp     [bp+var_4C], ax
loc_1C56A:
    jle     short loc_1C552
    jmp     short loc_1C4FC
loc_1C56E:
    mov     bx, [bp+arg_2]
    mov     ax, [bp+var_4C]
    cmp     [bx+6], ax
    jge     short loc_1C58A
    mov     ax, [bp+var_48]
    cmp     [bx+6], ax
    jge     short loc_1C58A
    mov     ax, [bp+var_4A]
    cmp     [bp+var_4E], ax
    jmp     short loc_1C56A
    ; align 2
    db 144
loc_1C58A:
    mov     ax, [bp+var_4C]
    cmp     [bx+4], ax
    jle     short loc_1C5A6
    mov     ax, [bp+var_48]
    cmp     [bx+4], ax
    jle     short loc_1C5A6
    mov     ax, [bp+var_4A]
    cmp     [bp+var_4E], ax
    jge     short loc_1C552
    jmp     loc_1C4FC
    ; align 2
    db 144
loc_1C5A6:
    mov     [bp+var_5A], 0
    cmp     byte_3B8FA, 4
    jz      short loc_1C61D
    cmp     [bp+var_4A], 0
    jge     short loc_1C61D
    cmp     [bp+var_4E], 140h
    jle     short loc_1C61D
    lea     ax, [bp+var_78]
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    push    [bp+var_48]
    push    [bp+var_4A]
    call    sub_2EB56
    add     sp, 0Ah
    or      ax, ax
    jnz     short loc_1C61D
    mov     di, [bp+var_72]
    sub     di, [bp+var_6E]
    jns     short loc_1C5EA
    mov     ax, di
    neg     ax
    jmp     short loc_1C5EC
    ; align 2
    db 144
loc_1C5EA:
    mov     ax, di
loc_1C5EC:
    cmp     ax, 60h ; '`'
    jge     short loc_1C61D
    cmp     [bp+var_76], 0
    jnz     short loc_1C602
    mov     ax, [bp+var_72]
    mov     [bp+var_32], ax
    mov     ax, [bp+var_6E]
    jmp     short loc_1C612
loc_1C602:
    cmp     [bp+var_76], 13Fh
    jnz     short loc_1C61D
    mov     ax, [bp+var_6E]
    mov     [bp+var_32], ax
    mov     ax, [bp+var_72]
loc_1C612:
    sub     ax, [bp+var_32]
    mov     [bp+var_26], ax
    mov     [bp+var_5A], 1
loc_1C61D:
    cmp     [bp+var_5A], 0
    jnz     short loc_1C626
    jmp     loc_1C852
loc_1C626:
    cmp     word_44984, 0
    jnz     short loc_1C630
    jmp     loc_1C7B6
loc_1C630:
    mov     [bp+var_30], 0
    mov     word_44A2C, 0
    mov     [bp+var_2E], 140h
    mov     word_44A2E, 140h
    cmp     byte_454A4, 0
    jz      short loc_1C660
    mov     bx, [bp+arg_2]
    mov     ax, [bx+4]
    mov     word_44A30, ax
    mov     ax, [bx+6]
    mov     word_44A32, ax
    jmp     loc_1C7AA
    ; align 2
    db 144
loc_1C660:
    mov     ax, [bp+var_32]
    add     ax, [bp+var_26]
    cmp     ax, [bp+var_32]
    jle     short loc_1C66E
    mov     ax, [bp+var_32]
loc_1C66E:
    sub     ax, word_454CE
    mov     word_44A30, ax
    mov     bx, [bp+arg_2]
    cmp     [bx+4], ax
    jle     short loc_1C683
    mov     ax, [bx+4]
    mov     word_44A30, ax
loc_1C683:
    mov     ax, [bp+var_32]
    add     ax, [bp+var_26]
    cmp     ax, [bp+var_32]
    jge     short loc_1C691
    mov     ax, [bp+var_32]
loc_1C691:
    mov     word_44A32, ax
    sub     si, si
loc_1C696:
    mov     byte ptr [si-6250h], 1
    inc     si
    cmp     si, 0Fh
    jl      short loc_1C696
    mov     byte_45525, 3
    mov     [bp+var_2C], 0
    mov     ax, word_44A30
    mov     [bp+var_2A], ax
    push    [bp+arg_2]
    lea     ax, [bp+var_30]
    push    ax
    call    sub_265EC
    add     sp, 4
    or      al, al
    jnz     short loc_1C728
    mov     byte_46166, 0
    mov     ax, 965Eh
    push    ax
    mov     ax, 0A9F6h
    push    ax
    lea     ax, [bp+var_30]
    push    ax
    mov     ax, 9294h
    push    ax
    push    word_3B80A
    mov     ax, 9DB0h
    push    ax
    mov     ax, 0Fh
    push    ax
    call    sub_26A52
    add     sp, 0Eh
    sub     di, di
    jmp     short loc_1C720
    ; align 2
    db 144
loc_1C6F2:
    mov     ax, di
    mov     cl, 3
    shl     ax, cl
    add     ax, 965Eh
    mov     [bp+var_28], ax
    mov     bx, ax
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk1
    call    clear_sprite1_color
    add     sp, 2
    inc     di
loc_1C720:
    mov     al, byte_46166
    cbw
    cmp     ax, di
    jg      short loc_1C6F2
loc_1C728:
    mov     ax, word_44A32
    mov     [bp+var_2C], ax
    mov     [bp+var_2A], 0C8h ; 'È'
    push    [bp+arg_2]
    lea     ax, [bp+var_30]
    push    ax
    call    sub_265EC
    add     sp, 4
    or      al, al
    jnz     short loc_1C7AA
    mov     byte_46166, 0
    mov     ax, 965Eh
    push    ax
    mov     ax, 0A9F6h
    push    ax
    lea     ax, [bp+var_30]
    push    ax
    mov     ax, 9294h
    push    ax
    push    word_3B80A
    mov     ax, 9DB0h
    push    ax
    mov     ax, 0Fh
    push    ax
    call    sub_26A52
    add     sp, 0Eh
    sub     di, di
    jmp     short loc_1C7A2
    ; align 2
    db 144
loc_1C774:
    mov     ax, di
    mov     cl, 3
    shl     ax, cl
    add     ax, 965Eh
    mov     [bp+var_28], ax
    mov     bx, ax
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk2
    call    clear_sprite1_color
    add     sp, 2
    inc     di
loc_1C7A2:
    mov     al, byte_46166
    cbw
    cmp     ax, di
    jg      short loc_1C774
loc_1C7AA:
    mov     ax, word_44A30
    mov     [bp+var_2C], ax
    mov     ax, word_44A32
    jmp     short loc_1C7C2
    ; align 2
    db 144
loc_1C7B6:
    mov     bx, [bp+arg_2]
    mov     ax, [bx+4]
    mov     [bp+var_2C], ax
    mov     ax, [bx+6]
loc_1C7C2:
    mov     [bp+var_2A], ax
    mov     [bp+var_30], 0
    mov     [bp+var_2E], 140h
    push    [bp+arg_2]
    lea     ax, [bp+var_30]
    push    ax
    call    sub_265EC
    add     sp, 4
    or      al, al
    jz      short loc_1C7E5
    jmp     loc_1CB77
loc_1C7E5:
    mov     [bp+var_34], 0
    cmp     [bp+var_26], 0
    jge     short loc_1C7F8
    mov     ax, [bp+var_26]
    neg     ax
    jmp     short loc_1C7FB
    ; align 2
    db 144
loc_1C7F8:
    mov     ax, [bp+var_26]
loc_1C7FB:
    mov     di, ax
    inc     di
    cmp     di, 20h ; ' '
    jle     short loc_1C806
    mov     di, 20h ; ' '
loc_1C806:
    sub     si, si
    jmp     short loc_1C84B
loc_1C80A:
    mov     ax, [bp+var_34]
    mov     [bp+var_30], ax
    mov     ax, 140h
    imul    si
    add     ax, 140h
    cwd
    idiv    di
    and     ax, word_454D4
    mov     [bp+var_2E], ax
    cmp     [bp+var_30], ax
    jz      short loc_1C84A
    mov     ax, [bp+var_26]
    imul    si
    cwd
    idiv    di
    add     ax, [bp+var_32]
    mov     [bp+var_50], ax
    push    ax
    push    [bp+arg_A]
    lea     ax, [bp+var_30]
    push    ax
    push    cs
    call near ptr sub_1C302
    add     sp, 6
    mov     ax, [bp+var_2E]
    mov     [bp+var_34], ax
loc_1C84A:
    inc     si
loc_1C84B:
    cmp     si, di
    jl      short loc_1C80A
    jmp     loc_1CB77
loc_1C852:
    mov     ax, [bp+var_4C]
    sub     ax, [bp+var_48]
    push    ax
    mov     ax, [bp+var_4E]
    sub     ax, [bp+var_4A]
    push    ax
    call    sub_2EA4E
    add     sp, 4
    mov     si, ax
smart
    and     si, 3FFh
nosmart
    mov     [bp+var_26], 2
    jmp     short loc_1C8F0
    ; align 2
    db 144
loc_1C876:
    mov     di, 1
loc_1C879:
    mov     bx, [bp+var_26]
    shl     bx, 1
    mov     ax, [bx+98Ch]
    add     ax, si
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    mov     ax, 3E80h
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     cx, [bx-4Eh]
    add     cx, ax
    mov     bx, [bp+var_26]
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     [bx-4Eh], cx
    mov     bx, [bp+var_26]
    shl     bx, 1
    mov     ax, [bx+98Ch]
    add     ax, si
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    mov     ax, 3E80h
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     cx, [bx-4Ch]
    add     cx, ax
    mov     bx, [bp+var_26]
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     [bx-4Ch], cx
    inc     [bp+var_26]
loc_1C8F0:
    cmp     [bp+var_26], 6
    jge     short loc_1C904
    cmp     [bp+var_26], 4
    jl      short loc_1C8FF
    jmp     loc_1C876
loc_1C8FF:
    sub     di, di
    jmp     loc_1C879
loc_1C904:
    push    [bp+var_44]
    push    [bp+var_46]
    push    [bp+var_40]
    push    [bp+var_42]
    push    [bp+var_48]
    push    [bp+var_4A]
    push    [bp+var_4C]
    push    [bp+var_4E]
    mov     ax, 4
    push    ax
    push    skybox_unk1
    call    sub_317DF
    add     sp, 14h
    push    [bp+var_38]
    push    [bp+var_3A]
    push    [bp+var_3C]
    push    [bp+var_3E]
    push    [bp+var_48]
    push    [bp+var_4A]
    push    [bp+var_4C]
    push    [bp+var_4E]
    mov     ax, 4
    push    ax
    push    skybox_unk2
    call    sub_317DF
    add     sp, 14h
    jmp     loc_1CB72
    ; align 2
    db 144
loc_1C958:
    mov     [bp+var_58], 0
    mov     ax, [bp+arg_C]
    neg     ax
    mov     [bp+var_56], ax
    mov     ax, 3A98h
    imul    [bp+arg_4]
    mov     [bp+var_54], ax
    lea     ax, [bp+var_24]
    push    ax
    push    [bp+arg_6]
    lea     ax, [bp+var_58]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_20], 0
    jge     short loc_1C9C0
    push    skybox_unk1
    call    clear_sprite1_color
    add     sp, 2
    cmp     word_44984, 0
    jnz     short loc_1C99D
    jmp     loc_1CB77
loc_1C99D:
    mov     [bp+var_5C], 1
    mov     word_44A2C, 0
    mov     word_44A2E, 140h
    mov     bx, [bp+arg_2]
    mov     ax, [bx+4]
    mov     word_44A30, ax
    mov     ax, [bx+6]
    mov     word_44A32, ax
    jmp     loc_1CB77
loc_1C9C0:
    lea     ax, [bp+var_4E]
    push    ax
    lea     ax, [bp+var_24]
    push    ax
    call    sub_323D9
    add     sp, 4
    mov     ax, [bp+var_4C]
    mov     [bp+var_50], ax
    mov     bx, [bp+arg_2]
    cmp     [bx+4], ax
    jle     short loc_1C9E4
    mov     ax, [bx+4]
    mov     [bp+var_50], ax
loc_1C9E4:
    cmp     [bp+arg_4], 1
    jz      short loc_1C9ED
    jmp     loc_1CB00
loc_1C9ED:
    cmp     word_44984, 0
    jz      short loc_1CA25
    cmp     byte_3B8FA, 4
    jnz     short loc_1CA02
    mov     ax, [bp+var_50]
    dec     ax
    jmp     short loc_1CA09
    ; align 2
    db 144
loc_1CA02:
    mov     ax, [bp+var_50]
    sub     ax, word_454CE
loc_1CA09:
    mov     word_44A30, ax
    mov     word_44A2C, 0
    mov     word_44A2E, 140h
    mov     ax, [bp+var_50]
    mov     word_44A32, ax
    cmp     byte_454A4, 0
    jz      short loc_1CA52
loc_1CA25:
    mov     [bp+var_30], 0
    mov     [bp+var_2E], 140h
    mov     bx, [bp+arg_2]
    mov     ax, [bx+4]
    mov     [bp+var_2C], ax
    mov     ax, [bx+6]
    mov     [bp+var_2A], ax
    push    [bp+var_50]
    push    [bp+arg_A]
    lea     ax, [bp+var_30]
    push    ax
    push    cs
    call near ptr sub_1C302
    add     sp, 6
    jmp     loc_1CB77
loc_1CA52:
    sub     si, si
loc_1CA54:
    mov     byte ptr [si-6250h], 1
    inc     si
    cmp     si, 0Fh
    jl      short loc_1CA54
    cmp     byte_3B8FA, 4
    jnz     short loc_1CA72
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, word_463D6
    mov     [bx-6D74h], ax
loc_1CA72:
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, [bp+arg_A]
    cmp     [bx-6D74h], ax
    jnz     short loc_1CAAC
    mov     bx, word_3B80A
    mov     ax, word_44A2C
    cmp     [bx+28h], ax
    jnz     short loc_1CAAC
    mov     ax, word_44A2E
    cmp     [bx+2Ah], ax
    jnz     short loc_1CAAC
    mov     ax, word_44A30
    cmp     [bx+2Ch], ax
    jnz     short loc_1CAAC
    mov     ax, word_44A32
    cmp     [bx+2Eh], ax
    jnz     short loc_1CAAC
    mov     byte_45525, 0
    jmp     short loc_1CAB1
    ; align 2
    db 144
loc_1CAAC:
    mov     byte_45525, 3
loc_1CAB1:
    mov     byte_46166, 0
    mov     ax, 965Eh
    push    ax
    mov     ax, 0A9F6h
    push    ax
    push    [bp+arg_2]
    mov     ax, 9294h
    push    ax
    push    word_3B80A
    mov     ax, 9DB0h
    push    ax
    mov     ax, 0Fh
    push    ax
    call    sub_26A52
    add     sp, 0Eh
    sub     di, di
    jmp     short loc_1CAF6
    ; align 2
    db 144
loc_1CADE:
    push    [bp+var_50]
    push    [bp+arg_A]
    mov     ax, di
    mov     cl, 3
    shl     ax, cl
    add     ax, 965Eh
    push    ax
    push    cs
    call near ptr sub_1C302
    add     sp, 6
    inc     di
loc_1CAF6:
    mov     al, byte_46166
    cbw
    cmp     ax, di
    jg      short loc_1CADE
    jmp     short loc_1CB77
loc_1CB00:
    mov     si, [bp+var_50]
    mov     bx, [bp+arg_2]
    sub     si, [bx+4]
    mov     ax, [bx+6]
    sub     ax, [bx+4]
    cmp     ax, si
    jge     short loc_1CB19
    mov     si, [bx+6]
    sub     si, [bx+4]
loc_1CB19:
    or      si, si
    jle     short loc_1CB41
    mov     ax, [bx+4]
    add     ax, si
    push    ax
    push    word ptr [bx+4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk2
    call    clear_sprite1_color
    add     sp, 2
loc_1CB41:
    mov     bx, [bp+arg_2]
    mov     si, [bx+6]
    sub     si, [bp+var_50]
    or      si, si
    jle     short loc_1CB72
    mov     ax, [bp+var_50]
    add     ax, si
    push    ax
    push    [bp+var_50]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk1
loc_1CB6A:
    call    clear_sprite1_color
    add     sp, 2
loc_1CB72:
    mov     [bp+var_5C], 1
loc_1CB77:
    mov     ax, [bp+var_5C]
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
skybox_op endp
sub_1CB80 proc far
    var_C = byte ptr -12
    var_8 = word ptr -8
    var_6 = byte ptr -6
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = byte ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 0Ch
    push    di
    push    si
    mov     ax, word_46434
    push    si
    push    di
    lea     di, [bp+var_6]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    lea     ax, [bp+var_C]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    lea     ax, [bp+var_6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     al, byte_45D7E
    cbw
    mov     si, ax
    mov     di, si
    shl     di, 1
    mov     ax, [bp+var_8]
    add     ax, [bp+arg_0]
    mov     [di-6296h], ax
    mov     al, [bp+arg_2]
    mov     [si-5D6Eh], al
    mov     [di-5954h], si
    inc     byte_45D7E
    add     word_46434, 14h
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_1CB80 endp
sub_1CBDC proc far
    var_42 = word ptr -66
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3C = byte ptr -60
    var_3A = byte ptr -58
    var_38 = byte ptr -56
    var_36 = byte ptr -54
    var_34 = word ptr -52
    var_32 = byte ptr -50
    var_30 = word ptr -48
    var_2E = byte ptr -46
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = word ptr -32
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = byte ptr -20
    var_13 = byte ptr -19
    var_12 = word ptr -18
    var_10 = byte ptr -16
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 42h
    push    di
    push    si
    mov     ax, word_3C112
    sub     ax, word_3C10C
    push    ax
    mov     ax, word_3C10E
    sub     ax, word_3C108
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_4], ax
    push    ax
    mov     ax, word_3C110
    sub     ax, word_3C10A
    push    ax
    call    sub_2EA4E
    add     sp, 4
    mov     [bp+var_34], ax
    mov     ax, 1
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_34]
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_28], ax
    lea     ax, [bp+var_C]
    push    ax
    push    [bp+var_28]
    mov     ax, 9A4h
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_2E]
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_323D9
    add     sp, 4
    mov     ax, [bp+var_2C]
    mov     [bp+var_30], ax
    or      ax, ax
    jge     short loc_1CC5B
    mov     [bp+var_30], 0
loc_1CC5B:
    mov     ax, [bp+var_30]
    sub     ax, skybox_current
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk1
    call    clear_sprite1_color
    add     sp, 2
    mov     ax, 64h ; 'd'
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    mov     ax, [bp+var_30]
    sub     ax, skybox_ptr3
    push    ax
    sub     ax, ax
    push    ax
    push    word ptr skyboxes+0Ah
    push    word ptr skyboxes+8
    call    sub_33BBC
    add     sp, 8
    mov     ax, [bp+var_30]
    sub     ax, skybox_ptr4
    push    ax
    mov     ax, 140h
    push    ax
    push    word ptr skyboxes+0Eh
    push    word ptr skyboxes+0Ch
    call    sub_33BBC
    add     sp, 8
    mov     ax, 0C8h ; 'È'
    push    ax
    push    [bp+var_30]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    skybox_unk2
    call    clear_sprite1_color
    add     sp, 2
    mov     ax, 0C8h ; 'È'
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    mov     ax, 1
    push    ax
    mov     ax, 9AAh
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_34]
    push    ax
    call    select_rect_rotate
    add     sp, 0Ah
    mov     [bp+var_1C], 0
    mov     [bp+var_1A], 0
    mov     [bp+var_16], 400h
    mov     [bp+var_38], 0
    jmp     loc_1D11B
    ; align 2
    db 144
loc_1CD34:
    mov     [bp+var_3A], 0
    mov     [bp+var_10], 0
loc_1CD3C:
    cmp     [bp+var_3A], 6
    jz      short loc_1CD45
    jmp     loc_1CE04
loc_1CD45:
    mov     di, word_3B8EA
    cmp     [bp+var_10], 0
    jz      short loc_1CD53
loc_1CD4F:
    mov     [bp+var_3A], 0
loc_1CD53:
    cmp     [bp+var_3A], 0
    jz      short loc_1CDCA
    mov     al, [bp+var_3A]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2C5Ah
    mov     [bp+var_3E], ax
    mov     bx, ax
    mov     ax, [bx+6]
    mov     [bp+var_20], ax
    mov     al, [bp+var_32]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    sub     ax, word_3C108
    sar     ax, 1
    mov     [bp+var_26], ax
    mov     ax, di
    sub     ax, word_3C10A
    sar     ax, 1
    mov     [bp+var_24], ax
    mov     al, [bp+var_38]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    sub     ax, word_3C10C
    sar     ax, 1
    mov     [bp+var_22], ax
    mov     bx, [bp+var_3E]
    mov     ax, [bx+2]
    mov     [bp+var_18], ax
    mov     [bp+var_14], 5
    mov     [bp+var_13], 0
    lea     ax, [bp+var_26]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1CDCA:
    cmp     [bp+var_10], 0
    jnz     short loc_1CDD3
    jmp     loc_1D086
loc_1CDD3:
    mov     al, [bp+var_10]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_3E], ax
    mov     bx, ax
    test    byte ptr [bx+0Bh], 1
    jnz     short loc_1CDF5
    jmp     loc_1CF14
loc_1CDF5:
    mov     al, [bp+var_38]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-7E84h]
    jmp     loc_1CF20
loc_1CE04:
    sub     di, di
    mov     al, [bp+var_10]
    sub     ah, ah
    cmp     ax, 69h ; 'i'
    jnb     short loc_1CE13
    jmp     loc_1CD53
loc_1CE13:
    cmp     ax, 6Ch ; 'l'
    jbe     short loc_1CE1B
    jmp     loc_1CD53
loc_1CE1B:
    mov     [bp+var_2A], di
    jmp     loc_1CED0
    ; align 2
    db 144
loc_1CE22:
    mov     al, [bp+var_32]
loc_1CE25:
    mov     [bp+var_36], al
    mov     al, [bp+var_38]
loc_1CE2B:
    mov     [bp+var_3C], al
loc_1CE2E:
    mov     al, [bp+var_36]
    cbw
    mov     [bp+var_42], ax
    mov     al, [bp+var_3C]
    cbw
    shl     ax, 1
    mov     [bp+var_40], ax
    mov     bx, ax
    mov     bx, [bx-73C4h]
    add     bx, [bp+var_42]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_3A], al
    or      al, al
    jz      short loc_1CECD
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2C5Ah
    mov     [bp+var_3E], ax
    mov     bx, ax
    mov     ax, [bx+4]
    mov     [bp+var_20], ax
    mov     bx, [bp+var_42]
    shl     bx, 1
    mov     ax, [bx-599Eh]
    sub     ax, word_3C108
    sar     ax, 1
    mov     [bp+var_26], ax
    mov     ax, word_3C10A
    neg     ax
    sar     ax, 1
    mov     [bp+var_24], ax
    mov     bx, [bp+var_40]
    mov     ax, [bx-55EAh]
    sub     ax, word_3C10C
    sar     ax, 1
    mov     [bp+var_22], ax
    mov     [bp+var_14], 5
    mov     [bp+var_1C], 0
    mov     [bp+var_1A], 0
    mov     bx, [bp+var_3E]
    mov     ax, [bx+2]
    mov     [bp+var_18], ax
    mov     [bp+var_16], 400h
    mov     [bp+var_13], 0
    lea     ax, [bp+var_26]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1CECD:
    inc     [bp+var_2A]
loc_1CED0:
    cmp     [bp+var_2A], 4
    jl      short loc_1CED9
    jmp     loc_1CD4F
loc_1CED9:
    mov     ax, [bp+var_2A]
    or      ax, ax
    jnz     short loc_1CEE3
    jmp     loc_1CE22
loc_1CEE3:
    cmp     ax, 1
    jz      short loc_1CEF6
    cmp     ax, 2
    jz      short loc_1CEFE
    cmp     ax, 3
    jz      short loc_1CF0C
    jmp     loc_1CE2E
    ; align 2
    db 144
loc_1CEF6:
    mov     al, [bp+var_32]
    inc     al
    jmp     loc_1CE25
loc_1CEFE:
    mov     al, [bp+var_32]
loc_1CF01:
    mov     [bp+var_36], al
    mov     al, [bp+var_38]
    inc     al
    jmp     loc_1CE2B
loc_1CF0C:
    mov     al, [bp+var_32]
    inc     al
    jmp     short loc_1CF01
    ; align 2
    db 144
loc_1CF14:
    mov     al, [bp+var_38]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
loc_1CF20:
    mov     [bp+var_12], ax
    mov     bx, [bp+var_3E]
    test    byte ptr [bx+0Bh], 2
    jz      short loc_1CF3A
    mov     al, [bp+var_32]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     si, [bx+7558h]
    jmp     short loc_1CF46
loc_1CF3A:
    mov     al, [bp+var_32]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     si, [bx-599Eh]
loc_1CF46:
    mov     ax, si
    sub     ax, word_3C108
    sar     ax, 1
    mov     [bp+var_C], ax
    mov     ax, di
    sub     ax, word_3C10A
    sar     ax, 1
    mov     [bp+var_A], ax
    mov     ax, [bp+var_12]
    sub     ax, word_3C10C
    sar     ax, 1
    mov     [bp+var_8], ax
    or      di, di
    jz      short loc_1CFBF
    mov     bx, [bp+var_3E]
    mov     al, [bx+0Bh]
    cbw
    or      ax, ax
    jz      short loc_1CF92
    cmp     ax, 1
    jnz     short loc_1CF7F
    jmp     loc_1D01A
loc_1CF7F:
    cmp     ax, 2
    jnz     short loc_1CF87
    jmp     loc_1D022
loc_1CF87:
    cmp     ax, 3
    jnz     short loc_1CF8F
    jmp     loc_1D02A
loc_1CF8F:
    jmp     short loc_1CF97
    ; align 2
    db 144
loc_1CF92:
    mov     [bp+var_20], 79FEh
loc_1CF97:
    push    si
    push    di
    lea     di, [bp+var_26]
    lea     si, [bp+var_C]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     [bp+var_18], 0
    mov     [bp+var_14], 5
    mov     [bp+var_13], 0
    lea     ax, [bp+var_26]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1CFBF:
    mov     bx, [bp+var_3E]
    cmp     byte ptr [bx+8], 0
    jz      short loc_1D042
    mov     al, [bx+8]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_2], ax
    mov     bx, ax
    cmp     word ptr [bx+6], 0
    jz      short loc_1D042
    mov     ax, [bx+6]
    mov     [bp+var_20], ax
    push    si
    push    di
    lea     di, [bp+var_26]
    lea     si, [bp+var_C]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [bp+var_3E]
    mov     ax, [bx+2]
    mov     [bp+var_18], ax
    mov     [bp+var_14], 5
    mov     bx, [bp+var_2]
    cmp     byte ptr [bx+9], 0
    jl      short loc_1D032
    mov     al, [bx+9]
    mov     [bp+var_13], al
    jmp     short loc_1D036
loc_1D01A:
    mov     [bp+var_20], 7E1Eh
    jmp     loc_1CF97
loc_1D022:
    mov     [bp+var_20], 7E34h
    jmp     loc_1CF97
loc_1D02A:
    mov     [bp+var_20], 7E4Ah
    jmp     loc_1CF97
loc_1D032:
    mov     [bp+var_13], 0
loc_1D036:
    lea     ax, [bp+var_26]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1D042:
    mov     bx, [bp+var_3E]
    mov     ax, [bx+6]
    mov     [bp+var_20], ax
    push    si
    push    di
    lea     di, [bp+var_26]
    lea     si, [bp+var_C]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     ax, [bx+2]
    mov     [bp+var_18], ax
    mov     al, [bx+0Ah]
    or      al, 4
    mov     [bp+var_14], al
    cmp     byte ptr [bx+9], 0
    jl      short loc_1D076
    mov     al, [bx+9]
    mov     [bp+var_13], al
    jmp     short loc_1D07A
loc_1D076:
    mov     [bp+var_13], 0
loc_1D07A:
    lea     ax, [bp+var_26]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1D086:
    call    sub_25FF6
    inc     [bp+var_32]
loc_1D08E:
    cmp     [bp+var_32], 1Eh
    jl      short loc_1D097
    jmp     loc_1D118
loc_1D097:
    mov     al, [bp+var_32]
    cbw
    mov     [bp+var_40], ax
    mov     al, [bp+var_38]
    cbw
    shl     ax, 1
    mov     [bp+var_42], ax
    mov     bx, ax
    mov     bx, [bx-5A30h]
    add     bx, [bp+var_40]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_10], al
    mov     bx, [bp+var_42]
    mov     bx, [bx-73C4h]
    add     bx, [bp+var_40]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_3A], al
    cmp     [bp+var_10], 0
    jnz     short loc_1D0DF
    jmp     loc_1CD3C
loc_1D0DF:
    cmp     al, 7
    jb      short loc_1D100
    cmp     al, 0Bh
    jnb     short loc_1D100
    mov     al, [bp+var_10]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_3A]
    push    ax
    call    map_terrain_track
    add     sp, 4
    mov     [bp+var_10], al
    mov     [bp+var_3A], 0
loc_1D100:
    mov     al, [bp+var_10]
    sub     ah, ah
    cmp     ax, 0FDh ; 'ý'
    jnb     short loc_1D10D
    jmp     loc_1CD3C
loc_1D10D:
    cmp     ax, 0FFh
    ja      short loc_1D115
    jmp     loc_1CD34
loc_1D115:
    jmp     loc_1CD3C
loc_1D118:
    inc     [bp+var_38]
loc_1D11B:
    cmp     [bp+var_38], 1Eh
    jge     short loc_1D128
    mov     [bp+var_32], 0
    jmp     loc_1D08E
loc_1D128:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1CBDC endp
draw_ingame_text proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    push    si
    mov     di, offset word_4617E
    mov     si, offset rect_unk4
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     si
    cmp     byte_44AE2, 0
    jnz     short loc_1D14E
    jmp     loc_1D1E6
loc_1D14E:
    mov     ax, offset aDm1 ; "Professional Driver"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0AAh ; 'ª'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
    mov     ax, offset aDm2 ; on Closed Circuit
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0B6h ; '¶'
loc_1D1D5:
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    jmp     loc_1D511
    ; align 2
    db 144
loc_1D1E6:
    cmp     byte_45DB2, 0
    jz      short loc_1D1F0
    jmp     loc_1D4B0
loc_1D1F0:
    cmp     state.field_3F5, 0
    jnz     short loc_1D22A
    mov     ax, offset aPre ; Fasten Your Seatbelt
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 5Ah ; 'Z'
    jmp     short loc_1D1D5
    ; align 2
    db 144
loc_1D22A:
    cmp     passed_security, 0
    jz      short loc_1D234
    jmp     loc_1D2BE
loc_1D234:
    mov     ax, offset aSe1 ; You forgot to disable the
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 5Dh ; ']'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
    mov     ax, offset aSe2 ; "Car's security system first"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 69h ; 'i'
    jmp     loc_1D1D5
loc_1D2BE:
    cmp     byte_463E0, 0
    jz      short loc_1D2C8
    jmp     loc_1D52B
loc_1D2C8:
    cmp     byte_3B8F5, 0
    jz      short loc_1D2D2
    jmp     loc_1D52B
loc_1D2D2:
    cmp     state.playerstate.field_C9, 0
    jz      short loc_1D2DC
    jmp     loc_1D52B
loc_1D2DC:
    mov     al, state.field_45D
    cbw
    cmp     ax, 1
    jz      short loc_1D2F2
    cmp     ax, 2
    jz      short loc_1D338
    cmp     ax, 3
    jz      short loc_1D34A
    jmp     short loc_1D31E
    ; align 2
    db 144
loc_1D2F2:
    mov     ax, 5Dh ; ']'
    push    ax
    mov     ax, 94h ; '”'
    push    ax
    push    word_45DCA
    push    word_45DC8
loc_1D302:
    call    sub_343B0
    add     sp, 8
    mov     ax, offset word_4617E
    push    ax
    mov     ax, 9DAh
loc_1D311:
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
loc_1D31E:
    mov     byte_463E4, 0
    mov     al, state.field_45E
    cbw
    cmp     ax, 1
    jz      short loc_1D398
    cmp     ax, 2
    jnz     short loc_1D334
    jmp     loc_1D47E
loc_1D334:
    jmp     loc_1D3E6
    ; align 2
    db 144
loc_1D338:
    mov     ax, 5Dh ; ']'
    push    ax
    mov     ax, 94h ; '”'
    push    ax
    push    word_45DCE
    push    word_45DCC
    jmp     short loc_1D302
loc_1D34A:
    mov     ax, offset aWww ; "Wrong Way"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 5Dh ; ']'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    jmp     loc_1D311
    ; align 2
    db 144
loc_1D398:
    mov     ax, 71h ; 'q'
    push    ax
    mov     ax, 44h ; 'D'
    push    ax
    push    word_45DCA
    push    word_45DC8
    call    sub_343B0
    add     sp, 8
    mov     ax, offset word_4617E
    push    ax
    mov     ax, offset unk_3C152
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
    mov     ax, offset aOpp ; "Opponent Near"
loc_1D3C7:
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
loc_1D3E6:
    cmp     byte_463E4, 0
    jz      short loc_1D422
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 74h ; 't'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
loc_1D422:
    cmp     byte_4499F, 0
    jnz     short loc_1D42C
    jmp     loc_1D52B
loc_1D42C:
    mov     ax, offset aPen ; "pen"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax              ; int
    push    word_461CA
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 66h ; 'f'
    jmp     loc_1D1D5
loc_1D47E:
    mov     ax, 71h ; 'q'
    push    ax
    mov     ax, 0E4h ; 'ä'
    push    ax
    push    word_45DCE
    push    word_45DCC
    call    sub_343B0
    add     sp, 8
    mov     ax, offset word_4617E
    push    ax
    mov     ax, 9EAh
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
    mov     ax, 9CEh
    jmp     loc_1D3C7
loc_1D4B0:
    cmp     byte_45DB2, 2
    jnz     short loc_1D52B
    mov     ax, state.game_frame
    sub     dx, dx
    div     framespersec
    mov     si, dx
    mov     ax, framespersec
    sar     ax, 1
    cmp     ax, si
    jle     short loc_1D52B
    mov     ax, offset aRpl_0; "rpl"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset word_4617E
    push    ax
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0Fh
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    mov     cl, 3
    shl     ax, cl
    sub     ax, 138h
    neg     ax
loc_1D511:
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    ax
    mov     ax, offset word_4617E
    push    ax
    call    sub_26572
    add     sp, 6
loc_1D52B:
    mov     ax, offset word_4617E
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
draw_ingame_text endp
sub_1D534 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    mov     di, framespersec
    mov     cl, 2
    shl     di, cl
    cmp     [bp+arg_0], di
    jle     short loc_1D54C
    mov     [bp+arg_0], di
loc_1D54C:
    mov     ax, framespersec
    shl     ax, 1
    shl     ax, 1
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_0]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_4]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     si, ax
    mov     word_4617E, 0
    mov     word_46180, 140h
    mov     di, [bp+arg_2]
    add     di, [bp+arg_4]
    mov     ax, di
    sub     ax, si
    mov     word_46182, ax
    mov     word_46184, di
    push    di
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    word_463E2
    call    clear_sprite1_color
    add     sp, 2
    mov     ax, 0AA0Eh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_1D534 endp
sub_1D5B4 proc far
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = dword ptr -8
    var_4 = dword ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 1Ah
    push    di
    push    si
    mov     ax, offset aCrak; "crak"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_shape_alt
    add     sp, 6
    mov     word ptr [bp+var_8], ax
    mov     word ptr [bp+var_8+2], dx
    mov     ax, offset aCinf; "cinf"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_shape_alt
    add     sp, 6
    mov     word ptr [bp+var_4], ax
    mov     word ptr [bp+var_4+2], dx
    mov     ax, framespersec
    cwd
    mov     cx, 7
    idiv    cx
    mov     cx, ax
    mov     ax, [bp+arg_0]
    cwd
    idiv    cx
    mov     [bp+var_1A], ax
    les     bx, [bp+var_4]
    mov     di, es:[bx]
    cmp     ax, di
    jl      short loc_1D614
    lea     ax, [di-1]
    mov     [bp+var_1A], ax
loc_1D614:
    mov     di, [bp+var_1A]
    shl     di, 1
    les     bx, [bp+var_4]
    mov     ax, es:[bx+di+2]
    mov     [bp+var_18], ax
    push    si
    mov     di, offset word_4617E
    mov     si, offset rect_unk4
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     si
    sub     si, si
    jmp     loc_1D790
loc_1D636:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    les     bx, [bp+var_8]
    mov     ax, es:[bx+di]
    mov     dx, es:[bx+di+2]
    mov     [bp+var_10], ax
    mov     [bp+var_E], dx
    mov     bx, si
    shl     bx, cl
    mov     di, word ptr [bp+var_8]
    mov     ax, es:[bx+di+4]
    mov     dx, es:[bx+di+6]
    mov     [bp+var_14], ax
    mov     [bp+var_12], dx
    mov     ax, 0C8h ; 'È'
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_4]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_E]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_E], ax
    mov     ax, 0C8h ; 'È'
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_4]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_12]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_12], ax
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_12]
    add     ax, [bp+arg_2]
    dec     ax
    push    ax
    push    [bp+var_14]
    mov     ax, [bp+var_E]
    add     ax, [bp+arg_2]
    dec     ax
    push    ax
    push    [bp+var_10]
    call    sub_2FDDE
    add     sp, 0Ah
    sub     ax, ax
    push    ax
    mov     ax, [bp+var_12]
    add     ax, [bp+arg_2]
    inc     ax
    push    ax
    push    [bp+var_14]
    mov     ax, [bp+var_E]
    add     ax, [bp+arg_2]
    inc     ax
    push    ax
    push    [bp+var_10]
    call    sub_2FDDE
    add     sp, 0Ah
    push    word_407CA
    mov     ax, [bp+var_12]
    add     ax, [bp+arg_2]
    push    ax
    push    [bp+var_14]
    mov     ax, [bp+var_E]
    add     ax, [bp+arg_2]
    push    ax
    push    [bp+var_10]
    call    sub_2FDDE
    add     sp, 0Ah
    cmp     word_44984, 0
    jnz     short loc_1D70F
    jmp     loc_1D78F
loc_1D70F:
    mov     ax, [bp+var_10]
    mov     [bp+var_C], ax
    mov     ax, [bp+var_E]
    add     ax, [bp+arg_2]
    dec     ax
    mov     [bp+var_A], ax
    mov     ax, 0AA0Eh
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_2637A
    add     sp, 4
    mov     ax, [bp+var_14]
    mov     [bp+var_C], ax
    mov     ax, [bp+var_12]
    add     ax, [bp+arg_2]
    inc     ax
    mov     [bp+var_A], ax
    mov     ax, 0AA0Eh
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_2637A
    add     sp, 4
    mov     ax, [bp+var_10]
    mov     [bp+var_C], ax
    mov     ax, [bp+var_E]
    add     ax, [bp+arg_2]
    inc     ax
    mov     [bp+var_A], ax
    mov     ax, 0AA0Eh
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_2637A
    add     sp, 4
    mov     ax, [bp+var_14]
    mov     [bp+var_C], ax
    mov     ax, [bp+var_12]
    add     ax, [bp+arg_2]
    dec     ax
    mov     [bp+var_A], ax
    mov     ax, 0AA0Eh
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_2637A
    add     sp, 4
loc_1D78F:
    inc     si
loc_1D790:
    cmp     [bp+var_18], si
    jle     short loc_1D798
    jmp     loc_1D636
loc_1D798:
    mov     ax, 0AA0Eh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_1D5B4 endp
load_skybox proc far
    var_4 = word ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    si
    test    [bp+arg_0], 8
    jz      short loc_1D7B6
smart
smart
    and     [bp+arg_0], 7
nosmart
    jmp     loc_1D88E
loc_1D7B6:
    cmp     byte_3B8F6, 0
    jz      short loc_1D7C8
    mov     al, byte_46167
    cmp     [bp+arg_0], al
    jnz     short loc_1D7C8
    jmp     loc_1D8AF
loc_1D7C8:
    push    cs
    call near ptr unload_skybox
    mov     al, [bp+arg_0]
    mov     byte_46167, al
    mov     byte_3B8F6, 1
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    add     ax, offset aDesert; "desert"
    push    ax
    call    load_2dshape_fatal_thunk
    add     sp, 2
    mov     skybox_res_ofs, ax
    mov     skybox_res_seg, dx
    mov     ax, offset skyboxes
    push    ax
    mov     ax, offset aScensce2sce3sce4; "scensce2sce3sce4"
    push    ax
    push    dx
    push    skybox_res_ofs
    call    locate_many_resources
    add     sp, 8
    les     bx, skyboxes
    mov     ax, es:[bx+2]
    mov     skybox_ptr1, ax
    les     bx, skyboxes+4
    mov     ax, es:[bx+2]
    mov     skybox_ptr2, ax
    les     bx, skyboxes+8
    mov     ax, es:[bx+2]
    mov     skybox_ptr3, ax
    les     bx, skyboxes+0Ch
    mov     ax, es:[bx+2]
    mov     skybox_ptr4, ax
    mov     si, skybox_ptr1
    mov     ax, si
    cmp     ax, skybox_ptr2
    jbe     short loc_1D846
    mov     si, skybox_ptr2
loc_1D846:
    mov     ax, si
    cmp     ax, skybox_ptr3
    jbe     short loc_1D852
    mov     si, skybox_ptr3
loc_1D852:
    mov     ax, si
    cmp     ax, skybox_ptr4
    jbe     short loc_1D85E
    mov     si, skybox_ptr4
loc_1D85E:
    mov     skybox_current, si
    mov     si, skybox_ptr1
    mov     ax, si
    cmp     ax, skybox_ptr2
    jnb     short loc_1D872
    mov     si, skybox_ptr2
loc_1D872:
    mov     ax, si
    cmp     ax, skybox_ptr3
    jnb     short loc_1D87E
    mov     si, skybox_ptr3
loc_1D87E:
    mov     ax, si
    cmp     ax, skybox_ptr4
    jnb     short loc_1D88A
    mov     si, skybox_ptr4
loc_1D88A:
    mov     word_454CE, si
loc_1D88E:
    mov     ax, word_40B0A
    mov     [bp+var_4], ax
    mov     bx, ax
    mov     ax, [bx+22h]
    mov     skybox_unk1, ax
    mov     ax, [bx+20h]
    mov     skybox_unk2, ax
    mov     ax, [bx+0C8h]
    mov     word_463E2, ax
    mov     ax, word_407CA
    mov     word_44D60, ax
loc_1D8AF:
    pop     si
    mov     sp, bp
    pop     bp
    retf
load_skybox endp
unload_skybox proc far

    cmp     byte_3B8F6, 0
    jz      short loc_1D8CB
    push    skybox_res_seg
    push    skybox_res_ofs
    call    mmgr_free
    add     sp, 4
loc_1D8CB:
    mov     byte_3B8F6, 0
    retf
    ; align 2
    db 144
unload_skybox endp
load_sdgame2_shapes proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    mov     ax, 0A0Dh
    push    ax              ; char *
    mov     ax, 8
    push    ax              ; int
    call    file_load_resource
    add     sp, 4
    mov     word_463D8, ax
    mov     word_463DA, dx
    mov     ax, 0A64Ch
    push    ax
    mov     ax, 0A15h
    push    ax
    push    dx
    push    word_463D8
    call    locate_many_resources
    add     sp, 8
    sub     si, si
loc_1D908:
    mov     bx, si
    shl     bx, 1
    mov     di, si
    shl     di, 1
    shl     di, 1
    les     di, [di-59B4h]
    mov     ax, es:[di]
    mov     [bx-613Ah], ax
    inc     si
    cmp     si, 3
    jl      short loc_1D908
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
load_sdgame2_shapes endp
sub_1D92A proc far

    push    word_463DA
    push    word_463D8
    call    mmgr_free
    add     sp, 4
    retf
    ; align 2
    db 144
sub_1D92A endp
setup_intro proc far
    var_5D4 = word ptr -1492
    var_5D2 = word ptr -1490
    var_5D0 = byte ptr -1488
    var_440 = word ptr -1088
    var_43E = byte ptr -1086
    var_2AE = word ptr -686
    var_2AC = word ptr -684
    var_2AA = word ptr -682
    var_2A8 = word ptr -680
    var_2A6 = word ptr -678
    var_2A4 = word ptr -676
    var_2A2 = byte ptr -674
    var_2A0 = word ptr -672
    var_29E = word ptr -670
    var_29C = word ptr -668
    var_29A = word ptr -666
    var_298 = word ptr -664
    var_296 = word ptr -662
    var_42 = word ptr -66
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = byte ptr -56
    var_36 = word ptr -54
    var_34 = byte ptr -52
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = byte ptr -22
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 5D4h
    push    di
    push    si
    mov     [bp+var_38], 0
    mov     ax, offset aTitle; "title"
    push    ax
    call    file_load_3dres
    add     sp, 2
    mov     [bp+var_2AE], ax
    mov     [bp+var_2AC], dx
    lea     ax, [bp+var_22]
    push    ax
    mov     ax, offset aLogolog2brav; "logolog2brav"
    push    ax
    push    dx
    push    [bp+var_2AE]
    call    locate_many_resources
    add     sp, 8
    mov     ax, offset logoshape
    push    ax
    push    [bp+var_20]
    push    [bp+var_22]
    call    shape3d_init_shape
    add     sp, 6
    mov     ax, offset logo2shape
    push    ax
    push    [bp+var_1C]
    push    [bp+var_1E]
    call    shape3d_init_shape
    add     sp, 6
    mov     ax, offset bravshape
    push    ax
    push    [bp+var_18]
    push    [bp+var_1A]
    call    shape3d_init_shape
    add     sp, 6
    cmp     byte_46436, 0
    jnz     short loc_1D9CA
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
loc_1D9CA:
    mov     [bp+var_42], 0
loc_1D9CF:
    call    get_kevinrandom
    mov     cl, 7
    shl     ax, cl
    sub     ax, 4000h
    mov     si, [bp+var_42]
    mov     cx, si
    shl     si, 1
    add     si, cx
    shl     si, 1
    mov     [bp+si+var_29A], ax
    call    get_kevinrandom
    mov     cl, 7
    shl     ax, cl
    sub     ax, 1388h
    neg     ax
    mov     si, [bp+var_42]
    mov     cx, si
    shl     si, 1
    add     si, cx
    shl     si, 1
    mov     [bp+si+var_298], ax
    call    get_kevinrandom
    mov     cl, 7
    shl     ax, cl
    sub     ax, 4000h
    mov     si, [bp+var_42]
    mov     cx, si
    shl     si, 1
    add     si, cx
    shl     si, 1
    mov     [bp+si+var_296], ax
    inc     [bp+var_42]
    cmp     [bp+var_42], 64h ; 'd'
    jl      short loc_1D9CF
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    mov     ax, 28h ; '('
    push    ax
    push    ax
    call    sub_322F3
    add     sp, 8
    mov     [bp+var_3E], 400h
    mov     [bp+var_3A], 400h
    mov     [bp+var_3C], 12Ch
    mov     [bp+var_E], 0
    mov     [bp+var_5D4], 0
    mov     ax, offset aCarcoun_0; "carcoun"
    push    ax
    call    file_load_resfile
    add     sp, 2
    mov     [bp+var_A], ax
    mov     [bp+var_8], dx
    mov     ax, 1
    push    ax
    push    dx
    push    [bp+var_A]
    call    setup_aero_trackdata
    add     sp, 6
    push    [bp+var_8]
    push    [bp+var_A]
    call    unload_resource
    add     sp, 4
    call    init_plantrak
    call    timer_get_delta
    mov     [bp+var_2AA], 0
    mov     [bp+var_440], 0
    mov     ax, word_44CEA
    mov     word_44984, ax
    mov     rect_unk, 0
    mov     rect_unk+2, 140h
    mov     rect_unk+4, 0
    mov     rect_unk+6, 0C8h ; 'È'
    mov     di, offset rect_unk2
    mov     si, offset rect_unk
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     di, offset rect_unk3
    mov     si, offset rect_unk
    movsw
    movsw
    movsw
    movsw
    mov     [bp+var_36], 0
    mov     [bp+var_2A2], 1
loc_1DADE:
    call    timer_get_delta
    mov     [bp+var_40], ax
    add     word_44DCC, ax
    jmp     loc_1DB8A
    ; align 2
    db 144
loc_1DAEE:
    mov     ax, word_4499C
    sub     word_44DCC, ax
    call    do_opponent_op
    mov     [bp+var_2A2], 1
    mov     ax, framespersec
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    inc     [bp+var_5D4]
    cmp     ax, [bp+var_5D4]
    jge     short loc_1DB8A
    mov     [bp+var_E], 1
    add     [bp+var_3C], 14h
    sub     [bp+var_3A], 5
    mov     ax, [bp+var_3E]
    sub     ax, 400h
    mov     [bp+var_42], ax
    or      ax, ax
    jge     short loc_1DB38
    neg     ax
    jmp     short loc_1DB3B
    ; align 4
    db 144
    db 144
loc_1DB38:
    mov     ax, [bp+var_42]
loc_1DB3B:
    cmp     ax, 0Ah
    jge     short loc_1DB48
    mov     [bp+var_3E], 400h
    jmp     short loc_1DB5E
    ; align 2
    db 144
loc_1DB48:
    cmp     [bp+var_42], 0
    jle     short loc_1DB54
    sub     [bp+var_3E], 0Ah
    jmp     short loc_1DB5E
loc_1DB54:
    cmp     [bp+var_42], 0
    jge     short loc_1DB5E
    add     [bp+var_3E], 0Ah
loc_1DB5E:
    cmp     [bp+var_6], 400h
    jle     short loc_1DB6A
    dec     [bp+var_6]
    jmp     short loc_1DB74
loc_1DB6A:
    cmp     [bp+var_6], 400h
    jge     short loc_1DB74
    inc     [bp+var_6]
loc_1DB74:
    cmp     [bp+var_2], 400h
    jle     short loc_1DB80
    dec     [bp+var_2]
    jmp     short loc_1DB8A
loc_1DB80:
    cmp     [bp+var_2], 400h
    jge     short loc_1DB8A
    inc     [bp+var_2]
loc_1DB8A:
    mov     ax, word_4499C
    cmp     word_44DCC, ax
    jle     short loc_1DB96
    jmp     loc_1DAEE
loc_1DB96:
    cmp     [bp+var_2A2], 0
    jnz     short loc_1DBA0
    jmp     loc_1DE19
loc_1DBA0:
    mov     [bp+var_2A2], 0
    cmp     byte_46436, 0
    jz      short loc_1DBB4
    call    setup_mcgawnd2
    jmp     short loc_1DBB9
    ; align 2
    db 144
loc_1DBB4:
    call    sprite_copy_wnd_to_1
loc_1DBB9:
    mov     [bp+var_2C], 0FFFFh
    mov     [bp+var_2A6], 1
    mov     ax, word ptr state.opponentstate.car_longvec1.long_x
    mov     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1DBCD:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DBCD
    mov     [bp+var_2A0], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_y
    mov     dx, word ptr state.opponentstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1DBE2:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DBE2
    mov     [bp+var_29E], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_z
    mov     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1DBF7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DBF7
    mov     [bp+var_29C], ax
    mov     ax, framespersec
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    cmp     ax, [bp+var_5D4]
    jle     short loc_1DC44
    mov     [bp+var_2A6], 0
    mov     ax, state.opponentstate.car_rotate.x
smart
    and     ah, 3
nosmart
    mov     [bp+var_2C], ax
    mov     [bp+var_2A4], 0
    mov     ax, [bp+var_2A0]
    mov     [bp+var_3E], ax
    mov     ax, [bp+var_29E]
    add     ax, 14h
    mov     [bp+var_3C], ax
    mov     ax, [bp+var_29C]
    mov     [bp+var_3A], ax
    jmp     short loc_1DC7D
    ; align 2
    db 144
loc_1DC44:
    mov     ax, framespersec
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    cmp     ax, [bp+var_5D4]
    jle     short loc_1DC7D
    mov     [bp+var_3E], 400h
    mov     [bp+var_3A], 400h
    mov     [bp+var_3C], 5Ah ; 'Z'
    mov     ax, [bp+var_2A0]
    mov     [bp+var_6], ax
    mov     ax, [bp+var_29E]
    mov     [bp+var_4], ax
    mov     ax, [bp+var_29C]
    mov     [bp+var_2], ax
loc_1DC7D:
    cmp     [bp+var_2C], 0FFFFh
    jnz     short loc_1DCD1
    mov     ax, [bp+var_2]
    sub     ax, [bp+var_3A]
    push    ax
    mov     ax, [bp+var_6]
    sub     ax, [bp+var_3E]
    push    ax
    call    sub_2EA4E
    add     sp, 4
    neg     ax
smart
    and     ah, 3
nosmart
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_2]
    sub     ax, [bp+var_3A]
    push    ax
    mov     ax, [bp+var_6]
    sub     ax, [bp+var_3E]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_C], ax
    push    ax
    mov     ax, [bp+var_4]
    sub     ax, [bp+var_3C]
    push    ax
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     [bp+var_2A4], ax
loc_1DCD1:
    cmp     word_44984, 0
    jz      short loc_1DCFC
    cmp     [bp+var_36], 0
    jnz     short loc_1DCEC
    lea     ax, [bp+var_43E]
    mov     [bp+var_2A8], ax
    lea     ax, [bp+var_2AA]
    jmp     short loc_1DCF8
loc_1DCEC:
    lea     ax, [bp+var_5D0]
    mov     [bp+var_2A8], ax
    lea     ax, [bp+var_440]
loc_1DCF8:
    mov     [bp+var_5D2], ax
loc_1DCFC:
    lea     ax, [bp+var_34]
    push    ax
    lea     ax, [bp+var_16]
    push    ax
    mov     bx, [bp+var_36]
    mov     cl, 3
    shl     bx, cl
    push    (rect_unk+6)[bx]
    push    (rect_unk+4)[bx]
    push    (rect_unk+2)[bx]
    push    rect_unk[bx]
    push    [bp+var_5D2]
    push    [bp+var_2A8]
    lea     ax, [bp+var_29A]
    push    ax
    push    [bp+var_E]
    push    [bp+var_2A6]
    push    [bp+var_2A4]
    push    [bp+var_2C]
    push    [bp+var_3A]
    push    [bp+var_3C]
    push    [bp+var_3E]
    push    cs
    call near ptr sub_1DEA6
    add     sp, 20h
    cmp     byte_46436, 0
    jz      short loc_1DD7E
    call    sub_28DB6
    call    setup_mcgawnd1
    call    sub_28D9E
    cmp     word_44984, 0
    jz      short loc_1DD77
    mov     bx, [bp+var_36]
    mov     cl, 3
    shl     bx, cl
    lea     di, rect_unk[bx]
    lea     si, [bp+var_16]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
loc_1DD77:
    xor     byte ptr [bp+var_36], 1
    jmp     loc_1DE19
loc_1DD7E:
    call    sprite_copy_2_to_1_2
    cmp     word_44984, 0
    jz      short loc_1DDFC
    lea     ax, [bp+var_2A]
    push    ax
    mov     ax, offset rect_unk6
    push    ax
    lea     ax, [bp+var_34]
    push    ax
    call    sub_26572
    add     sp, 6
    mov     ax, 0A1E2h
    push    ax
    lea     ax, [bp+var_2A]
    push    ax
    call    sub_265EC
    add     sp, 4
    or      al, al
    jnz     short loc_1DE19
    push    [bp+var_24]
    push    [bp+var_26]
    push    [bp+var_28]
    push    [bp+var_2A]
    call    set_sprite1_size
    add     sp, 8
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    call    sub_28D9E
    mov     di, 9294h
    lea     si, [bp+var_16]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     di, offset rect_unk6
    lea     si, [bp+var_34]
    movsw
    movsw
    movsw
    movsw
    jmp     short loc_1DE19
    ; align 2
    db 144
loc_1DDFC:
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    call    sub_28D9E
loc_1DE19:
    push    [bp+var_40]
    call    input_do_checking
    add     sp, 2
    or      ax, ax
    jz      short loc_1DE2E
    mov     [bp+var_38], 1
    jmp     short loc_1DE3E
loc_1DE2E:
    mov     ax, 17h
    imul    framespersec
    cmp     ax, [bp+var_5D4]
    jle     short loc_1DE3E
    jmp     loc_1DADE
loc_1DE3E:
    cmp     byte_46436, 0
    jz      short loc_1DE7C
    call    get_0
    or      ax, ax
    jz      short loc_1DE8C
    call    setup_mcgawnd2
    sub     ax, ax
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    call    sub_35C4E
    add     sp, 0Ah
    call    sub_28DB6
    call    setup_mcgawnd1
    call    sub_28D9E
    jmp     short loc_1DE8C
    ; align 2
    db 144
loc_1DE7C:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
loc_1DE8C:
    push    [bp+var_2AC]
    push    [bp+var_2AE]
    call    mmgr_free
    add     sp, 4
    mov     al, [bp+var_38]
    cbw
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
setup_intro endp
sub_1DEA6 proc far
    var_42 = word ptr -66
    var_40 = byte ptr -64
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_36 = word ptr -54
    var_34 = word ptr -52
    var_32 = word ptr -50
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = byte ptr -36
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_9 = byte ptr -9
    var_8 = byte ptr -8
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = word ptr 16
    arg_C = word ptr 18
    arg_E = word ptr 20
    arg_10 = word ptr 22
    arg_12 = word ptr 24
    arg_14 = byte ptr 26
    arg_1C = word ptr 34
    arg_1E = word ptr 36

    push    bp
    mov     bp, sp
    sub     sp, 42h
    push    di
    push    si
    push    si
    push    di
    lea     di, [bp+var_8]
    mov     si, 3394h
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    sub     ax, ax
    push    ax
    mov     ax, 0A46h
loc_1DEC4:
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_8]
    sub     ax, ax
    push    ax
    call    select_rect_rotate
    add     sp, 0Ah
    cmp     [bp+arg_C], 0
    jz      short loc_1DEE4
    mov     [bp+var_16], 8BC0h
    jmp     short loc_1DEE9
    ; align 2
    db 144
loc_1DEE4:
    mov     [bp+var_16], 8BFCh
loc_1DEE9:
    mov     ax, 400h
    sub     ax, [bp+arg_0]
    mov     [bp+var_1C], ax
    mov     ax, [bp+arg_2]
    neg     ax
    mov     [bp+var_1A], ax
    mov     ax, 400h
    sub     ax, [bp+arg_4]
    mov     [bp+var_18], ax
    cmp     word_44984, 0
    jz      short loc_1DF16
    lea     ax, [bp+var_8]
    mov     [bp+var_14], ax
    mov     [bp+var_A], 0Ch
    jmp     short loc_1DF1A
loc_1DF16:
    mov     [bp+var_A], 4
loc_1DF1A:
    mov     [bp+var_12], 0
    mov     [bp+var_10], 0
    mov     [bp+var_E], 0
    mov     [bp+var_C], 400h
    mov     [bp+var_9], 0
    lea     ax, [bp+var_1C]
    push    ax
    call    transformed_shape_op
    add     sp, 2
    cmp     [bp+arg_A], 0
    jnz     short loc_1DF47
    jmp     loc_1DFCF
loc_1DF47:
    mov     ax, word ptr state.opponentstate.car_longvec1.long_x
    mov     dx, word ptr state.opponentstate.car_longvec1.long_x+2
    mov     cl, 6
loc_1DF50:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DF50
    sub     ax, [bp+arg_0]
    mov     [bp+var_1C], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_y
    mov     dx, word ptr state.opponentstate.car_longvec1.long_y+2
    mov     cl, 6
loc_1DF67:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DF67
    sub     ax, [bp+arg_2]
    mov     [bp+var_1A], ax
    mov     ax, word ptr state.opponentstate.car_longvec1.long_z
    mov     dx, word ptr state.opponentstate.car_longvec1.long_z+2
    mov     cl, 6
loc_1DF7E:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1DF7E
    sub     ax, [bp+arg_4]
    mov     [bp+var_18], ax
    mov     [bp+var_16], 9580h
    cmp     word_44984, 0
    jz      short loc_1DFA4
    lea     ax, [bp+var_8]
    mov     [bp+var_14], ax
    mov     [bp+var_A], 0Ch
    jmp     short loc_1DFA8
loc_1DFA4:
    mov     [bp+var_A], 4
loc_1DFA8:
    mov     [bp+var_12], 0
    mov     [bp+var_10], 0
    mov     ax, state.opponentstate.car_rotate.x
    neg     ax
    mov     [bp+var_E], ax
    mov     [bp+var_C], 400h
    mov     [bp+var_9], 0
    lea     ax, [bp+var_1C]
    push    ax
    call    transformed_shape_op
    add     sp, 2
loc_1DFCF:
    cmp     word_44984, 0
    jnz     short loc_1DFD9
    jmp     loc_1E06C
loc_1DFD9:
    mov     bx, [bp+arg_12]
    cmp     word ptr [bx], 0
    jz      short loc_1E013
    sub     si, si
    jmp     short loc_1E00C
    ; align 2
    db 144
loc_1DFE6:
    mov     bx, [bp+arg_10]
    mov     ax, si
    shl     ax, 1
    shl     ax, 1
    add     bx, ax
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     [bp+var_3A], ax
    mov     [bp+var_38], dx
    sub     ax, ax
    push    ax
    push    dx
    push    [bp+var_3A]
    call    sub_35B26
    add     sp, 6
    inc     si
loc_1E00C:
    mov     bx, [bp+arg_12]
    cmp     [bx], si
    jg      short loc_1DFE6
loc_1E013:
    lea     ax, [bp+var_2C]
    push    ax
    lea     ax, [bp+arg_14]
    push    ax
    push    [bp+arg_1C]
    call    sub_26572
    add     sp, 6
    mov     ax, 0A1E2h
    push    ax
    lea     ax, [bp+var_2C]
    push    ax
    call    sub_265EC
    add     sp, 4
    or      al, al
    jnz     short loc_1E059
    push    [bp+var_26]
    push    [bp+var_28]
    push    [bp+var_2A]
    push    [bp+var_2C]
    call    set_sprite1_size
    add     sp, 8
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
loc_1E059:
    push    si
    push    di
    lea     di, [bp+var_24]
    lea     si, [bp+var_8]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    jmp     short loc_1E08F
    ; align 2
    db 144
loc_1E06C:
    push    word_3C1BC
    push    word_3C1BA
    push    word_3C1B8
    push    word_3C1B6
    call    set_sprite1_size
    add     sp, 8
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
loc_1E08F:
    push    word_3C1BC
    push    word_3C1BA
    push    word_3C1B8
    push    word_3C1B6
    call    set_sprite1_size
    add     sp, 8
    sub     di, di
    sub     si, si
loc_1E0AB:
    mov     ax, [bp+arg_E]
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    shl     cx, 1
    add     ax, cx
    mov     [bp+var_42], ax
    mov     bx, ax
    mov     ax, [bx]
    sub     ax, [bp+arg_0]
    mov     [bp+var_36], ax
    mov     ax, [bx+2]
    sub     ax, [bp+arg_2]
    mov     [bp+var_34], ax
    mov     ax, [bx+4]
    sub     ax, [bp+arg_4]
    mov     [bp+var_32], ax
    lea     ax, [bp+var_40]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    lea     ax, [bp+var_36]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_3C], 0C8h ; 'È'
    jle     short loc_1E157
    lea     ax, [bp+var_3A]
    push    ax
    lea     ax, [bp+var_40]
    push    ax
    call    sub_323D9
    add     sp, 4
    push    word_3C1BE
    push    [bp+var_38]
    push    [bp+var_3A]
    call    sub_35B26
    add     sp, 6
    cmp     word_44984, 0
    jz      short loc_1E144
    mov     bx, [bp+arg_10]
    mov     ax, di
    inc     di
    shl     ax, 1
    shl     ax, 1
    add     bx, ax
    mov     ax, [bp+var_3A]
    mov     dx, [bp+var_38]
    mov     [bx], ax
    mov     [bx+2], dx
    lea     ax, [bp+var_24]
    push    ax
    lea     ax, [bp+var_3A]
    push    ax
    call    sub_2637A
    add     sp, 4
loc_1E144:
    inc     word_3C1BE
    mov     ax, word_407CC
    cmp     word_3C1BE, ax
    jnz     short loc_1E157
    mov     word_3C1BE, 1
loc_1E157:
    inc     si
    cmp     si, 64h ; 'd'
    jge     short loc_1E160
    jmp     loc_1E0AB
loc_1E160:
    cmp     word_44984, 0
    jz      short loc_1E16C
    mov     bx, [bp+arg_12]
    mov     [bx], di
loc_1E16C:
    call    sub_25FF6
    cmp     word_44984, 0
    jz      short loc_1E19A
    mov     bx, [bp+arg_1C]
    push    si
    push    di
    mov     di, bx
    lea     si, [bp+var_8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [bp+arg_1E]
    push    si
    push    di
    mov     di, bx
    lea     si, [bp+var_24]
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
loc_1E19A:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1DEA6 endp
seg003 ends
end
