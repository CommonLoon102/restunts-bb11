.model medium
nosmart
    include structs.inc
    include custom.inc
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
seg000 segment byte public 'STUNTSC' use16
    assume cs:seg000
    assume es:nothing, ss:nothing, ds:dseg
    public _main
    public run_intro_looped
    public run_intro
    public load_intro_resources
    public run_menu
    public run_tracks_menu
    public highscore_write_a
    public sub_1168E
    public sub_118D4
    public enter_hiscore
    public highscore_write_b
    public run_car_menu
    public run_opponent_menu
    public run_option_menu
    public off_1314A
    public end_hiscore
    public security_check
    public set_default_car
_main proc far
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    p_argc = word ptr 6
    p_argv = dword ptr 8
    envp = dword ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 12h
    push    di
    push    si
    push    word ptr [bp+p_argv]
    push    [bp+p_argc]
    call    init_video
    add     sp, 4
    call    init_div0
    sub     si, si
loc_1001D:
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_C], ax
    mov     ax, 1Dh
    sub     ax, si
    mov     [bp+var_E], ax
    mov     ax, 1Eh
    imul    [bp+var_E]
    mov     bx, [bp+var_C]
    mov     trackrows[bx], ax
    mov     ax, 1Eh
    imul    si
    mov     bx, [bp+var_C]
    mov     terrainrows[bx], ax
    mov     ax, [bp+var_E]
    mov     cl, 0Ah
    shl     ax, cl
    mov     [bp+var_10], ax
    mov     bx, [bp+var_C]
    mov     trackpos[bx], ax
    mov     bx, [bp+var_C]
    mov     ax, [bp+var_10]
    add     ah, 2
    mov     trackcenterpos[bx], ax
    mov     ax, si
    shl     ax, cl
    mov     [bp+var_12], ax
    mov     bx, [bp+var_C]
    mov     terrainpos[bx], ax
    mov     bx, [bp+var_C]
    mov     ax, [bp+var_12]
    add     ah, 2
    mov     terraincenterpos[bx], ax
    inc     si
    cmp     si, 1Eh
    jl      short loc_1001D
    sub     si, si
loc_10086:
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_12], ax
    mov     bx, ax
    mov     ax, si
    mov     cl, 0Ah
    shl     ax, cl
    mov     trackpos2[bx], ax
    mov     bx, [bp+var_12]
    mov     ax, si
    shl     ax, cl
    add     ah, 2
    mov     trackcenterpos2[bx], ax
    inc     si
    cmp     si, 1Eh
    jl      short loc_10086
    mov     ax, offset aMain; "main"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     word_44CEC, ax
    mov     word_44CEE, dx
    mov     ax, offset aFontdef_fnt; "fontdef.fnt"
    push    ax              ; char *
    sub     ax, ax
    push    ax              ; int
    call    load_resource   ; type 0 = binary file
    add     sp, 4
    mov     word_454C6, ax
    mov     word_454C8, dx
    mov     ax, offset aFontn_fnt; "fontn.fnt"
    push    ax              ; char *
    sub     ax, ax
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_44D22, ax
    mov     word_44D24, dx
    call    set_fontdef
    call    init_polyinfo
    mov     si, 6BF3h       ; bytes to allocate
    mov     ax, si
    cwd
    push    dx
    push    ax
    mov     ax, offset aTrakdata; "trakdata"
    push    ax
    call    alloc_resbytes
    add     sp, 6
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     word ptr trackdata1, ax
    mov     word ptr trackdata1+2, dx
    add     [bp+var_4], 70Ah
    mov     ax, [bp+var_4]
    mov     word ptr trackdata2, ax
    mov     word ptr trackdata2+2, dx
    add     [bp+var_4], 70Ah
    mov     ax, [bp+var_4]
    mov     word ptr trackdata3, ax
    mov     word ptr trackdata3+2, dx
    add     [bp+var_4], 70Ah
    mov     ax, [bp+var_4]
    mov     word ptr trackdata4, ax
    mov     word ptr trackdata4+2, dx
    add     [bp+var_4], 80h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata5, ax
    mov     word ptr trackdata5+2, dx
    add     [bp+var_4], 80h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata6, ax
    mov     word ptr trackdata6+2, dx
    add     [bp+var_4], 80h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata7, ax
    mov     word ptr trackdata7+2, dx
    add     [bp+var_4], 80h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata8, ax
    mov     word ptr trackdata8+2, dx
    add     [bp+var_4], 60h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata9, ax
    mov     word ptr trackdata9+2, dx
    add     [bp+var_4], 180h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata10, ax
    mov     word ptr trackdata10+2, dx
    add     [bp+var_4], 120h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata11, ax
    mov     word ptr trackdata11+2, dx
    add     [bp+var_4], 16Ch
    mov     ax, [bp+var_4]
    mov     word ptr trackdata12, ax
    mov     word ptr trackdata12+2, dx
    add     [bp+var_4], 0F0h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata13, ax
    mov     word ptr trackdata13+2, dx
    add     [bp+var_4], 1Ah
    mov     ax, [bp+var_4]
    mov     word ptr trackdata14, ax
    mov     word ptr trackdata14+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata15, ax
    mov     word ptr trackdata15+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata16, ax
    mov     word ptr trackdata16+2, dx
    add     [bp+var_4], 2EE0h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata17, ax
    mov     word ptr trackdata17+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata18, ax
    mov     word ptr trackdata18+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata19, ax
    mov     word ptr trackdata19+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata20, ax
    mov     word ptr trackdata20+2, dx
    add     [bp+var_4], 7ACh
    mov     ax, [bp+var_4]
    mov     word ptr trackdata21, ax
    mov     word ptr trackdata21+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata22, ax
    mov     word ptr trackdata22+2, dx
    add     [bp+var_4], 385h
    mov     ax, [bp+var_4]
    mov     word ptr trackdata23, ax
    mov     word ptr trackdata23+2, dx
    add     [bp+var_4], 30h
    call    sub_22532
    mov     ax, offset aKevin; "kevin"
    push    ax
    call    init_kevinrandom
    add     sp, 2
    mov     ax, offset aDefault_0; "DEFAULT"
    push    ax
    mov     ax, offset byte_449B1
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    sub     si, si
    mov     ax, 1
    push    ax
    call    input_do_checking
    add     sp, 2
    mov     ax, 1
    push    ax
    call    input_do_checking
    add     sp, 2
    call    sub_28DB6
    mov     byte_3B8F8, 0
    mov     byte_45E1A, 1
    push    cs
    call    near ptr set_default_car
    mov     si, 1
    jmp     _do_intro
    nop
_tracks_menu0:
    sub     ax, ax
_tracks_menu_ax:
    push    ax
    push    cs
    call    near ptr run_tracks_menu
    add     sp, 2
    jmp     _show_menu
    nop
_do_opp_menu:
    call    check_input
    call    show_waiting
    push    cs
    call    near ptr run_opponent_menu
    jmp     _show_menu
    nop
_do_option_menu:
    call    check_input
    call    show_waiting
    push    cs
    call    near ptr run_option_menu
    or      al, al
    jnz     short _goto_game1
    jmp     _show_menu
_goto_game1:
    mov     [bp+var_A], 1
    jmp     short _do_game1
    nop
_do_car_menu:
    call    check_input
    call    show_waiting
    sub     ax, ax
    push    ax
    mov     ax, offset byte_449A9
    push    ax
    mov     ax, offset byte_449A8
    push    ax
    mov     ax, offset player_car_id
    push    ax
    push    cs
    call    near ptr run_car_menu
    add     sp, 8
    jmp     _show_menu
    nop
_do_game0:
    mov     [bp+var_A], 0
_do_game1:
    push    si
    push    di
    mov     di, offset unk_45530
    mov     si, offset player_car_id
    push    ds
    pop     es
    mov     cx, 0Dh
    repne movsw
    pop     di
    pop     si
    sub     si, si
loc_1032F:
    les     bx, trackdata14
    mov     al, es:[bx+si]
    les     bx, trackdata20
    mov     es:[bx+si], al
    inc     si
    cmp     si, 70Ah
    jl      short loc_1032F
    sub     si, si
loc_10346:
    les     bx, trackdata20
    mov     al, byte_3B80C[si]
    mov     es:[bx+si+70Ah], al
    les     bx, trackdata20
    mov     al, byte_3B85E[si]
    mov     es:[bx+si+75Bh], al
    inc     si
    cmp     si, 51h ; 'Q'
    jl      short loc_10346
    cmp     byte_44AE2, 0
    jnz     short _find_tedit
    call    setup_track
    or      al, al
    jz      short _sec_check1
    mov     ax, 1
    jmp     _tracks_menu_ax
_sec_check1:
    call    random_wait
    cmp     byte_45E1A, 0
    jnz     short _init_replay
    call    get_super_random
    cwd
    mov     cx, 14h
    idiv    cx
    mov     ax, dx
    cbw
    push    ax
    push    cs
    call    near ptr security_check
    add     sp, 2
_init_replay:
    call    unload_audio_res
    mov     ax, 5780h       ; size to allocate, 20*1120
    cwd
    push    dx
    push    ax
    mov     ax, offset aCvx ; "cvx"
    push    ax
    call    alloc_resbytes
    add     sp, 6
    mov     word_45A20, ax
    mov     word_45A22, dx
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
    cmp     [bp+var_A], 0
    jnz     short loc_103D1
    jmp     loc_104A6
loc_103D1:
    mov     byte_43966, 0
    jmp     loc_104AC
    nop
_find_tedit:
    mov     ax, offset aTedit__0; "tedit.*"
    push    ax
    call    file_find
    add     sp, 2
    or      ax, ax
    jnz     short _init_replay
_prepare_intro:
    call    unload_audio_res
loc_103EF:
    sub     si, si
_do_intro:
    mov     ax, 2
    push    ax
    call    ensure_file_exists
    add     sp, 2
    or      si, si
    jz      short loc_1042D
    mov     ax, offset track_full_path
    push    ax              ; char *
    mov     ax, offset a_trk; ".trk"
    push    ax              ; int
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, offset byte_3B80C
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    push    word ptr trackdata14+2
    push    word ptr trackdata14
    mov     ax, offset track_full_path
    push    ax
    call    file_read_fatal
    add     sp, 6
loc_1042D:
    mov     byte_44AE2, 0
    push    cs
    call    near ptr run_intro_looped
    mov     di, ax
    cmp     di, 1Bh
    jnz     short _show_menu
    jmp     _ask_dos
_show_menu:
    mov     ax, 2
    push    ax
    call    ensure_file_exists
    add     sp, 2
    cmp     byte_3B8F3, 0
    jnz     short loc_10467
    mov     ax, offset aSlct; "SLCT"
    push    ax
    mov     ax, offset aSkidms_0; "skidms"
    push    ax
    mov     ax, offset aSkidslct; "skidslct"
    push    ax              ; char *
    call    load_audio_res
    add     sp, 6
loc_10467:
    push    cs
    call    near ptr run_menu
    cbw
    cmp     ax, 0FFFFh
    jnz     short loc_10474
    jmp     _prepare_intro
loc_10474:
    or      ax, ax
    jnz     short loc_1047B
    jmp     _do_game0
loc_1047B:
    cmp     ax, 1
    jnz     short loc_10483
    jmp     _do_car_menu
loc_10483:
    cmp     ax, 2
    jnz     short _do_show_menu
    jmp     _do_opp_menu
_do_show_menu:
    cmp     ax, 3
    jnz     short loc_10493
    jmp     _tracks_menu0
loc_10493:
    cmp     ax, 4
    jnz     short loc_1049B
    jmp     _do_option_menu
loc_1049B:
    jmp     short _show_menu
    nop
loc_1049E:
    mov     byte_43966, 4
    jmp     short loc_104AC
    nop
loc_104A6:
    mov     word_449BC, 0
loc_104AC:
    call    show_waiting
    call    run_game
    cmp     byte_44AE2, 0
    jnz     short loc_104D2
    cmp     byte_43966, 0
    jz      short loc_104D2
    push    cs
    call    near ptr end_hiscore
    cbw
    or      ax, ax
    jz      short loc_1049E
    cmp     ax, 1
    jz      short loc_104A6
loc_104D2:
    push    si
    push    di
    mov     di, offset player_car_id
    mov     si, offset unk_45530
    push    ds
    pop     es
    mov     cx, 0Dh
    repne movsw
    pop     di
    pop     si
    sub     si, si
loc_104E5:
    les     bx, trackdata20
    mov     al, es:[bx+si]
    les     bx, trackdata14
    mov     es:[bx+si], al
    inc     si
    cmp     si, 70Ah
    jl      short loc_104E5
    sub     si, si
loc_104FC:
    les     bx, trackdata20
    mov     al, es:[bx+si+70Ah]
    mov     byte_3B80C[si], al
    les     bx, trackdata20
    mov     al, es:[bx+si+75Bh]
    mov     byte_3B85E[si], al
    inc     si
    cmp     si, 51h ; 'Q'
    jl      short loc_104FC
    push    word_45A22
    push    word_45A20
    call    mmgr_op_unk2
    add     sp, 4
    cmp     byte_44AE2, 0
    jnz     short loc_10536
    jmp     _show_menu
loc_10536:
    jmp     loc_103EF
    nop
_ask_dos:
    sub     ax, ax
    push    ax
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, offset aDos ; "dos"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    cmp     ax, 1
    jge     short loc_10575
    jmp     loc_103EF
loc_10575:
    call    sub_28DB6
    call    audio_stop_unk
    call    audiodrv_atexit
    call    sub_30883
    call    kb_shift_checking1
    call    video_set_mode7
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    nop
_main endp
run_intro_looped proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     ax, offset aTitl; "TITL"
    push    ax
    mov     ax, offset aSkidms; "skidms"
    push    ax
    mov     ax, offset aSkidtitl; "skidtitl"
    push    ax              ; char *
    call    load_audio_res
    add     sp, 6
    mov     ax, offset aSdtitl; "sdtitl"
    push    ax              ; char *
    mov     ax, 2
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_45E12, ax
    mov     word_45E14, dx
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    push    cs
    call    near ptr run_intro
    mov     si, ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    push    word_45E14
    push    word_45E12
    call    mmgr_free
    add     sp, 4
    or      si, si
    jnz     short loc_1068E
    call    setup_intro
    cbw
    mov     si, ax
    or      si, si
    jnz     short loc_1068E
    mov     ax, offset aSdcred; "sdcred"
    push    ax              ; char *
    mov     ax, 2
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_45E12, ax
    mov     word_45E14, dx
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    call    sub_28F7C
    sub     ax, ax
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    push    cs
    call    near ptr load_intro_resources
    cbw
    mov     si, ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    push    word_45E14
    push    word_45E12
    call    mmgr_free
    add     sp, 4
loc_1068E:
    call    unload_audio_res
    mov     ax, si
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 4
    db 144
    db 144
run_intro_looped endp
run_intro proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    call    sub_28DB6
    call    sub_28F4E
    call    sub_28D9E
    call    sub_28F7C
    mov     ax, offset aProd; "prod"
    push    ax
    push    word_45E14
    push    word_45E12
    call    locate_shape_1
    add     sp, 6
    mov     bx, ax
    mov     es, dx
    cmp     word ptr es:[bx+0Ah], 0
    jz      short loc_106DE
    mov     waitflag, 0A0h ; ' '
    jmp     short loc_106E4
loc_106DE:
    mov     waitflag, 0B4h ; '´'
loc_106E4:
    mov     ax, offset aProd_0; "prod"
    push    ax
    push    word_45E14
    push    word_45E12
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D4E
    add     sp, 4
    mov     ax, 0FFFFh
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     si, ax
    or      si, si
    jnz     short loc_1077F
    mov     ax, 190h
    push    ax
    call    input_repeat_check
    add     sp, 2
    mov     si, ax
    or      si, si
    jnz     short loc_1077F
    call    sub_28F7C
    mov     waitflag, 0B4h ; '´'
    mov     ax, offset aTitl_0; "titl"
    push    ax
    push    word_45E14
    push    word_45E12
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D4E
    add     sp, 4
    mov     ax, 0FFFFh
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     si, ax
    or      si, si
    jnz     short loc_1077F
    mov     ax, 190h
    push    ax
    call    input_repeat_check
    add     sp, 2
    mov     si, ax
