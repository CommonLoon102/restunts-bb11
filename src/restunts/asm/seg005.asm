.model medium
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
    include seg001.inc
    include seg002.inc
    include seg003.inc
    include seg004.inc
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
seg005 segment byte public 'STUNTSC' use16
    assume cs:seg005
    assume es:nothing, ss:nothing, ds:dseg
    public run_game
    public sub_223FA
    public sub_22532
    public sub_2255A
    public sub_22576
    public sub_22698
    public sub_2298C
    public sub_22C92
    public sub_22CE8
    public setup_car_shapes
    public setup_player_cars
    public sub_239B4
    public sub_23A50
    public sub_23A98
    public sub_23B4C
    public off_2481A
    public off_24D20
    ; align 2
    db 144
run_game proc far
    var_16 = byte ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
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
    sub     sp, 16h
    push    si
    mov     [bp+var_C], 0FFFFh
    mov     word_43934, 0
    mov     word_43936, 140h
    mov     [bp+var_2], 0FFFFh
    mov     word_449EA, 0FFFFh
    call    sub_19E7B
    mov     cl, 3
    shl     ax, cl
    mov     word_42D28, ax
    mov     byte_4616F, 1
    mov     byte_454B8, 0
    cmp     byte_44AE2, 0
    jz      short loc_21BEC
    inc     byte_3B8F5
    cmp     byte_3B8F5, 4
    jnz     short loc_21BCA
    mov     byte_3B8F5, 0
loc_21BCA:
    mov     byte_45DB2, 2
    mov     ax, 307Ch
    push    ax
    sub     ax, ax
    push    ax              ; char *
    push    cs
    call    near ptr sub_22C92
    add     sp, 4
    or      al, al
    jz      short loc_21BE4
    jmp     loc_223F4
loc_21BE4:
    call    setup_track
    jmp     short loc_21C0F
    ; align 2
    db 144
loc_21BEC:
    cmp     word_449BC, 0
    jnz     short loc_21C00
    mov     byte_3B8F5, 0
    mov     byte_45DB2, 1
    jmp     short loc_21C0F
    ; align 2
    db 144
loc_21C00:
    mov     byte_3B8F5, 0
    mov     byte_45DB2, 2
    mov     byte_454B8, 1
loc_21C0F:
    push    cs
    call    near ptr setup_player_cars
    or      ax, ax
    jz      short loc_21C24
    push    cs
    call    near ptr sub_239B4
    call    sub_2A200
    jmp     loc_223E4
    ; align 2
    db 144
loc_21C24:
    mov     byte_3B8F8, 0
    mov     byte_449E6, 0
    mov     byte_449DA, 1
    push    cs
    call    near ptr sub_2255A
    mov     byte_461C8, 0FFh
    mov     byte_44346, 0
    mov     byte_4432A, 0
    mov     byte_46467, 0
    mov     byte_43950, 0
    cmp     byte_44AE2, 0
    jz      short loc_21C6E
    mov     al, byte_449BA
    cbw
    mov     word_449D0, ax
loc_21C5E:
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
    jmp     loc_21DA2
    ; align 2
    db 144
loc_21C6E:
    cmp     byte_454B8, 0
    jz      short loc_21C78
    jmp     loc_21D2C
loc_21C78:
    mov     byte_3B8F5, 0
    mov     byte_43950, 1
    mov     byte_4499F, 0
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     al, byte ptr word_44D4E
    mov     byte_449BA, al
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
    mov     word_45D94, 0
    mov     byte_45D3E, 0
    mov     byte_4393C, 1
    mov     al, byte_3B8F2
    cbw
    push    ax
    push    cs
    call    near ptr sub_23A50
    add     sp, 2
    mov     byte_45DB2, 1
    mov     ax, 0FF10h
    push    ax
    push    word_45516
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    cwd
    mov     cl, 6
loc_21CDC:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_21CDC
    add     word_445E6, ax
    adc     word_445E8, dx
    mov     ax, 0FF10h
    push    ax
    push    word_45516
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    cwd
    mov     cl, 6
loc_21D08:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_21D08
    add     word_445EE, ax
    adc     word_445F0, dx
    add     word_445EA, 580h
    adc     word_445EC, 0
    mov     byte_43966, 1
    jmp     short loc_21DA2
    ; align 4
    db 144
    db 144
loc_21D2C:
    mov     byte_3B8F5, 0
    mov     byte_45DB2, 2
    mov     word_44DCA, 1F4h
    mov     al, byte_449BA
    cbw
    mov     word_449D0, ax
    sub     ax, ax
    push    ax
    call    sub_16F3A
    add     sp, 2
    push    word_449BC
    call    sub_16F3A
    add     sp, 2
    jmp     short loc_21D72
loc_21D5C:
    mov     ax, 1
    push    ax
    call    sub_289E2
    add     sp, 2
    cmp     ax, 1Bh
    jz      short loc_21D7B
    call    sub_17008
loc_21D72:
    mov     ax, word_445D4
    cmp     word_449BC, ax
    jnz     short loc_21D5C
loc_21D7B:
    mov     ax, word_449BC
    mov     word_42D02, ax
    jmp     short loc_21DA2
    ; align 2
    db 144
loc_21D84:
    cmp     byte_3B8F2, 0
    jnz     short loc_21D92
    cmp     byte_3FE00, 0
    jz      short loc_21D9D
loc_21D92:
    cmp     byte_45DB2, 0
    jnz     short loc_21D9D
    push    cs
    call    near ptr sub_23A98
loc_21D9D:
    call    sub_17008
loc_21DA2:
    mov     ax, word_42D02
    cmp     word_445D4, ax
    jnz     short loc_21D84
    cmp     byte_45DB2, 0
    jnz     short loc_21DCB
    cmp     byte_449DA, 0
    jnz     short loc_21DCB
    cmp     byte_44889, 0
    jz      short loc_21DCB
    mov     ax, word_445D4
    cmp     [bp+var_C], ax
    jz      short loc_21DA2
    mov     [bp+var_C], ax
loc_21DCB:
    cmp     byte_44889, 0
    jnz     short loc_21DEB
    cmp     byte_45DB2, 0
    jnz     short loc_21DEB
    mov     word_42D02, 0
    mov     word_449BC, 0
    mov     word_445D4, 0
loc_21DEB:
    mov     ax, word_44CEA
    cmp     word_44984, ax
    jz      short loc_21DFC
    mov     word_44984, ax
    call    sub_1A096
loc_21DFC:
    cmp     byte_46467, 0
    jz      short loc_21E76
    call    sub_29AEC
    call    sub_37216
    sub     ax, ax
    push    ax
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 3084h
    push    ax
    push    word_449A2
    push    word_449A0
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
    mov     si, ax
    cmp     si, 0FFFFh
    jnz     short loc_21E49
    sub     si, si
loc_21E49:
    call    sub_372F4
    mov     word_3F88E, 0
    call    sub_29B08
    or      si, si
    jz      short loc_21E71
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    call    sub_195E6
    add     sp, 4
    mov     byte_449DA, 1
loc_21E71:
    mov     byte_46467, 0
loc_21E76:
    cmp     byte_46436, 0
    jz      short loc_21E8A
    call    sub_3A9A0
    mov     al, byte_44346
    mov     byte_4432A, al
    jmp     short loc_21E8F
loc_21E8A:
    call    sub_28F6A
loc_21E8F:
    mov     al, byte_461C8
    cmp     byte_45DB2, al
    jnz     short loc_21EBF
    mov     al, byte_4644A
    cmp     byte_43950, al
    jnz     short loc_21EBF
    mov     al, byte_455D2
    cmp     byte_4616F, al
    jnz     short loc_21EBF
    mov     al, byte_45634
    cmp     byte_454B8, al
    jnz     short loc_21EBF
    mov     al, byte_42A38
    cmp     byte_463E0, al
    jnz     short loc_21EBF
    jmp     loc_21F7A
loc_21EBF:
    mov     al, byte_45DB2
    mov     byte_461C8, al
    mov     al, byte_43950
    mov     byte_4644A, al
    mov     al, byte_4616F
    mov     byte_455D2, al
    mov     al, byte_454B8
    mov     byte_45634, al
    mov     al, byte_463E0
    mov     byte_42A38, al
    mov     word_45DB8, 0
    mov     byte_449E2, 0
    cmp     byte_45DB2, 2
    jnz     short loc_21F0A
    cmp     byte_44AE2, 0
    jnz     short loc_21F0A
    cmp     byte_4616F, 0
    jz      short loc_21F00
    jmp     loc_21FEE
loc_21F00:
    cmp     byte_454B8, 0
    jz      short loc_21F0A
    jmp     loc_21FEE
loc_21F0A:
    mov     byte_46484, 0
loc_21F0F:
    cmp     byte_44AE2, 0
    jnz     short loc_21F19
    jmp     loc_21FF6
loc_21F19:
    mov     word_454AE, 0C8h ; 'È'
loc_21F1F:
    mov     ax, word_45DB8
    cmp     [bp+var_2], ax
    jnz     short loc_21F38
    mov     ax, word_449EA
    cmp     word_454AE, ax
    jnz     short loc_21F38
    mov     ax, word_459D4
    cmp     [bp+var_E], ax
    jz      short loc_21F7A
loc_21F38:
    mov     al, byte_459F1
    mov     byte_454A4, al
    push    word_454AE
    mov     ax, 140h
    push    ax
    mov     ax, word_454AE
    cwd
    mov     cx, 6
    idiv    cx
    push    ax
    mov     ax, 23h ; '#'
    push    ax
    call    sub_322F3
    add     sp, 8
    mov     ax, word_45DB8
    mov     word_43938, ax
    mov     ax, word_454AE
    mov     word_4393A, ax
    mov     ax, word_45DB8
    mov     [bp+var_2], ax
    mov     ax, word_454AE
    mov     word_449EA, ax
    mov     ax, word_459D4
    mov     [bp+var_E], ax
loc_21F7A:
    cmp     byte_454A4, 0
    jnz     short loc_21F84
    jmp     loc_22052
loc_21F84:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     byte ptr [bx-6D98h], 0
    cmp     byte_449E2, 0
    jz      short loc_21FB8
    push    word_459D4
    push    word_454AE
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr setup_car_shapes
    add     sp, 2
loc_21FB8:
    cmp     byte_46484, 0
    jnz     short loc_21FC2
    jmp     loc_22064
loc_21FC2:
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
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     short loc_22064
    ; align 2
    db 144
loc_21FEE:
    mov     byte_46484, 1
    jmp     loc_21F0F
loc_21FF6:
    cmp     byte_43950, 0
    jz      short loc_22034
    cmp     byte_463E0, 0
    jnz     short loc_22034
    cmp     byte_45DB2, 2
    jnz     short loc_2201A
    cmp     byte_46484, 0
    jz      short loc_2201A
    mov     word_459D4, 97h ; '—'
    jmp     short loc_22020
loc_2201A:
    mov     word_459D4, 0C8h ; 'È'
loc_22020:
    mov     byte_449E2, 1
    mov     ax, word_461CC
    mov     word_45DB8, ax
    mov     ax, word_45DBA
    mov     word_454AE, ax
    jmp     loc_21F1F
loc_22034:
    cmp     byte_45DB2, 2
    jz      short loc_2203E
    jmp     loc_21F19
loc_2203E:
    cmp     byte_46484, 0
    jnz     short loc_22048
    jmp     loc_21F19
loc_22048:
    mov     word_454AE, 97h ; '—'
    jmp     loc_21F1F
    ; align 2
    db 144
loc_22052:
    cmp     byte_46484, 0
    jnz     short loc_22064
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     byte ptr [bx-6D98h], 0
loc_22064:
    mov     ax, 81C4h
    push    ax
    mov     al, byte_44346
    cbw
    push    ax
    call    sub_1A0F4
    add     sp, 4
    cmp     word_461C2, 0
    jz      short loc_220DB
    cmp     byte_449E2, 0
    jz      short loc_220DB
    cmp     word_44984, 0
    jz      short loc_220BB
    mov     [bp+var_A], 0
    mov     [bp+var_8], 140h
    mov     ax, word_461C2
    mov     [bp+var_6], ax
    mov     ax, word_454AE
    mov     [bp+var_4], ax
    cmp     word_3B80A, 0
    jz      short loc_220BB
    push    word_3B80A
    lea     ax, [bp+var_A]
    push    ax
    push    word_3B80A
    call    sub_26572
    add     sp, 6
loc_220BB:
    push    word_454A2
    push    word_454A0
    call    sub_33B02
    add     sp, 4
    push    word_4549E
    push    word_4549C
    call    sub_342F6
    add     sp, 4
loc_220DB:
    mov     ax, 81C4h
    push    ax
    call    sub_19F14
    add     sp, 2
    cmp     byte_449E2, 0
    jz      short loc_22126
    push    word_459D4
    push    word_454AE
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr setup_car_shapes
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
loc_22126:
    cmp     byte_454A4, 0
    jz      short loc_22131
    dec     byte_454A4
loc_22131:
    cmp     byte_46436, 0
    jz      short loc_22152
    call    sub_28DB6
    call    sub_3A958
    xor     byte_44346, 1
    mov     al, byte_44346
    mov     byte_4432A, al
    call    sub_28D9E
loc_22152:
    cmp     byte_45DB2, 1
    jnz     short loc_2217D
    cmp     byte_4393C, 0
    jnz     short loc_2217D
    mov     byte_45DB2, 0
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     al, byte ptr word_44D4E
    mov     byte_449BA, al
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
loc_2217D:
    cmp     byte_44AE2, 0
    jz      short loc_221AA
    call    sub_30519
    or      ax, ax
    jz      short loc_22190
    jmp     loc_22298
loc_22190:
    cmp     byte_449DA, 0
    jz      short loc_2219A
    jmp     loc_22298
