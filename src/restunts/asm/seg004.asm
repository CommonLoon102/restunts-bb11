.model medium
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
    include seg001.inc
    include seg002.inc
    include seg003.inc
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
seg004 segment byte public 'STUNTSC' use16
    assume cs:seg004
    assume es:nothing, ss:nothing, ds:dseg
    public sub_1E1A0
    public off_1F750
    public off_1F87E
    public sub_1FAE4
    public load_game_3dshapes
    public unload_game_resources
    public sub_1FF92
    public sub_20438
    public sub_204AE
    public setup_track
    public off_2147C
    public sub_217CA
    public map_terrain_track
sub_1E1A0 proc far
    var_40 = dword ptr -64
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_36 = word ptr -54
    var_34 = byte ptr -52
    var_32 = word ptr -50
    var_2E = word ptr -46
    var_2C = word ptr -44
    var_28 = word ptr -40
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = byte ptr -32
    var_1E = word ptr -30
    var_1C = word ptr -28
    var_1A = byte ptr -26
    var_18 = word ptr -24
    var_14 = word ptr -20
    var_12 = byte ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = byte ptr -8
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 40h
    push    di
    push    si
    mov     word_44DC6, 0
    mov     word_4428E, 0FFFFh
    mov     word_44A8C, 0FFF4h
    mov     word_44988, 0FC18h
    mov     byte_44D46, 0
    mov     byte_44D47, 4
    mov     byte_4392C, 1
    sub     si, si
    mov     [bp+var_38], si
    mov     [bp+var_10], si
    mov     word_449EC, si
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    mov     cl, 0Ah
    sar     ax, cl
    mov     [bp+var_1A], al
    mov     ax, [bx+4]
    sar     ax, cl
    mov     [bp+var_20], al
    mov     [bp+var_24], 0FFFFh
    cmp     [bp+var_1A], 0
    jge     short loc_1E1FD
    jmp     loc_1F8CD
loc_1E1FD:
    cmp     [bp+var_1A], 1Dh
    jle     short loc_1E206
    jmp     loc_1F8CD
loc_1E206:
    or      al, al
    jge     short loc_1E20D
    jmp     loc_1F8CD
loc_1E20D:
    cmp     al, 1Dh
    jle     short loc_1E214
    jmp     loc_1F8CD
loc_1E214:
    mov     al, [bp+var_1A]
    cbw
    mov     di, ax
    mov     bx, di
    shl     bx, 1
    mov     ax, [bx-599Eh]
    mov     word_449E0, ax
    mov     al, [bp+var_20]
    cbw
    shl     ax, 1
    mov     [bp+var_3C], ax
    mov     bx, ax
    mov     ax, [bx+750Eh]
    mov     word_449F6, ax
    mov     bx, [bx-5A30h]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx+di]
    mov     [bp+var_34], al
    or      al, al
    jz      short loc_1E276
    sub     ah, ah
    cmp     ax, 1
    jnz     short loc_1E257
    jmp     loc_1E2FB
loc_1E257:
    cmp     ax, 2
    jz      short loc_1E29A
    cmp     ax, 3
    jz      short loc_1E2A0
    cmp     ax, 4
    jz      short loc_1E2A6
    cmp     ax, 5
    jz      short loc_1E2AC
    cmp     ax, 6
    jnz     short loc_1E276
    mov     ax, word_3B8EA
    mov     word_449EC, ax
loc_1E276:
    mov     al, [bp+var_20]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, [bx-73C4h]
    mov     al, [bp+var_1A]
    cbw
    add     bx, ax
    les     di, trackdata14
    mov     al, es:[bx+di]
    mov     [bp+var_8], al
    or      al, al
    jnz     short loc_1E304
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1E29A:
    mov     si, 80h ; 'Ä'
    jmp     short loc_1E2AF
    ; align 2
    db 144
loc_1E2A0:
    mov     si, 0FD80h
    jmp     short loc_1E2AF
    ; align 2
    db 144
loc_1E2A6:
    mov     si, 0FE80h
    jmp     short loc_1E2AF
    ; align 2
    db 144
loc_1E2AC:
    mov     si, 0FF80h
loc_1E2AF:
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    sub     ax, word_449E0
    mov     [bp+var_2C], ax
    mov     ax, [bx+4]
    sub     ax, word_449F6
    mov     [bp+var_28], ax
    push    ax
    push    si
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    push    [bp+var_2C]
    push    si
    mov     di, ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, di
    mov     [bp+var_22], ax
    or      ax, ax
    jl      short loc_1E2FB
    jmp     loc_1E276
loc_1E2FB:
    mov     byte_44D47, 5
    jmp     loc_1E276
    ; align 2
    db 144
loc_1E304:
    cmp     [bp+var_8], 0FDh ; '˝'
    jnb     short loc_1E30D
    jmp     loc_1E40C
loc_1E30D:
    mov     al, [bp+var_8]
    sub     ah, ah
    cmp     ax, 0FDh ; '˝'
    jz      short loc_1E328
    cmp     ax, 0FEh ; '˛'
    jz      short loc_1E390
    cmp     ax, 0FFh
    jnz     short loc_1E324
    jmp     loc_1E3CC
loc_1E324:
    jmp     loc_1E464
    ; align 2
    db 144
loc_1E328:
    mov     al, [bp+var_20]
    cbw
    mov     di, ax
    shl     di, 1
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    add     bx, [di-73C2h]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx-1]
    mov     [bp+var_8], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    byte ptr [bx+20A3h], 1
    jz      short loc_1E365
    mov     ax, [di-6CE0h]
loc_1E362:
    mov     word_449F6, ax
loc_1E365:
    mov     al, [bp+var_8]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    byte ptr [bx+20A3h], 2
    jnz     short loc_1E380
    jmp     loc_1E464
loc_1E380:
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+7556h]
    jmp     loc_1E461
    ; align 2
    db 144
loc_1E390:
    mov     al, [bp+var_20]
    cbw
    mov     di, ax
    shl     di, 1
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    add     bx, [di-73C2h]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_8], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    byte ptr [bx+20A3h], 1
    jz      short loc_1E43D
    mov     ax, [di-6CE0h]
    jmp     short loc_1E43A
    ; align 2
    db 144
loc_1E3CC:
    mov     al, [bp+var_20]
    cbw
    mov     di, ax
    shl     di, 1
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    add     bx, [di-73C4h]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx-1]
    mov     [bp+var_8], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    byte ptr [bx+20A3h], 1
    jnz     short loc_1E405
    jmp     loc_1E365
loc_1E405:
    mov     ax, [di-6CE2h]
    jmp     loc_1E362
loc_1E40C:
    mov     al, [bp+var_8]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, [bx+20A3h]
    mov     byte ptr [bp+var_3C], al
    cmp     al, ah
    jz      short loc_1E464
    test    byte ptr [bp+var_3C], 1
    jz      short loc_1E43D
    mov     al, [bp+var_20]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-6CE2h]
loc_1E43A:
    mov     word_449F6, ax
loc_1E43D:
    mov     al, [bp+var_8]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    byte ptr [bx+20A3h], 2
    jz      short loc_1E464
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+7558h]
loc_1E461:
    mov     word_449E0, ax
loc_1E464:
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    sub     ax, word_449E0
    mov     [bp+var_2C], ax
    mov     ax, [bx+4]
    sub     ax, word_449F6
    mov     [bp+var_28], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx]
    sub     ax, word_449E0
    mov     [bp+var_18], ax
    mov     ax, [bx+4]
    sub     ax, word_449F6
    mov     [bp+var_14], ax
    cmp     [bp+var_8], 0
    jz      short loc_1E4B6
    cmp     [bp+var_34], 7
    jb      short loc_1E4B6
    cmp     [bp+var_34], 0Bh
    jnb     short loc_1E4B6
    mov     al, [bp+var_8]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_34]
    push    ax
    push    cs
    call    near ptr map_terrain_track
    add     sp, 4
    mov     [bp+var_8], al
loc_1E4B6:
    mov     al, [bp+var_8]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_3A], ax
    mov     bx, ax
    mov     al, [bx+0Ch]
    cbw
    mov     [bp+var_24], ax
    mov     ax, [bx+2]
    mov     [bp+var_10], ax
    or      ax, ax
    jz      short loc_1E4EF
    cmp     ax, 100h
    jz      short loc_1E562
    cmp     ax, 200h
    jz      short loc_1E540
    cmp     ax, 300h
    jz      short loc_1E516
loc_1E4EF:
    mov     [bp+var_36], 0
    mov     bx, [bp+var_3A]
    mov     al, [bx+9]
    inc     al
    mov     [bp+var_12], al
    cmp     al, 1
    jge     short loc_1E507
    mov     [bp+var_12], 1
loc_1E507:
    cmp     [bp+var_2C], 0
    jge     short loc_1E58A
    mov     ax, [bp+var_2C]
    neg     ax
    jmp     short loc_1E58D
    db 144
    db 144
loc_1E516:
    mov     ax, [bp+var_2C]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_28]
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_1E]
    neg     ax
    mov     [bp+var_28], ax
    mov     ax, [bp+var_18]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_14]
    mov     [bp+var_18], ax
    mov     ax, [bp+var_1E]
    neg     ax
loc_1E53B:
    mov     [bp+var_14], ax
    jmp     short loc_1E4EF
loc_1E540:
    mov     ax, [bp+var_28]
    neg     ax
    mov     [bp+var_28], ax
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_14]
    neg     ax
    mov     [bp+var_14], ax
    mov     ax, [bp+var_18]
    neg     ax
    mov     [bp+var_18], ax
    jmp     short loc_1E4EF
loc_1E562:
    mov     ax, [bp+var_2C]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_28]
    neg     ax
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_1E]
    mov     [bp+var_28], ax
    mov     ax, [bp+var_18]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_14]
    neg     ax
    mov     [bp+var_18], ax
    mov     ax, [bp+var_1E]
    jmp     short loc_1E53B
    ; align 2
    db 144
loc_1E58A:
    mov     ax, [bp+var_2C]
loc_1E58D:
    mov     [bp+var_32], ax
    cmp     [bp+var_28], 0
    jge     short loc_1E59E
    mov     ax, [bp+var_28]
    neg     ax
    jmp     short loc_1E5A1
    ; align 2
    db 144
loc_1E59E:
    mov     ax, [bp+var_28]
loc_1E5A1:
    mov     [bp+var_2E], ax
    mov     ax, [bp+var_24]
    cmp     ax, 4Ah ; 'J'
    jbe     short loc_1E5AF
    jmp     loc_1F7E6
loc_1E5AF:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_1F750[bx]
    ; align 2
    db 144
loc_1E5B8:
    cmp     state.field_3F5, 0
    jnz     short loc_1E5E1
    cmp     [bp+var_2C], 0
    jle     short loc_1E5E1
    cmp     [bp+var_28], 0FE84h
    jge     short loc_1E5D4
    mov     word_44DC6, 83h ; 'É'
    jmp     short loc_1E5E1
loc_1E5D4:
    cmp     [bp+var_28], 0FED4h
    jge     short loc_1E5E1
    mov     word_44DC6, 84h ; 'Ñ'
loc_1E5E1:
    cmp     [bp+var_32], 78h ; 'x'
loc_1E5E5:
    jl      short loc_1E5EA
    jmp     loc_1F7E6
loc_1E5EA:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1E5F4:
    cmp     [bp+var_32], 78h ; 'x'
    jl      short loc_1E5EA
    cmp     [bp+var_2E], 78h ; 'x'
    jmp     short loc_1E5E5
loc_1E600:
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_2C], ax
loc_1E608:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     [bp+var_2C], 0
    jle     short loc_1E624
    mov     ax, [bp+var_28]
    neg     ax
    mov     [bp+var_28], ax
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_2C], ax
loc_1E624:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 4
loc_1E631:
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_A], ax
    cmp     ax, 588h
    jg      short loc_1E645
    jmp     loc_1F7E6
loc_1E645:
    cmp     ax, 678h
    jmp     short loc_1E5E5
loc_1E64A:
    cmp     [bp+var_32], 78h ; 'x'
    jl      short loc_1E5EA
loc_1E650:
    mov     ax, [bp+var_28]
    add     ah, 2
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 2
loc_1E65D:
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_A], ax
    cmp     ax, 188h
    jg      short loc_1E671
    jmp     loc_1F7E6
loc_1E671:
    cmp     ax, 278h
    jmp     loc_1E5E5
    ; align 2
    db 144
loc_1E678:
    cmp     [bp+var_32], 78h ; 'x'
    jge     short loc_1E681
    jmp     loc_1E5EA
loc_1E681:
    mov     ax, [bp+var_28]
    add     ah, 2
    push    ax
    mov     ax, 200h
    sub     ax, [bp+var_2C]
    jmp     short loc_1E65D
loc_1E690:
    cmp     [bp+var_2C], 188h
    jl      short loc_1E624
    cmp     [bp+var_2C], 278h
    jg      short loc_1E6A1
    jmp     loc_1E5EA
loc_1E6A1:
    jmp     short loc_1E624
    ; align 2
    db 144
loc_1E6A4:
    cmp     [bp+var_2C], 0FD88h
    jl      short loc_1E6B5
    cmp     [bp+var_2C], 0FE78h
    jg      short loc_1E6B5
    jmp     loc_1E5EA
loc_1E6B5:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, 400h
    sub     ax, [bp+var_2C]
    jmp     loc_1E631
    ; align 2
    db 144
loc_1E6C6:
    cmp     [bp+var_2C], 0
    jge     short loc_1E6D4
    mov     ax, [bp+var_2C]
    neg     ax
    jmp     short loc_1E6D7
    ; align 2
    db 144
loc_1E6D4:
    mov     ax, [bp+var_2C]
loc_1E6D7:
    mov     [bp+var_1C], ax
    sub     si, si
    jmp     short loc_1E6DF
loc_1E6DE:
    inc     si
loc_1E6DF:
    mov     bx, si
    shl     bx, 1
    mov     ax, [bp+var_28]
    cmp     [bx+2EA8h], ax
    jl      short loc_1E6DE
    mov     di, si
    shl     di, 1
    mov     ax, [di+2EB4h]
    cmp     [di+2EB6h], ax
    jz      short loc_1E72E
    mov     di, si
    shl     di, 1
    mov     ax, [di+2EA8h]
    sub     ax, [di+2EA6h]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_28]
    sub     ax, [di+2EA6h]
    cwd
    push    dx
    push    ax
    mov     ax, [di+2EB6h]
    sub     ax, [di+2EB4h]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, [di+2EB4h]
loc_1E72E:
    mov     [bp+var_1E], ax
    mov     di, si
    shl     di, 1
    mov     ax, [di+2EC2h]
    cmp     [di+2EC4h], ax
    jz      short loc_1E773
    mov     di, si
    shl     di, 1
    mov     ax, [di+2EA8h]
    sub     ax, [di+2EA6h]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_28]
    sub     ax, [di+2EA6h]
    cwd
    push    dx
    push    ax
    mov     ax, [di+2EC4h]
    sub     ax, [di+2EC2h]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, [di+2EC2h]
loc_1E773:
    mov     [bp+var_22], ax
    mov     ax, [bp+var_1E]
    cmp     [bp+var_1C], ax
    jle     short loc_1E789
    mov     ax, [bp+var_22]
    cmp     [bp+var_1C], ax
    jge     short loc_1E789
    jmp     loc_1E5EA
loc_1E789:
    cmp     [bp+var_28], 0
    jge     short loc_1E792
    jmp     loc_1F7E6
loc_1E792:
    cmp     [bp+var_1C], 78h ; 'x'
    jle     short loc_1E79B
    jmp     loc_1F7E6
loc_1E79B:
    mov     word_44DC6, 1
    cmp     [bp+var_28], 14Eh
    jl      short loc_1E7C0
    cmp     [bp+var_18], 0FF88h
    jle     short loc_1E7F9
loc_1E7AE:
    cmp     [bp+var_18], 78h ; 'x'
    jge     short loc_1E7B7
    jmp     loc_1F7E6
loc_1E7B7:
    mov     word_4428E, 0BAh ; '∫'
    jmp     loc_1F7E6
loc_1E7C0:
    cmp     [bp+var_18], 0
    jl      short loc_1E7D0
    mov     word_4428E, 0BBh ; 'ª'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1E7D0:
    mov     word_4428E, 0BDh ; 'Ω'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1E7DA:
    cmp     [bp+var_32], 168h
    jle     short loc_1E7E4
    jmp     loc_1F7E6
loc_1E7E4:
    cmp     [bp+var_32], 78h ; 'x'
    jle     short loc_1E7ED
    jmp     loc_1E5EA
loc_1E7ED:
    mov     word_44DC6, 1
    cmp     [bp+var_18], 0FF88h
    jg      short loc_1E7AE
loc_1E7F9:
    mov     word_4428E, 0BCh ; 'º'
    jmp     loc_1F7E6
loc_1E802:
    cmp     [bp+var_28], 0
    jle     short loc_1E810
    mov     byte_4392C, 0
    jmp     short loc_1E82B
    ; align 2
    db 144
loc_1E810:
    cmp     [bp+var_14], 0
    jl      short loc_1E82B
    mov     word_4428E, 66h ; 'f'
    jmp     short loc_1E82B
loc_1E81E:
    cmp     [bp+var_14], 1DCh
    jl      short loc_1E82B
    mov     word_4428E, 67h ; 'g'
loc_1E82B:
    cmp     [bp+var_18], 0
    jge     short loc_1E838
    mov     ax, [bp+var_18]
    neg     ax
    jmp     short loc_1E83B
loc_1E838:
    mov     ax, [bp+var_18]
loc_1E83B:
    cmp     ax, 78h ; 'x'
    jge     short loc_1E886
    mov     word_44DC6, 3
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     word_4428E, 0FFFFh
    jz      short loc_1E856
    jmp     loc_1F7E6
loc_1E856:
    cmp     [bp+var_28], 0
    jge     short loc_1E85F
    jmp     loc_1F7E6
loc_1E85F:
    cmp     [bp+var_32], 78h ; 'x'
    jge     short loc_1E868
    jmp     loc_1F7E6
loc_1E868:
    mov     word_44A8C, 2Ah ; '*'
    mov     word_44988, 0FFF4h
    cmp     [bp+var_2C], 0
    jge     short loc_1E87D
    jmp     loc_1E96F