loc_1077F:
    mov     ax, si
    pop     si
    mov     sp, bp
    pop     bp
    retf
run_intro endp
load_intro_resources proc far
    var_46 = word ptr -70
    var_44 = word ptr -68
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_36 = word ptr -54
    var_34 = dword ptr -52
    var_30 = dword ptr -48
    var_C = word ptr -12
    var_A = word ptr -10
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 46h
    push    di
    push    si
    mov     ax, offset aCred; "cred"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_3A], ax
    mov     [bp+var_38], dx
    lea     ax, [bp+var_34]
    push    ax
    mov     ax, offset aArowarrwarw1ar; "arowarrwarw1arw2arw3arw4arw5arw6arw7arw"...
    push    ax
    push    word_45E14
    push    word_45E12
    call    locate_many_resources
    add     sp, 8
    mov     waitflag, 96h ; '–'
    call    sub_28F7C
    les     bx, [bp+var_30]
    mov     ax, es:[bx+8]
    mov     [bp+var_2], ax
    mov     ax, es:[bx+0Ah]
    mov     [bp+var_4], ax
    mov     ax, es:[bx]
    imul    word_44AE4
    mov     [bp+var_3E], ax
    mov     ax, es:[bx+2]
    mov     [bp+var_44], ax
    mov     ax, offset aCre ; "cre"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407DA
    push    word_407D8
    sub     ax, ax
    push    ax
    mov     ax, 78h ; 'x'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGds0; "gds0"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 0Ch
    push    ax
    mov     ax, 3Ch ; '<'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGds1; "gds1"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 14h
    push    ax
    mov     ax, 68h ; 'h'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aDes ; "des"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407DE
    push    word_407DC
    mov     ax, 20h ; ' '
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGdon; "gdon"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 2Ch ; ','
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGkev; "gkev"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 34h ; '4'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGbra; "gbra"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 3Ch ; '<'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGrob; "grob"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 44h ; 'D'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGsta; "gsta"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 4Ch ; 'L'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aMus ; "mus"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407EA
    push    word_407E8
    mov     ax, 5Ch ; '\'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGmsy; "gmsy"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 68h ; 'h'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGkri; "gkri"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 70h ; 'p'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGbri; "gbri"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 78h ; 'x'
    push    ax
    mov     ax, 14h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aPro ; "pro"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407E2
    push    word_407E0
    mov     ax, 20h ; ' '
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGkev_0; "gkev"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 2Ch ; ','
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aOpr ; "opr"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407E2
    push    word_407E0
    mov     ax, 38h ; '8'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGbra_0; "gbra"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 40h ; '@'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGric; "gric"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 48h ; 'H'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aArt ; "art"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407E6
    push    word_407E4
    mov     ax, 54h ; 'T'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGmsm; "gmsm"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 60h ; '`'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGdav; "gdav"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 68h ; 'h'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGnic; "gnic"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 70h ; 'p'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, offset aGkev_1; "gkev"
    push    ax
    push    [bp+var_38]
    push    [bp+var_3A]
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    push    word_407D6
    push    word_407D4
    mov     ax, 78h ; 'x'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    push    [bp+var_38]
    push    [bp+var_3A]
    call    unload_resource
    add     sp, 4
    mov     ax, 0FFFFh
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    call    sub_28F3C
    call    timer_get_delta2
    mov     si, 14Ah
loc_10D77:
    call    timer_get_delta2
    mov     [bp+var_40], ax
    shl     ax, 1
    sub     si, ax
    cmp     [bp+var_2], si
    jle     short loc_10DA0
loc_10D88:
    les     bx, [bp+var_34]
    mov     ax, es:[bx+0Ah]
    mov     [bp+var_4], ax
    mov     [bp+var_36], 0
    sub     si, si
    mov     di, 2
    jmp     loc_10E83
    ; align 2
    db 144
loc_10DA0:
    call    sub_28DB6
    push    [bp+var_4]
    push    si
    push    word ptr [bp+var_30+2]
    push    word ptr [bp+var_30]
    call    sub_33BBC
    add     sp, 8
    sub     ax, ax
    push    ax
    push    [bp+var_44]
    mov     ax, 20h ; ' '
    push    ax
    push    [bp+var_4]
    mov     ax, [bp+var_3E]
    add     ax, si
    push    ax
    call    sub_33578
    add     sp, 0Ah
    call    sub_28D9E
    push    [bp+var_40]
    call    input_do_checking
    add     sp, 2
    mov     [bp+var_46], ax
    or      ax, ax
    jz      short loc_10D77
    jmp     short loc_10D88
    ; align 2
    db 144
loc_10DEC:
    call    sub_28F6A
    mov     ax, 0C8h ; 'È'
    push    ax
    push    [bp+var_4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    mov     bx, di
    shl     bx, 1
    shl     bx, 1
    add     bx, bp
    push    word ptr [bx-32h]
    push    word ptr [bx-34h]
    call    sub_33D4E
    add     sp, 4
    call    sub_28F3C
    mov     ax, 0C8h ; 'È'
    push    ax
    push    [bp+var_4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    call    sub_28D9E
    add     [bp+var_36], 5
    jmp     short loc_10E7D
loc_10E66:
    call    timer_get_delta2
    mov     [bp+var_40], ax
    push    ax
    call    input_do_checking
    add     sp, 2
    mov     [bp+var_46], ax
    add     si, [bp+var_40]
loc_10E7D:
    cmp     [bp+var_36], si
    jg      short loc_10E66
    inc     di
loc_10E83:
    cmp     di, 0Ah
    jge     short loc_10E91
    cmp     [bp+var_46], 0
    jnz     short loc_10E91
    jmp     loc_10DEC
loc_10E91:
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
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_3477E
    add     sp, 4
    call    sub_28F6A
    mov     ax, 0C8h ; 'È'
    push    ax
    push    [bp+var_4]
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    push    word ptr [bp+var_34+2]
    push    word ptr [bp+var_34]
    call    sub_33D4E
    add     sp, 4
    push    [bp+var_A]
    push    [bp+var_C]
    call    sub_33D4E
    add     sp, 4
    sub     ax, ax
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_46], ax
    or      ax, ax
    jnz     short loc_10F2B
    mov     ax, 1F4h
    push    ax
    call    input_repeat_check
    add     sp, 2
    or      ax, ax
    jz      short loc_10F34
loc_10F2B:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_10F34:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
load_intro_resources endp
run_menu proc far
    var_10 = byte ptr -16
    var_E = byte ptr -14
    var_C = byte ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 10h
    mov     [bp+var_6], 0FFh
    mov     [bp+var_10], 0
    mov     [bp+var_C], 0FFh
    call    show_waiting
    mov     waitflag, 0B4h ; '´'
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    mov     ax, 273h
    push    ax              ; char *
    mov     ax, 2
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    call    sub_28F6A
    mov     ax, 27Ah
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D4E
    add     sp, 4
    push    [bp+var_2]
    push    [bp+var_4]
    call    mmgr_free
    add     sp, 4
loc_10FB9:
    mov     al, [bp+var_C]
    cmp     [bp+var_10], al
    jz      short loc_10FEF
    mov     al, [bp+var_10]
    mov     [bp+var_C], al
    call    sub_28F6A
    mov     al, [bp+var_6]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_6], 0FEh ; 'þ'
    call    sub_28F3C
    call    sub_29772
loc_10FEF:
    push    word_407D0
    push    word_407CE
    mov     ax, 2AAh
    push    ax
    mov     ax, 2A0h
    push    ax
    mov     ax, 296h
    push    ax
    mov     ax, 28Ch
    push    ax
    mov     al, [bp+var_10]
    cbw
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_8], ax
    push    ax
    call    input_checking
    add     sp, 2
    mov     [bp+var_A], ax
    mov     ax, 2AAh
    push    ax
    mov     ax, 2A0h
    push    ax
    mov     ax, 296h
    push    ax
    mov     ax, 28Ch
    push    ax
    mov     ax, 5
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_E], al
    cmp     al, 0FFh
    jz      short loc_11049
    mov     [bp+var_10], al
loc_11049:
    mov     ax, [bp+var_8]
    add     word_449F2, ax
    cmp     word_449F2, 1770h
    jle     short loc_11062
    mov     word_449F2, 0
    inc     byte_44AE2
loc_11062:
    cmp     byte_44AE2, 0
    jz      short loc_11072
    mov     [bp+var_10], 0
    mov     [bp+var_A], 0Dh
loc_11072:
    cmp     [bp+var_A], 0
    jnz     short loc_1107B
    jmp     loc_10FB9
loc_1107B:
    mov     ax, [bp+var_A]
    cmp     ax, 0Dh
    jz      short loc_110BA
    cmp     ax, 1Bh
    jz      short loc_110B6
    cmp     ax, 20h ; ' '
    jz      short loc_110BA
    cmp     ax, 4B00h
    jz      short loc_1109A
    cmp     ax, 4D00h
    jz      short loc_110AA
    jmp     loc_10FB9
loc_1109A:
    mov     al, [bp+var_10]
    cbw
    mov     bx, ax
    mov     al, [bx+280h]
loc_110A4:
    mov     [bp+var_10], al
    jmp     loc_10FB9
loc_110AA:
    mov     al, [bp+var_10]
    cbw
    mov     bx, ax
    mov     al, [bx+286h]
    jmp     short loc_110A4
loc_110B6:
    mov     [bp+var_10], 0FFh
loc_110BA:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    mov     al, [bp+var_10]
    cbw
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
run_menu endp
run_tracks_menu proc far
    var_16 = byte ptr -22
    var_14 = byte ptr -20
    var_12 = byte ptr -18
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_9 = byte ptr -9
    var_8 = byte ptr -8
    var_7 = byte ptr -7
    var_6 = byte ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 16h
    push    di
    push    si
    mov     ax, 3
    push    ax
    call    ensure_file_exists
    add     sp, 2
    cmp     [bp+arg_0], 0
    jz      short loc_110ED
    jmp     loc_1156A
loc_110ED:
    mov     [bp+var_6], 0FFh
    mov     [bp+var_16], 0
    mov     [bp+var_12], 0FFh
    call    show_waiting
    mov     waitflag, 9Bh ; '›'
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    les     bx, trackdata14
    mov     al, es:[bx+384h]
    sub     ah, ah
    push    ax
    call    load_skybox
    add     sp, 2
    call    load_game_3dshapes
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    mov     ax, 28h ; '('
    push    ax
    push    ax
    call    sub_322F3
    add     sp, 8
    mov     ax, 0FFFEh
    push    ax
    call    init_game_state
    add     sp, 2
    call    sub_28F6A
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
    call    sub_1CBDC
    call    unload_game_resources
    call    unload_skybox
    call    sub_28F6A
    mov     ax, offset asc_3BA24; "'"
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, offset asc_3BA26; "'"
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 6
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
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr highscore_write_a
    add     sp, 2
    or      al, al
    jz      short loc_111F9
    jmp     loc_112E5
loc_111F9:
    mov     ax, 34h ; '4'
    imul    word_46170
    mov     di, ax
    les     bx, trackdata11
    cmp     word ptr es:[bx+di+32h], 0FFFFh
    jnz     short loc_11210
    jmp     loc_112E5
loc_11210:
    mov     ax, offset unk_3BA28
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 12h
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
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    lea     ax, [bp+var_A]
    push    ax
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_118D4
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    call    sub_34B0C
    add     sp, 4
    mov     ax, 1Eh
    push    ax
    mov     ax, 10h
    push    ax
    mov     al, [bp+var_A]
    cbw
    add     ax, offset byte_463E4
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 1Eh
    push    ax
    mov     ax, 78h ; 'x'
    push    ax
    mov     al, [bp+var_9]
    cbw
    add     ax, offset byte_463E4
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 1Eh
    push    ax
    mov     ax, 0E0h ; 'à'
    push    ax
    mov     al, [bp+var_8]
    cbw
    add     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 1Eh
    push    ax
    mov     ax, 110h
    push    ax
    mov     al, [bp+var_7]
    cbw
    add     ax, offset byte_463E4
    push    ax
    call    sub_33742
    add     sp, 6
    call    set_fontdef
loc_112E5:
    mov     ax, offset aTedit; "tedit"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 18h
    push    ax
    mov     ax, 5Eh ; '^'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, 11h
    push    ax
    mov     ax, offset aBmt ; "bmt"
    push    ax
    push    dx
    push    [bp+var_4]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 18h
    push    ax
    mov     ax, 5Eh ; '^'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, 71h ; 'q'
    push    ax
    mov     ax, offset aBet ; "bet"
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 18h
    push    ax
    mov     ax, 5Eh ; '^'
    push    ax
    mov     ax, 0ACh ; '¬'
    push    ax
    mov     ax, 0D1h ; 'Ñ'
    push    ax
    mov     ax, offset aBmm ; "bmm"
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    push    [bp+var_2]
    push    [bp+var_4]
    call    unload_resource
    add     sp, 4
loc_113B4:
    mov     al, [bp+var_12]
    cmp     [bp+var_16], al
    jz      short loc_113E5
    mov     al, [bp+var_16]
    mov     [bp+var_12], al
    mov     al, [bp+var_6]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_6], 0FEh ; 'þ'
    call    sub_28F3C
    call    sub_29772
loc_113E5:
    push    word_407D0
    push    word_407CE
    mov     ax, offset unk_3BA5E
    push    ax
    mov     ax, offset unk_3BA58
    push    ax
    mov     ax, offset unk_3BA52
    push    ax
    mov     ax, offset unk_3BA4C
    push    ax
    mov     al, [bp+var_16]
    cbw
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_C], ax
    add     word_449F2, ax
    cmp     word_449F2, 1770h
    jle     short loc_11423
    mov     word_449F2, 0
    inc     byte_44AE2
loc_11423:
    push    [bp+var_C]
    call    input_checking
    add     sp, 2
    mov     [bp+var_E], ax
    mov     ax, offset unk_3BA5E
    push    ax
    mov     ax, offset unk_3BA58
    push    ax
    mov     ax, offset unk_3BA52
    push    ax
    mov     ax, offset unk_3BA4C
    push    ax
    mov     ax, 3
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_14], al
    cmp     al, 0FFh
    jz      short loc_11457
    mov     [bp+var_16], al
loc_11457:
    cmp     byte_44AE2, 0
    jz      short loc_11467
    mov     [bp+var_16], 2
    mov     [bp+var_E], 0Dh
loc_11467:
    cmp     [bp+var_E], 0
    jnz     short loc_11470
    jmp     loc_113B4
loc_11470:
    mov     ax, [bp+var_E]
    cmp     ax, 0Dh
    jz      short loc_114BC
    cmp     ax, 1Bh
    jz      short loc_114B8
    cmp     ax, 20h ; ' '
    jz      short loc_114BC
    cmp     ax, 4B00h
    jz      short loc_11490
    cmp     ax, 4D00h
    jz      short loc_114A4
    jmp     loc_113B4
    ; align 2
    db 144
loc_11490:
    cmp     [bp+var_16], 0
    jz      short loc_1149C
    dec     [bp+var_16]
    jmp     loc_113B4
loc_1149C:
    mov     [bp+var_16], 2
    jmp     loc_113B4
    ; align 2
    db 144
loc_114A4:
    cmp     [bp+var_16], 2
    jge     short loc_114B0
    inc     [bp+var_16]
    jmp     loc_113B4
loc_114B0:
    mov     [bp+var_16], 0
    jmp     loc_113B4
    ; align 2
    db 144
loc_114B8:
    mov     [bp+var_16], 0FFh
loc_114BC:
    mov     al, [bp+var_16]
    cbw
    or      ax, ax
    jz      short loc_114E2
    cmp     ax, 1
    jnz     short loc_114CC
    jmp     loc_1155A
loc_114CC:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_114E2:
    mov     ax, offset aTrk ; "trk"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx              ; int
    push    ax              ; int
    mov     ax, offset a_trk_0; ".trk"
    push    ax              ; int
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, 9Ch ; 'œ'
    push    ax              ; char *
    call    sub_27ED4
    add     sp, 0Ah
    cbw
    mov     si, ax
    mov     ax, offset track_full_path
    push    ax              ; char *
    mov     ax, offset a_trk_1; ".trk"
    push    ax              ; int
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, 9Ch ; 'œ'
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    or      si, si
    jz      short loc_11552
    push    word ptr trackdata14+2
    push    word ptr trackdata14
    mov     ax, 95F8h
    push    ax
    call    file_read_fatal
    add     sp, 6
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    jmp     loc_110ED
loc_11552:
    mov     [bp+var_12], 0FFh
    jmp     loc_113B4
    ; align 2
    db 144
