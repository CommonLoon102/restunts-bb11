.model medium
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
    include seg001.inc
    include seg002.inc
    include seg003.inc
    include seg004.inc
    include seg005.inc
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
seg006 segment byte public 'STUNTSC' use16
    assume cs:seg006
    assume es:nothing, ss:nothing, ds:dseg
    public init_polyinfo
    public sub_24DC2
    public sub_24DE6
    public sub_24E06
    public sub_24E9E
    public sub_25E24
    public sub_25EE2
    public sub_25F2E
    public sub_25FF6
    public mat_rot_zxy
    public off_26362
    public sub_2637A
    public sub_263C6
    public sub_26500
    public sub_26572
    public sub_265EC
    public sub_26670
    public sub_2695E
    public sub_2699C
    public sub_269D0
    public sub_26A52
    public sub_26B4A
init_polyinfo proc far

    mov     ax, 28A0h
    cwd
    push    dx
    push    ax
    mov     ax, 32E8h
    push    ax
    call    alloc_resbytes
    add     sp, 6
    mov     word_40ED2, ax
    mov     word_40ED4, dx
    sub     ax, ax
    push    ax
    mov     ax, 60CEh
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     ax, 100h
    push    ax
    mov     ax, 572Eh
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     ax, 200h
    push    ax
    mov     ax, 571Ch
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     ax, 300h
    push    ax
    mov     ax, 5748h
    push    ax
    call    mat_rot_y
    add     sp, 4
    push    cs
    call    near ptr sub_26500
    retf
    ; align 2
    db 144
init_polyinfo endp
sub_24DC2 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    mov     ax, [bp+arg_0]
    mov     word_45632, ax
    mov     ax, [bp+arg_2]
    mov     word_459D6, ax
    mov     ax, [bp+arg_4]
    mov     word_45D1E, ax
    mov     ax, [bp+arg_6]
    mov     word_45D92, ax
    mov     ax, [bp+arg_8]
    mov     word_459FC, ax
    pop     bp
    retf
    ; align 2
    db 144
sub_24DC2 endp
sub_24DE6 proc far

    mov     word_442E6, 0
    mov     word_41850, 0
    mov     word_40ECE, 0
    mov     word_411F6, 0FFFFh
    mov     word_443F2, 190h
    retf
    ; align 2
    db 144
sub_24DE6 endp
sub_24E06 proc far
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 0Eh
    push    di
    push    si
    mov     ax, 1
    push    ax
    push    [bp+arg_4]
    push    [bp+arg_2]
    push    [bp+arg_0]
    push    cs
    call    near ptr mat_rot_zxy
    add     sp, 8
    mov     di, 0ACDCh
    mov     si, ax
    push    ds
    pop     es
    mov     cx, 9
    repne movsw
    push    cs
    call    near ptr sub_24DE6
    mov     ax, [bp+arg_6]
    mov     di, 5740h
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     ax, [bp+arg_8]
    mov     word_40ED0, ax
    sub     ax, ax
    push    ax
    mov     ax, [bp+arg_4]
    neg     ax
    push    ax
    mov     ax, [bp+arg_2]
    neg     ax
    push    ax
    mov     ax, [bp+arg_0]
    neg     ax
    push    ax
    push    cs
    call    near ptr mat_rot_zxy
    add     sp, 8
    mov     [bp+var_8], ax
    mov     [bp+var_A], 2710h
    mov     [bp+var_C], 0
    mov     [bp+var_E], 0
    lea     ax, [bp+var_6]
    push    ax
    push    [bp+var_8]
    lea     ax, [bp+var_E]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_2]
    push    [bp+var_6]
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_24E06 endp
sub_24E9E proc far
    var_B7C = word ptr -2940
    var_B7A = word ptr -2938
    var_B78 = dword ptr -2936
    var_B74 = word ptr -2932
    var_B72 = word ptr -2930
    var_B70 = word ptr -2928
    var_574 = word ptr -1396
    var_572 = word ptr -1394
    var_570 = word ptr -1392
    var_56E = dword ptr -1390
    var_56A = word ptr -1386
    var_568 = word ptr -1384
    var_566 = word ptr -1382
    var_564 = word ptr -1380
    var_562 = byte ptr -1378
    var_462 = word ptr -1122
    var_460 = word ptr -1120
    var_45E = word ptr -1118
    var_45C = word ptr -1116
    var_45A = word ptr -1114
    var_458 = word ptr -1112
    var_456 = word ptr -1110
    var_454 = word ptr -1108
    var_450 = word ptr -1104
    var_44E = word ptr -1102
    var_44A = byte ptr -1098
    var_448 = word ptr -1096
    var_446 = byte ptr -1094
    var_434 = dword ptr -1076
    var_430 = dword ptr -1072
    var_42C = word ptr -1068
    var_42A = byte ptr -1066
    var_418 = word ptr -1048
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 0B7Ch
    push    di
    push    si
    cmp     word_40ECE, 0
    jz      short loc_24EB8
loc_24EAE:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_24EB8:
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx]
    mov     word_4187C, ax
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx+0Ah]
    mov     dx, [bx+0Ch]
    mov     word ptr dword_41870, ax
    mov     word ptr dword_41870+2, dx
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx+2]
    mov     dx, [bx+4]
    mov     word ptr dword_4187E, ax
    mov     word ptr dword_4187E+2, dx
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     al, [bx+8]
    cbw
    mov     word_41852, ax
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx+0Eh]
    mov     dx, [bx+10h]
    mov     word ptr [bp+var_B78], ax
    mov     word ptr [bp+var_B78+2], dx
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx+12h]
    mov     dx, [bx+14h]
    mov     word ptr [bp+var_434], ax
    mov     word ptr [bp+var_434+2], dx
    mov     bx, [bp+arg_0]
    mov     al, [bx+13h]
    mov     byte_4187A, al
    cmp     byte ptr word_41852, al
    ja      short loc_24F32
    mov     byte_4187A, 0
loc_24F32:
    mov     al, [bx+12h]
    mov     byte_4186E, al
    test    byte_4186E, 8
    jz      short loc_24F45
    mov     ax, [bx+8]
    mov     word_41878, ax
loc_24F45:
    sub     si, si
    jmp     short loc_24F50
    ; align 2
    db 144
loc_24F4A:
    mov     [bp+si+var_562], 0FFh
    inc     si
loc_24F50:
    mov     ax, si
    cmp     ax, word_4187C
    jb      short loc_24F4A
    test    byte_4186E, 2
    jz      short loc_24FB6
    sub     ax, ax
    push    ax
    mov     bx, [bp+arg_0]
    push    word ptr [bx+0Eh]
    push    word ptr [bx+0Ch]
    push    word ptr [bx+0Ah]
    push    cs
    call    near ptr mat_rot_zxy
    add     sp, 8
    mov     [bp+var_42C], ax
    lea     ax, [bp+var_42A]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    push    [bp+var_42C]
    call    mat_multiply
    add     sp, 6
    mov     ax, [bp+arg_0]
    push    si
    push    di
    lea     di, [bp+var_14]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
loc_24F9F:
    mov     [bp+var_45C], 0FFFFh
    mov     [bp+var_45A], 0FFFFh
    sub     ax, ax
    mov     [bp+var_8], ax
    mov     [bp+var_A], ax
    jmp     loc_250A3
loc_24FB6:
    sub     ax, ax
    push    ax
    mov     bx, [bp+arg_0]
    push    word ptr [bx+0Eh]
    push    word ptr [bx+0Ch]
    push    word ptr [bx+0Ah]
    push    cs
    call    near ptr mat_rot_zxy
    add     sp, 8
    mov     [bp+var_42C], ax
    lea     ax, [bp+var_14]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    push    [bp+arg_0]
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_42A]
    push    ax
    mov     ax, 0ACDCh
    push    ax
    push    [bp+var_42C]
    call    mat_multiply
    add     sp, 6
    lea     ax, [bp+var_446]
    push    ax
    lea     ax, [bp+var_42A]
    push    ax
    call    mat_copy
    add     sp, 4
    mov     [bp+var_458], 0
    mov     [bp+var_456], 0
    mov     [bp+var_454], 1000h
    lea     ax, [bp+var_56A]
    push    ax
    lea     ax, [bp+var_446]
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_568], 0
    jle     short loc_25046
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+2], 0
    jge     short loc_25046
    jmp     loc_24F9F
loc_25046:
    push    [bp+var_14]     ; int
    call    _abs
    add     sp, 2
    mov     bx, [bp+arg_0]
    mov     cx, [bx+10h]
    shl     cx, 1
    cmp     cx, ax
    jle     short loc_25077
    push    [bp+var_10]     ; int
    call    _abs
    add     sp, 2
    mov     bx, [bp+arg_0]
    mov     cx, [bx+10h]
    shl     cx, 1
    cmp     cx, ax
    jle     short loc_25077
    jmp     loc_24F9F
loc_25077:
    lea     ax, [bp+var_56A]
    push    ax
    push    cs
    call    near ptr sub_263C6
    add     sp, 2
    mov     byte_4393D, al
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    mov     ax, [bx+3312h]
    mov     dx, [bx+3314h]
    mov     [bp+var_45C], ax
    mov     [bp+var_45A], dx
    mov     [bp+var_A], ax
    mov     [bp+var_8], dx
loc_250A3:
    mov     ax, word_443F2
    mov     word_4394E, ax
    mov     word_45D98, ax
    mov     word_4554A, 0
    mov     [bp+var_45E], 0
    cmp     word_4187C, 8
    jbe     short loc_250C6
    mov     byte_449F4, 8
    jmp     short loc_250CC