loc_1E87D:
    mov     word_4428E, 65h ; 'e'
    jmp     loc_1F7E6
loc_1E886:
    cmp     byte_4392C, 0
    jnz     short loc_1E890
    jmp     loc_1F7E6
loc_1E890:
    cmp     [bp+var_32], 78h ; 'x'
    jle     short loc_1E899
    jmp     loc_1F7E6
loc_1E899:
    mov     word_44DC6, 3
    cmp     word_4428E, 0FFFFh
    jz      short loc_1E8A9
    jmp     loc_1F7E6
loc_1E8A9:
    mov     [bp+var_38], 200h
loc_1E8AE:
    cmp     [bp+var_2C], 0
    jmp     loc_1E96A
    ; align 2
    db 144
loc_1E8B6:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 186h
    jle     short loc_1E8D8
loc_1E8C5:
    mov     byte_4392C, 0
loc_1E8CA:
    cmp     [bp+var_18], 0
    jge     short loc_1E8F8
    mov     ax, [bp+var_18]
    neg     ax
    jmp     short loc_1E8FB
    ; align 2
    db 144
loc_1E8D8:
    cmp     [bp+var_2E], 78h ; 'x'
loc_1E8DC:
    jle     short loc_1E8E1
    jmp     loc_1F7E6
loc_1E8E1:
    jmp     loc_1E5EA
loc_1E8E4:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 186h
    jg      short loc_1E8F6
    jmp     loc_1F7E6
loc_1E8F6:
    jmp     short loc_1E8C5
loc_1E8F8:
    mov     ax, [bp+var_18]
loc_1E8FB:
    cmp     ax, 78h ; 'x'
    jg      short loc_1E942
    mov     word_44DC6, 2
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     byte_4392C, 0
    jz      short loc_1E92F
    cmp     [bp+var_14], 1DCh
    jl      short loc_1E922
    mov     word_4428E, 67h ; 'g'
    jmp     short loc_1E92F
loc_1E922:
    cmp     [bp+var_14], 0FE24h
    jg      short loc_1E92F
    mov     word_4428E, 68h ; 'h'
loc_1E92F:
    cmp     [bp+var_32], 78h ; 'x'
    jge     short loc_1E938
    jmp     loc_1F7E6
loc_1E938:
    mov     word_44A8C, 2Ah ; '*'
    jmp     loc_1E8AE
    ; align 2
    db 144
loc_1E942:
    cmp     byte_4392C, 0
    jnz     short loc_1E94C
    jmp     loc_1F7E6
loc_1E94C:
    cmp     [bp+var_32], 78h ; 'x'
    jle     short loc_1E955
    jmp     loc_1F7E6
loc_1E955:
    mov     word_44DC6, 2
    mov     word_44A8C, 2Ah ; '*'
    mov     [bp+var_38], 200h
    cmp     [bp+var_18], 0
loc_1E96A:
    jl      short loc_1E96F
    jmp     loc_1E87D
loc_1E96F:
    mov     word_4428E, 64h ; 'd'
    jmp     loc_1F7E6
loc_1E978:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 186h
    jg      short loc_1E98A
    jmp     loc_1F7E6
loc_1E98A:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 4
    push    ax
    call    sub_300B6
    add     sp, 4
    sub     ax, 600h
    mov     [bp+var_A], ax
    cmp     ax, 0FF6Ah
    jg      short loc_1E9AE
    jmp     loc_1F7E6
loc_1E9AE:
    cmp     ax, 96h ; 'ñ'
    jl      short loc_1E9B6
    jmp     loc_1F7E6
loc_1E9B6:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     word_44DC6, 2
    mov     byte_4392C, 0
    cmp     [bp+var_A], 0FF94h
    jl      short loc_1E9D6
    cmp     [bp+var_A], 6Ch ; 'l'
    jg      short loc_1E9D6
    jmp     loc_1F7E6
loc_1E9D6:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 4
    push    ax
    call    sub_2EA4E
    add     sp, 4
    sub     ah, ah
    mov     cx, 12h
    imul    cx
    mov     [bp+var_22], ax
    mov     cl, 8
    sar     ax, cl
    sub     ax, 11h
    neg     ax
    mov     [bp+var_1E], ax
    mov     word_44A8C, 2Ah ; '*'
    mov     word_44988, 0FFF4h
    cmp     [bp+var_A], 0
    jge     short loc_1EA1A
    add     ax, 69h ; 'i'
    jmp     short loc_1EA20
    ; align 2
    db 144
loc_1EA1A:
    mov     ax, [bp+var_1E]
    add     ax, 7Bh ; '{'
loc_1EA20:
    mov     word_4428E, ax
    jmp     loc_1F7E6
loc_1EA26:
    mov     [bp+var_1C], 23h ; '#'
    mov     [bp+var_1E], 0
    mov     si, 0FD60h
    jmp     short loc_1EA43
    ; align 2
    db 144
loc_1EA36:
    mov     [bp+var_1C], 19h
    mov     [bp+var_1E], 1
    mov     si, 0A0h ; '†'
loc_1EA43:
    cmp     [bp+var_32], 78h ; 'x'
    jle     short loc_1EA4C
    jmp     loc_1F7E6
loc_1EA4C:
    cmp     [bp+var_1E], 0
    jnz     short loc_1EA66
    cmp     [bp+var_18], 0FF88h
    jg      short loc_1EA66
    mov     [bp+var_38], 200h
    mov     word_4428E, 64h ; 'd'
    jmp     short loc_1EA7D
    ; align 2
    db 144
loc_1EA66:
    cmp     [bp+var_1E], 0
    jz      short loc_1EA7D
    cmp     [bp+var_18], 78h ; 'x'
    jl      short loc_1EA7D
    mov     [bp+var_38], 200h
    mov     word_4428E, 65h ; 'e'
loc_1EA7D:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     [bp+var_28], 0FEB2h
    jge     short loc_1EA90
    mov     ax, [bp+var_1C]
    jmp     short loc_1EA9D
    ; align 2
    db 144
loc_1EA90:
    cmp     [bp+var_28], 14Eh
    jl      short loc_1EAA4
    mov     ax, [bp+var_1C]
    add     ax, 9
loc_1EA9D:
    mov     word_44DC6, ax
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EAA4:
    cmp     [bp+var_28], 0FF58h
    jge     short loc_1EABA
    mov     ax, [bp+var_1C]
    inc     ax
    mov     word_44DC6, ax
    mov     [bp+var_1E], 0
    jmp     short loc_1EAFD
    ; align 2
    db 144
loc_1EABA:
    cmp     [bp+var_28], 0
    jge     short loc_1EAD0
    mov     ax, [bp+var_1C]
    add     ax, 3
    mov     word_44DC6, ax
    mov     [bp+var_1E], 1
    jmp     short loc_1EAFD
loc_1EAD0:
    cmp     [bp+var_28], 0A8h ; '®'
    jge     short loc_1EAE8
    mov     ax, [bp+var_1C]
    add     ax, 5
    mov     word_44DC6, ax
    mov     [bp+var_1E], 2
    jmp     short loc_1EAFD
    ; align 2
    db 144
loc_1EAE8:
    cmp     [bp+var_28], 14Eh
    jge     short loc_1EAFD
    mov     ax, [bp+var_1C]
    add     ax, 7
    mov     word_44DC6, ax
    mov     [bp+var_1E], 3
loc_1EAFD:
    mov     ax, [bp+var_28]
    mov     bx, [bp+var_1E]
    shl     bx, 1
    sub     ax, [bx+2E6Eh]
    push    ax
    push    si
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    push    [bp+var_2C]
    push    si
    mov     di, ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, di
    mov     [bp+var_22], ax
    or      ax, ax
    jg      short loc_1EB3F
    jmp     loc_1F7E6
loc_1EB3F:
    inc     word_44DC6
    jmp     loc_1F7E6
loc_1EB46:
    cmp     [bp+var_32], 78h ; 'x'
    jle     short loc_1EB4F
    jmp     loc_1F7E6
loc_1EB4F:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     word_44DC6, 6
    cmp     [bp+var_18], 78h ; 'x'
    jge     short loc_1EB64
    jmp     loc_1F7E6
loc_1EB64:
    mov     [bp+var_38], 200h
    jmp     loc_1E87D
loc_1EB6C:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 4
    push    ax
    call    sub_300B6
    add     sp, 4
    sub     ax, 600h
    mov     [bp+var_A], ax
    cmp     ax, 0FF88h
    jg      short loc_1EB90
    jmp     loc_1F7E6
loc_1EB90:
    cmp     ax, 7Eh ; '~'
    jl      short loc_1EB98
    jmp     loc_1F7E6
loc_1EB98:
    mov     ax, [bp+var_28]
    add     ah, 4
    push    ax
    mov     ax, [bp+var_2C]
    add     ah, 4
    push    ax
    call    sub_2EA4E
    add     sp, 4
    sub     ah, ah
    mov     cx, 12h
    imul    cx
    mov     [bp+var_22], ax
    mov     cl, 8
    sar     ax, cl
    sub     ax, 11h
    neg     ax
    mov     [bp+var_1E], ax
    add     ax, 7
    mov     word_44DC6, ax
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     [bp+var_A], 66h ; 'f'
    jg      short loc_1EBD9
    jmp     loc_1F7E6
loc_1EBD9:
    mov     [bp+var_38], 200h
    mov     ax, [bp+var_1E]
    add     ax, 7Bh ; '{'
    mov     word_4428E, ax
loc_1EBE7:
    mov     byte_4392C, 0
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EBF0:
    cmp     [bp+var_28], 0
    jge     short loc_1EC0A
    mov     [bp+var_1C], 33h ; '3'
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_6], ax
    mov     ax, [bp+var_28]
    neg     ax
    jmp     short loc_1EC18
loc_1EC0A:
    mov     [bp+var_1C], 2Dh ; '-'
    mov     ax, [bp+var_2C]
    mov     [bp+var_6], ax
    mov     ax, [bp+var_28]
loc_1EC18:
    mov     [bp+var_2], ax
    mov     ax, word_3E590
    dec     ax
    cmp     [bp+var_2], ax
    jle     short loc_1EC38
    mov     ax, word_3E590
    add     ax, 64h ; 'd'
    cmp     [bp+var_2], ax
    jle     short loc_1EC32
    jmp     loc_1EDAD
loc_1EC32:
    mov     ax, word_3E590
    dec     ax
    jmp     short loc_1EC3B
loc_1EC38:
    mov     ax, [bp+var_2]
loc_1EC3B:
    mov     [bp+var_1E], ax
    sub     si, si
    jmp     short loc_1EC43
loc_1EC42:
    inc     si
loc_1EC43:
    mov     bx, si
    shl     bx, 1
    mov     ax, [bp+var_1E]
    cmp     [bx+2E1Ch], ax
    jl      short loc_1EC42
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 20Ch
    jg      short loc_1EC62
    jmp     loc_1ED04
loc_1EC62:
    mov     ax, 5
    sub     ax, si
    mov     si, ax
    mov     di, si
    shl     di, 1
    mov     ax, [bp+var_6]
    cmp     word_3E598[di], ax
    jle     short loc_1EC79
    jmp     loc_1F7E6
loc_1EC79:
    mov     ax, word_3E59A[di]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jge     short loc_1EC88
    jmp     loc_1F7E6
loc_1EC88:
    mov     ax, [bp+var_6]
    cmp     word_3E59A[di], ax
    jge     short loc_1EC9D
    mov     ax, word_3E598[di]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jg      short loc_1ECF3
loc_1EC9D:
    mov     di, si
    shl     di, 1
    mov     ax, word_3E58C[di]
    sub     ax, word_3E58A[di]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E58A[di]
    sub     ax, [bp+var_1E]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E598[di]
    sub     ax, word_3E59A[di]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_22], ax
    mov     ax, word_3E598[di]
    add     ax, [bp+var_22]
    mov     [bp+var_3C], ax
    mov     ax, [bp+var_6]
    cmp     [bp+var_3C], ax
    jl      short loc_1ECE5
    jmp     loc_1F7E6
loc_1ECE5:
    mov     ax, [bp+var_3C]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jg      short loc_1ECF3
    jmp     loc_1F7E6
loc_1ECF3:
    mov     ax, [bp+var_1C]
    add     ax, si
    mov     word_44DC6, ax
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    jmp     loc_1EBE7
loc_1ED04:
    cmp     si, 1
    jle     short loc_1ED1B
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 64h ; 'd'
    jge     short loc_1ED1B
    jmp     loc_1EDAD
loc_1ED1B:
    mov     di, si
    shl     di, 1
    mov     ax, [bp+var_6]
    cmp     word_3E598[di], ax
    jle     short loc_1ED2B
    jmp     loc_1EDAD
loc_1ED2B:
    mov     ax, word_3E59A[di]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jl      short loc_1EDAD
    mov     ax, [bp+var_6]
    cmp     word_3E59A[di], ax
    jge     short loc_1ED4C
    mov     ax, word_3E598[di]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jg      short loc_1ECF3
loc_1ED4C:
    mov     di, si
    shl     di, 1
    mov     ax, word_3E598[di]
    cmp     word_3E59A[di], ax
    jz      short loc_1EDAD
    mov     di, si
    shl     di, 1
    mov     ax, word_3E58C[di]
    sub     ax, word_3E58A[di]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E58A[di]
    sub     ax, [bp+var_1E]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E598[di]
    sub     ax, word_3E59A[di]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_22], ax
    mov     ax, word_3E598[di]
    add     ax, [bp+var_22]
    mov     [bp+var_3C], ax
    mov     ax, [bp+var_6]
    cmp     [bp+var_3C], ax
    jge     short loc_1EDAD
    mov     ax, [bp+var_3C]
    add     ax, 190h
    cmp     ax, [bp+var_6]
    jle     short loc_1EDAD
    jmp     loc_1ECF3
loc_1EDAD:
    sub     si, si
    jmp     short loc_1EDB3
    ; align 2
    db 144
loc_1EDB2:
    inc     si
loc_1EDB3:
    mov     bx, si
    shl     bx, 1
    mov     ax, [bp+var_2]
    cmp     word_3E5B6[bx], ax
    jl      short loc_1EDB2
    mov     di, si
    shl     di, 1
    mov     ax, word_3E5B6[di]
    sub     ax, word_3E5B4[di]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_2]
    sub     ax, word_3E5B4[di]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E5C4[di]
    sub     ax, word_3E5C2[di]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, word_3E5C2[di]
    mov     [bp+var_1E], ax
    mov     ax, word_3E5B6[di]
    sub     ax, word_3E5B4[di]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_2]
    sub     ax, word_3E5B4[di]
    cwd
    push    dx
    push    ax
    mov     ax, word_3E5D2[di]
    sub     ax, word_3E5D0[di]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, word_3E5D0[di]
    mov     [bp+var_22], ax
    mov     ax, [bp+var_1E]
    cmp     [bp+var_6], ax
    jge     short loc_1EE35
    jmp     loc_1F7E6
loc_1EE35:
    mov     ax, [bp+var_22]
    cmp     [bp+var_6], ax
    jmp     loc_1E8DC
loc_1EE3E:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 90h ; 'ê'
    jge     short loc_1EE5C
    mov     bx, [bp+arg_2]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 90h ; 'ê'
    jl      short loc_1EE76
loc_1EE5C:
    cmp     [bp+var_32], 10Eh
    jl      short loc_1EE66
    jmp     loc_1F7E6
loc_1EE66:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     word_44DC6, 85h ; 'Ö'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EE76:
    cmp     [bp+var_32], 78h ; 'x'
    jge     short loc_1EE82
    mov     al, [bp+var_12]
    mov     byte_44D47, al
loc_1EE82:
    cmp     [bp+var_2C], 78h ; 'x'
    jl      short loc_1EEC6
    cmp     [bp+var_2C], 10Eh
    jg      short loc_1EEC6
    mov     word_44A8C, 90h ; 'ê'
    cmp     [bp+var_14], 0FE00h
    jle     short loc_1EEE6
    cmp     [bp+var_14], 200h
    jge     short loc_1EEF7
    cmp     [bp+var_18], 78h ; 'x'
    jg      short loc_1EEB2
    mov     word_4428E, 98h ; 'ò'
    jmp     loc_1F7E6
loc_1EEB2:
    cmp     [bp+var_18], 10Eh
    jge     short loc_1EEBC
    jmp     loc_1F7E6
loc_1EEBC:
    mov     word_4428E, 96h ; 'ñ'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EEC6:
    cmp     [bp+var_2C], 0FF88h
    jle     short loc_1EECF
    jmp     loc_1F7E6
loc_1EECF:
    cmp     [bp+var_2C], 0FEF2h
    jge     short loc_1EED9
    jmp     loc_1F7E6
loc_1EED9:
    mov     word_44A8C, 90h ; 'ê'
    cmp     [bp+var_14], 0FE00h
    jg      short loc_1EEF0
loc_1EEE6:
    mov     word_4428E, 9Ah ; 'ö'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EEF0:
    cmp     [bp+var_14], 200h
    jl      short loc_1EF00
loc_1EEF7:
    mov     word_4428E, 99h ; 'ô'
    jmp     loc_1F7E6
loc_1EF00:
    cmp     [bp+var_18], 0FF88h
    jl      short loc_1EF10
    mov     word_4428E, 97h ; 'ó'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EF10:
    cmp     [bp+var_18], 0FEF2h
    jle     short loc_1EF1A
    jmp     loc_1F7E6
loc_1EF1A:
    mov     word_4428E, 95h ; 'ï'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EF24:
    cmp     [bp+var_18], 0
    jge     short loc_1EF32
    mov     ax, [bp+var_18]
    neg     ax
    jmp     short loc_1EF35
    ; align 2
    db 144
loc_1EF32:
    mov     ax, [bp+var_18]
loc_1EF35:
    cmp     ax, 73h ; 's'
    jl      short loc_1EF60
    cmp     [bp+var_32], 0A4h ; '§'
    jg      short loc_1EF60
    mov     word_44A8C, 97h ; 'ó'
    cmp     [bp+var_18], 0
    jle     short loc_1EF56
    mov     word_4428E, 9Fh ; 'ü'
    jmp     loc_1F7E6
loc_1EF56:
    mov     word_4428E, 0A0h ; '†'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1EF60:
    cmp     [bp+var_32], 73h ; 's'
    jl      short loc_1EF69
    jmp     loc_1F7E6