loc_2219A:
    call    sub_30538
    or      ax, ax
    jnz     short loc_221A6
    jmp     loc_21DA2
loc_221A6:
    jmp     loc_22298
    ; align 2
    db 144
loc_221AA:
    cmp     byte_449DA, 0
    jz      short loc_22208
    cmp     byte_45DB2, 0
    jnz     short loc_221C2
    cmp     byte_4488A, 4
    jz      short loc_221C2
    jmp     loc_22298
loc_221C2:
    cmp     byte_449DA, 2
    jnz     short loc_221CC
    jmp     loc_22298
loc_221CC:
    mov     byte_449DA, 0
    mov     byte_45DB2, 2
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_23A50
    add     sp, 2
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     byte_454B8, 1
    call    sub_188A4
loc_22208:
    cmp     byte_45DB2, 2
    jnz     short loc_22222
    sub     ax, ax
    push    ax
    push    ax
    mov     ax, 3
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_21DA2
    ; align 2
    db 144
loc_22222:
    call    sub_30519
    mov     [bp+var_12], ax
    or      ax, ax
    jz      short loc_22236
    push    ax
    push    cs
    call    near ptr sub_223FA
    add     sp, 2
loc_22236:
    mov     ax, [bp+var_12]
    cmp     ax, 4800h
    jz      short loc_22222
    cmp     ax, 4B00h
    jz      short loc_22222
    cmp     ax, 4D00h
    jz      short loc_22222
    cmp     ax, 5000h
    jz      short loc_22222
    cmp     byte_45DB2, 1
    jz      short loc_22257
    jmp     loc_21DA2
loc_22257:
    mov     ax, 0AA5Eh
    push    ax
    mov     ax, 0A9FCh
    push    ax
    mov     ax, 8B78h
    push    ax
    call    sub_36A60
    add     sp, 6
    test    byte ptr word_442E8, 3
    jnz     short loc_2227E
    call    sub_30538
    test    al, 30h
    jnz     short loc_2227E
    jmp     loc_21DA2
loc_2227E:
    mov     byte_45DB2, 0
    mov     byte_4393C, 0
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     al, byte ptr word_44D4E
    mov     byte_449BA, al
    jmp     loc_21C5E
    ; align 2
    db 144
loc_22298:
    cmp     byte_46436, 0
    jz      short loc_222D3
    call    sub_3A45C
    or      ax, ax
    jz      short loc_222D3
    call    sub_28DB6
    call    sub_3A9A0
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
    call    sub_3A958
    call    sub_28D9E
loc_222D3:
    call    sub_28F3C
    mov     byte_454B8, 1
    call    sub_188A4
    call    sub_26BD5
    cmp     byte_45DB2, 0
    jz      short loc_222F1
    jmp     loc_223CD
loc_222F1:
    cmp     opponent_index, 0
    jnz     short loc_222FB
    jmp     loc_223CD
loc_222FB:
    cmp     byte_4477F, 0
    jz      short loc_22305
    jmp     loc_223CD
loc_22305:
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_16]
    push    ax
    push    word_40800
    mov     ax, 50h ; 'P'
    push    ax
    mov     ax, 0FFFFh
    push    ax
    mov     ax, 3088h
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 3
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     byte_45D3E, 1
    mov     si, word_449D0
    dec     si
loc_22347:
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_22698
    add     sp, 2
    call    sub_17008
    inc     si
    mov     ax, word_449D0
    cmp     si, ax
    jnz     short loc_2239F
    sub     si, si
    mov     ax, 1
    push    ax              ; int
    mov     ax, word_445D4
    add     ax, word_45A24
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    call    sub_28DB6
    push    [bp+var_14]
    mov     ax, 0AC74h
    push    ax
    call    sub_29606
    add     sp, 2
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_345BC
    add     sp, 6
    call    sub_28D9E
loc_2239F:
    mov     ax, 1
    push    ax
    call    sub_289E2
    add     sp, 2
    cmp     ax, 1Bh
    jz      short loc_223CD
    cmp     byte_4477F, 0
    jnz     short loc_223CD
    mov     ax, 5DCh
    imul    word_449D0
    mov     cx, word_445D4
    add     cx, word_45A24
    cmp     ax, cx
    jz      short loc_223CD
    jmp     loc_22347
loc_223CD:
    mov     byte_45D3E, 0
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_23A50
    add     sp, 2
    push    cs
    call    near ptr sub_22576
    push    cs
    call    near ptr sub_239B4
loc_223E4:
    mov     word_44382, 64h ; 'd'
    call    check_input
    call    show_waiting
loc_223F4:
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
run_game endp
sub_223FA proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    mov     ax, [bp+arg_0]
    cmp     ax, 64h ; 'd'
    jz      short loc_22470
    jbe     short loc_2240A
    jmp     loc_224EE
loc_2240A:
    cmp     ax, 44h ; 'D'
    jz      short loc_22470
    jbe     short loc_22414
    jmp     loc_224AC
loc_22414:
    cmp     ax, 1Bh
    jz      short loc_22420
    cmp     ax, 43h ; 'C'
    jmp     loc_224BE
    ; align 2
    db 144
loc_22420:
    cmp     byte_45DB2, 0
    jnz     short loc_22436
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    call    sub_195E6
    add     sp, 4
loc_22436:
    mov     byte_449DA, 1
    jmp     loc_224E9
loc_2243E:
    mov     byte_3B8F5, 1
    jmp     loc_224E9
loc_22446:
    mov     byte_3B8F5, 2
    jmp     loc_224E9
loc_2244E:
    mov     byte_3B8F5, 3
    jmp     loc_224E9
loc_22456:
    xor     byte_3B8F4, 1
    jmp     loc_224E9
loc_2245E:
    call    sub_29DF4
    mov     al, byte_3B8F2
    cbw
    push    ax
    push    cs
    call    near ptr sub_23A50
    jmp     short loc_224E6
    ; align 4
    db 144
    db 144
loc_22470:
    xor     byte_43950, 1
    jmp     short loc_224E9
    ; align 2
    db 144
loc_22478:
    xor     byte_4616F, 1
    jmp     short loc_224E9
    ; align 2
    db 144
loc_22480:
    cmp     byte_45DB2, 1
    jz      short loc_224E9
    inc     byte_3B8F5
    cmp     byte_3B8F5, 4
    jnz     short loc_224E9
loc_22492:
    mov     byte_3B8F5, 0
    jmp     short loc_224E9
    ; align 2
    db 144
loc_2249A:
    cmp     opponent_index, 0
    jz      short loc_224E9
    xor     byte_463E0, 1
    jmp     short loc_224E9
loc_224A8:
    sub     ax, ax
    pop     bp
    retf
loc_224AC:
    cmp     ax, 48h ; 'H'
    jz      short loc_22456
    cmp     ax, 4Dh ; 'M'
    jz      short loc_2245E
    cmp     ax, 52h ; 'R'
    jz      short loc_22478
    cmp     ax, 63h ; 'c'
loc_224BE:
    jz      short loc_22480
loc_224C0:
    cmp     byte_45DB2, 1
    jnz     short loc_224A8
    mov     byte_45DB2, 0
    mov     byte_4393C, 0
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     al, byte ptr word_44D4E
    mov     byte_449BA, al
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
loc_224E6:
    add     sp, 2
loc_224E9:
    mov     ax, 1
    pop     bp
    retf
loc_224EE:
    cmp     ax, 74h ; 't'
    jz      short loc_2249A
    ja      short loc_22510
    cmp     ax, 68h ; 'h'
    jnz     short loc_224FD
    jmp     loc_22456
loc_224FD:
    cmp     ax, 6Dh ; 'm'
    jnz     short loc_22505
    jmp     loc_2245E
loc_22505:
    cmp     ax, 72h ; 'r'
    jnz     short loc_2250D
    jmp     loc_22478
loc_2250D:
    jmp     short loc_224C0
    ; align 2
    db 144
loc_22510:
    cmp     ax, 3B00h
    jnz     short loc_22518
    jmp     loc_22492
loc_22518:
    cmp     ax, 3C00h
    jnz     short loc_22520
    jmp     loc_2243E
loc_22520:
    cmp     ax, 3D00h
    jnz     short loc_22528
    jmp     loc_22446
loc_22528:
    cmp     ax, 3E00h
    jnz     short loc_22530
    jmp     loc_2244E
loc_22530:
    jmp     short loc_224C0
sub_223FA endp
sub_22532 proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     byte_44A8A, 1
    mov     byte_4552F, 2
    sub     si, si
    mov     word_42D02, si
    sub     al, al
    mov     byte_449DA, al
    mov     byte_4393C, al
    mov     word_44DCA, si
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_22532 endp
sub_2255A proc far

    mov     word_46468, 0
    mov     ax, 0A26h
    mov     dx, seg seg005
    push    dx
    push    ax
    call    sub_302AA
    add     sp, 4
    mov     byte_442E4, 0
    retf
sub_2255A endp
sub_22576 proc far

    mov     ax, 0Ah
    cwd
    push    dx
    push    ax
    call    sub_32805
    add     sp, 4
    mov     ax, 0A26h
    mov     dx, seg seg005
    push    dx
    push    ax
    call    sub_302DE
    add     sp, 4
    retf
    ; align 2
    db 144
    call    sub_3031D
    or      ax, ax
    jnz     short loc_225A2
    jmp     locret_22696
loc_225A2:
    cmp     byte_442E4, 0
    jz      short loc_225AC
    jmp     locret_22696
loc_225AC:
    inc     byte_442E4
    cmp     byte_442E4, 1
    jz      short loc_225BA
    jmp     loc_22692
loc_225BA:
    inc     word_443F4
    mov     ax, word_4499C
    cmp     word_443F4, ax
    jl      short loc_225FE
    mov     ax, word_449E4
    cmp     word_44D1E, ax
    jz      short loc_225FE
    push    word_443F4
    mov     ax, 22h ; '"'
    imul    word_44D1E
    add     ax, 97DCh
    push    ax
    call    sub_18D06
    add     sp, 4
    mov     word_443F4, 0
    inc     word_44D1E
    cmp     word_44D1E, 28h ; '('
    jnz     short loc_225FE
    mov     word_44D1E, 0
loc_225FE:
    cmp     byte_449DA, 0
    jz      short loc_22608
    jmp     loc_22692
loc_22608:
    cmp     byte_46467, 0
    jz      short loc_22612
    jmp     loc_22692
loc_22612:
    cmp     byte_454B8, 0
    jz      short loc_22620
    cmp     byte_45DB2, 2
    jz      short loc_22692
loc_22620:
    cmp     byte_45DB2, 0
    jnz     short loc_2263E
    mov     ax, word_445CE
    cmp     word_445CC, ax
    jl      short loc_2263E
    mov     byte_454B8, 1
    call    sub_188A4
    jmp     short loc_22692
    db 144
    db 144
loc_2263E:
    dec     byte_44A8A
    jnz     short loc_22692
    mov     al, byte ptr word_4499C
    mov     byte_44A8A, al
    inc     word_46468
    cmp     byte_45DB2, 2
    jnz     short loc_22688
    mov     al, byte_449E6
    cbw
    cmp     ax, 2
    jz      short loc_22666
    cmp     ax, 3
    jz      short loc_2267E
    jmp     short loc_22688
    ; align 2
    db 144
loc_22666:
    dec     byte_4552F
    jnz     short loc_22692
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_22698
    add     sp, 2
    mov     byte_4552F, 2
    jmp     short loc_22692
    ; align 2
    db 144
loc_2267E:
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_22698
    add     sp, 2
loc_22688:
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_22698
    add     sp, 2
loc_22692:
    dec     byte_442E4
locret_22696:
    retf
    ; align 2
    db 144
sub_22576 endp
sub_22698 proc far
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    cmp     [bp+arg_0], 0
    jz      short loc_226AC
loc_226A6:
    sub     si, si
    jmp     loc_22817
    ; align 2
    db 144
loc_226AC:
    cmp     byte_45DB2, 2
    jnz     short loc_226E6
    mov     ax, word_42D02
    cmp     word_449BC, ax
    jbe     short loc_226C6
    inc     word_42D02
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_226C6:
    cmp     byte_449DA, 0
    jz      short loc_226D0
    jmp     loc_22985
loc_226D0:
    mov     byte_454B8, 1
    call    sub_188A4
loc_226DA:
    mov     byte_449DA, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_226E6:
    cmp     byte_449DA, 0
    jnz     short loc_226A6
    cmp     byte_4488A, 0
    jnz     short loc_226A6
    cmp     byte_45DB2, 1
    jz      short loc_226A6
    cmp     byte_45E1A, 0
    jnz     short loc_22725
    cmp     byte_4393C, 0
    jnz     short loc_22725
    mov     ax, word_449D0
    shl     ax, 1
    shl     ax, 1
    cmp     ax, word_445D4
    jnb     short loc_22725
    sub     ax, ax
    push    ax
    mov     ax, 1
    push    ax
    call    sub_195E6
    add     sp, 4
loc_22725:
    cmp     byte_3B8F2, 0
    jnz     short loc_22736
    cmp     byte_3FE00, 0
    jnz     short loc_22736
    jmp     loc_227E8
loc_22736:
    cmp     byte_3B8F2, 0
    jz      short loc_2279A
    mov     ax, 0AA5Eh
    push    ax
    mov     ax, 0A9FCh
    push    ax
    mov     ax, 8B78h
    push    ax
    call    sub_36A60
    add     sp, 6
    mov     di, word_4616C
    sub     di, 0A0h ; ' '
    push    di              ; int
    call    _abs
    add     sp, 2
    cmp     ax, 12h
    jge     short loc_2276C
    sub     di, di
    jmp     short loc_22779
    ; align 2
    db 144
loc_2276C:
    or      di, di
    jle     short loc_22776
    sub     di, 12h
    jmp     short loc_22779
    ; align 2
    db 144