loc_250C6:
    mov     al, byte ptr word_4187C
    mov     byte_449F4, al
loc_250CC:
    cmp     byte_449F4, 4
    jbe     short loc_250E6
    les     bx, dword_4187E
    mov     ax, es:[bx+1Ah]
    cmp     es:[bx+2], ax
    jnz     short loc_250E6
    mov     byte_449F4, 4
loc_250E6:
    mov     [bp+var_44A], 0Fh
    mov     [bp+var_460], 1
    mov     [bp+var_1A], 0
    sub     si, si
    jmp     short loc_2513F
loc_250FA:
    mov     [bp+var_460], 0
    mov     [bp+si+var_562], 0
    mov     bx, si
    shl     bx, 1
    push    word ptr [bx+60E4h]
    lea     ax, [bp+var_56A]
    push    ax
    call    sub_323D9
    add     sp, 4
    cmp     [bp+var_44A], 0
    jz      short loc_25134
    mov     bx, si
    shl     bx, 1
    push    word ptr [bx+60E4h]
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
loc_25134:
    cmp     [bp+var_44A], 0
    jnz     short loc_2513E
    jmp     loc_25220
loc_2513E:
    inc     si
loc_2513F:
    mov     al, byte_449F4
    sub     ah, ah
    cmp     ax, si
    ja      short loc_2514B
    jmp     loc_251F6
loc_2514B:
    mov     bx, si
    shl     bx, 1
    mov     ax, si
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    sub     ax, 416h
    mov     [bx+60E4h], ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_4187E
    mov     dx, word ptr dword_4187E+2
    push    si
    push    di
    lea     di, [bp+var_458]
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
    cmp     word_40ED0, 0
    jz      short loc_25196
    sar     [bp+var_458], 1
    sar     [bp+var_456], 1
    sar     [bp+var_454], 1
loc_25196:
    lea     ax, [bp+var_56A]
    push    ax
    lea     ax, [bp+var_42A]
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_14]
    add     [bp+var_56A], ax
    mov     ax, [bp+var_12]
    add     [bp+var_568], ax
    mov     ax, [bp+var_10]
    add     [bp+var_566], ax
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    push    si
    push    di
    lea     di, [bx-0B6Eh]
    lea     si, [bp+var_56A]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    cmp     [bp+var_566], 0Ch
    jl      short loc_251E9
    jmp     loc_250FA
loc_251E9:
    mov     [bp+si+var_562], 1
    mov     [bp+var_1A], 1
    jmp     loc_2513E
loc_251F6:
    cmp     [bp+var_460], 0
    jnz     short loc_25216
    cmp     [bp+var_1A], 0
    jz      short loc_25216
    push    [bp+var_14]     ; int
    call    _abs
    add     sp, 2
    mov     bx, [bp+arg_0]
    cmp     [bx+10h], ax
    jge     short loc_25220
loc_25216:
    mov     ax, 0FFFFh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_25220:
    mov     bx, [bp+arg_0]
    mov     bx, [bx+6]
    mov     ax, [bx+0Ah]
    mov     dx, [bx+0Ch]
    mov     word ptr dword_41870, ax
    mov     word ptr dword_41870+2, dx
loc_25233:
    les     bx, dword_41870
    mov     bl, es:[bx]
    sub     bh, bh
    mov     al, [bx+32F2h]
    sub     ah, ah
    add     ax, word_41852
    add     ax, word ptr dword_41870
    mov     dx, es
    add     ax, 2
    mov     word_4186A, ax
    mov     word_4186C, dx
    mov     bx, word ptr dword_41870
    mov     al, es:[bx+1]
    sub     ah, ah
    mov     [bp+var_2], ax
    mov     [bp+var_4], 0
    les     bx, [bp+var_B78]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    and     ax, [bp+var_45C]
    and     dx, [bp+var_45A]
    or      dx, ax
    jnz     short loc_25282
    jmp     loc_25801
loc_25282:
    les     bx, dword_41870
    mov     al, es:[bx]
    sub     ah, ah
    mov     [bp+var_418], ax
    mov     bx, ax
    mov     al, [bx+32F2h]
    mov     byte_449F4, al
    mov     al, [bx+3302h]
    mov     [bp+var_6], al
    mov     ax, word_41850
    add     ax, word_40ED2
    mov     dx, word_40ED4
    mov     word ptr dword_40ECA, ax
    mov     word ptr dword_40ECA+2, dx
    mov     bx, word_442E6
    shl     bx, 1
    shl     bx, 1
    mov     [bx+5A88h], ax
    mov     [bx+5A8Ah], dx
    mov     bl, byte_4187A
    sub     bh, bh
    add     bx, word ptr dword_41870
    mov     es, word ptr dword_41870+2
    mov     al, es:[bx+2]
    mov     byte_41882, al
    mov     ax, word_41852
    add     ax, 2
    add     word ptr dword_41870, ax
    mov     [bp+var_44A], 0Fh
    mov     [bp+var_460], 1
    mov     [bp+var_1A], 0
    mov     ax, word ptr dword_41870
    mov     dx, es
    mov     word_41874, ax
    mov     word_41876, dx
    mov     [bp+var_E], 0
    jmp     short loc_25328
    ; align 2
    db 144
loc_25304:
    mov     [bp+var_460], 0
loc_2530A:
    cmp     [bp+var_44A], 0
    jz      short loc_25325
    mov     bx, [bp+var_E]
    shl     bx, 1
    push    word ptr [bx+60E4h]
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
loc_25325:
    inc     [bp+var_E]
loc_25328:
    mov     al, byte_449F4
    sub     ah, ah
    cmp     [bp+var_E], ax
    jb      short loc_25335
    jmp     loc_2542A
loc_25335:
    mov     bx, word_41874
    inc     word_41874
    mov     es, word_41876
    mov     al, es:[bx]
    mov     si, ax
    mov     bx, [bp+var_E]
    shl     bx, 1
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    sub     ax, 416h
    mov     [bx+60E4h], ax
    mov     al, [bp+si+var_562]
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
loc_25370:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_4187E
    mov     dx, word ptr dword_4187E+2
    push    si
    push    di
    lea     di, [bp+var_458]
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
    cmp     word_40ED0, 0
    jz      short loc_253A8
    sar     [bp+var_458], 1
    sar     [bp+var_456], 1
    sar     [bp+var_454], 1
loc_253A8:
    lea     ax, [bp+var_56A]
    push    ax
    lea     ax, [bp+var_42A]
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_14]
    add     [bp+var_56A], ax
    mov     ax, [bp+var_12]
    add     [bp+var_568], ax
    mov     ax, [bp+var_10]
    add     [bp+var_566], ax
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    push    si
    push    di
    lea     di, [bx-0B6Eh]
    lea     si, [bp+var_56A]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    cmp     [bp+var_566], 0Ch
    jge     short loc_25406
    mov     [bp+si+var_562], 1
loc_253FD:
    mov     [bp+var_1A], 1
    jmp     loc_25325
    ; align 2
    db 144
loc_25406:
    mov     [bp+var_460], 0
    mov     [bp+si+var_562], 0
    mov     bx, [bp+var_E]
    shl     bx, 1
    push    word ptr [bx+60E4h]
    lea     ax, [bp+var_56A]
    push    ax
    call    sub_323D9
    add     sp, 4
    jmp     loc_2530A
loc_2542A:
    cmp     [bp+var_460], 0
    jz      short loc_25434
    jmp     loc_25801
loc_25434:
    cmp     [bp+var_44A], 0
    jz      short loc_25444
    cmp     [bp+var_1A], 0
    jnz     short loc_25444
    jmp     loc_25801
loc_25444:
    mov     al, [bp+var_6]
    sub     ah, ah
    or      ax, ax
    jz      short loc_25470
    cmp     ax, 1
    jnz     short loc_25455
    jmp     loc_2585C
loc_25455:
    cmp     ax, 2
    jnz     short loc_2545D
    jmp     loc_25BAA
loc_2545D:
    cmp     ax, 3
    jnz     short loc_25465
    jmp     loc_2598E
loc_25465:
    cmp     ax, 5
    jnz     short loc_2546D
    jmp     loc_25CE0
loc_2546D:
    jmp     loc_25801
loc_25470:
    mov     ax, word ptr dword_40ECA
    mov     dx, word ptr dword_40ECA+2
    add     ax, 6
    mov     word ptr [bp+var_430], ax
    mov     word ptr [bp+var_430+2], dx
    mov     ax, word ptr dword_41870
    mov     dx, word ptr dword_41870+2
    mov     word_41874, ax
    mov     word_41876, dx
    sub     ax, ax
    mov     [bp+var_16], ax
    mov     [bp+var_18], ax
    mov     [bp+var_44A], 0Fh
    cmp     [bp+var_1A], ax
    jnz     short loc_25518
    sub     si, si
    jmp     short loc_254A7
loc_254A6:
    inc     si
loc_254A7:
    mov     al, byte_449F4
    sub     ah, ah
    cmp     ax, si
    ja      short loc_254B3
    jmp     loc_2571A
loc_254B3:
    mov     bx, word_41874
    inc     word_41874
    mov     es, word_41876
    mov     al, es:[bx]
    mov     [bp+var_C], ax
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0B6Ah]
    cwd
    add     [bp+var_18], ax
    adc     [bp+var_16], dx
    mov     ax, si
    shl     ax, 1
    add     ax, 60E4h
    mov     [bp+var_B7A], ax
    mov     bx, ax
    mov     bx, [bx]
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, [bp+var_430]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    cmp     [bp+var_44A], 0
    jz      short loc_25511
    mov     bx, [bp+var_B7A]
    push    word ptr [bx]
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
loc_25511:
    add     word ptr [bp+var_430], 4
    jmp     short loc_254A6
