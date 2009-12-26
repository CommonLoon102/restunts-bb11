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
    include seg027.inc
    include seg028.inc
    include seg029.inc
    include seg030.inc
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
seg031 segment byte public 'STUNTSC' use16
    assume cs:seg031
    assume es:nothing, ss:nothing, ds:dseg
    public load_2dshape_0_2
    public sub_39E24
    public sub_39E4C
    public nullsub_1
    public nullsub_2
    public init_video
    public random_wait
    public load_palandcursor
    public get_0
    public alloc_resbytes
    public get_res_ofs_diff_scaled
    public get_res_size_scaled
load_2dshape_0_2 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    push    [bp+arg_0]
    call    j_load_2dshape_0
    add     sp, 2
    pop     bp
    retf
load_2dshape_0_2 endp
sub_39E24 proc far
    var_50 = byte ptr -80
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 50h
    lea     ax, [bp+var_50]
    push    ax              ; char *
    push    [bp+arg_4]      ; int
    push    word ptr [bp+arg_0+2]
    push    word ptr [bp+arg_0]; char *
    call    combine_file_path
    add     sp, 8
    lea     ax, [bp+var_50]
    push    ax
    call    find_filename
    mov     sp, bp
    pop     bp
    retf
sub_39E24 endp
sub_39E4C proc far

    call    find_next
    retf
sub_39E4C endp
nullsub_1 proc far

    retf
    ; align 2
    db 144
nullsub_1 endp
nullsub_2 proc far

    retf
    ; align 2
    db 144
nullsub_2 endp
init_video proc far
    var_1E = word ptr -30
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_12 = byte ptr -18
    var_10 = word ptr -16
    var_E = byte ptr -14
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

    push    bp
    mov     bp, sp
    sub     sp, 1Eh
    push    di
    push    si
    call    sub_30812
    call    kb_shift_checking2
    call    call_readchar_callback
    mov     ax, offset do_mrl_textres
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 7
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_joy_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 0Ah
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_key_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 0Bh
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_mof_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 3200h
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_pau_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 10h
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_pau_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 70h ; 'p'
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_dos_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 11h
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_sonsof_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 13h
    push    ax
    call    reg_callback
    add     sp, 6
    mov     ax, offset do_dos_restext
    mov     dx, seg seg008
    push    dx
    push    ax
    mov     ax, 18h
    push    ax
    call    reg_callback
    add     sp, 6
    mov     word_44AE4, 1
    mov     word_44DC8, 1
    mov     word_454D4, 0FFFFh
    mov     word_461D0, 1
    call    mmgr_alloc_a000
    mov     byte_46436, 0
    mov     byte_459F1, 1
    mov     byte_4645E, 65h ; 'e'
    mov     [bp+var_12], 0
    mov     [bp+var_E], 0
    mov     [bp+var_2], 0
    mov     si, 1
    jmp     short loc_39F63
loc_39F5E:
    mov     [bp+var_12], 1
loc_39F62:
    inc     si
loc_39F63:
    cmp     [bp+arg_0], si
    jg      short loc_39F6B
    jmp     loc_3A07A
loc_39F6B:
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     ax, [bx]
    mov     [bp+var_1E], ax
    mov     bx, ax
    cmp     byte ptr [bx], 2Fh ; '/'
    jnz     short loc_39F62
    mov     al, [bx+1]
    cbw
    cmp     ax, 68h ; 'h'
    jz      short loc_39F5E
    cmp     ax, 6Eh ; 'n'
    jnz     short loc_39F91
    jmp     loc_3A046
loc_39F91:
    cmp     ax, 73h ; 's'
    jnz     short loc_39F62
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     ax, [bx]
    mov     [bp+var_1E], ax
    mov     bx, ax
    mov     al, [bx+2]
    cbw
    mov     bx, ax
    test    byte ptr [bx+382Fh], 1
    jz      short loc_39FC8
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    mov     al, [bx+2]
    cbw
    add     ax, 20h ; ' '
    jmp     short loc_39FD7
    ; align 2
    db 144