loc_1EF69:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 0ABh ; '´'
    jl      short loc_1EF7B
    jmp     loc_1F7E6
loc_1EF7B:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    cmp     [bp+var_32], 1Fh
    jge     short loc_1EF90
    mov     word_44DC6, 46h ; 'F'
    jmp     loc_1F7E6
loc_1EF90:
    cmp     [bp+var_2C], 0FFACh
    jge     short loc_1EFA6
    mov     word_44DC6, 49h ; 'I'
    mov     [bp+var_1E], 0FF9Ch
    mov     si, 0FFFBh
    jmp     short loc_1EFE0
loc_1EFA6:
    cmp     [bp+var_2C], 0
    jge     short loc_1EFBC
    mov     word_44DC6, 47h ; 'G'
    mov     [bp+var_1E], 0FFC7h
    mov     si, 0FFF8h
    jmp     short loc_1EFE0
loc_1EFBC:
    cmp     [bp+var_2C], 54h ; 'T'
    jle     short loc_1EFD2
    mov     word_44DC6, 4Dh ; 'M'
    mov     [bp+var_1E], 64h ; 'd'
    mov     si, 5
    jmp     short loc_1EFE0
loc_1EFD2:
    mov     word_44DC6, 4Bh ; 'K'
    mov     [bp+var_1E], 39h ; '9'
    mov     si, 8
loc_1EFE0:
    push    [bp+var_28]
    push    si
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    mov     cx, [bp+var_2C]
    sub     cx, [bp+var_1E]
    push    cx
    push    si
    mov     di, ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, di
    mov     [bp+var_22], ax
    or      ax, ax
    jl      short loc_1F01C
    jmp     loc_1F7E6
loc_1F01C:
    jmp     loc_1EB3F
    ; align 2
    db 144
loc_1F020:
    mov     [bp+var_22], 1
    jmp     short loc_1F02D
    ; align 2
    db 144
loc_1F028:
    mov     [bp+var_22], 0
loc_1F02D:
    cmp     [bp+var_18], 0
    jge     short loc_1F03A
    mov     ax, [bp+var_18]
    neg     ax
    jmp     short loc_1F03D
loc_1F03A:
    mov     ax, [bp+var_18]
loc_1F03D:
    cmp     ax, 0A4h ; '§'
    jl      short loc_1F068
    cmp     [bp+var_32], 0A4h ; '§'
    jg      short loc_1F068
    mov     word_44A8C, 97h ; 'ó'
    cmp     [bp+var_18], 0
    jle     short loc_1F05E
    mov     word_4428E, 9Bh ; 'õ'
    jmp     loc_1F7E6
loc_1F05E:
    mov     word_4428E, 9Ch ; 'ú'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F068:
    cmp     [bp+var_32], 0A4h ; '§'
    jl      short loc_1F072
    jmp     loc_1F7E6
loc_1F072:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 109h
    jl      short loc_1F084
    jmp     loc_1F7E6
loc_1F084:
    cmp     [bp+var_32], 82h ; 'Ç'
    jge     short loc_1F091
    mov     al, [bp+var_12]
    mov     byte_44D47, al
loc_1F091:
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 97h ; 'ó'
    jle     short loc_1F0A4
    mov     [bp+var_1E], 1
    jmp     short loc_1F0A9
loc_1F0A4:
    mov     [bp+var_1E], 0
loc_1F0A9:
    cmp     [bp+var_22], 0
    jz      short loc_1F0E8
    cmp     [bp+var_1E], 0
    jnz     short loc_1F0E8
    cmp     [bp+var_32], 54h ; 'T'
    jg      short loc_1F0E8
    cmp     [bp+var_2E], 4Bh ; 'K'
    jg      short loc_1F0E8
    mov     word_44DC6, 45h ; 'E'
    cmp     [bp+var_14], 0FFB5h
    jg      short loc_1F0D6
    mov     word_4428E, 9Dh ; 'ù'
    jmp     loc_1F7E6
loc_1F0D6:
    cmp     [bp+var_14], 4Bh ; 'K'
    jge     short loc_1F0DF
    jmp     loc_1F7E6
loc_1F0DF:
    mov     word_4428E, 9Eh ; 'û'
    jmp     loc_1F7E6
loc_1F0E8:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 58h ; 'X'
    jle     short loc_1F116
    cmp     [bp+var_1E], 0
    jnz     short loc_1F116
    cmp     [bp+var_2C], 0
    jge     short loc_1F10C
    mov     word_44DC6, 3Ch ; '<'
    jmp     loc_1F7E6
loc_1F10C:
    mov     word_44DC6, 42h ; 'B'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F116:
    cmp     [bp+var_32], 1Fh
    jge     short loc_1F136
    cmp     [bp+var_1E], 0
    jz      short loc_1F12C
    mov     word_44DC6, 3Fh ; '?'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F12C:
    mov     word_44DC6, 39h ; '9'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F136:
    cmp     [bp+var_2C], 0FFACh
    jge     short loc_1F156
    cmp     [bp+var_1E], 0
    jz      short loc_1F14C
    mov     word_44DC6, 3Dh ; '='
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F14C:
    mov     word_44DC6, 3Bh ; ';'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F156:
    cmp     [bp+var_2C], 0
    jge     short loc_1F176
    cmp     [bp+var_1E], 0
    jz      short loc_1F16C
    mov     word_44DC6, 3Eh ; '>'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F16C:
    mov     word_44DC6, 3Ah ; ':'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F176:
    cmp     [bp+var_2C], 54h ; 'T'
    jle     short loc_1F196
    cmp     [bp+var_1E], 0
    jz      short loc_1F18C
    mov     word_44DC6, 41h ; 'A'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F18C:
    mov     word_44DC6, 43h ; 'C'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F196:
    cmp     [bp+var_1E], 0
    jz      short loc_1F1A6
    mov     word_44DC6, 40h ; '@'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F1A6:
    mov     word_44DC6, 44h ; 'D'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F1B0:
    cmp     [bp+var_32], 96h ; 'ñ'
    jl      short loc_1F1BA
    jmp     loc_1F7E6
loc_1F1BA:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 109h
    jl      short loc_1F1CC
    jmp     loc_1F7E6
loc_1F1CC:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 97h ; 'ó'
    jle     short loc_1F1E6
    mov     [bp+var_1E], 1
    jmp     short loc_1F1EB
    ; align 2
    db 144
loc_1F1E6:
    mov     [bp+var_1E], 0
loc_1F1EB:
    mov     [bp+var_22], 0
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 58h ; 'X'
    jle     short loc_1F21A
    cmp     [bp+var_1E], 0
    jnz     short loc_1F21A
    cmp     [bp+var_2C], 0
    jge     short loc_1F212
    mov     [bp+var_22], 3
    jmp     loc_1F295
    db 144
    db 144
loc_1F212:
    mov     [bp+var_22], 9
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F21A:
    cmp     [bp+var_32], 1Fh
    jge     short loc_1F22E
    cmp     [bp+var_1E], 0
    jz      short loc_1F295
    mov     [bp+var_22], 6
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F22E:
    cmp     [bp+var_2C], 0FFACh
    jge     short loc_1F24A
    cmp     [bp+var_1E], 0
    jz      short loc_1F242
    mov     [bp+var_22], 4
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F242:
    mov     [bp+var_22], 2
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F24A:
    cmp     [bp+var_2C], 0
    jge     short loc_1F266
    cmp     [bp+var_1E], 0
    jz      short loc_1F25E
    mov     [bp+var_22], 5
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F25E:
    mov     [bp+var_22], 1
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F266:
    cmp     [bp+var_2C], 54h ; 'T'
    jle     short loc_1F282
    cmp     [bp+var_1E], 0
    jz      short loc_1F27A
    mov     [bp+var_22], 8
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F27A:
    mov     [bp+var_22], 0Ah
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F282:
    cmp     [bp+var_1E], 0
    jz      short loc_1F290
    mov     [bp+var_22], 7
    jmp     short loc_1F295
    ; align 2
    db 144
loc_1F290:
    mov     [bp+var_22], 0Bh
loc_1F295:
    cmp     [bp+var_22], 0
    jz      short loc_1F2B8
    mov     di, [bp+var_22]
    shl     di, 1
    mov     ax, [bp+var_28]
    cmp     [di+2E76h], ax
    jge     short loc_1F2B8
    cmp     [di+2E8Eh], ax
    jle     short loc_1F2B8
    mov     ax, [bp+var_22]
    add     ax, 39h ; '9'
    mov     word_44DC6, ax
loc_1F2B8:
    cmp     word_44DC6, 0
    jz      short loc_1F2C2
    jmp     loc_1F7E6
loc_1F2C2:
    cmp     [bp+var_2E], 200h
    jl      short loc_1F2CC
    jmp     loc_1F7E6
loc_1F2CC:
    mov     word_4428E, 0B9h ; 'π'
    mov     byte_44D46, 1
    mov     word_44A8C, 75h ; 'u'
    jmp     loc_1F7E6
loc_1F2E0:
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_1E], ax
    mov     [bp+var_22], 4Fh ; 'O'
    mov     [bp+var_C], 32h ; '2'
    mov     [bp+var_E], 4Bh ; 'K'
    jmp     short loc_1F30F
    ; align 2
    db 144
loc_1F2FA:
    mov     ax, [bp+var_2C]
    mov     [bp+var_1E], ax
    mov     [bp+var_22], 69h ; 'i'
    mov     [bp+var_C], 0
    mov     [bp+var_E], 19h
loc_1F30F:
    mov     byte_44D46, 1
    cmp     [bp+var_28], 0
    jge     short loc_1F350
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 64h ; 'd'
    jge     short loc_1F350
    cmp     [bp+var_1E], 0
    jle     short loc_1F350
    cmp     [bp+var_1E], 278h
    jl      short loc_1F339
    jmp     loc_1F7E6
loc_1F339:
    cmp     [bp+var_1E], 188h
    jg      short loc_1F343
    jmp     loc_1F7E6
loc_1F343:
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     ax, [bp+var_22]
    jmp     loc_1EA9D
    ; align 2
    db 144
loc_1F350:
    cmp     [bp+var_28], 0
    jle     short loc_1F3A8
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2]
    sub     ax, word_449EC
    cmp     ax, 15Eh
    jle     short loc_1F3A8
    cmp     [bp+var_1E], 2B4h
    jge     short loc_1F3A8
    cmp     [bp+var_1E], 14Ch
    jle     short loc_1F3A8
    mov     word_44A8C, 2Ah ; '*'
    mov     word_44988, 0FFF4h
    cmp     [bp+var_1E], 200h
    jle     short loc_1F38C
    mov     ax, [bp+var_C]
    jmp     short loc_1F38F
    ; align 2
    db 144
loc_1F38C:
    mov     ax, [bp+var_E]
loc_1F38F:
    add     ax, 18h
    mov     word_4428E, ax
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     ax, [bp+var_22]
    add     ax, 19h
    mov     word_44DC6, ax
    jmp     loc_1EBE7
    ; align 2
    db 144
loc_1F3A8:
    push    [bp+var_28]
    push    [bp+var_1E]
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_A], ax
    cmp     ax, 14Ch
    jg      short loc_1F3C1
    jmp     loc_1F7E6
loc_1F3C1:
    cmp     ax, 2B4h
    jl      short loc_1F3C9
    jmp     loc_1F7E6
loc_1F3C9:
    push    [bp+var_28]
    push    [bp+var_1E]
    call    sub_2EA4E
    add     sp, 4
    sub     ax, 100h
    neg     ax
smart
    and     ah, 3
nosmart
    mov     cx, 18h
    imul    cx
    mov     si, ax
    mov     cl, 0Ah
    sar     si, cl
    mov     ax, [bp+var_22]
    add     ax, si
    inc     ax
    mov     word_44DC6, ax
    mov     al, [bp+var_12]
    mov     byte_44D47, al
    mov     byte_4392C, 0
    mov     word_44A8C, 2Ah ; '*'
    mov     word_44988, 0FFF4h
    mov     ax, [bp+var_A]
    sub     ax, 200h
    cmp     ax, 5Ah ; 'Z'
    jle     short loc_1F41E
    mov     ax, [bp+var_C]
loc_1F418:
    add     ax, si
    jmp     loc_1EA20
    ; align 2
    db 144
loc_1F41E:
    mov     ax, [bp+var_A]
    sub     ax, 200h
    cmp     ax, 0FFA6h
    jl      short loc_1F42C
    jmp     loc_1F7E6
loc_1F42C:
    mov     ax, [bp+var_E]
    jmp     short loc_1F418
    ; align 2
    db 144
loc_1F432:
    cmp     [bp+var_32], 78h ; 'x'
    jge     short loc_1F43E
    mov     al, [bp+var_12]
    mov     byte_44D47, al
loc_1F43E:
    cmp     [bp+var_2C], 17h
    jl      short loc_1F4A0
    cmp     [bp+var_2C], 61h ; 'a'
    jg      short loc_1F4A0
    cmp     [bp+var_28], 0FEF1h
    jle     short loc_1F4A0
    cmp     [bp+var_28], 0FF0Fh
    jge     short loc_1F4A0
    mov     word_44A8C, 2Ah ; '*'
    cmp     [bp+var_14], 0FEF1h
    jge     short loc_1F46E
    mov     word_4428E, 91h ; 'ë'
    jmp     loc_1F7E6
loc_1F46E:
    cmp     [bp+var_14], 0FF0Fh
    jle     short loc_1F47E
    mov     word_4428E, 92h ; 'í'
    jmp     loc_1F7E6
loc_1F47E:
    cmp     [bp+var_18], 17h
    jge     short loc_1F48E
    mov     word_4428E, 94h ; 'î'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F48E:
    cmp     [bp+var_18], 61h ; 'a'
    jg      short loc_1F497
    jmp     loc_1F7E6
loc_1F497:
    mov     word_4428E, 93h ; 'ì'
    jmp     loc_1F7E6
loc_1F4A0:
    cmp     [bp+var_2C], 0FFE9h
    jle     short loc_1F4A9
    jmp     loc_1F7E6
loc_1F4A9:
    cmp     [bp+var_2C], 0FF9Fh
    jge     short loc_1F4B2
    jmp     loc_1F7E6
loc_1F4B2:
    cmp     [bp+var_28], 10Fh
    jl      short loc_1F4BC
    jmp     loc_1F7E6
loc_1F4BC:
    cmp     [bp+var_28], 0F1h ; 'Ò'
    jg      short loc_1F4C6
    jmp     loc_1F7E6
loc_1F4C6:
    mov     word_44A8C, 2Ah ; '*'
    cmp     [bp+var_14], 10Fh
    jle     short loc_1F4DC
    mov     word_4428E, 8Dh ; 'ç'
    jmp     loc_1F7E6
loc_1F4DC:
    cmp     [bp+var_14], 0F1h ; 'Ò'
    jge     short loc_1F4EC
    mov     word_4428E, 8Eh ; 'é'
    jmp     loc_1F7E6
loc_1F4EC:
    cmp     [bp+var_18], 0FFE9h
    jle     short loc_1F4FC
    mov     word_4428E, 8Fh ; 'è'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F4FC:
    cmp     [bp+var_18], 0FF9Fh
    jl      short loc_1F505
    jmp     loc_1F7E6
loc_1F505:
    mov     word_4428E, 90h ; 'ê'
    jmp     loc_1F7E6
loc_1F50E:
    cmp     [bp+var_32], 96h ; 'ñ'
    jle     short loc_1F518
    jmp     loc_1F7E6
loc_1F518:
    cmp     [bp+var_2E], 96h ; 'ñ'
    jle     short loc_1F522
    jmp     loc_1F7E6
loc_1F522:
    mov     word_44A8C, 1A9h
    cmp     [bp+var_14], 0FF6Ah
    jg      short loc_1F538
    mov     word_4428E, 0A1h ; '°'
    jmp     loc_1F7E6
loc_1F538:
    cmp     [bp+var_14], 96h ; 'ñ'
    jl      short loc_1F548
    mov     word_4428E, 0A2h ; '¢'
    jmp     loc_1F7E6
loc_1F548:
    cmp     [bp+var_18], 96h ; 'ñ'
    jl      short loc_1F558
    mov     word_4428E, 0A3h ; '£'
    jmp     loc_1F7E6
loc_1F558:
    cmp     [bp+var_18], 0FF6Ah
    jle     short loc_1F562
    jmp     loc_1F7E6
loc_1F562:
    mov     word_4428E, 0A4h ; '§'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F56C:
    cmp     [bp+var_2C], 0FF38h
    jge     short loc_1F576
    jmp     loc_1F7E6
loc_1F576:
    cmp     [bp+var_2C], 104h
    jle     short loc_1F580
    jmp     loc_1F7E6
loc_1F580:
    cmp     [bp+var_2E], 50h ; 'P'
    jle     short loc_1F589
    jmp     loc_1F7E6
loc_1F589:
    mov     word_44A8C, 0E6h ; 'Ê'
    cmp     [bp+var_14], 0FFB0h
    jg      short loc_1F59E
    mov     word_4428E, 0A5h ; '•'
    jmp     loc_1F7E6
loc_1F59E:
    cmp     [bp+var_14], 50h ; 'P'
    jl      short loc_1F5AE
    mov     word_4428E, 0A8h ; '®'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F5AE:
    cmp     [bp+var_18], 0FF38h
    jg      short loc_1F5BE
    mov     word_4428E, 0A6h ; '¶'
    jmp     loc_1F7E6
loc_1F5BE:
    cmp     [bp+var_18], 104h
    jge     short loc_1F5C8
    jmp     loc_1F7E6
loc_1F5C8:
    mov     word_4428E, 0A7h ; 'ß'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F5D2:
    cmp     [bp+var_32], 0B4h ; '¥'
    jle     short loc_1F5DC
    jmp     loc_1F7E6
loc_1F5DC:
    cmp     [bp+var_2E], 64h ; 'd'
    jle     short loc_1F5E5
    jmp     loc_1F7E6
loc_1F5E5:
    mov     word_44A8C, 0F8h ; '¯'
    cmp     [bp+var_14], 0FF9Ch
    jg      short loc_1F5FA
    mov     word_4428E, 0A9h ; '©'
    jmp     loc_1F7E6