loc_22776:
    add     di, 12h
loc_22779:
    mov     ax, di
    mov     byte_40D6A, al
    test    byte ptr word_442E8, 1
    jz      short loc_2278A
    mov     si, 2
    jmp     short loc_227D2
loc_2278A:
    test    byte ptr word_442E8, 2
    jz      short loc_22796
    mov     si, 1
    jmp     short loc_227D2
loc_22796:
    sub     si, si
    jmp     short loc_227D2
loc_2279A:
    call    sub_307E3
    mov     byte_40D6A, al
    or      al, al
    jle     short loc_227B0
    cbw
    mov     bx, ax
    mov     al, [bx+30ECh]
    jmp     short loc_227C5
    ; align 2
    db 144
loc_227B0:
    cmp     byte_40D6A, 0
    jge     short loc_227C8
    mov     al, byte_40D6A
    cbw
    mov     bx, ax
    neg     bx
    mov     al, [bx+30ECh]
    neg     al
loc_227C5:
    mov     byte_40D6A, al
loc_227C8:
    call    sub_30538
    mov     si, ax
smart
    and     si, 33h
nosmart
loc_227D2:
    mov     di, word_42D02
smart
    and     di, 3Fh
nosmart
    mov     al, byte_40D6A
    mov     [di-74DEh], al
    mov     byte ptr [di-7486h], 1
    jmp     short loc_227EF
    ; align 2
    db 144
loc_227E8:
    call    sub_30538
    mov     si, ax
loc_227EF:
    mov     ax, 1Eh
    push    ax
    call    sub_30A0D
    add     sp, 2
    or      ax, ax
    jz      short loc_22803
smart
    nop
    or      si, 10h
nosmart
loc_22803:
    mov     ax, 2Ch ; ','
    push    ax
    call    sub_30A0D
    add     sp, 2
    or      ax, ax
    jz      short loc_22817
smart
    nop
    or      si, 20h
nosmart
loc_22817:
    mov     ax, 5DCh
    imul    word_449D0
    mov     cx, word_42D02
    add     cx, word_45A24
    cmp     ax, cx
    ja      short loc_2283C
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    call    sub_195E6
    add     sp, 4
    jmp     loc_226DA
loc_2283C:
    cmp     word_42D02, 2EE0h
    jz      short loc_22847
    jmp     loc_2296C
loc_22847:
    cmp     word_45A24, 0
    jnz     short loc_22866
    cmp     byte_45D3E, 0
    jnz     short loc_22866
    mov     byte_45D3E, 1
    mov     byte_46467, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_22866:
    sub     di, di
    jmp     loc_22909
    ; align 2
    db 144
loc_2286C:
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, di
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, 5A0h
    adc     dx, 0
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     es, dx
    mov     bx, ax
    mov     ax, [bp+var_6]
    sub     es:[bx], ax
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, di
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, 460h
    adc     dx, 0
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     cx, 460h
    sub     bx, bx
    push    bx
    push    cx
    mov     cx, ax
    mov     ax, di
    mov     bx, dx
    cwd
    push    dx
    push    ax
    mov     [bp+var_A], cx
    mov     [bp+var_8], bx
    call    __aFlmul
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     bx, ax
    mov     es, dx
    mov     ax, [bp+var_A]
    mov     dx, [bp+var_8]
    push    si
    push    di
    mov     di, bx
    mov     si, ax
    push    ds
    mov     ds, dx
    mov     cx, 230h
    repne movsw
    pop     ds
    pop     di
    pop     si
    inc     di
loc_22909:
    mov     ax, 1Eh
    imul    word_449D0
    mov     [bp+var_6], ax
    mov     ax, 2EE0h
    cwd
    mov     cx, [bp+var_6]
    idiv    cx
    dec     ax
    cmp     ax, di
    jle     short loc_22924
    jmp     loc_2286C
loc_22924:
    sub     di, di
    jmp     short loc_2293E
loc_22928:
    mov     bx, [bp+var_A]
    add     bx, word ptr dword_4562E
    mov     es, word ptr dword_4562E+2
    mov     al, es:[bx+di]
    mov     bx, word ptr dword_4562E
    mov     es:[bx+di], al
    inc     di
loc_2293E:
    mov     ax, 1Eh
    imul    word_449D0
    mov     [bp+var_A], ax
    mov     ax, 2EE0h
    sub     ax, [bp+var_A]
    cmp     ax, di
    jg      short loc_22928
    mov     ax, 1Eh
    imul    word_449D0
    mov     [bp+var_A], ax
    sub     word_42D02, ax
    sub     word_449BC, ax
    add     word_45A24, ax
    sub     word_445D4, ax
loc_2296C:
    mov     bx, word_42D02
    inc     word_42D02
    add     bx, word ptr dword_4562E
    mov     es, word ptr dword_4562E+2
    mov     ax, si
    mov     es:[bx], al
    inc     word_449BC
loc_22985:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_22698 endp
sub_2298C proc far
    var_34 = dword ptr -52
    var_30 = word ptr -48
    var_2E = word ptr -46
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
    var_14 = word ptr -20
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 34h
    push    di
    push    si
    mov     [bp+var_2A], 1
    cmp     opponent_index, 0
    jz      short loc_229A5
    mov     [bp+var_2A], 2
loc_229A5:
    sub     si, si
    jmp     loc_22C53
loc_229AA:
    mov     [bp+var_24], 8F46h
loc_229AF:
    mov     bx, [bp+var_24]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_229BA:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_229BA
    mov     [bp+var_1A], ax
    mov     bx, [bp+var_24]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_229CF:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_229CF
    mov     [bp+var_1C], ax
    mov     bx, [bp+var_24]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_229E5:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_229E5
    mov     [bp+var_18], ax
    mov     bx, [bp+var_24]
    push    si
    push    di
    lea     di, [bp+var_6]
    lea     si, [bx+0A4h]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [bp+var_24]
    mov     ax, [bx+48h]
    mov     [bp+var_E], ax
    or      si, si
    jnz     short loc_22A1E
    cmp     byte_448EF, 0
    jnz     short loc_22A40
    cmp     byte_448F0, 0
    jnz     short loc_22A40
loc_22A1E:
    cmp     word ptr [bx+0B6h], 0
    jnz     short loc_22A40
    cmp     byte ptr [bx+0C9h], 0
    jnz     short loc_22A40
    cmp     word ptr [bx+4Ah], 0FFFFh
    jz      short loc_22A40
    cmp     [bp+var_E], 80h ; '€'
    jle     short loc_22A4D
    cmp     [bp+var_E], 380h
    jge     short loc_22A4D
loc_22A40:
    push    si
    push    di
    lea     di, [bp+var_6]
    lea     si, [bp+var_1C]
    movsw
    movsw
    movsw
    pop     di
    pop     si
loc_22A4D:
    mov     [bp+var_22], 1C2h
    mov     ax, [bp+var_1A]
    add     ax, 10Eh
    mov     [bp+var_14], ax
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     ax, [bx-71BAh]
    sub     ax, [bp+var_14]
    mov     [bp+var_C], ax
    or      ax, ax
    jz      short loc_22A96
    mov     di, ax
    cmp     di, 1Eh
    jle     short loc_22A80
    mov     di, 1Eh
    jmp     short loc_22A88
    ; align 2
    db 144
loc_22A80:
    cmp     di, 0FFE2h
    jge     short loc_22A88
    mov     di, 0FFE2h
loc_22A88:
    mov     bx, si
    mov     ax, bx
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    sub     [bx-71BAh], di
loc_22A96:
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_2E], ax
    mov     ax, [bp+var_2]
    mov     bx, [bp+var_2E]
    sub     ax, [bx-71B8h]
    push    ax
    mov     ax, [bp+var_6]
    sub     ax, [bx-71BCh]
    push    ax
    call    sub_2EA4E
    add     sp, 4
    mov     [bp+var_10], ax
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_30], ax
    mov     ax, [bp+var_18]
    mov     bx, [bp+var_30]
    sub     ax, [bx-71B8h]
    push    ax
    mov     ax, [bp+var_1C]
    sub     ax, [bx-71BCh]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     di, ax
    cmp     [bp+var_22], di
    jge     short loc_22B53
    sub     di, [bp+var_22]
    cmp     word_449D0, 14h
    jnz     short loc_22B04
    cmp     di, 78h ; 'x'
    jle     short loc_22B0D
    mov     di, 78h ; 'x'
    jmp     short loc_22B0D
loc_22B04:
    cmp     di, 0F0h ; 'ð'
    jle     short loc_22B0D
    mov     di, 0F0h ; 'ð'
loc_22B0D:
    push    [bp+var_10]
    call    sin_fast
    add     sp, 2
    push    ax
    push    di
    call    sub_30044
    add     sp, 4
    mov     bx, si
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
    add     [bx-71BCh], ax
    push    [bp+var_10]
    call    cos_fast
    add     sp, 2
    push    ax
    push    di
    call    sub_30044
    add     sp, 4
    mov     bx, si
    mov     cx, bx
    shl     bx, 1
    add     bx, cx
    shl     bx, 1
    add     [bx-71B8h], ax
loc_22B53:
    mov     ax, word_445D4
    sub     dx, dx
    mov     cx, word_449D0
    sar     cx, 1
    div     cx
    or      dx, dx
    jz      short loc_22B67
    jmp     loc_22C52
loc_22B67:
    mov     [bp+var_2C], 2710h
    mov     [bp+var_A], 0
    jmp     short loc_22BE3
    ; align 4
    db 144
    db 144
loc_22B74:
    mov     ax, [bp+var_20]
    mov     dx, [bp+var_1E]
loc_22B7A:
    mov     cx, ax
    mov     ax, [bp+var_2C]
    mov     bx, dx
    cwd
    cmp     bx, dx
    jg      short loc_22BE0
    jl      short loc_22B8C
    cmp     cx, ax
    jnb     short loc_22BE0
loc_22B8C:
    cmp     [bp+var_26], 0
    jge     short loc_22BA2
    mov     ax, [bp+var_28]
    mov     dx, [bp+var_26]
    neg     ax
    adc     dx, 0
    neg     dx
    jmp     short loc_22BA8
    ; align 2
    db 144
loc_22BA2:
    mov     ax, [bp+var_28]
    mov     dx, [bp+var_26]
loc_22BA8:
    mov     cx, ax
    mov     ax, [bp+var_2C]
    mov     bx, dx
    cwd
    cmp     bx, dx
    jg      short loc_22BE0
    jl      short loc_22BBA
    cmp     cx, ax
    jnb     short loc_22BE0
loc_22BBA:
    push    [bp+var_28]
    push    [bp+var_20]
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_8], ax
    mov     ax, [bp+var_2C]
    cmp     [bp+var_8], ax
    jge     short loc_22BE0
    mov     al, [bp+var_A]
    mov     [si-6EE5h], al
    mov     ax, [bp+var_8]
    mov     [bp+var_2C], ax
loc_22BE0:
    inc     [bp+var_A]
loc_22BE3:
    mov     al, byte_4616E
    cmp     [bp+var_A], al
    jge     short loc_22C52
    mov     al, [bp+var_A]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word_449DC
    mov     dx, word_449DE
    mov     word ptr [bp+var_34], ax
    mov     word ptr [bp+var_34+2], dx
    mov     ax, [bp+var_1C]
    cwd
    les     bx, [bp+var_34]
    mov     cx, ax
    mov     ax, es:[bx]
    mov     bx, dx
    cwd
    sub     ax, cx
    sbb     dx, bx
    mov     [bp+var_20], ax
    mov     [bp+var_1E], dx
    mov     ax, [bp+var_18]
    cwd
    mov     bx, word ptr [bp+var_34]
    mov     cx, ax
    mov     ax, es:[bx+4]
    mov     bx, dx
    cwd
    sub     ax, cx
    sbb     dx, bx
    mov     [bp+var_28], ax
    mov     [bp+var_26], dx
    cmp     [bp+var_1E], 0
    jl      short loc_22C41
    jmp     loc_22B74
loc_22C41:
    mov     ax, [bp+var_20]
    mov     dx, [bp+var_1E]
    neg     ax
    adc     dx, 0
    neg     dx
    jmp     loc_22B7A
    ; align 2
    db 144
loc_22C52:
    inc     si
loc_22C53:
    cmp     [bp+var_2A], si
    jle     short loc_22C8C
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_2E], ax
    mov     bx, ax
    lea     ax, [bx-71BCh]
    mov     bx, [bp+var_2E]
    push    si
    push    di
    lea     di, [bx-71B0h]
    mov     si, ax
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    or      si, si
    jz      short loc_22C84
    jmp     loc_229AA
loc_22C84:
    mov     [bp+var_24], 8E76h
    jmp     loc_229AF
loc_22C8C:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_2298C endp
sub_22C92 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    push    di
    push    si
    mov     ax, 95F8h
    push    ax              ; char *
    mov     ax, 310Eh
    push    ax              ; int
    push    word ptr [bp+arg_0+2]
    push    word ptr [bp+arg_0]; char *
    call    combine_file_path
    add     sp, 8
    mov     byte_3B8FB, 1
    push    word ptr dword_46162+2
    push    word ptr dword_46162
    mov     ax, 95F8h
    push    ax
    call    load_binary_file
    add     sp, 6
    mov     ax, word ptr dword_46162
    mov     dx, word ptr dword_46162+2
    mov     di, 9234h
    mov     si, ax
    push    ds
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 0Dh
    repne movsw
    pop     ds
    mov     byte_3B8FB, 0
    sub     ax, ax
    pop     si
    pop     di
    pop     bp
    retf