loc_39FC8:
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    mov     al, [bx+2]
    cbw
loc_39FD7:
    cmp     ax, 73h ; 's'
    jnz     short loc_3A022
    mov     bx, [bp+var_1E]
    mov     al, [bx+3]
    cbw
    mov     bx, ax
    test    byte ptr [bx+382Fh], 1
    jz      short loc_3A000
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    mov     al, [bx+3]
    cbw
    add     ax, 20h ; ' '
    jmp     short loc_3A00F
loc_3A000:
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    mov     al, [bx+3]
    cbw
loc_3A00F:
    cmp     ax, 62h ; 'b'
    jnz     short loc_3A022
    mov     audiodriverstring, 61h ; 'a'
    mov     audiodriverstring+1, 64h ; 'd'
    jmp     loc_39F62
    ; align 2
    db 144
loc_3A022:
    mov     ax, [bp+arg_2]
    mov     cx, si
    shl     cx, 1
    add     ax, cx
    mov     [bp+var_1E], ax
    mov     bx, ax
    mov     bx, [bx]
    mov     al, [bx+2]
    mov     audiodriverstring, al
    mov     bx, [bp+var_1E]
    mov     bx, [bx]
    mov     al, [bx+3]
    mov     audiodriverstring+1, al
    jmp     loc_39F62
loc_3A046:
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    cmp     byte ptr [bx+2], 73h ; 's'
    jnz     short loc_3A05E
    mov     [bp+var_E], 1
    jmp     loc_39F62
loc_3A05E:
    mov     bx, [bp+arg_2]
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     bx, [bx]
    cmp     byte ptr [bx+2], 64h ; 'd'
    jz      short loc_3A072
    jmp     loc_39F62
loc_3A072:
    mov     [bp+var_2], 1
    jmp     loc_39F62
    ; align 2
    db 144
loc_3A07A:
    call    sub_33816
    cmp     [bp+var_12], 0
    jz      short loc_3A08A
    call    set_bios_mode4
loc_3A08A:
    call    setup_timercallback
    call    sub_28F4E
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    mouse_init
    add     sp, 4
    sub     ax, ax
    push    ax              ; int
    push    ax
    mov     ax, 53A2h
    push    ax              ; char *
    call    load_audio_driver
    add     sp, 6
    or      ax, ax
    jz      short loc_3A0C9
    call    audio_stop_unk
    mov     ax, 1
    push    ax
    call    far ptr sub_2CDEC
    add     sp, 2
loc_3A0C9:
    cmp     [bp+var_E], 0
    jz      short loc_3A0D9
    call    sub_373E8
    call    sub_37708
loc_3A0D9:
    mov     ax, offset do_dea_textres
    mov     dx, seg seg008
    push    dx
    push    ax
    call    set_criterr_handler
    add     sp, 4
    push    cs
    call    near ptr load_palandcursor
    call    set_sprite2_as_1
    mov     ax, 78h ; 'x'
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    call    get_timerdelta2
    sub     si, si
loc_3A10F:
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    inc     si
    cmp     si, 0Fh
    jl      short loc_3A10F
    call    get_timerdelta2
    mov     [bp+var_C], ax
    mov     ax, 3Ch ; '<'
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    sub     si, si
loc_3A140:
    mov     [bp+var_A], 0
    mov     [bp+var_8], 0
    mov     [bp+var_6], 0
    mov     [bp+var_4], 0
    sub     di, di
loc_3A156:
    mov     [bp+var_1A], di
    mov     [bp+var_18], di
    lea     ax, [bp+var_A]
    push    ax
    lea     ax, [bp+var_1A]
    push    ax
    call    sub_2637A
    add     sp, 4
    inc     di
    cmp     di, 190h
    jl      short loc_3A156
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    inc     si
    cmp     si, 0Fh
    jl      short loc_3A140
    call    get_timerdelta2
    mov     [bp+var_10], ax
    sub     si, si
    jmp     short loc_3A1AB
loc_3A190:
    inc     di
