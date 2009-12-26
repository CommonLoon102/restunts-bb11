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
seg027 segment byte public 'STUNTSC' use16
    assume cs:seg027
    assume es:nothing, ss:nothing, ds:dseg
    public init_audio_resources
    public load_audio_finalize
    public sub_37216
    public sub_372F4
    public sub_3736A
    public sub_373B2
    public sub_373B8
    public sub_373E8
    public sub_37470
    public sub_374DE
    public sub_374FA
    public check_audio_flag
    public init_audio_chunk2
    public sub_37696
    public sub_376CA
    public sub_37708
    public sub_3771E
    public audio_driver_func3F
    public sub_37868
    public load_audio_driver
    public audiodrv_atexit
    public load_sfx_ge
    public load_sfx_file
    public load_song_file
    public load_voice_file
    public init_audio_chunk
    public load_vce
    public sub_38156
    public sub_38178
    public audio_hdr1_unk
    public off_383A0
    public sub_384FA
    public sub_38514
    public copy_4_bytes
init_audio_resources proc far
    var_C = dword ptr -12
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = dword ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 0Ch
    push    [bp+arg_8]
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    init_audio_resource
    add     sp, 6
    mov     word ptr [bp+var_C], ax
    mov     word ptr [bp+var_C+2], dx
    or      dx, ax
    jnz     short loc_370FA
loc_370F3:
    sub     ax, ax
    cwd
    mov     sp, bp
    pop     bp
    retf
loc_370FA:
    mov     ax, offset aHdr1; "hdr1"
    push    ax
    push    word ptr [bp+var_C+2]
    push    word ptr [bp+var_C]
    call    init_audio_resource
    add     sp, 6
    mov     word ptr [bp+var_4], ax
    mov     word ptr [bp+var_4+2], dx
    or      ax, dx
    jz      short loc_370F3
    les     bx, [bp+var_4]
    cmp     byte ptr es:[bx+5], 1
    jz      short loc_37172
    push    [bp+arg_6]
    push    [bp+arg_4]
    push    word ptr [bp+var_C+2]
    push    word ptr [bp+var_C]
    push    cs
    call    near ptr load_vce
    add     sp, 8
    push    word ptr [bp+var_C+2]
    push    word ptr [bp+var_C]
    push    cs
    call    near ptr audio_hdr1_unk
    add     sp, 4
    les     bx, [bp+var_4]
    mov     byte ptr es:[bx+5], 1
    les     bx, [bp+var_C]
    mov     al, es:[bx+4]
    sub     ah, ah
    mov     cl, 3
    shl     ax, cl
    add     ax, bx
    mov     dx, es
    inc     ax
    mov     [bp+var_8], ax
    mov     [bp+var_6], dx
    lea     ax, [bp+var_8]
    push    ss
    push    ax
    push    word ptr [bp+var_4+2]
    push    word ptr [bp+var_4]
    push    cs
    call    near ptr copy_4_bytes
    add     sp, 8
loc_37172:
    mov     ax, word ptr [bp+var_4]
    mov     dx, word ptr [bp+var_4+2]
    mov     sp, bp
    pop     bp
    retf
init_audio_resources endp
load_audio_finalize proc far
    var_6 = dword ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    si
    mov     word_4063A, 1
    push    cs
    call    near ptr sub_3736A
    mov     ax, word ptr [bp+arg_0]
    or      ax, word ptr [bp+arg_0+2]
    jz      short loc_3720F
    les     bx, [bp+arg_0]
    cmp     byte ptr es:[bx+4], 0
    jnz     short loc_3720F
    cmp     byte ptr es:[bx+5], 1
    jnz     short loc_3720F
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 18h
    mov     word ptr [bp+var_6+2], dx
    mov     word ptr [bp+var_6], ax
    call    [bp+var_6]
    mov     word_44D48, 0
    mov     word_454BA, 80h ; '€'
    les     bx, [bp+arg_0]
    mov     al, es:[bx+6]
    sub     ah, ah
    shl     ax, 1
    shl     ax, 1
    add     ax, 7
    mov     [bp+var_2], ax
    mov     bx, ax
    inc     [bp+var_2]
    mov     si, word ptr [bp+arg_0]
    mov     al, es:[bx+si]
    mov     byte_44290, al
    mov     ax, 20h ; ' '
    push    ax
    mov     al, byte_45950
    sub     ah, ah
    push    ax
    push    [bp+var_2]
    push    es
    push    si
    mov     al, byte_44290
    dec     ax
    push    ax
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr init_audio_chunk
    add     sp, 0Eh
    mov     byte_40632, 1
    mov     word_4063A, 0
loc_3720F:
    pop     si
    mov     sp, bp
    pop     bp
    retf
    db 144
    db 144
load_audio_finalize endp
sub_37216 proc far
    var_C = word ptr -12
    var_A = dword ptr -10
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 0Ch
    push    di
    push    si
    mov     byte_40630, 1
    mov     word_4063A, 1
    cmp     byte_40634, 0
    jnz     short loc_37262
    sub     si, si
    mov     di, 8224h
loc_37235:
    cmp     byte_40633, 1
    jz      short loc_37241
    cmp     si, 10h
    jge     short loc_37253
loc_37241:
    mov     al, [di]
    mov     [si+714Eh], al
    sub     ax, ax
    push    ax
    push    si
    call    sub_38C4C
    add     sp, 4
loc_37253:
    add     di, 4Ch ; 'L'
    inc     si
    cmp     si, 18h
    jl      short loc_37235
    mov     [bp+var_2], si
    jmp     short loc_37286
    ; align 2
    db 144
loc_37262:
    mov     byte_40639, 0
    mov     ax, 4EC6h
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_A+2], dx
    mov     word ptr [bp+var_A], ax
    call    [bp+var_A]
    add     sp, 6
loc_37286:
    cmp     byte_40634, 0
    jnz     short loc_372E7
    mov     [bp+var_2], 10h
    mov     di, 0A2B6h
    mov     [bp+var_C], 10h