loc_1F5FA:
    cmp     [bp+var_14], 64h ; 'd'
    jl      short loc_1F60A
    mov     word_4428E, 0ACh ; '¨'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F60A:
    cmp     [bp+var_18], 0FF4Ch
    jg      short loc_1F61A
    mov     word_4428E, 0ABh ; '´'
    jmp     loc_1F7E6
loc_1F61A:
    cmp     [bp+var_18], 0B4h ; '¥'
    jge     short loc_1F624
    jmp     loc_1F7E6
loc_1F624:
    mov     word_4428E, 0AAh ; '™'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F62E:
    cmp     [bp+var_32], 0C8h ; '»'
    jle     short loc_1F638
    jmp     loc_1F7E6
loc_1F638:
    cmp     [bp+var_2E], 0C8h ; '»'
    jle     short loc_1F642
    jmp     loc_1F7E6
loc_1F642:
    mov     word_44A8C, 226h
    cmp     [bp+var_14], 0FF38h
    jg      short loc_1F658
    mov     word_4428E, 0ADh ; '≠'
    jmp     loc_1F7E6
loc_1F658:
    cmp     [bp+var_14], 0C8h ; '»'
    jl      short loc_1F668
    mov     word_4428E, 0AEh ; 'Æ'
    jmp     loc_1F7E6
loc_1F668:
    cmp     [bp+var_18], 0FF38h
    jg      short loc_1F678
    mov     word_4428E, 0AFh ; 'Ø'
    jmp     loc_1F7E6
loc_1F678:
    cmp     [bp+var_18], 0C8h ; '»'
    jge     short loc_1F682
    jmp     loc_1F7E6
loc_1F682:
    mov     word_4428E, 0B0h ; '∞'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F68C:
    cmp     [bp+var_32], 72h ; 'r'
    jle     short loc_1F695
    jmp     loc_1F7E6
loc_1F695:
    cmp     [bp+var_2E], 72h ; 'r'
    jle     short loc_1F69E
    jmp     loc_1F7E6
loc_1F69E:
    mov     word_44A8C, 1EFh
    cmp     [bp+var_14], 0FF8Eh
    jg      short loc_1F6B4
    mov     word_4428E, 0B4h ; '¥'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F6B4:
    cmp     [bp+var_14], 72h ; 'r'
    jl      short loc_1F6C4
    mov     word_4428E, 0B2h ; '≤'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F6C4:
    cmp     [bp+var_18], 0FF8Eh
    jg      short loc_1F6D4
    mov     word_4428E, 0B1h ; '±'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F6D4:
    cmp     [bp+var_18], 72h ; 'r'
    jge     short loc_1F6DD
    jmp     loc_1F7E6
loc_1F6DD:
    mov     word_4428E, 0B3h ; '≥'
    jmp     loc_1F7E6
loc_1F6E6:
    cmp     [bp+var_2C], 0FF56h
    jge     short loc_1F6F0
    jmp     loc_1F7E6
loc_1F6F0:
    cmp     [bp+var_2C], 104h
    jle     short loc_1F6FA
    jmp     loc_1F7E6
loc_1F6FA:
    cmp     [bp+var_2E], 6Eh ; 'n'
    jle     short loc_1F703
    jmp     loc_1F7E6
loc_1F703:
    mov     word_44A8C, 0E6h ; 'Ê'
    cmp     [bp+var_14], 0FF92h
    jg      short loc_1F71A
    mov     word_4428E, 0B5h ; 'µ'
    jmp     loc_1F7E6
    db 144
    db 144
loc_1F71A:
    cmp     [bp+var_14], 6Eh ; 'n'
    jl      short loc_1F72A
    mov     word_4428E, 0B8h ; '∏'
    jmp     loc_1F7E6
    ; align 2
    db 144
loc_1F72A:
    cmp     [bp+var_18], 0FF56h
    jg      short loc_1F73C
    mov     word_4428E, 0B7h ; '∑'
    jmp     loc_1F7E6
    ; align 4
    db 144
    db 144
loc_1F73C:
    cmp     [bp+var_18], 104h
    jge     short loc_1F746
    jmp     loc_1F7E6
loc_1F746:
    mov     word_4428E, 0B6h ; '∂'
    jmp     loc_1F7E6
    ; align 2
    db 144
off_1F750     dw offset loc_1E5B8
    dw offset loc_1E5E1
    dw offset loc_1E650
    dw offset loc_1E624
    dw offset loc_1E608
    dw offset loc_1E600
    dw offset loc_1E64A
    dw offset loc_1E678
    dw offset loc_1E690
    dw offset loc_1E6A4
    dw offset loc_1E6C6
    dw offset loc_1E7DA
    dw offset loc_1E5F4
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1E802
    dw offset loc_1E81E
    dw offset loc_1E8E4
    dw offset loc_1E8E4
    dw offset loc_1E8CA
    dw offset loc_1E978
    dw offset loc_1E8B6
    dw offset loc_1EA36
    dw offset loc_1EA26
    dw offset loc_1EB46
    dw offset loc_1EB6C
    dw offset loc_1EBF0
    dw offset loc_1EE3E
    dw offset loc_1EF24
    dw offset loc_1F028
    dw offset loc_1F020
    dw offset loc_1F2E0
    dw offset loc_1F2FA
    dw offset loc_1F432
    dw offset loc_1F1B0
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F50E
    dw offset loc_1F56C
    dw offset loc_1F5D2
    dw offset loc_1F62E
    dw offset loc_1F68C
    dw offset loc_1F6E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
    dw offset loc_1F7E6
loc_1F7E6:
    cmp     [bp+var_34], 7
    jnb     short loc_1F7EF
    jmp     loc_1F8CD
loc_1F7EF:
    mov     al, [bp+var_1A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     di, [bp+arg_0]
    mov     ax, [di]
    sub     ax, [bx-599Eh]
    mov     [bp+var_2C], ax
    mov     al, [bp+var_20]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [di+4]
    sub     ax, [bx+750Eh]
    mov     [bp+var_28], ax
    mov     al, [bp+var_34]
    sub     ah, ah
    sub     ax, 7
    cmp     ax, 0Bh
    ja      short loc_1F896
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_1F87E[bx]
loc_1F82A:
    mov     [bp+var_10], 0
    jmp     short loc_1F896
    ; align 2
    db 144
loc_1F832:
    mov     [bp+var_10], 300h
    mov     ax, [bp+var_2C]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_28]
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_1E]
    neg     ax
loc_1F848:
    mov     [bp+var_28], ax
    jmp     short loc_1F896
    ; align 2
    db 144
loc_1F84E:
    mov     [bp+var_10], 200h
    mov     ax, [bp+var_28]
    neg     ax
    mov     [bp+var_28], ax
    mov     ax, [bp+var_2C]
    neg     ax
    mov     [bp+var_2C], ax
    jmp     short loc_1F896
    ; align 2
    db 144
loc_1F866:
    mov     [bp+var_10], 100h
    mov     ax, [bp+var_2C]
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_28]
    neg     ax
    mov     [bp+var_2C], ax
    mov     ax, [bp+var_1E]
    jmp     short loc_1F848
off_1F87E     dw offset loc_1F82A
    dw offset loc_1F832
    dw offset loc_1F84E
    dw offset loc_1F866
    dw offset loc_1F82A
    dw offset loc_1F832
    dw offset loc_1F84E
    dw offset loc_1F866
    dw offset loc_1F82A
    dw offset loc_1F832
    dw offset loc_1F84E
    dw offset loc_1F866
loc_1F896:
    mov     al, [bp+var_34]
    sub     ah, ah
    cmp     ax, 7
    jb      short loc_1F8CD
    cmp     ax, 0Ah
    jbe     short loc_1F8C0
    cmp     ax, 0Bh
    jb      short loc_1F8CD
    cmp     ax, 0Eh
    jbe     short loc_1F8FC
    cmp     ax, 0Fh
    jb      short loc_1F8CD
    cmp     ax, 12h
    ja      short loc_1F8BC
    jmp     loc_1F940
loc_1F8BC:
    jmp     short loc_1F8CD
    ; align 4
    db 144
    db 144
loc_1F8C0:
    cmp     word_44DC6, 0
    jnz     short loc_1F8CD
    mov     word_44DC6, 3
loc_1F8CD:
    cmp     word_44DC6, 0
    jg      short loc_1F8D7
    jmp     loc_1F992
loc_1F8D7:
    mov     cl, 2
    shl     word_44DC6, cl
    mov     ax, [bp+var_10]
    cmp     ax, 100h
    jnz     short loc_1F8E8
    jmp     loc_1F9CC
loc_1F8E8:
    cmp     ax, 200h
    jnz     short loc_1F8F0
    jmp     loc_1F9C4
loc_1F8F0:
    cmp     ax, 300h
    jnz     short loc_1F8F8
    jmp     loc_1F98E
loc_1F8F8:
    jmp     loc_1F992
    ; align 2
    db 144
loc_1F8FC:
    push    [bp+var_28]
    mov     ax, 0FF80h
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    push    [bp+var_2C]
    mov     cx, 0FF80h
    push    cx
    mov     di, ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, di
    mov     [bp+var_22], ax
    or      ax, ax
    jge     short loc_1F8CD
    mov     word_44DC6, 4
    jmp     short loc_1F8CD
    ; align 2
    db 144
loc_1F940:
    push    [bp+var_28]
    mov     ax, 0FF80h
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    push    [bp+var_2C]
    mov     cx, 0FF80h
    push    cx
    mov     di, ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    scale_value
    add     sp, 4
    add     ax, di
    mov     [bp+var_22], ax
    or      ax, ax
    jle     short loc_1F984
    mov     word_44DC6, 5
    jmp     loc_1F8CD
loc_1F984:
    mov     word_449EC, 1C2h
    jmp     loc_1F8CD
    ; align 2
    db 144
loc_1F98E:
    inc     word_44DC6
loc_1F992:
    mov     ax, 22h ; '"'
    imul    word_44DC6
    add     ax, word_454C2
    mov     dx, word_454C4
    mov     word_44D64, ax
    mov     word_44D66, dx
    cmp     byte_44D47, 4
    jnz     short loc_1F9D4
    mov     bx, [bp+arg_0]
    mov     ax, [bx+4]
    xor     ax, [bx]
    mov     cl, 8
    sar     ax, cl
smart
    and     ax, 1
nosmart
    add     word_449EC, ax
    jmp     short loc_1F9D9
loc_1F9C4:
    add     word_44DC6, 2
    jmp     short loc_1F992
    ; align 2
    db 144
loc_1F9CC:
    add     word_44DC6, 3
    jmp     short loc_1F992
    ; align 2
    db 144
loc_1F9D4:
    add     word_449EC, 2
loc_1F9D9:
    cmp     word_4428E, 0
    jge     short loc_1F9E3
    jmp     loc_1FADE
loc_1F9E3:
    mov     bx, word_4428E
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    les     di, dword_46460
    mov     ax, es:[bx+di]
    neg     ax
    add     ax, [bp+var_10]
    add     ax, [bp+var_38]
smart
    and     ah, 3
nosmart
    mov     word_45D96, ax
    mov     ax, [bp+var_10]
    or      ax, ax
    jz      short loc_1FA20
    cmp     ax, 100h
    jnz     short loc_1FA13
    jmp     loc_1FAA6
loc_1FA13:
    cmp     ax, 200h
    jz      short loc_1FA78
    cmp     ax, 300h
    jz      short loc_1FA4C
    jmp     loc_1FAD0
loc_1FA20:
    mov     ax, word_4428E
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_46460
    mov     dx, es
    mov     word ptr [bp+var_40], ax
    mov     word ptr [bp+var_40+2], dx
    les     bx, [bp+var_40]
    mov     ax, es:[bx+2]
    mov     word_42D20, ax
    mov     ax, es:[bx+4]
loc_1FA45:
    mov     word_42D2C, ax
    jmp     loc_1FAD0
    ; align 2
    db 144
loc_1FA4C:
    mov     ax, word_4428E
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_46460
    mov     dx, word ptr dword_46460+2
    mov     word ptr [bp+var_40], ax
    mov     word ptr [bp+var_40+2], dx
    les     bx, [bp+var_40]
    mov     ax, es:[bx+4]
    neg     ax
    mov     word_42D20, ax
    mov     ax, es:[bx+2]
    jmp     short loc_1FA45
    ; align 2
    db 144
loc_1FA78:
    mov     ax, word_4428E
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_46460
    mov     dx, word ptr dword_46460+2
    mov     word ptr [bp+var_40], ax
    mov     word ptr [bp+var_40+2], dx
    les     bx, [bp+var_40]
    mov     ax, es:[bx+2]
    neg     ax
    mov     word_42D20, ax
    mov     ax, es:[bx+4]
loc_1FAA1:
    neg     ax
    jmp     short loc_1FA45
    ; align 2
    db 144
loc_1FAA6:
    mov     ax, word_4428E
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word ptr dword_46460
    mov     dx, word ptr dword_46460+2
    mov     word ptr [bp+var_40], ax
    mov     word ptr [bp+var_40+2], dx
    les     bx, [bp+var_40]
    mov     ax, es:[bx+4]
    mov     word_42D20, ax
    mov     ax, es:[bx+2]
    jmp     short loc_1FAA1
    ; align 2
    db 144
loc_1FAD0:
    mov     ax, word_449E0
    add     word_42D20, ax
    mov     ax, word_449F6
    add     word_42D2C, ax
loc_1FADE:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1E1A0 endp
sub_1FAE4 proc far
    var_14 = word ptr -20
    var_10 = byte ptr -16
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 14h
    push    di
    push    si
    mov     bx, [bp+arg_2]
    shl     bx, 1
    mov     bx, trackrows[bx]
    add     bx, [bp+arg_0]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_2], al
    or      al, al
    jnz     short loc_1FB12
loc_1FB0A:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_1FB12:
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
    mov     [bp+var_6], ax
    mov     bx, [bp+arg_2]
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    mov     [bp+var_C], ax
    cmp     [bp+var_2], 0FDh ; '˝'
    jnb     short loc_1FB33
    jmp     loc_1FC34
loc_1FB33:
    mov     al, [bp+var_2]
    sub     ah, ah
    cmp     ax, 0FDh ; '˝'
    jz      short loc_1FB4E
    cmp     ax, 0FEh ; '˛'
    jz      short loc_1FBB4
    cmp     ax, 0FFh
    jnz     short loc_1FB4A
    jmp     loc_1FBF2
loc_1FB4A:
    jmp     loc_1FC86
    ; align 2
    db 144
loc_1FB4E:
    mov     ax, [bp+arg_2]
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, ax
    mov     bx, word_45D3E[bx]
    add     bx, [bp+arg_0]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx-1]
    mov     [bp+var_2], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 1
    jz      short loc_1FB8D
    mov     bx, [bp+var_14]
    mov     ax, (trackpos+2)[bx]
loc_1FB8A:
    mov     [bp+var_C], ax
loc_1FB8D:
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 2
    jnz     short loc_1FBA8
    jmp     loc_1FC86
loc_1FBA8:
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, trackpos2[bx]
    jmp     loc_1FC83
loc_1FBB4:
    mov     ax, [bp+arg_2]
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, ax
    mov     bx, word_45D3E[bx]
    add     bx, [bp+arg_0]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_2], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 1
    jz      short loc_1FC62
    mov     bx, [bp+var_14]
    mov     ax, (trackpos+2)[bx]
    jmp     short loc_1FC5F
    ; align 2
    db 144
loc_1FBF2:
    mov     ax, [bp+arg_2]
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, [bp+arg_0]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx-1]
    mov     [bp+var_2], al
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 1
    jnz     short loc_1FC2A
    jmp     loc_1FB8D
loc_1FC2A:
    mov     bx, [bp+var_14]
    mov     ax, trackpos[bx]
    jmp     loc_1FB8A
loc_1FC34:
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, sceneshapes.scene_translateflag[bx]
    mov     byte ptr [bp+var_14], al
    cmp     al, ah
    jz      short loc_1FC86
    test    byte ptr [bp+var_14], 1
    jz      short loc_1FC62
    mov     bx, [bp+arg_2]
    shl     bx, 1
    mov     ax, trackpos[bx]
loc_1FC5F:
    mov     [bp+var_C], ax
loc_1FC62:
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 2
    jz      short loc_1FC86
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, (trackpos2+2)[bx]
loc_1FC83:
    mov     [bp+var_6], ax
loc_1FC86:
    sub     di, di
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, sceneshapes.scene_unk4[bx]
    cbw
    cmp     ax, 20h ; ' '
    jz      short loc_1FCC2
    jg      short loc_1FCF4
    cmp     ax, 0Bh
    jz      short loc_1FCE0
    cmp     ax, 12h
    jz      short loc_1FCEA
    jmp     short loc_1FCBA
    ; align 2
    db 144
loc_1FCB2:
    mov     di, 2
    mov     [bp+var_4], offset unk_3E676
loc_1FCBA:
    or      di, di
    jnz     short loc_1FD14
    jmp     loc_1FB0A
    ; align 2
    db 144
loc_1FCC2:
    mov     di, 2
    mov     [bp+var_4], offset unk_3E682
    jmp     short loc_1FCBA
loc_1FCCC:
    mov     di, 2
    mov     [bp+var_4], offset unk_3E68E
    jmp     short loc_1FCBA
loc_1FCD6:
    mov     di, 4
    mov     [bp+var_4], offset unk_3E69A
    jmp     short loc_1FCBA
loc_1FCE0:
    mov     di, 1
    mov     [bp+var_4], offset unk_3E640
    jmp     short loc_1FCBA
loc_1FCEA:
    mov     di, 8
    mov     [bp+var_4], offset unk_3E646
    jmp     short loc_1FCBA
loc_1FCF4:
    cmp     ax, 22h ; '"'
    jz      short loc_1FCD6
    jg      short loc_1FD02
    cmp     ax, 21h ; '!'
    jz      short loc_1FCCC
    jmp     short loc_1FCBA
loc_1FD02:
    cmp     ax, 23h ; '#'
    jz      short loc_1FCB2
    cmp     ax, 47h ; 'G'
    jl      short loc_1FCBA
    cmp     ax, 4Ah ; 'J'
    jle     short loc_1FCE0
    jmp     short loc_1FCBA
    ; align 2
    db 144
loc_1FD14:
    mov     bx, [bp+arg_2]
    shl     bx, 1
    mov     bx, terrainrows[bx]
    add     bx, [bp+arg_0]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_10], al
    cmp     al, 6
    jnz     short loc_1FD3A
    mov     ax, word_3B8EA
    mov     [bp+var_A], ax
    jmp     short loc_1FD3F