sub_22C92 endp
sub_22CE8 proc far
    var_6 = word ptr -6
    var_4 = word ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    les     bx, dword_46162
    push    si
    mov     di, bx
    mov     si, 9234h
    mov     cx, 0Dh
    repne movsw
    pop     si
    mov     ax, word_449BC
    add     ax, 724h
    cwd
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
    mov     byte_3B8FB, 1
    push    dx
    push    ax
    push    es
    push    bx
    push    [bp+arg_0]
    call    sub_3250B
    add     sp, 0Ah
    mov     byte_3B8FB, 0
    cbw
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_22CE8 endp
setup_car_shapes proc far
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = byte ptr -28
    var_1A = byte ptr -26
    var_18 = byte ptr -24
    var_16 = byte ptr -22
    var_14 = byte ptr -20
    var_E = word ptr -14
    var_C = dword ptr -12
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 20h
    push    di
    push    si
    mov     ax, [bp+arg_0]
    or      ax, ax
    jz      short loc_22D5C
    cmp     ax, 1
    jnz     short loc_22D45
    jmp     loc_22F76
loc_22D45:
    cmp     ax, 2
    jnz     short loc_22D4D
    jmp     loc_23030
loc_22D4D:
    cmp     ax, 3
    jnz     short loc_22D55
    jmp     loc_236AC
loc_22D55:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_22D5C:
    mov     al, player_car_id
    mov     byte_3E916, al
    mov     al, byte_449A5
    mov     byte_3E917, al
    mov     al, byte_449A6
    mov     byte_3E918, al
    mov     al, byte_449A7
    mov     byte_3E919, al
    mov     al, player_car_id
    mov     byte_3E920, al
    mov     al, byte_449A5
    mov     byte_3E921, al
    mov     al, byte_449A6
    mov     byte_3E922, al
    mov     al, byte_449A7
    mov     byte_3E923, al
    mov     ax, 31A2h
    push    ax              ; char *
    mov     ax, 3
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_40D7C, ax
    mov     word_40D7E, dx
    mov     ax, 31ACh
    push    ax              ; char *
    mov     ax, 2
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_40D84, ax
    mov     word_40D86, dx
    mov     ax, 5640h
    push    ax
    mov     ax, 3113h
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_many_resources
    add     sp, 8
    mov     ax, 5664h
    push    ax
    mov     ax, 3138h
    push    ax
    push    word_40D86
    push    word_40D84
    call    locate_many_resources
    add     sp, 8
    cmp     word_45F84, 0
    jnz     short loc_22E09
    mov     ax, 5618h
    push    ax
    mov     ax, 3151h
    push    ax
    push    word_40D86
    push    word_40D84
    call    locate_many_resources
    add     sp, 8
loc_22E09:
    mov     ax, 0Fh
    push    ax
    les     bx, dword_40DBC
    push    word ptr es:[bx+2]
    mov     ax, es:[bx]
    imul    word_44AE4
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_40D80, ax
    mov     word ptr dword_40D80+2, dx
    mov     ax, 0Fh
    push    ax
    les     bx, dword_40DC0
    push    word ptr es:[bx+2]
    mov     ax, es:[bx]
    imul    word_44AE4
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_40DEC, ax
    mov     word ptr dword_40DEC+2, dx
    mov     ax, 0Fh
    push    ax
    les     bx, dword_40DC0
    push    word ptr es:[bx+2]
    mov     ax, es:[bx]
    imul    word_44AE4
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_40DFC, ax
    mov     word ptr dword_40DFC+2, dx
    mov     ax, 317Ah
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_shape
    add     sp, 6
    mov     word ptr [bp+var_C], ax
    mov     word ptr [bp+var_C+2], dx
    push    word ptr dword_40DFC+2
    push    word ptr dword_40DFC
    call    sub_35AF6
    add     sp, 4
    les     bx, [bp+var_C]
    mov     ax, es:[bx+0Ah]
    les     bx, dword_40DC0
    sub     ax, es:[bx+0Ah]
    push    ax
    les     bx, [bp+var_C]
    mov     ax, es:[bx+8]
    les     bx, dword_40DC0
    sub     ax, es:[bx+8]
    push    ax
    push    word ptr [bp+var_C+2]
    push    word ptr [bp+var_C]
    call    sub_33EB4
    add     sp, 8
    call    set_sprite2_as_1
    les     bx, [bp+var_C]
    mov     ax, es:[bx+0Ah]
    mov     word_45DBA, ax
    mov     ax, 317Fh
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    sub_30F92
    add     sp, 6
    or      dx, ax
    jz      short loc_22F12
    mov     ax, 3184h
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_shape
    add     sp, 6
    mov     bx, ax
    mov     es, dx
    mov     ax, es:[bx+2]
    mov     word_461CC, ax
    jmp     short loc_22F18
loc_22F12:
    mov     word_461CC, 0
loc_22F18:
    mov     ax, 3189h
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    sub_30F92
    add     sp, 6
    mov     word ptr [bp+var_C], ax
    mov     word ptr [bp+var_C+2], dx
    or      ax, dx
    jz      short loc_22F6A
    les     bx, [bp+var_C]
    mov     ax, es:[bx+0Ah]
    mov     word_461C2, ax
    mov     ax, bx
    mov     word_4549C, ax
    mov     word_4549E, dx
    mov     ax, 318Eh
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_shape
    add     sp, 6
    mov     word_454A0, ax
    mov     word_454A2, dx
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_22F6A:
    mov     word_461C2, 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_22F76:
    call    sub_28DB6
    mov     ax, 3193h
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    sub_30F92
    add     sp, 6
    or      dx, ax
    jz      short loc_22FB1
    mov     ax, 3198h
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_shape
    add     sp, 6
    push    dx
    push    ax
    call    sub_33E00
    add     sp, 4
loc_22FB1:
    mov     ax, 319Dh
    push    ax
    push    word_40D7E
    push    word_40D7C
    call    locate_shape
    add     sp, 6
    push    dx
    push    ax
    call    sub_33ED2
    add     sp, 4
    push    word_40DB6
    push    word_40DB4
    call    sub_33ED2
    add     sp, 4
    call    sub_28D9E
    sub     si, si
    mov     al, byte_4432A
    cbw
    mov     [bp+var_1E], ax
    mov     bx, ax
    sub     al, al
    mov     [bx-6D98h], al
    mov     bx, [bp+var_1E]
    mov     [bx+568Ah], al
    mov     ax, [bp+var_1E]
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    mov     [bx+5686h], si
    mov     bx, [bp+var_1E]
    sub     al, al
    mov     [bx+5680h], al
    dec     si
    mov     bx, [bp+var_20]
    mov     [bx+5690h], si
    mov     bx, [bp+var_20]
    mov     [bx+5608h], si
    mov     bx, [bp+var_20]
    mov     [bx+55FCh], si
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_23030:
    mov     al, byte_446B1
    cbw
    mov     cx, ax
    mov     al, byte_446B0
    cbw
    or      ax, cx
    jnz     short loc_2309A
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    cmp     byte ptr [bx+568Ah], 0
    jz      short loc_2309A
    cmp     byte_46436, 0
    jnz     short loc_23057
    call    sub_28DB6
loc_23057:
    push    word_459D4
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    les     bx, dword_40DC0
    push    word ptr es:[bx+0Ah]
    push    word ptr es:[bx+8]
    les     bx, dword_40DFC
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
    add     sp, 8
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     byte ptr [bx+568Ah], 0
    jmp     loc_2319D
loc_2309A:
    mov     al, byte_4432A
    cbw
    mov     [bp+var_20], ax
    mov     bx, ax
    mov     al, byte_446B0
    cmp     [bx+568Ah], al
    jnz     short loc_230DE
    mov     ax, bx
    shl     ax, 1
    mov     [bp+var_1E], ax
    mov     bx, ax
    mov     ax, word_4461A
    cmp     [bx+5600h], ax
    jnz     short loc_230DE
    mov     ax, word_4461E
    cmp     [bx+5604h], ax
    jnz     short loc_230DE
    cmp     byte_446B1, 0
    jnz     short loc_230D1
    jmp     loc_2319D
loc_230D1:
    mov     bx, [bp+var_20]
    cmp     byte ptr [bx+568Ah], 0
    jz      short loc_230DE
    jmp     loc_2319D
loc_230DE:
    push    word ptr dword_40DEC+2
    push    word ptr dword_40DEC
    call    sub_35AF6
    add     sp, 4
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     byte ptr [bx+568Ah], 1
    sub     ax, ax
    push    ax
    push    ax
    push    word ptr dword_40DC0+2
    push    word ptr dword_40DC0
    call    sub_33EB4
    add     sp, 8
    mov     si, word_4461A
    mov     di, word_4461E
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    mov     [bx+5600h], si
    mov     bx, [bp+var_20]
    mov     [bx+5604h], di
    push    di
    push    si
    push    word_40DDA
    push    word_40DD8
    call    sub_3386C
    add     sp, 8
    push    di
    push    si
    push    word_40DD6
    push    word_40DD4
    call    sub_34060
    add     sp, 8
    cmp     byte_46436, 0
    jz      short loc_2315E
    call    sub_3A9A0
    jmp     short loc_23168
    ; align 2
    db 144
loc_2315E:
    call    sub_28F3C
    call    sub_28DB6
loc_23168:
    push    word_459D4
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    les     bx, dword_40DC0
    push    word ptr es:[bx+0Ah]
    push    word ptr es:[bx+8]
    les     bx, dword_40DEC
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
    add     sp, 8
loc_2319D:
    mov     [bp+var_1A], 0
    mov     ax, word_44606
    cwd
    xor     ax, dx
    sub     ax, dx
    mov     cx, 3
    sar     ax, cl
    xor     ax, dx
    sub     ax, dx
    mov     [bp+var_4], ax
    mov     [bp+var_2], 1
    cmp     ax, 0FFF6h
    jge     short loc_231C4
    mov     [bp+var_2], 0
    jmp     short loc_231CE
loc_231C4:
    cmp     [bp+var_4], 0Ah
    jle     short loc_231CE
    mov     [bp+var_2], 2
loc_231CE:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     al, [bp+var_2]
    cmp     [bx+5680h], al
    jnz     short loc_231E7
    cmp     byte_454A4, 0
    jnz     short loc_231E7
    jmp     loc_23286
loc_231E7:
    cmp     byte_46436, 0
    jnz     short loc_231F3
    call    sub_28DB6
loc_231F3:
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    cmp     word ptr [bx+5686h], 0
    jz      short loc_23239
    push    word ptr [bx+5686h]
    push    word ptr [bx+5682h]
    mov     al, byte_44346
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+5676h]
    push    word ptr [bx+5674h]
    call    sub_33BBC
    add     sp, 8
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     word ptr [bx+5686h], 0
    mov     [bp+var_1A], 1
loc_23239:
    mov     al, [bp+var_2]
    cbw
    or      ax, ax
    jz      short loc_2324E
    cmp     ax, 1
    jz      short loc_23272
    cmp     ax, 2
    jz      short loc_2327C
    jmp     short loc_2325E
    ; align 2
    db 144
loc_2324E:
    push    word_40DB2
    push    word_40DB0
loc_23256:
    call    sub_33ED2
    add     sp, 4
loc_2325E:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     al, [bp+var_2]
    mov     [bx+5680h], al
    mov     [bp+var_18], 1
    jmp     short loc_2328A
    ; align 2
    db 144
loc_23272:
    push    word_40DB6
    push    word_40DB4
    jmp     short loc_23256
loc_2327C:
    push    word_40DBA
    push    word_40DB8
    jmp     short loc_23256
loc_23286:
    mov     [bp+var_18], 0
loc_2328A:
    mov     ax, word_45F84
    cmp     ax, 0FFFFh
    jz      short loc_232B6
    or      ax, ax
    jnz     short loc_23299
    jmp     loc_233A2
loc_23299:
    mov     [bp+var_6], 0
    mov     ax, word_44610
    sub     dx, dx
    mov     cx, 280h
    div     cx
    mov     si, ax
    cmp     word_45F86, si
    jg      short loc_232BC
    mov     si, word_45F86
    dec     si
    jmp     short loc_232BC
loc_232B6:
    sub     si, si
    mov     [bp+var_6], 2
loc_232BC:
    mov     ax, word_44608
    mov     cl, 7
    shr     ax, cl
    mov     di, ax
    cmp     word_4605C, di
    jg      short loc_232D0
    mov     di, word_4605C
    dec     di
loc_232D0:
    cmp     [bp+var_18], 0
    jnz     short loc_232F7
    cmp     byte_454A4, 0
    jnz     short loc_232F7
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    cmp     [bx+5608h], si
    jnz     short loc_232F7
    cmp     [bx+55FCh], di
    jnz     short loc_232F7
    jmp     loc_23540
loc_232F7:
    cmp     byte_46436, 0
    jnz     short loc_23303
    call    sub_28DB6
loc_23303:
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    cmp     word ptr [bx+5686h], 0
    jz      short loc_23349
    push    word ptr [bx+5686h]
    push    word ptr [bx+5682h]
    mov     al, byte_44346
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+5676h]
    push    word ptr [bx+5674h]
    call    sub_33BBC
    add     sp, 8
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     word ptr [bx+5686h], 0
    mov     [bp+var_1A], 1
loc_23349:
    push    word ptr dword_40D80+2
    push    word ptr dword_40D80
    call    sub_35AF6
    add     sp, 4
    sub     ax, ax
    push    ax
    push    ax
    push    word ptr dword_40DBC+2
    push    word ptr dword_40DBC
    call    sub_33DE2
    add     sp, 8
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    mov     [bx+5608h], si
    mov     bx, [bp+var_20]
    mov     [bx+55FCh], di
    cmp     [bp+var_6], 1
    jz      short loc_2338C
    jmp     loc_23456
loc_2338C:
    mov     [bp+var_8], 0
    cmp     si, 0C8h ; 'È'
    jl      short loc_233B2
    mov     [bp+var_8], 2
    sub     si, 0C8h ; 'È'
    jmp     short loc_233BF