loc_3729A:
    mov     si, di
    push    word ptr [si+12h]
    push    word ptr [si+10h]
    push    word ptr [si+2Ah]
    push    si
    mov     al, [si+2Ch]
    sub     ah, ah
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 27h ; '''
    mov     word ptr [bp+var_A+2], dx
    mov     word ptr [bp+var_A], ax
    call    [bp+var_A]
    add     sp, 0Ah
    add     di, 2Eh ; '.'
    dec     [bp+var_C]
    jnz     short loc_3729A
    mov     [bp+var_4], si
    mov     ax, 0A2B6h
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 30h ; '0'
    mov     word ptr [bp+var_A+2], dx
    mov     word ptr [bp+var_A], ax
    call    [bp+var_A]
    add     sp, 2
loc_372E7:
    mov     word_4063A, 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_37216 endp
sub_372F4 proc far
    var_6 = dword ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    si
    mov     byte_40630, 1
    mov     word_4063A, 1
    cmp     byte_40634, 0
    jnz     short loc_37336
    sub     si, si
loc_3730F:
    cmp     byte_40633, 1
    jz      short loc_3731B
    cmp     si, 10h
    jge     short loc_3732B
loc_3731B:
    mov     al, [si+714Eh]
    sub     ah, ah
    push    ax
    push    si
    call    sub_38C4C
    add     sp, 4
loc_3732B:
    inc     si
    cmp     si, 18h
    jl      short loc_3730F
    mov     [bp+var_2], si
    jmp     short loc_3735A
loc_37336:
    mov     byte_40639, 64h ; 'd'
    mov     ax, 4EC6h
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_6+2], dx
    mov     word ptr [bp+var_6], ax
    call    [bp+var_6]
    add     sp, 6
loc_3735A:
    mov     word_4063A, 0
    mov     byte_40630, 0
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_372F4 endp
sub_3736A proc far

    mov     word_4063A, 1
    mov     byte_40632, 0
    mov     ax, 0Fh
    push    ax
    sub     ax, ax
    push    ax
    call    audio_driver_func1E
    add     sp, 4
    sub     ax, ax
    push    ax
    mov     al, byte_45950
    sub     ah, ah
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    mov     ax, 0Fh
    push    ax
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr init_audio_chunk
    add     sp, 0Eh
    mov     byte_44290, 0
    call    sub_39700
    mov     word_4063A, 0
    retf
    ; align 2
    db 144
sub_3736A endp
sub_373B2 proc far

    mov     byte_40631, 1
    retf
sub_373B2 endp
sub_373B8 proc far

    mov     byte_40631, 0
    mov     word_4063A, 1
    cmp     byte_44290, 0
    jz      short loc_373DC
    mov     al, byte_44290
    sub     ah, ah
    dec     ax
    push    ax
    sub     ax, ax
    push    ax
    call    audio_driver_func1E
    add     sp, 4
loc_373DC:
    call    sub_39700
    mov     word_4063A, 0
    retf
sub_373B8 endp
sub_373E8 proc far

    cmp     byte_40631, 1
    jnz     short loc_373F6
    push    cs
    call    near ptr sub_373B8
    sub     ax, ax
    retf
loc_373F6:
    push    cs
    call    near ptr sub_373B2
    mov     ax, 1
    retf
    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    cmp     byte_40630, 1
    jz      short loc_37414
    cmp     byte_40631, 0
    jnz     short loc_3741E
loc_37414:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_3741E:
    mov     word ptr [bp-2], 0
    mov     al, byte_44290
    sub     ah, ah
    or      ax, ax
    jz      short loc_37414
    mov     di, ax
    mov     si, 81FCh
    mov     cx, [bp-2]
loc_37434:
    mov     ax, [si]
    or      ax, [si+2]
    jz      short loc_37446
    sub     ax, ax
    mov     [bp-2], cx
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_37446:
    add     si, 4Ch ; 'L'
    inc     cx
    mov     ax, cx
    cmp     ax, di
    jb      short loc_37434
    mov     [bp-2], cx
    jmp     short loc_37414
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    mov     ax, 40h ; '@'
    push    ax
    mov     ax, 0FFFFh
    push    ax
    push    word ptr [bp+8]
    push    word ptr [bp+6]
    push    cs
    call    near ptr sub_374FA
    add     sp, 8
    pop     bp
    retf
sub_373E8 endp
sub_37470 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = byte ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    cmp     [bp+arg_0], 0FFFFh
    jnz     short loc_374CE
    mov     si, 10h
    mov     di, 86BCh
    mov     cx, [bp+arg_0]
loc_37487:
    cmp     cx, 0FFFFh
    jnz     short loc_374A5
    mov     ax, [di]
    or      ax, [di+2]
    jnz     short loc_3749C
    cmp     byte ptr [si-59D6h], 0
    jnz     short loc_3749C
    mov     cx, si
loc_3749C:
    add     di, 4Ch ; 'L'
    inc     si
    cmp     si, 17h
    jle     short loc_37487
loc_374A5:
    mov     [bp+var_2], si
    mov     [bp+arg_0], cx
    cmp     cx, 0FFFFh
    jz      short loc_374C5
    mov     bx, cx
    mov     byte ptr [bx-59D6h], 1
    mov     ax, 4Ch ; 'L'
    imul    cx
loc_374BC:
    mov     bx, ax
    mov     al, [bp+arg_2]
    mov     [bx-7DE0h], al
loc_374C5:
    mov     ax, [bp+arg_0]
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_374CE:
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx-59D6h], 1
    mov     ax, 4Ch ; 'L'
    imul    bx
    jmp     short loc_374BC
    ; align 2
    db 144
sub_37470 endp
sub_374DE proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    cmp     [bp+arg_0], 0FFFFh
    jle     short loc_374F7
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx-59D6h], 0
    push    bx
    push    cs
    call    near ptr init_audio_chunk2
    add     sp, 2
loc_374F7:
    pop     bp
    retf
    ; align 2
    db 144
sub_374DE endp
sub_374FA proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = byte ptr 12

    push    bp
    mov     bp, sp
    mov     al, byte_45948
    sub     ah, ah
    push    ax
    mov     al, [bp+arg_6]
    push    ax
    push    [bp+arg_4]
    push    [bp+arg_2]
    push    [bp+arg_0]
    push    cs
    call    near ptr check_audio_flag
    add     sp, 0Ah
    pop     bp
    retf
    ; align 2
    db 144
sub_374FA endp
check_audio_flag proc far
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10
    arg_6 = byte ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 8
    push    di
    push    si
    cmp     byte_40633, 0
    jnz     short loc_37532
loc_37529:
    mov     ax, 0FFFFh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_37532:
    mov     ax, word ptr [bp+arg_0]
    or      ax, word ptr [bp+arg_0+2]
    jz      short loc_37529
    les     bx, [bp+arg_0]
    cmp     byte ptr es:[bx+5], 1
    jnz     short loc_37529
    cmp     byte_45948, 0
    jz      short loc_37568
    mov     al, byte_45948
    sub     ah, ah
    mov     cx, ax
    mov     ax, [bp+arg_8]
    mov     dx, cx
    mov     cl, 7
    shl     ax, cl
    mov     cx, dx
    sub     dx, dx
    div     cx
    dec     ax
    mov     [bp+arg_8], ax
    jmp     short loc_3756D
    ; align 2
    db 144
loc_37568:
    mov     [bp+arg_8], 0
loc_3756D:
    cmp     [bp+arg_4], 0FFFFh
    jz      short loc_37576
    jmp     loc_37609
loc_37576:
    mov     si, 10h
    mov     di, offset audiochunks_unk2
    mov     cx, [bp+arg_4]
loc_3757F:
    cmp     cx, 0FFFFh
    jnz     short loc_3759D
    mov     ax, [di]
    or      ax, [di+2]
    jnz     short loc_37594
    cmp     byte_45D9A[si], 0
    jnz     short loc_37594
    mov     cx, si
loc_37594:
    add     di, 4Ch ; 'L'
    inc     si
    cmp     si, 17h
    jle     short loc_3757F
loc_3759D:
    mov     [bp+var_4], si
    mov     [bp+arg_4], cx
    cmp     cx, 0FFFFh
    jnz     short loc_37609
    mov     cx, 0FFh
    mov     di, 10h
    mov     si, 86E0h
    mov     dx, [bp+arg_4]
loc_375B4:
    mov     al, [si]
    sub     ah, ah
    cmp     ax, cx
    ja      short loc_375CB
    mov     ax, offset byte_45D9A
    or      ax, ax
    jnz     short loc_375CB
    mov     al, [si]
    sub     ah, ah
    mov     cx, ax
    mov     dx, di
loc_375CB:
    add     si, 4Ch ; 'L'
    inc     di
    cmp     di, 17h
    jl      short loc_375B4
    mov     [bp+var_4], di
    mov     [bp+arg_4], dx
    mov     [bp+var_6], cx
    cmp     dx, 0FFFFh
    jz      short loc_37609
    mov     ax, 4Ch ; 'L'
    imul    dx
    mov     bx, ax
    mov     al, [bp+arg_6]
    cmp     (audiochunks_unk+24h)[bx], al
    ja      short loc_37609
    mov     bx, [bp+arg_4]
    cmp     byte_45D9A[bx], 0
    jz      short loc_37601
    mov     byte_45D9A[bx], 0
loc_37601:
    push    bx
    push    cs
    call    near ptr init_audio_chunk2
    add     sp, 2
loc_37609:
    cmp     [bp+arg_4], 0FFFFh
    jnz     short loc_37612
    jmp     loc_37529
loc_37612:
    les     bx, [bp+arg_0]
    mov     al, es:[bx+6]
    sub     ah, ah
    shl     ax, 1
    shl     ax, 1
    add     ax, 8
    mov     [bp+var_2], ax
    mov     al, [bp+arg_6]
    sub     ah, ah
    push    ax
    push    [bp+arg_8]
    push    [bp+var_2]
    push    es
    push    bx
    push    [bp+arg_4]
    push    [bp+arg_4]
    push    cs
    call    near ptr init_audio_chunk
    add     sp, 0Eh
    mov     ax, [bp+arg_4]
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
check_audio_flag endp
init_audio_chunk2 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    cmp     [bp+arg_0], 10h
    jl      short loc_37694
    cmp     [bp+arg_0], 17h
    jg      short loc_37694
    mov     ax, 4Ch ; 'L'
    imul    [bp+arg_0]
    mov     bx, ax
    sub     ax, ax
    mov     word ptr (audiochunks_unk+2)[bx], ax
    mov     word ptr audiochunks_unk[bx], ax
    push    [bp+arg_0]
    push    [bp+arg_0]
    call    audio_driver_func1E
    add     sp, 4
    sub     ax, ax
    push    ax
    mov     al, byte_45948
    sub     ah, ah
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    [bp+arg_0]
    push    [bp+arg_0]
    push    cs
    call    near ptr init_audio_chunk
    add     sp, 0Eh
loc_37694:
    pop     bp
    retf
init_audio_chunk2 endp
sub_37696 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    cmp     byte_40633, 1
    jz      short loc_376C5
    mov     si, 10h
loc_376A7:
    mov     al, byte_428D6[si]
    sub     ah, ah
    push    ax
    push    si
    call    sub_38C4C
    add     sp, 4
    inc     si
    cmp     si, 18h
    jl      short loc_376A7
    mov     [bp+var_2], si
    mov     byte_40633, 1
loc_376C5:
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_37696 endp
sub_376CA proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    cmp     byte_40633, 0
    jz      short loc_37702
    mov     si, 10h
    mov     di, (offset audiochunks_unk2+28h)
loc_376DF:
    mov     al, [di]
    mov     byte_428D6[si], al
    sub     ax, ax
    push    ax
    push    si
    call    sub_38C4C
    add     sp, 4
    add     di, 4Ch ; 'L'
    inc     si
    cmp     si, 18h
    jl      short loc_376DF
    mov     [bp+var_2], si
    mov     byte_40633, 0
loc_37702:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_376CA endp
sub_37708 proc far

    cmp     byte_40633, 1
    jnz     short loc_37716
    push    cs
    call    near ptr sub_376CA
    sub     ax, ax
    retf
loc_37716:
    push    cs
    call    near ptr sub_37696
    mov     ax, 1
    retf
sub_37708 endp
sub_3771E proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    cmp     byte_40633, 0
    jnz     short loc_3772E
loc_37728:
    mov     ax, 1
    pop     bp
    retf
    ; align 2
    db 144
loc_3772E:
    cmp     [bp+arg_0], 10h
    jl      short loc_37728
    cmp     [bp+arg_0], 17h
    jg      short loc_37728
    mov     ax, 4Ch ; 'L'
    imul    [bp+arg_0]
    mov     bx, ax
    mov     ax, word ptr audiochunks_unk[bx]
    or      ax, word ptr (audiochunks_unk+2)[bx]
    jz      short loc_37728
    sub     ax, ax
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     ax, 4Ch ; 'L'
    mul     word ptr [bp+6]
    mov     bx, ax
    mov     ax, [bp+8]
    mov     dx, [bp+0Ah]
    mov     word ptr (audiochunks_unk+48h)[bx], ax
    mov     word ptr (audiochunks_unk+4Ah)[bx], dx
    pop     bp
    retf
    ; align 2
    db 144
sub_3771E endp
audio_driver_func3F proc far
    var_A = dword ptr -10
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    si
    cmp     byte_40634, 0
    jnz     short loc_377C6
    mov     al, byte_45950
    sub     ah, ah
    mov     [bp+var_2], ax
    or      ax, ax
    jg      short loc_37789
    jmp     loc_37820
loc_37789:
    mov     ax, [bp+arg_0]
    cwd
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
    mov     si, [bp+var_2]
loc_37796:
    mov     word_4063A, 1
    push    si
    push    cs
    call    near ptr sub_37868
    add     sp, 2
    mov     word_4063A, 0
    push    [bp+var_4]
    push    [bp+var_6]
    call    copy_timer_counter
    add     sp, 4
    call    sub_327D7
    sub     si, 2
    or      si, si
    jg      short loc_37796
    jmp     short loc_3781D
loc_377C6:
    mov     si, 64h ; 'd'
    mov     ax, [bp+arg_0]
    cwd
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
loc_377D3:
    mov     word_4063A, 1
    mov     ax, si
    mov     byte_40639, al
    mov     ax, offset unk_40636
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_A+2], dx
    mov     word ptr [bp+var_A], ax
    call    [bp+var_A]
    add     sp, 6
    mov     word_4063A, 0
    push    [bp+var_4]
    push    [bp+var_6]
    call    copy_timer_counter
    add     sp, 4
    call    sub_327D7
    sub     si, 2
    or      si, si
    jg      short loc_377D3
loc_3781D:
    mov     [bp+var_2], si
loc_37820:
    push    cs
    call    near ptr sub_3736A
    cmp     byte_40634, 0
    jz      short loc_37862
    mov     ax, 32h ; '2'
    cwd
    push    dx
    push    ax
    call    copy_timer_counter
    add     sp, 4
    call    sub_327D7
    mov     byte_40639, 64h ; 'd'
    mov     ax, offset unk_40636
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_A+2], dx
    mov     word ptr [bp+var_A], ax
    call    [bp+var_A]
    add     sp, 6
loc_37862:
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
audio_driver_func3F endp
sub_37868 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    sub     si, si
    mov     di, [bp+arg_0]
    jmp     short loc_37883
    ; align 2
    db 144
loc_37878:
    push    di
    push    si
    call    sub_38C4C
    add     sp, 4
    inc     si
loc_37883:
    mov     ax, si
    mov     cl, byte_44290
    sub     ch, ch
    cmp     ax, cx
    jb      short loc_37878
    mov     [bp+var_2], si
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     al, [bp+6]
    mov     byte_45950, al
    push    word ptr [bp+6]
    push    cs
    call    near ptr sub_37868
    add     sp, 2
    pop     bp
    retf
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    mov     bx, [bp+6]
    mov     al, [bx-6A6Ah]
    sub     ah, ah
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     bx, [bp+6]
    mov     al, [bx-6CA6h]
    sub     ah, ah
    pop     bp
    retf
sub_37868 endp
load_audio_driver proc far
    var_C = dword ptr -12
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 0Ch
    push    di
    push    si
    cmp     [bp+arg_4], 473Ah
    jnz     short loc_378DE
    mov     byte_40635, 1
loc_378DE:
    mov     ax, word ptr audiodriverbinary
    or      ax, word ptr audiodriverbinary+2
    jz      short loc_378EE
    push    cs
    call    near ptr audiodrv_atexit
    jmp     short loc_378FE
    ; align 2
    db 144
loc_378EE:
    mov     ax, offset audiodrv_atexit
    mov     dx, seg seg027
    push    dx
    push    ax
    call    add_exit_handler
    add     sp, 4
loc_378FE:
    sub     ax, ax
    mov     word ptr audiodriverbinary+2, ax
    mov     word ptr audiodriverbinary, ax
    push    ds
    pop     es
    mov     di, word ptr [bp+arg_0]
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    dec     cx
    mov     [bp+var_8], cx
    or      cx, cx
    jz      short loc_37932
    mov     dx, word ptr [bp+arg_0]
loc_3791F:
    mov     bx, cx
    mov     si, dx
    cmp     byte ptr [bx+si], 5Ch ; '\'
    jz      short loc_3792F
    cmp     byte ptr [bx+si], 3Ah ; ':'
    jz      short loc_3792F
    loop    loc_3791F
loc_3792F:
    mov     [bp+var_8], cx
loc_37932:
    cmp     [bp+var_8], 0
    jz      short loc_3793B
    inc     [bp+var_8]
loc_3793B:
    mov     si, [bp+var_8]
    add     si, word ptr [bp+arg_0]
    mov     al, [si]
    mov     byte_42DAE, al
    mov     al, [si+1]
    mov     byte_42DAF, al
    mov     byte_42DB0, 0
    mov     ax, offset unk_4063E
    push    ax              ; int
    mov     ax, offset aDrv ; "drv"
    push    ax
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    mov     [bp+var_6], ax
    push    ax
    call    load_res0_1_type
    add     sp, 2
    mov     word ptr audiodriverbinary, ax
    mov     word ptr audiodriverbinary+2, dx
    mov     byte_45950, 7Fh ; ''
    mov     byte_45948, 7Fh ; ''
    or      ax, dx
    jnz     short loc_37988
    jmp     loc_37A52
loc_37988:
    call    audiodriverbinary
    mov     byte_459D2, al
    or      al, al
    jz      short loc_37997
    cmp     al, 0FFh
    jnz     short loc_379A2
loc_37997:
    mov     ax, 2
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    db 144
    db 144
loc_379A2:
    cmp     byte_459D2, 7Fh ; ''
    jbe     short loc_379B8
    mov     byte_459D2, 10h
    mov     byte_40634, 1
    mov     byte_40635, 0
loc_379B8:
    push    cs
    call    near ptr sub_38178
    mov     ax, offset audiodriver_timer
    mov     dx, seg seg028
    push    dx
    push    ax
    call    reg_timer_callback
    add     sp, 4
    cmp     byte_40634, 0
    jz      short loc_37A35
    mov     ax, offset aMt32_plb; "mt32.plb"
    push    ax
    call    load_res0_1_type
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_37A35
    push    dx
    push    [bp+var_4]
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 42h ; 'B'
    mov     word ptr [bp+var_C+2], dx
    mov     word ptr [bp+var_C], ax
    call    [bp+var_C]
    add     sp, 4
    push    [bp+var_2]
    push    [bp+var_4]
    call    mmgr_op_unk2
    add     sp, 4
    mov     byte_40639, 64h ; 'd'
    mov     ax, offset unk_40636
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_C+2], dx
    mov     word ptr [bp+var_C], ax
    call    [bp+var_C]
    add     sp, 6
loc_37A35:
    mov     byte_40630, 0
    mov     byte_40631, 1
    mov     byte_40632, 0
    mov     byte_40633, 1
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_37A52:
    mov     ax, offset aCanTFindDriver; "Can't find driver!\n"
    push    ax
    call    far ptr fatal_error
    add     sp, 2
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
load_audio_driver endp
audiodrv_atexit proc far
    var_4 = dword ptr -4
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 4
    mov     word_4063A, 1
    mov     ax, word ptr audiodriverbinary
    or      ax, word ptr audiodriverbinary+2
    jnz     short loc_37A7C
    jmp     loc_37B09
loc_37A7C:
    mov     ax, 0Ch
    mov     dx, seg seg028
    push    dx
    push    ax
    call    remove_timer_callback
    add     sp, 4
    mov     byte_40631, 0
    mov     byte_40633, 0
    cmp     byte_40634, 0
    jz      short loc_37AC1
    mov     byte_40639, 64h ; 'd'
    mov     ax, 4EC6h
    push    ds
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3Fh ; '?'
    mov     word ptr [bp+var_4+2], dx
    mov     word ptr [bp+var_4], ax
    call    [bp+var_4]
    add     sp, 6
loc_37AC1:
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 6
    mov     word ptr [bp+var_4+2], dx
    mov     word ptr [bp+var_4], ax
    call    [bp+var_4]
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 3
    mov     word ptr [bp+var_4+2], dx
    mov     word ptr [bp+var_4], ax
    call    [bp+var_4]
    push    word ptr audiodriverbinary+2
    push    word ptr audiodriverbinary
    call    mmgr_op_unk2
    add     sp, 4
    sub     ax, ax
    mov     word ptr audiodriverbinary+2, ax
    mov     word ptr audiodriverbinary, ax
    mov     byte_40634, 0
    mov     byte_40635, 0
loc_37B09:
    mov     word_4063A, 0
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
audiodrv_atexit endp
load_sfx_ge proc far
    var_8 = byte ptr -8
    var_7 = byte ptr -7
    var_6 = byte ptr -6
    var_5 = byte ptr -5
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 8
    push    [bp+arg_4]      ; int
    push    word ptr [bp+arg_0+2]
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    load_res0_1_type
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_37B48
loc_37B3E:
    mov     ax, [bp+var_4]
    mov     dx, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
loc_37B48:
    mov     [bp+var_8], 50h ; 'P'
    mov     bx, word ptr [bp+arg_0+2]
    mov     al, [bx]
    mov     [bp+var_7], al
    mov     al, [bx+1]
    mov     [bp+var_6], al
    mov     [bp+var_5], 0
    push    [bp+arg_4]      ; int
    lea     ax, [bp+var_8]
    push    ax
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    decompress_file_nofatal
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jnz     short loc_37B3E
    mov     ax, 4EF0h
    push    ax              ; int
    push    word ptr [bp+arg_0+2]
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    load_res0_1_type
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jnz     short loc_37B3E
    mov     ax, 4EF3h
    push    ax              ; int
    lea     ax, [bp+var_8]
    push    ax
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    decompress_file_nofatal
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_37BD1
    jmp     loc_37B3E
loc_37BD1:
    mov     ax, 4EF6h
    push    ax              ; int
    push    word ptr [bp+arg_0+2]
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    load_res0_1_type
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_37BF9
    jmp     loc_37B3E
loc_37BF9:
    mov     ax, 4EF7h
    push    ax              ; int
    lea     ax, [bp+var_8]
    push    ax
    push    word ptr [bp+arg_0]; char *
    call    sub_39CCE
    add     sp, 6
    push    ax
    call    decompress_file_nofatal
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_37C22
    jmp     loc_37B3E
loc_37C22:
    push    word ptr [bp+arg_0]
    call    load_res0_1_type
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jmp     loc_37B3E
    push    bp
    mov     bp, sp
    mov     ax, [bp+6]
    mov     word_4063C, ax
    pop     bp
    retf
    ; align 2
    db 144
load_sfx_ge endp
load_sfx_file proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    sub     ax, ax
    mov     [bp+var_2], ax
    mov     [bp+var_4], ax
    cmp     byte_40635, 0
    jz      short loc_37C71
    mov     ax, 763Eh
    push    ax              ; int
    mov     ax, 4EF8h
    push    ax
    push    word ptr [bp+arg_0]; char *
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_37C71:
    mov     ax, [bp+var_4]
    or      ax, [bp+var_2]
    jnz     short loc_37C91
    mov     ax, 763Eh
    push    ax              ; int
    mov     ax, 4EFCh
    push    ax
    push    word ptr [bp+arg_0]; char *
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_37C91:
    mov     ax, [bp+var_4]
    or      ax, [bp+var_2]
    jnz     short loc_37CAF
    cmp     word_4063C, 0
    jz      short loc_37CAF
    push    word ptr [bp+arg_0]
    mov     ax, 4F00h
    push    ax
    call    far ptr fatal_error
    add     sp, 4
loc_37CAF:
    mov     ax, [bp+var_4]
    mov     dx, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
load_sfx_file endp
load_song_file proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    sub     ax, ax
    mov     [bp+var_2], ax
    mov     [bp+var_4], ax
    mov     ax, offset byte_42DAE
    push    ax              ; int
    mov     ax, offset aKms ; "kms"
    push    ax
    push    word ptr [bp+arg_0]; char *
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jnz     short loc_37CFA
    cmp     word_4063C, 0
    jz      short loc_37CFA
    push    word ptr [bp+arg_0]
    mov     ax, offset aCannotLoadSongFileS; "cannot load song file %s"
    push    ax
    call    far ptr fatal_error
    add     sp, 4
loc_37CFA:
    mov     ax, [bp+var_4]
    mov     dx, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
load_song_file endp
load_voice_file proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    sub     ax, ax
    mov     [bp+var_2], ax
    mov     [bp+var_4], ax
    cmp     byte_40635, 0
    jz      short loc_37D31
    mov     ax, offset byte_42DAE
    push    ax              ; int
    mov     ax, offset aDvc ; "dvc"
    push    ax
    push    word ptr [bp+arg_0]; char *
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_37D31:
    mov     ax, [bp+var_4]
    or      ax, [bp+var_2]
    jnz     short loc_37D51
    mov     ax, offset byte_42DAE
    push    ax              ; int
    mov     ax, offset aVce ; "vce"
    push    ax
    push    word ptr [bp+arg_0]; char *
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_37D51:
    mov     ax, [bp+var_4]
    or      ax, [bp+var_2]
    jnz     short loc_37D6F
    cmp     word_4063C, 0
    jz      short loc_37D6F
    push    word ptr [bp+arg_0]
    mov     ax, offset aCannotLoadVoiceFileS; "cannot load voice file %s"
    push    ax
    call    far ptr fatal_error
    add     sp, 4
loc_37D6F:
    mov     ax, [bp+var_4]
    mov     dx, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    sub     sp, 4
    mov     ax, 763Eh
    push    ax
    mov     ax, 4F57h
    push    ax
    push    word ptr [bp+6]
    push    cs
    call    near ptr load_sfx_ge
    add     sp, 6
    mov     [bp-4], ax
    mov     [bp-2], dx
    or      ax, dx
    jnz     short loc_37DB2
    cmp     word_4063C, 0
    jz      short loc_37DB2
    push    word ptr [bp+6]
    mov     ax, 4F5Bh
    push    ax
    call    far ptr fatal_error
    add     sp, 4
loc_37DB2:
    mov     ax, [bp-4]
    mov     dx, [bp-2]
    mov     sp, bp
    pop     bp
    retf
load_voice_file endp
init_audio_chunk proc far
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = byte ptr 16
    arg_C = byte ptr 18

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    mov     ax, [bp+arg_0]
    mov     [bp+var_6], ax
    mov     ax, [bp+arg_2]
    cmp     [bp+var_6], ax
    jle     short loc_37DD5
    jmp     loc_37EBA
loc_37DD5:
    mov     ax, 4Ch ; 'L'
    imul    [bp+arg_0]
    add     ax, offset audiochunks_unk
    mov     [bp+var_A], ax
    mov     di, [bp+var_6]
loc_37DE4:
    mov     si, [bp+var_A]
    sub     ax, ax
    mov     [si+4Ah], ax
    mov     [si+48h], ax
    mov     byte ptr [si+22h], 7Fh ; ''
    mov     ax, di
    mov     [si+23h], al
    mov     byte ptr [si+16h], 0Fh
    mov     byte ptr [di-6A6Ah], 0
    mov     byte ptr [di-6CA6h], 0
    mov     byte ptr [si+32h], 0
    mov     byte ptr [si+4], 0
    mov     al, [bp+arg_C]
    mov     [si+24h], al
    mov     byte ptr [si+15h], 0
    sub     ax, ax
    mov     [si+1Ah], ax
    mov     [si+18h], ax
    mov     byte ptr [si+1Ch], 0
    mov     [si+20h], ax
    mov     [si+1Eh], ax
    mov     al, [bp+arg_A]
    mov     [si+28h], al
    mov     byte ptr [si+25h], 0
    mov     word ptr [si+26h], 0
    mov     byte ptr [si+29h], 0
    mov     byte ptr [si+2Ah], 0
    mov     byte ptr [si+2Bh], 0
    mov     byte ptr [si+2Ch], 0
    mov     byte ptr [si+47h], 0FFh
    mov     ax, [bp+arg_4]
    or      ax, [bp+arg_6]
    jz      short loc_37EA0
    mov     ax, [bp+arg_8]
    add     ax, [bp+arg_4]
    mov     dx, [bp+arg_6]
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    push    dx
    push    ax
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    add     ax, 4
    mov     [si+5], ax
    mov     [si+7], dx
    push    [bp+var_2]
    push    [bp+var_4]
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    add     ax, 4
    mov     [si], ax
    mov     [si+2], dx
    add     [bp+arg_8], 5
    mov     ax, [bp+arg_4]
    mov     dx, [bp+arg_6]
    add     ax, 7
    mov     [si+2Eh], ax
    mov     [si+30h], dx
    jmp     short loc_37EA7
loc_37EA0:
    sub     ax, ax
    mov     [si+2], ax
    mov     [si], ax
loc_37EA7:
    add     [bp+var_A], 4Ch ; 'L'
    inc     di
    cmp     di, [bp+arg_2]
    jg      short loc_37EB4
    jmp     loc_37DE4
loc_37EB4:
    mov     [bp+var_6], di
    mov     [bp+var_8], si
loc_37EBA:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
init_audio_chunk endp
load_vce proc far
    var_22 = word ptr -34
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_16 = dword ptr -22
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_6 = byte ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 22h
    push    di
    push    si
    mov     [bp+var_6], 0
    mov     ax, 4F76h
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    init_audio_resource
    add     sp, 6
    mov     word ptr [bp+var_16], ax
    mov     word ptr [bp+var_16+2], dx
    or      ax, dx
    jnz     short loc_37EEB
    jmp     loc_38036
loc_37EEB:
    mov     [bp+var_10], 0
    les     bx, [bp+var_16]
    mov     al, es:[bx+6]
    sub     ah, ah
    or      ax, ax
    jnz     short loc_37F00
    jmp     loc_37F87
loc_37F00:
    mov     [bp+var_1C], ax
    mov     [bp+var_1E], 0
    mov     ax, bx
    add     ax, 7
    mov     [bp+var_22], ax
    mov     [bp+var_20], dx
loc_37F13:
    mov     [bp+var_12], 0
    mov     di, [bp+var_1E]
    sub     cx, cx
    mov     [bp+var_1A], ds
    lds     si, [bp+var_16]
loc_37F23:
    mov     bx, di
    add     bx, cx
    mov     al, [bx+si+7]
    mov     bx, cx
    add     bx, bp
    mov     ss:[bx+var_A], al
    inc     cx
    cmp     cx, 4
    jl      short loc_37F23
    mov     ds, [bp+var_1A]
    mov     [bp+var_12], cx
    mov     ax, [bp+var_22]
    mov     dx, [bp+var_20]
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    lea     ax, [bp+var_A]
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     [bp+var_E], ax
    mov     [bp+var_C], dx
    lea     ax, [bp+var_E]
    push    ss
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    push    cs
    call    near ptr copy_4_bytes
    add     sp, 8
    add     [bp+var_1E], 4
    add     [bp+var_22], 4
    inc     [bp+var_10]
    mov     ax, [bp+var_1C]
    cmp     [bp+var_10], ax
    jb      short loc_37F13
loc_37F87:
    mov     ax, 4F7Bh
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_42A34, ax
    mov     word_42A36, dx
    mov     ax, 4F80h
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_4393E, ax
    mov     word_43940, dx
    mov     ax, 4F85h
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_4394A, ax
    mov     word_4394C, dx
    mov     ax, 4F8Ah
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_454CA, ax
    mov     word_454CC, dx
    mov     ax, 4F8Fh
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_4432C, ax
    mov     word_4432E, dx
    mov     ax, 4F94h
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_43928, ax
    mov     word_4392A, dx
    mov     ax, 4F99h
    push    ax
    push    [bp+arg_6]
    push    [bp+arg_4]
    call    init_audio_resource
    add     sp, 6
    mov     word_44368, ax
    mov     word_4436A, dx
loc_38036:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    sub     sp, 22h
    push    di
    push    si
    mov     ax, [bp+0Ah]
    or      ax, [bp+0Ch]
    jnz     short loc_3804F
    jmp     loc_3814F
loc_3804F:
    mov     ax, [bp+6]
    or      ax, [bp+8]
    jnz     short loc_3805A
    jmp     loc_3814F
loc_3805A:
    les     bx, [bp+6]
    mov     al, es:[bx+4]
    sub     ah, ah
    mov     [bp-22h], ax
    mov     [bp-14h], ax
    mov     word ptr [bp-16h], 0
    or      ax, ax
    jnz     short loc_38075
    jmp     loc_3814F
loc_38075:
    mov     word ptr [bp-20h], 0
loc_3807A:
    mov     word ptr [bp-18h], 0
    mov     di, [bp-20h]
    sub     cx, cx
    mov     word ptr [bp-1Ch], ds
    lds     si, [bp+6]
loc_3808A:
    mov     bx, di
    add     bx, cx
    mov     al, [bx+si+6]
    mov     bx, cx
    add     bx, bp
    mov     ss:[bx-0Ah], al
    inc     cx
    cmp     cx, 4
    jl      short loc_3808A
    mov     ds, word ptr [bp-1Ch]
    mov     [bp-18h], cx
    lea     ax, [bp-0Ah]
    push    ax
    push    word ptr [bp+8]
    push    word ptr [bp+6]
    call    init_audio_resource
    add     sp, 6
    mov     [bp-4], ax
    mov     [bp-2], dx
    les     bx, [bp-4]
    cmp     byte ptr es:[bx+5], 3
    jz      short loc_380CE
    cmp     byte ptr es:[bx+5], 6
    jnz     short loc_38137
loc_380CE:
    cmp     byte ptr es:[bx+0Ah], 0
    jnz     short loc_38137
    mov     word ptr [bp-18h], 0
    mov     ax, bx
    mov     dx, es
    add     ax, 6
    mov     cx, 2
    lea     di, [bp-0Ah]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    repne movsw
    pop     ds
    mov     word ptr [bp-18h], 4
    mov     ax, bx
    add     ax, 6
    mov     [bp-0Eh], ax
    mov     [bp-0Ch], dx
    lea     ax, [bp-0Ah]
    push    ax
    push    word ptr [bp+0Ch]
    push    word ptr [bp+0Ah]
    call    locate_shape_0
    add     sp, 6
    mov     [bp-12h], ax
    mov     [bp-10h], dx
    or      ax, dx
    jz      short loc_38137
    lea     ax, [bp-12h]
    push    ss
    push    ax
    push    word ptr [bp-0Ch]
    push    word ptr [bp-0Eh]
    push    cs
    call    near ptr copy_4_bytes
    add     sp, 8
    les     bx, [bp-4]
    mov     byte ptr es:[bx+0Ah], 0FFh
loc_38137:
    add     word ptr [bp-20h], 4
    inc     word ptr [bp-16h]
    les     bx, [bp+6]
    mov     al, es:[bx+4]
    sub     ah, ah
    cmp     ax, [bp-16h]
    jbe     short loc_3814F
    jmp     loc_3807A
loc_3814F:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
load_vce endp
sub_38156 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 2
    mov     ax, 2Eh ; '.'
    imul    [bp+arg_0]
    add     ax, offset unk_45A26
    mov     [bp+var_2], ax
    mov     bx, ax
    mov     word ptr [bx+0Ch], 1
    mov     word ptr [bx+0Eh], 0
    mov     sp, bp
    pop     bp
    retf
sub_38156 endp
sub_38178 proc far
    var_10 = dword ptr -16
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 10h
    push    di
    push    si
    mov     word_4063A, 1
    sub     ax, ax
    push    ax
    mov     ax, 7Fh ; ''
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    mov     ax, 17h
    push    ax
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr init_audio_chunk
    add     sp, 0Eh
    mov     [bp+var_2], 0
    mov     al, byte_459D2
    sub     ah, ah
    or      ax, ax
    jz      short loc_38221
    mov     di, 0A2B7h
    mov     [bp+var_6], 0A2B6h
    mov     [bp+var_8], 0A2B8h
    mov     [bp+var_A], 0A2C6h
    mov     [bp+var_C], 0A2E2h
    mov     si, [bp+var_2]
loc_381C8:
    push    si
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 1Eh
    mov     word ptr [bp+var_10+2], dx
    mov     word ptr [bp+var_10], ax
    call    [bp+var_10]
    add     sp, 2
    mov     byte ptr [di], 0
    mov     bx, [bp+var_6]
    mov     byte ptr [bx], 0FFh
    mov     bx, [bp+var_8]
    mov     byte ptr [bx], 0
    mov     bx, [bp+var_A]
    sub     ax, ax
    mov     [bx+2], ax
    mov     [bx], ax
    mov     bx, [bp+var_C]
    mov     byte ptr [bx], 0FFh
    add     di, 2Eh ; '.'
    add     [bp+var_6], 2Eh ; '.'
    add     [bp+var_8], 2Eh ; '.'
    add     [bp+var_A], 2Eh ; '.'
    add     [bp+var_C], 2Eh ; '.'
    inc     si
    mov     ax, si
    mov     cl, byte_459D2
    sub     ch, ch
    cmp     ax, cx
    jb      short loc_381C8
    mov     [bp+var_2], si
loc_38221:
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 18h
    mov     word ptr [bp+var_10+2], dx
    mov     word ptr [bp+var_10], ax
    call    [bp+var_10]
    mov     ax, word ptr audiodriverbinary
    mov     dx, word ptr audiodriverbinary+2
    add     ax, 6
    mov     word ptr [bp+var_10+2], dx
    mov     word ptr [bp+var_10], ax
    call    [bp+var_10]
    mov     word_4063A, 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_38178 endp
audio_hdr1_unk proc far
    var_32 = word ptr -50
    var_30 = word ptr -48
    var_2E = word ptr -46
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = byte ptr -36
    var_22 = word ptr -34
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = dword ptr -20
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = byte ptr -12
    var_8 = byte ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 32h
    push    di
    push    si
    mov     [bp+var_8], 0
    mov     ax, [bp+arg_0]
    mov     dx, [bp+arg_2]
    add     ax, 4
    push    dx
    push    ax
    push    cs
    call    near ptr sub_38514
    add     sp, 4
    mov     [bp+var_16], ax
    mov     ax, [bp+arg_0]
    mov     dx, [bp+arg_2]
    add     ax, 6
    mov     [bp+var_28], ax
    mov     [bp+var_26], dx
    mov     ax, [bp+var_16]
    shl     ax, 1
    shl     ax, 1
    add     ax, [bp+var_28]
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
    mov     ax, [bp+var_16]
    mov     cl, 3
    shl     ax, cl
    add     ax, [bp+arg_0]
    add     ax, 6
    mov     [bp+var_22], ax
    mov     [bp+var_20], dx
    mov     [bp+var_2A], 0
    jmp     loc_383C9
    ; align 2
    db 144
loc_382B0:
    inc     word ptr [bp+var_14]
loc_382B3:
    les     bx, [bp+var_14]
    test    byte ptr es:[bx], 80h
    jnz     short loc_382B0
    inc     word ptr [bp+var_14]
    mov     bx, word ptr [bp+var_14]
    mov     al, es:[bx]
    sub     ah, ah
    sub     ax, 0D9h ; 'Ù'
    cmp     ax, 11h
    jbe     short loc_382D2
    jmp     loc_3837E
loc_382D2:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_383A0[bx]
loc_382DA:
    add     word ptr [bp+var_14], 2
    mov     ax, 4
    push    ax
    lea     ax, [bp+var_C]
    push    ss
    push    ax
    push    es
    push    word ptr [bp+var_14]
    call    sub_39C84
    add     sp, 0Ah
    push    [bp+var_26]
    push    [bp+var_28]
    lea     ax, [bp+var_C]
    push    ax
    push    [bp+var_16]
    sub     ax, ax
    push    ax
    call    sub_39B5A
    add     sp, 0Ah
    mov     [bp+var_2E], ax
    cmp     ax, 0FFFFh
    jz      short loc_3834A
    shl     ax, 1
    shl     ax, 1
    add     ax, [bp+var_6]
    mov     dx, [bp+var_4]
    push    dx
    push    ax
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    mov     [bp+var_1E], ax
    mov     [bp+var_1C], dx
    add     ax, [bp+var_22]
    mov     dx, [bp+var_20]
    mov     [bp+var_10], ax
    mov     [bp+var_E], dx
    lea     ax, [bp+var_10]
    push    ss
    push    ax
    push    word ptr [bp+var_14+2]
    push    word ptr [bp+var_14]
    push    cs
    call    near ptr copy_4_bytes
    add     sp, 8
loc_3834A:
    add     word ptr [bp+var_14], 4
loc_3834E:
    mov     ax, [bp+var_1A]
    mov     dx, [bp+var_18]
    cmp     word ptr [bp+var_14], ax
    jnb     short loc_383C6
    jmp     loc_382B3
loc_3835C:
    add     word ptr [bp+var_14], 2
    jmp     short loc_3834E
loc_38362:
    add     word ptr [bp+var_14], 3
    jmp     short loc_3834E
loc_38368:
    inc     word ptr [bp+var_14]
    les     bx, [bp+var_14]
    inc     word ptr [bp+var_14]
    mov     al, es:[bx]
    mov     [bp+var_2], al
    sub     ah, ah
    add     word ptr [bp+var_14], ax
    jmp     short loc_3834E
loc_3837E:
    mov     bx, word ptr [bp+var_14]
    mov     al, es:[bx]
    sub     ah, ah
    cmp     ax, 80h ; '€'
    jb      short loc_3838E
    inc     word ptr [bp+var_14]
loc_3838E:
    inc     word ptr [bp+var_14]
    mov     bx, word ptr [bp+var_14]
    test    byte ptr es:[bx], 80h
    jnz     short loc_3838E
loc_3839A:
    inc     word ptr [bp+var_14]
    jmp     short loc_3834E
    ; align 2
    db 144
off_383A0     dw offset loc_3839A
    dw offset loc_3839A
    dw offset loc_3839A
    dw offset loc_3835C
    dw offset loc_3835C
    dw offset loc_3835C
    dw offset loc_38362
    dw offset loc_3835C
    dw offset loc_3835C
    dw offset loc_3835C
    dw offset loc_3839A
    dw offset loc_3835C
    dw offset loc_38362
    dw offset loc_382DA
    dw offset loc_38368
    dw offset loc_38368
    dw offset loc_3835C
    dw offset loc_3835C
    jmp     short loc_3834E
loc_383C6:
    inc     [bp+var_2A]
loc_383C9:
    mov     ax, [bp+var_16]
    cmp     [bp+var_2A], ax
    jl      short loc_383D4
    jmp     loc_384F4
loc_383D4:
    mov     ax, [bp+var_2A]
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_32], ax
    add     ax, [bp+var_6]
    mov     dx, [bp+var_4]
    push    dx
    push    ax
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    add     ax, [bp+var_22]
    mov     dx, [bp+var_20]
    mov     word ptr [bp+var_14], ax
    mov     word ptr [bp+var_14+2], dx
    lea     ax, [bp+var_14]
    push    ss
    push    ax
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    add     ax, word ptr [bp+var_14]
    mov     dx, word ptr [bp+var_14+2]
    mov     [bp+var_1A], ax
    mov     [bp+var_18], dx
    add     word ptr [bp+var_14], 4
    mov     ax, 4
    push    ax
    mov     ax, 4F9Eh
    push    ds
    push    ax
    mov     ax, [bp+var_32]
    add     ax, [bp+var_28]
    mov     dx, [bp+var_26]
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    call    sub_39AD4
    add     sp, 0Ch
    or      ax, ax
    jnz     short loc_3843B
    jmp     loc_3834E
loc_3843B:
    add     word ptr [bp+var_14], 2
    les     bx, [bp+var_14]
    mov     al, es:[bx]
    mov     [bp+var_24], al
    sub     ah, ah
    shl     ax, 1
    shl     ax, 1
    inc     ax
    add     word ptr [bp+var_14], ax
    mov     bx, word ptr [bp+var_14]
    mov     al, es:[bx]
    mov     [bp+var_24], al
    inc     word ptr [bp+var_14]
    mov     [bp+var_2C], 0
    sub     ah, ah
    or      ax, ax
    jnz     short loc_3846C
    jmp     loc_383C6
loc_3846C:
    mov     [bp+var_30], ax
    mov     di, [bp+var_2C]
loc_38472:
    mov     ax, 4
    push    ax
    lea     ax, [bp+var_C]
    push    ss
    push    ax
    push    word ptr [bp+var_14+2]
    push    word ptr [bp+var_14]
    call    sub_39C84
    add     sp, 0Ah
    push    [bp+var_26]
    push    [bp+var_28]
    lea     ax, [bp+var_C]
    push    ax
    push    [bp+var_16]
    sub     ax, ax
    push    ax
    call    sub_39B5A
    add     sp, 0Ah
    mov     si, ax
    cmp     si, 0FFFFh
    jz      short loc_384DF
    shl     ax, 1
    shl     ax, 1
    add     ax, [bp+var_6]
    mov     dx, [bp+var_4]
    push    dx
    push    ax
    push    cs
    call    near ptr sub_384FA
    add     sp, 4
    mov     [bp+var_1E], ax
    mov     [bp+var_1C], dx
    add     ax, [bp+var_22]
    mov     dx, [bp+var_20]
    mov     [bp+var_10], ax
    mov     [bp+var_E], dx
    lea     ax, [bp+var_10]
    push    ss
    push    ax
    push    word ptr [bp+var_14+2]
    push    word ptr [bp+var_14]
    push    cs
    call    near ptr copy_4_bytes
    add     sp, 8
loc_384DF:
    add     word ptr [bp+var_14], 5
    inc     di
    mov     ax, di
    cmp     ax, [bp+var_30]
    jb      short loc_38472
    mov     [bp+var_2C], di
    mov     [bp+var_2E], si
    jmp     loc_383C6
loc_384F4:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
audio_hdr1_unk endp
sub_384FA proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 4
    les     bx, [bp+arg_0]
    mov     ax, es:[bx]
    mov     dx, es:[bx+2]
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     sp, bp
    pop     bp
    retf
sub_384FA endp
sub_38514 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 2
    les     bx, [bp+arg_0]
    mov     ax, es:[bx]
    mov     [bp+var_2], ax
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_38514 endp
copy_4_bytes proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = dword ptr 10

    push    bp
    mov     bp, sp
    les     bx, [bp+arg_4]
    inc     word ptr [bp+arg_4]
    mov     al, es:[bx]
    les     bx, [bp+arg_0]
    inc     word ptr [bp+arg_0]
    mov     es:[bx], al
    les     bx, [bp+arg_4]
    inc     word ptr [bp+arg_4]
    mov     al, es:[bx]
    les     bx, [bp+arg_0]
    inc     word ptr [bp+arg_0]
    mov     es:[bx], al
    les     bx, [bp+arg_4]
    inc     word ptr [bp+arg_4]
    mov     al, es:[bx]
    les     bx, [bp+arg_0]
    inc     word ptr [bp+arg_0]
    mov     es:[bx], al
    les     bx, [bp+arg_4]
    mov     al, es:[bx]
    les     bx, [bp+arg_0]
    mov     es:[bx], al
    pop     bp
    retf
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    sub     sp, 8
    push    di
    push    si
    mov     al, byte_40633
    sub     ah, ah
    push    ax
    mov     al, byte_40632
    push    ax
    mov     al, byte_40631
    push    ax
    mov     al, byte_40630
    push    ax
    mov     ax, 4FA3h
    push    ax
    call    sub_3219D
    add     sp, 0Ah
    mov     al, byte_45948
    sub     ah, ah
    push    ax
    mov     al, byte_45950
    push    ax
    mov     ax, 4FD5h
    push    ax
    call    sub_3219D
    add     sp, 6
    sub     si, si
    mov     di, 8214h
    mov     word ptr [bp-6], 81FCh
loc_385B6:
    push    word ptr [di+2]
    push    word ptr [di]
    mov     bx, [bp-6]
    push    word ptr [bx+2]
    push    word ptr [bx]
    push    si
    mov     ax, 4FFBh
    push    ax
    call    sub_3219D
    add     sp, 0Ch
    add     di, 4Ch ; 'L'
    add     word ptr [bp-6], 4Ch ; 'L'
    inc     si
    cmp     si, 18h
    jl      short loc_385B6
    mov     [bp-2], si
    mov     ax, 5010h
    push    ax
    call    sub_3219D
    add     sp, 2
    call    flush_stdin
    sub     si, si
    mov     di, 0A2C2h
    mov     word ptr [bp-4], 0A2BEh
    mov     word ptr [bp-8], 0A2B7h
loc_38600:
    push    word ptr [di+2]
    push    word ptr [di]
    mov     bx, [bp-4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    mov     bx, [bp-8]
    mov     al, [bx]
    sub     ah, ah
    push    ax
    push    si
    mov     ax, 501Dh
    push    ax
    call    sub_3219D
    add     sp, 0Eh
    add     di, 2Eh ; '.'
    add     word ptr [bp-4], 2Eh ; '.'
    add     word ptr [bp-8], 2Eh ; '.'
    inc     si
    cmp     si, 10h
    jl      short loc_38600
    mov     [bp-2], si
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
copy_4_bytes endp
seg027 ends
end