loc_1FD3A:
    mov     [bp+var_A], 0
loc_1FD3F:
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, sceneshapes.scene_rot_y[bx]
    mov     [bp+var_8], ax
    sub     si, si
    jmp     short loc_1FDA6
    ; align 2
    db 144
loc_1FD5C:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+var_4]
    add     bx, ax
    mov     ax, [bx]
    add     ax, [bp+var_6]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+2]
    add     ax, [bp+var_A]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx+2], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+4]
loc_1FD99:
    add     ax, [bp+var_C]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx+4], ax
loc_1FDA5:
    inc     si
loc_1FDA6:
    cmp     si, di
    jl      short loc_1FDAD
    jmp     loc_1FE8C
loc_1FDAD:
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_1FD5C
    cmp     ax, 100h
    jnz     short loc_1FDBC
    jmp     loc_1FE4A
loc_1FDBC:
    cmp     ax, 200h
    jz      short loc_1FE08
    cmp     ax, 300h
    jnz     short loc_1FDA5
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+var_4]
    add     bx, ax
    mov     ax, [bx+4]
    neg     ax
    add     ax, [bp+var_6]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+2]
    add     ax, [bp+var_A]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx+2], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx]
    jmp     short loc_1FD99
    ; align 2
    db 144
loc_1FE08:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+var_4]
    add     bx, ax
    mov     ax, [bx]
    neg     ax
    add     ax, [bp+var_6]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+2]
    add     ax, [bp+var_A]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx+2], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+4]
    jmp     short loc_1FE87
    ; align 2
    db 144
loc_1FE4A:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+var_4]
    add     bx, ax
    mov     ax, [bx+4]
    add     ax, [bp+var_6]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx+2]
    add     ax, [bp+var_A]
    mov     bx, [bp+arg_4]
    add     bx, [bp+var_14]
    mov     [bx+2], ax
    mov     bx, [bp+var_4]
    add     bx, [bp+var_14]
    mov     ax, [bx]
loc_1FE87:
    neg     ax
    jmp     loc_1FD99
loc_1FE8C:
    mov     ax, di
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1FAE4 endp
load_game_3dshapes proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    sub     ax, ax
    mov     word_461C6, ax
    mov     word_461C4, ax
    mov     word_463D4, ax
    mov     word_463D2, ax
    call    get_res_ofs_diff_scaled
    or      dx, dx
    jg      short loc_1FEC2
    jl      short loc_1FEB9
    cmp     ax, 0FDE8h
    jnb     short loc_1FEC2
loc_1FEB9:
    mov     ax, 1
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_1FEC2:
    mov     ax, offset aGame1; "game1"
    push    ax
    call    load_3dshape
    add     sp, 2
    mov     word_461C4, ax
    mov     word_461C6, dx
    mov     ax, offset aGame2; "game2"
    push    ax
    call    load_3dshape
    add     sp, 2
    mov     word_463D2, ax
    mov     word_463D4, dx
    sub     si, si
loc_1FEEA:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    add     ax, offset aBarn; "barn"
    push    ax
    push    word_461C6
    push    word_461C4
    call    locate_shape_nofatal
    add     sp, 6
    mov     word_449F8, ax
    mov     word_449FA, dx
    or      ax, dx
    jnz     short loc_1FF38
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    add     ax, offset aBarn; "barn"
    push    ax
    push    word_463D4
    push    word_463D2
    call    locate_shape_fatal
    add     sp, 6
    mov     word_449F8, ax
    mov     word_449FA, dx
loc_1FF38:
    mov     ax, 16h
    imul    si
    add     ax, offset game3dshapes
    push    ax
    push    word_449FA
    push    word_449F8
    call    setup_3d_res
    add     sp, 6
    inc     si
    cmp     si, 74h ; 't'
    jl      short loc_1FEEA
    sub     ax, ax
    pop     si
    mov     sp, bp
    pop     bp
    retf
load_game_3dshapes endp
unload_game_resources proc far

    mov     ax, word_461C4
    or      ax, word_461C6
    jz      short loc_1FF77
    push    word_461C6
    push    word_461C4
    call    mmgr_free
    add     sp, 4
loc_1FF77:
    mov     ax, word_463D2
    or      ax, word_463D4
    jz      short locret_1FF90
    push    word_463D4
    push    word_463D2
    call    mmgr_free
    add     sp, 4
locret_1FF90:
    retf
    ; align 2
    db 144
unload_game_resources endp
sub_1FF92 proc far
    var_10 = byte ptr -16
    var_E = dword ptr -14
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 10h
    push    di
    push    si
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    mov     byte ptr aStxxx+2, al
    mov     al, [bx+1]
    mov     byte ptr aStxxx+3, al
    mov     al, [bx+2]
    mov     byte ptr aStxxx+4, al
    mov     al, [bx+3]
    mov     byte ptr aStxxx+5, al
    mov     ax, offset aStxxx; "stxxx"
    push    ax
    call    load_3dshape
    add     sp, 2
    mov     word ptr dword_454AA, ax
    mov     word ptr dword_454AA+2, dx
    mov     ax, (offset game3dshapes.shape3d_numverts+0AA8h)
    push    ax
    mov     ax, offset aCar0; "car0"
    push    ax
    push    dx
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0AD4h)
    push    ax
    mov     ax, offset aCar1; "car1"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, word ptr game3dshapes.shape3d_verts+0AD4h
    mov     dx, word ptr game3dshapes.shape3d_verts+0AD6h
    add     ax, 30h ; '0'
    mov     word ptr [bp+var_E], ax
    mov     word ptr [bp+var_E+2], dx
    les     bx, [bp+var_E]
    mov     ax, es:[bx+4]
    mov     word_42CBE, ax
    mov     ax, es:[bx+12h]
    add     ax, es:[bx]
    sar     ax, 1
    mov     word_42CBA, ax
    mov     ax, es:[bx+28h]
    mov     word_42CC4, ax
    mov     ax, es:[bx+24h]
    add     ax, es:[bx+36h]
    sar     ax, 1
    mov     word_42CC0, ax
    sub     si, si
loc_20044:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    mov     ax, word_42CBA
    les     bx, [bp+var_E]
    sub     ax, es:[bx+di]
    mov     word_443FA[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42CBE
    sub     ax, es:[bx+di+4]
    mov     word_443FE[di], ax
    les     bx, [bp+var_E]
    mov     ax, es:[bx+di+2]
    mov     word_443FC[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42CC0
    sub     ax, es:[bx+di+24h]
    mov     word_4441E[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42CC4
    sub     ax, es:[bx+di+28h]
    mov     word_44422[di], ax
    les     bx, [bp+var_E]
    mov     ax, es:[bx+di+26h]
    mov     word_44420[di], ax
    les     bx, [bp+var_E]
    push    si
    push    di
    lea     si, [bx+di+48h]
    lea     di, word_44442[di]
    push    ds
    push    ds
    push    es
    pop     ds
    pop     es
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    les     bx, [bp+var_E]
    push    si
    push    di
    lea     si, [bx+di+6Ch]
    lea     di, word_44466[di]
    push    ds
    push    ds
    push    es
    pop     ds
    pop     es
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    inc     si
    cmp     si, 6
    jge     short loc_200D4
    jmp     loc_20044
loc_200D4:
    sub     si, si
loc_200D6:
    mov     bx, si
    shl     bx, 1
    mov     word_443E8[bx], 0
    inc     si
    cmp     si, 5
    jl      short loc_200D6
    mov     ax, 814Ch
    push    ax
    mov     ax, offset aCar2; "car2"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, 8044h
    push    ax
    mov     ax, offset aExp0_0; "exp0"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, 805Ah
    push    ax
    mov     ax, offset aExp1_0; "exp1"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A24h)
    push    ax
    mov     ax, offset aExp2_0; "exp2"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A3Ah)
    push    ax
    mov     ax, offset aExp3_0; "exp3"
    push    ax
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     bx, [bp+arg_2]
    mov     al, [bx]
    mov     [bp+var_10], al
    cmp     al, 0FFh
    jnz     short loc_2019F
    jmp     loc_2042A
loc_2019F:
    mov     bx, [bp+arg_0]
    cmp     [bx], al
    jz      short loc_201A9
    jmp     loc_2022A
loc_201A9:
    mov     di, [bp+arg_2]
    mov     al, [di+1]
    cmp     [bx+1], al
    jnz     short loc_2022A
    mov     al, [di+2]
    cmp     [bx+2], al
    jnz     short loc_2022A
    mov     al, [di+3]
    cmp     [bx+3], al
    jnz     short loc_2022A
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    get_res_size_scaled
    add     sp, 4
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
    push    dx
    push    ax
    mov     ax, offset aCar2_0; "car2"
    push    ax
    call    mmgr_alloc_resbytes
    add     sp, 6
    mov     word ptr dword_454B0, ax
    mov     word ptr dword_454B0+2, dx
    sub     ax, ax
    mov     [bp+var_8], ax
    mov     [bp+var_A], ax
    jmp     short loc_20204
    ; align 4
    db 144
    db 144
    db 144
loc_201FC:
    add     [bp+var_A], 1
    adc     [bp+var_8], 0
loc_20204:
    mov     ax, [bp+var_6]
    mov     dx, [bp+var_4]
    cmp     [bp+var_8], dx
    jg      short loc_20257
    jl      short loc_20216
    cmp     [bp+var_A], ax
    jnb     short loc_20257
loc_20216:
    mov     bx, [bp+var_A]
    les     di, dword_454AA
    mov     al, es:[bx+di]
    les     di, dword_454B0
    mov     es:[bx+di], al
    jmp     short loc_201FC
    ; align 2
    db 144
loc_2022A:
    mov     bx, [bp+arg_2]
    mov     al, [bx]
    mov     byte ptr aStxxx+2, al
    mov     al, [bx+1]
    mov     byte ptr aStxxx+3, al
    mov     al, [bx+2]
    mov     byte ptr aStxxx+4, al
    mov     al, [bx+3]
    mov     byte ptr aStxxx+5, al
    mov     ax, offset aStxxx; "stxxx"
    push    ax
    call    load_3dshape
    add     sp, 2
    mov     word ptr dword_454B0, ax
    mov     word ptr dword_454B0+2, dx
loc_20257:
    mov     ax, (offset game3dshapes.shape3d_numverts+0ABEh)
    push    ax
    mov     ax, offset aCar0_0; "car0"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0AEAh)
    push    ax
    mov     ax, offset aCar1_0; "car1"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, word ptr game3dshapes.shape3d_verts+0AEAh
    mov     dx, word ptr game3dshapes.shape3d_verts+0AECh
    add     ax, 30h ; '0'
    mov     word ptr [bp+var_E], ax
    mov     word ptr [bp+var_E+2], dx
    les     bx, [bp+var_E]
    mov     ax, es:[bx+4]
    mov     word_42D08, ax
    mov     ax, es:[bx+12h]
    add     ax, es:[bx]
    sar     ax, 1
    mov     word_42D04, ax
    mov     ax, es:[bx+28h]
    mov     word_42D0E, ax
    mov     ax, es:[bx+24h]
    add     ax, es:[bx+36h]
    sar     ax, 1
    mov     word_42D0A, ax
    sub     si, si
loc_202D7:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    mov     ax, word_42D04
    les     bx, [bp+var_E]
    sub     ax, es:[bx+di]
    mov     word_448F4[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42D08
    sub     ax, es:[bx+di+4]
    mov     word_448F8[di], ax
    les     bx, [bp+var_E]
    mov     ax, es:[bx+di+2]
    mov     word_448F6[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42D0A
    sub     ax, es:[bx+di+24h]
    mov     word_44918[di], ax
    les     bx, [bp+var_E]
    mov     ax, word_42D0E
    sub     ax, es:[bx+di+28h]
    mov     word_4491C[di], ax
    les     bx, [bp+var_E]
    mov     ax, es:[bx+di+26h]
    mov     word_4491A[di], ax
    les     bx, [bp+var_E]
    push    si
    push    di
    lea     si, [bx+di+48h]
    lea     di, word_4493C[di]
    push    ds
    push    ds
    push    es
    pop     ds
    pop     es
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    les     bx, [bp+var_E]
    push    si
    push    di
    lea     si, [bx+di+6Ch]
    lea     di, word_44960[di]
    push    ds
    push    ds
    push    es
    pop     ds
    pop     es
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    inc     si
    cmp     si, 6
    jge     short loc_20367
    jmp     loc_202D7
loc_20367:
    sub     si, si
loc_20369:
    mov     bx, si
    shl     bx, 1
    mov     word_4448A[bx], 0
    inc     si
    cmp     si, 5
    jl      short loc_20369
    mov     ax, 8162h
    push    ax
    mov     ax, offset aCar2_1; "car2"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A50h)
    push    ax
    mov     ax, offset aExp0_1; "exp0"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A66h)
    push    ax
    mov     ax, offset aExp1_1; "exp1"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A7Ch)
    push    ax
    mov     ax, offset aExp2_1; "exp2"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    mov     ax, (offset game3dshapes.shape3d_numverts+0A92h)
    push    ax
    mov     ax, offset aExp3_1; "exp3"
    push    ax
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    locate_shape_fatal
    add     sp, 6
    push    dx
    push    ax
    call    setup_3d_res
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_2042A:
    sub     ax, ax
    mov     word ptr dword_454B0+2, ax
    mov     word ptr dword_454B0, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_1FF92 endp
sub_20438 proc far

    mov     ax, word ptr dword_454B0
    or      ax, word ptr dword_454B0+2
    jz      short loc_20477
    mov     ax, 7594h
    push    ax
    mov     ax, 9184h
    push    ax
    mov     ax, 8D1Ah
    push    ax
    mov     ax, 2FA0h
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, word ptr game3dshapes.shape3d_verts+0AEAh
    mov     dx, word ptr game3dshapes.shape3d_verts+0AECh
    add     ax, 30h ; '0'
    push    dx
    push    ax
    push    cs
    call    near ptr sub_204AE
    add     sp, 0Eh
    push    word ptr dword_454B0+2
    push    word ptr dword_454B0
    call    mmgr_op_unk2
    add     sp, 4
loc_20477:
    mov     ax, 754Ah
    push    ax
    mov     ax, 8C8Ah
    push    ax
    mov     ax, 8C78h
    push    ax
    mov     ax, 2FA0h
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, word ptr game3dshapes.shape3d_verts+0AD4h
    mov     dx, word ptr game3dshapes.shape3d_verts+0AD6h
    add     ax, 30h ; '0'
    push    dx
    push    ax
    push    cs
    call    near ptr sub_204AE
    add     sp, 0Eh
    push    word ptr dword_454AA+2
    push    word ptr dword_454AA
    call    mmgr_free
    add     sp, 4
    retf
sub_20438 endp
sub_204AE proc far
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_8 = word ptr -8
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = word ptr 16
    arg_C = word ptr 18

    push    bp
    mov     bp, sp
    sub     sp, 14h
    push    di
    push    si
    mov     bx, [bp+arg_8]
    mov     ax, [bp+arg_4]
    cmp     [bx+8], ax
    jnz     short loc_204C4
    jmp     loc_2064C
loc_204C4:
    sar     ax, 1
    push    ax
    call    sin_fast
    add     sp, 2
    mov     [bp+var_C], ax
    mov     ax, [bp+arg_4]
    sar     ax, 1
    push    ax
    call    cos_fast
    add     sp, 2
    mov     [bp+var_2], ax
    sub     si, si
loc_204E5:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_10], ax
    push    [bp+var_2]
    mov     bx, [bp+arg_A]
    mov     ax, si
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     bx, ax
    push    word ptr [bx]
    call    scale_value
    add     sp, 4
    push    [bp+var_C]
    mov     bx, [bp+arg_A]
    add     bx, [bp+var_10]
    push    word ptr [bx+4]
    mov     [bp+var_14], ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_C]
    mov     cx, [bx]
    add     cx, ax
    add     cx, [bp+var_14]
    les     bx, [bp+arg_0]
    add     bx, [bp+var_10]
    mov     es:[bx], cx
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_12], ax
    push    [bp+var_2]
    mov     bx, [bp+arg_A]
    mov     ax, si
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     bx, ax
    push    word ptr [bx+4]
    call    scale_value
    add     sp, 4
    push    [bp+var_C]
    mov     bx, [bp+arg_A]
    add     bx, [bp+var_12]
    push    word ptr [bx]
    mov     [bp+var_14], ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_C]
    mov     cx, [bx+4]
    add     cx, ax
    add     cx, [bp+var_14]
    les     bx, [bp+arg_0]
    add     bx, [bp+var_12]
    mov     es:[bx+4], cx
    inc     si
    cmp     si, 6
    jge     short loc_20592
    jmp     loc_204E5
loc_20592:
    mov     si, 6
loc_20595:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    push    [bp+var_2]
    mov     bx, [bp+arg_A]
    mov     ax, si
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     bx, ax
    push    word ptr [bx]
    call    scale_value
    add     sp, 4
    push    [bp+var_C]
    mov     bx, [bp+arg_A]
    add     bx, [bp+var_14]
    push    word ptr [bx+4]
    mov     [bp+var_10], ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_C]
    mov     cx, [bx+6]
    add     cx, ax
    add     cx, [bp+var_10]
    les     bx, [bp+arg_0]
    add     bx, [bp+var_14]
    mov     es:[bx], cx
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_12], ax
    push    [bp+var_2]
    mov     bx, [bp+arg_A]
    mov     ax, si
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     bx, ax
    push    word ptr [bx+4]
    call    scale_value
    add     sp, 4
    push    [bp+var_C]
    mov     bx, [bp+arg_A]
    add     bx, [bp+var_12]
    push    word ptr [bx]
    mov     [bp+var_10], ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_C]
    mov     cx, [bx+0Ah]
    add     cx, ax
    add     cx, [bp+var_10]
    les     bx, [bp+arg_0]
    add     bx, [bp+var_12]
    mov     es:[bx+4], cx
    inc     si
    cmp     si, 0Ch
    jge     short loc_20643
    jmp     loc_20595
loc_20643:
    mov     bx, [bp+arg_8]
    mov     ax, [bp+arg_4]
    mov     [bx+8], ax
loc_2064C:
    sub     di, di
    jmp     short loc_20687