loc_233A2:
    mov     [bp+var_6], 1
    mov     ax, word_44610
    mov     cl, 8
    shr     ax, cl
    mov     si, ax
    jmp     loc_232BC
loc_233B2:
    cmp     si, 64h ; 'd'
    jl      short loc_233BF
    mov     [bp+var_8], 1
    sub     si, 64h ; 'd'
loc_233BF:
    cmp     [bp+var_8], 0
    jz      short loc_233EA
    mov     al, byte_45F89
    sub     ah, ah
    push    ax
    mov     al, byte_45F88
    push    ax
    mov     bx, [bp+var_8]
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+561Ah]
    push    word ptr [bx+5618h]
    call    sub_34084
    add     sp, 8
    mov     [bp+var_1C], 1
loc_233EA:
    mov     ax, si
    cwd
    mov     cx, 0Ah
    idiv    cx
    mov     [bp+var_8], ax
    or      ax, ax
    jnz     short loc_233FF
    cmp     [bp+var_1C], 0
    jz      short loc_23433
loc_233FF:
    mov     al, byte_45F8B
    sub     ah, ah
    push    ax
    mov     al, byte_45F8A
    push    ax
    mov     bx, [bp+var_8]
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+561Ah]
    push    word ptr [bx+5618h]
    call    sub_34084
    add     sp, 8
    mov     ax, [bp+var_8]
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    sub     si, ax
    mov     [bp+var_1C], 1
loc_23433:
    mov     al, byte_45F8D
    sub     ah, ah
    push    ax
    mov     al, byte_45F8C
    push    ax
    mov     bx, si
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+561Ah]
    push    word ptr [bx+5618h]
    call    sub_34084
    add     sp, 8
    jmp     short loc_23485
    ; align 2
    db 144
loc_23456:
    cmp     [bp+var_6], 0
    jnz     short loc_23485
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_20], ax
    push    word_44D60
    mov     bx, ax
    mov     al, [bx-57E7h]
    sub     ah, ah
    push    ax
    mov     al, [bx-57E8h]
    push    ax
    push    word_45F84
    push    word_45F82
    call    sub_2FDDE
    add     sp, 0Ah
loc_23485:
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_20], ax
    push    word_44D60
    mov     bx, ax
    mov     al, [bx-5711h]
    sub     ah, ah
    push    ax
    mov     al, [bx-5712h]
    push    ax
    push    word_4605A
    push    word_46058
    call    sub_2FDDE
    add     sp, 0Ah
    mov     al, [bp+var_2]
    cbw
    or      ax, ax
    jz      short loc_234BE
    cmp     ax, 2
    jz      short loc_234EC
    jmp     short loc_234DE
    ; align 2
    db 144
loc_234BE:
    push    word_40DCE
    push    word_40DCC
    call    sub_33B02
    add     sp, 4
    push    word_40DC6
    push    word_40DC4
loc_234D6:
    call    sub_342F6
    add     sp, 4
loc_234DE:
    cmp     byte_46436, 0
    jz      short loc_23506
    call    sub_3A9A0
    jmp     short loc_2350B
loc_234EC:
    push    word_40DD2
    push    word_40DD0
    call    sub_33B02
    add     sp, 4
    push    word_40DCA
    push    word_40DC8
    jmp     short loc_234D6
loc_23506:
    call    sub_28F3C
loc_2350B:
    push    word_459D4
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    les     bx, dword_40DBC
    push    word ptr es:[bx+0Ah]
    push    word ptr es:[bx+8]
    les     bx, dword_40D80
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BBC
    add     sp, 8
loc_23540:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bp+var_4]
    cmp     [bx+5690h], ax
    jnz     short loc_23561
    cmp     byte_454A4, 0
    jnz     short loc_23561
    cmp     [bp+var_1A], 0
    jnz     short loc_23561
    jmp     loc_236A0
loc_23561:
    cmp     byte_46436, 0
    jnz     short loc_2356D
    call    sub_28DB6
loc_2356D:
    push    word_459D4
    sub     ax, ax
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    call    set_sprite1_size
    add     sp, 8
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     bx, ax
    cmp     word ptr [bx+5686h], 0
    jz      short loc_235C5
    push    word ptr [bx+5686h]
    push    word ptr [bx+5682h]
    mov     al, byte_44346
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+5676h]
    push    word ptr [bx+5674h]
    call    sub_33BBC
    add     sp, 8
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     word ptr [bx+5686h], 0
loc_235C5:
    cmp     [bp+var_4], 0
    jge     short loc_235D2
    mov     ax, [bp+var_4]
    neg     ax
    jmp     short loc_235D5
loc_235D2:
    mov     ax, [bp+var_4]
loc_235D5:
    shl     ax, 1
    add     ax, 0A7D4h
    mov     [bp+var_E], ax
    mov     bx, ax
    mov     al, [bx+1]
    mov     [bp+var_16], al
    mov     al, [bx]
    mov     [bp+var_14], al
    cmp     [bp+var_4], 0
    jge     short loc_235F9
    sub     al, byte_45F44
    shl     al, 1
    sub     [bp+var_14], al
loc_235F9:
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_20], ax
    mov     al, [bp+var_14]
    sub     ah, ah
    les     bx, dword_40DDC
    sub     ax, es:[bx+4]
    and     ax, word_454D4
    mov     bx, [bp+var_20]
    mov     [bx+5682h], ax
    mov     al, [bp+var_16]
    sub     ah, ah
    les     bx, dword_40DDC
    sub     ax, es:[bx+6]
    mov     bx, [bp+var_20]
    mov     [bx+5686h], ax
    mov     al, [bp+var_16]
    sub     ah, ah
    les     bx, dword_40DDC
    sub     ax, es:[bx+6]
    push    ax
    mov     bx, [bp+var_20]
    push    word ptr [bx+5682h]
    mov     al, byte_44346
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+5676h]
    push    word ptr [bx+5674h]
    call    sub_3475A
    add     sp, 8
    mov     al, [bp+var_16]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_14]
    push    ax
    push    word_40DE2
    push    word_40DE0
    call    sub_3386C
    add     sp, 8
    mov     al, [bp+var_16]
    sub     ah, ah
    push    ax
    mov     al, [bp+var_14]
    push    ax
    push    word ptr dword_40DDC+2
    push    word ptr dword_40DDC
    call    sub_34060
    add     sp, 8
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bp+var_4]
    mov     [bx+5690h], ax
loc_236A0:
    call    sub_28D9E
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_236AC:
    push    word ptr dword_40DFC+2
    push    word ptr dword_40DFC
    call    sub_324AA
    add     sp, 4
    push    word ptr dword_40DEC+2
    push    word ptr dword_40DEC
    call    sub_324AA
    add     sp, 4
    push    word ptr dword_40D80+2
    push    word ptr dword_40D80
    call    sub_324AA
    add     sp, 4
    push    word_40D86
    push    word_40D84
    call    sub_3147C
    add     sp, 4
    push    word_40D7E
    push    word_40D7C
    call    sub_3147C
    add     sp, 4
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
setup_car_shapes endp
setup_player_cars proc far
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 8
    sub     ax, ax
    mov     word ptr dword_44D26+2, ax
    mov     word ptr dword_44D26, ax
    mov     ax, 2
    push    ax
    call    ensure_file_exists
    add     sp, 2
    mov     ax, offset opponent_car_id
    push    ax
    mov     ax, offset player_car_id
    push    ax
    call    sub_1FF92
    add     sp, 4
    mov     al, player_car_id
    mov     byte ptr aCarcoun+3, al
    mov     al, byte_449A5
    mov     byte ptr aCarcoun+4, al
    mov     al, byte_449A6
    mov     byte ptr aCarcoun+5, al
    mov     al, byte_449A7
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
    call    setup_car_from_simd
    add     sp, 6
    push    [bp+var_2]
    push    [bp+var_4]
    call    unload_resource
    add     sp, 4
    cmp     opponent_index, 0
    jz      short loc_237D3
    mov     al, opponent_car_id
    mov     byte ptr aCarcoun+3, al
    mov     al, opponent_car_id+1
    mov     byte ptr aCarcoun+4, al
    mov     al, opponent_car_id+2
    mov     byte ptr aCarcoun+5, al
    mov     al, opponent_car_id+3
    mov     byte ptr aCarcoun+6, al
    mov     ax, offset aCarcoun; "carcoun"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     ax, 1
    push    ax
    push    dx
    push    [bp+var_4]
    call    setup_car_from_simd
    add     sp, 6
    push    [bp+var_2]
    push    [bp+var_4]
    call    unload_resource
    add     sp, 4
    mov     ax, 4
    push    ax
    call    ensure_file_exists
    add     sp, 2
    call    sub_217CA
loc_237D3:
    mov     ax, 3
    push    ax
    call    ensure_file_exists
    add     sp, 2
    mov     ax, offset aEng1; "eng1"
    push    ax              ; char *
    mov     ax, 5
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_454A6, ax
    mov     word_454A8, dx
    mov     ax, offset aEng ; "eng"
    push    ax              ; char *
    mov     ax, 6
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_45E0E, ax
    mov     word_45E10, dx
    call    add_audiodriver_timer
    push    word_45E10
    push    word_45E0E
    push    word_454A8
    push    word_454A6
    mov     ax, offset unk_3E7FC
    push    ds
    push    ax
    mov     ax, 21h ; '!'
    push    ax
    call    init_sfx_engine
    add     sp, 0Eh
    mov     word_43964, ax
    mov     byte_459D8, 0
    mov     byte_42D26, 0
    mov     byte_42D2A, 0
    cmp     opponent_index, 0
    jz      short loc_23870
    push    word_45E10
    push    word_45E0E
    push    word_454A8
    push    word_454A6
    mov     ax, offset unk_3E82C
    push    ds
    push    ax
    mov     ax, 20h ; ' '
    push    ax
    call    init_sfx_engine
    add     sp, 0Eh
    mov     word_4408C, ax
loc_23870:
    mov     word_44D1E, 0
    mov     word_449E4, 0
    mov     word_443F4, 0
    mov     ax, offset aFontled_fnt; "fontled.fnt"
    push    ax              ; char *
    sub     ax, ax
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_459F4, ax
    mov     word_459F6, dx
    mov     ax, word_44CEA
    mov     word_44984, ax
    call    sub_1A096
    cmp     byte_44AE2, 0
    jnz     short loc_238B4
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr setup_car_shapes
    add     sp, 2
loc_238B4:
    cmp     byte_44AE2, 0
    jnz     short loc_238DE
    mov     ax, offset aSdgame; "sdgame"
    push    ax              ; char *
    mov     ax, 3
    push    ax              ; int
    call    load_resource
    add     sp, 4
    mov     word_45D08, ax
    mov     word_45D0A, dx
    sub     ax, ax
    push    ax
    push    ax
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
loc_238DE:
    mov     ax, offset aGame; "game"
    push    ax
    call    load_res_file
    add     sp, 2
    mov     word_449A0, ax
    mov     word_449A2, dx
    mov     ax, offset aPlan; "plan"
    push    ax
    push    dx
    push    word_449A0
    call    locate_shape_alt
    add     sp, 6
    mov     word_454C2, ax
    mov     word_454C4, dx
    mov     ax, offset aWall; "wall"
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_shape_alt
    add     sp, 6
    mov     word ptr dword_46460, ax
    mov     word ptr dword_46460+2, dx
    call    load_sdgame2_shapes
    les     bx, dword_44D42
    mov     al, es:[bx+384h]; 384h = sky box position in track data
    sub     ah, ah
    push    ax
    call    load_skybox
    add     sp, 2
    call    load_game_3dshapes
    or      ax, ax
    jz      short loc_2394E
loc_23946:
    mov     ax, 1
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_2394E:
    cmp     byte_46436, 0
    jnz     short loc_239A3
    mov     ax, word_44AE4
    imul    word_461D0
    cwd
    push    dx
    push    ax
    mov     ax, 0FA00h
    sub     dx, dx
    push    dx
    push    ax
    call    __aFldiv
    add     ax, 12h
    adc     dx, 0
    mov     [bp+var_8], ax
    mov     [bp+var_6], dx
    call    sub_3A484
    cmp     dx, [bp+var_6]
    jg      short loc_23988
    jl      short loc_23946
    cmp     ax, [bp+var_8]
    jbe     short loc_23946
loc_23988:
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    make_wnd_sprite
    add     sp, 6
    mov     word ptr dword_44D26, ax
    mov     word ptr dword_44D26+2, dx
loc_239A3:
    mov     byte_463E0, 0
    mov     byte_45634, 0FFh
    sub     ax, ax
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
setup_player_cars endp
sub_239B4 proc far

    cmp     byte_46436, 0
    jnz     short loc_239D4
    mov     ax, word ptr dword_44D26
    or      ax, word ptr dword_44D26+2
    jz      short loc_239D4
    push    word ptr dword_44D26+2
    push    word ptr dword_44D26
    call    sub_324AA
    add     sp, 4
loc_239D4:
    call    sub_1FF5E
    call    unload_skybox
    call    sub_1D92A
    push    word_449A2
    push    word_449A0
    call    unload_resource
    add     sp, 4
    cmp     byte_44AE2, 0
    jnz     short loc_23A15
    push    word_45D0A
    push    word_45D08
    call    sub_3147C
    add     sp, 4
    mov     ax, 3
    push    ax
    push    cs
    call    near ptr setup_car_shapes
    add     sp, 2
loc_23A15:
    push    word_459F6
    push    word_459F4
    call    sub_3147C
    add     sp, 4
    call    sub_26BD5
    push    word_45E10
    push    word_45E0E
    call    sub_3147C
    add     sp, 4
    push    word_454A8
    push    word_454A6
    call    sub_3147C
    add     sp, 4
    call    sub_20438
    retf