loc_1155A:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
loc_1156A:
    call    check_input
    call    show_waiting
    mov     waitflag, 82h ; '‚'
    call    setup_track
    call    sub_2A2BC
    jmp     loc_110ED
    ; align 2
    db 144
run_tracks_menu endp
highscore_write_a proc far
    var_3A = word ptr -58
    var_38 = byte ptr -56
    var_27 = byte ptr -39
    var_F = byte ptr -15
    var_E = byte ptr -14
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 3Ah
    push    di
    push    si
    mov     byte_449CE, 0FFh
    mov     [bp+var_3A], 0
loc_1159A:
    mov     bx, [bp+var_3A]
    shl     bx, 1
    mov     ax, [bp+var_3A]
    mov     word_46170[bx], ax
    inc     [bp+var_3A]
    cmp     [bp+var_3A], 7
    jl      short loc_1159A
    mov     ax, offset track_full_path
    push    ax              ; char *
    mov     ax, offset a_hig_0; ".hig"
    push    ax              ; int
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, 9Ch ; 'œ'
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    cmp     [bp+arg_0], 0
    jnz     short loc_1160A
    mov     byte_3B8FB, 1
    push    word ptr trackdata11+2
    push    word ptr trackdata11
    mov     ax, 95F8h
    push    ax
    mov     ax, 0Ah
    push    ax
    call    sub_29A86
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     byte_3B8FB, 0
    or      ax, dx
    jnz     short loc_11602
loc_115F9:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_11602:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_1160A:
    mov     ax, offset a______________; "...................."
    push    ax
    lea     ax, [bp+var_38]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, offset a_______________________; "......................."
    push    ax
    lea     ax, [bp+var_27]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     [bp+var_F], 0
    mov     ax, offset a______; "../...."
    push    ax
    lea     ax, [bp+var_E]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     [bp+var_6], 0FFFFh
    mov     [bp+var_3A], 0
loc_11648:
    mov     ax, 34h ; '4'
    imul    [bp+var_3A]
    mov     bx, ax
    les     si, trackdata11
    lea     di, [bx+si]
    lea     si, [bp+var_38]
    mov     cx, 1Ah
    repne movsw
    inc     [bp+var_3A]
    cmp     [bp+var_3A], 7
    jl      short loc_11648
    mov     ax, 16Ch
    cwd
    push    dx
    push    ax
    push    word ptr trackdata11+2
    push    word ptr trackdata11
    mov     ax, offset track_full_path
    push    ax
    call    file_write_fatal
    add     sp, 0Ah
    mov     [bp+var_3A], ax
    or      ax, ax
    jnz     short loc_1168B
    jmp     loc_11602
loc_1168B:
    jmp     loc_115F9
highscore_write_a endp
sub_1168E proc far
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_5 = byte ptr -5
    var_4 = byte ptr -4
    var_3 = byte ptr -3
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    call    sub_28F6A
    mov     ax, offset aHs1 ; "hs1"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, offset asc_3BAA2; " '"
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, offset asc_3BAA5; "'"
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 5
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    mov     ax, offset aHs2 ; "hs2"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0Fh
    push    ax
    mov     ax, 10h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29008
    add     sp, 0Ah
    mov     ax, offset aHs3 ; "hs3"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0Fh
    push    ax
    mov     ax, 78h ; 'x'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29008
    add     sp, 0Ah
    mov     ax, offset aHs5 ; "hs5"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0Fh
    push    ax
    mov     ax, 0E0h ; 'à'
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29008
    add     sp, 0Ah
    mov     ax, offset aHs4 ; "hs4"
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 0Fh
    push    ax
    mov     ax, 110h
    push    ax
    mov     ax, offset byte_463E4
    push    ax
    call    sub_29008
    add     sp, 0Ah
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    mov     [bp+var_A], 0
    jmp     loc_118A0
loc_1181A:
    mov     [bp+var_2], 0
loc_1181F:
    mov     al, [bp+var_A]
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 19h
    mov     [bp+var_8], ax
    sub     ax, ax
    push    ax
    push    [bp+var_2]
    call    sub_34B0C
    add     sp, 4
    push    [bp+var_8]
    mov     ax, 10h
    push    ax
    mov     al, [bp+var_6]
    cbw
    add     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    push    [bp+var_8]
    mov     ax, 78h ; 'x'
    push    ax
    mov     al, [bp+var_5]
    cbw
    add     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    push    [bp+var_8]
    mov     ax, 0E0h ; 'à'
    push    ax
    mov     al, [bp+var_4]
    cbw
    add     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    push    [bp+var_8]
    mov     ax, 110h
    push    ax
    mov     al, [bp+var_3]
    cbw
    add     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    inc     [bp+var_A]
loc_118A0:
    cmp     [bp+var_A], 7
    jge     short loc_118CA
    lea     ax, [bp+var_6]
    push    ax
    mov     al, [bp+var_A]
    cbw
    push    ax
    push    cs
    call    near ptr sub_118D4
    add     sp, 4
    mov     al, byte_449CE
    cmp     [bp+var_A], al
    jz      short loc_118C1
    jmp     loc_1181A
loc_118C1:
    mov     ax, word_40802
    mov     [bp+var_2], ax
    jmp     loc_1181F
loc_118CA:
    call    set_fontdef
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_1168E endp
sub_118D4 proc far
    var_4A = byte ptr -74
    var_39 = byte ptr -57
    var_21 = byte ptr -33
    var_20 = byte ptr -32
    var_18 = word ptr -24
    var_16 = byte ptr -22
    var_14 = word ptr -20
    var_12 = byte ptr -18
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4Ah
    push    di
    push    si
    mov     bx, [bp+arg_0]
    shl     bx, 1
    mov     ax, 34h ; '4'
    imul    word_46170[bx]
    add     ax, word ptr trackdata11
    mov     dx, word ptr trackdata11+2
    push    si
    lea     di, [bp+var_4A]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 1Ah
    repne movsw
    pop     ds
    pop     si
    mov     bx, [bp+arg_2]
    mov     byte ptr [bx], 0
    lea     ax, [bp+var_4A]
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    inc     al
    mov     [bp+var_16], al
    mov     bx, [bp+arg_2]
    mov     [bx+1], al
    lea     ax, [bp+var_39]
    push    ax
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    inc     al
    add     [bp+var_16], al
    mov     bx, [bp+arg_2]
    mov     al, [bp+var_16]
    mov     [bx+2], al
    mov     al, [bp+var_16]
    cbw
    mov     si, ax
    add     si, offset byte_463E4
    mov     byte ptr [si], 0
    cmp     [bp+var_21], 1
    jnz     short loc_11981
    mov     ax, offset asc_3BAB7; "("
    push    ax
    push    si              ; char *
    call    _strcat
    add     sp, 4
loc_11981:
    lea     ax, [bp+var_20]
    push    ax
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    cmp     [bp+var_21], 1
    jnz     short loc_119AF
    mov     ax, offset asc_3BAB9; ")"
    push    ax
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcat
    add     sp, 4
loc_119AF:
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    inc     al
    add     [bp+var_16], al
    mov     ax, word_449D0
    mov     [bp+var_14], ax
    mov     word_449D0, 14h
    cmp     [bp+var_18], 0FFFFh
    jz      short loc_119E0
    mov     ax, 1
    push    ax
    push    [bp+var_18]
    jmp     short loc_119E7
    ; align 2
    db 144
loc_119E0:
    mov     ax, 1
    push    ax              ; int
    sub     ax, ax
    push    ax
loc_119E7:
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    mov     bx, [bp+arg_2]
    mov     al, [bp+var_16]
    mov     [bx+3], al
    lea     ax, [bp+var_12]
    push    ax
    mov     al, [bp+var_16]
    cbw
    add     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, [bp+var_14]
    mov     word_449D0, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_118D4 endp
enter_hiscore proc far
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = byte ptr -56
    var_36 = byte ptr -54
    var_25 = byte ptr -37
    var_D = byte ptr -13
    var_C = byte ptr -12
    var_A = byte ptr -10
    var_9 = byte ptr -9
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = byte ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 3Ch
    push    di
    push    si
    cmp     word_449D0, 0Ah
    jnz     short loc_11A2E
    shl     [bp+arg_0], 1
loc_11A2E:
    les     bx, trackdata11
    mov     ax, [bp+arg_0]
    cmp     es:[bx+16Ah], ax
    ja      short loc_11A3F
    jmp     loc_11BAA
loc_11A3F:
    mov     [bp+var_38], 0
    jmp     short loc_11A5D
    ; align 2
    db 144
loc_11A46:
    cmp     [bp+var_38], 7
    jge     short loc_11A71
    mov     al, [bp+var_38]
    cbw
    mov     si, ax
    mov     bx, si
    shl     bx, 1
    mov     word_46170[bx], si
    inc     [bp+var_38]
loc_11A5D:
    mov     al, 34h ; '4'
    imul    [bp+var_38]
    mov     di, ax
    les     bx, trackdata11
    mov     ax, [bp+arg_0]
    cmp     es:[bx+di+32h], ax
    jbe     short loc_11A46
loc_11A71:
    mov     al, [bp+var_38]
    mov     [bp+var_2], al
    mov     byte_449CE, al
    jmp     short loc_11A8D
loc_11A7C:
    mov     al, [bp+var_38]
    cbw
    mov     si, ax
    mov     bx, si
    shl     bx, 1
    mov     word_46172[bx], si
    inc     [bp+var_38]
loc_11A8D:
    cmp     [bp+var_38], 6
    jl      short loc_11A7C
    mov     al, [bp+var_2]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     word_46170[bx], 6
    mov     ax, [bp+arg_0]
    mov     [bp+var_4], ax
    mov     [bp+var_36], 0
    mov     ax, offset unk_44384
    push    ax
    lea     ax, [bp+var_25]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     al, [bp+arg_6]
    mov     [bp+var_D], al
    cmp     opponent_index, 0
    jz      short loc_11AE6
    mov     ax, offset unk_46464
    push    ax
    lea     ax, [bp+var_C]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     [bp+var_A], 2Fh ; '/'
    mov     ax, 81F7h
    push    ax
    lea     ax, [bp+var_9]
    jmp     short loc_11AED
    ; align 2
    db 144
loc_11AE6:
    mov     ax, offset asc_3BABB; " "
    push    ax
    lea     ax, [bp+var_C]
loc_11AED:
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    les     bx, trackdata11
    lea     di, [bx+138h]
    lea     si, [bp+var_36]
    mov     cx, 1Ah
    repne movsw
    call    sub_28F6A
    push    cs
    call    near ptr sub_1168E
    mov     ax, 0FFFFh
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_3C]
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    push    [bp+arg_4]
    push    [bp+arg_2]
    sub     ax, ax
    push    ax
    mov     ax, 3
    push    ax
    call    show_dialog
    add     sp, 12h
    call    check_input
    mov     ax, (offset terraincenterpos+22h)
    cwd
    push    dx              ; int
    push    ax              ; int
    push    [bp+var_3A]     ; int
    push    [bp+var_3C]     ; int
    mov     ax, 10h
    push    ax
    mov     ax, offset unk_459E0
    push    ax              ; char *
    call    sub_290BC
    add     sp, 0Ch
    mov     ax, offset unk_459E0
    push    ax
    lea     ax, [bp+var_36]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    les     bx, trackdata11
    lea     di, [bx+138h]
    lea     si, [bp+var_36]
    mov     cx, 1Ah
    repne movsw
    call    sub_28F6A
    push    cs
    call    near ptr sub_1168E
    mov     ax, 0FFFFh
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    push    cs
    call    near ptr highscore_write_b
loc_11BAA:
    push    cs
    call    near ptr sub_1168E
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
enter_hiscore endp
highscore_write_b proc far
    var_16E = word ptr -366
    var_16C = byte ptr -364
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 16Eh
    push    di
    push    si
    mov     [bp+var_16E], 0
loc_11BC3:
    mov     bx, [bp+var_16E]
    shl     bx, 1
    mov     ax, 34h ; '4'
    imul    word_46170[bx]
    add     ax, word ptr trackdata11
    mov     dx, word ptr trackdata11+2
    mov     cx, ax
    mov     ax, 34h ; '4'
    mov     bx, dx
    imul    [bp+var_16E]
    mov     si, ax
    lea     di, [bp+si+var_16C]
    mov     si, cx
    push    ss
    pop     es
    push    ds
    mov     ds, bx
    mov     cx, 1Ah
    repne movsw
    pop     ds
    inc     [bp+var_16E]
    cmp     [bp+var_16E], 7
    jl      short loc_11BC3
    mov     ax, offset track_full_path
    push    ax              ; char *
    mov     ax, offset a_hig; ".hig"
    push    ax              ; int
    mov     ax, offset byte_449B1
    push    ax
    mov     ax, 9Ch ; 'œ'
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    mov     byte_3B8FB, 1
    mov     ax, 16Ch
    cwd
    push    dx
    push    ax
    lea     ax, [bp+var_16C]
    push    ss
    push    ax
    mov     ax, 95F8h
    push    ax
    call    file_write_fatal
    add     sp, 0Ah
    mov     byte_3B8FB, 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
highscore_write_b endp
run_car_menu proc far
    var_10C = dword ptr -268
    var_108 = byte ptr -264
    var_106 = byte ptr -262
    var_104 = word ptr -260
    var_102 = word ptr -258
    var_100 = word ptr -256
    var_FE = word ptr -254
    var_FC = dword ptr -252
    var_F8 = word ptr -248
    var_F6 = byte ptr -246
    var_F4 = word ptr -244
    var_F2 = byte ptr -242
    var_F0 = byte ptr -240
    var_EE = byte ptr -238
    var_ED = byte ptr -237
    var_EC = byte ptr -236
    var_EB = byte ptr -235
    var_EA = byte ptr -234
    var_4C = word ptr -76
    var_4A = word ptr -74
    var_48 = word ptr -72
    var_46 = byte ptr -70
    var_44 = word ptr -68
    var_42 = dword ptr -66
    var_3E = byte ptr -62
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = byte ptr -56
    var_36 = word ptr -54
    var_34 = word ptr -52
    var_32 = byte ptr -50
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = byte ptr -32
    var_1F = byte ptr -31
    var_1E = word ptr -30
    var_1C = byte ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = byte ptr -16
    var_8 = word ptr -8
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
    sub     sp, 10Ch
    push    di
    push    si
    push    si
    lea     di, [bp+var_32]
    mov     si, offset unk_3BB5E
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     [bp+var_2C], 80F4h
    mov     [bp+var_28], 0
    mov     [bp+var_26], 0
    mov     [bp+var_22], 7530h
    mov     ax, word_44CEA
    mov     word_44984, ax
    or      ax, ax
    jz      short loc_11C82
    lea     ax, [bp+var_10]
    mov     [bp+var_2A], ax
    mov     [bp+var_20], 8
    jmp     short loc_11C86
loc_11C82:
    mov     [bp+var_20], 0
loc_11C86:
    mov     ax, 2
    push    ax
    call    ensure_file_exists
    add     sp, 2
    mov     ax, offset a_res_0; ".res"
    push    ax              ; int
    mov     ax, offset aCar ; "car*"
    push    ax
    sub     ax, ax
    push    ax              ; char *
    call    sub_39E24
    add     sp, 6
    mov     [bp+var_F4], ax
    or      ax, ax
    jnz     short loc_11CB8
    call    nullsub_1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_11CB8:
    mov     bx, [bp+var_F4]
    mov     al, [bx+3]
    mov     [bp+var_EE], al
    mov     al, [bx+4]
    mov     [bp+var_ED], al
    mov     al, [bx+5]
    mov     [bp+var_EC], al
    mov     al, [bx+6]
    mov     [bp+var_EB], al
    mov     [bp+var_EA], 0
    mov     [bp+var_46], 1