loc_20650:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+arg_A]
    add     bx, ax
    mov     ax, [bx+2]
    sub     ax, [bp+var_8]
    les     bx, [bp+arg_0]
    add     bx, [bp+var_14]
    mov     es:[bx+2], ax
    inc     si
loc_20673:
    cmp     [bp+var_4], si
    jg      short loc_20650
    mov     bx, [bp+arg_8]
    mov     ax, di
    shl     ax, 1
    add     bx, ax
    mov     ax, [bp+var_8]
    mov     [bx], ax
loc_20686:
    inc     di
loc_20687:
    cmp     di, 4
    jge     short loc_206CE
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, [bp+arg_6]
    add     bx, ax
    mov     ax, [bx]
    cwd
    xor     ax, dx
    sub     ax, dx
    mov     cx, 6
    sar     ax, cl
    xor     ax, dx
    sub     ax, dx
    mov     [bp+var_8], ax
    mov     bx, [bp+arg_8]
    add     bx, [bp+var_14]
    cmp     [bx], ax
    jz      short loc_20686
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_E], ax
    add     ax, 6
    mov     [bp+var_4], ax
    mov     si, [bp+var_E]
    jmp     short loc_20673
    ; align 2
    db 144
loc_206CE:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_204AE endp
setup_track proc far
    var_AF0 = dword ptr -2800
    var_AEC = word ptr -2796
    var_AEA = word ptr -2794
    var_AE8 = byte ptr -2792
    var_AE6 = byte ptr -2790
    var_AE4 = byte ptr -2788
    var_AE2 = word ptr -2786
    var_AE0 = word ptr -2784
    var_ADE = word ptr -2782
    var_ADC = word ptr -2780
    var_ADA = word ptr -2778
    var_AD8 = word ptr -2776
    var_AD6 = word ptr -2774
    var_AD4 = byte ptr -2772
    var_74E = byte ptr -1870
    var_74C = byte ptr -1868
    var_74A = word ptr -1866
    var_748 = word ptr -1864
    var_746 = byte ptr -1862
    var_744 = byte ptr -1860
    var_742 = byte ptr -1858
    var_740 = byte ptr -1856
    var_73E = byte ptr -1854
    var_73A = byte ptr -1850
    var_738 = byte ptr -1848
    var_3B0 = byte ptr -944
    var_3AE = word ptr -942
    var_3AC = word ptr -940
    var_3AA = byte ptr -938
    var_3A8 = byte ptr -936
    var_3A6 = byte ptr -934
    var_3A4 = byte ptr -932
    var_3A2 = word ptr -930
    var_3A0 = dword ptr -928
    var_39C = word ptr -924
    var_39A = byte ptr -922
    var_398 = byte ptr -920
    var_12 = byte ptr -18
    var_10 = byte ptr -16
    var_E = byte ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = byte ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_2092 = word ptr 8344

    push    bp
    mov     bp, sp
    sub     sp, 0AF0h
    push    di
    push    si
    mov     ax, 380h
    cwd
    push    dx
    push    ax
    mov     ax, offset aTcomp; "tcomp"
    push    ax
    call    mmgr_alloc_resbytes
    add     sp, 6
    mov     [bp+var_AE2], ax
    mov     [bp+var_AE0], dx
    or      ax, dx
    jnz     short loc_20704
    mov     ax, 2
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_20704:
    mov     ax, [bp+var_AE2]
    mov     dx, [bp+var_AE0]
    mov     [bp+var_74A], ax
    mov     [bp+var_748], dx
    mov     [bp+var_3A6], 0
    mov     [bp+var_4], 0
    mov     word_45DD0, 0
    sub     si, si
loc_20725:
    les     bx, trackdata19
    mov     byte ptr es:[bx+si], 0FFh
    inc     si
    cmp     si, 385h
    jl      short loc_20725
    mov     [bp+var_73A], 0
    jmp     short loc_207A4
    ; align 2
    db 144
loc_2073C:
    mov     bl, [bp+var_74E]
    sub     bh, bh
    mov     al, byte_3E73E[bx]
    mov     [bp+var_12], al
    inc     [bp+var_3A4]
loc_2074D:
    cmp     [bp+var_3A4], 1Eh
    jge     short loc_207A0
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, terrainrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_74E], al
    mov     bl, al
    sub     bh, bh
    mov     al, [bp+var_12]
    cmp     byte_3E72A[bx], al
    jz      short loc_2073C
    cmp     al, 63h ; 'c'
    jz      short loc_2073C
loc_20788:
    mov     [bp+var_3B0], 0Bh
loc_2078D:
    cmp     [bp+var_3A4], 0FFh
    jz      short loc_20797
    jmp     loc_2177A
loc_20797:
    mov     [bp+var_3A4], 0
    jmp     loc_21786
    ; align 2
    db 144
loc_207A0:
    inc     [bp+var_73A]
loc_207A4:
    cmp     [bp+var_73A], 1Eh
    jge     short loc_207B6
    mov     [bp+var_12], 63h ; 'c'
    mov     [bp+var_3A4], 0
    jmp     short loc_2074D
loc_207B6:
    mov     [bp+var_3A4], 0
    jmp     short loc_20812
    ; align 2
    db 144
loc_207BE:
    mov     bl, [bp+var_74E]
    sub     bh, bh
    mov     al, byte_3E766[bx]
    mov     [bp+var_12], al
    inc     [bp+var_73A]
loc_207CF:
    cmp     [bp+var_73A], 1Eh
    jge     short loc_2080E
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, terrainrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_74E], al
    mov     bl, al
    sub     bh, bh
    mov     al, [bp+var_12]
    cmp     byte_3E752[bx], al
    jz      short loc_207BE
    cmp     al, 63h ; 'c'
    jz      short loc_207BE
    jmp     loc_20788
    ; align 2
    db 144
loc_2080E:
    inc     [bp+var_3A4]
loc_20812:
    cmp     [bp+var_3A4], 1Eh
    jge     short loc_20824
    mov     [bp+var_12], 63h ; 'c'
    mov     [bp+var_73A], 0
    jmp     short loc_207CF
loc_20824:
    mov     [bp+var_73A], 0
    jmp     loc_20956
loc_2082C:
    mov     track_angle, 0
loc_20832:
    cmp     [bp+var_3A6], 0
    jz      short loc_2087A
    mov     [bp+var_3B0], 3
    jmp     loc_2078D
    ; align 2
    db 144
loc_20842:
    mov     track_angle, 200h
    jmp     short loc_20832
loc_2084A:
    mov     track_angle, 100h
    jmp     short loc_20832
loc_20852:
    mov     track_angle, 300h
    jmp     short loc_20832
loc_2085A:
    cmp     ax, 94h ; 'î'
    jz      short loc_20842
    cmp     ax, 95h ; 'ï'
    jz      short loc_2084A
    cmp     ax, 96h ; 'ñ'
    jz      short loc_20852
    cmp     ax, 0B3h ; '≥'
    jz      short loc_20842
    cmp     ax, 0B4h ; '¥'
    jz      short loc_2084A
    cmp     ax, 0B5h ; 'µ'
loc_20876:
    jz      short loc_20852
    jmp     short loc_208BB
loc_2087A:
    mov     al, [bp+var_3A4]
    mov     startcol2, al
    mov     al, [bp+var_73A]
    mov     startrow2, al
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, terrainrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_74E], al
    cmp     al, 6
    jnz     short loc_208B2
    mov     byte_4499B, 1
    jmp     short loc_208B7
loc_208B2:
    mov     byte_4499B, 0
loc_208B7:
    inc     [bp+var_3A6]
loc_208BB:
    inc     [bp+var_3A4]
loc_208BF:
    cmp     [bp+var_3A4], 1Eh
    jl      short loc_208C9
    jmp     loc_20952
loc_208C9:
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_E], al
    cmp     al, 0FDh ; '˝'
    jb      short loc_208F3
    mov     [bp+var_E], 0
loc_208F3:
    cmp     [bp+var_E], 0B6h ; '∂'
    jb      short loc_20919
    mov     [bp+var_E], 4
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata14
    mov     byte ptr es:[bx], 4
loc_20919:
    mov     al, [bp+var_E]
    sub     ah, ah
    cmp     ax, 93h ; 'ì'
    jnz     short loc_20926
    jmp     loc_2082C
loc_20926:
    jbe     short loc_2092B
    jmp     loc_2085A
loc_2092B:
    cmp     ax, 1
    jnz     short loc_20933
    jmp     loc_2082C
loc_20933:
    cmp     ax, 86h ; 'Ü'
    jnz     short loc_2093B
    jmp     loc_2082C
loc_2093B:
    cmp     ax, 87h ; 'á'
    jnz     short loc_20943
    jmp     loc_20842
loc_20943:
    cmp     ax, 88h ; 'à'
    jnz     short loc_2094B
    jmp     loc_2084A
loc_2094B:
    cmp     ax, 89h ; 'â'
    jmp     loc_20876
    ; align 2
    db 144
loc_20952:
    inc     [bp+var_73A]
loc_20956:
    cmp     [bp+var_73A], 1Eh
    jge     short loc_20966
    mov     [bp+var_3A4], 0
    jmp     loc_208BF
    ; align 2
    db 144
loc_20966:
    cmp     [bp+var_3A6], 0
    jnz     short loc_20976
    mov     [bp+var_3B0], 1
    jmp     loc_2078D
    ; align 2
    db 144
loc_20976:
    mov     word_45DD0, 0
    mov     [bp+var_746], 0
    mov     byte_45635, 0
    mov     byte_4616E, 0
    mov     [bp+var_3A8], 0
    mov     [bp+var_AE8], 0
    sub     si, si
loc_20997:
    mov     [bp+si+var_738], 0
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_AEA], ax
    mov     bx, ax
    add     bx, word ptr trackdata1
    mov     es, word ptr trackdata1+2
    mov     word ptr es:[bx], 0FFFFh
    mov     bx, [bp+var_AEA]
    add     bx, word ptr trackdata2
    mov     es, word ptr trackdata2+2
    mov     word ptr es:[bx], 0FFFFh
    inc     si
    cmp     si, 385h
    jl      short loc_20997
    mov     al, startcol2
    mov     [bp+var_3A4], al
    mov     al, startrow2
    mov     [bp+var_73A], al
    mov     ax, track_angle
    mov     [bp+var_8], ax
    mov     [bp+var_12], 0
    mov     [bp+var_3AC], 0FFFFh
loc_209E9:
    mov     [bp+var_2], 0
    cmp     [bp+var_3A4], 0
    jl      short loc_20A0C
    cmp     [bp+var_73A], 0
    jl      short loc_20A0C
    cmp     [bp+var_3A4], 1Dh
    jg      short loc_20A0C
    cmp     [bp+var_73A], 1Dh
    jg      short loc_20A0C
    jmp     loc_20AC2
loc_20A0C:
    cmp     [bp+var_746], 0
    jnz     short loc_20A16
    jmp     loc_20F20
loc_20A16:
    dec     [bp+var_746]
    mov     al, [bp+var_746]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_74A]
    mov     dx, [bp+var_748]
    mov     word ptr [bp+var_3A0], ax
    mov     word ptr [bp+var_3A0+2], dx
    les     bx, [bp+var_3A0]
    mov     al, es:[bx]
    mov     [bp+var_3A4], al
    mov     al, es:[bx+1]
    mov     [bp+var_73A], al
    mov     al, es:[bx+2]
    mov     [bp+var_E], al
    mov     al, es:[bx+3]
    mov     [bp+var_742], al
    mov     al, es:[bx+4]
    mov     [bp+var_73E], al
    mov     al, es:[bx+0Bh]
    mov     [bp+var_12], al
    mov     ax, es:[bx+0Ch]
    mov     [bp+var_3AC], ax
    mov     al, es:[bx+5]
    mov     [bp+var_3A8], al
    mov     al, es:[bx+6]
    mov     [bp+var_10], al
    mov     al, es:[bx+7]
    mov     [bp+var_39A], al
    mov     al, es:[bx+8]
    mov     [bp+var_AE6], al
    mov     al, es:[bx+9]
    mov     [bp+var_3AA], al
    mov     al, es:[bx+0Ah]
    mov     [bp+var_740], al
    mov     [bp+var_2], 1
loc_20AA8:
    cmp     [bp+var_2], 0
    jnz     short loc_20AB1
    jmp     loc_209E9
loc_20AB1:
    cmp     [bp+var_4], 1
    ja      short loc_20ABA
    jmp     loc_20F6A
loc_20ABA:
    mov     [bp+var_3B0], 0Ah
    jmp     loc_2078D
loc_20AC2:
    mov     al, [bp+var_3A4]
    cbw
    mov     [bp+var_AEA], ax
    mov     al, [bp+var_73A]
    cbw
    shl     ax, 1
    mov     [bp+var_AEC], ax
    mov     bx, ax
    mov     bx, trackrows[bx]
    add     bx, [bp+var_AEA]
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_E], al
    mov     bx, [bp+var_AEC]
    mov     bx, terrainrows[bx]
    add     bx, [bp+var_AEA]
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    mov     al, es:[bx]
    mov     [bp+var_74E], al
    cmp     [bp+var_E], 0
    jz      short loc_20B30
    or      al, al
    jz      short loc_20B30
    cmp     al, 7
    jb      short loc_20B30
    cmp     al, 0Bh
    jnb     short loc_20B30
    mov     al, [bp+var_E]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_74E]
    push    ax
    push    cs
    call    near ptr map_terrain_track
    add     sp, 4
    mov     [bp+var_E], al
loc_20B30:
    cmp     [bp+var_E], 0FDh ; '˝'
    jnb     short loc_20B39
    jmp     loc_20C12
loc_20B39:
    mov     al, [bp+var_E]
    sub     ah, ah
    cmp     ax, 0FDh ; '˝'
    jz      short loc_20B76
    cmp     ax, 0FEh ; '˛'
    jz      short loc_20BA2
    cmp     ax, 0FFh
    jnz     short loc_20B50
    jmp     loc_20BD4
loc_20B50:
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata14
    mov     es, word ptr trackdata14+2
    mov     al, es:[bx]
    mov     [bp+var_E], al
    jmp     loc_20C2E
    ; align 2
    db 144
loc_20B76:
    dec     [bp+var_3A4]
    dec     [bp+var_73A]
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_20B96
    cmp     ax, 100h
    jz      short loc_20C02
    cmp     ax, 200h
    jz      short loc_20C02
    cmp     ax, 300h
    jz      short loc_20B9C
    jmp     short loc_20B50
loc_20B96:
    mov     [bp+var_6], 0Ch
    jmp     short loc_20B50
loc_20B9C:
    mov     [bp+var_6], 9
    jmp     short loc_20B50
loc_20BA2:
    dec     [bp+var_73A]
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_20BBE
    cmp     ax, 100h
    jz      short loc_20BC4
    cmp     ax, 200h
    jz      short loc_20C02
    cmp     ax, 300h
    jz      short loc_20BCC
    jmp     short loc_20B50
loc_20BBE:
    mov     [bp+var_6], 0Bh
    jmp     short loc_20B50
loc_20BC4:
    mov     [bp+var_6], 6
    jmp     short loc_20B50
    ; align 4
    db 144
    db 144
loc_20BCC:
    mov     [bp+var_6], 7
    jmp     loc_20B50
    ; align 2
    db 144
loc_20BD4:
    dec     [bp+var_3A4]
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_20BF2
    cmp     ax, 100h
    jz      short loc_20C02
    cmp     ax, 200h
    jz      short loc_20BFA
    cmp     ax, 300h
    jz      short loc_20C0A
    jmp     loc_20B50
    ; align 2
    db 144
loc_20BF2:
    mov     [bp+var_6], 0Ah
    jmp     loc_20B50
    ; align 2
    db 144
loc_20BFA:
    mov     [bp+var_6], 5
    jmp     loc_20B50
    ; align 2
    db 144
loc_20C02:
    mov     [bp+var_6], 0
    jmp     loc_20B50
    ; align 2
    db 144
loc_20C0A:
    mov     [bp+var_6], 8
    jmp     loc_20B50
    ; align 2
    db 144
loc_20C12:
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_20C2A
    cmp     ax, 100h
    jz      short loc_20C48
    cmp     ax, 200h
    jz      short loc_20C42
    cmp     ax, 300h
    jz      short loc_20C4E
    jmp     short loc_20C2E
loc_20C2A:
    mov     [bp+var_6], 2
loc_20C2E:
    cmp     [bp+var_4], 0
    jnz     short loc_20C54
    cmp     [bp+var_6], 0
    jnz     short loc_20C54
    mov     [bp+var_3B0], 2
    jmp     loc_2078D
loc_20C42:
    mov     [bp+var_6], 1
    jmp     short loc_20C2E
loc_20C48:
    mov     [bp+var_6], 4
    jmp     short loc_20C2E
loc_20C4E:
    mov     [bp+var_6], 3
    jmp     short loc_20C2E
loc_20C54:
    mov     [bp+var_2], 0
    mov     al, [bp+var_E]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_39C], ax
    or      ax, ax
    jnz     short loc_20C78
    jmp     loc_20E82
loc_20C78:
    sub     si, si
    jmp     loc_20E3A
    ; align 2
    db 144
loc_20C7E:
    mov     [bp+var_AE4], 0
    jmp     short loc_20CA2
    ; align 2
    db 144
loc_20C86:
    mov     bx, [bp+var_3AE]
    mov     al, [bp+var_6]
    cmp     [bx+2], al
    jnz     short loc_20CA2
    mov     al, [bp+var_12]
    cmp     [bx+4], al
    jz      short loc_20C9D
    jmp     loc_20E79
loc_20C9D:
    mov     [bp+var_AE4], 1
loc_20CA2:
    cmp     [bp+var_AE4], 0
    jge     short loc_20CAC
    jmp     loc_20D48
loc_20CAC:
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, bp
    cmp     byte ptr [bx-738h], 0
    jz      short loc_20D48
    sub     di, di
    jmp     short loc_20D0E
    ; align 2
    db 144
loc_20CCE:
    mov     [bp+var_AE4], 0FFh
    mov     ax, [bp+var_3AC]
    shl     ax, 1
    add     ax, word ptr trackdata1
    mov     dx, word ptr trackdata1+2
    mov     word ptr [bp+var_AF0], ax
    mov     word ptr [bp+var_AF0+2], dx
    les     bx, [bp+var_AF0]
    cmp     word ptr es:[bx], 0FFFFh
    jz      short loc_20D01
    mov     bx, [bp+var_3AC]
    shl     bx, 1
    add     bx, word ptr trackdata2
    mov     es, word ptr trackdata2+2