sub_239B4 endp
sub_23A50 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    cmp     [bp+arg_0], 0
    jz      short loc_23A82
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 131h
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 0Fh
    push    ax
    call    sub_3695C
    add     sp, 8
    mov     ax, 64h ; 'd'
    push    ax
    mov     ax, 0A0h ; ' '
    push    ax
    call    sub_36A2C
    add     sp, 4
    pop     bp
    retf
loc_23A82:
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    call    sub_3695C
    add     sp, 8
    pop     bp
    retf
sub_23A50 endp
sub_23A98 proc far
    var_A = byte ptr -10
    var_8 = byte ptr -8
    var_6 = byte ptr -6
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    mov     si, word_445D4
smart
    and     si, 3Fh
nosmart
    cmp     byte ptr [si-7486h], 0
    jnz     short loc_23AB1
    jmp     loc_23B45
loc_23AB1:
    mov     al, [si-74DEh]
    cbw
    mov     di, ax
    mov     ax, word_44612
    mov     cl, 0Ah
    shr     ax, cl
smart
    and     al, 0FCh
nosmart
    mov     [bp+var_8], al
    cbw
    mov     bx, ax
    add     bx, word_449E8
    mov     al, [bx+1]
    mov     [bp+var_A], al
    cmp     word_44606, di
    jge     short loc_23AE0
    cmp     word_44606, 0FFFFh
    jge     short loc_23AF2
    jmp     short loc_23AED
loc_23AE0:
    cmp     word_44606, di
    jle     short loc_23AF2
    cmp     word_44606, 1
    jle     short loc_23AF2
loc_23AED:
    mov     cl, 2
    shl     [bp+var_A], cl
loc_23AF2:
    cmp     word_44606, di
    jle     short loc_23B0C
    mov     al, [bp+var_A]
    cbw
    mov     cx, word_44606
    sub     cx, ax
    cmp     cx, di
    jl      short loc_23B0C
    mov     [bp+var_6], 8
    jmp     short loc_23B28
loc_23B0C:
    cmp     word_44606, di
    jge     short loc_23B24
    mov     al, [bp+var_A]
    cbw
    add     ax, word_44606
    cmp     ax, di
    jg      short loc_23B24
    mov     [bp+var_6], 4
    jmp     short loc_23B28
loc_23B24:
    mov     [bp+var_6], 0
loc_23B28:
    cmp     [bp+var_6], 0
    jz      short loc_23B40
    mov     bx, word_445D4
    add     bx, word ptr dword_4562E
    mov     es, word ptr dword_4562E+2
    mov     al, [bp+var_6]
    or      es:[bx], al
loc_23B40:
    mov     byte ptr [si-7486h], 0
loc_23B45:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_23A98 endp
sub_23B4C proc far
    var_44 = word ptr -68
    var_42 = word ptr -66
    var_40 = byte ptr -64
    var_3E = byte ptr -62
    var_3D = byte ptr -61
    var_3C = byte ptr -60
    var_3B = byte ptr -59
    var_38 = byte ptr -56
    var_37 = byte ptr -55
    var_36 = byte ptr -54
    var_35 = byte ptr -53
    var_34 = byte ptr -52
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = byte ptr -32
    var_1E = byte ptr -30
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = byte ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 44h
    push    di
    push    si
    mov     ax, [bp+arg_0]
    or      ax, ax
    jz      short loc_23B70
    cmp     ax, 1
    jz      short loc_23BA6
    cmp     ax, 2
    jz      short loc_23B8D
    cmp     ax, 3
    jnz     short loc_23B6D
    jmp     loc_23FB8
loc_23B6D:
    jmp     loc_24D5E
loc_23B70:
    mov     ax, 569Eh
    push    ax
    mov     ax, 31E0h
    push    ax
    push    word_45D0A
    push    word_45D08
    call    locate_many_resources
    add     sp, 8
    mov     [bp+arg_2], 4
loc_23B8D:
    sub     si, si
loc_23B8F:
    mov     byte ptr [si+56FAh], 0
    inc     si
    cmp     si, 9
    jl      short loc_23B8F
    mov     bx, [bp+arg_2]
    mov     byte ptr [bx+56FAh], 1
    jmp     loc_24D5E
    ; align 2
    db 144
loc_23BA6:
    mov     al, byte_4432A
    cbw
    mov     [bp+var_42], ax
    mov     bx, ax
    cmp     byte ptr [bx-6D98h], 0
    jz      short loc_23BB9
    jmp     loc_23C66
loc_23BB9:
    mov     byte ptr [bx-6D98h], 1
    mov     bx, [bp+var_42]
    mov     byte ptr [bx+5704h], 0FFh
    mov     bx, [bp+var_42]
    mov     byte ptr [bx+5698h], 0FFh
    sub     si, si
loc_23BD0:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     ax, si
    shl     ax, 1
    add     bx, ax
    mov     byte ptr [bx+570Ah], 0
    inc     si
    cmp     si, 9
    jl      short loc_23BD0
    call    sub_28DB6
    push    word_40E10
    push    word_40E0E
    call    sub_33E00
    add     sp, 4
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_42], ax
    mov     bx, ax
    mov     word ptr [bx+569Ah], 0FFFFh
    mov     bx, [bp+var_42]
    mov     word ptr [bx+5706h], 0FFFFh
    mov     ax, 1
    push    ax              ; int
    mov     ax, word_449BC
    add     ax, word_45A24
    push    ax
    mov     ax, 0AC74h
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    call    sub_34B0C
    add     sp, 4
    push    word_459F6
    push    word_459F4
    call    sub_2988A
    add     sp, 4
    mov     ax, 0BBh ; '»'
    push    ax
    mov     ax, 0D8h ; 'Ø'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_345BC
    add     sp, 6
    call    set_fontdef
loc_23C66:
    mov     ax, [bp+arg_4]
    add     ax, word_45A24
    mov     [bp+var_42], ax
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    add     ax, 569Ah
    mov     [bp+var_44], ax
    mov     bx, ax
    mov     ax, [bp+var_42]
    cmp     [bx], ax
    jz      short loc_23CD7
    mov     [bx], ax
    mov     ax, 1
    push    ax              ; int
    push    [bp+var_42]
    mov     ax, 0AC74h
    push    ax              ; char *
    call    sub_298B8
    add     sp, 6
    sub     ax, ax
    push    ax
    push    word_407CA
    call    sub_34B0C
    add     sp, 4
    call    sub_28DB6
    push    word_459F6
    push    word_459F4
    call    sub_2988A
    add     sp, 4
    mov     ax, 0BBh ; '»'
    push    ax
    mov     ax, 98h ; '˜'
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_345BC
    add     sp, 6
    call    set_fontdef
loc_23CD7:
    mov     al, byte_4432A
    cbw
    mov     [bp+var_44], ax
    mov     bx, ax
    mov     al, byte_3B8F5
    cmp     [bx+5704h], al
    jz      short loc_23D46
    mov     [bx+5704h], al
    mov     bx, [bp+var_44]
    shl     bx, 1
    mov     word ptr [bx+5706h], 0FFFFh
    call    sub_28DB6
    mov     al, byte_3B8F5
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+56A4h]
    push    word ptr [bx+56A2h]
    call    sub_33E00
    add     sp, 4
    mov     al, byte_3B8F5
    cbw
    mov     bx, ax
    mov     al, [bx+3294h]
    mov     byte ptr [bp+var_42], al
    mov     al, byte_3E9DB
    cmp     byte ptr [bp+var_42], al
    jge     short loc_23D32
    mov     al, byte ptr [bp+var_42]
    mov     byte_3E9DB, al
loc_23D32:
    mov     al, byte_4432A
    cbw
    add     ax, 5698h
    mov     [bp+var_44], ax
    mov     bx, ax
    cmp     byte ptr [bx], 6
    jle     short loc_23D46
    mov     byte ptr [bx], 0FFh
loc_23D46:
    cmp     word_449BC, 0
    jnz     short loc_23D54
    sub     si, si
    sub     di, di
    jmp     short loc_23D94
    ; align 2
    db 144
loc_23D54:
    mov     ax, word_449BC
    cwd
    push    dx
    push    ax
    mov     ax, 6Eh ; 'n'
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_2]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     si, ax
    mov     ax, word_449BC
    cwd
    push    dx
    push    ax
    mov     ax, 6Eh ; 'n'
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
    mov     di, ax
loc_23D94:
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_44], ax
    mov     bx, ax
    cmp     [bx+5706h], si
    jnz     short loc_23DAB
    cmp     [bx+5694h], di
    jz      short loc_23E1A
loc_23DAB:
    call    sub_28DB6
    mov     al, byte_4432A
    cbw
    shl     ax, 1
    mov     [bp+var_44], ax
    mov     bx, ax
    mov     [bx+5706h], si
    mov     bx, [bp+var_44]
    mov     [bx+5694h], di
    push    word_407FC
    mov     ax, 6
    push    ax
    mov     ax, 74h ; 't'
    push    ax
    mov     ax, 0B1h ; '±'
    push    ax
    mov     ax, 9Ah ; 'š'
    push    ax
    call    sub_335D2
    add     sp, 0Ah
    push    word_407CA
    mov     ax, 6
    push    ax
    push    ax
    mov     ax, 0B1h ; '±'
    push    ax
    lea     ax, [si+9Ah]
    push    ax
    call    sub_335D2
    add     sp, 0Ah
    push    word_407FE
    mov     ax, 0B6h ; '¶'
    push    ax
    lea     ax, [di+9Fh]
    push    ax
    mov     ax, 0B1h ; '±'
    push    ax
    lea     ax, [di+9Ah]
    push    ax
    call    sub_361BC
    add     sp, 0Ah
loc_23E1A:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     al, byte_3E9DB
    cmp     [bx+5698h], al
    jz      short loc_23E68
loc_23E29:
    call    sub_28DB6
    mov     al, byte_4432A
    cbw
    mov     [bp+var_44], ax
    mov     bx, ax
    cmp     byte ptr [bx+5698h], 0FFh
    jnz     short loc_23E41
    jmp     loc_23EC6
loc_23E41:
    mov     al, [bx+5698h]
    cbw
    mov     [bp+var_42], ax
    mov     bx, ax
    shl     bx, 1
    add     bx, [bp+var_44]
    cmp     byte ptr [bx+570Ah], 0
    jz      short loc_23E9A
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+56D8h]
    push    word ptr [bx+56D6h]
    jmp     short loc_23EB3
    ; align 2
    db 144
loc_23E68:
    mov     [bp+var_20], 0
    jmp     short loc_23E71
loc_23E6E:
    inc     [bp+var_20]
loc_23E71:
    cmp     [bp+var_20], 7
    jl      short loc_23E7A
    jmp     loc_23FB0
loc_23E7A:
    mov     al, [bp+var_20]
    cbw
    mov     [bp+var_44], ax
    mov     bx, ax
    mov     al, [bx+56FAh]
    mov     cx, ax
    mov     al, byte_4432A
    cbw
    shl     bx, 1
    add     bx, ax
    cmp     [bx+570Ah], cl
    jz      short loc_23E6E
    jmp     short loc_23E29
    ; align 2
    db 144
loc_23E9A:
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     al, [bx+5698h]
    cbw
    mov     bx, ax
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+56B4h]
    push    word ptr [bx+56B2h]
loc_23EB3:
    call    sub_33E00
    add     sp, 4
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     byte ptr [bx+5698h], 0FFh
loc_23EC6:
    mov     [bp+var_20], 0
loc_23ECA:
    mov     al, [bp+var_20]
    cbw
    mov     [bp+var_44], ax
    mov     bx, ax
    cmp     byte ptr [bx+56FAh], 0
    jnz     short loc_23F18
    mov     al, [bx+56FAh]
    mov     cx, ax
    mov     al, byte_4432A
    cbw
    shl     bx, 1
    add     bx, ax
    cmp     [bx+570Ah], cl
    jz      short loc_23F18
    mov     bx, [bp+var_44]
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+56B4h]
    push    word ptr [bx+56B2h]
    call    sub_33E00
    add     sp, 4
    mov     al, [bp+var_20]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     al, byte_4432A
    cbw
    add     bx, ax
    mov     byte ptr [bx+570Ah], 0
loc_23F18:
    inc     [bp+var_20]
    cmp     [bp+var_20], 7
    jl      short loc_23ECA
    mov     [bp+var_20], 0
loc_23F25:
    mov     al, [bp+var_20]
    cbw
    mov     [bp+var_44], ax
    mov     bx, ax
    cmp     byte ptr [bx+56FAh], 0
    jz      short loc_23F6C
    mov     al, byte_4432A
    cbw
    shl     bx, 1
    add     bx, ax
    mov     byte ptr [bx+570Ah], 1
    mov     bx, [bp+var_44]
    shl     bx, 1
    shl     bx, 1
    push    word ptr [bx+56D8h]
    push    word ptr [bx+56D6h]
    call    sub_33E00
    add     sp, 4
    mov     al, [bp+var_20]
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     al, byte_4432A
    cbw
    add     bx, ax
    mov     byte ptr [bx+570Ah], 1
loc_23F6C:
    inc     [bp+var_20]
    cmp     [bp+var_20], 7
    jl      short loc_23F25
    mov     al, byte_4432A
    cbw
    mov     bx, ax
    mov     al, byte_3E9DB
    mov     [bx+5698h], al
    cmp     byte_3E9DB, 0FFh
    jz      short loc_23FB0
    mov     al, byte_3E9DB
    cbw
    shl     ax, 1
    mov     [bp+var_44], ax
    push    word_407FE
    mov     bx, ax
    push    word ptr [bx+32CEh]
    push    word ptr [bx+32AAh]
    push    word ptr [bx+32BCh]
    push    word ptr [bx+3298h]
    call    sub_361BC
    add     sp, 0Ah
loc_23FB0:
    call    sub_28D9E
    jmp     loc_24D5E