loc_11CE1:
    call    sub_39E4C
    mov     [bp+var_F4], ax
    or      ax, ax
    jz      short loc_11D44
    mov     al, [bp+var_46]
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    mov     di, ax
    add     di, bp
    mov     bx, [bp+var_F4]
    mov     al, [bx+3]
    mov     [di-0EEh], al
    mov     bx, [bp+var_F4]
    mov     al, [bx+4]
    mov     [di-0EDh], al
    mov     bx, [bp+var_F4]
    mov     al, [bx+5]
    mov     [di-0ECh], al
    mov     bx, [bp+var_F4]
    mov     al, [bx+6]
    mov     [di-0EBh], al
    mov     al, [bp+var_46]
    cbw
    mov     di, ax
    shl     di, 1
    shl     di, 1
    add     di, ax
    mov     [bp+di+var_EA], 0
    inc     [bp+var_46]
    cmp     [bp+var_46], 20h ; ' '
    jnz     short loc_11CE1
loc_11D44:
    call    nullsub_1
    cmp     [bp+var_46], 1
    jg      short loc_11D52
    jmp     loc_11E10
loc_11D52:
    mov     [bp+var_4A], 0
    jmp     loc_11DFB
loc_11D5A:
    inc     [bp+var_44]
loc_11D5D:
    mov     al, [bp+var_46]
    cbw
    cmp     ax, [bp+var_44]
    ja      short loc_11D69
    jmp     loc_11DF8
loc_11D69:
    mov     di, [bp+var_44]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax
    mov     di, [bp+var_4A]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax              ; char *
    call    _strcmp
    add     sp, 4
    or      ax, ax
    jle     short loc_11D5A
    mov     di, [bp+var_4A]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax
    mov     ax, offset byte_463E4
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     di, [bp+var_44]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax
    mov     di, [bp+var_4A]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, offset byte_463E4
    push    ax
    mov     di, [bp+var_44]
    mov     ax, di
    shl     di, 1
    shl     di, 1
    add     di, ax
    lea     ax, [bp+di+var_EE]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    jmp     loc_11D5A
loc_11DF8:
    inc     [bp+var_4A]
loc_11DFB:
    mov     al, [bp+var_46]
    cbw
    dec     ax
    cmp     ax, [bp+var_4A]
    jle     short loc_11E10
    mov     ax, [bp+var_4A]
    inc     ax
    mov     [bp+var_44], ax
    jmp     loc_11D5D
    ; align 2
    db 144
loc_11E10:
    mov     [bp+var_F0], 0
    mov     [bp+var_F6], 0
    jmp     short loc_11E20
loc_11E1C:
    inc     [bp+var_F6]
loc_11E20:
    mov     al, [bp+var_46]
    cmp     [bp+var_F6], al
    jge     short loc_11E68
    mov     al, [bp+var_F6]
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    mov     di, ax
    add     di, bp
    mov     bx, [bp+arg_0]
    mov     al, [di-0EEh]
    cmp     [bx], al
    jnz     short loc_11E1C
    mov     al, [di-0EDh]
    cmp     [bx+1], al
    jnz     short loc_11E1C
    mov     al, [di-0ECh]
    cmp     [bx+2], al
    jnz     short loc_11E1C
    mov     al, [di-0EBh]
    cmp     [bx+3], al
    jnz     short loc_11E1C
    mov     al, [bp+var_F6]
    mov     [bp+var_F0], al
loc_11E68:
    mov     waitflag, 5Ah ; 'Z'
    mov     [bp+var_3E], 0FFh
    mov     byte_45514, 2Dh ; '-'
    mov     ax, offset aSdcsel; "sdcsel"
    push    ax
    call    load_2dshape1
    add     sp, 2
    mov     [bp+var_36], ax
    mov     [bp+var_34], dx
    cmp     [bp+arg_6], 0
    jnz     short loc_11EA2
    mov     ax, offset aMisc_0; "misc"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     word_455CC, ax
    mov     word_455CE, dx
loc_11EA2:
    cmp     [bp+arg_6], 0
    jnz     short loc_11EAB
    jmp     loc_11F4A
loc_11EAB:
    mov     word_3BB58, 0F0h ; 'ð'
    cmp     byte_46436, 0
    jnz     short loc_11EBB
    jmp     loc_11F50
loc_11EBB:
    mov     bx, [bp+arg_6]
    shl     bx, 1
    shl     bx, 1
    mov     ax, word_45D22[bx]
    mov     dx, word_45D24[bx]
    mov     word ptr [bp+var_10C], ax
    mov     word ptr [bp+var_10C+2], dx
    mov     ax, 0Fh
    push    ax
    les     bx, [bp+var_10C]
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr [bp+var_42], ax
    mov     word ptr [bp+var_42+2], dx
    call    setup_mcgawnd2
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    mov     al, byte ptr [bp+arg_6]
    add     al, 30h ; '0'
    cbw
    push    ax
    push    word_44A02
    push    word_44A00
    call    nullsub_2
    add     sp, 6
    sub     ax, ax
    push    ax
    push    ax
    mov     bx, [bp+arg_6]
    shl     bx, 1
    shl     bx, 1
    push    word_45D24[bx]
    push    word_45D22[bx]
    call    sub_343B0
    add     sp, 8
    sub     ax, ax
    push    ax
    push    ax
    les     bx, [bp+var_42]
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_3475A
    add     sp, 8
    jmp     short loc_11F50
    ; align 2
    db 144
loc_11F4A:
    mov     word_3BB58, 140h
loc_11F50:
    mov     [bp+var_38], 0FFh
    mov     [bp+var_1E], 0
    mov     [bp+var_106], 0
    call    sub_29772
    mov     [bp+var_4C], 0
    mov     [bp+var_F2], 0FFh
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 140h
    push    ax
    mov     ax, 11h
    push    ax
    mov     ax, 24h ; '$'
    push    ax
    call    sub_322F3
    add     sp, 8
    call    timer_get_delta2
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
loc_11FA5:
    mov     al, [bp+var_38]
    cmp     [bp+var_F0], al
    jnz     short loc_11FB1
    jmp     loc_12405
loc_11FB1:
    cmp     al, 0FFh
    jz      short loc_11FC8
    push    [bp+var_2]
    push    [bp+var_4]
    call    unload_resource
    add     sp, 4
    call    sub_20438
loc_11FC8:
    mov     ax, 923Bh
    push    ax
    mov     al, [bp+var_F0]
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    add     ax, bp
    sub     ax, 0EEh ; 'î'
    push    ax
    call    sub_1FF92
    add     sp, 4
    mov     al, [bp+var_F0]
    cbw
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    mov     di, ax
    add     di, bp
    mov     al, [di-0EEh]
    mov     byte ptr aCarcoun+3, al
    mov     al, [di-0EDh]
    mov     byte ptr aCarcoun+4, al
    mov     al, [di-0ECh]
    mov     byte ptr aCarcoun+5, al
    mov     al, [di-0EBh]
    mov     byte ptr aCarcoun+6, al
    mov     ax, offset aCarcoun; "carcoun"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    sub     ax, ax
    push    ax
    push    dx
    push    [bp+var_4]
    call    setup_aero_trackdata
    add     sp, 6
    call    sub_28F7C
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 61h ; 'a'
    push    ax
    mov     ax, 140h
    push    ax
    mov     ax, 67h ; 'g'
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 55h ; 'U'
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 6Dh ; 'm'
    push    ax
    mov     ax, 5
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 55h ; 'U'
    push    ax
    mov     ax, 8Ch ; 'Œ'
    push    ax
    mov     ax, 6Dh ; 'm'
    push    ax
    mov     ax, 52h ; 'R'
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     ax, offset aGrap; "grap"
    push    ax
    push    [bp+var_34]
    push    [bp+var_36]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33D4E
    add     sp, 4
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    push    word_407CA
    sub     ax, ax
    push    ax
    call    sub_34B0C
    add     sp, 4
    mov     ax, 73h ; 's'
    push    ax
    mov     ax, 9
    push    ax
    mov     ax, offset a150 ; "150"
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 87h ; '‡'
    push    ax
    mov     ax, 9
    push    ax
    mov     ax, offset a100 ; "100"
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 9Bh ; '›'
    push    ax
    mov     ax, 9
    push    ax
    mov     ax, offset a50  ; " 50"
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, 9
    push    ax
    mov     ax, offset a0   ; "  0"
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 0B9h ; '¹'
    push    ax
    mov     ax, 1Ah
    push    ax
    mov     ax, offset a02040; "0  20  40"
    push    ax
    call    sub_33742
    add     sp, 6
    call    set_fontdef
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB3C
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    mov     ax, offset aBdo_0; "bdo"
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB3E
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    mov     ax, offset aBnx_0; "bnx"
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB40
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    mov     ax, offset aBla_0; "bla"
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx], 0
    jz      short loc_12226
    mov     ax, offset aBau ; "bau"
    jmp     short loc_12229
loc_12226:
    mov     ax, offset aBma ; "bma"
loc_12229:
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    mov     [bp+var_3C], ax
    mov     [bp+var_3A], dx
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB42
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    push    dx
    push    [bp+var_3C]
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB44
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    mov     ax, offset aBco ; "bco"
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     ax, word_449D0
    mov     [bp+var_12], ax
    mov     word_449D0, 14h
    mov     ax, 0FFFEh
    push    ax
    call    init_game_state
    add     sp, 2
    mov     state.playerstate.car_transmission, 1
    mov     [bp+var_4A], 0
loc_122CE:
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 1
    push    ax
    call    sub_17A78
    add     sp, 8
    mov     ax, state.playerstate.car_trackgrip
    mov     cl, 8
    shr     ax, cl
    mov     [bp+var_8], ax
    mov     ax, 96h ; '–'
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_8]
    cwd
    mov     cl, 6
loc_122FB:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_122FB
    push    dx
    push    ax
    call    __aFuldiv
    sub     ax, 0B5h ; 'µ'
    neg     ax
    mov     [bp+var_48], ax
    cmp     ax, 75h ; 'u'
    jb      short loc_12344
    mov     ax, 26h ; '&'
    imul    [bp+var_4A]
    sub     dx, dx
    mov     cx, 320h
    div     cx
    add     ax, 1Ch
    mov     [bp+var_44], ax
    push    word_40800
    push    [bp+var_48]
    push    ax
    call    sub_35B26
    add     sp, 6
    inc     [bp+var_4A]
    cmp     [bp+var_4A], 320h
    jl      short loc_122CE
loc_12344:
    mov     ax, [bp+var_12]
    mov     word_449D0, ax
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    mov     ax, offset aDes_1; "des"
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    locate_text_res
    add     sp, 6
    mov     word ptr [bp+var_FC], ax
    mov     word ptr [bp+var_FC+2], dx
    mov     [bp+var_44], 0
    mov     [bp+var_48], 74h ; 't'
loc_1237E:
    les     bx, [bp+var_FC]
    inc     word ptr [bp+var_FC]
    mov     al, es:[bx]
    mov     [bp+var_1C], al
    cmp     al, 5Dh ; ']'
    jnz     short loc_123BE
    cmp     [bp+var_44], 0
    jz      short loc_123B1
    mov     bx, [bp+var_44]
    mov     byte_463E4[bx], 0
    push    [bp+var_48]
    mov     ax, 58h ; 'X'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
loc_123B1:
    mov     [bp+var_44], 0
    mov     ax, word_459F2
    add     [bp+var_48], ax
    jmp     short loc_123CB
loc_123BE:
    mov     bx, [bp+var_44]
    inc     [bp+var_44]
    mov     al, [bp+var_1C]
    mov     byte_463E4[bx], al
loc_123CB:
    les     bx, [bp+var_FC]
    cmp     byte ptr es:[bx], 0
    jnz     short loc_1237E
    call    set_fontdef
    call    timer_get_delta2
    mov     [bp+var_F2], 0FFh
    mov     [bp+var_104], 0
    mov     [bp+var_102], 140h
    mov     [bp+var_100], 0
    mov     [bp+var_FE], 0C8h ; 'È'
    mov     [bp+var_108], 0
    mov     [bp+var_6], 3
loc_12405:
    mov     ax, [bp+var_4C]
    add     [bp+var_1E], ax
    mov     al, [bp+var_6]
    cbw
    or      ax, ax
    jz      short loc_12423
    cmp     ax, 1
    jnz     short loc_1241B
    jmp     loc_125FE
loc_1241B:
    cmp     ax, 3
    jz      short loc_12423
    jmp     loc_124DE
loc_12423:
    push    word_3BB62
    push    word_3BB60
    call    sub_2EA4E
    add     sp, 4
    mov     [bp+var_F8], ax
    cmp     word_44984, 0
    jz      short loc_12448
    push    si
    lea     di, [bp+var_10]
    mov     si, offset rect_unk4
    jmp     short loc_1244F
    ; align 2
    db 144
loc_12448:
    push    si
    lea     di, [bp+var_10]
    mov     si, 3B0h
loc_1244F:
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     si
    sub     ax, ax
    push    ax
    mov     ax, 3B0h
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_F8]
    push    ax
    call    select_rect_rotate
    add     sp, 0Ah
    mov     bx, [bp+arg_2]
    mov     al, byte ptr game3dshapes.shape3d_numpaints+0AA8h
    cmp     [bx], al
    jl      short loc_1247A
    mov     byte ptr [bx], 0
loc_1247A:
    mov     ax, [bp+var_1E]
    mov     [bp+var_24], ax
    mov     bx, [bp+arg_2]
    mov     al, [bx]
    mov     [bp+var_1F], al
    lea     ax, [bp+var_32]
    push    ax
    call    transformed_shape_op
    add     sp, 2
    mov     al, [bp+var_38]
    cmp     [bp+var_F0], al
    jnz     short loc_124A6
    mov     word_3BB5C, 5Fh ; '_'
    jmp     short loc_124AC
    ; align 2
    db 144
loc_124A6:
    mov     word_3BB5C, 0C8h ; 'È'
loc_124AC:
    mov     ax, 3E6h
    push    ax
    lea     ax, [bp+var_10]
    push    ax
    call    sub_265EC
    add     sp, 4
    lea     ax, [bp+var_1A]
    push    ax
    lea     ax, [bp+var_104]
    push    ax
    lea     ax, [bp+var_10]
    push    ax
    call    sub_26572
    add     sp, 6
    cmp     [bp+var_6], 3
    jnz     short loc_124DA
    jmp     loc_125FE
loc_124DA:
    mov     [bp+var_6], 1
loc_124DE:
    mov     al, [bp+var_F2]
    cmp     [bp+var_106], al
    jz      short loc_12541
    cmp     al, 0FFh
    jz      short loc_12534
    call    sub_28F3C
    mov     ax, word_3BB4E
    inc     ax
    push    ax
    push    word_3BB3C
    mov     ax, word_3BB32
    add     ax, word_44DC8
    and     ax, word_454D4
    push    ax
    push    word_3BB28
    call    set_sprite1_size
    add     sp, 8
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    call    sub_28D9E
    call    sub_28F3C
loc_12534:
    call    sub_29772
    mov     al, [bp+var_106]
    mov     [bp+var_F2], al
loc_12541:
    call    sub_28F3C
    push    word_407D0
    push    word_407CE
    mov     ax, 3D6h
    push    ax
    mov     ax, 3CCh
    push    ax
    mov     ax, 3C2h
    push    ax
    mov     ax, 3B8h
    push    ax
    mov     al, [bp+var_106]
    cbw
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_4C], ax
    add     word_449F2, ax
    cmp     word_449F2, 2EE0h
    jle     short loc_12585
    mov     word_449F2, 0
    inc     byte_44AE2
loc_12585:
    push    [bp+var_4C]
    call    input_checking
    add     sp, 2
    mov     si, ax
    mov     ax, 3D6h
    push    ax
    mov     ax, 3CCh
    push    ax
    mov     ax, 3C2h
    push    ax
    mov     ax, 3B8h
    push    ax
    mov     ax, 5
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_F6], al
    cmp     al, 0FFh
    jz      short loc_125BA
    mov     [bp+var_106], al
loc_125BA:
    cmp     byte_44AE2, 0
    jz      short loc_125C9
    mov     [bp+var_106], 0
    mov     si, 0Dh
loc_125C9:
    or      si, si
    jnz     short loc_125D0
    jmp     loc_11FA5
loc_125D0:
    mov     ax, si
    cmp     ax, 0Dh
    jnz     short loc_125DA
    jmp     loc_12732