loc_25518:
    mov     [bp+var_E], 0
    mov     bl, byte_449F4
    sub     bh, bh
    add     bx, word ptr dword_41870
    mov     es, word ptr dword_41870+2
    mov     al, es:[bx-1]
    sub     ah, ah
    mov     [bp+var_448], ax
    sub     si, si
    jmp     loc_255EE
loc_2553A:
    mov     bx, [bp+var_448]
    add     bx, bp
    cmp     byte ptr [bx-562h], 0
    jz      short loc_2554A
    jmp     loc_255E6
loc_2554A:
    mov     ax, 0Ch
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    mov     ax, [bp+var_C]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    mov     ax, [bp+var_448]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    call    sub_33014
    add     sp, 8
    lea     ax, [bp+var_574]
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    sub_323D9
    add     sp, 4
    mov     ax, [bp+var_448]
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_B7C], ax
    mov     bx, ax
    mov     ax, [bp+var_574]
    cmp     [bx-416h], ax
    jnz     short loc_255B4
    mov     ax, [bp+var_572]
    cmp     [bx-414h], ax
    jz      short loc_255E6
loc_255B4:
    cmp     [bp+var_44A], 0
    jz      short loc_255CB
    lea     ax, [bp+var_574]
    push    ax
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
loc_255CB:
    les     bx, [bp+var_430]
    mov     ax, [bp+var_574]
    mov     dx, [bp+var_572]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
loc_255DE:
    add     word ptr [bp+var_430], 4
    inc     [bp+var_E]
loc_255E6:
    mov     ax, [bp+var_C]
    mov     [bp+var_448], ax
    inc     si
loc_255EE:
    mov     al, byte_449F4
    sub     ah, ah
    cmp     ax, si
    ja      short loc_255FA
    jmp     loc_25714
loc_255FA:
    mov     bx, word_41874
    inc     word_41874
    mov     es, word_41876
    mov     al, es:[bx]
    mov     [bp+var_C], ax
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_B7A], ax
    mov     bx, ax
    mov     ax, [bx-0B6Ah]
    cwd
    add     [bp+var_18], ax
    adc     [bp+var_16], dx
    mov     bx, cx
    add     bx, bp
    cmp     byte ptr [bx-562h], 0
    jz      short loc_25635
    jmp     loc_2553A
loc_25635:
    mov     bx, [bp+var_448]
    add     bx, bp
    cmp     byte ptr [bx-562h], 0
    jnz     short loc_25645
    jmp     loc_256D7
loc_25645:
    mov     ax, 0Ch
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    mov     ax, [bp+var_448]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    mov     ax, [bp+var_B7A]
    sub     ax, 0B6Eh
    push    ax
    call    sub_33014
    add     sp, 8
    lea     ax, [bp+var_574]
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    sub_323D9
    add     sp, 4
    mov     ax, [bp+var_C]
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_B7C], ax
    mov     bx, ax
    mov     ax, [bp+var_574]
    cmp     [bx-416h], ax
    jnz     short loc_256A5
    mov     ax, [bp+var_572]
    cmp     [bx-414h], ax
    jz      short loc_256D7
loc_256A5:
    cmp     [bp+var_44A], 0
    jz      short loc_256BC
    lea     ax, [bp+var_574]
    push    ax
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
loc_256BC:
    les     bx, [bp+var_430]
    mov     ax, [bp+var_574]
    mov     dx, [bp+var_572]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    add     word ptr [bp+var_430], 4
    inc     [bp+var_E]
loc_256D7:
    mov     ax, si
    shl     ax, 1
    add     ax, 60E4h
    mov     [bp+var_B7C], ax
    mov     bx, ax
    mov     bx, [bx]
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, [bp+var_430]
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    cmp     [bp+var_44A], 0
    jnz     short loc_25700
    jmp     loc_255DE
loc_25700:
    mov     bx, [bp+var_B7C]
    push    word ptr [bx]
    push    cs
    call    near ptr sub_25EE2
    add     sp, 2
    and     [bp+var_44A], al
    jmp     loc_255DE
loc_25714:
    mov     al, byte ptr [bp+var_E]
    mov     byte_449F4, al
loc_2571A:
    cmp     byte_449F4, 0
    jnz     short loc_25724
    jmp     loc_25801
loc_25724:
    cmp     [bp+var_44A], 0
    jz      short loc_2572E
    jmp     loc_25801
loc_2572E:
    test    byte ptr [bp+var_2], 1
    jnz     short loc_25760
    les     bx, [bp+var_434]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    and     ax, [bp+var_A]
    and     dx, [bp+var_8]
    or      dx, ax
    jnz     short loc_25760
    mov     ax, word ptr dword_40ECA
    mov     dx, word ptr dword_40ECA+2
    add     ax, 6
    push    dx
    push    ax
    push    cs
    call    near ptr sub_25F2E
    add     sp, 4
    or      al, al
    jz      short loc_25763
loc_25760:
    inc     [bp+var_4]
loc_25763:
    cmp     [bp+var_4], 0
    jnz     short loc_2576C
    jmp     loc_25801
loc_2576C:
    test    byte_4186E, 8
    jnz     short loc_25776
    jmp     loc_25801
loc_25776:
    mov     ax, word ptr dword_40ECA
    mov     dx, word ptr dword_40ECA+2
    add     ax, 6
    mov     word ptr [bp+var_56E], ax
    mov     word ptr [bp+var_56E+2], dx
    mov     [bp+var_E], 0
    jmp     short loc_257F7
    ; align 2
    db 144
loc_25790:
    les     bx, [bp+var_56E]
    mov     ax, es:[bx]
    mov     [bp+var_564], ax
    mov     ax, es:[bx+2]
    mov     [bp+var_570], ax
    add     word ptr [bp+var_56E], 4
    mov     bx, word_41878
    mov     ax, [bx]
    cmp     [bp+var_564], ax
    jge     short loc_257BA
    mov     ax, [bp+var_564]
    mov     [bx], ax
loc_257BA:
    mov     ax, [bp+var_564]
    inc     ax
    mov     [bp+var_B7C], ax
    mov     bx, word_41878
    cmp     [bx+2], ax
    jge     short loc_257CF
    mov     [bx+2], ax
loc_257CF:
    mov     bx, word_41878
    mov     ax, [bp+var_570]
    cmp     [bx+4], ax
    jle     short loc_257DF
    mov     [bx+4], ax
loc_257DF:
    mov     ax, [bp+var_570]
    inc     ax
    mov     [bp+var_B7C], ax
    mov     bx, word_41878
    cmp     [bx+6], ax
    jge     short loc_257F4
    mov     [bx+6], ax
loc_257F4:
    inc     [bp+var_E]
loc_257F7:
    mov     al, byte_449F4
    sub     ah, ah
    cmp     [bp+var_E], ax
    jb      short loc_25790
loc_25801:
    mov     ax, word_4186A
    mov     dx, word_4186C
    mov     word ptr dword_41870, ax
    mov     word ptr dword_41870+2, dx
    add     word ptr [bp+var_434], 4
    add     word ptr [bp+var_B78], 4
    cmp     [bp+var_4], 0
    jz      short loc_25822
    jmp     loc_25D3C
loc_25822:
    test    byte ptr [bp+var_2], 2
    jz      short loc_2582B
    jmp     loc_25E04
loc_2582B:
    les     bx, dword_41870
    test    byte ptr es:[bx+1], 2
    jnz     short loc_25839
    jmp     loc_25E04
loc_25839:
    mov     bl, es:[bx]
    sub     bh, bh
    mov     al, [bx+32F2h]
    sub     ah, ah
    add     ax, word_41852
    add     ax, 2
    add     word ptr dword_41870, ax
    add     word ptr [bp+var_B78], 4
    add     word ptr [bp+var_434], 4
    jmp     short loc_2582B
    ; align 2
    db 144
loc_2585C:
    les     bx, dword_41870
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    mov     al, es:[bx+1]
    mov     di, ax
    mov     al, [bp+di+var_562]
    cbw
    mov     cx, ax
    mov     al, [bp+si+var_562]
    cbw
    add     ax, cx
    cmp     ax, 2
    jz      short loc_25801
    cmp     [bp+si+var_562], 0
    jz      short loc_258BC
    mov     ax, 0Ch
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    call    sub_33014
    add     sp, 8
    mov     ax, si
    jmp     short loc_258F6
loc_258BC:
    cmp     [bp+di+var_562], 0
    jz      short loc_2590D
    mov     ax, 0Ch
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    sub     ax, 0B6Eh
    push    ax
    call    sub_33014
    add     sp, 8
    mov     ax, di
loc_258F6:
    shl     ax, 1
    shl     ax, 1
    add     ax, bp
    sub     ax, 416h
    push    ax
    lea     ax, [bp+var_458]
    push    ax
    call    sub_323D9
    add     sp, 4
loc_2590D:
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0B6Ah]
    mov     bx, di
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
    add     bx, bp
    add     ax, [bx-0B6Ah]
    cwd
    mov     [bp+var_18], ax
    mov     [bp+var_16], dx
    mov     bx, word_41854
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, word_41856
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    test    byte_4186E, 8
    jz      short loc_25983
    push    word_41878
    push    word_41854
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
    push    word_41878
    push    word_41856
loc_2597C:
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
loc_25983:
    mov     byte_449F4, 2
loc_25988:
    inc     [bp+var_4]
    jmp     loc_25801