loc_20D01:
    mov     es:[bx], di
    or      di, di
    jnz     short loc_20D0D
    mov     [bp+var_AE8], 1
loc_20D0D:
    inc     di
loc_20D0E:
    cmp     word_45DD0, di
    jle     short loc_20D48
    les     bx, trackdata21
    mov     al, [bp+var_3A4]
    cmp     es:[bx+di], al
    jnz     short loc_20D0D
    les     bx, trackdata22
    mov     al, [bp+var_73A]
    cmp     es:[bx+di], al
    jnz     short loc_20D0D
    mov     ax, si
    cmp     [bp+di+var_AD4], al
    jnz     short loc_20D0D
    mov     al, [bp+var_AE4]
    cmp     [bp+di+var_398], al
    jz      short loc_20CCE
    mov     [bp+var_3B0], 5
    jmp     loc_2078D
loc_20D48:
    cmp     [bp+var_AE4], 0
    jge     short loc_20D52
    jmp     loc_20E39
loc_20D52:
    cmp     [bp+var_2], 0
    jnz     short loc_20D6A
    mov     ax, si
    mov     [bp+var_742], al
    mov     al, [bp+var_AE4]
    mov     [bp+var_73E], al
    jmp     loc_20E36
    ; align 2
    db 144
loc_20D6A:
    cmp     [bp+var_746], 40h ; '@'
    jnz     short loc_20D7A
    mov     [bp+var_3B0], 8
    jmp     loc_2078D
    ; align 2
    db 144
loc_20D7A:
    mov     al, [bp+var_746]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_74A]
    mov     dx, [bp+var_748]
    mov     word ptr [bp+var_3A0], ax
    mov     word ptr [bp+var_3A0+2], dx
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_3A4]
    mov     es:[bx], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_73A]
    mov     es:[bx+1], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_E]
    mov     es:[bx+2], al
    les     bx, [bp+var_3A0]
    mov     ax, si
    mov     es:[bx+3], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_AE4]
    mov     es:[bx+4], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_12]
    mov     es:[bx+0Bh], al
    les     bx, [bp+var_3A0]
    mov     ax, [bp+var_3AC]
    mov     es:[bx+0Ch], ax
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_3A8]
    mov     es:[bx+5], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_10]
    mov     es:[bx+6], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_39A]
    mov     es:[bx+7], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_AE6]
    mov     es:[bx+8], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_3AA]
    mov     es:[bx+9], al
    les     bx, [bp+var_3A0]
    mov     al, [bp+var_740]
    mov     es:[bx+0Ah], al
    inc     [bp+var_746]
loc_20E36:
    inc     [bp+var_2]
loc_20E39:
    inc     si
loc_20E3A:
    mov     bx, [bp+var_39C]
    mov     al, [bx]
    sub     ah, ah
    cmp     ax, si
    jbe     short loc_20E82
    mov     [bp+var_AE4], 0FFh
    mov     ax, bx
    mov     cx, si
    mov     dx, cx
    shl     cx, 1
    add     cx, dx
    shl     cx, 1
    add     cx, dx
    shl     cx, 1
    add     ax, cx
    mov     [bp+var_3AE], ax
    mov     bx, ax
    mov     al, [bp+var_6]
    cmp     [bx+1], al
    jz      short loc_20E6E
    jmp     loc_20C86
loc_20E6E:
    mov     al, [bp+var_12]
    cmp     [bx+3], al
    jnz     short loc_20E79
    jmp     loc_20C7E
loc_20E79:
    mov     [bp+var_3B0], 4
    jmp     loc_2078D
    ; align 2
    db 144
loc_20E82:
    cmp     [bp+var_2], 0
    jz      short loc_20E8B
    jmp     loc_20AA8
loc_20E8B:
    cmp     [bp+var_12], 1
    jz      short loc_20E94
    jmp     loc_20A0C
loc_20E94:
    cmp     [bp+var_4], 2
    jb      short loc_20E9D
    jmp     loc_20A0C
loc_20E9D:
    cmp     [bp+var_3A8], 2
    jnb     short loc_20EAC
    mov     [bp+var_3B0], 9
    jmp     loc_2078D
loc_20EAC:
    inc     [bp+var_3A8]
    inc     [bp+var_4]
    mov     ax, [bp+var_8]
    or      ax, ax
    jz      short loc_20ECC
    cmp     ax, 100h
    jz      short loc_20EF6
    cmp     ax, 200h
    jz      short loc_20EE4
    cmp     ax, 300h
    jz      short loc_20F08
    jmp     loc_209E9
loc_20ECC:
    mov     al, [bp+var_10]
    mov     [bp+var_3A4], al
    mov     al, [bp+var_39A]
    sub     al, [bp+var_4]
    dec     al
loc_20EDC:
    mov     [bp+var_73A], al
    jmp     loc_209E9
    ; align 2
    db 144
loc_20EE4:
    mov     al, [bp+var_10]
    mov     [bp+var_3A4], al
    mov     al, [bp+var_39A]
    add     al, [bp+var_4]
    inc     al
    jmp     short loc_20EDC
loc_20EF6:
    mov     al, [bp+var_39A]
    mov     [bp+var_73A], al
    mov     al, [bp+var_10]
    add     al, [bp+var_4]
    inc     al
    jmp     short loc_20F18
loc_20F08:
    mov     al, [bp+var_39A]
    mov     [bp+var_73A], al
    mov     al, [bp+var_10]
    sub     al, [bp+var_4]
    dec     al
loc_20F18:
    mov     [bp+var_3A4], al
    jmp     loc_209E9
    ; align 2
    db 144
loc_20F20:
    cmp     [bp+var_AE8], 0
    jnz     short loc_20F30
    mov     [bp+var_3B0], 7
    jmp     loc_2078D
    ; align 2
    db 144
loc_20F30:
    mov     al, startcol2
    mov     byte_45D90, al
    mov     al, startrow2
    mov     byte_45E16, al
    mov     ax, word_45DD0
    cwd
    mov     cx, 3
    idiv    cx
    mov     si, ax
    cmp     si, 40h ; '@'
    jle     short loc_20F4F
    mov     si, 40h ; '@'
loc_20F4F:
    mov     ax, si
    mov     byte_4616E, al
    sub     si, si
loc_20F56:
    mov     [bp+si+var_AD4], 0
    inc     si
    cmp     si, 385h
    jl      short loc_20F56
    sub     di, di
    sub     si, si
    jmp     loc_21688
    ; align 2
    db 144
loc_20F6A:
    mov     [bp+var_4], 0
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, bp
    mov     byte ptr [bx-738h], 1
    mov     bx, word_45DD0
    add     bx, bp
    mov     al, [bp+var_742]
    mov     [bx-0AD4h], al
    mov     bx, word_45DD0
    add     bx, bp
    mov     al, [bp+var_73E]
    mov     [bx-398h], al
    cmp     [bp+var_3AC], 0FFFFh
    jz      short loc_20FE0
    mov     ax, [bp+var_3AC]
    shl     ax, 1
    add     ax, word ptr trackdata1
    mov     dx, word ptr trackdata1+2
    mov     word ptr [bp+var_AF0], ax
    mov     word ptr [bp+var_AF0+2], dx
    les     bx, [bp+var_AF0]
    cmp     word ptr es:[bx], 0FFFFh
    jz      short loc_20FDA
    mov     bx, [bp+var_3AC]
    shl     bx, 1
    add     bx, word ptr trackdata2
    mov     es, word ptr trackdata2+2
loc_20FDA:
    mov     ax, word_45DD0
    mov     es:[bx], ax
loc_20FE0:
    mov     ax, word_45DD0
    mov     [bp+var_3AC], ax
    mov     bx, ax
    add     bx, word ptr trackdata21
    mov     es, word ptr trackdata21+2
    mov     al, [bp+var_3A4]
    mov     es:[bx], al
    mov     bx, word_45DD0
    add     bx, word ptr trackdata22
    mov     es, word ptr trackdata22+2
    mov     al, [bp+var_73A]
    mov     es:[bx], al
    mov     bx, word_45DD0
    add     bx, word ptr trackdata18
    mov     es, word ptr trackdata18+2
    mov     al, [bp+var_73E]
    mov     cl, 4
    shl     al, cl
    add     al, [bp+var_742]
    mov     es:[bx], al
    mov     bx, word_45DD0
    add     bx, word ptr trackdata17
    mov     es, word ptr trackdata17+2
    mov     al, [bp+var_E]
    mov     es:[bx], al
    mov     al, [bp+var_E]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_39C], ax
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_39C]
    mov     [bp+var_3AE], ax
    mov     bx, ax
    mov     al, [bx+0Ch]
    mov     [bp+var_74C], al
    or      al, al
    jnz     short loc_21080
    inc     [bp+var_3A8]
    jmp     loc_21367
    ; align 2
    db 144
loc_21080:
    cmp     [bp+var_74C], 0FFh
    jnz     short loc_2108A
    jmp     loc_21362
loc_2108A:
    cmp     [bp+var_3A8], 3
    ja      short loc_21094
    jmp     loc_21362
loc_21094:
    cmp     byte_45635, 30h ; '0'
    jnz     short loc_2109E
    jmp     loc_21362
loc_2109E:
    mov     al, [bp+var_E]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_39C], ax
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_39C]
    mov     [bp+var_3AE], ax
    mov     bx, ax
    mov     al, [bx+0Ch]
    mov     [bp+var_74C], al
    mov     al, [bp+var_AE6]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_39C], ax
    mov     al, [bp+var_3AA]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_39C]
    mov     [bp+var_3AE], ax
    cmp     [bp+var_740], 0
    jz      short loc_21122
    mov     bx, ax
    cmp     word ptr [bx+0Ah], 0
    jz      short loc_21126
    mov     ax, [bx+0Ah]
    jmp     short loc_21129
loc_21122:
    mov     bx, [bp+var_3AE]
loc_21126:
    mov     ax, [bx+8]
loc_21129:
    mov     [bp+var_ADE], ax
    mov     [bp+var_ADC], ds
    cmp     [bp+var_740], 0
    jz      short loc_21156
    mov     bx, [bp+var_3AE]
    mov     al, [bx+5]
    sub     ah, ah
    shl     ax, 1
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_ADE]
    mov     dx, ds
    add     ax, 0Ch
    jmp     short loc_21174
loc_21156:
    mov     bx, [bp+var_3AE]
    mov     al, [bx+5]
    sub     ah, ah
    shl     ax, 1
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_ADE]
    mov     dx, [bp+var_ADC]
    add     ax, 6
loc_21174:
    push    si
    push    di
    lea     di, [bp+var_ADA]
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
    cmp     [bp+var_73E], 0
    jz      short loc_2119C
    mov     al, [bp+var_74C]
    cbw
    mov     bx, ax
    mov     al, byte_3E724[bx]
    jmp     short loc_211A7
    ; align 2
    db 144
loc_2119C:
    mov     al, [bp+var_74C]
    cbw
    mov     bx, ax
    mov     al, byte_3E71E[bx]
loc_211A7:
    mov     [bp+var_74C], al
    mov     bx, [bp+var_3AE]
    mov     ax, [bx+6]
    mov     [bp+var_8], ax
    cmp     ax, 100h
    jz      short loc_21214
    cmp     ax, 200h
    jz      short loc_211FE
    cmp     ax, 300h
    jnz     short loc_211DE
    mov     ax, [bp+var_ADA]
    mov     [bp+var_3A2], ax
    mov     ax, [bp+var_AD6]
    neg     ax
    mov     [bp+var_ADA], ax
    mov     ax, [bp+var_3A2]
loc_211DA:
    mov     [bp+var_AD6], ax
loc_211DE:
    cmp     [bp+var_740], 0
    jz      short loc_2122C
    mov     al, byte_45635
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata8
    mov     es, word ptr trackdata8+2
    mov     ax, [bp+var_8]
    xor     ah, 2
    jmp     short loc_2123F
    ; align 2
    db 144
loc_211FE:
    mov     ax, [bp+var_AD6]
    neg     ax
    mov     [bp+var_AD6], ax
    mov     ax, [bp+var_ADA]
    neg     ax
    mov     [bp+var_ADA], ax
    jmp     short loc_211DE
loc_21214:
    mov     ax, [bp+var_ADA]
    mov     [bp+var_3A2], ax
    mov     ax, [bp+var_AD6]
    mov     [bp+var_ADA], ax
    mov     ax, [bp+var_3A2]
    neg     ax
    jmp     short loc_211DA
loc_2122C:
    mov     al, byte_45635
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata8
    mov     es, word ptr trackdata8+2
    mov     ax, [bp+var_8]
loc_2123F:
    mov     es:[bx], ax
    mov     al, byte_45635
    cbw
    mov     bx, ax
    add     bx, word ptr trackdata23
    mov     es, word ptr trackdata23+2
    mov     al, [bp+var_74C]
    mov     es:[bx], al
    mov     al, [bp+var_39A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, terrainrows[bx]
    mov     al, [bp+var_10]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    cmp     byte ptr es:[bx], 6
    jnz     short loc_2127E
    add     [bp+var_AD8], 1C2h
loc_2127E:
    mov     al, byte_45635
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     ax, [bp+var_AD8]
    mov     es:[bx+2], ax
    mov     al, [bp+var_AE6]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 1
    jz      short loc_212C2
    mov     al, [bp+var_39A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackpos[bx]
    jmp     short loc_212CF
loc_212C2:
    mov     al, [bp+var_39A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
loc_212CF:
    add     ax, [bp+var_AD6]
    mov     cx, ax
    mov     al, byte_45635
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     es:[bx+4], cx
    mov     al, [bp+var_AE6]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 2
    jz      short loc_21314
    mov     al, [bp+var_10]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, (trackpos2+2)[bx]
    jmp     short loc_21320
loc_21314:
    mov     al, [bp+var_10]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
loc_21320:
    add     ax, [bp+var_ADA]
    mov     cx, ax
    mov     al, byte_45635
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, word ptr trackdata10
    mov     es, word ptr trackdata10+2
    mov     es:[bx], cx
    mov     al, [bp+var_39A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, trackrows[bx]
    mov     al, [bp+var_10]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata19
    mov     es, word ptr trackdata19+2
    mov     al, byte_45635
    mov     es:[bx], al
    inc     byte_45635
loc_21362:
    mov     [bp+var_3A8], 0
loc_21367:
    inc     word_45DD0
    cmp     word_45DD0, 385h
    jnz     short loc_2137C
    mov     [bp+var_3B0], 6
    jmp     loc_2078D
    ; align 2
    db 144
loc_2137C:
    mov     al, [bp+var_E]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_39C], ax
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_39C]
    mov     [bp+var_3AE], ax
    cmp     [bp+var_73E], 0
    jz      short loc_213C4
    mov     bx, ax
    mov     al, [bx+1]
    mov     [bp+var_744], al
    mov     al, [bx+3]
    jmp     short loc_213D2
loc_213C4:
    mov     bx, [bp+var_3AE]
    mov     al, [bx+2]
    mov     [bp+var_744], al
    mov     al, [bx+4]
loc_213D2:
    mov     [bp+var_12], al
    mov     al, [bp+var_3A4]
    mov     [bp+var_10], al
    mov     al, [bp+var_73A]
    mov     [bp+var_39A], al
    mov     al, [bp+var_73E]
    mov     [bp+var_740], al
    mov     al, [bp+var_742]
    mov     [bp+var_3AA], al
    mov     al, [bp+var_E]
    mov     [bp+var_AE6], al
    mov     al, [bp+var_744]
    sub     ah, ah
    sub     ax, 1
    cmp     ax, 0Bh
    jbe     short loc_2140C
    jmp     loc_209E9
loc_2140C:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_2147C[bx]
loc_21414:
    dec     [bp+var_73A]
loc_21418:
    mov     [bp+var_8], 0
    jmp     loc_209E9
loc_21420:
    dec     [bp+var_73A]
    inc     [bp+var_3A4]
    jmp     short loc_21418
loc_2142A:
    inc     [bp+var_73A]
loc_2142E:
    dec     [bp+var_3A4]
    mov     [bp+var_8], 300h
    jmp     loc_209E9
loc_2143A:
    inc     [bp+var_3A4]
loc_2143E:
    inc     [bp+var_73A]
loc_21442:
    mov     [bp+var_8], 200h
    jmp     loc_209E9
loc_2144A:
    inc     [bp+var_3A4]
loc_2144E:
    add     [bp+var_73A], 2
    jmp     short loc_21442
    ; align 2
    db 144
loc_21456:
    inc     [bp+var_3A4]
loc_2145A:
    mov     [bp+var_8], 100h
    jmp     loc_209E9
loc_21462:
    inc     [bp+var_3A4]
loc_21466:
    inc     [bp+var_73A]
    jmp     short loc_2145A
loc_2146C:
    add     [bp+var_3A4], 2
    jmp     short loc_2145A
    ; align 2
    db 144
loc_21474:
    add     [bp+var_3A4], 2
    jmp     short loc_21466
    ; align 2
    db 144
off_2147C     dw offset loc_21414
    dw offset loc_2143E
    dw offset loc_21456
    dw offset loc_2142E
    dw offset loc_21420
    dw offset loc_2142A
    dw offset loc_21462
    dw offset loc_2146C
    dw offset loc_21474
    dw offset loc_2143A
    dw offset loc_2144E
    dw offset loc_2144A
    jmp     loc_209E9
    ; align 2
    db 144
loc_21498:
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, [bp+var_3AE]
    mov     ax, [bx+8]
loc_214B1:
    mov     [bp+var_ADE], ax
    mov     [bp+var_ADC], ds
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_3AE]
    mov     word ptr [bp+var_AF0], ax
    mov     bx, ax
    mov     al, [bx+5]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    shl     ax, 1
    add     ax, [bp+var_ADE]
    mov     dx, ds
    push    si
    push    di
    lea     di, [bp+var_ADA]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    movsw
    movsw
    movsw
    pop     ds
    pop     di
    pop     si
    mov     ax, [bx+6]
    mov     [bp+var_8], ax
    cmp     ax, 100h
    jz      short loc_21576
    cmp     ax, 200h
    jz      short loc_21560
    cmp     ax, 300h
    jnz     short loc_2152A
    mov     ax, [bp+var_ADA]
    mov     [bp+var_3A2], ax
    mov     ax, [bp+var_AD6]
    neg     ax
    mov     [bp+var_ADA], ax
    mov     ax, [bp+var_3A2]
loc_21526:
    mov     [bp+var_AD6], ax
loc_2152A:
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     bx, terrainrows[bx]
    mov     al, [bp+var_3A4]
    cbw
    add     bx, ax
    add     bx, word ptr trackdata15
    mov     es, word ptr trackdata15+2
    cmp     byte ptr es:[bx], 6
    jnz     short loc_2158E
    mov     bx, di
    shl     bx, 1
    add     bx, word ptr trackdata7
    mov     es, word ptr trackdata7+2
    mov     word ptr es:[bx], 1C2h
    jmp     short loc_2159F
    ; align 2
    db 144
loc_21560:
    mov     ax, [bp+var_AD6]
    neg     ax
    mov     [bp+var_AD6], ax
    mov     ax, [bp+var_ADA]
    neg     ax
    mov     [bp+var_ADA], ax
    jmp     short loc_2152A
loc_21576:
    mov     ax, [bp+var_ADA]
    mov     [bp+var_3A2], ax
    mov     ax, [bp+var_AD6]
    mov     [bp+var_ADA], ax
    mov     ax, [bp+var_3A2]
    neg     ax
    jmp     short loc_21526
loc_2158E:
    mov     bx, di
    shl     bx, 1
    add     bx, word ptr trackdata7
    mov     es, word ptr trackdata7+2
    mov     word ptr es:[bx], 0
loc_2159F:
    mov     ax, di
    shl     ax, 1
    mov     word ptr [bp+var_AF0], ax
    mov     bx, ax
    add     bx, word ptr trackdata6
    mov     es, word ptr trackdata6+2
    mov     word ptr es:[bx], 0
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_AEC], ax
    mov     bx, word ptr [bp+var_AF0]
    add     bx, word ptr trackdata7
    mov     es, word ptr trackdata7+2
    mov     ax, es:[bx]
    add     ax, [bp+var_AD8]
    mov     bx, [bp+var_AEC]
    add     bx, word ptr trackdata9
    mov     es, word ptr trackdata9+2
    mov     es:[bx+2], ax
    mov     bx, [bp+var_C]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 1
    jz      short loc_21614
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackpos[bx]
    add     ax, [bp+var_AD6]
    mov     bx, [bp+var_AEC]
    jmp     short loc_2162F