loc_125DA:
    cmp     ax, 1Bh
    jnz     short loc_125E2
    jmp     loc_12732
loc_125E2:
    cmp     ax, 20h ; ' '
    jnz     short loc_125EA
    jmp     loc_12732
loc_125EA:
    cmp     ax, 4800h
    jnz     short loc_125F2
    jmp     loc_12910
loc_125F2:
    cmp     ax, 5000h
    jnz     short loc_125FA
    jmp     loc_12926
loc_125FA:
    jmp     loc_11FA5
    ; align 2
    db 144
loc_125FE:
    mov     [bp+var_6], 0
    mov     [bp+var_108], 1
    call    sub_28F6A
    push    [bp+var_14]
    push    [bp+var_16]
    push    [bp+var_18]
    push    [bp+var_1A]
    call    set_sprite1_size
    add     sp, 8
    mov     ax, 3A3h
    push    ax
    push    [bp+var_34]
    push    [bp+var_36]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33BDA
    add     sp, 4
    call    sub_25FF6
    call    sub_28F6A
    push    [bp+var_14]
    push    [bp+var_16]
    push    [bp+var_18]
    push    [bp+var_1A]
    call    set_sprite1_size
    add     sp, 8
    push    si
    lea     di, [bp+var_104]
    lea     si, [bp+var_10]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    movsw
    pop     si
    cmp     [bp+arg_6], 0
    jz      short loc_126D1
    mov     al, [bp+var_38]
    cmp     [bp+var_F0], al
    jz      short loc_126D1
    call    sub_28F6A
    cmp     byte_46436, 0
    jnz     short loc_126B8
    mov     al, byte ptr [bp+arg_6]
    add     al, 30h ; '0'
    cbw
    push    ax
    push    word_44A02
    push    word_44A00
    call    nullsub_2
    add     sp, 6
    sub     ax, ax
    push    ax
    mov     ax, 0F0h ; 'ð'
    push    ax
    mov     bx, [bp+arg_6]
    shl     bx, 1
    shl     bx, 1
    push    word_45D24[bx]
    push    word_45D22[bx]
    call    sub_343B0
    jmp     short loc_126CE
loc_126B8:
    sub     ax, ax
    push    ax
    mov     ax, 0F0h ; 'ð'
    push    ax
    les     bx, [bp+var_42]
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
loc_126CE:
    add     sp, 8
loc_126D1:
    call    sub_28F3C
    push    [bp+var_14]
    push    [bp+var_16]
    push    [bp+var_18]
    push    [bp+var_1A]
    call    set_sprite1_size
    add     sp, 8
    call    sub_28DB6
    cmp     [bp+var_3E], 0FEh ; 'þ'
    jz      short loc_12710
    mov     al, [bp+var_3E]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_3E], 0FEh ; 'þ'
    jmp     short loc_12723
loc_12710:
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
loc_12723:
    call    sub_28D9E
    mov     al, [bp+var_F0]
    mov     [bp+var_38], al
    jmp     loc_124DE
loc_12732:
    mov     al, [bp+var_106]
    cbw
    or      ax, ax
    jz      short loc_1275E
    cmp     ax, 1
    jnz     short loc_12743
    jmp     loc_12836
loc_12743:
    cmp     ax, 2
    jnz     short loc_1274B
    jmp     loc_1284E
loc_1274B:
    cmp     ax, 3
    jnz     short loc_12753
    jmp     loc_12864
loc_12753:
    cmp     ax, 4
    jnz     short loc_1275B
    jmp     loc_12904
loc_1275B:
    jmp     loc_11FA5
loc_1275E:
    cmp     [bp+var_108], 0
    jnz     short loc_12768
    jmp     loc_11FA5
loc_12768:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    push    [bp+var_2]
    push    [bp+var_4]
    call    unload_resource
    add     sp, 4
    call    sub_20438
    cmp     [bp+arg_6], 0
    jz      short loc_127A6
    cmp     byte_46436, 0
    jz      short loc_127A6
    push    word ptr [bp+var_42+2]
    push    word ptr [bp+var_42]
    call    release_window
    add     sp, 4
loc_127A6:
    cmp     [bp+arg_6], 0
    jnz     short loc_127BC
    push    word_455CE
    push    word_455CC
    call    unload_resource
    add     sp, 4
loc_127BC:
    push    [bp+var_34]
    push    [bp+var_36]
    call    mmgr_free
    add     sp, 4
    call    sub_28DB6
    mov     al, [bp+var_F0]
    cbw
    mov     di, ax
    shl     di, 1
    shl     di, 1
    add     di, ax
    mov     al, [bp+di+var_EE]
    mov     bx, [bp+arg_0]
    mov     [bx], al
    mov     al, [bp+var_F0]
    cbw
    mov     di, ax
    shl     di, 1
    shl     di, 1
    add     di, ax
    mov     al, [bp+di+var_ED]
    mov     bx, [bp+arg_0]
    mov     [bx+1], al
    mov     al, [bp+var_F0]
    cbw
    mov     di, ax
    shl     di, 1
    shl     di, 1
    add     di, ax
    mov     al, [bp+di+var_EC]
    mov     bx, [bp+arg_0]
    mov     [bx+2], al
    mov     al, [bp+var_F0]
    cbw
    mov     di, ax
    shl     di, 1
    shl     di, 1
    add     di, ax
    mov     al, [bp+di+var_EB]
    mov     bx, [bp+arg_0]
    mov     [bx+3], al
    mov     byte_44AE2, 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_12836:
    inc     [bp+var_F0]
    mov     al, [bp+var_46]
    cmp     [bp+var_F0], al
    jz      short loc_12846
    jmp     loc_11FA5
loc_12846:
    mov     [bp+var_F0], 0
    jmp     loc_11FA5
loc_1284E:
    dec     [bp+var_F0]
    js      short loc_12857
    jmp     loc_11FA5
loc_12857:
    mov     al, [bp+var_46]
    dec     al
    mov     [bp+var_F0], al
    jmp     loc_11FA5
    ; align 2
    db 144
loc_12864:
    mov     bx, [bp+arg_4]
    xor     byte ptr [bx], 1
    call    sub_28F6A
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx], 0
    jz      short loc_1287C
    mov     ax, offset aBau_0; "bau"
    jmp     short loc_1287F
loc_1287C:
    mov     ax, offset aBma_0; "bma"
loc_1287F:
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    mov     [bp+var_3C], ax
    mov     [bp+var_3A], dx
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB42
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    push    dx
    push    [bp+var_3C]
    call    sub_292DC
    add     sp, 14h
    call    sub_28F3C
    call    sub_28DB6
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 10h
    push    ax
    mov     ax, 56h ; 'V'
    push    ax
    mov     ax, word_3BB42
    inc     ax
    push    ax
    mov     ax, word_3BB28
    inc     ax
    push    ax
    push    [bp+var_3A]
    push    [bp+var_3C]
    call    sub_292DC
    add     sp, 14h
    call    sub_28D9E
    jmp     loc_11FA5
loc_12904:
    mov     bx, [bp+arg_2]
    inc     byte ptr [bx]
    mov     [bp+var_6], 3
    jmp     loc_11FA5
loc_12910:
    cmp     [bp+var_106], 0
    jz      short loc_1291E
    dec     [bp+var_106]
    jmp     loc_11FA5
loc_1291E:
    mov     [bp+var_106], 4
    jmp     loc_11FA5
loc_12926:
    cmp     [bp+var_106], 4
    jge     short loc_12934
    inc     [bp+var_106]
    jmp     loc_11FA5
loc_12934:
    mov     [bp+var_106], 0
    jmp     loc_11FA5
run_car_menu endp
run_opponent_menu proc far
    var_1E = byte ptr -30
    var_1C = byte ptr -28
    var_1A = dword ptr -26
    var_16 = byte ptr -22
    var_14 = byte ptr -20
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = byte ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 1Eh
    push    si
    mov     ax, 4
    push    ax
    call    ensure_file_exists
    add     sp, 2
    mov     ax, 3F4h
    push    ax
    call    load_res_file
    add     sp, 2
    mov     word_455CC, ax
    mov     word_455CE, dx
    mov     ax, 3F9h
    push    ax              ; char *
    mov     ax, 8
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_44A00, ax
    mov     word_44A02, dx
    mov     ax, 0A5B2h
    push    ax
    mov     ax, 400h
    push    ax
    push    dx
    push    word_44A00
    call    locate_many_resources
    add     sp, 8
    mov     [bp+var_1C], 0
    mov     [bp+var_6], 0
    mov     [bp+var_1E], 0FFh
    mov     [bp+var_4], 0FFh
    call    sub_29772
loc_129A3:
    call    sub_28D9E
loc_129A8:
    mov     al, opponent_index
    cmp     [bp+var_1E], al
    jnz     short loc_129B3
    jmp     loc_12CFB
loc_129B3:
    cmp     [bp+var_1E], 0FFh
    jz      short loc_129DD
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    cmp     [bp+var_6], 0
    jz      short loc_129DD
    push    [bp+var_A]
    push    [bp+var_C]
    call    unload_resource
    add     sp, 4
loc_129DD:
    mov     ax, 4
    push    ax
    call    ensure_file_exists
    add     sp, 2
    cmp     opponent_index, 0
    jz      short loc_12A10
    mov     al, opponent_index
    add     al, 30h ; '0'
    mov     byte ptr aOpp1+3, al
    mov     ax, offset aOpp1; "opp1"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_C], ax
    mov     [bp+var_A], dx
    mov     [bp+var_6], 1
    jmp     short loc_12A14
loc_12A10:
    mov     [bp+var_6], 0
loc_12A14:
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    mov     al, opponent_index
    mov     [bp+var_1E], al
    mov     [bp+var_14], 0FFh
    cmp     byte_46436, 0
    jnz     short loc_12A48
    call    sub_28F6A
    jmp     short loc_12A4D
    ; align 2
    db 144
loc_12A48:
    call    setup_mcgawnd2
loc_12A4D:
    sub     ax, ax
    push    ax
    call    clear_sprite1_color
    add     sp, 2
    mov     ax, 37h ; '7'
    push    ax
    push    word_44A02
    push    word_44A00
    call    nullsub_2
    add     sp, 6
    mov     ax, 41Dh
    push    ax
    push    word_44A02
    push    word_44A00
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_34526
    add     sp, 4
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 12h
    push    ax
    mov     ax, 36h ; '6'
    push    ax
    mov     ax, word_3BBC8
    inc     ax
    push    ax
    mov     ax, 15h
    push    ax
    mov     ax, 422h
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 12h
    push    ax
    mov     ax, 36h ; '6'
    push    ax
    mov     ax, word_3BBC8
    inc     ax
    push    ax
    mov     ax, 4Dh ; 'M'
    push    ax
    mov     ax, 426h
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 12h
    push    ax
    mov     ax, 36h ; '6'
    push    ax
    mov     ax, word_3BBC8
    inc     ax
    push    ax
    mov     ax, 85h ; '…'
    push    ax
    mov     ax, 42Ah
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 12h
    push    ax
    mov     ax, 36h ; '6'
    push    ax
    mov     ax, word_3BBC8
    inc     ax
    push    ax
    mov     ax, 0BDh ; '½'
    push    ax
    mov     ax, 42Eh
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 12h
    push    ax
    mov     ax, 36h ; '6'
    push    ax
    mov     ax, word_3BBC8
    inc     ax
    push    ax
    mov     ax, 0F5h ; 'õ'
    push    ax
    mov     ax, 432h
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     al, opponent_index
    add     al, 30h ; '0'
    cbw
    push    ax
    push    word_44A02
    push    word_44A00
    call    nullsub_2
    add     sp, 6
    mov     al, opponent_index
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx-5A4Ch]
    push    word ptr [bx-5A4Eh]
    call    sub_34526
    add     sp, 4
    mov     ax, 37h ; '7'
    push    ax
    push    word_44A02
    push    word_44A00
    call    nullsub_2
    add     sp, 6
    mov     ax, 436h
    push    ax
    push    word_44A02
    push    word_44A00
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_34526
    add     sp, 4
    cmp     byte_46436, 0
    jz      short loc_12C46
    sub     ax, ax
    push    ax
    push    ax
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_3475A
    add     sp, 8
    call    sub_28F6A
loc_12C46:
    cmp     opponent_index, 0
    jz      short loc_12C5A
    mov     ax, 43Bh
    push    ax
    push    [bp+var_A]
    push    [bp+var_C]
    jmp     short loc_12C66
    ; align 2
    db 144
loc_12C5A:
    mov     ax, 43Fh
    push    ax
    push    word_455CE
    push    word_455CC
loc_12C66:
    call    locate_text_res
    add     sp, 6
    mov     word ptr [bp+var_1A], ax
    mov     word ptr [bp+var_1A+2], dx
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    push    word_407CA
    sub     ax, ax
    push    ax
    call    sub_34B0C
    add     sp, 4
    mov     [bp+var_8], 0
    mov     [bp+var_E], 0
loc_12C9D:
    les     bx, [bp+var_1A]
    inc     word ptr [bp+var_1A]
    mov     al, es:[bx]
    mov     [bp+var_2], al
    cmp     al, 5Dh ; ']'
    jnz     short loc_12CE0
    cmp     [bp+var_8], 0
    jz      short loc_12CD2
    mov     bx, [bp+var_8]
    mov     byte ptr [bx-538Ch], 0
    mov     ax, [bp+var_E]
    add     ax, 21h ; '!'
    push    ax
    mov     ax, 0Ch
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
loc_12CD2:
    mov     [bp+var_8], 0
    mov     ax, word_459F2
    add     [bp+var_E], ax
    jmp     short loc_12CED
    ; align 2
    db 144
loc_12CE0:
    mov     bx, [bp+var_8]
    inc     [bp+var_8]
    mov     al, [bp+var_2]
    mov     [bx-538Ch], al
loc_12CED:
    les     bx, [bp+var_1A]
    cmp     byte ptr es:[bx], 0
    jnz     short loc_12C9D
    call    set_fontdef
loc_12CFB:
    mov     al, [bp+var_14]
    cmp     [bp+var_1C], al
    jz      short loc_12D2C
    mov     al, [bp+var_1C]
    mov     [bp+var_14], al
    mov     al, [bp+var_4]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_4], 0FEh ; 'þ'
    call    timer_get_delta2
    call    sub_29772
loc_12D2C:
    push    word_407D0
    push    word_407CE
    mov     ax, 462h
    push    ax
    mov     ax, 458h
    push    ax
    mov     ax, 44Eh
    push    ax
    mov     ax, 444h
    push    ax
    mov     al, [bp+var_1C]
    cbw
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_10], ax
    push    ax
    call    input_checking
    add     sp, 2
    mov     si, ax
    mov     ax, 462h
    push    ax
    mov     ax, 458h
    push    ax
    mov     ax, 44Eh
    push    ax
    mov     ax, 444h
    push    ax
    mov     ax, 5
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_16], al
    cmp     al, 0FFh
    jz      short loc_12D93
    cmp     opponent_index, 0
    jnz     short loc_12D8D
    cmp     al, 3
    jz      short loc_12D93
loc_12D8D:
    mov     al, [bp+var_16]
    mov     [bp+var_1C], al
loc_12D93:
    or      si, si
    jnz     short loc_12D9A
    jmp     loc_129A8
loc_12D9A:
    mov     ax, si
    cmp     ax, 0Dh
    jz      short loc_12DBE
    cmp     ax, 1Bh
    jz      short loc_12DBE
    cmp     ax, 20h ; ' '
    jz      short loc_12DBE
    cmp     ax, 4B00h
    jnz     short loc_12DB3
    jmp     loc_12EF6
loc_12DB3:
    cmp     ax, 4D00h
    jnz     short loc_12DBB
    jmp     loc_12F20
loc_12DBB:
    jmp     loc_129A8
loc_12DBE:
    mov     al, [bp+var_1C]
    cbw
    or      ax, ax
    jz      short loc_12DE0
    cmp     ax, 1
    jz      short loc_12DF6
    cmp     ax, 2
    jz      short loc_12E0C
    cmp     ax, 3
    jz      short loc_12E14
    cmp     ax, 4
    jnz     short loc_12DDD
    jmp     loc_12E6A
loc_12DDD:
    jmp     loc_129A8