loc_2598E:
    cmp     [bp+var_1A], 0
    jz      short loc_25997
    jmp     loc_25801
loc_25997:
    mov     bx, word_41854
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, word_41856
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    mov     bx, word_41858
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+0Eh], ax
    mov     es:[bx+10h], dx
    mov     bx, word_4185A
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+12h], ax
    mov     es:[bx+14h], dx
    mov     ax, word ptr dword_40ECA
    mov     dx, word ptr dword_40ECA+2
    add     ax, 6
    push    dx
    push    ax
    push    cs
    call    near ptr sub_25F2E
    add     sp, 4
    or      al, al
    jnz     short loc_25A7C
    mov     bx, word_4185A
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, word_4185C
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+0Ah], ax
    mov     es:[bx+0Ch], dx
    mov     bx, word_4185E
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+0Eh], ax
    mov     es:[bx+10h], dx
    mov     bx, word_41854
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+12h], ax
    mov     es:[bx+14h], dx
    les     bx, dword_41870
    mov     al, es:[bx+3]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0B6Ah]
    cwd
    mov     cl, 2
loc_25A71:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_25A71
    jmp     short loc_25A9E
    ; align 2
    db 144
loc_25A7C:
    les     bx, dword_41870
    mov     al, es:[bx]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0B6Ah]
    cwd
    mov     cl, 2
loc_25A96:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_25A96
loc_25A9E:
    mov     [bp+var_18], ax
    mov     [bp+var_16], dx
    les     bx, dword_40ECA
    mov     ax, es:[bx+8]
    sub     ax, es:[bx+0Ch]
    push    ax
    mov     ax, es:[bx+6]
    sub     ax, es:[bx+0Ah]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     si, ax
    les     bx, dword_40ECA
    mov     ax, es:[bx+8]
    sub     ax, es:[bx+10h]
    push    ax
    mov     ax, es:[bx+6]
    sub     ax, es:[bx+0Eh]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     di, ax
    cmp     di, si
    jle     short loc_25AEA
    mov     si, di
loc_25AEA:
    test    byte_4186E, 8
    jnz     short loc_25AF4
    jmp     loc_25B9C
loc_25AF4:
    les     bx, dword_40ECA
    mov     ax, es:[bx+6]
    sub     ax, si
    dec     ax
    mov     [bp+var_450], ax
    mov     ax, es:[bx+8]
    sub     ax, si
    dec     ax
    mov     [bp+var_44E], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
    les     bx, dword_40ECA
    mov     ax, es:[bx+8]
    add     ax, si
    inc     ax
    mov     [bp+var_44E], ax
    mov     ax, es:[bx+6]
    add     ax, si
    inc     ax
    mov     [bp+var_450], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
    les     bx, dword_40ECA
    mov     ax, es:[bx+12h]
    sub     ax, si
    dec     ax
    mov     [bp+var_450], ax
    mov     ax, es:[bx+14h]
    sub     ax, si
    dec     ax
    mov     [bp+var_44E], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
    les     bx, dword_40ECA
    mov     ax, es:[bx+14h]
    add     ax, si
    inc     ax
    mov     [bp+var_44E], ax
    mov     ax, es:[bx+12h]
    add     ax, si
    inc     ax
    mov     [bp+var_450], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
loc_25B9C:
    mov     byte_449F4, 4
    mov     [bp+var_4], 1
    jmp     loc_25801
    ; align 2
    db 144
loc_25BAA:
    les     bx, dword_41870
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
    mov     [bp+var_B7C], ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_B7A], ax
    mov     bx, ax
    mov     ax, [bx-0B6Ah]
    mov     bx, [bp+var_B7C]
    add     ax, [bx-0B6Ah]
    cwd
    mov     [bp+var_18], ax
    mov     [bp+var_16], dx
    mov     al, [bp+di+var_562]
    cbw
    mov     cx, ax
    mov     al, [bp+si+var_562]
    cbw
    add     ax, cx
    jz      short loc_25C01
    jmp     loc_25801
loc_25C01:
    mov     bx, word_41854
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    mov     bx, [bp+var_B7A]
    push    si
    push    di
    lea     di, [bp+var_56A]
    lea     si, [bx-0B6Eh]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [bp+var_B7C]
    push    si
    push    di
    lea     di, [bp+var_B74]
    lea     si, [bx-0B6Eh]
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     ax, [bp+var_56A]
    sub     ax, [bp+var_B74]
    mov     [bp+var_458], ax
    mov     ax, [bp+var_568]
    sub     ax, [bp+var_B72]
    mov     [bp+var_456], ax
    mov     ax, [bp+var_566]
    sub     ax, [bp+var_B70]
    mov     [bp+var_454], ax
    push    [bp+var_566]
    lea     ax, [bp+var_458]
    push    ax
    call    sub_2EA08
    add     sp, 2
    push    ax
    call    sub_3275C
    add     sp, 4
    mov     [bp+var_462], ax
    les     bx, dword_40ECA
    mov     es:[bx+0Ah], ax
    test    byte_4186E, 8
    jnz     short loc_25C92
    jmp     loc_25983
loc_25C92:
    mov     bx, word_41854
    mov     ax, [bx+2]
    sub     ax, [bp+var_462]
    mov     [bp+var_44E], ax
    mov     ax, [bx]
    sub     ax, [bp+var_462]
    mov     [bp+var_450], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
    mov     ax, [bp+var_462]
    mov     bx, word_41854
    add     ax, [bx]
    mov     [bp+var_450], ax
    mov     ax, [bx+2]
    add     ax, [bp+var_462]
    mov     [bp+var_450], ax
    push    word_41878
    lea     ax, [bp+var_450]
    push    ax
    jmp     loc_2597C
loc_25CE0:
    les     bx, dword_41870
    mov     al, es:[bx]
    sub     ah, ah
    mov     si, ax
    cmp     [bp+si+var_562], ah
    jz      short loc_25CF4
    jmp     loc_25801
loc_25CF4:
    mov     bx, si
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0B6Ah]
    cwd
    mov     [bp+var_18], ax
    mov     [bp+var_16], dx
    mov     bx, word_41854
    mov     ax, [bx]
    mov     dx, [bx+2]
    les     bx, dword_40ECA
    mov     es:[bx+6], ax
    mov     es:[bx+8], dx
    test    byte_4186E, 8
    jz      short loc_25D34
    push    word_41878
    push    word_41854
    push    cs
    call    near ptr sub_2637A
    add     sp, 4
loc_25D34:
    mov     byte_449F4, 1
    jmp     loc_25988
loc_25D3C:
    inc     [bp+var_45E]
    les     bx, dword_40ECA
    mov     al, byte_449F4
    mov     es:[bx+3], al
    les     bx, dword_40ECA
    mov     al, [bp+var_6]
    mov     es:[bx+4], al
    cmp     byte_41882, 2Dh ; '-'
    jnz     short loc_25D66
    les     bx, dword_40ECA
    mov     al, byte_45514
    jmp     short loc_25D6D
loc_25D66:
    les     bx, dword_40ECA
    mov     al, byte_41882
loc_25D6D:
    mov     es:[bx+2], al
    mov     al, byte_449F4
    sub     ah, ah
    cmp     ax, 1
    jz      short loc_25D9C
    cmp     ax, 2
    jz      short loc_25DB8
    cmp     ax, 4
    jz      short loc_25DC6
    cmp     ax, 8
    jz      short loc_25DDA
    sub     cx, cx
    push    cx
    push    ax
    push    [bp+var_16]
    push    [bp+var_18]
    call    __aFuldiv
    jmp     short loc_25DC2
    ; align 2
    db 144
loc_25D9C:
    mov     si, [bp+var_18]
loc_25D9F:
    les     bx, dword_40ECA
    mov     es:[bx], si
    test    byte_4186E, 1
    jnz     short loc_25DB3
    test    byte ptr [bp+var_2], 2
    jz      short loc_25DEE
loc_25DB3:
    sub     ax, ax
    jmp     short loc_25DF1
    ; align 2
    db 144
loc_25DB8:
    mov     ax, [bp+var_18]
    mov     dx, [bp+var_16]
    sar     dx, 1
    rcr     ax, 1
loc_25DC2:
    mov     si, ax
    jmp     short loc_25D9F
loc_25DC6:
    mov     ax, [bp+var_18]
    mov     dx, [bp+var_16]
    mov     cl, 2
loc_25DCE:
    or      cl, cl
    jz      short loc_25DC2
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jmp     short loc_25DCE
loc_25DDA:
    mov     ax, [bp+var_18]
    mov     dx, [bp+var_16]
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
    push    cs
    call    near ptr sub_25E24
    add     sp, 4
    mov     word_40ECE, ax
    or      ax, ax
    jz      short loc_25E04
    jmp     loc_24EAE
loc_25E04:
    les     bx, dword_41870
    cmp     byte ptr es:[bx], 0
    jz      short loc_25E11
    jmp     loc_25233
loc_25E11:
    cmp     [bp+var_45E], 0
    jnz     short loc_25E1B
    jmp     loc_25216
loc_25E1B:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_24E9E endp
sub_25E24 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    cmp     [bp+arg_2], 0
    jnz     short loc_25E3E
    mov     bx, word_45D98
    shl     bx, 1
    mov     di, [bx+5766h]
    jmp     short loc_25E7B
loc_25E3E:
    mov     ax, word_4394E
    mov     word_45D98, ax
    mov     bx, ax
    shl     bx, 1
    mov     di, [bx+5766h]
    mov     si, word_4554A
    jmp     short loc_25E77