loc_23FB8:
    mov     al, byte_3B8F5
    cbw
    mov     bx, ax
    mov     al, [bx+3294h]
    mov     byte ptr [bp+var_44], al
    mov     al, byte_3E9DB
    cmp     byte ptr [bp+var_44], al
    jge     short loc_23FDA
    cmp     byte_3B8F5, 2
    jz      short loc_23FDA
    mov     al, byte ptr [bp+var_44]
    mov     byte_3E9DB, al
loc_23FDA:
    call    set_sprite2_as_1
    cmp     byte_46436, 0
    jz      short loc_23FEE
    mov     al, byte_44346
    xor     al, 1
    mov     byte_4432A, al
loc_23FEE:
    call    get_timerdelta2
    push    ax
    call    sub_287C4
    add     sp, 2
    mov     [bp+var_16], ax
    mov     ax, 32CEh
    push    ax
    mov     ax, 32BCh
    push    ax
    mov     ax, 32AAh
    push    ax
    mov     ax, 3298h
    push    ax
    mov     al, byte_3B8F5
    cbw
    mov     bx, ax
    mov     al, [bx+3294h]
    cbw
    inc     ax
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_20], al
    cmp     al, 0FFh
    jnz     short loc_2402E
    jmp     loc_240D8
loc_2402E:
    mov     al, byte_3E9DB
    cmp     [bp+var_20], al
    jz      short loc_24041
    cmp     [bp+var_16], 0
    jnz     short loc_24041
    mov     [bp+var_16], 1
loc_24041:
    mov     al, [bp+var_20]
    mov     byte_3E9DB, al
    cmp     [bp+var_16], 20h ; ' '
    jz      short loc_24056
    cmp     [bp+var_16], 0Dh
    jz      short loc_24056
    jmp     loc_2410C
loc_24056:
    cmp     byte_3E9DB, 7
    jge     short loc_24060
    jmp     loc_2410C
loc_24060:
    jnz     short loc_24082
    mov     ax, word_3EA3A
    add     ax, word_3EA4C
    sar     ax, 1
    cmp     ax, word_461CE
    jge     short loc_2407A
loc_24071:
    mov     [bp+var_16], 5000h
    jmp     loc_2410C
    ; align 2
    db 144
loc_2407A:
    mov     [bp+var_16], 4800h
    jmp     loc_2410C
loc_24082:
    mov     ax, word_3EA3C
    add     ax, word_3EA4E
    sar     ax, 1
    sub     ax, word_461CE
    push    ax
    mov     ax, word_4616C
    mov     cx, word_3EA18
    add     cx, word_3EA2A
    sar     cx, 1
    sub     ax, cx
    push    ax
    call    sub_2EA4E
    add     sp, 4
    add     ax, 80h ; '€'
smart
    and     ah, 3
nosmart
    mov     cl, 8
    shr     ax, cl
    or      ax, ax
    jz      short loc_2407A
    cmp     ax, 1
    jz      short loc_240C8
    cmp     ax, 2
    jz      short loc_24071
    cmp     ax, 3
    jz      short loc_240D0
    jmp     short loc_2410C
    ; align 2
    db 144
loc_240C8:
    mov     [bp+var_16], 4D00h
    jmp     short loc_2410C
    ; align 2
    db 144
loc_240D0:
    mov     [bp+var_16], 4B00h
    jmp     short loc_2410C
    ; align 2
    db 144
loc_240D8:
    mov     ax, 32E6h
    push    ax
    mov     ax, 32E4h
    push    ax
    mov     ax, 32E2h
    push    ax
    mov     ax, 32E0h
    push    ax
    mov     ax, 1
    push    ax
    call    sub_28E90
    add     sp, 0Ah
    mov     [bp+var_20], al
    or      al, al
    jnz     short loc_2410C
    cmp     [bp+var_16], 20h ; ' '
    jz      short loc_24107
    cmp     [bp+var_16], 0Dh
    jnz     short loc_2410C
loc_24107:
    mov     [bp+var_16], 63h ; 'c'
loc_2410C:
    cmp     [bp+var_16], 0
    jz      short loc_24129
    cmp     [bp+var_16], 1Bh
    jz      short loc_24129
    push    [bp+var_16]
    push    cs
    call    near ptr sub_223FA
    add     sp, 2
    or      al, al
    jz      short loc_24129
    jmp     loc_24D5E
loc_24129:
    cmp     byte_454B8, 0
    jnz     short loc_2415A
    cmp     [bp+var_16], 0
    jnz     short loc_2415A
    cmp     byte_46484, 0
    jnz     short loc_24140
    jmp     loc_24D5E
loc_24140:
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_2415A:
    cmp     byte_46484, 0
    jnz     short loc_2416C
    mov     byte_45634, 0FFh
    mov     word_449EA, 0FFFFh
loc_2416C:
    cmp     byte_454B8, 0
    jz      short loc_24193
    cmp     byte_40E6D, 0
    jnz     short loc_24181
    cmp     byte_40E6C, 0
    jz      short loc_24193
loc_24181:
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
loc_24193:
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     [bp+var_40], 0
    mov     ax, 1Dh
    push    ax
    call    sub_30A0D
    add     sp, 2
    or      ax, ax
    jnz     short loc_241C8
    cmp     byte_3E9DB, 8
    jnz     short loc_241CC
    test    byte ptr word_454C0, 30h
    jz      short loc_241CC
loc_241C8:
    mov     [bp+var_40], 1
loc_241CC:
    cmp     [bp+var_40], 0
    jz      short loc_241FE
    mov     ax, [bp+var_16]
    cmp     ax, 2Bh ; '+'
    jnz     short loc_241DD
    jmp     loc_2429C
loc_241DD:
    cmp     ax, 2Dh ; '-'
    jnz     short loc_241E5
    jmp     loc_2426E
loc_241E5:
    cmp     ax, 4800h
    jz      short loc_24242
    cmp     ax, 4B00h
    jz      short loc_24236
    cmp     ax, 4D00h
    jz      short loc_2422A
    cmp     ax, 5000h
    jz      short loc_24258
loc_241F9:
    mov     [bp+var_16], 0
loc_241FE:
    mov     ax, [bp+var_16]
    cmp     ax, 2Bh ; '+'
    jnz     short loc_24209
    jmp     loc_2429C
loc_24209:
    jbe     short loc_2420E
    jmp     loc_24D32
loc_2420E:
    cmp     ax, 0Dh
    jnz     short loc_24216
    jmp     loc_24334
loc_24216:
    cmp     ax, 1Bh
    jnz     short loc_2421E
    jmp     loc_24346
loc_2421E:
    cmp     ax, 20h ; ' '
    jnz     short loc_24226
    jmp     loc_24334
loc_24226:
    jmp     loc_242E7
    ; align 2
    db 144
loc_2422A:
    add     word_3B8EE, 10h
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_24236:
    sub     word_3B8EE, 10h
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_24242:
    mov     ax, word_3B8F0
    add     ax, 10h
    cmp     ax, 100h
    jge     short loc_241F9
    add     word_3B8F0, 10h
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_24258:
    mov     ax, word_3B8F0
    sub     ax, 10h
    cmp     ax, 0FF00h
    jle     short loc_241F9
    sub     word_3B8F0, 10h
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_2426E:
    cmp     byte_3B8F5, 3
    jnz     short loc_24288
    cmp     word_44D20, 0
    jg      short loc_2427F
    jmp     loc_241F9
loc_2427F:
    sub     word_44D20, 1Eh
    jmp     loc_24D5E
    ; align 2
    db 144
loc_24288:
    cmp     word_3B8EC, 5DCh
    jl      short loc_24293
    jmp     loc_241F9
loc_24293:
    add     word_3B8EC, 1Eh
    jmp     loc_24D5E
    ; align 2
    db 144
loc_2429C:
    cmp     byte_3B8F5, 3
    jnz     short loc_242B6
    cmp     word_44D20, 384h
    jl      short loc_242AE
    jmp     loc_241F9
loc_242AE:
    add     word_44D20, 1Eh
    jmp     loc_24D5E
loc_242B6:
    cmp     word_3B8EC, 78h ; 'x'
    jg      short loc_242C0
    jmp     loc_241F9
loc_242C0:
    sub     word_3B8EC, 1Eh
    jmp     loc_24D5E
loc_242C8:
    mov     al, byte_3E9DB
    cbw
    mov     bx, ax
    mov     al, [bx+326Ch]
    mov     byte ptr [bp+var_44], al
    mov     al, byte_3B8F5
    cbw
    mov     bx, ax
    mov     al, byte ptr [bp+var_44]
    cmp     [bx+3294h], al
    jl      short loc_242E7
loc_242E4:
    mov     byte_3E9DB, al
loc_242E7:
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_23FEE
    ; align 2
    db 144
loc_242FE:
    mov     al, byte_3E9DB
    cbw
    mov     bx, ax
    mov     al, [bx+3276h]
    jmp     short loc_242E4
loc_2430A:
    cmp     byte_3E9DB, 7
    jz      short loc_2429C
    mov     al, byte_3E9DB
    cbw
    mov     bx, ax
    mov     al, [bx+3280h]
    jmp     short loc_242E4
    ; align 2
    db 144
loc_2431E:
    cmp     byte_3E9DB, 7
    jnz     short loc_24328
    jmp     loc_2426E
loc_24328:
    mov     al, byte_3E9DB
    cbw
    mov     bx, ax
    mov     al, [bx+328Ah]
    jmp     short loc_242E4
loc_24334:
    mov     al, byte_3E9DB
    cbw
    cmp     ax, 6
    ja      short loc_242E7
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_24D20[bx]
    ; align 2
    db 144
loc_24346:
    mov     byte_454B8, 1
    call    sub_188A4
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    sub     si, si
loc_24377:
    mov     bx, si
    shl     bx, 1
    add     bx, bp
    mov     word ptr [bx-14h], 0
    inc     si
    cmp     si, 8
    jl      short loc_24377
    cmp     byte_446AF, 0
    jz      short loc_24394
    mov     [bp+var_E], 1
loc_24394:
    cmp     word_449BC, 0
    jz      short loc_243A2
    cmp     word_45A24, 0
    jz      short loc_243A7
loc_243A2:
    mov     [bp+var_A], 1
loc_243A7:
    cmp     byte_45E1A, 0
    jnz     short loc_243B8
    mov     [bp+var_10], 1
    mov     [bp+var_E], 1
loc_243B8:
    test    byte_43966, 4
    jnz     short loc_243C4
    mov     [bp+var_12], 1
loc_243C4:
    mov     al, byte_459F1
    mov     byte_454A4, al
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_14]
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 323Dh
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     [bp+var_2], al
    cbw
    sub     ax, 1
    cmp     ax, 6
    jbe     short loc_2440E
    jmp     loc_24828
loc_2440E:
    add     ax, ax
    xchg    ax, bx
    jmp     cs:off_2481A[bx]
loc_24416:
    call    check_input
    mov     ax, word_44D4E
    mov     word_449D0, ax
    mov     al, byte ptr word_44D4E
    mov     byte_449BA, al
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
    mov     word_42D02, 0
    mov     word_449BC, 0
    mov     byte_45D3E, 0
    mov     byte_43966, 1
    jmp     short loc_244B0
    ; align 2
    db 144
loc_2444C:
    test    byte_43966, 2
    jz      short loc_2445A
loc_24453:
    mov     byte_43966, 3
    jmp     short loc_244A7
loc_2445A:
    mov     ax, word_42D02
    cmp     word_449BC, ax
    jz      short loc_244A2
    sub     ax, ax
    push    ax
    push    ax
    push    word_40800
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 3241h
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     si, ax
    cmp     si, 1
    jge     short loc_2449F
    jmp     loc_24828
loc_2449F:
    jmp     short loc_24453
    ; align 2
    db 144
loc_244A2:
    mov     byte_43966, 1
loc_244A7:
    mov     ax, word_445D4
    mov     word_42D02, ax
    mov     word_449BC, ax
loc_244B0:
    mov     byte_43950, 1
    mov     byte_4499F, 0
    mov     byte_463E0, 0
    mov     byte_45DB2, 0
    mov     byte_3B8F5, 0
    mov     byte_4488A, 0
    mov     word_445CC, 0
    mov     byte_449E6, 0
    sub     ax, ax
    push    ax
    mov     ax, 3
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     byte_454B8, 0
    mov     al, byte_3B8F2
    cbw
    push    ax
    push    cs
    call    near ptr sub_23A50
    add     sp, 2
    call    check_input
    mov     byte_3B8F8, 0
    jmp     loc_24828
    ; align 2
    db 144
loc_2450A:
    mov     byte_43966, 0
    call    sub_188A4
    mov     ax, 3245h
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx              ; int
    push    ax              ; int
    mov     ax, 3249h
    push    ax              ; int
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    call    sub_27ED4
    add     sp, 0Ah
    cbw
    mov     si, ax
    or      si, si
    jnz     short loc_24548
    jmp     loc_24828
loc_24548:
    mov     word_44382, 96h ; '–'
    call    show_waiting
    push    si
    push    di
    lea     di, [bp+var_3E]
    mov     si, 9234h
    push    ss
    pop     es
    mov     cx, 0Dh
    repne movsw
    pop     di
    pop     si
    les     bx, dword_44D42
    mov     al, es:[bx+384h]
    sub     ah, ah
    mov     [bp+var_4], ax
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    push    cs
    call    near ptr sub_22C92
    add     sp, 4
    or      al, al
    jz      short loc_2458B
    mov     word_449BC, 0
loc_2458B:
    mov     byte_43950, 0
    call    setup_track
    sub     si, si
    les     bx, dword_44D42
    mov     al, es:[bx+384h]
    sub     ah, ah
    cmp     ax, [bp+var_4]
    jz      short loc_245AA
    mov     si, 1