loc_21614:
    mov     al, [bp+var_73A]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos[bx]
    add     ax, [bp+var_AD6]
    mov     bx, di
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
loc_2162F:
    add     bx, word ptr trackdata9
    mov     es, word ptr trackdata9+2
    mov     es:[bx+4], ax
    mov     bx, [bp+var_C]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    test    sceneshapes.scene_translateflag[bx], 2
    jz      short loc_21660
    mov     al, [bp+var_3A4]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, (trackpos2+2)[bx]
    jmp     short loc_2166D
loc_21660:
    mov     al, [bp+var_3A4]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, trackcenterpos2[bx]
loc_2166D:
    add     ax, [bp+var_ADA]
    mov     bx, di
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
    add     bx, word ptr trackdata9
    mov     es, word ptr trackdata9+2
    mov     es:[bx], ax
    inc     di
loc_21687:
    inc     si
loc_21688:
    mov     al, byte_4616E
    cbw
    mov     word ptr [bp+var_AF0], ax
    cmp     ax, si
    jg      short loc_21697
    jmp     loc_2176E
loc_21697:
    mov     ax, word_45DD0
    imul    si
    cwd
    mov     cx, word ptr [bp+var_AF0]
    idiv    cx
    mov     [bp+var_A], ax
    mov     bx, ax
    add     bx, word ptr trackdata21
    mov     es, word ptr trackdata21+2
    mov     al, es:[bx]
    mov     [bp+var_3A4], al
    mov     bx, [bp+var_A]
    add     bx, word ptr trackdata22
    mov     es, word ptr trackdata22+2
    mov     al, es:[bx]
    mov     [bp+var_73A], al
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, terrainrows[bx]
    mov     cx, ax
    mov     al, [bp+var_3A4]
    cbw
    add     cx, ax
    add     cx, bp
    sub     cx, 0AD4h
    mov     [bp+var_AEC], cx
    mov     bx, cx
    cmp     byte ptr [bx], 0
    jnz     short loc_21687
    mov     byte ptr [bx], 1
    mov     bx, [bp+var_A]
    add     bx, word ptr trackdata17
    mov     es, word ptr trackdata17+2
    mov     al, es:[bx]
    sub     ah, ah
    mov     [bp+var_C], ax
    mov     bx, [bp+var_A]
    add     bx, word ptr trackdata18
    mov     es, word ptr trackdata18+2
    mov     al, es:[bx]
smart
    and     al, 0Fh
nosmart
    mov     [bp+var_742], al
    mov     bx, [bp+var_A]
    add     bx, word ptr trackdata18
    mov     al, es:[bx]
smart
    and     al, 10h
nosmart
    mov     [bp+var_AE4], al
    mov     bx, [bp+var_C]
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, word ptr sceneshapes.scene_unk[bx]
    mov     [bp+var_3AE], ax
    cmp     [bp+var_AE4], 0
    jnz     short loc_21747
    jmp     loc_21498
loc_21747:
    mov     al, [bp+var_742]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, [bp+var_3AE]
    mov     ax, [bx+0Ah]
    mov     [bp+var_AEA], ax
    or      ax, ax
    jnz     short loc_2176B
    jmp     loc_21498
loc_2176B:
    jmp     loc_214B1
loc_2176E:
    mov     ax, di
    mov     byte_4616E, al
    mov     [bp+var_3B0], 0
    jmp     short loc_217AE
loc_2177A:
    cmp     [bp+var_3A4], 1Eh
    jnz     short loc_21786
    mov     [bp+var_3A4], 1Dh
loc_21786:
    cmp     [bp+var_73A], 0FFh
    jnz     short loc_21794
    mov     [bp+var_73A], 0
    jmp     short loc_217A0
loc_21794:
    cmp     [bp+var_73A], 1Eh
    jnz     short loc_217A0
    mov     [bp+var_73A], 1Dh
loc_217A0:
    mov     al, [bp+var_3A4]
    mov     byte_45D90, al
    mov     al, [bp+var_73A]
    mov     byte_45E16, al
loc_217AE:
    push    [bp+var_AE0]
    push    [bp+var_AE2]
    call    mmgr_op_unk2
    add     sp, 4
    mov     al, [bp+var_3B0]
    cbw
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
setup_track endp
sub_217CA proc far
    var_F30 = word ptr -3888
    var_B2E = word ptr -2862
    var_B2C = word ptr -2860
    var_B2A = word ptr -2858
    var_B28 = word ptr -2856
    var_B26 = word ptr -2854
    var_B24 = word ptr -2852
    var_414 = word ptr -1044
    var_412 = word ptr -1042
    var_410 = word ptr -1040
    var_20E = word ptr -526
    var_20C = word ptr -524
    var_A = dword ptr -10
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 0F30h
    push    di
    push    si
    mov     al, gameconfig.game_opponenttype
    add     al, 30h ; '0'
    mov     byte ptr aOpp1+3, al
    mov     ax, offset aOpp1; "opp1"
    push    ax
    call    file_load_resfile
    add     sp, 2
    mov     [bp+var_B2E], ax
    mov     [bp+var_B2C], dx
    mov     ax, offset aNam ; "nam"
    push    ax
    push    dx
    push    [bp+var_B2E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset unk_46464
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset aPath; "path"
    push    ax
    push    [bp+var_B2C]
    push    [bp+var_B2E]
    call    locate_shape_alt
    add     sp, 6
    mov     [bp+var_414], ax
    mov     [bp+var_412], dx
    mov     ax, offset aSped; "sped"
    push    ax
    push    [bp+var_B2C]
    push    [bp+var_B2E]
    call    locate_shape_alt
    add     sp, 6
    mov     word ptr [bp+var_A], ax
    mov     word ptr [bp+var_A+2], dx
    sub     si, si
loc_21846:
    les     bx, [bp+var_A]
    mov     al, es:[bx+si]
    mov     byte_44D50[si], al
    inc     si
    cmp     si, 10h
    jl      short loc_21846
    mov     [bp+var_B26], 423Fh
    mov     [bp+var_B24], 0Fh
    mov     [bp+var_B28], 0
    sub     ax, ax
    mov     [bp+var_20C], ax
    mov     [bp+var_20E], ax
    mov     [bp+var_B2A], ax
    sub     si, si
loc_21878:
    mov     [bp+var_410], 0
    mov     bx, si
    shl     bx, 1
    add     bx, word ptr trackdata1
    mov     es, word ptr trackdata1+2
    mov     ax, es:[bx]
    mov     [bp+var_4], ax
    or      ax, ax
    jnz     short loc_218A2
    mov     [bp+var_2], 1
loc_21899:
    mov     [bp+var_410], 1
    jmp     short loc_218DA
    ; align 2
    db 144
loc_218A2:
    cmp     [bp+var_4], 0FFFFh
    jnz     short loc_218B0
    mov     [bp+var_2], 0
    jmp     short loc_21899
    ; align 2
    db 144
loc_218B0:
    cmp     [bp+var_B28], 0
    jz      short loc_218DA
    sub     di, di
    jmp     short loc_218D4
    ; align 2
    db 144
loc_218BC:
    mov     bx, di
    shl     bx, 1
    add     bx, bp
    cmp     [bx-0B20h], si
    jnz     short loc_218D3
    mov     [bp+var_2], 0
    mov     [bp+var_410], 1
loc_218D3:
    inc     di
loc_218D4:
    cmp     [bp+var_B28], di
    jg      short loc_218BC
loc_218DA:
    mov     bx, [bp+var_B28]
    inc     [bp+var_B28]
    shl     bx, 1
    add     bx, bp
    mov     [bx-0B20h], si
    les     bx, trackdata17
    mov     bl, es:[bx+si]
    sub     bh, bh
    add     bx, word ptr [bp+var_A]
    mov     es, word ptr [bp+var_A+2]
    mov     al, es:[bx]
    sub     ah, ah
    inc     ax
    sub     dx, dx
    add     [bp+var_20E], ax
    adc     [bp+var_20C], dx
    cmp     [bp+var_410], dx
    jnz     short loc_21912
    jmp     loc_219FE
loc_21912:
    cmp     [bp+var_2], dx
    jnz     short loc_2191A
    jmp     loc_219A5
loc_2191A:
    mov     ax, [bp+var_B26]
    mov     dx, [bp+var_B24]
    cmp     [bp+var_20C], dx
    ja      short loc_219A5
    jb      short loc_21930
    cmp     [bp+var_20E], ax
    jnb     short loc_219A5
loc_21930:
    mov     bx, [bp+var_B28]
    inc     [bp+var_B28]
    shl     bx, 1
    add     bx, bp
    mov     word ptr [bx-0B20h], 0
    mov     ax, [bp+var_20E]
    mov     dx, [bp+var_20C]
    mov     [bp+var_B26], ax
    mov     [bp+var_B24], dx
    sub     di, di
    jmp     short loc_21978
    ; align 4
    db 144
    db 144
loc_21958:
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_F30], ax
    mov     bx, ax
    add     bx, bp
    mov     ax, [bx-0B20h]
    mov     bx, [bp+var_F30]
    add     bx, word ptr trackdata3
    mov     es, word ptr trackdata3+2
    mov     es:[bx], ax
    inc     di
loc_21978:
    cmp     [bp+var_B28], di
    jg      short loc_21958
    mov     bx, [bp+var_B28]
    shl     bx, 1
    add     bx, word ptr trackdata3
    mov     es, word ptr trackdata3+2
    mov     word ptr es:[bx], 0
    mov     bx, [bp+var_B28]
    shl     bx, 1
    add     bx, word ptr trackdata3
    mov     es, word ptr trackdata3+2
    mov     word ptr es:[bx+2], 1
loc_219A5:
    cmp     [bp+var_B2A], 0
    jnz     short loc_219C2
    push    [bp+var_B2C]
    push    [bp+var_B2E]
    call    unload_resource
    add     sp, 4
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_219C2:
    dec     [bp+var_B2A]
    mov     ax, [bp+var_B2A]
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_F30], ax
    mov     bx, ax
    mov     si, [bx-20Ah]
    mov     ax, [bx-40Eh]
    mov     [bp+var_B28], ax
    mov     bx, [bp+var_B2A]
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     ax, [bx-0F2Eh]
    mov     dx, [bx-0F2Ch]
    mov     [bp+var_20E], ax
    mov     [bp+var_20C], dx
    jmp     loc_21878
    ; align 2
    db 144
loc_219FE:
    mov     bx, si
    shl     bx, 1
    add     bx, word ptr trackdata2
    mov     es, word ptr trackdata2+2
    mov     ax, es:[bx]
    mov     [bp+var_6], ax
    cmp     ax, 0FFFFh
    jz      short loc_21A54
    mov     ax, [bp+var_B2A]
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_F30], ax
    mov     bx, ax
    mov     ax, [bp+var_6]
    mov     [bx-20Ah], ax
    mov     bx, [bp+var_F30]
    mov     ax, [bp+var_B28]
    mov     [bx-40Eh], ax
    mov     bx, [bp+var_B2A]
    inc     [bp+var_B2A]
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    mov     ax, [bp+var_20E]
    mov     dx, [bp+var_20C]
    mov     [bx-0F2Eh], ax
    mov     [bx-0F2Ch], dx
loc_21A54:
    mov     si, [bp+var_4]
    jmp     loc_21878
sub_217CA endp
map_terrain_track proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = byte ptr 8

    push    bp
    mov     bp, sp
    mov     al, [bp+arg_0]
    sub     ah, ah
    cmp     ax, 7
    jz      short loc_21A80
    cmp     ax, 8
    jz      short loc_21ABE
    cmp     ax, 9
    jnz     short loc_21A74
    jmp     loc_21AFE
loc_21A74:
    cmp     ax, 0Ah
    jnz     short loc_21A7C
    jmp     loc_21B3C
loc_21A7C:
    sub     ax, ax
    pop     bp
    retf
loc_21A80:
    mov     al, [bp+arg_2]
    sub     ah, ah
    cmp     ax, 4
    jz      short loc_21AA6
    cmp     ax, 0Eh
    jz      short loc_21AAC
    cmp     ax, 18h
    jz      short loc_21AB2
    cmp     ax, 27h ; '''
    jz      short loc_21AB8
    cmp     ax, 3Bh ; ';'
    jz      short loc_21AB8
    cmp     ax, 62h ; 'b'
    jz      short loc_21AB8
    jmp     short loc_21A7C
    ; align 2
    db 144
loc_21AA6:
    mov     ax, 0B6h ; '∂'
    jmp     loc_21B77
loc_21AAC:
    mov     ax, 0BAh ; '∫'
    jmp     loc_21B77
loc_21AB2:
    mov     ax, 0BEh ; 'æ'
    jmp     loc_21B77
loc_21AB8:
    mov     ax, 0C2h ; '¬'
    jmp     loc_21B77
loc_21ABE:
    mov     al, [bp+arg_2]
    sub     ah, ah
    cmp     ax, 5
    jz      short loc_21AE4
    cmp     ax, 0Fh
    jz      short loc_21AEA
    cmp     ax, 19h
    jz      short loc_21AF0
    cmp     ax, 24h ; '$'
    jz      short loc_21AF8
    cmp     ax, 38h ; '8'
    jz      short loc_21AF8
    cmp     ax, 5Fh ; '_'
    jz      short loc_21AF8
    jmp     short loc_21A7C
    ; align 2
    db 144
loc_21AE4:
    mov     ax, 0B7h ; '∑'
    jmp     loc_21B77
loc_21AEA:
    mov     ax, 0BBh ; 'ª'
    jmp     loc_21B77
loc_21AF0:
    mov     ax, 0BFh ; 'ø'
    jmp     loc_21B77
    ; align 4
    db 144
    db 144
loc_21AF8:
    mov     ax, 0C3h ; '√'
    jmp     short loc_21B77
    ; align 2
    db 144
loc_21AFE:
    mov     al, [bp+arg_2]
    sub     ah, ah
    cmp     ax, 4
    jz      short loc_21B24
    cmp     ax, 0Eh
    jz      short loc_21B2A
    cmp     ax, 18h
    jz      short loc_21B30
    cmp     ax, 26h ; '&'
    jz      short loc_21B36
    cmp     ax, 3Ah ; ':'
    jz      short loc_21B36
    cmp     ax, 61h ; 'a'
    jz      short loc_21B36
    jmp     loc_21A7C
loc_21B24:
    mov     ax, 0B8h ; '∏'
    jmp     short loc_21B77
    ; align 2
    db 144
loc_21B2A:
    mov     ax, 0BCh ; 'º'
    jmp     short loc_21B77
    ; align 2
    db 144
loc_21B30:
    mov     ax, 0C0h ; '¿'
    jmp     short loc_21B77
    ; align 2
    db 144
loc_21B36:
    mov     ax, 0C4h ; 'ƒ'
    pop     bp
    retf
    ; align 2
    db 144
loc_21B3C:
    mov     al, [bp+arg_2]
    sub     ah, ah
    cmp     ax, 5
    jz      short loc_21B62
    cmp     ax, 0Fh
    jz      short loc_21B68
    cmp     ax, 19h
    jz      short loc_21B6E
    cmp     ax, 25h ; '%'
    jz      short loc_21B74
    cmp     ax, 39h ; '9'
    jz      short loc_21B74
    cmp     ax, 60h ; '`'
    jz      short loc_21B74
    jmp     loc_21A7C
loc_21B62:
    mov     ax, 0B9h ; 'π'
    pop     bp
    retf
    ; align 2
    db 144
loc_21B68:
    mov     ax, 0BDh ; 'Ω'
    pop     bp
    retf
    ; align 2
    db 144
loc_21B6E:
    mov     ax, 0C1h ; '¡'
    pop     bp
    retf
    ; align 2
    db 144
loc_21B74:
    mov     ax, 0C5h ; '≈'
loc_21B77:
    pop     bp
    retf
map_terrain_track endp
seg004 ends
end