loc_25E52:
    mov     ax, si
    dec     si
    or      ax, ax
    jz      short loc_25E7B
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    les     bx, [bx+5A88h]
    mov     ax, [bp+arg_0]
    cmp     es:[bx], ax
    jl      short loc_25E7B
    mov     word_45D98, di
    mov     bx, di
    shl     bx, 1
    mov     di, [bx+5766h]
loc_25E77:
    or      di, di
    jge     short loc_25E52
loc_25E7B:
    mov     bx, word_442E6
    shl     bx, 1
    mov     [bx+5766h], di
    mov     bx, word_45D98
    shl     bx, 1
    mov     ax, word_442E6
    mov     [bx+5766h], ax
    inc     word_4554A
    or      di, di
    jge     short loc_25EA0
    mov     ax, word_442E6
    mov     word_443F2, ax
loc_25EA0:
    mov     bx, word_45D98
    shl     bx, 1
    mov     ax, [bx+5766h]
    mov     word_45D98, ax
    inc     word_442E6
    mov     al, byte_449F4
    sub     ah, ah
    shl     ax, 1
    shl     ax, 1
    add     ax, 6
    add     word_41850, ax
    cmp     word_442E6, 190h
    jz      short loc_25ED1
    cmp     word_41850, 2872h
    jle     short loc_25EDA
loc_25ED1:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_25EDA:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_25E24 endp
sub_25EE2 proc far
    var_4 = byte ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    si
    mov     si, [bp+arg_0]
    mov     ax, word_40EB4
    cmp     [si+2], ax
    jge     short loc_25EFA
    mov     [bp+var_4], 1
    jmp     short loc_25F0C
loc_25EFA:
    mov     ax, word_40EB6
    cmp     [si+2], ax
    jle     short loc_25F08
    mov     [bp+var_4], 2
    jmp     short loc_25F0C
loc_25F08:
    mov     [bp+var_4], 0
loc_25F0C:
    mov     ax, word_40EB0
    cmp     [si], ax
    jge     short loc_25F1A
smart
    or      [bp+var_4], 4
nosmart
    jmp     short loc_25F25
    ; align 2
    db 144
loc_25F1A:
    mov     ax, word_40EB2
    cmp     [si], ax
    jle     short loc_25F25
smart
    or      [bp+var_4], 8
nosmart
loc_25F25:
    mov     al, [bp+var_4]
    cbw
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_25EE2 endp
sub_25F2E proc far
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 10h
    push    di
    push    si
    les     bx, [bp+arg_0]
    mov     ax, es:[bx+4]
    cwd
    mov     cx, ax
    mov     ax, es:[bx]
    mov     bx, dx
    cwd
    sub     ax, cx
    sbb     dx, bx
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     bx, word ptr [bp+arg_0]
    mov     ax, es:[bx+4]
    cwd
    mov     cx, ax
    mov     ax, es:[bx+8]
    mov     bx, dx
    cwd
    sub     ax, cx
    sbb     dx, bx
    mov     [bp+var_C], ax
    mov     [bp+var_A], dx
    mov     ax, [bp+var_4]
    or      ax, [bp+var_2]
    jnz     short loc_25F7A
    mov     ax, [bp+var_C]
    or      ax, dx
    jz      short loc_25FEE
loc_25F7A:
    mov     bx, word ptr [bp+arg_0]
    mov     ax, es:[bx+2]
    sub     ax, es:[bx+6]
    cwd
    mov     [bp+var_8], ax
    mov     [bp+var_6], dx
    mov     ax, es:[bx+0Ah]
    sub     ax, es:[bx+6]
    cwd
    mov     [bp+var_10], ax
    mov     [bp+var_E], dx
    mov     ax, [bp+var_8]
    or      ax, [bp+var_6]
    jnz     short loc_25FAA
    mov     ax, [bp+var_10]
    or      ax, dx
    jz      short loc_25FEE
loc_25FAA:
    push    [bp+var_E]
    push    [bp+var_10]
    push    [bp+var_2]
    push    [bp+var_4]
    call    __aFlmul
    push    [bp+var_6]
    push    [bp+var_8]
    push    [bp+var_A]
    push    [bp+var_C]
    mov     si, ax
    mov     di, dx
    call    __aFlmul
    sub     ax, si
    sbb     dx, di
    or      dx, dx
    jl      short loc_25FE4
    jg      short loc_25FDE
    or      ax, ax
    jz      short loc_25FE4
loc_25FDE:
    mov     al, 1
    jmp     short loc_25FE6
    ; align 4
    db 144
    db 144
loc_25FE4:
    sub     al, al
loc_25FE6:
    cbw
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_25FEE:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_25F2E endp
sub_25FF6 proc far
    var_40 = word ptr -64
    var_3E = dword ptr -62
    var_38 = dword ptr -56
    var_32 = byte ptr -50
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 40h
    push    di
    push    si
    mov     di, 190h
    sub     si, si
    jmp     loc_260AC
loc_26006:
    les     bx, [bp+var_38]
    mov     al, es:[bx+3]
    cbw
    mov     [bp+var_2], ax
    mov     ax, bx
    mov     dx, es
    add     ax, 6
    mov     word ptr [bp+var_3E], ax
    mov     word ptr [bp+var_3E+2], dx
    lea     ax, [bp+var_32]
    mov     [bp+var_A], ax
    mov     [bp+var_4], 0
    jmp     short loc_26049
    ; align 2
    db 144
loc_2602C:
    les     bx, [bp+var_3E]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    mov     bx, [bp+var_A]
    mov     [bx], ax
    mov     [bx+2], dx
    add     [bp+var_A], 4
    add     word ptr [bp+var_3E], 4
    inc     [bp+var_4]
loc_26049:
    mov     ax, [bp+var_2]
    cmp     [bp+var_4], ax
    jb      short loc_2602C
    mov     bx, [bp+var_6]
    shl     bx, 1
    add     bx, word_45D1E
    mov     ax, [bx]
    or      ax, ax
    jz      short loc_26070
    cmp     ax, 1
    jz      short loc_26084
    cmp     ax, 2
    jnz     short loc_2606D
    jmp     loc_26118
loc_2606D:
    jmp     short loc_260AB
    ; align 2
    db 144
loc_26070:
    lea     ax, [bp+var_32]
    push    ax
    push    [bp+var_2]
    push    [bp+var_8]
    call    sub_317B2
loc_2607F:
    add     sp, 6
    jmp     short loc_260AB
loc_26084:
    mov     bx, [bp+var_6]
    shl     bx, 1
    add     bx, word_45D92
    mov     ax, [bx]
    mov     [bp+var_40], ax
    or      ax, ax
    jz      short loc_260AB
    lea     ax, [bp+var_32]
    push    ax
    push    [bp+var_2]
    push    [bp+var_8]
    push    [bp+var_40]
    call    sub_32886
loc_260A8:
    add     sp, 8
loc_260AB:
    inc     si
loc_260AC:
    mov     ax, si
    cmp     ax, word_442E6
    jb      short loc_260B7
    jmp     loc_261F0
loc_260B7:
    mov     bx, di
    shl     bx, 1
    mov     di, [bx+5766h]
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    mov     ax, [bx+5A88h]
    mov     dx, [bx+5A8Ah]
    mov     word ptr [bp+var_38], ax
    mov     word ptr [bp+var_38+2], dx
    les     bx, [bp+var_38]
    mov     al, es:[bx+2]
    sub     ah, ah
    mov     [bp+var_6], ax
    mov     bx, ax
    shl     bx, 1
    add     bx, word_45632
    mov     ax, [bx]
    mov     [bp+var_8], ax
    mov     bx, word ptr [bp+var_38]
    mov     al, es:[bx+4]
    cbw
    or      ax, ax
    jnz     short loc_260FB
    jmp     loc_26006
loc_260FB:
    cmp     ax, 1
    jz      short loc_26144
    cmp     ax, 2
    jnz     short loc_26108
    jmp     loc_261C0
loc_26108:
    cmp     ax, 3
    jz      short loc_26166
    cmp     ax, 5
    jnz     short loc_26115
    jmp     loc_261DA
loc_26115:
    jmp     short loc_260AB
    ; align 2
    db 144
loc_26118:
    mov     ax, [bp+var_6]
    shl     ax, 1
    mov     [bp+var_40], ax
    lea     ax, [bp+var_32]
    push    ax
    push    [bp+var_2]
    push    [bp+var_8]
    mov     bx, [bp+var_40]
    add     bx, word_459D6
    push    word ptr [bx]
    mov     bx, [bp+var_40]
    add     bx, word_45D92
    push    word ptr [bx]
    call    sub_2F3DA
    jmp     short loc_2615F
    ; align 2
    db 144
loc_26144:
    push    [bp+var_8]
    les     bx, [bp+var_38]
    push    word ptr es:[bx+0Ch]
    push    word ptr es:[bx+0Ah]
    push    word ptr es:[bx+8]
    push    word ptr es:[bx+6]
    call    sub_2FDDE
loc_2615F:
    add     sp, 0Ah
    jmp     loc_260AB
    ; align 2
    db 144
loc_26166:
    mov     [bp+var_4], 0
loc_2616B:
    mov     ax, [bp+var_4]
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_40], ax
    mov     bx, ax
    add     bx, word ptr [bp+var_38]
    mov     es, word ptr [bp+var_38+2]
    mov     ax, es:[bx+6]
    mov     dx, es:[bx+8]
    mov     bx, [bp+var_40]
    add     bx, bp
    mov     [bx-32h], ax
    mov     [bx-30h], dx
    inc     [bp+var_4]
    cmp     [bp+var_4], 4
    jb      short loc_2616B
    mov     ax, [bp+var_6]
    shl     ax, 1
    add     ax, word_45632
    mov     [bp+var_40], ax
    mov     bx, ax
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    [bp+var_8]
    mov     ax, 2500h
    push    ax
    lea     ax, [bp+var_32]
    push    ax
    call    sub_36C7E
    jmp     short loc_2615F
    ; align 2
    db 144