loc_3A191:
    cmp     di, 0FFh
    jge     short loc_3A1AA
    lea     ax, [bp+var_A]
    push    ax
    lea     ax, [bp+var_1A]
    push    ax
    call    sub_2637A
    add     sp, 4
    jmp     short loc_3A190
    ; align 2
    db 144
loc_3A1AA:
    inc     si
loc_3A1AB:
    cmp     si, 92h ; '’'
    jge     short loc_3A1B6
    sub     di, di
    jmp     short loc_3A191
    ; align 2
    db 144
loc_3A1B6:
    call    get_timerdelta2
    mov     [bp+var_1C], ax
    mov     ax, [bp+var_C]
    cmp     [bp+var_10], ax
    jle     short loc_3A1CE
    mov     word_44CEA, 0
    jmp     short loc_3A1D4
loc_3A1CE:
    mov     word_44CEA, 1
loc_3A1D4:
    cmp     [bp+var_1C], 4Bh ; 'K'
    jge     short loc_3A1E2
    mov     word_44D4E, 14h
    jmp     short loc_3A1E8
loc_3A1E2:
    mov     word_44D4E, 0Ah
loc_3A1E8:
    cmp     [bp+var_1C], 23h ; '#'
    jge     short loc_3A1F6
    mov     byte_3B8FA, 0
    jmp     short loc_3A22B
    ; align 2
    db 144
loc_3A1F6:
    cmp     [bp+var_1C], 37h ; '7'
    jge     short loc_3A204
    mov     byte_3B8FA, 1
    jmp     short loc_3A22B
    ; align 2
    db 144
loc_3A204:
    cmp     [bp+var_1C], 4Bh ; 'K'
    jge     short loc_3A212
    mov     byte_3B8FA, 2
    jmp     short loc_3A22B
    ; align 2
    db 144
loc_3A212:
    cmp     [bp+var_1C], 64h ; 'd'
    jl      short loc_3A21F
    cmp     word_44CEA, 0
    jnz     short loc_3A226
loc_3A21F:
    mov     byte_3B8FA, 3
    jmp     short loc_3A22B
loc_3A226:
    mov     byte_3B8FA, 4
loc_3A22B:
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     ax, word_44CEA
    mov     word_44984, ax
    push    cs
    call    near ptr random_wait
    sub     ax, ax
    push    ax
    push    word_40B10
    push    word_40B0E
    push    word_40B0C
    push    word_40B0A
    call    sub_24DC2
    add     sp, 0Ah
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
init_video endp
random_wait proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    call    sub_32FFC
    mov     si, ax
    sub     di, di
    jmp     short loc_3A277
    ; align 2
    db 144
loc_3A270:
    cmp     di, 2EE0h
    jge     short loc_3A280
    inc     di
loc_3A277:
    call    sub_32FFC
    cmp     ax, si
    jz      short loc_3A270
loc_3A280:
    cmp     di, 400h
    jnz     short loc_3A29A
    mov     bx, 46Ch
    mov     al, [bx]
    cbw
    mov     di, ax
    jmp     short loc_3A29A
loc_3A290:
    call    _rand
    call    get_kevinrandom
loc_3A29A:
    mov     ax, di
    dec     di
    or      ax, ax
    jnz     short loc_3A290
smart
    and     di, 0FFh
nosmart
    jmp     short loc_3A2B2
    ; align 2
    db 144
loc_3A2A8:
    call    get_kevinrandom
    call    _rand
loc_3A2B2:
    mov     ax, di
    dec     di
    or      ax, ax
    jnz     short loc_3A2A8
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
random_wait endp
load_palandcursor proc far
    var_312 = word ptr -786
    var_310 = dword ptr -784
    var_30C = word ptr -780
    var_30A = word ptr -778
    var_308 = word ptr -776
    var_306 = word ptr -774
    var_304 = dword ptr -772
    var_300 = byte ptr -768
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 312h
    push    si
    mov     ax, 53A7h
    push    ax
    call    load_2dshape1
    add     sp, 2
    mov     [bp+var_30C], ax
    mov     [bp+var_30A], dx
    mov     ax, 53AEh
    push    ax
    push    dx
    push    [bp+var_30C]
    call    locate_shape_1
    add     sp, 6
    mov     word ptr [bp+var_304], ax
    mov     word ptr [bp+var_304+2], dx
    add     word ptr [bp+var_304], 10h
    mov     [bp+var_308], 0