loc_12DE0:
    dec     opponent_index
    cmp     opponent_index, 1
    jl      short loc_12DEE
    jmp     loc_129A8
loc_12DEE:
    mov     opponent_index, 6
    jmp     loc_129A8
loc_12DF6:
    inc     opponent_index
    cmp     opponent_index, 7
    jz      short loc_12E04
    jmp     loc_129A8
loc_12E04:
    mov     opponent_index, 1
    jmp     loc_129A8
loc_12E0C:
    mov     opponent_index, 0
    jmp     loc_129A8
loc_12E14:
    cmp     opponent_index, 0
    jnz     short loc_12E1E
    jmp     loc_129A8
loc_12E1E:
    call    check_input
    call    sub_28DB6
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    push    [bp+var_A]
    push    [bp+var_C]
    call    unload_resource
    add     sp, 4
    call    show_waiting
    mov     al, opponent_index
    cbw
    push    ax
    mov     ax, 9240h
    push    ax
    mov     ax, 923Fh
    push    ax
    mov     ax, 923Bh
    push    ax
    push    cs
    call    near ptr run_car_menu
    add     sp, 8
    mov     [bp+var_1E], 0FFh
    jmp     loc_129A3
loc_12E6A:
    cmp     opponent_index, 0
    jz      short loc_12EA2
    cmp     opponent_car_id, 0FFh
    jnz     short loc_12EA7
    mov     al, player_car_id
    mov     opponent_car_id, al
    mov     al, byte_449A5
    mov     opponent_car_id+1, al
    mov     al, byte_449A6
    mov     opponent_car_id+2, al
    mov     al, byte_449A7
    mov     opponent_car_id+3, al
    mov     al, byte_449A8
smart
    and     al, 1
nosmart
    xor     al, 1
    mov     byte_449AF, al
    mov     byte_449B0, 0
    jmp     short loc_12EA7
    ; align 2
    db 144
loc_12EA2:
    mov     opponent_car_id, 0FFh
loc_12EA7:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    cmp     [bp+var_6], 0
    jz      short loc_12ECB
    push    [bp+var_A]
    push    [bp+var_C]
    call    unload_resource
    add     sp, 4
loc_12ECB:
    push    word_44A02
    push    word_44A00
    call    mmgr_free
    add     sp, 4
    push    word_455CE
    push    word_455CC
    call    unload_resource
    add     sp, 4
    call    sub_28DB6
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_12EF6:
    cmp     [bp+var_1C], 0
    jz      short loc_12F02
    dec     [bp+var_1C]
    jmp     short loc_12F06
    ; align 2
    db 144
loc_12F02:
    mov     [bp+var_1C], 4
loc_12F06:
    cmp     opponent_index, 0
    jz      short loc_12F10
    jmp     loc_129A8
loc_12F10:
    cmp     [bp+var_1C], 3
    jz      short loc_12F19
    jmp     loc_129A8
loc_12F19:
    dec     [bp+var_1C]
    jmp     loc_129A8
    ; align 2
    db 144
loc_12F20:
    cmp     [bp+var_1C], 4
    jge     short loc_12F2C
    inc     [bp+var_1C]
    jmp     short loc_12F30
    ; align 2
    db 144
loc_12F2C:
    mov     [bp+var_1C], 0
loc_12F30:
    cmp     opponent_index, 0
    jz      short loc_12F3A
    jmp     loc_129A8
loc_12F3A:
    cmp     [bp+var_1C], 3
    jz      short loc_12F43
    jmp     loc_129A8
loc_12F43:
    inc     [bp+var_1C]
    jmp     loc_129A8
    ; align 2
    db 144
run_opponent_menu endp
run_option_menu proc far
    var_6 = byte ptr -6
    var_4 = byte ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 6
    mov     ax, 46Ch
    push    ax
    call    load_res_file
    add     sp, 2
    mov     word_455CC, ax
    mov     word_455CE, dx
    call    sub_28F3C
    push    word_407FA
    call    clear_sprite1_color
    add     sp, 2
    mov     ax, 471h
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 6
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     ax, 476h
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_shape_alt
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 10h
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28F98
    add     sp, 0Ah
    mov     [bp+var_4], 1
loc_13004:
    sub     ax, ax
    push    ax
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 47Bh
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     [bp+var_2], al
    cbw
    sub     ax, 0FFFFh
    cmp     ax, 7
    jbe     short loc_13046
    jmp     loc_1315A
loc_13046:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_1314A[bx]
loc_1304E:
    cmp     byte_3B8F2, 0
    jz      short loc_1305C
    mov     [bp+var_6], 2
    jmp     short loc_1306E
    ; align 2
    db 144
loc_1305C:
    cmp     byte_3FE00, 0
    jz      short loc_1306A
    mov     [bp+var_6], 1
    jmp     short loc_1306E
    ; align 2
    db 144
loc_1306A:
    mov     [bp+var_6], 0
loc_1306E:
    mov     al, [bp+var_6]
    cbw
    push    ax
    sub     ax, ax
    push    ax
    push    word_40800
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 47Fh
    push    ax
    push    word_455CE
    push    word_455CC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     [bp+var_2], al
    cbw
    or      ax, ax
    jz      short loc_130BA
    cmp     ax, 1
    jz      short loc_130C2
    cmp     ax, 2
    jz      short loc_130CA
    jmp     loc_1315A
loc_130BA:
    call    do_key_restext
    jmp     loc_1315A
loc_130C2:
    call    do_joy_restext
    jmp     loc_1315A
loc_130CA:
    call    sub_29DF4
    jmp     loc_1315A
loc_130D2:
    call    do_mof_restext
    jmp     loc_1315A
loc_130DA:
    call    do_sonsof_restext
    jmp     short loc_1315A
    ; align 2
    db 144
loc_130E2:
    mov     ax, 483h
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx              ; int
    push    ax              ; int
    mov     ax, 487h
    push    ax              ; int
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    call    sub_27ED4
    add     sp, 0Ah
    mov     [bp+var_6], al
    or      al, al
    jz      short loc_1315A
    mov     waitflag, 96h ; '–'
    call    show_waiting
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    call    sub_22C92
    add     sp, 4
    mov     [bp+var_4], 1
    jmp     short loc_13163
loc_13134:
    call    do_mrl_textres
    jmp     short loc_1315A
    ; align 2
    db 144
loc_1313C:
    call    do_dos_restext
    jmp     short loc_1315A
    ; align 2
    db 144
loc_13144:
    mov     [bp+var_4], 0
    jmp     short loc_1315A
off_1314A     dw offset loc_13144
    dw offset loc_1304E
    dw offset loc_130D2
    dw offset loc_130DA
    dw offset loc_130E2
    dw offset loc_13134
    dw offset loc_1313C
    dw offset loc_13144
loc_1315A:
    cmp     [bp+var_4], 0
    jz      short loc_13163
    jmp     loc_13004
loc_13163:
    push    word_455CE
    push    word_455CC
    call    unload_resource
    mov     al, [bp+var_4]
    cbw
    mov     sp, bp
    pop     bp
    retf
run_option_menu endp
end_hiscore proc far
    var_9E = word ptr -158
    var_9C = word ptr -156
    var_9A = byte ptr -154
    var_98 = byte ptr -152
    var_92 = byte ptr -146
    var_90 = word ptr -144
    var_8E = byte ptr -142
    var_8C = word ptr -140
    var_8A = word ptr -138
    var_88 = word ptr -136
    var_86 = dword ptr -134
    var_82 = byte ptr -130
    var_80 = word ptr -128
    var_7E = word ptr -126
    var_7C = word ptr -124
    var_7A = word ptr -122
    var_78 = byte ptr -120
    var_72 = word ptr -114
    var_70 = word ptr -112
    var_6E = byte ptr -110
    var_6C = byte ptr -108
    var_6A = byte ptr -106
    var_68 = word ptr -104
    var_66 = word ptr -102
    var_64 = byte ptr -100
    var_62 = byte ptr -98
    var_5C = word ptr -92
    var_5A = word ptr -90
    var_58 = word ptr -88
    var_56 = dword ptr -86
    var_52 = byte ptr -82
    var_50 = word ptr -80
    var_4E = word ptr -78
    var_4C = word ptr -76
    var_4A = dword ptr -74
    var_46 = dword ptr -70
    var_42 = word ptr -66
    var_40 = word ptr -64
    var_3E = byte ptr -62
    var_3C = byte ptr -60
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = byte ptr -24
    var_16 = byte ptr -22
    var_14 = byte ptr -20
    var_12 = byte ptr -18
    var_11 = byte ptr -17
    var_10 = byte ptr -16
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 9Eh
    push    di
    push    si
    mov     ax, 4
    push    ax
    call    ensure_file_exists
    add     sp, 2
    mov     ax, 48Ch
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_4E], ax
    mov     [bp+var_4C], dx
    cmp     opponent_index, 0
    jz      short loc_131C0
    mov     al, opponent_index
    add     al, 30h ; '0'
    mov     byte ptr aOpp1+3, al
    mov     ax, offset aOpp1; "opp1"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_68], ax
    mov     [bp+var_66], dx
loc_131C0:
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr wndsprite, ax
    mov     word ptr wndsprite+2, dx
    cmp     byte_46436, 0
    jz      short loc_131FC
    mov     ax, 0Fh
    push    ax
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr [bp+var_46], ax
    mov     word ptr [bp+var_46+2], dx
loc_131FC:
    mov     [bp+var_52], 0FFh
    call    sub_28F7C
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 63h ; 'c'
    push    ax
    mov     ax, 140h
    push    ax
    mov     ax, 65h ; 'e'
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     [bp+var_70], 6Bh ; 'k'
    mov     ax, 491h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    cmp     word_4434E, 0
    jnz     short loc_13281
    jmp     loc_13354
loc_13281:
    mov     ax, 1
    push    ax              ; int
    mov     ax, word_4434E
    sub     ax, word_44356
    push    ax
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    test    byte_43966, 2
    jz      short loc_132DC
    mov     ax, 495h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
loc_132DC:
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
    cmp     word_44356, 0
    jnz     short loc_1330D
    jmp     loc_133A7
loc_1330D:
    mov     ax, 499h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 1
    push    ax              ; int
    push    word_44356
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    jmp     short loc_13380
    ; align 2
    db 144
loc_13354:
    mov     ax, 49Dh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
loc_13380:
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
loc_133A7:
    mov     [bp+var_18], 2
    cmp     opponent_index, 0
    jnz     short loc_133B5
    jmp     loc_134DC
loc_133B5:
    cmp     word_44350, 0
    jnz     short loc_1340C
    mov     ax, 4A1h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 4A5h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    jmp     loc_134AA
    ; align 2
    db 144
loc_1340C:
    cmp     word_4434E, 0
    jz      short loc_1341C
    mov     ax, word_4434E
    cmp     word_44350, ax
    jnb     short loc_13466
loc_1341C:
    mov     ax, 4A9h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 1
    push    ax              ; int
    push    word_44350
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     [bp+var_18], 1
    jmp     short loc_134B5
loc_13466:
    mov     ax, 4ADh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 1
    push    ax              ; int
    push    word_44350
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
loc_134AA:
    cmp     word_4434E, 0
    jz      short loc_134B5
    mov     [bp+var_18], 0
loc_134B5:
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
loc_134DC:
    cmp     [bp+var_18], 0
    jnz     short loc_134F0
    mov     ax, 4B1h
    push    ax
    mov     ax, 4B6h
    push    ax
    mov     ax, 4BDh
    jmp     short loc_134FB
    ; align 2
    db 144
loc_134F0:
    mov     ax, 4C6h
    push    ax
    mov     ax, 4CBh
    push    ax
    mov     ax, 4D2h
loc_134FB:
    push    ax              ; char *
    call    load_audio_res
    add     sp, 6
    mov     al, opponent_index
    mov     [bp+var_16], al
    cmp     [bp+var_18], 2
    jnz     short loc_1351D
    mov     ax, word_44352
    cmp     word_44354, ax
    jz      short loc_1351D
    mov     [bp+var_16], 0
loc_1351D:
    mov     ax, 4DBh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, word_44352
    add     ax, word_45A24
    jz      short loc_1356A
    mov     ax, word_44352
    add     ax, word_45A24
    sub     cx, cx
    push    cx
    push    ax
    push    word_4434A
    push    word_44348
    call    __aFuldiv
    mov     al, ah
    mov     ah, dl
    mov     dl, dh
    sub     dh, dh
    mov     di, ax
    jmp     short loc_1356C
loc_1356A:
    sub     di, di
loc_1356C:
    mov     ax, 3
    push    ax              ; int
    sub     ax, ax
    push    ax              ; int
    push    di
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_296E0
    add     sp, 8
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, 4DFh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
    cmp     word_44358, 0
    jnz     short loc_135ED
    jmp     loc_1368B
loc_135ED:
    mov     ax, 4E3h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 3
    push    ax              ; int
    sub     ax, ax
    push    ax              ; int
    mov     ax, word_44358
    mov     cl, 8
    shr     ax, cl
    push    ax
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_296E0
    add     sp, 8
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, 4E7h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
loc_1368B:
    mov     ax, 4EBh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 3
    push    ax              ; int
    sub     ax, ax
    push    ax              ; int
    mov     ax, word_4435A
    mov     cl, 8
    shr     ax, cl
    push    ax
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_296E0
    add     sp, 8
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    mov     ax, 4EFh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    add     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    add     [bp+var_70], 0Ah
    cmp     word_4435C, 0
    jz      short loc_1379A
    mov     ax, 4F3h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 3
    push    ax              ; int
    sub     ax, ax
    push    ax              ; int
    push    word_4435C
    lea     ax, [bp+var_12]
    push    ax              ; char *
    call    sub_296E0
    add     sp, 8
    lea     ax, [bp+var_12]
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    sub     ax, ax
    push    ax
    push    word_407CA
    push    [bp+var_70]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
loc_1379A:
    cmp     [bp+var_16], 0
    jnz     short loc_137A3
    jmp     loc_138FF
loc_137A3:
    test    byte_43966, 4
    jz      short loc_137AD
    jmp     loc_1384B
loc_137AD:
    mov     ax, word_40D40
    mov     word_40D3A, ax
    mov     ax, word_40D42
    mov     word_40D3C, ax
    mov     ax, word_40D44
    mov     word_40D3E, ax
    call    get_super_random
    cwd
    mov     cx, 3
    idiv    cx
    mov     word_40D40, dx
    mov     ax, word_40D3A
    cmp     dx, ax
    jnz     short loc_137E0
    mov     bx, dx
    shl     bx, 1
    mov     ax, [bx+56Eh]
    mov     word_40D40, ax
loc_137E0:
    call    get_super_random
    cwd
    mov     cx, 3
    idiv    cx
    mov     word_40D44, dx
    mov     ax, word_40D3E
    cmp     dx, ax
    jnz     short loc_13801
    mov     bx, dx
    shl     bx, 1
    mov     ax, [bx+56Eh]
    mov     word_40D44, ax
loc_13801:
    cmp     [bp+var_18], 1
    jnz     short loc_1382A
    cmp     word_4434E, 0
    jz      short loc_1381E
    call    get_super_random
    cwd
    mov     cx, 2
    idiv    cx
    add     dx, cx
    jmp     short loc_13835
    ; align 2
    db 144
loc_1381E:
    call    get_super_random
    cwd
    mov     cx, 2
    jmp     short loc_13833
    ; align 2
    db 144
loc_1382A:
    call    get_super_random
    cwd
    mov     cx, 4
loc_13833:
    idiv    cx
loc_13835:
    mov     word_40D42, dx
    mov     ax, word_40D3C
    cmp     dx, ax
    jnz     short loc_1384B
    mov     bx, dx
    shl     bx, 1
    mov     ax, [bx+574h]
    mov     word_40D42, ax
loc_1384B:
    cmp     [bp+var_18], 1
    jnz     short loc_138B6
    mov     al, opponent_index
    add     al, 30h ; '0'
    mov     byte_3BD17, al
    mov     ax, 5A4h
    push    ax              ; char *
    mov     ax, 3
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     [bp+var_1C], ax
    mov     [bp+var_1A], dx
    mov     ax, 4F7h
    push    ax
    push    [bp+var_66]
    push    [bp+var_68]
    call    locate_shape_alt
    add     sp, 6
    mov     [bp+var_5A], ax
    mov     [bp+var_58], dx
    cmp     word_4434E, 0
    jz      short loc_138A0
    call    get_kevinrandom
    add     ax, word_4434C