loc_261C0:
    push    [bp+var_8]
    les     bx, [bp+var_38]
    push    word ptr es:[bx+0Ah]
    push    word ptr es:[bx+8]
    push    word ptr es:[bx+6]
    call    sub_33072
    jmp     loc_260A8
loc_261DA:
    push    [bp+var_8]
    les     bx, [bp+var_38]
    push    word ptr es:[bx+8]
    push    word ptr es:[bx+6]
    call    sub_35B26
    jmp     loc_2607F
loc_261F0:
    push    cs
    call    near ptr sub_24DE6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_25FF6 endp
mat_rot_zxy proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = byte ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    sub     si, si
    test    [bp+arg_0], 3FFh
    jz      short loc_2621C
    mov     si, 4
    push    [bp+arg_0]
    mov     ax, offset mat_unk_1
    push    ax
    call    mat_rot_z
    add     sp, si
loc_2621C:
    test    [bp+arg_2], 3FFh
    jz      short loc_26236
smart
    nop
    or      si, 2
nosmart
    push    [bp+arg_2]
    mov     ax, offset mat_unk_2
    push    ax
    call    mat_rot_x
    add     sp, 4
loc_26236:
    test    [bp+arg_4], 3FFh
    jz      short loc_26285
smart
    nop
    or      si, 1
nosmart
    mov     ax, [bp+arg_4]
smart
    and     ah, 3
nosmart
    cmp     ax, word_3EB02
    jnz     short loc_26252
loc_2624D:
    mov     di, offset mat_unk_3
    jmp     short loc_26285
loc_26252:
    mov     ax, [bp+arg_4]
smart
    and     ah, 3
nosmart
    cmp     ax, 100h
    jz      short loc_26282
    cmp     ax, 200h
    jz      short loc_26298
    cmp     ax, 300h
    jz      short loc_2629E
    push    [bp+arg_4]
    mov     ax, offset mat_unk_3
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     ax, [bp+arg_4]
smart
    and     ah, 3
nosmart
    mov     word_3EB02, ax
    jmp     short loc_2624D
    ; align 2
    db 144
loc_26282:
    mov     di, offset mat_y100
loc_26285:
    mov     ax, si
    cmp     ax, 7
    jbe     short loc_2628F
    jmp     loc_26372
loc_2628F:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_26362[bx]
    ; align 2
    db 144
loc_26298:
    mov     di, offset mat_y200
    jmp     short loc_26285
    ; align 2
    db 144
loc_2629E:
    mov     di, offset mat_y300
    jmp     short loc_26285
    ; align 2
    db 144
loc_262A4:
    mov     di, offset mat_y0
    jmp     loc_26372
loc_262AA:
    test    [bp+arg_6], 1
    jz      short loc_262BC
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_2
loc_262B7:
    push    ax
    push    di
    jmp     short loc_262C5
    ; align 2
    db 144
loc_262BC:
    mov     ax, offset mat_rot_temp
    push    ax
    push    di
loc_262C1:
    mov     ax, offset mat_unk_2
loc_262C4:
    push    ax
loc_262C5:
    call    mat_multiply
    add     sp, 6
    mov     di, offset mat_rot_temp
    jmp     loc_26372
    ; align 2
    db 144
loc_262D4:
    test    [bp+arg_6], 1
    jz      short loc_262E4
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_1
    jmp     short loc_262B7
    ; align 2
    db 144
loc_262E4:
    mov     ax, offset mat_rot_temp
    push    ax
    push    di
loc_262E9:
    mov     ax, offset mat_unk_1
    jmp     short loc_262C4
loc_262EE:
    test    [bp+arg_6], 1
    jz      short loc_262FE
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_1
    push    ax
    jmp     short loc_262C1
loc_262FE:
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_2
    push    ax
    jmp     short loc_262E9
loc_26308:
    test    [bp+arg_6], 1
    jz      short loc_26338
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_2
    push    ax
    push    di
    call    mat_multiply
    add     sp, 6
    mov     ax, offset mat_unk_2
    push    ax
    mov     ax, offset mat_unk_1
    push    ax
    mov     ax, offset mat_rot_temp
    push    ax
    call    mat_multiply
    add     sp, 6
loc_26333:
    mov     di, offset mat_unk_2
    jmp     short loc_26372
loc_26338:
    mov     ax, offset mat_rot_temp
    push    ax
    mov     ax, offset mat_unk_2
    push    ax
    mov     ax, offset mat_unk_1
    push    ax
    call    mat_multiply
    add     sp, 6
    mov     ax, offset mat_unk_1
    push    ax
    push    di
    mov     ax, offset mat_rot_temp
    push    ax
    call    mat_multiply
    add     sp, 6
loc_2635D:
    mov     di, offset mat_unk_1
    jmp     short loc_26372
off_26362     dw offset loc_262A4
    dw offset loc_26372
    dw offset loc_26333
    dw offset loc_262AA
    dw offset loc_2635D
    dw offset loc_262D4
    dw offset loc_262EE
    dw offset loc_26308
loc_26372:
    mov     ax, di
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
mat_rot_zxy endp
sub_2637A proc far
    var_6 = word ptr -6
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bx]
    mov     di, [bx+2]
    mov     bx, [bp+arg_2]
    cmp     [bx], si
    jle     short loc_26393
    mov     [bx], si
loc_26393:
    lea     ax, [si+1]
    mov     [bp+var_6], ax
    mov     bx, [bp+arg_2]
    cmp     [bx+2], ax
    jge     short loc_263A4
    mov     [bx+2], ax
loc_263A4:
    mov     bx, [bp+arg_2]
    cmp     [bx+4], di
    jle     short loc_263AF
    mov     [bx+4], di
loc_263AF:
    lea     ax, [di+1]
    mov     [bp+var_6], ax
    mov     bx, [bp+arg_2]
    cmp     [bx+6], ax
    jge     short loc_263C0
    mov     [bx+6], ax
loc_263C0:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_2637A endp
sub_263C6 proc far
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 0Eh
    push    di
    push    si
    mov     bx, [bp+arg_0]
    push    word ptr [bx+2] ; int
    call    _abs
    add     sp, 2
    cwd
    mov     [bp+var_E], ax
    mov     [bp+var_C], dx
    mov     bx, [bp+arg_0]
    push    word ptr [bx+4] ; int
    call    _abs
    add     sp, 2
    push    ax
    mov     bx, [bp+arg_0]
    push    word ptr [bx]   ; int
    call    _abs
    add     sp, 2
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], 0
    mov     ax, word_41888
    mov     dx, word_4188A
    cmp     word_41884, ax
    jnz     short loc_2643A
    cmp     word_41886, dx
    jnz     short loc_2643A
    mov     ax, [bp+var_E]
    mov     dx, [bp+var_C]
    cmp     [bp+var_2], dx
    jg      short loc_2646E
    jl      short loc_26435
    cmp     [bp+var_4], ax
    jnb     short loc_2646E
loc_26435:
    mov     ax, 1
    jmp     short loc_26470
loc_2643A:
    push    word_4188A
    push    word_41888
    push    [bp+var_C]
    push    [bp+var_E]
    call    __aFlmul
    push    word_41886
    push    word_41884
    push    [bp+var_2]
    push    [bp+var_4]
    mov     si, ax
    mov     di, dx
    call    __aFlmul
    cmp     dx, di
    jg      short loc_2646E
    jl      short loc_26435
    cmp     ax, si
    jb      short loc_26435
loc_2646E:
    sub     ax, ax
loc_26470:
    mov     [bp+var_8], ax
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+2], 0
    jge     short loc_2648A
    or      ax, ax
    jz      short loc_264A0
    mov     ax, 1Eh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_2648A:
    cmp     word ptr [bx+2], 0
    jle     short loc_264A0
    cmp     [bp+var_8], 0
    jz      short loc_264A0
    mov     ax, 1Fh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_264A0:
    cmp     word ptr [bx+2], 0
    jle     short loc_264AC
    mov     [bp+var_A], 0Fh
    jmp     short loc_264B0
loc_264AC:
    mov     [bp+var_A], 0
loc_264B0:
    mov     ax, [bx]
    neg     ax
    push    ax
    push    word ptr [bx+4]
    call    sub_2EA4E
    add     sp, 4
    neg     ax
    mov     [bp+var_6], ax
    or      ax, ax
    jge     short loc_264CD
    add     byte ptr [bp+var_6+1], 4
loc_264CD:
    mov     ax, [bp+var_6]
    cwd
    mov     cx, ax
    mov     bx, dx
    shl     ax, 1
    rcl     dx, 1
    shl     ax, 1
    rcl     dx, 1
    shl     ax, 1
    rcl     dx, 1
    shl     ax, 1
    rcl     dx, 1
    sub     ax, cx
    sbb     dx, bx
    mov     cl, 0Ah