loc_3A300:
    mov     bx, [bp+var_308]
    les     si, [bp+var_304]
    mov     al, es:[bx+si]
    mov     si, bx
    mov     [bp+si+var_300], al
    inc     [bp+var_308]
    cmp     [bp+var_308], 300h
    jl      short loc_3A300
    lea     ax, [bp+var_300]
    push    ax
    mov     ax, 100h
    push    ax
    sub     ax, ax
    push    ax
    call    sub_346A3
    add     sp, 6
    mov     ax, 53B3h
    push    ax
    push    [bp+var_30A]
    push    [bp+var_30C]
    call    locate_shape_1
    add     sp, 6
    mov     word ptr [bp+var_310], ax
    mov     word ptr [bp+var_310+2], dx
    les     bx, [bp+var_310]
    mov     ax, es:[bx]
    imul    word_44DC8
    mov     [bp+var_312], ax
    mov     ax, es:[bx+2]
    mov     [bp+var_306], ax
    push    [bp+var_30A]
    push    [bp+var_30C]
    call    mmgr_free
    add     sp, 4
    mov     ax, 0Fh
    push    ax
    push    [bp+var_306]
    push    [bp+var_312]
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_449C2, ax
    mov     word ptr dword_449C2+2, dx
    mov     ax, 0Fh
    push    ax
    push    [bp+var_306]
    push    [bp+var_312]
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_44364, ax
    mov     word ptr dword_44364+2, dx
    mov     ax, 0Fh
    push    ax
    push    [bp+var_306]
    mov     ax, [bp+var_312]
    add     ax, word_44DC8
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_455C8, ax
    mov     word ptr dword_455C8+2, dx
    mov     ax, 53B8h
    push    ax
    call    load_2dshape1
    add     sp, 2
    mov     [bp+var_30C], ax
    mov     [bp+var_30A], dx
    push    word ptr dword_449C2+2
    push    word ptr dword_449C2
    call    set_sprite1
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    mov     ax, 53BFh
    push    ax
    push    [bp+var_30A]
    push    [bp+var_30C]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D30
    add     sp, 8
    push    word ptr dword_44364+2
    push    word ptr dword_44364
    call    set_sprite1
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    mov     ax, 53C4h
    push    ax
    push    [bp+var_30A]
    push    [bp+var_30C]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D30
    add     sp, 8
    push    [bp+var_30A]
    push    [bp+var_30C]
    call    mmgr_free
    add     sp, 4
    call    sub_28F3C
    pop     si
    mov     sp, bp
    pop     bp
    retf
load_palandcursor endp
get_0 proc far

    sub     ax, ax
    retf
    ; align 2
    db 144
get_0 endp
alloc_resbytes proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    mov     ax, 10h
    cwd
    push    dx
    push    ax
    push    [bp+arg_4]
    push    [bp+arg_2]
    call    __aFldiv
    inc     ax
    push    ax
    push    [bp+arg_0]
    call    mmgr_alloc_pages
    add     sp, 4
    pop     bp
    retf
    ; align 2
    db 144
alloc_resbytes endp
get_res_ofs_diff_scaled proc far

    call    mmgr_get_ofs_diff
    sub     dx, dx
    mov     cl, 4
loc_3A48D:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_3A48D
    retf
get_res_ofs_diff_scaled endp
get_res_size_scaled proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    mmgr_get_chunk_size
    add     sp, 4
    sub     dx, dx
    mov     cl, 4
loc_3A4AB:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_3A4AB
    pop     bp
    retf
get_res_size_scaled endp
seg031 ends
end