loc_245AA:
    mov     al, player_car_id
    cmp     [bp+var_3E], al
    jnz     short loc_245CA
    mov     al, byte_449A5
    cmp     [bp+var_3D], al
    jnz     short loc_245CA
    mov     al, byte_449A6
    cmp     [bp+var_3C], al
    jnz     short loc_245CA
    mov     al, byte_449A7
    cmp     [bp+var_3B], al
    jz      short loc_245D0
loc_245CA:
    mov     si, 1
    jmp     short loc_2460D
    ; align 2
    db 144
loc_245D0:
    mov     al, opponent_index
    cmp     [bp+var_38], al
    jnz     short loc_245CA
    or      al, al
    jz      short loc_2460D
    mov     al, opponent_car_id
    cmp     [bp+var_37], al
    jnz     short loc_245CA
    mov     al, opponent_car_id+1
    cmp     [bp+var_36], al
    jnz     short loc_245CA
    mov     al, opponent_car_id+2
    cmp     [bp+var_35], al
    jnz     short loc_245CA
    mov     al, opponent_car_id+3
    cmp     [bp+var_34], al
    jnz     short loc_245CA
    mov     ax, 2
    push    ax
    call    ensure_file_exists
    add     sp, 2
    call    sub_217CA
loc_2460D:
    or      si, si
    jz      short loc_24619
    push    cs
    call    near ptr sub_239B4
    push    cs
    call    near ptr setup_player_cars
loc_24619:
    mov     al, byte_449BA
    cbw
    mov     word_449D0, ax
    mov     ax, 0FFFFh
    push    ax
    call    init_game_state
    add     sp, 2
    jmp     loc_24828
    ; align 2
    db 144
loc_24630:
    call    sub_188A4
loc_24635:
    mov     [bp+var_1E], 0
loc_24639:
    cmp     [bp+var_1E], 0
    jz      short loc_24642
    jmp     loc_24828
loc_24642:
    mov     ax, 324Eh
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx              ; int
    push    ax              ; int
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    call    sub_2863A
    add     sp, 8
    or      al, al
    jnz     short loc_2466F
    jmp     loc_246F0
loc_2466F:
    mov     ax, 95F8h
    push    ax              ; char *
    mov     ax, 3252h
    push    ax              ; int
    mov     ax, 140h
    push    ax
    mov     ax, 0EEh ; 'î'
    push    ax              ; char *
    call    combine_file_path
    add     sp, 8
    mov     [bp+var_1E], 1
    mov     byte_3B8FB, 1
    mov     ax, 95F8h
    push    ax
    call    find_filename
    add     sp, 2
    or      ax, ax
    jz      short loc_246E8
    sub     ax, ax
    push    ax
    push    ax
    push    word_40800
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 3257h
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     si, ax
    cmp     si, 0FFFFh
    jnz     short loc_246E0
    mov     [bp+var_1E], 0FFh
    jmp     short loc_246E8
    ; align 2
    db 144
loc_246E0:
    or      si, si
    jnz     short loc_246E8
    mov     [bp+var_1E], 0
loc_246E8:
    mov     byte_3B8FB, 0
    jmp     short loc_246F4
    ; align 2
    db 144
loc_246F0:
    mov     [bp+var_1E], 0FFh
loc_246F4:
    cmp     [bp+var_1E], 1
    jz      short loc_246FD
    jmp     loc_24639
loc_246FD:
    mov     ax, 95F8h
    push    ax
    push    cs
    call    near ptr sub_22CE8
    add     sp, 2
    mov     [bp+var_20], al
    or      al, al
    jnz     short loc_24712
    jmp     loc_24639
loc_24712:
    sub     ax, ax
    push    ax
    push    ax
    push    word_40800
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 325Bh
    push    ax
    push    word_44CEE
    push    word_44CEC
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 1
    push    ax
    call    show_dialog
    add     sp, 12h
    jmp     loc_24635
    ; align 2
    db 144
loc_24748:
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    call    sub_195E6
    add     sp, 4
loc_24757:
    mov     byte_449DA, 2
    jmp     loc_24828
    ; align 2
    db 144
loc_24760:
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    call    sub_195E6
    add     sp, 4
    mov     byte_43966, 0
    jmp     short loc_24757
loc_24776:
    sub     si, si
loc_24778:
    mov     bx, si
    shl     bx, 1
    add     bx, bp
    mov     word ptr [bx-14h], 0
    inc     si
    cmp     si, 5
    jl      short loc_24778
    cmp     opponent_index, 0
    jnz     short loc_24795
    mov     [bp+var_C], 1
loc_24795:
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_14]
    push    ax
    push    word_40802
    mov     ax, 0FFFFh
    push    ax
    push    ax
    mov     ax, 325Fh
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    mov     ax, 2
    push    ax
    call    show_dialog
    add     sp, 12h
    mov     [bp+var_2], al
    cbw
    or      ax, ax
    jz      short loc_247E8
    cmp     ax, 1
    jz      short loc_247F0
    cmp     ax, 2
    jz      short loc_247F8
    cmp     ax, 3
    jz      short loc_2480A
    cmp     ax, 4
    jz      short loc_24812
    jmp     short loc_24828
loc_247E8:
    xor     byte_43950, 1
    jmp     short loc_24828
    ; align 2
    db 144
loc_247F0:
    xor     byte_4616F, 1
    jmp     short loc_24828
    ; align 2
    db 144
loc_247F8:
    inc     byte_3B8F5
    cmp     byte_3B8F5, 4
    jnz     short loc_24828
    mov     byte_3B8F5, 0
    jmp     short loc_24828
loc_2480A:
    call    do_mrl_textres
    jmp     short loc_24828
    ; align 2
    db 144
loc_24812:
    xor     byte_463E0, 1
    jmp     short loc_24828
    ; align 2
    db 144
off_2481A     dw offset loc_24748
    dw offset loc_24416
    dw offset loc_2444C
    dw offset loc_2450A
    dw offset loc_24630
    dw offset loc_24776
    dw offset loc_24760
loc_24828:
    call    check_input
    jmp     loc_24D5E
loc_24830:
    mov     byte_454B8, 1
    call    sub_188A4
    sub     ax, ax
    push    ax
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    call    get_timerdelta2
    mov     [bp+var_24], 14h
    mov     [bp+var_22], 0
    jmp     loc_248F4
    ; align 2
    db 144
loc_2485C:
    mov     ax, 32h ; '2'
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    mov     di, ax
    add     di, 3
    cmp     di, 64h ; 'd'
    jle     short loc_2487A
    mov     di, 64h ; 'd'
loc_2487A:
    call    get_timerdelta2
    mov     [bp+var_18], ax
    imul    di
    mov     si, ax
    cwd
    add     [bp+var_24], ax
    adc     [bp+var_22], dx
    mov     ax, word_449BC
    sub     ax, word_42D02
    mov     [bp+var_44], ax
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    cmp     ax, [bp+var_44]
    jbe     short loc_248C4
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_44]
    call    __aFlmul
    mov     [bp+var_24], ax
    mov     [bp+var_22], dx
loc_248C4:
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    add     ax, word_42D02
    push    ax
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    push    [bp+var_18]
    call    sub_289E2
    add     sp, 2
loc_248F4:
    test    byte ptr word_454C0, 30h
    jz      short loc_248FE
    jmp     loc_2485C
loc_248FE:
    mov     ax, word_449BC
    sub     ax, word_42D02
    mov     [bp+var_44], ax
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    cmp     ax, [bp+var_44]
    jbe     short loc_24935
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    push    [bp+var_44]
    call    __aFlmul
    mov     [bp+var_24], ax
    mov     [bp+var_22], dx
loc_24935:
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    mov     si, ax
    add     si, word_42D02
    cmp     word_449BC, si
    jge     short loc_24956
    mov     si, word_449BC
loc_24956:
    push    si
    call    sub_16F3A
    add     sp, 2
    mov     word_42D02, si
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     ax, 3263h
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28AD6
    add     sp, 6
    cmp     word_44984, 0
    jz      short loc_249D2
    push    word_3B808
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 64h ; 'd'
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
    push    ax
    push    word_3B808
    call    sub_26572
    jmp     short loc_24A0D
loc_249D2:
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 64h ; 'd'
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
    jmp     short loc_24A10
loc_249F8:
    call    sub_17008
    push    word_42D02
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
loc_24A0D:
    add     sp, 6
loc_24A10:
    mov     ax, word_42D02
    cmp     word_445D4, ax
    jnz     short loc_249F8
loc_24A19:
    mov     ax, 3E8h
    push    ax
    call    sub_289E2
    add     sp, 2
    jmp     loc_24D5E
loc_24A28:
    mov     byte_454B8, 1
    call    sub_188A4
    sub     ax, ax
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    call    get_timerdelta2
    mov     [bp+var_24], 14h
    mov     [bp+var_22], 0
    jmp     loc_24AEA
    ; align 4
    db 144
    db 144
loc_24A58:
    mov     ax, 32h ; '2'
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    mov     di, ax
    add     di, 3
    cmp     di, 64h ; 'd'
    jle     short loc_24A76
    mov     di, 64h ; 'd'
loc_24A76:
    call    get_timerdelta2
    mov     [bp+var_18], ax
    imul    di
    mov     si, ax
    cwd
    add     [bp+var_24], ax
    adc     [bp+var_22], dx
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    cmp     ax, word_42D02
    jbe     short loc_24AB8
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    push    word_42D02
    call    __aFlmul
    mov     [bp+var_24], ax
    mov     [bp+var_22], dx
loc_24AB8:
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    mov     cx, word_42D02
    sub     cx, ax
    push    cx
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    push    [bp+var_18]
    call    sub_289E2
    add     sp, 2
loc_24AEA:
    test    byte ptr word_454C0, 30h
    jz      short loc_24AF4
    jmp     loc_24A58
loc_24AF4:
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    cmp     ax, word_42D02
    jbe     short loc_24B23
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    push    word_42D02
    call    __aFlmul
    mov     [bp+var_24], ax
    mov     [bp+var_22], dx
loc_24B23:
    mov     ax, 14h
    cwd
    push    dx
    push    ax
    push    [bp+var_22]
    push    [bp+var_24]
    call    __aFldiv
    mov     di, ax
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    or      di, di
    jnz     short loc_24B4F
    jmp     loc_24C43
loc_24B4F:
    mov     ax, 3267h
    push    ax
    push    word_449A2
    push    word_449A0
    call    locate_text_res
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 0AC74h
    push    ax
    call    sub_28AD6
    add     sp, 6
    cmp     word_44984, 0
    jz      short loc_24BB0
    push    word_3B808
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 64h ; 'd'
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
    push    ax
    push    word_3B808
    call    sub_26572
    add     sp, 6
    jmp     short loc_24BD4
    ; align 2
    db 144
loc_24BB0:
    sub     ax, ax
    push    ax
    push    word_407CA
    mov     ax, 64h ; 'd'
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
loc_24BD4:
    mov     si, word_42D02
    sub     si, di
    push    si
    call    sub_16F3A
    add     sp, 2
    mov     word_42D02, si
    mov     ax, si
    sub     ax, word_445D4
    mov     [bp+var_4], ax
    or      ax, ax
    jz      short loc_24C43
    mov     si, ax
    jmp     short loc_24C3A
loc_24BF8:
    call    sub_17008
    dec     si
    push    word_42D02
    mov     ax, [bp+var_4]
    cwd
    push    dx
    push    ax
    mov     ax, di
    cwd
    push    dx
    push    ax
    mov     ax, si
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, word_42D02
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     ax, 1
    push    ax
    call    sub_289E2
    add     sp, 2
loc_24C3A:
    mov     ax, word_42D02
    cmp     word_445D4, ax
    jnz     short loc_24BF8
loc_24C43:
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_24A19
    ; align 2
    db 144
loc_24C5A:
    mov     byte_449E6, 0
    sub     ax, ax
    push    ax
    mov     ax, 3
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_24D18
loc_24C74:
    mov     byte_454B8, 1
    call    sub_188A4
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_242E7
loc_24CA6:
    mov     byte_454B8, 1
    call    sub_188A4
    sub     ax, ax
    push    ax
    mov     ax, 5
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    push    word_445D4
    push    word_445D4
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    sub     ax, ax
    push    ax
    call    sub_16F3A
    add     sp, 2
    mov     ax, 32h ; '2'
    cwd
    push    dx
    push    ax
    call    sub_32805
    add     sp, 4
    sub     ax, ax
    push    ax
    mov     ax, 4
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    jmp     loc_24140
    ; align 2
    db 144
loc_24D04:
    sub     ax, ax
    push    ax
    mov     ax, 2
    push    ax
    push    ax
    push    cs
    call    near ptr sub_23B4C
    add     sp, 6
    mov     byte_449E6, 3
loc_24D18:
    mov     byte_454B8, 0
    jmp     loc_242E7
off_24D20     dw offset loc_24830
    dw offset loc_24A28
    dw offset loc_24D04
    dw offset loc_24C5A
    dw offset loc_24C74
    dw offset loc_24CA6
    dw offset loc_24346
    jmp     loc_242E7
    ; align 2
    db 144
loc_24D32:
    cmp     ax, 2Dh ; '-'
    jnz     short loc_24D3A
    jmp     loc_2426E
loc_24D3A:
    cmp     ax, 4800h
    jnz     short loc_24D42
    jmp     loc_2430A
loc_24D42:
    cmp     ax, 4B00h
    jnz     short loc_24D4A
    jmp     loc_242C8
loc_24D4A:
    cmp     ax, 4D00h
    jnz     short loc_24D52
    jmp     loc_242FE
loc_24D52:
    cmp     ax, 5000h
    jnz     short loc_24D5A
    jmp     loc_2431E
loc_24D5A:
    jmp     loc_242E7
    ; align 2
    db 144
loc_24D5E:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_23B4C endp
seg005 ends
end