loc_264EB:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_264EB
    add     [bp+var_A], al
    mov     al, [bp+var_A]
    cbw
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_263C6 endp
sub_26500 proc far

    mov     ax, 80h ; '€'
    push    ax
    call    sin_fast
    add     sp, 2
    cwd
    mov     word_41888, ax
    mov     word_4188A, dx
    mov     ax, 80h ; '€'
    push    ax
    call    cos_fast
    add     sp, 2
    cwd
    mov     word_41884, ax
    mov     word_41886, dx
    mov     ax, 80h ; '€'
    push    ax
    call    sin_fast
    add     sp, 2
    cwd
    mov     word_41890, ax
    mov     word_41892, dx
    mov     ax, 80h ; '€'
    push    ax
    call    cos_fast
    add     sp, 2
    cwd
    mov     word_4188C, ax
    mov     word_4188E, dx
    retf
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    cmp     word ptr [bp+8], 0
    jge     short loc_2656A
    mov     ax, [bp+6]
    mov     dx, [bp+8]
    neg     ax
    adc     dx, 0
    neg     dx
    pop     bp
    retf
loc_2656A:
    mov     ax, [bp+6]
    mov     dx, [bp+8]
    pop     bp
    retf
sub_26500 endp
sub_26572 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    push    si
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_0]
    mov     ax, [si]
    mov     si, [bp+arg_2]
    cmp     ax, [si]
    jle     short loc_26587
    mov     ax, [si]
loc_26587:
    mov     [bx], ax
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_0]
    mov     ax, [si+2]
    mov     si, [bp+arg_2]
    cmp     ax, [si+2]
    jge     short loc_2659D
    mov     ax, [si+2]
loc_2659D:
    mov     [bx+2], ax
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_0]
    mov     ax, [si+4]
    mov     si, [bp+arg_2]
    cmp     ax, [si+4]
    jle     short loc_265B4
    mov     ax, [si+4]
loc_265B4:
    mov     [bx+4], ax
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_0]
    mov     ax, [si+6]
    mov     si, [bp+arg_2]
    cmp     ax, [si+6]
    jge     short loc_265CB
    mov     ax, [si+6]
loc_265CB:
    mov     [bx+6], ax
    cmp     word_44DC8, 1
    jz      short loc_265E9
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+2]
    add     ax, word_44DC8
    dec     ax
    and     ax, word_454D4
    mov     [bx+2], ax
loc_265E9:
    pop     si
    pop     bp
    retf
sub_26572 endp
sub_265EC proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    si
    mov     bx, [bp+arg_0]
    mov     si, bx
    mov     ax, [si]
    cmp     [bx+2], ax
    jge     short loc_26602
loc_265FC:
    mov     ax, 1
    pop     si
    pop     bp
    retf
loc_26602:
    mov     bx, [bp+arg_2]
    mov     si, [bp+arg_0]
    mov     ax, [si]
    cmp     [bx+2], ax
    jle     short loc_265FC
    mov     bx, si
    mov     si, [bp+arg_2]
    mov     ax, [si]
    cmp     [bx+2], ax
    jle     short loc_265FC
    mov     ax, [si+6]
    cmp     [bx+4], ax
    jge     short loc_265FC
    mov     ax, [si+4]
    cmp     [bx+6], ax
    jle     short loc_265FC
    mov     bx, si
    mov     si, [bx]
    mov     bx, [bp+arg_0]
    cmp     [bx], si
    jge     short loc_26638
    mov     [bx], si
loc_26638:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+2]
    cmp     [bx+2], ax
    jle     short loc_26649
    mov     [bx+2], ax
loc_26649:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+4]
    cmp     [bx+4], ax
    jge     short loc_2665A
    mov     [bx+4], ax
loc_2665A:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+6]
    cmp     [bx+6], ax
    jle     short loc_2666B
    mov     [bx+6], ax
loc_2666B:
    sub     ax, ax
    pop     si
    pop     bp
    retf
sub_265EC endp
sub_26670 proc far
    var_22 = byte ptr -34
    var_20 = byte ptr -32
    var_1A = word ptr -26
    var_18 = byte ptr -24
    var_16 = word ptr -22
    var_14 = byte ptr -20
    var_12 = byte ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = byte ptr -8
    var_4 = word ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 22h
    push    di
    push    si
    cmp     word_44DC8, 1
    jz      short loc_26693
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+2]
    add     ax, word_44DC8
    dec     ax
    and     ax, word_454D4
    mov     [bx+2], ax
loc_26693:
    mov     [bp+var_14], 0
    jmp     short loc_266C9
    ; align 2
    db 144
loc_2669A:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    add     di, [bp+arg_2]
    push    si
    push    di
    lea     si, [di+8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     [bp+var_12]
loc_266B3:
    mov     al, [bp+var_12]
    cbw
    mov     si, ax
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    cbw
    dec     ax
    cmp     ax, si
    jg      short loc_2669A
    dec     byte ptr [bx]
loc_266C6:
    inc     [bp+var_14]
loc_266C9:
    mov     bx, [bp+arg_0]
    mov     al, [bp+var_14]
    cmp     [bx], al
    jg      short loc_266D6
    jmp     loc_26880
loc_266D6:
    cbw
    mov     cl, 3
    shl     ax, cl
    add     ax, [bp+arg_2]
    mov     [bp+var_16], ax
    push    ax
    push    [bp+arg_4]
    push    cs
    call    near ptr sub_2695E
    add     sp, 4
    or      al, al
    jz      short loc_266C6
    push    [bp+var_16]
    push    [bp+arg_4]
    push    cs
    call    near ptr sub_2699C
    add     sp, 4
    or      al, al
    jz      short loc_26704
    jmp     loc_26957
loc_26704:
    push    [bp+arg_4]
    push    [bp+var_16]
    push    cs
    call    near ptr sub_2699C
    add     sp, 4
    or      al, al
    jz      short loc_2671E
    mov     al, [bp+var_14]
    mov     [bp+var_12], al
    jmp     short loc_266B3
    ; align 2
    db 144
loc_2671E:
    mov     ax, [bp+var_16]
    lea     di, [bp+var_10]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     bx, ax
    mov     si, [bp+arg_4]
    mov     ax, [si+4]
    cmp     [bx+4], ax
    jge     short loc_2675A
    mov     ax, bx
    lea     di, [bp+var_20]
    mov     si, ax
    movsw
    movsw
    movsw
    movsw
    mov     bx, [bp+arg_4]
    mov     ax, [bx+4]
    mov     [bp+var_1A], ax
    mov     ax, [bx+4]
    mov     [bp+var_C], ax
loc_26753:
    mov     [bp+var_18], 1
    jmp     short loc_26784
    ; align 2
    db 144
loc_2675A:
    mov     bx, [bp+arg_4]
    mov     si, [bp+var_16]
    mov     ax, [si+4]
    cmp     [bx+4], ax
    jge     short loc_26780
    mov     ax, bx
    lea     di, [bp+var_20]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     bx, [bp+var_16]
    mov     ax, [bx+4]
    mov     [bp+var_1A], ax
    jmp     short loc_26753
loc_26780:
    mov     [bp+var_18], 0
loc_26784:
    mov     bx, [bp+var_16]
    mov     si, [bp+arg_4]
    mov     ax, [si+6]
    cmp     [bx+6], ax
    jle     short loc_267B4
    mov     ax, bx
    lea     di, [bp+var_8]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     bx, [bp+arg_4]
    mov     ax, [bx+6]
    mov     [bp+var_4], ax
    mov     ax, [bx+6]
    mov     [bp+var_A], ax
loc_267AE:
    mov     [bp+var_22], 1
    jmp     short loc_267DE
loc_267B4:
    mov     bx, [bp+arg_4]
    mov     si, [bp+var_16]
    mov     ax, [si+6]
    cmp     [bx+6], ax
    jle     short loc_267DA
    mov     ax, bx
    lea     di, [bp+var_8]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     bx, [bp+var_16]
    mov     ax, [bx+6]
    mov     [bp+var_4], ax
    jmp     short loc_267AE
loc_267DA:
    mov     [bp+var_22], 0
loc_267DE:
    mov     bx, [bp+arg_4]
    mov     ax, [bx]
    mov     bx, [bp+var_16]
    cmp     ax, [bx]
    jle     short loc_267EC
    mov     ax, [bx]
loc_267EC:
    mov     [bp+var_10], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+2]
    mov     bx, [bp+var_16]
    cmp     ax, [bx+2]
    jge     short loc_26800
    mov     ax, [bx+2]
loc_26800:
    mov     [bp+var_E], ax
    mov     al, [bp+var_14]
    mov     [bp+var_12], al
    jmp     short loc_26825
    ; align 2
    db 144