smart
    and     ax, 1
nosmart
    add     ax, 2
    jmp     short loc_138AC
    ; align 2
    db 144
loc_138A0:
    call    get_kevinrandom
    add     ax, word_4434C
smart
    and     ax, 1
nosmart
loc_138AC:
    mov     word_40D42, ax
    mov     [bp+var_6A], 76h ; 'v'
    jmp     short loc_138FF
    ; align 2
    db 144
loc_138B6:
    mov     al, opponent_index
    add     al, 30h ; '0'
    mov     byte_3BD1F, al
    mov     ax, 5ACh
    push    ax              ; char *
    mov     ax, 3
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     [bp+var_1C], ax
    mov     [bp+var_1A], dx
    mov     ax, 4FCh
    push    ax
    push    [bp+var_66]
    push    [bp+var_68]
    call    locate_shape_alt
    add     sp, 6
    mov     [bp+var_5A], ax
    mov     [bp+var_58], dx
    call    get_kevinrandom
    add     ax, word_4434C
smart
    and     ax, 3
nosmart
    mov     word_40D42, ax
    mov     [bp+var_6A], 64h ; 'd'
loc_138FF:
    mov     [bp+var_6E], 0
    mov     ax, 95F8h
    push    ax              ; char *
    mov     ax, 501h
    push    ax              ; int
    mov     ax, 9241h
    push    ax
    mov     ax, 9Ch ; 'œ'
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    mov     ax, 95F8h
    push    ax              ; char *
    mov     ax, 1
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word ptr [bp+var_4A], ax
    mov     word ptr [bp+var_4A+2], dx
    or      ax, dx
    jnz     short loc_1397F
    sub     ax, ax
    push    ax
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 506h
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 1
    push    ax
    push    ax
    call    show_dialog
    add     sp, 12h
    or      ax, ax
    jz      short loc_1397F
    mov     ax, 95F8h
    push    ax              ; char *
    mov     ax, 1
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word ptr [bp+var_4A], ax
    mov     word ptr [bp+var_4A+2], dx
loc_1397F:
    mov     ax, word ptr [bp+var_4A]
    or      ax, word ptr [bp+var_4A+2]
    jz      short loc_139B6
    sub     di, di
    jmp     short loc_1398D
    ; align 2
    db 144
loc_1398C:
    inc     di
loc_1398D:
    cmp     di, 385h
    jge     short loc_139A6
    les     bx, trackdata14
    mov     al, es:[bx+di]
    les     bx, [bp+var_4A]
    cmp     es:[bx+di], al
    jz      short loc_1398C
    mov     [bp+var_6E], 0FFh
loc_139A6:
    push    word ptr [bp+var_4A+2]
    push    word ptr [bp+var_4A]
    call    mmgr_op_unk2
    add     sp, 4
    jmp     short loc_139BA
loc_139B6:
    mov     [bp+var_6E], 0FFh
loc_139BA:
    cmp     [bp+var_6E], 0FFh
    jz      short loc_139E1
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr highscore_write_a
    add     sp, 2
    or      al, al
    jz      short loc_139E1
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr highscore_write_a
    add     sp, 2
    or      al, al
    jz      short loc_139E1
    mov     [bp+var_6E], 0FFh
loc_139E1:
    cmp     [bp+var_6E], 0
    jnz     short loc_13A0F
    cmp     word_4434E, 0
    jz      short loc_13A0F
    mov     ax, word_4434E
    mov     [bp+var_88], ax
    test    byte_43966, 6
    jnz     short loc_13A0F
    or      ax, ax
    jz      short loc_13A0F
    les     bx, trackdata11
    cmp     es:[bx+16Ah], ax
    jbe     short loc_13A0F
    mov     [bp+var_6E], 1
loc_13A0F:
    mov     [bp+var_8E], 0
    mov     [bp+var_42], 1Eh
    mov     [bp+var_14], 1
loc_13A1D:
    cmp     [bp+var_16], 0
    jz      short loc_13A42
    cmp     [bp+var_6E], 2
    jnz     short loc_13A42
    mov     [bp+var_6E], 0
    call    sub_28F6A
    push    cs
    call    near ptr sub_1168E
    mov     [bp+var_92], 1
    mov     [bp+var_14], 1
    jmp     loc_13FDA
loc_13A42:
    cmp     [bp+var_16], 0
    jnz     short loc_13A4B
    jmp     loc_13F48
loc_13A4B:
    mov     byte_3BD29, 31h ; '1'
    mov     ax, 5B6h
    push    ax
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    locate_shape_1
    add     sp, 6
    mov     word ptr [bp+var_56], ax
    mov     word ptr [bp+var_56+2], dx
    les     bx, [bp+var_56]
    mov     ax, es:[bx]
    imul    word_44AE4
    mov     [bp+var_70], ax
    mov     ax, 138h
    sub     ax, [bp+var_70]
    mov     [bp+var_8C], ax
    mov     ax, 63h ; 'c'
    sub     ax, es:[bx+2]
    sar     ax, 1
    mov     [bp+var_90], ax
    push    word_407D2
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, es:[bx+2]
    add     ax, 5
    push    ax
    mov     ax, [bp+var_70]
    add     ax, 5
    push    ax
    mov     ax, [bp+var_90]
    sub     ax, 3
    push    ax
    mov     ax, [bp+var_8C]
    sub     ax, 3
    push    ax
    call    sub_2916E
    add     sp, 0Eh
    mov     al, [bp+var_8E]
    cbw
    mov     bx, ax
    add     bx, [bp+var_5A]
    mov     es, [bp+var_58]
    mov     al, es:[bx]
    add     al, 30h ; '0'
    mov     byte_3BD29, al
    push    [bp+var_90]
    push    [bp+var_8C]
    mov     ax, 5B6h
    push    ax
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33DE2
    add     sp, 8
    mov     al, [bp+var_8E]
    mov     [bp+var_6C], al
    sub     ax, ax
    push    ax
    push    ax
    call    sub_34B0C
    add     sp, 4
    mov     [bp+var_70], 8
    mov     [bp+var_40], 0
    mov     [bp+var_50], 0
    mov     [bp+var_80], 0
    cmp     [bp+var_18], 2
    jnz     short loc_13B2C
    mov     [bp+var_7A], 1
    jmp     short loc_13B31
    ; align 2
    db 144
loc_13B2C:
    mov     [bp+var_7A], 3
loc_13B31:
    sub     di, di
    jmp     loc_13CAF
loc_13B36:
    cmp     [bp+var_18], 2
    jnz     short loc_13B42
    mov     ax, 50Ah
    jmp     short loc_13B57
    ; align 2
    db 144
loc_13B42:
    mov     al, [bp+var_6A]
    mov     [bp+var_12], al
    mov     [bp+var_11], 31h ; '1'
    mov     al, byte ptr word_40D40
loc_13B4F:
    add     al, 61h ; 'a'
    mov     [bp+var_10], al
    lea     ax, [bp+var_12]
loc_13B57:
    push    ax
    push    [bp+var_66]
    push    [bp+var_68]
    call    locate_text_res
    add     sp, 6
    mov     word ptr [bp+var_86], ax
    mov     word ptr [bp+var_86+2], dx
loc_13B6E:
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
loc_13B7E:
    les     bx, [bp+var_86]
    inc     word ptr [bp+var_86]
    mov     al, es:[bx]
    mov     [bp+var_3E], al
    cmp     al, 20h ; ' '
    jz      short loc_13B97
    or      al, al
    jz      short loc_13B97
    jmp     loc_13C92
loc_13B97:
    mov     bx, [bp+var_80]
    add     bx, bp
    mov     byte ptr [bx-3Ch], 0
    lea     ax, [bp+var_3C]
    push    ax
    call    sub_32843
    add     sp, 2
    mov     [bp+var_8A], ax
    add     ax, [bp+var_50]
    mov     cx, [bp+var_8C]
    sub     cx, 10h
    cmp     ax, cx
    jge     short loc_13C1A
    mov     ax, [bp+var_40]
    add     ax, [bp+var_80]
    cmp     ax, 50h ; 'P'
    jge     short loc_13C1A
    mov     [bp+var_5C], 0
    jmp     short loc_13BF3
loc_13BD0:
    mov     al, [bp+var_6A]
    mov     [bp+var_12], al
    mov     [bp+var_11], 32h ; '2'
    mov     al, byte ptr word_40D42
    jmp     loc_13B4F
loc_13BE0:
    mov     al, [bp+var_6A]
    mov     [bp+var_12], al
    mov     [bp+var_11], 33h ; '3'
    mov     al, byte ptr word_40D44
    jmp     loc_13B4F
loc_13BF0:
    inc     [bp+var_5C]
loc_13BF3:
    mov     ax, [bp+var_80]
    cmp     [bp+var_5C], ax
    jge     short loc_13C10
    mov     bx, [bp+var_5C]
    add     bx, bp
    mov     al, [bx-3Ch]
    mov     bx, [bp+var_40]
    inc     [bp+var_40]
    mov     [bx-538Ch], al
    jmp     short loc_13BF0
    ; align 2
    db 144
loc_13C10:
    mov     ax, [bp+var_8A]
    add     [bp+var_50], ax
    jmp     short loc_13C86
    ; align 2
    db 144
loc_13C1A:
    mov     bx, [bp+var_40]
    mov     byte ptr [bx-538Ch], 0
    push    [bp+var_70]
    mov     ax, 8
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    add     [bp+var_70], 8
    cmp     [bp+var_3C], 20h ; ' '
    jnz     short loc_13C46
    mov     [bp+var_5C], 1
    jmp     short loc_13C4B
loc_13C46:
    mov     [bp+var_5C], 0
loc_13C4B:
    mov     [bp+var_40], 0
    jmp     short loc_13C67
loc_13C52:
    mov     bx, [bp+var_5C]
    add     bx, bp
    mov     al, [bx-3Ch]
    mov     bx, [bp+var_40]
    inc     [bp+var_40]
    mov     [bx-538Ch], al
    inc     [bp+var_5C]
loc_13C67:
    mov     ax, [bp+var_80]
    cmp     [bp+var_5C], ax
    jl      short loc_13C52
    mov     bx, [bp+var_40]
    mov     byte ptr [bx-538Ch], 0
    mov     ax, 0AC74h
    push    ax
    call    sub_32843
    add     sp, 2
    mov     [bp+var_50], ax
loc_13C86:
    mov     [bp+var_80], 1
    mov     [bp+var_3C], 20h ; ' '
    jmp     short loc_13CA0
    ; align 2
    db 144
loc_13C92:
    mov     bx, [bp+var_80]
    inc     [bp+var_80]
    add     bx, bp
    mov     al, [bp+var_3E]
    mov     [bx-3Ch], al
loc_13CA0:
    cmp     [bp+var_3E], 0
    jz      short loc_13CA9
    jmp     loc_13B7E
loc_13CA9:
    call    set_fontdef
    inc     di
loc_13CAF:
    cmp     [bp+var_7A], di
    jle     short loc_13CD0
    mov     ax, di
    or      ax, ax
    jnz     short loc_13CBD
    jmp     loc_13B36
loc_13CBD:
    cmp     ax, 1
    jnz     short loc_13CC5
    jmp     loc_13BD0
loc_13CC5:
    cmp     ax, 2
    jnz     short loc_13CCD
    jmp     loc_13BE0
loc_13CCD:
    jmp     loc_13B6E
loc_13CD0:
    cmp     [bp+var_40], 0
    jz      short loc_13D06
    push    word_44D24
    push    word_44D22
    call    sub_2988A
    add     sp, 4
    mov     bx, [bp+var_40]
    mov     byte ptr [bx-538Ch], 0
    push    [bp+var_70]
    mov     ax, 8
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    call    set_fontdef
loc_13D06:
    mov     [bp+var_14], 0
    cmp     [bp+var_6E], 0
    jg      short loc_13D13
    jmp     loc_13FDA
loc_13D13:
    mov     [bp+var_6E], 0
    mov     [bp+var_14], 1
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 15h
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, 81h ; ''
    push    ax
    mov     ax, 50Eh
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     al, [bp+var_52]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_52], 0FEh ; 'þ'
    call    sub_29772
    call    check_input
    mov     [bp+var_70], 1
    call    sub_28F3C
loc_13D83:
    push    word_407D0
    push    word_407CE
    mov     ax, 59Ah
    push    ax
    mov     ax, 590h
    push    ax
    mov     ax, 586h
    push    ax
    mov     ax, 57Ch
    push    ax
    mov     ax, 4
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_72], ax
    add     [bp+var_42], ax
    cmp     [bp+var_42], 1Eh
    jl      short loc_13DD3
    sub     [bp+var_42], 1Eh
    inc     [bp+var_8E]
    mov     al, [bp+var_8E]
    cbw
    mov     bx, ax
    add     bx, [bp+var_5A]
    mov     es, [bp+var_58]
    cmp     byte ptr es:[bx], 0
    jnz     short loc_13DD3
    mov     [bp+var_8E], 0
loc_13DD3:
    mov     al, [bp+var_6C]
    cmp     [bp+var_8E], al
    jnz     short loc_13DDF
    jmp     loc_13EA5
loc_13DDF:
    mov     al, [bp+var_8E]
    mov     [bp+var_6C], al
    cbw
    mov     bx, ax
    add     bx, [bp+var_5A]
    mov     es, [bp+var_58]
    mov     al, es:[bx]
    add     al, 30h ; '0'
    mov     byte_3BD29, al
    call    sub_28DB6
    mov     ax, 5B6h
    push    ax
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    locate_shape_1
    add     sp, 6
    mov     word ptr [bp+var_56], ax
    mov     word ptr [bp+var_56+2], dx
    cmp     byte_46436, 0
    jz      short loc_13E8A
    push    word ptr [bp+var_46+2]
    push    word ptr [bp+var_46]
    call    set_sprite1
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    push    word ptr [bp+var_56+2]
    push    word ptr [bp+var_56]
    call    sub_33DE2
    add     sp, 8
    call    sub_28F3C
    les     bx, [bp+var_56]
    mov     ax, es:[bx+2]
    add     ax, [bp+var_90]
    push    ax
    push    [bp+var_90]
    mov     ax, es:[bx]
    imul    word_44AE4
    add     ax, [bp+var_8C]
    push    ax
    push    [bp+var_8C]
    call    set_sprite1_size
    add     sp, 8
    push    [bp+var_90]
    push    [bp+var_8C]
    les     bx, [bp+var_46]
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
    add     sp, 8
    call    sub_28F3C
    jmp     short loc_13EA0
    ; align 2
    db 144
loc_13E8A:
    push    [bp+var_90]
    push    [bp+var_8C]
    push    word ptr [bp+var_56+2]
    push    word ptr [bp+var_56]
    call    sub_33DE2
    add     sp, 8
loc_13EA0:
    call    sub_28D9E
loc_13EA5:
    push    di
    call    input_checking
    add     sp, 2
    mov     si, ax
    cmp     si, 0Dh
    jz      short loc_13EBF
    cmp     si, 20h ; ' '
    jz      short loc_13EBF
    cmp     si, 1Bh
    jnz     short loc_13EC4
loc_13EBF:
    mov     [bp+var_70], 0
loc_13EC4:
    cmp     [bp+var_70], 0
    jz      short loc_13ECD
    jmp     loc_13D83
loc_13ECD:
    call    sub_28F6A
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    mov     ax, word_3BD0A
    inc     ax
    push    ax
    push    word_3BD00
    mov     ax, 138h
    push    ax
    mov     ax, 8
    push    ax
    call    set_sprite1_size
    add     sp, 8
    push    word_407F8
    call    clear_sprite1_color
    add     sp, 2
    call    sub_28DB6
    mov     al, [bp+var_18]
    cbw
    push    ax
    mov     ax, 512h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    push    [bp+var_88]
    push    cs
    call    near ptr enter_hiscore
    add     sp, 8
    jmp     loc_13FDA
loc_13F48:
    cmp     [bp+var_6E], 0
    jle     short loc_13F84
    call    check_input
    call    sub_28DB6
    sub     ax, ax
    push    ax
    mov     ax, 516h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    push    [bp+var_88]
    push    cs
    call    near ptr enter_hiscore
    add     sp, 8
    mov     [bp+var_6E], 0
    mov     [bp+var_52], 0FEh ; 'þ'
    jmp     short loc_13FDA