loc_2680C:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    add     di, [bp+arg_2]
    push    si
    push    di
    lea     si, [di+8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     [bp+var_12]
loc_26825:
    mov     al, [bp+var_12]
    cbw
    mov     si, ax
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    cbw
    dec     ax
    cmp     ax, si
    jg      short loc_2680C
    dec     byte ptr [bx]
    cmp     [bp+var_18], 0
    jz      short loc_2684F
    lea     ax, [bp+var_20]
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    push    cs
    call    near ptr sub_26670
    add     sp, 6
loc_2684F:
    lea     ax, [bp+var_10]
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    push    cs
    call    near ptr sub_26670
    add     sp, 6
    cmp     [bp+var_22], 0
    jnz     short loc_26869
    jmp     loc_26957
loc_26869:
    lea     ax, [bp+var_8]
loc_2686C:
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    push    cs
    call    near ptr sub_26670
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_26880:
    mov     [bp+var_14], 0
    jmp     short loc_268BB
loc_26886:
    mov     di, si
    mov     cl, 3
    shl     di, cl
    add     di, [bp+arg_2]
    push    si
    push    di
    lea     si, [di+8]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     [bp+var_12]
loc_2689F:
    mov     al, [bp+var_12]
    cbw
    mov     si, ax
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    cbw
    dec     ax
    cmp     ax, si
    jg      short loc_26886
    dec     byte ptr [bx]
    lea     ax, [bp+var_10]
    jmp     short loc_2686C
    ; align 2
    db 144
loc_268B8:
    inc     [bp+var_14]
loc_268BB:
    mov     bx, [bp+arg_0]
    mov     al, [bp+var_14]
    cmp     [bx], al
    jle     short loc_26936
    cbw
    mov     cl, 3
    shl     ax, cl
    add     ax, [bp+arg_2]
    mov     [bp+var_16], ax
    push    [bp+arg_4]
    push    ax
    push    cs
    call    near ptr sub_269D0
    add     sp, 4
    or      al, al
    jz      short loc_268B8
    mov     bx, [bp+var_16]
    mov     ax, [bx]
    mov     bx, [bp+arg_4]
    cmp     ax, [bx]
    jle     short loc_268ED
    mov     ax, [bx]
loc_268ED:
    mov     [bp+var_10], ax
    mov     bx, [bp+var_16]
    mov     ax, [bx+2]
    mov     bx, [bp+arg_4]
    cmp     ax, [bx+2]
    jge     short loc_26901
    mov     ax, [bx+2]
loc_26901:
    mov     [bp+var_E], ax
    mov     bx, [bp+var_16]
    mov     ax, [bx+4]
    mov     bx, [bp+arg_4]
    cmp     ax, [bx+4]
    jle     short loc_26915
    mov     ax, [bx+4]
loc_26915:
    mov     [bp+var_C], ax
    mov     bx, [bp+var_16]
    mov     ax, [bx+6]
    mov     bx, [bp+arg_4]
    cmp     ax, [bx+6]
    jge     short loc_26929
    mov     ax, [bx+6]
loc_26929:
    mov     [bp+var_A], ax
    mov     al, [bp+var_14]
    mov     [bp+var_12], al
    jmp     loc_2689F
    ; align 2
    db 144
loc_26936:
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    cbw
    mov     bx, ax
    mov     cl, 3
    shl     bx, cl
    mov     si, [bp+arg_2]
    mov     ax, [bp+arg_4]
    lea     di, [bx+si]
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    movsw
    mov     bx, [bp+arg_0]
    inc     byte ptr [bx]
loc_26957:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_26670 endp
sub_2695E proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si]
    cmp     [bx+2], ax
    jg      short loc_26974
loc_2696F:
    sub     ax, ax
    pop     si
    pop     bp
    retf
loc_26974:
    mov     bx, [bp+arg_2]
    mov     si, [bp+arg_0]
    mov     ax, [si]
    cmp     [bx+2], ax
    jle     short loc_2696F
    mov     bx, si
    mov     si, [bp+arg_2]
    mov     ax, [si+6]
    cmp     [bx+4], ax
    jge     short loc_2696F
    mov     ax, [si+4]
    cmp     [bx+6], ax
    jle     short loc_2696F
    mov     ax, 1
    pop     si
    pop     bp
    retf
sub_2695E endp
sub_2699C proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+2]
    cmp     [bx+2], ax
    jg      short loc_269CA
    mov     ax, [si]
    cmp     [bx], ax
    jl      short loc_269CA
    mov     ax, [si+4]
    cmp     [bx+4], ax
    jl      short loc_269CA
    mov     ax, [si+6]
    cmp     [bx+6], ax
    jg      short loc_269CA
    mov     ax, 1
    pop     si
    pop     bp
    retf
loc_269CA:
    sub     ax, ax
    pop     si
    pop     bp
    retf
    ; align 2
    db 144
sub_2699C endp
sub_269D0 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+4]
    cmp     [bx+6], ax
    jnz     short loc_26A02
    mov     ax, [si]
    cmp     [bx], ax
    jz      short loc_269EE
loc_269E8:
    sub     ax, ax
    pop     si
    pop     bp
    retf
    ; align 2
    db 144
loc_269EE:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+2]
    cmp     [bx+2], ax
    jnz     short loc_269E8
loc_269FC:
    mov     ax, 1
    pop     si
    pop     bp
    retf
loc_26A02:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si+6]
    cmp     [bx+4], ax
    jnz     short loc_26A20
    mov     ax, [si]
    cmp     [bx], ax
    jnz     short loc_269E8
    mov     ax, [si+2]
    cmp     [bx+2], ax
loc_26A1C:
    jz      short loc_269FC
    jmp     short loc_269E8
loc_26A20:
    mov     bx, [bp+arg_0]
    mov     si, [bp+arg_2]
    mov     ax, [si]
    cmp     [bx+2], ax
    jnz     short loc_26A3E
loc_26A2D:
    mov     ax, [si+4]
    cmp     [bx+4], ax
    jnz     short loc_269E8
    mov     ax, [si+6]
    cmp     [bx+6], ax
    jmp     short loc_26A1C
    ; align 2
    db 144
loc_26A3E:
    mov     bx, [bp+arg_2]
    mov     si, [bp+arg_0]
    mov     ax, [si]
    cmp     [bx+2], ax
    jnz     short loc_269E8
    mov     bx, si
    mov     si, [bp+arg_2]
    jmp     short loc_26A2D
sub_269D0 endp
sub_26A52 proc far
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = byte ptr -24
    var_16 = byte ptr -22
    var_14 = byte ptr -20
    var_C = byte ptr -12
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = word ptr 16
    arg_C = word ptr 18

    push    bp
    mov     bp, sp
    sub     sp, 1Ch
    push    di
    push    si
    mov     [bp+var_18], 0
    jmp     short loc_26AC0
loc_26A60:
    mov     ax, [bp+var_1A]
loc_26A63:
    mov     [bp+var_4], ax
    mov     [bp+var_2], 1
    jmp     short loc_26A86
loc_26A6C:
    test    [bp+var_16], 2
    jz      short loc_26A82
    mov     bx, [bp+var_1C]
    mov     si, bx
    mov     ax, [si]
    cmp     [bx+2], ax
    jle     short loc_26A82
    mov     ax, bx
    jmp     short loc_26A63
loc_26A82:
    mov     [bp+var_2], 0
loc_26A86:
    cmp     [bp+var_2], 0
    jz      short loc_26ABD
    mov     ax, [bp+var_4]
    lea     di, [bp+var_14]
    mov     si, ax
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    push    [bp+arg_8]
    lea     ax, [bp+var_14]
    push    ax
    push    cs
    call    near ptr sub_265EC
    add     sp, 4
    or      al, al
    jnz     short loc_26ABD
    lea     ax, [bp+var_14]
    push    ax
    push    [bp+arg_C]
    push    [bp+arg_A]
    push    cs
    call    near ptr sub_26670
    add     sp, 6
loc_26ABD:
    inc     [bp+var_18]
loc_26AC0:
    mov     al, [bp+arg_0]
    cmp     [bp+var_18], al
    jnb     short loc_26B44
    mov     al, [bp+var_18]
    cbw
    mov     si, ax
    mov     bx, [bp+arg_2]
    mov     al, [bx+si]
    mov     [bp+var_16], al
    test    [bp+var_16], 1
    jz      short loc_26AEA
    mov     ax, [bp+arg_4]
    mov     dx, si
    mov     cl, 3
    shl     dx, cl
    add     ax, dx
    mov     [bp+var_1A], ax
loc_26AEA:
    test    [bp+var_16], 2
    jz      short loc_26AFE
    mov     al, [bp+var_18]
    cbw
    mov     cl, 3
    shl     ax, cl
    add     ax, [bp+arg_6]
    mov     [bp+var_1C], ax
loc_26AFE:
    test    [bp+var_16], 1
    jnz     short loc_26B07
    jmp     loc_26A6C
loc_26B07:
    mov     bx, [bp+var_1A]
    mov     si, bx
    mov     ax, [si]
    cmp     [bx+2], ax
    jg      short loc_26B16
    jmp     loc_26A6C
loc_26B16:
    test    [bp+var_16], 2
    jnz     short loc_26B1F
    jmp     loc_26A60
loc_26B1F:
    mov     bx, [bp+var_1C]
    mov     si, bx
    mov     ax, [si]
    cmp     [bx+2], ax
    jg      short loc_26B2E
    jmp     loc_26A60
loc_26B2E:
    lea     ax, [bp+var_C]
    push    ax
    push    bx
    push    [bp+var_1A]
    push    cs
    call    near ptr sub_26572
    add     sp, 6
    lea     ax, [bp+var_C]
    jmp     loc_26A63
    ; align 2
    db 144
loc_26B44:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_26A52 endp
sub_26B4A proc far
    var_202 = word ptr -514
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 202h
    push    di
    push    si
    cmp     [bp+arg_0], 1
    jle     short loc_26BA0
    sub     si, si
    jmp     short loc_26B7C
    ; align 2
    db 144
loc_26B5E:
    mov     di, si
    shl     di, 1
    mov     bx, [bp+arg_2]
    mov     ax, si
    mov     cl, 3
    shl     ax, cl
    add     bx, ax
    mov     ax, [bx+4]
    neg     ax
    mov     [bp+di+var_202], ax
    mov     bx, [bp+arg_4]
    mov     [bx+di], si
    inc     si
loc_26B7C:
    mov     al, [bp+arg_0]
    cbw
    cmp     ax, si
    jg      short loc_26B5E
    push    [bp+arg_4]
    lea     ax, [bp+var_202]
    push    ax
    mov     al, [bp+arg_0]
    cbw
    push    ax
    call    sub_36BE8
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_26BA0:
    mov     bx, [bp+arg_4]
    mov     word ptr [bx], 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_26B4A endp
seg006 ends
end