loc_13F84:
    call    sub_28DB6
    cmp     [bp+var_6E], 0FFh
    jnz     short loc_13FD6
    mov     ax, 51Ah
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    sub     ax, ax
    push    ax
loc_13FB2:
    push    word_407CA
    mov     ax, 32h ; '2'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_29008
    add     sp, 0Ah
    jmp     short loc_13FDA
    ; align 2
    db 144
loc_13FD6:
    push    cs
    call    near ptr sub_1168E
loc_13FDA:
    mov     [bp+var_92], 1
    mov     [bp+var_78], 1
    call    sub_29772
    call    sub_28F6A
    cmp     [bp+var_16], 0
    jz      short loc_13FF9
    cmp     [bp+var_6E], 0FFh
    jnz     short loc_14002
loc_13FF9:
    mov     [bp+var_9C], 0FFDCh
    jmp     short loc_14058
    ; align 2
    db 144
loc_14002:
    mov     [bp+var_9C], 0
    cmp     [bp+var_14], 0
    jz      short loc_14014
    mov     ax, 51Eh
    jmp     short loc_14017
    ; align 2
    db 144
loc_14014:
    mov     ax, 522h
loc_14017:
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    mov     [bp+var_7E], ax
    mov     [bp+var_7C], dx
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 15h
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, word_3BCEC
    inc     ax
    push    ax
    push    dx
    push    [bp+var_7E]
    call    sub_292DC
    add     sp, 14h
loc_14058:
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 15h
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, [bp+var_9C]
    add     ax, word_3BCEE
    inc     ax
    push    ax
    mov     ax, 526h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    cmp     [bp+var_16], 0
    jz      short loc_140A4
    mov     ax, 52Ah
    jmp     short loc_140A7
loc_140A4:
    mov     ax, 52Eh
loc_140A7:
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    mov     [bp+var_7E], ax
    mov     [bp+var_7C], dx
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 15h
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, [bp+var_9C]
    add     ax, word_3BCF0
    inc     ax
    push    ax
    push    dx
    push    [bp+var_7E]
    call    sub_292DC
    add     sp, 14h
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 15h
    push    ax
    mov     ax, 46h ; 'F'
    push    ax
    mov     ax, 0AFh ; '¯'
    push    ax
    mov     ax, [bp+var_9C]
    add     ax, word_3BCF2
    inc     ax
    push    ax
    mov     ax, 532h
    push    ax
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    call    sub_292DC
    add     sp, 14h
    sub     di, di
loc_14130:
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_9E], ax
    mov     bx, ax
    mov     ax, [bx+57Ch]
    add     ax, [bp+var_9C]
    add     bx, bp
    mov     [bx-64h], ax
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_9E], ax
    mov     bx, ax
    mov     ax, [bx+586h]
    add     ax, [bp+var_9C]
    add     bx, bp
    mov     [bx-9Ah], ax
    inc     di
    cmp     di, 4
    jl      short loc_14130
    call    check_input
    mov     al, [bp+var_52]
    cbw
    push    ax
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    sub_29620
    add     sp, 6
    mov     [bp+var_52], 0FEh ; 'þ'
    call    sub_28F3C
loc_14188:
    mov     al, [bp+var_78]
    cmp     [bp+var_92], al
    jz      short loc_141DC
    mov     al, [bp+var_92]
    mov     [bp+var_78], al
    call    sub_28F3C
    mov     ax, word_3BD0A
    inc     ax
    push    ax
    push    word_3BD00
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    call    sub_28DB6
    les     bx, wndsprite
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    call    sub_28D9E
    call    timer_get_delta2
    call    sub_29772
loc_141DC:
    push    word_407D0
    push    word_407CE
    mov     ax, 59Ah
    push    ax
    mov     ax, 590h
    push    ax
    lea     ax, [bp+var_9A]
    push    ax
    lea     ax, [bp+var_64]
    push    ax
    mov     al, [bp+var_92]
    cbw
    push    ax
    call    sub_29786
    add     sp, 0Eh
    mov     [bp+var_72], ax
    cmp     [bp+var_14], 0
    jz      short loc_1420F
    jmp     loc_14337
loc_1420F:
    cmp     [bp+var_18], 2
    jnz     short loc_14218
    jmp     loc_14337
loc_14218:
    add     [bp+var_42], ax
    cmp     [bp+var_42], 1Eh
    jl      short loc_14241
    sub     [bp+var_42], 1Eh
    inc     [bp+var_8E]
    mov     al, [bp+var_8E]
    cbw
    mov     bx, ax
    add     bx, [bp+var_5A]
    mov     es, [bp+var_58]
    cmp     byte ptr es:[bx], 0
    jnz     short loc_14241
    mov     [bp+var_8E], 0
loc_14241:
    mov     al, [bp+var_6C]
    cmp     [bp+var_8E], al
    jnz     short loc_1424D
    jmp     loc_14337
loc_1424D:
    mov     al, [bp+var_8E]
    mov     [bp+var_6C], al
    cbw
    mov     bx, ax
    add     bx, [bp+var_5A]
    mov     es, [bp+var_58]
    mov     al, es:[bx]
    add     al, 30h ; '0'
    mov     byte_3BD29, al
    call    sub_28DB6
    mov     ax, 5B6h
    push    ax
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    locate_shape_1
    add     sp, 6
    mov     word ptr [bp+var_56], ax
    mov     word ptr [bp+var_56+2], dx
    cmp     byte_46436, 0
    jz      short loc_142F8
    push    word ptr [bp+var_46+2]
    push    word ptr [bp+var_46]
    call    set_sprite1
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    push    word ptr [bp+var_56+2]
    push    word ptr [bp+var_56]
    call    sub_33DE2
    add     sp, 8
    call    sub_28F3C
    les     bx, [bp+var_56]
    mov     ax, es:[bx+2]
    add     ax, [bp+var_90]
    push    ax
    push    [bp+var_90]
    mov     ax, es:[bx]
    imul    word_44AE4
    add     ax, [bp+var_8C]
    push    ax
    push    [bp+var_8C]
    call    set_sprite1_size
    add     sp, 8
    push    [bp+var_90]
    push    [bp+var_8C]
    les     bx, [bp+var_46]
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
    add     sp, 8
    call    sub_28F3C
    jmp     short loc_1430E
    ; align 2
    db 144
loc_142F8:
    push    [bp+var_90]
    push    [bp+var_8C]
    push    word ptr [bp+var_56+2]
    push    word ptr [bp+var_56]
    call    sub_33DE2
    add     sp, 8
loc_1430E:
    push    [bp+var_90]
    push    [bp+var_8C]
    mov     ax, 5B6h
    push    ax
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    locate_shape_1
    add     sp, 6
    push    dx
    push    ax
    call    sub_33DE2
    add     sp, 8
    call    sub_28D9E
loc_14337:
    cmp     [bp+var_16], 0
    jz      short loc_14343
    cmp     [bp+var_6E], 0FFh
    jnz     short loc_1436C
loc_14343:
    mov     ax, 59Ch
    push    ax
    mov     ax, 592h
    push    ax
    lea     ax, [bp+var_98]
    push    ax
    lea     ax, [bp+var_62]
    push    ax
    mov     ax, 3
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_82], al
    cmp     al, 0FFh
    jz      short loc_14395
    inc     al
    jmp     short loc_14391
loc_1436C:
    mov     ax, 59Ah
    push    ax
    mov     ax, 590h
    push    ax
    lea     ax, [bp+var_9A]
    push    ax
    lea     ax, [bp+var_64]
    push    ax
    mov     ax, 4
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_82], al
    cmp     al, 0FFh
    jz      short loc_14395
loc_14391:
    mov     [bp+var_92], al
loc_14395:
    push    [bp+var_72]
    call    input_checking
    add     sp, 2
    mov     si, ax
    or      si, si
    jnz     short loc_143A9
    jmp     loc_14188
loc_143A9:
    cmp     ax, 0Dh
    jz      short loc_143C6
    cmp     ax, 20h ; ' '
    jz      short loc_143C6
    cmp     ax, 4B00h
    jnz     short loc_143BB
    jmp     loc_1447A
loc_143BB:
    cmp     ax, 4D00h
    jnz     short loc_143C3
    jmp     loc_144A4
loc_143C3:
    jmp     loc_14188
loc_143C6:
    cmp     [bp+var_92], 0
    jnz     short loc_1440C
    call    sub_28F6A
    sub     ax, ax
    push    ax
    push    word_407F8
    push    word_407F6
    push    word_407F4
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    ax
    call    sub_292DC
    add     sp, 14h
    cmp     [bp+var_14], 0
    jz      short loc_14404
    mov     [bp+var_6E], 0
    jmp     loc_13A1D
loc_14404:
    mov     [bp+var_6E], 2
    jmp     loc_13A1D
    ; align 2
    db 144
loc_1440C:
    call    unload_audio_res
    cmp     [bp+var_16], 0
    jz      short loc_14425
    push    [bp+var_1A]
    push    [bp+var_1C]
    call    mmgr_op_unk2
    add     sp, 4
loc_14425:
    cmp     byte_46436, 0
    jz      short loc_1443A
    push    word ptr [bp+var_46+2]
    push    word ptr [bp+var_46]
    call    release_window
    add     sp, 4
loc_1443A:
    push    word ptr wndsprite+2
    push    word ptr wndsprite
    call    release_window
    add     sp, 4
    cmp     opponent_index, 0
    jz      short loc_1445F
    push    [bp+var_66]
    push    [bp+var_68]
    call    unload_resource
    add     sp, 4
loc_1445F:
    push    [bp+var_4C]
    push    [bp+var_4E]
    call    unload_resource
    add     sp, 4
    mov     al, [bp+var_92]
    cbw
    dec     ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_1447A:
    cmp     [bp+var_16], 0
    jz      short loc_14486
    cmp     [bp+var_6E], 0FFh
    jnz     short loc_14496
loc_14486:
    cmp     [bp+var_92], 1
    jg      short loc_1449D
loc_1448D:
    mov     [bp+var_92], 3
    jmp     loc_14188
    ; align 2
    db 144
loc_14496:
    cmp     [bp+var_92], 0
    jz      short loc_1448D
loc_1449D:
    dec     [bp+var_92]
    jmp     loc_14188
loc_144A4:
    cmp     [bp+var_92], 3
    jge     short loc_144B2
    inc     [bp+var_92]
    jmp     loc_14188
loc_144B2:
    cmp     [bp+var_16], 0
    jz      short loc_144BE
    cmp     [bp+var_6E], 0FFh
    jnz     short loc_144C6
loc_144BE:
    mov     [bp+var_92], 1
    jmp     loc_14188
loc_144C6:
    mov     [bp+var_92], 0
    jmp     loc_14188
    pop     si
end_hiscore endp
security_check proc far
    var_440 = byte ptr -1088
    var_43E = byte ptr -1086
    var_428 = word ptr -1064
    var_426 = word ptr -1062
    var_424 = word ptr -1060
    var_422 = word ptr -1058
    var_420 = word ptr -1056
    var_41E = word ptr -1054
    var_41C = word ptr -1052
    var_41A = word ptr -1050
    var_40E = word ptr -1038
    var_40C = word ptr -1036
    var_40A = word ptr -1034
    var_408 = word ptr -1032
    var_406 = byte ptr -1030
    var_405 = byte ptr -1029
    var_404 = byte ptr -1028
    var_403 = byte ptr -1027
    var_402 = byte ptr -1026
    var_401 = byte ptr -1025
    var_400 = byte ptr -1024
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 440h
    push    si
    mov     bx, [bp+arg_0]
    mov     al, [bx+5C4h]
    mov     byte_3BD4C, al
    mov     al, [bx+5C4h]
    mov     byte_3BD50, al
    mov     ax, 5BBh
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_40C], ax
    mov     [bp+var_40A], dx
    mov     ax, 5C0h
    push    ax
    push    dx
    push    [bp+var_40C]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    lea     ax, [bp+var_400]
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 5DAh
    push    ax
    push    [bp+var_40A]
    push    [bp+var_40C]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, 0AC7Ah
    push    ax
    lea     ax, [bp+var_400]
    push    ax              ; char *
    call    _strcat
    add     sp, 4
    sub     si, si
loc_14551:
    mov     al, [si-538Ch]
    mov     [bp+si+var_406], al
    inc     si
    cmp     si, 6
    jl      short loc_14551
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_428]
    push    ax
    push    word_40800
    mov     ax, 78h ; 'x'
    push    ax
    mov     ax, 0FFFFh
    push    ax
    lea     ax, [bp+var_400]
    push    ss
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 3
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     byte_463E6, 0
    mov     al, [bp+var_406]
    mov     byte_463E4, al
    mov     al, [bp+var_405]
    mov     byte_463E5, al
    push    [bp+var_426]
    push    [bp+var_428]
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    mov     al, [bp+var_404]
    mov     byte_463E4, al
    mov     al, [bp+var_403]
    mov     byte_463E5, al
    push    [bp+var_422]
    push    [bp+var_424]
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    mov     al, [bp+var_402]
    mov     byte_463E4, al
    mov     al, [bp+var_401]
    mov     byte_463E5, al
    push    [bp+var_41E]
    push    [bp+var_420]
    mov     ax, 0AC74h
    push    ax
    call    sub_33742
    add     sp, 6
    mov     ax, 5DEh
    push    ax
    push    [bp+var_40A]
    push    [bp+var_40C]
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    copy_string
    add     sp, 6
    mov     ax, [bp+var_41C]
    mov     dx, [bp+var_41A]
    mov     [bp+var_428], ax
    mov     [bp+var_426], dx
    mov     ax, 0AC74h
    push    ax              ; char *
    call    _strlen
    add     sp, 2
    mov     [bp+var_408], ax
    mov     [bp+var_43E], 0
    mov     [bp+var_40E], 0
loc_14641:
    mov     ax, 7530h
    cwd
    push    dx              ; int
    push    ax              ; int
    push    [bp+var_426]    ; int
    push    [bp+var_428]    ; int
    push    [bp+var_408]
    lea     ax, [bp+var_43E]
    push    ax              ; char *
    call    sub_290BC
    add     sp, 0Ch
    sub     si, si
    jmp     short loc_1468D
loc_14664:
    mov     al, [bp+si+var_43E]
    cbw
    mov     bx, ax
    mov     al, [bx+382Fh]
smart
    and     al, 1
nosmart
    mov     [bp+var_440], al
    or      al, al
    jz      short loc_1468C
    jz      short loc_14684
    mov     al, [bp+si+var_43E]
    add     al, 20h ; ' '
    jmp     short loc_14688
    ; align 2
    db 144
loc_14684:
    mov     al, [bp+si+var_43E]
loc_14688:
    mov     [bp+si+var_43E], al
loc_1468C:
    inc     si
loc_1468D:
    cmp     [bp+si+var_43E], 0
    jnz     short loc_14664
    mov     ax, 0AC74h
    push    ax
    lea     ax, [bp+var_43E]
    push    ax              ; char *
    call    _strcmp
    add     sp, 4
    or      ax, ax
    jnz     short loc_146B0
    mov     byte_45E1A, 1
    jmp     short loc_146B4
loc_146B0:
    inc     [bp+var_40E]
loc_146B4:
    cmp     byte_45E1A, 0
    jnz     short loc_146C5
    cmp     [bp+var_40E], 3
    jz      short loc_146C5
    jmp     loc_14641
loc_146C5:
    call    sub_275C6
    call    sub_28D9E
    push    [bp+var_40A]
    push    [bp+var_40C]
    call    unload_resource
    add     sp, 4
    pop     si
    mov     sp, bp
    pop     bp
    retf
security_check endp
set_default_car proc far

    mov     player_car_id, 43h ; 'C'
    mov     byte_449A5, 4Fh ; 'O'
    mov     byte_449A6, 55h ; 'U'
    mov     byte_449A7, 4Eh ; 'N'
    mov     byte_449A8, 0
    mov     opponent_index, 0
    mov     byte_449AF, 0
    mov     byte_449A9, 1
    mov     opponent_car_id, 0FFh
    retf
set_default_car endp
seg000 ends
end
