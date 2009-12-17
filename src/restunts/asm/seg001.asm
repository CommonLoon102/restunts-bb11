.model medium
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
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
seg001 segment byte public 'STUNTSC' use16
    assume cs:seg001
    assume es:nothing, ss:nothing, ds:dseg
    public sub_14712
    public sub_14D6C
    public sub_14DA2
    public sub_16898
    public init_game_state
    public sub_16F3A
    public sub_17008
    public sub_171E8
    public sub_17816
    public sub_17A78
    public sub_17FD6
    public sub_18466
    public sub_185F6
    public sub_187B2
    public sub_188A4
    public sub_18CD8
    public sub_18D06
    public sub_18D60
    public sub_19074
    public sub_193E0
    public sub_195E0
    public sub_195E6
    public sub_19794
    public sub_19926
    public sub_199B2
    public sub_19A2C
    public sub_19BA0
    public sub_19CA2
sub_14712 proc far
    var_40 = word ptr -64
    var_3E = word ptr -62
    var_3C = word ptr -60
    var_3A = word ptr -58
    var_38 = word ptr -56
    var_36 = word ptr -54
    var_34 = word ptr -52
    var_32 = word ptr -50
    var_30 = word ptr -48
    var_2E = word ptr -46
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = byte ptr -32
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = byte ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 40h
    push    di
    push    si
    cmp     word_449D0, 14h
    jnz     short loc_1472E
    mov     [bp+var_14], 8
    mov     [bp+var_10], 1
    jmp     short loc_14738
    ; align 2
    db 144
loc_1472E:
    mov     [bp+var_14], 10h
    mov     [bp+var_10], 2
loc_14738:
    cmp     word_446EC, 0
    jnz     short loc_14746
    cmp     byte_44889, 2
    jnz     short loc_1474C
loc_14746:
    mov     [bp+var_20], 1
    jmp     short loc_14750
loc_1474C:
    mov     [bp+var_20], 0
loc_14750:
    mov     ax, word_446B6
    mov     dx, word_446B8
    mov     cl, 6
loc_14759:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14759
    mov     [bp+var_28], ax
    mov     ax, word_446BA
    mov     dx, word_446BC
    mov     cl, 6
loc_1476D:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1476D
    mov     [bp+var_2A], ax
    mov     ax, word_446BE
    mov     dx, word_446C0
    mov     cl, 6
loc_14781:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14781
    mov     [bp+var_36], ax
    mov     ax, word_445E6
    mov     dx, word_445E8
    mov     cl, 6
loc_14795:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14795
    mov     [bp+var_34], ax
    mov     ax, word_445EA
    mov     dx, word_445EC
    mov     cl, 6
loc_147A9:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_147A9
    mov     [bp+var_3E], ax
    mov     ax, word_445EE
    mov     dx, word_445F0
    mov     cl, 6
loc_147BD:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_147BD
    mov     [bp+var_40], ax
    mov     byte_44785, 0
    mov     byte_448F2, 0
    mov     ax, 1
    push    ax
    push    word_446CE
    push    word_446D0
    push    word_446D2
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_16], ax
    mov     byte_44785, 1
    cmp     byte_4477F, 0
    jz      short loc_1480C
    cmp     word_446E2, 0
    jz      short loc_14803
    jmp     loc_14B72
loc_14803:
    mov     byte_44785, 0
    jmp     loc_14B72
    ; align 2
    db 144
loc_1480C:
    push    si
    lea     di, [bp+var_3C]
    mov     si, 8FEAh
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    cmp     [bp+var_3A], 0FFFFh
    jz      short loc_14848
    mov     ax, [bp+var_3C]
    sub     ax, [bp+var_28]
    mov     [bp+var_32], ax
    mov     ax, [bp+var_3A]
    sub     ax, [bp+var_2A]
    mov     [bp+var_30], ax
    mov     ax, [bp+var_38]
    sub     ax, [bp+var_36]
    mov     [bp+var_2E], ax
    lea     ax, [bp+var_32]
    push    ax
    call    sub_2EA08
    add     sp, 2
    jmp     short loc_1485E
loc_14848:
    mov     ax, [bp+var_38]
    sub     ax, [bp+var_36]
    push    ax
    mov     ax, [bp+var_3C]
    sub     ax, [bp+var_28]
    push    ax
    call    sub_300B6
    add     sp, 4
loc_1485E:
    mov     si, ax
    cmp     si, 0C8h ; 'È'
    jge     short loc_148B3
loc_14866:
    mov     ax, 911Dh
    push    ax
    mov     al, byte_44784
    inc     byte_44784
    sub     ah, ah
    push    ax
    mov     ax, 8FEAh
    push    ax
    mov     bx, word_44700
    shl     bx, 1
    les     di, dword_43942
    push    word ptr es:[bx+di]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    or      al, al
    jz      short loc_148B3
    inc     word_44700
    mov     bx, word_44700
    shl     bx, 1
    les     di, dword_43942
    cmp     word ptr es:[bx+di], 0
    jnz     short loc_148AE
    inc     byte_44783
    mov     word_44700, 0
loc_148AE:
    mov     byte_44784, 0
loc_148B3:
    cmp     byte_44889, 2
    jnz     short loc_148EC
loc_148BA:
    push    si
    lea     di, [bp+var_C]
    mov     si, 8FEAh
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
loc_148C7:
    push    si
    lea     di, [bp+var_26]
    lea     si, [bp+var_C]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     ax, [bp+var_28]
    sub     [bp+var_26], ax
    cmp     [bp+var_A], 0FFFFh
    jz      short loc_148E3
    jmp     loc_14A6E
loc_148E3:
    mov     [bp+var_24], 0
    jmp     loc_14A74
    ; align 2
    db 144
loc_148EC:
    mov     ax, [bp+var_34]
    sub     ax, [bp+var_28]
    mov     [bp+var_26], ax
    mov     ax, [bp+var_3E]
    sub     ax, [bp+var_2A]
    mov     [bp+var_24], ax
    mov     ax, [bp+var_40]
    sub     ax, [bp+var_36]
    mov     [bp+var_22], ax
    lea     ax, [bp+var_1C]
    push    ax
    push    [bp+var_16]
    lea     ax, [bp+var_26]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_1A], 5Ah ; 'Z'
    jg      short loc_148BA
    cmp     [bp+var_1C], 0
    jge     short loc_1492E
    mov     ax, [bp+var_1C]
    neg     ax
    jmp     short loc_14931
    ; align 2
    db 144
loc_1492E:
    mov     ax, [bp+var_1C]
loc_14931:
    cmp     ax, 0B4h ; '´'
    jg      short loc_148BA
    cmp     [bp+var_18], 258h
    jle     short loc_14940
    jmp     loc_148BA
loc_14940:
    cmp     [bp+var_18], 0FF4Ch
    jge     short loc_1494A
    jmp     loc_148BA
loc_1494A:
    mov     ax, [bp+var_34]
    sub     ax, word_4475A
    mov     [bp+var_26], ax
    cmp     word_4475C, 0FFFFh
    jnz     short loc_14962
    mov     [bp+var_24], 0
    jmp     short loc_1496C
loc_14962:
    mov     ax, [bp+var_3E]
    sub     ax, word_4475C
    mov     [bp+var_24], ax
loc_1496C:
    mov     ax, [bp+var_40]
    sub     ax, word_4475E
    mov     [bp+var_22], ax
    lea     ax, [bp+var_6]
    push    ax
    push    [bp+var_16]
    lea     ax, [bp+var_26]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_6], 0
    jge     short loc_149FE
    mov     ax, word_44766
    cwd
    mov     cx, ax
    mov     ax, word_4475A
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_C], ax
    cmp     word_4475C, 0FFFFh
    jnz     short loc_149B4
    mov     [bp+var_A], 0FFFFh
    jmp     short loc_149CB
loc_149B4:
    mov     ax, word_44768
    cwd
    mov     cx, ax
    mov     ax, word_4475C
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_A], ax
loc_149CB:
    mov     ax, word_4476A
    cwd
    mov     cx, ax
    mov     ax, word_4475E
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_8], ax
    cmp     [bp+var_18], 0FFB2h
    jg      short loc_149EB
    jmp     loc_148C7
loc_149EB:
    cmp     byte_446AF, 0
    jz      short loc_149F5
    jmp     loc_148C7
loc_149F5:
    mov     byte_448F2, 2
    jmp     loc_148C7
    ; align 2
    db 144
loc_149FE:
    mov     ax, word_44760
    cwd
    mov     cx, ax
    mov     ax, word_4475A
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_C], ax
    cmp     word_4475C, 0FFFFh
    jnz     short loc_14A24
    mov     [bp+var_A], 0FFFFh
    jmp     short loc_14A3B
    ; align 2
    db 144
loc_14A24:
    mov     ax, word_44762
    cwd
    mov     cx, ax
    mov     ax, word_4475C
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_A], ax
loc_14A3B:
    mov     ax, word_44764
    cwd
    mov     cx, ax
    mov     ax, word_4475E
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     [bp+var_8], ax
    cmp     [bp+var_18], 0FFB2h
    jg      short loc_14A5B
    jmp     loc_148C7
loc_14A5B:
    cmp     byte_446AF, 0
    jz      short loc_14A65
    jmp     loc_148C7
loc_14A65:
    mov     byte_448F2, 1
    jmp     loc_148C7
    ; align 2
    db 144
loc_14A6E:
    mov     ax, [bp+var_2A]
    sub     [bp+var_24], ax
loc_14A74:
    mov     ax, [bp+var_36]
    sub     [bp+var_22], ax
    lea     ax, [bp+var_3C]
    push    ax
    push    [bp+var_16]
    lea     ax, [bp+var_26]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_38]
    push    [bp+var_3C]
    call    sub_2EA4E
    add     sp, 4
    mov     [bp+var_2C], ax
    cmp     byte_4477D, 0
    jnz     short loc_14B03
    or      ax, ax
    jge     short loc_14AAE
    neg     ax
    jmp     short loc_14AB1
    ; align 2
    db 144
loc_14AAE:
    mov     ax, [bp+var_2C]
loc_14AB1:
    cmp     ax, 100h
    jle     short loc_14B03
    mov     ax, 911Dh
    push    ax
    mov     al, byte_44784
    inc     byte_44784
    sub     ah, ah
    push    ax
    mov     ax, 8FEAh
    push    ax
    mov     bx, word_44700
    shl     bx, 1
    les     di, dword_43942
    push    word ptr es:[bx+di]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    or      al, al
    jz      short loc_14B03
    inc     word_44700
    mov     bx, word_44700
    shl     bx, 1
    les     di, dword_43942
    cmp     word ptr es:[bx+di], 0
    jnz     short loc_14AFE
    inc     byte_44783
    mov     word_44700, 0
loc_14AFE:
    mov     byte_44784, 0
loc_14B03:
    cmp     [bp+var_2C], 41h ; 'A'
    jle     short loc_14B1E
    cmp     [bp+var_20], 0
    jnz     short loc_14B16
loc_14B0F:
    mov     [bp+var_20], 1
    jmp     loc_14866
loc_14B16:
    mov     [bp+var_2C], 41h ; 'A'
    jmp     short loc_14B2F
    ; align 2
    db 144
loc_14B1E:
    cmp     [bp+var_2C], 0FFBFh
    jge     short loc_14B2F
    cmp     [bp+var_20], 0
    jz      short loc_14B0F
    mov     [bp+var_2C], 0FFBFh
loc_14B2F:
    cmp     byte_44775, 0
    jnz     short loc_14B3B
    mov     [bp+var_2C], 0
loc_14B3B:
    mov     si, [bp+var_2C]
    sub     si, word_446D6
    jns     short loc_14B4A
    mov     ax, si
    neg     ax
    jmp     short loc_14B4C
loc_14B4A:
    mov     ax, si
loc_14B4C:
    cmp     ax, [bp+var_14]
    jle     short loc_14B6C
    mov     ax, word_446D6
    cmp     [bp+var_2C], ax
    jge     short loc_14B62
    mov     ax, [bp+var_14]
    sub     word_446D6, ax
    jmp     short loc_14B72
loc_14B62:
    mov     ax, [bp+var_14]
    add     word_446D6, ax
    jmp     short loc_14B72
    ; align 2
    db 144
loc_14B6C:
    mov     ax, [bp+var_2C]
    mov     word_446D6, ax
loc_14B72:
    mov     [bp+var_E], 0
    cmp     byte_44776, 0
    jz      short loc_14BFA
    cmp     byte_4477F, 0
    jnz     short loc_14BF6
    cmp     word_446EC, 0
    jz      short loc_14BB6
    mov     ax, [bp+var_10]
    mov     cl, 9
    shl     ax, cl
    cmp     ax, word_446E2
    jbe     short loc_14BA8
    mov     word_446E2, 0
    mov     word_446EC, 0
    jmp     short loc_14BFA
    ; align 4
    db 144
    db 144
loc_14BA8:
    mov     ax, [bp+var_10]
    mov     cl, 9
    shl     ax, cl
    sub     word_446E2, ax
    jmp     short loc_14BFA
    ; align 2
    db 144
loc_14BB6:
    mov     ax, word_446FC
    cmp     word_446FA, ax
    jg      short loc_14BF6
    cmp     byte_44889, 2
    jnz     short loc_14BCE
    mov     [bp+var_12], 4000h
    jmp     short loc_14BD7
    ; align 2
    db 144
loc_14BCE:
    mov     ah, byte_4488D
    sub     al, al
    mov     [bp+var_12], ax
loc_14BD7:
    mov     ax, [bp+var_12]
    sub     ax, 100h
    cmp     ax, word_446E0
    jbe     short loc_14BEA
    mov     [bp+var_E], 1
    jmp     short loc_14BFA
    ; align 2
    db 144
loc_14BEA:
    mov     ax, [bp+var_12]
    add     ah, 3
    cmp     ax, word_446E0
    jnb     short loc_14BFA
loc_14BF6:
    mov     [bp+var_E], 2
loc_14BFA:
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    mov     ax, 1
    push    ax
    mov     al, [bp+var_E]
    cbw
    push    ax
    push    cs
    call    near ptr sub_17A78
    add     sp, 8
    sub     ax, ax
    push    ax
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    push    cs
    call    near ptr sub_17FD6
    add     sp, 6
    mov     ax, 1
    push    ax
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    push    cs
    call    near ptr sub_14DA2
    add     sp, 0Ah
    cmp     byte_4477F, 0
    jz      short loc_14C49
    jmp     loc_14CD7
loc_14C49:
    push    si
    lea     di, [bp+var_26]
    mov     si, 8FEAh
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     ax, word_446B6
    mov     dx, word_446B8
    mov     cl, 6
loc_14C5F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14C5F
    sub     [bp+var_26], ax
    mov     ax, word_446BA
    mov     dx, word_446BC
    mov     cl, 6
loc_14C73:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14C73
    sub     [bp+var_24], ax
    mov     ax, word_446BE
    mov     dx, word_446C0
    mov     cl, 6
loc_14C87:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_14C87
    sub     [bp+var_22], ax
    mov     ax, 1
    push    ax
    push    word_446CE
    push    word_446D0
    push    word_446D2
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_16], ax
    lea     ax, [bp+var_3C]
    push    ax
    push    [bp+var_16]
    lea     ax, [bp+var_26]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_38]
    mov     ax, [bp+var_3C]
    neg     ax
    push    ax
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     word_446FE, ax
loc_14CD7:
    cmp     byte_44783, 0
    jnz     short loc_14CE1
    jmp     loc_14D66
loc_14CE1:
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    mov     cx, word_446BE
    mov     bx, word_446C0
    mov     dx, cx
    mov     cl, 6
loc_14CF9:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_14CF9
    sub     ax, dx
    push    ax
    push    word_45516
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     si, ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    mov     cx, word_446B6
    mov     bx, word_446B8
    mov     dx, cx
    mov     cl, 6
loc_14D33:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_14D33
    sub     ax, dx
    push    ax
    push    word_45516
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     si, ax
    jns     short loc_14D66
    mov     ax, 1
    push    ax
    mov     ax, 3
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_14D66:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_14712 endp
sub_14D6C proc far
    var_12 = byte ptr -18
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 12h
    push    di
    push    si
    mov     ax, [bp+arg_2]
    mov     dx, [bp+arg_4]
    lea     di, [bp+var_12]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 9
    repne movsw
    pop     ds
    push    [bp+arg_6]
    lea     ax, [bp+var_12]
    push    ax
    push    [bp+arg_0]
    call    mat_mul_vector
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_14D6C endp
sub_14DA2 proc far
    var_1E4 = word ptr -484
    var_1E2 = word ptr -482
    var_1E0 = word ptr -480
    var_1DE = word ptr -478
    var_1DC = word ptr -476
    var_1DA = word ptr -474
    var_1D8 = word ptr -472
    var_1D6 = word ptr -470
    var_1D4 = word ptr -468
    var_1D2 = word ptr -466
    var_1D0 = word ptr -464
    var_1CE = word ptr -462
    var_1CC = word ptr -460
    var_1CA = word ptr -458
    var_1C8 = word ptr -456
    var_1C6 = word ptr -454
    var_1C4 = word ptr -452
    var_1C2 = word ptr -450
    var_1C0 = word ptr -448
    var_1BE = word ptr -446
    var_1BC = word ptr -444
    var_1BA = word ptr -442
    var_1B8 = word ptr -440
    var_1B6 = word ptr -438
    var_1B4 = word ptr -436
    var_1B2 = word ptr -434
    var_1B0 = word ptr -432
    var_1AE = word ptr -430
    var_1AC = word ptr -428
    var_1AA = word ptr -426
    var_1A8 = word ptr -424
    var_1A6 = word ptr -422
    var_1A4 = word ptr -420
    var_1A2 = word ptr -418
    var_1A0 = word ptr -416
    var_19E = word ptr -414
    var_19C = word ptr -412
    var_19A = word ptr -410
    var_198 = word ptr -408
    var_196 = word ptr -406
    var_194 = word ptr -404
    var_192 = word ptr -402
    var_190 = word ptr -400
    var_18E = word ptr -398
    var_18C = word ptr -396
    var_18A = word ptr -394
    var_188 = word ptr -392
    var_186 = word ptr -390
    var_184 = word ptr -388
    var_182 = word ptr -386
    var_180 = word ptr -384
    var_17E = word ptr -382
    var_17C = word ptr -380
    var_17A = word ptr -378
    var_178 = word ptr -376
    var_176 = byte ptr -374
    var_146 = word ptr -326
    var_144 = word ptr -324
    var_142 = word ptr -322
    var_140 = word ptr -320
    var_138 = word ptr -312
    var_136 = byte ptr -310
    var_134 = byte ptr -308
    var_122 = word ptr -290
    var_120 = word ptr -288
    var_11E = word ptr -286
    var_11C = byte ptr -284
    var_11A = word ptr -282
    var_118 = word ptr -280
    var_116 = word ptr -278
    var_114 = word ptr -276
    var_112 = word ptr -274
    var_110 = word ptr -272
    var_10E = byte ptr -270
    var_FC = word ptr -252
    var_FA = word ptr -250
    var_F8 = word ptr -248
    var_F4 = word ptr -244
    var_F2 = word ptr -242
    var_F0 = word ptr -240
    var_EE = word ptr -238
    var_EC = byte ptr -236
    var_EA = word ptr -234
    var_E8 = byte ptr -232
    var_E6 = word ptr -230
    var_E4 = word ptr -228
    var_E2 = word ptr -226
    var_E0 = word ptr -224
    var_DE = word ptr -222
    var_DC = byte ptr -220
    var_1C = word ptr -28
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = dword ptr -6
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = byte ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 1E4h
    push    di
    push    si
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     word_449BE, ax
    mov     word_449C0, dx
    mov     [bx+0Ch], ax
    mov     [bx+0Eh], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     word_449C6, ax
    mov     word_449C8, dx
    mov     [bx+10h], ax
    mov     [bx+12h], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     word_449CA, ax
    mov     word_449CC, dx
    mov     [bx+14h], ax
    mov     [bx+16h], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bx+1Ch]
    mov     word_44998, ax
    mov     word_44F42, ax
    mov     ax, [bx+1Ah]
    mov     word_44986, ax
    mov     word_44F44, ax
    mov     ax, [bx+18h]
    mov     word_4435E, ax
    mov     word_44F46, ax
    cmp     byte ptr [bx+0C1h], 0
    jz      short loc_14E1E
    mov     ax, [bx+40h]
    sar     ax, 1
    sar     ax, 1
    mov     [bp+var_142], ax
    jmp     short loc_14E24
loc_14E1E:
    mov     [bp+var_142], 0
loc_14E24:
    cmp     word_449D0, 0Ah
    jnz     short loc_14E30
    mov     ax, 1E00h
    jmp     short loc_14E33
loc_14E30:
    mov     ax, 3C00h
loc_14E33:
    sub     dx, dx
    push    dx
    push    ax
    mov     ax, 580h
    cwd
    push    dx
    push    ax
    mov     bx, [bp+arg_0]
    sub     ax, ax
    push    ax
    push    word ptr [bx+2Ch]
    call    __aFlmul
    push    dx
    push    ax
    call    __aFuldiv
    mov     [bp+var_E6], ax
    sub     ax, ax
    push    ax
    mov     ax, word_4435E
    neg     ax
    push    ax
    mov     ax, word_44986
    neg     ax
    push    ax
    mov     ax, word_44998
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    push    si
    mov     di, 8B62h
    mov     si, ax
    push    ds
    pop     es
    mov     cx, 9
    repne movsw
    pop     si
    cmp     word_44986, 0
    jnz     short loc_14E8F
    cmp     word_44998, 0
    jz      short loc_14EC6
loc_14E8F:
    mov     [bp+var_1C6], 0
    mov     [bp+var_1C4], 0
    mov     [bp+var_1C2], 82h ; '‚'
    lea     ax, [bp+var_FC]
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_FA]
    neg     ax
    mov     [bx+1Eh], ax
    jmp     short loc_14ECE
    ; align 2
    db 144
loc_14EC6:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+1Eh], 0
loc_14ECE:
    mov     bx, [bp+arg_0]
    test    word ptr [bx+3Eh], 3FFh
    jz      short loc_14F04
    mov     [bp+var_EC], 1
    sub     ax, ax
    push    ax
    mov     ax, [bx+3Eh]
    neg     ax
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    push    si
    lea     di, [bp+var_10E]
    mov     si, ax
    push    ss
    pop     es
    mov     cx, 9
    repne movsw
    pop     si
    jmp     short loc_14F09
    ; align 2
    db 144
loc_14F04:
    mov     [bp+var_EC], 0
loc_14F09:
    mov     [bp+var_1C6], 0
    mov     [bp+var_1C4], 7530h
    mov     [bp+var_1C2], 0
    lea     ax, [bp+var_FC]
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C1h], 0
    jz      short loc_14F76
    cmp     [bp+var_FA], 0
    jge     short loc_14F76
    cmp     word ptr [bx+2Ch], 1E00h
    jbe     short loc_14F6E
    mov     [bp+var_F0], 0C0h ; 'À'
    mov     [bp+var_1C4], 0FF40h
    lea     ax, [bp+var_E4]
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    jmp     short loc_14F7C
    ; align 2
    db 144
loc_14F6E:
    mov     [bp+var_F0], 0FF40h
    jmp     short loc_14F7C
loc_14F76:
    mov     [bp+var_F0], 0
loc_14F7C:
    mov     word_44F36, 0
    mov     word_44F38, 0
    mov     word_44F4A, 0FFFFh
    lea     ax, [bp+var_1C0]
    mov     [bp+var_DE], ax
    lea     ax, [bp+var_176]
    mov     [bp+var_146], ax
    mov     [bp+var_E8], 0
    jmp     short loc_14FFA
    ; align 2
    db 144
loc_14FA6:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+36h]
loc_14FAC:
    mov     word_44F48, ax
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     ax, word_44F48
    mov     [bp+di+var_140], ax
    push    cs
    call    near ptr sub_19794
    mov     ax, word_44F3C
    cwd
    mov     bx, [bp+var_DE]
    add     [bx], ax
    adc     [bx+2], dx
    mov     ax, word_44F3E
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+4], ax
    adc     [bx+6], dx
    mov     ax, word_44F40
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+8], ax
    adc     [bx+0Ah], dx
loc_14FEC:
    add     [bp+var_DE], 0Ch
    add     [bp+var_146], 0Ch
    inc     [bp+var_E8]
loc_14FFA:
    cmp     [bp+var_E8], 4
    jl      short loc_15004
    jmp     loc_1513E
loc_15004:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     bx, [bp+arg_2]
    push    si
    lea     si, [bx+di+0D2h]
    lea     di, [bp+var_1C6]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [bx+di+54h]
    add     ax, 180h
    neg     ax
    mov     [bp+var_1C4], ax
    cmp     [bp+var_F0], 0
    jge     short loc_1504A
    mov     ax, [bp+var_F0]
    sub     [bp+var_1C4], ax
loc_1504A:
    cmp     [bp+var_EC], 0
    jz      short loc_15077
    lea     ax, [bp+var_FC]
    push    ax
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    si
    lea     di, [bp+var_1C6]
    lea     si, [bp+var_FC]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
loc_15077:
    lea     ax, [bp+var_FC]
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_FC]
    cwd
    add     ax, word_449BE
    adc     dx, word_449C0
    mov     bx, [bp+var_DE]
    mov     [bx], ax
    mov     [bx+2], dx
    mov     ax, [bp+var_FA]
    cwd
    add     ax, word_449C6
    adc     dx, word_449C8
    mov     bx, [bp+var_DE]
    mov     [bx+4], ax
    mov     [bx+6], dx
    mov     ax, [bp+var_F8]
    cwd
    add     ax, word_449CA
    adc     dx, word_449CC
    mov     bx, [bp+var_DE]
    mov     [bx+8], ax
    mov     [bx+0Ah], dx
    mov     bx, [bp+var_146]
    mov     di, [bp+var_DE]
    mov     ax, [di]
    mov     dx, [di+2]
    mov     [bx], ax
    mov     [bx+2], dx
    mov     bx, [bp+var_146]
    mov     di, [bp+var_DE]
    mov     ax, [di+4]
    mov     dx, [di+6]
    mov     [bx+4], ax
    mov     [bx+6], dx
    mov     bx, [bp+var_146]
    mov     di, [bp+var_DE]
    mov     ax, [di+8]
    mov     dx, [di+0Ah]
    mov     [bx+8], ax
    mov     [bx+0Ah], dx
    cmp     [bp+var_E6], 0
    jnz     short loc_15115
    jmp     loc_14FEC
loc_15115:
    mov     ax, [bp+var_E6]
    mov     word_44F3A, ax
    cmp     [bp+var_142], 0
    jnz     short loc_15126
    jmp     loc_14FA6
loc_15126:
    cmp     [bp+var_E8], 2
    jl      short loc_15130
    jmp     loc_14FA6
loc_15130:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+36h]
    sub     ax, [bp+var_142]
    jmp     loc_14FAC
    ; align 2
    db 144
loc_1513E:
    mov     [bp+var_2], 0
loc_15142:
    inc     [bp+var_2]
    cmp     [bp+var_2], 5
    jnz     short loc_151A2
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+36h], 200h
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_15163:
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C2h], 5
    jnz     short loc_15192
    cmp     byte ptr [bx+0C3h], 5
    jnz     short loc_15192
    cmp     byte ptr [bx+0C4h], 5
    jnz     short loc_15192
    cmp     byte ptr [bx+0C5h], 5
    jnz     short loc_15192
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 2
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_15192:
    lea     ax, [bp+var_1C0]
    mov     [bp+var_DE], ax
    mov     [bp+var_E8], 0
    jmp     loc_15DD1
loc_151A2:
    lea     ax, [bp+var_1C0]
    mov     [bp+var_DE], ax
    lea     ax, [bp+var_176]
    mov     [bp+var_146], ax
    mov     [bp+var_E8], 0
    jmp     loc_15D39
loc_151BA:
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+arg_0]
    add     ax, 74h ; 't'
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    sub_1E1A0
    add     sp, 4
loc_151DB:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    mov     bx, [bp+arg_0]
    mov     al, byte_44D47
    mov     [bx+di+0C2h], al
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_151F7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_151F7
    mov     [bp+var_1C6], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_1520F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1520F
    mov     [bp+var_1C4], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_15227:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15227
    mov     [bp+var_1C2], ax
    cmp     byte_44889, 2
    jnz     short loc_15240
    mov     ax, [bp+var_1C4]
    jmp     short loc_15257
loc_15240:
    push    [bp+var_1C2]
    push    [bp+var_1C4]
    push    [bp+var_1C6]
    push    word_44DC6
    push    cs
    call    near ptr sub_19926
    add     sp, 8
loc_15257:
    mov     word_459FE, ax
    cmp     word_4428E, 0FFFFh
    jnz     short loc_15264
    jmp     loc_15950
loc_15264:
    mov     ax, word_44988
    cmp     word_459FE, ax
    jg      short loc_15270
    jmp     loc_15950
loc_15270:
    mov     ax, word_44A8C
    cmp     word_459FE, ax
    jl      short loc_1527C
    jmp     loc_15950
loc_1527C:
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, [bp+arg_0]
    mov     ax, [di+74h]
    sub     ax, word_42D20
    mov     [bp+var_182], ax
    mov     [bp+var_180], 0
    mov     ax, [di+78h]
    sub     ax, word_42D2C
    mov     [bp+var_17E], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_152B5:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_152B5
    sub     ax, word_42D20
    mov     [bp+var_1E4], ax
    mov     [bp+var_1E2], 0
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_152D7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_152D7
    sub     ax, word_42D2C
    mov     [bp+var_1E0], ax
    mov     ax, word_45D96
    neg     ax
    sub     ax, 100h
    push    ax
    lea     ax, [bp+var_134]
    push    ax
    call    mat_rot_y
    add     sp, 4
    lea     ax, [bp+var_C]
    push    ax
    lea     ax, [bp+var_134]
    push    ax
    lea     ax, [bp+var_182]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_1C]
    push    ax
    lea     ax, [bp+var_134]
    push    ax
    lea     ax, [bp+var_1E4]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_18], 0
    jle     short loc_15338
    cmp     [bp+var_8], 0
    jle     short loc_15338
    jmp     loc_15950
loc_15338:
    cmp     [bp+var_18], 0
    jge     short loc_15347
    cmp     [bp+var_8], 0
    jge     short loc_15347
    jmp     loc_15950
loc_15347:
    mov     ax, [bp+var_8]
    cmp     [bp+var_18], ax
    jle     short loc_1537C
    mov     [bp+var_136], 1
    push    si
    lea     di, [bp+var_FC]
    lea     si, [bp+var_1C]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    push    si
    lea     di, [bp+var_1C]
    lea     si, [bp+var_C]
    movsw
    movsw
    movsw
    pop     si
    push    si
    lea     di, [bp+var_C]
    lea     si, [bp+var_FC]
    movsw
    movsw
    movsw
    pop     si
    jmp     short loc_15381
    ; align 2
    db 144
loc_1537C:
    mov     [bp+var_136], 0
loc_15381:
    cmp     [bp+var_18], 0
    jnz     short loc_15398
    mov     ax, [bp+var_E6]
    mov     [bp+var_F4], ax
    mov     [bp+var_F2], 0
    jmp     short loc_1540C
    ; align 2
    db 144
loc_15398:
    cmp     [bp+var_8], 0
    jnz     short loc_153AE
    mov     [bp+var_F4], 0
    mov     ax, [bp+var_E6]
    mov     [bp+var_F2], ax
    jmp     short loc_1540C
loc_153AE:
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_FC]
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    lea     ax, [bp+var_1C]
    push    ax
    call    sub_33014
    add     sp, 8
    mov     ax, [bp+var_1C]
    sub     ax, [bp+var_FC]
    mov     cl, 6
    shl     ax, cl
    mov     [bp+var_17C], ax
    mov     ax, [bp+var_1A]
    sub     ax, [bp+var_FA]
    shl     ax, cl
    mov     [bp+var_17A], ax
    mov     ax, [bp+var_18]
    sub     ax, [bp+var_F8]
    shl     ax, cl
    mov     [bp+var_178], ax
    lea     ax, [bp+var_17C]
    push    ax
    call    sub_2EA08
    add     sp, 2
    mov     [bp+var_F2], ax
    mov     ax, [bp+var_E6]
    sub     ax, [bp+var_F2]
    mov     [bp+var_F4], ax
loc_1540C:
    mov     ax, word_4435E
    neg     ax
    sub     ax, word_45D96
smart
    and     ah, 3
nosmart
    mov     [bp+var_EE], ax
    mov     ax, [bp+var_F2]
    mov     [bp+var_F8], ax
    mov     [bp+var_FA], 0
    cmp     [bp+var_EE], 100h
    jl      short loc_1543A
    cmp     [bp+var_EE], 300h
    jle     short loc_1544A
loc_1543A:
    mov     ax, word_45D96
    mov     [bp+var_EE], ax
    mov     [bp+var_FC], 300h
    jmp     short loc_1545D
    ; align 2
    db 144
loc_1544A:
    mov     ax, word_45D96
    add     ah, 2
smart
    and     ah, 3
nosmart
    mov     [bp+var_EE], ax
    mov     [bp+var_FC], 0FD00h
loc_1545D:
    cmp     [bp+var_136], 0
    jz      short loc_1546E
    mov     ax, [bp+var_FC]
    neg     ax
    mov     [bp+var_FC], ax
loc_1546E:
    sub     ax, ax
    push    ax
    push    [bp+var_EE]
    mov     ax, word_44986
    neg     ax
    push    ax
    mov     ax, word_44998
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_EA], ax
    lea     ax, [bp+var_1C]
    push    ax
    push    [bp+var_EA]
    lea     ax, [bp+var_FC]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, word_4435E
    neg     ax
    mov     si, ax
    sub     si, [bp+var_EE]
smart
    and     si, 3FFh
nosmart
    mov     [bp+var_138], 0
    cmp     si, 100h
    jle     short loc_154CA
    mov     ax, 400h
    sub     ax, si
    mov     si, ax
    mov     [bp+var_138], 1
loc_154CA:
    mov     ax, 46h ; 'F'
    imul    si
    mov     cl, 8
    sar     ax, cl
    sub     ax, 64h ; 'd'
    neg     ax
    mov     ch, al
    sub     cl, cl
    mov     [bp+var_190], cx
    mov     bx, [bp+arg_0]
    mov     ax, cx
    cmp     [bx+2Ch], ax
    jbe     short loc_15513
    cmp     [bp+var_138], 0
    jz      short loc_154F8
    mov     ax, si
    neg     ax
    jmp     short loc_154FA
    ; align 2
    db 144
loc_154F8:
    mov     ax, si
loc_154FA:
    shl     ax, 1
    mov     [bp+var_138], ax
    mov     [bx+36h], ax
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_15513:
    mov     bx, [bp+arg_0]
smart
    or      byte ptr [bx+0CFh], 10h
nosmart
    lea     ax, [bp+var_1C0]
    mov     [bp+var_DE], ax
    lea     ax, [bp+var_176]
    mov     [bp+var_146], ax
    sub     si, si
    jmp     short loc_15599
    ; align 2
    db 144
loc_15530:
    mov     [bp+var_C], 0
    mov     [bp+var_A], 0
    mov     [bp+var_8], 0
loc_1553F:
    mov     ax, [bp+var_C]
    add     ax, [bp+var_1C]
    cwd
    mov     bx, [bp+var_146]
    add     ax, [bx]
    adc     dx, [bx+2]
    mov     bx, [bp+var_DE]
    mov     [bx], ax
    mov     [bx+2], dx
    mov     ax, [bp+var_A]
    add     ax, [bp+var_1A]
    cwd
    mov     bx, [bp+var_146]
    add     ax, [bx+4]
    adc     dx, [bx+6]
    mov     bx, [bp+var_DE]
    mov     [bx+4], ax
    mov     [bx+6], dx
    mov     ax, [bp+var_8]
    add     ax, [bp+var_18]
    cwd
    mov     bx, [bp+var_146]
    add     ax, [bx+8]
    adc     dx, [bx+0Ah]
    mov     bx, [bp+var_DE]
    mov     [bx+8], ax
    mov     [bx+0Ah], dx
    add     [bp+var_DE], 0Ch
    add     [bp+var_146], 0Ch
    inc     si
loc_15599:
    cmp     si, 4
    jl      short loc_155A1
    jmp     loc_15142
loc_155A1:
    cmp     [bp+var_F4], 0
    jz      short loc_15530
    mov     ax, [bp+var_E6]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     bx, [bp+var_146]
    sub     ax, [bx]
    sbb     dx, [bx+2]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_C], ax
    mov     ax, [bp+var_E6]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     bx, [bp+var_146]
    sub     ax, [bx+4]
    sbb     dx, [bx+6]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_A], ax
    mov     ax, [bp+var_E6]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     bx, [bp+var_146]
    sub     ax, [bx+8]
    sbb     dx, [bx+0Ah]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_8], ax
    jmp     loc_1553F
loc_15642:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [di+602h]
    add     [bx+di+4Ch], ax
    mov     al, [bp+var_E8]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, [bp+arg_0]
    mov     ax, [bx+4Ch]
    cwd
    mov     bx, [bp+var_DE]
    sub     [bx+4], ax
    sbb     [bx+6], dx
    cmp     word_449D0, 0Ah
    jnz     short loc_156A3
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [di+602h]
    add     [bx+di+4Ch], ax
    mov     al, [bp+var_E8]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, [bp+arg_0]
    mov     ax, [bx+4Ch]
    cwd
    mov     bx, [bp+var_DE]
    sub     [bx+4], ax
    sbb     [bx+6], dx
loc_156A3:
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_156AF:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_156AF
    mov     [bp+var_1C4], ax
    cmp     byte_44889, 2
    jz      short loc_156D6
    push    [bp+var_1C2]
    push    ax
    push    [bp+var_1C6]
    push    word_44DC6
    push    cs
    call    near ptr sub_19926
    add     sp, 8
loc_156D6:
    mov     word_459FE, ax
    cmp     ax, 0Ch
    jle     short loc_156ED
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+di+0C2h], 0
loc_156ED:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     ax, word_459FE
    mov     [bp+di+var_16], ax
    or      ax, ax
    jnz     short loc_15703
    jmp     loc_15CE8
loc_15703:
    or      ax, ax
    jl      short loc_1570A
    jmp     loc_15D2B
loc_1570A:
    mov     ax, 22h ; '"'
    imul    word_44DC6
    add     ax, word_454C2
    mov     dx, word_454C4
    mov     word ptr [bp+var_6], ax
    mov     word ptr [bp+var_6+2], dx
    les     bx, [bp+var_6]
    mov     ax, es:[bx+4]
    add     ax, word_449E0
    mov     [bp+var_122], ax
    mov     ax, es:[bx+6]
    add     ax, word_449EC
    mov     [bp+var_120], ax
    mov     ax, es:[bx+8]
    add     ax, word_449F6
    mov     [bp+var_11E], ax
    mov     bx, [bp+var_146]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_15751:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15751
    sub     ax, [bp+var_122]
    mov     [bp+var_182], ax
    mov     bx, [bp+var_146]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_1576D:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1576D
    sub     ax, [bp+var_120]
    mov     [bp+var_180], ax
    mov     bx, [bp+var_146]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_15789:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15789
    sub     ax, [bp+var_11E]
    mov     [bp+var_17E], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_157A4:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_157A4
    sub     ax, [bp+var_122]
    mov     [bp+var_1E4], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_157C0:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_157C0
    sub     ax, [bp+var_120]
    mov     [bp+var_1E2], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_157DC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_157DC
    sub     ax, [bp+var_11E]
    mov     [bp+var_1E0], ax
    mov     ax, word ptr [bp+var_6]
    mov     dx, word ptr [bp+var_6+2]
    add     ax, 10h
    push    si
    lea     di, [bp+var_134]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 9
    repne movsw
    pop     ds
    pop     si
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_134]
    push    ax
    call    mat_copy
    add     sp, 4
    lea     ax, [bp+var_C]
    push    ax
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_182]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    lea     ax, [bp+var_1C]
    push    ax
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_1E4]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     [bp+var_136], 0
    cmp     byte_4392C, 0
    jnz     short loc_15879
    cmp     [bp+var_A], 0FFF4h
    jge     short loc_15879
    cmp     [bp+var_1A], 0FFF4h
    jge     short loc_15879
    cmp     [bp+var_1A], 0FFE8h
    jle     short loc_158DA
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 5
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
    mov     [bp+var_136], 1
loc_15879:
    cmp     [bp+var_1A], 0
    jz      short loc_15882
    jmp     loc_1599E
loc_15882:
    mov     word_44F36, 0
    mov     word_44F38, 0
    mov     word_44F3A, 40h ; '@'
    mov     ax, word_44DC6
    mov     word_44F4A, ax
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     ax, [bp+di+var_140]
    mov     word_44F48, ax
    push    cs
    call    near ptr sub_19794
    mov     ax, word_44F3C
    cwd
    mov     bx, [bp+var_DE]
    sub     [bx], ax
    sbb     [bx+2], dx
    mov     ax, word_44F3E
    cwd
    mov     bx, [bp+var_DE]
    sub     [bx+4], ax
    sbb     [bx+6], dx
    mov     ax, word_44F40
    cwd
    mov     bx, [bp+var_DE]
    sub     [bx+8], ax
    sbb     [bx+0Ah], dx
    jmp     loc_15CDF
loc_158DA:
    mov     word_44DC6, 0
    mov     ax, word_454C2
    mov     dx, word_454C4
    mov     word_44D64, ax
    mov     word_44D66, dx
    mov     byte_4392C, 1
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_158FE:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_158FE
    mov     [bp+var_1C6], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_15916:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15916
    mov     [bp+var_1C4], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_1592E:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1592E
    mov     [bp+var_1C2], ax
    push    ax
    push    [bp+var_1C4]
    push    [bp+var_1C6]
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_19926
    add     sp, 8
    mov     word_459FE, ax
loc_15950:
    cmp     word_459FE, 0
    jg      short loc_1595A
    jmp     loc_156ED
loc_1595A:
    cmp     [bp+var_F0], 0
    jg      short loc_15964
    jmp     loc_15642
loc_15964:
    cmp     word_459FE, 18h
    jl      short loc_1596E
    jmp     loc_15642
loc_1596E:
    mov     ax, [bp+var_E4]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx], ax
    adc     [bx+2], dx
    mov     ax, [bp+var_E2]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+4], ax
    adc     [bx+6], dx
    mov     ax, [bp+var_E0]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+8], ax
    adc     [bx+0Ah], dx
    jmp     loc_156ED
    ; align 2
    db 144
loc_1599E:
    cmp     [bp+var_A], 0
    jle     short loc_159AD
    cmp     [bp+var_1A], 0
    jge     short loc_159AD
    jmp     loc_15A30
loc_159AD:
    mov     word_44F36, 0
    mov     word_44F38, 0
    mov     ax, [bp+var_E6]
    mov     word_44F3A, ax
    mov     ax, word_44DC6
    mov     word_44F4A, ax
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     ax, [bp+di+var_140]
    mov     word_44F48, ax
    push    cs
    call    near ptr sub_19794
    mov     ax, word_44F3C
    cwd
    mov     bx, [bp+var_146]
    mov     cx, [bx]
    mov     di, [bx+2]
    add     cx, ax
    adc     di, dx
    mov     bx, [bp+var_DE]
    mov     [bx], cx
    mov     [bx+2], di
    mov     ax, word_44F3E
    cwd
    mov     bx, [bp+var_146]
    mov     cx, [bx+4]
    mov     di, [bx+6]
    add     cx, ax
    adc     di, dx
    mov     bx, [bp+var_DE]
    mov     [bx+4], cx
    mov     [bx+6], di
    mov     ax, word_44F40
    cwd
    mov     bx, [bp+var_146]
    mov     cx, [bx+8]
    mov     di, [bx+0Ah]
    add     cx, ax
    adc     di, dx
    mov     bx, [bp+var_DE]
    mov     [bx+8], cx
    mov     [bx+0Ah], di
    jmp     loc_15C04
    ; align 2
    db 144
loc_15A30:
    mov     ax, [bp+var_8]
    mov     [bp+var_EE], ax
    mov     ax, [bp+var_A]
    neg     ax
    mov     [bp+var_8], ax
    mov     ax, [bp+var_EE]
    mov     [bp+var_A], ax
    mov     ax, [bp+var_18]
    mov     [bp+var_EE], ax
    mov     ax, [bp+var_1A]
    neg     ax
    mov     [bp+var_18], ax
    mov     ax, [bp+var_EE]
    mov     [bp+var_1A], ax
    sub     ax, ax
    push    ax
    lea     ax, [bp+var_FC]
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    lea     ax, [bp+var_1C]
    push    ax
    call    sub_33014
    add     sp, 8
    mov     ax, [bp+var_1C]
    sub     ax, [bp+var_FC]
    mov     cl, 6
    shl     ax, cl
    mov     [bp+var_17C], ax
    mov     ax, [bp+var_1A]
    sub     ax, [bp+var_FA]
    shl     ax, cl
    mov     [bp+var_17A], ax
    mov     ax, [bp+var_18]
    sub     ax, [bp+var_F8]
    shl     ax, cl
    mov     [bp+var_178], ax
    lea     ax, [bp+var_17C]
    push    ax
    call    sub_2EA08
    add     sp, 2
    mov     [bp+var_EE], ax
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [bx+di+4Ch]
    add     ax, [bp+var_E6]
    mov     [bp+var_F4], ax
    sub     ax, [bp+var_EE]
    mov     [bp+var_F2], ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F2]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     bx, [bp+var_146]
    sub     ax, [bx]
    sbb     dx, [bx+2]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_C], ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F2]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     bx, [bp+var_146]
    sub     ax, [bx+4]
    sbb     dx, [bx+6]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_A], ax
    mov     ax, [bp+var_F4]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_F2]
    cwd
    push    dx
    push    ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     bx, [bp+var_146]
    sub     ax, [bx+8]
    sbb     dx, [bx+0Ah]
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_8], ax
    mov     word_44F36, 0
    mov     word_44F38, 0
    mov     ax, [bp+var_EE]
    mov     word_44F3A, ax
    mov     ax, word_44DC6
    mov     word_44F4A, ax
    mov     ax, [bp+di+var_140]
    mov     word_44F48, ax
    push    cs
    call    near ptr sub_19794
    mov     bx, [bp+var_146]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cx, ax
    mov     ax, [bp+var_C]
    mov     bx, dx
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, word_44F3C
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, bx
    mov     bx, [bp+var_DE]
    mov     [bx], cx
    mov     [bx+2], ax
    mov     bx, [bp+var_146]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cx, ax
    mov     ax, [bp+var_A]
    mov     bx, dx
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, word_44F3E
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, bx
    mov     bx, [bp+var_DE]
    mov     [bx+4], cx
    mov     [bx+6], ax
    mov     bx, [bp+var_146]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cx, ax
    mov     ax, [bp+var_8]
    mov     bx, dx
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, word_44F40
    cwd
    add     cx, ax
    adc     bx, dx
    mov     ax, bx
    mov     bx, [bp+var_DE]
    mov     [bx+8], cx
    mov     [bx+0Ah], ax
loc_15C04:
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_15C0F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15C0F
    mov     [bp+var_1C6], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_15C27:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15C27
    mov     [bp+var_1C4], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_15C3F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15C3F
    mov     [bp+var_1C2], ax
    push    ax
    push    [bp+var_1C4]
    push    [bp+var_1C6]
    push    word_44DC6
    push    cs
    call    near ptr sub_19926
    add     sp, 8
    mov     word_459FE, ax
    or      ax, ax
    jge     short loc_15CDF
    cmp     [bp+var_136], 0
    jz      short loc_15C75
    neg     ax
    add     ax, 6
    mov     word_459FE, ax
loc_15C75:
    mov     [bp+var_1C2], 0
    mov     [bp+var_1C6], 0
    mov     ax, word_459FE
    neg     ax
    mov     cl, 6
    shl     ax, cl
    mov     [bp+var_1C4], ax
    lea     ax, [bp+var_FC]
    push    ax
    mov     ax, 22h ; '"'
    imul    word_44DC6
    add     ax, word_454C2
    mov     dx, word_454C4
    add     ax, 10h
    push    dx
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    push    cs
    call    near ptr sub_14D6C
    add     sp, 8
    mov     ax, [bp+var_FC]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx], ax
    adc     [bx+2], dx
    mov     ax, [bp+var_FA]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+4], ax
    adc     [bx+6], dx
    mov     ax, [bp+var_F8]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+8], ax
    adc     [bx+0Ah], dx
loc_15CDF:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
loc_15CE8:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+di+4Ch], 0FAh ; 'ú'
    jle     short loc_15CF7
smart
    or      byte ptr [bx+0CFh], 20h
nosmart
loc_15CF7:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+di+4Ch], 5AEBh
    jle     short loc_15D1A
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_15D1A:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+di+4Ch], 0
loc_15D2B:
    add     [bp+var_DE], 0Ch
    add     [bp+var_146], 0Ch
    inc     [bp+var_E8]
loc_15D39:
    cmp     [bp+var_E8], 4
    jl      short loc_15D43
    jmp     loc_15163
loc_15D43:
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_15D4E:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15D4E
    mov     [bp+var_1C6], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_15D66:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15D66
    mov     [bp+var_1C4], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_15D7E:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15D7E
    mov     [bp+var_1C2], ax
    cmp     byte_44889, 2
    jz      short loc_15D94
    jmp     loc_151BA
loc_15D94:
    mov     word_4428E, 0FFFFh
    mov     byte_44D47, 1
    mov     word_44DC6, 0
    mov     ax, word_454C2
    mov     dx, word_454C4
    mov     word_44D64, ax
    mov     word_44D66, dx
    jmp     loc_151DB
loc_15DB6:
    mov     ax, [bp+var_EE]
    add     ax, 180h
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+4], ax
    adc     [bx+6], dx
loc_15DC8:
    add     [bp+var_DE], 0Ch
    inc     [bp+var_E8]
loc_15DD1:
    cmp     [bp+var_E8], 4
    jl      short loc_15DDB
    jmp     loc_15EE2
loc_15DDB:
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_15DE6:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15DE6
    mov     cx, ax
    mov     al, [bp+var_E8]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, [bp+arg_0]
    mov     [bx+74h], cx
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_15E0F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15E0F
    mov     cx, ax
    mov     al, [bp+var_E8]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, [bp+arg_0]
    mov     [bx+76h], cx
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_15E38:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15E38
    mov     cx, ax
    mov     al, [bp+var_E8]
    cbw
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, [bp+arg_0]
    mov     [bx+78h], cx
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    push    di
    mov     bx, di
    shl     bx, 1
    add     bx, bp
    push    word ptr [bx-16h]
    push    [bp+arg_0]
    push    cs
    call    near ptr sub_185F6
    add     sp, 6
    mov     [bp+var_EE], ax
    cmp     word_44998, 0
    jnz     short loc_15E85
    cmp     word_44986, 0
    jnz     short loc_15E85
    jmp     loc_15DB6
loc_15E85:
    mov     [bp+var_1C2], 0
    mov     [bp+var_1C6], 0
    mov     ax, [bp+var_EE]
    add     ax, 180h
    mov     [bp+var_1C4], ax
    lea     ax, [bp+var_182]
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_182]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx], ax
    adc     [bx+2], dx
    mov     ax, [bp+var_180]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+4], ax
    adc     [bx+6], dx
    mov     ax, [bp+var_17E]
    cwd
    mov     bx, [bp+var_DE]
    add     [bx+8], ax
    adc     [bx+0Ah], dx
    jmp     loc_15DC8
    ; align 2
    db 144
loc_15EE2:
    mov     ax, [bp+var_1C0]
    mov     dx, [bp+var_1BE]
    add     ax, [bp+var_1B4]
    adc     dx, [bp+var_1B2]
    add     ax, [bp+var_1A8]
    adc     dx, [bp+var_1A6]
    add     ax, [bp+var_19C]
    adc     dx, [bp+var_19A]
    mov     cl, 2
loc_15F04:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15F04
    mov     word_449BE, ax
    mov     word_449C0, dx
    mov     ax, [bp+var_1BC]
    mov     dx, [bp+var_1BA]
    add     ax, [bp+var_1B0]
    adc     dx, [bp+var_1AE]
    add     ax, [bp+var_1A4]
    adc     dx, [bp+var_1A2]
    add     ax, [bp+var_198]
    adc     dx, [bp+var_196]
    mov     cl, 2
loc_15F35:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15F35
    mov     word_449C6, ax
    mov     word_449C8, dx
    mov     ax, [bp+var_1B8]
    mov     dx, [bp+var_1B6]
    add     ax, [bp+var_1AC]
    adc     dx, [bp+var_1AA]
    add     ax, [bp+var_1A0]
    adc     dx, [bp+var_19E]
    add     ax, [bp+var_194]
    adc     dx, [bp+var_192]
    mov     cl, 2
loc_15F66:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_15F66
    mov     word_449CA, ax
    mov     word_449CC, dx
    lea     ax, [bp+var_1C0]
    mov     [bp+var_DE], ax
    mov     [bp+var_E8], 0
loc_15F82:
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    mov     bx, [bp+var_DE]
    mov     ax, [bx]
    sub     ax, word_449BE
    mov     [di-1DEh], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+4]
    sub     ax, word_449C6
    mov     [di-1DCh], ax
    mov     bx, [bp+var_DE]
    mov     ax, [bx+8]
    sub     ax, word_449CA
    mov     [di-1DAh], ax
    add     [bp+var_DE], 0Ch
    inc     [bp+var_E8]
    cmp     [bp+var_E8], 4
    jl      short loc_15F82
    cmp     word_449C8, 0
    jge     short loc_15FDE
    sub     ax, ax
    mov     word_449C8, ax
    mov     word_449C6, ax
loc_15FDE:
    cmp     word_449C0, 1Dh
    jl      short loc_15FFE
    jg      short loc_15FEF
    cmp     word_449BE, 0F100h
    jbe     short loc_15FFE
loc_15FEF:
    mov     word_449BE, 0F0FFh
    mov     word_449C0, 1Dh
    jmp     short loc_1601B
    ; align 2
    db 144
loc_15FFE:
    cmp     word_449C0, 0
    jg      short loc_1601B
    jl      short loc_1600F
    cmp     word_449BE, 0F00h
    jnb     short loc_1601B
loc_1600F:
    mov     word_449BE, 0F00h
    mov     word_449C0, 0
loc_1601B:
    cmp     word_449CC, 1Dh
    jl      short loc_1603A
    jg      short loc_1602C
    cmp     word_449CA, 0F100h
    jbe     short loc_1603A
loc_1602C:
    mov     word_449CA, 0F0FFh
    mov     word_449CC, 1Dh
    jmp     short loc_16057
loc_1603A:
    cmp     word_449CC, 0
    jg      short loc_16057
    jl      short loc_1604B
    cmp     word_449CA, 0F00h
    jnb     short loc_16057
loc_1604B:
    mov     word_449CA, 0F00h
    mov     word_449CC, 0
loc_16057:
    mov     ax, [bp+var_1CC]
    add     ax, [bp+var_1D2]
    sub     ax, [bp+var_1DE]
    sub     ax, [bp+var_1D8]
    mov     [bp+var_EE], ax
    mov     ax, [bp+var_1C8]
    add     ax, [bp+var_1CE]
    sub     ax, [bp+var_1DA]
    sub     ax, [bp+var_1D4]
    mov     [bp+var_F2], ax
    neg     ax
    push    ax
    push    [bp+var_EE]
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     word_4435E, ax
    push    ax
    lea     ax, [bp+var_10E]
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     [bp+var_E8], 0
loc_160A7:
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    sub     di, 1DEh
    push    si
    push    di
    mov     si, di
    lea     di, [bp+var_FC]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    push    di
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_FC]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    inc     [bp+var_E8]
    cmp     [bp+var_E8], 4
    jl      short loc_160A7
    mov     ax, [bp+var_1C8]
    add     ax, [bp+var_1CE]
    sub     ax, [bp+var_1DA]
    sub     ax, [bp+var_1D4]
    mov     [bp+var_F2], ax
    mov     ax, [bp+var_1CA]
    add     ax, [bp+var_1D0]
    sub     ax, [bp+var_1DC]
    sub     ax, [bp+var_1D6]
    mov     [bp+var_F4], ax
    or      ax, ax
    jnz     short loc_1611C
    cmp     [bp+var_F2], 0
    jl      short loc_16146
loc_1611C:
    push    [bp+var_F4]
    mov     ax, [bp+var_F2]
    neg     ax
    push    ax
    call    sub_2EA4E
    add     sp, 4
    sub     ax, 100h
    mov     word_44986, ax
    or      ax, ax
    jge     short loc_1613E
    neg     ax
    jmp     short loc_16141
    ; align 2
    db 144
loc_1613E:
    mov     ax, word_44986
loc_16141:
    cmp     ax, 2
    jge     short loc_1614C
loc_16146:
    mov     word_44986, 0
loc_1614C:
    cmp     word_44986, 0
    jz      short loc_161AB
    push    word_44986
    lea     ax, [bp+var_10E]
    push    ax
    call    mat_rot_x
    add     sp, 4
    mov     [bp+var_E8], 0
loc_16169:
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    sub     di, 1DEh
    push    si
    push    di
    mov     si, di
    lea     di, [bp+var_FC]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    push    di
    lea     ax, [bp+var_10E]
    push    ax
    lea     ax, [bp+var_FC]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    inc     [bp+var_E8]
    cmp     [bp+var_E8], 4
    jl      short loc_16169
loc_161AB:
    mov     ax, [bp+var_1D8]
    add     ax, [bp+var_1D2]
    sub     ax, [bp+var_1DE]
    sub     ax, [bp+var_1CC]
    mov     [bp+var_F2], ax
    mov     ax, [bp+var_1D6]
    add     ax, [bp+var_1D0]
    sub     ax, [bp+var_1DC]
    sub     ax, [bp+var_1CA]
    mov     [bp+var_F4], ax
    or      ax, ax
    jnz     short loc_161DE
    cmp     [bp+var_F2], 0
    jg      short loc_16204
loc_161DE:
    push    [bp+var_F4]
    push    [bp+var_F2]
    call    sub_2EA4E
    add     sp, 4
    sub     ax, 100h
    mov     word_44998, ax
    or      ax, ax
    jge     short loc_161FC
    neg     ax
    jmp     short loc_161FF
loc_161FC:
    mov     ax, word_44998
loc_161FF:
    cmp     ax, 2
    jge     short loc_1620A
loc_16204:
    mov     word_44998, 0
loc_1620A:
    mov     bx, [bp+arg_0]
    mov     di, bx
    mov     al, [di+0C2h]
    add     al, [di+0C3h]
    mov     [bx+0BFh], al
    mov     bx, [bp+arg_0]
    mov     di, bx
    mov     al, [di+0C4h]
    add     al, [di+0C5h]
    mov     [bx+0C0h], al
    cmp     byte_44889, 2
    jnz     short loc_16236
    jmp     loc_16840
loc_16236:
    cmp     byte_454B8, 0
    jnz     short loc_1625F
    cmp     [bp+arg_8], 0
    jz      short loc_1624A
    push    word_4408C
    jmp     short loc_1624E
    ; align 2
    db 144
loc_1624A:
    push    word_43964
loc_1624E:
    mov     bx, [bp+arg_0]
    mov     al, [bx+0CFh]
    sub     ah, ah
    push    ax
    push    cs
    call    near ptr sub_18CD8
    add     sp, 4
loc_1625F:
    sub     ax, ax
    push    ax
    mov     ax, word_4435E
    neg     ax
    push    ax
    mov     ax, word_44986
    neg     ax
    push    ax
    mov     ax, word_44998
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_EA], ax
    mov     [bp+var_E8], 0
    jmp     loc_1632C
loc_16288:
    mov     ax, word_44DC6
    mov     [bp+var_E], ax
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    push    si
    lea     si, [bx+di+8Ch]
    lea     di, [bp+var_1C6]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    lea     ax, [bp+var_17C]
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    sub_1E1A0
    add     sp, 4
    mov     ax, word_44DC6
    cmp     [bp+var_E], ax
    jnz     short loc_16309
    push    [bp+var_1C2]
    push    [bp+var_1C4]
    push    [bp+var_1C6]
    push    ax
    push    cs
    call    near ptr sub_19926
    add     sp, 8
    mov     [bp+var_138], ax
    cmp     byte_45DB2, 1
    jz      short loc_16309
    or      si, si
    jge     short loc_162EE
    or      ax, ax
    jg      short loc_162F9
loc_162EE:
    or      si, si
    jle     short loc_16309
    cmp     [bp+var_138], 0
    jge     short loc_16309
loc_162F9:
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 5
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_16309:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     bx, [bp+arg_0]
    push    si
    lea     di, [bx+di+8Ch]
    lea     si, [bp+var_17C]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     si
    inc     [bp+var_E8]
loc_1632C:
    cmp     [bp+var_E8], 4
    jl      short loc_16336
    jmp     loc_16428
loc_16336:
    mov     al, [bp+var_E8]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     bx, [bp+arg_2]
    push    si
    lea     si, [bx+di+0D2h]
    lea     di, [bp+var_1C6]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     bx, [bp+arg_2]
    mov     ax, [bx+0CAh]
    mov     cl, 6
    shl     ax, cl
    mov     [bp+var_1C4], ax
    lea     ax, [bp+var_FC]
    push    ax
    push    [bp+var_EA]
    lea     ax, [bp+var_1C6]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, [bp+var_FC]
    cwd
    add     ax, word_449BE
    adc     dx, word_449C0
    mov     cl, 6
loc_16389:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16389
    mov     [bp+var_1C6], ax
    mov     ax, [bp+var_FA]
    cwd
    add     ax, word_449C6
    adc     dx, word_449C8
    mov     cl, 6
loc_163A4:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_163A4
    mov     [bp+var_1C4], ax
    mov     ax, [bp+var_F8]
    cwd
    add     ax, word_449CA
    adc     dx, word_449CC
    mov     cl, 6
loc_163BF:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_163BF
    mov     [bp+var_1C2], ax
    push    si
    lea     di, [bp+var_17C]
    lea     si, [bp+var_1C6]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     al, [bp+var_E8]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+arg_0]
    add     ax, 8Ch ; 'Œ'
    push    ax
    lea     ax, [bp+var_1C6]
    push    ax
    call    sub_1E1A0
    add     sp, 4
    push    [bp+var_1C2]
    push    [bp+var_1C4]
    push    [bp+var_1C6]
    push    word_44DC6
    push    cs
    call    near ptr sub_19926
    add     sp, 8
    mov     si, ax
    cmp     word_44DC6, 4
    jl      short loc_1641E
    jmp     loc_16288
loc_1641E:
    or      si, si
    jle     short loc_16425
    jmp     loc_16309
loc_16425:
    jmp     loc_162F9
loc_16428:
    mov     bx, [bp+arg_0]
    mov     al, [bx+0BFh]
    add     al, [bx+0C0h]
    mov     [bp+var_11C], al
    cmp     [bp+arg_8], 0
    jnz     short loc_1644C
    or      al, al
    jnz     short loc_1644C
    cmp     byte ptr [bx+0C1h], 0
    jz      short loc_1644C
    inc     word_445E4
loc_1644C:
    mov     al, [bp+var_11C]
    mov     [bx+0C1h], al
    mov     ax, word_449BE
    mov     dx, word_449C0
    mov     cl, 6
loc_1645D:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1645D
    mov     [bp+var_11A], ax
    mov     ax, word_449C6
    mov     dx, word_449C8
    mov     cl, 6
loc_16472:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16472
    mov     [bp+var_118], ax
    mov     ax, word_449CA
    mov     dx, word_449CC
    mov     cl, 6
loc_16487:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16487
    mov     [bp+var_116], ax
    mov     ax, word_44998
    mov     [bp+var_114], ax
    mov     ax, word_44986
    mov     [bp+var_112], ax
    mov     ax, word_4435E
    mov     [bp+var_110], ax
    cmp     byte_449AA, 0
    jnz     short loc_164B2
    jmp     loc_16578
loc_164B2:
    mov     bx, [bp+arg_4]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     cl, 6
loc_164BC:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_164BC
    mov     [bp+var_18E], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+4]
    mov     dx, [bx+6]
    mov     cl, 6
loc_164D3:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_164D3
    mov     [bp+var_18C], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+8]
    mov     dx, [bx+0Ah]
    mov     cl, 6
loc_164EA:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_164EA
    mov     [bp+var_18A], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+1Ch]
    mov     [bp+var_188], ax
    mov     ax, [bx+1Ah]
    mov     [bp+var_186], ax
    mov     ax, [bx+18h]
    mov     [bp+var_184], ax
    lea     ax, [bp+var_18E]
    push    ax
    mov     ax, [bp+arg_6]
    add     ax, 0C8h ; 'È'
    push    ax
    lea     ax, [bp+var_11A]
    push    ax
    mov     ax, [bp+arg_2]
    add     ax, 0C8h ; 'È'
    push    ax
    push    cs
    call    near ptr sub_19074
    add     sp, 8
    or      al, al
    jz      short loc_16578
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C8h], 0
    jz      short loc_1653E
    jmp     loc_16892
loc_1653E:
    push    [bp+arg_4]
    push    bx
    push    cs
    call    near ptr sub_18466
    add     sp, 4
    or      al, al
    jnz     short loc_16550
    jmp     loc_16892
loc_16550:
    mov     al, [bp+arg_8]
    cbw
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
    mov     al, [bp+arg_8]
    cbw
    xor     al, 1
loc_16566:
    push    ax
    mov     ax, 1
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_16578:
    mov     ax, [bp+var_11A]
    mov     cl, 0Ah
    sar     ax, cl
    mov     [bp+var_FC], ax
    mov     ax, [bp+var_116]
    sar     ax, cl
    sub     ax, 1Dh
    neg     ax
    mov     [bp+var_F8], ax
    mov     [bp+var_188], 0
    mov     [bp+var_186], 0
    mov     [bp+var_184], 0
    cmp     [bp+var_FC], 0
    jge     short loc_165AF
    jmp     loc_16840
loc_165AF:
    cmp     [bp+var_FC], 1Eh
    jl      short loc_165B9
    jmp     loc_16840
loc_165B9:
    or      ax, ax
    jge     short loc_165C0
    jmp     loc_16840
loc_165C0:
    cmp     ax, 1Eh
    jl      short loc_165C8
    jmp     loc_16840
loc_165C8:
    lea     ax, [bp+var_DC]
    push    ax
    push    [bp+var_F8]
    push    [bp+var_FC]
    call    sub_1FAE4
    add     sp, 6
    mov     [bp+var_EC], al
    or      al, al
    jz      short loc_16650
    sub     si, si
    jmp     short loc_165F0
    ; align 2
    db 144
loc_165EA:
    add     [bp+var_144], 6
    inc     si
loc_165F0:
    mov     al, [bp+var_EC]
    sub     ah, ah
    cmp     ax, si
    jbe     short loc_16650
    mov     ax, si
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    mov     ax, [di-0DCh]
    mov     [bp+var_18E], ax
    mov     ax, [di-0DAh]
    mov     [bp+var_18C], ax
    mov     ax, [di-0D8h]
    mov     [bp+var_18A], ax
    lea     ax, [bp+var_18E]
    push    ax
    mov     ax, 5FAh
    push    ax
    lea     ax, [bp+var_11A]
    push    ax
    mov     ax, [bp+arg_2]
    add     ax, 0C8h ; 'È'
    push    ax
    push    cs
    call    near ptr sub_19074
    add     sp, 8
    or      al, al
    jz      short loc_165EA
    mov     bx, [bp+arg_0]
    sub     word ptr [bx+36h], 200h
loc_16648:
    mov     al, [bp+arg_8]
    cbw
    jmp     loc_16566
    ; align 2
    db 144
loc_16650:
    mov     bx, [bp+var_F8]
    shl     bx, 1
    mov     bx, [bx-5A30h]
    add     bx, [bp+var_FC]
    les     di, dword_45E56
    mov     al, es:[bx+di]
    cbw
    mov     si, ax
    cmp     si, 0FFFFh
    jnz     short loc_16670
    jmp     loc_16710
loc_16670:
    cmp     byte ptr [si-6EE2h], 0
    jz      short loc_1667A
    jmp     loc_16710
loc_1667A:
    mov     bx, si
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    les     di, dword_42A30
    mov     ax, es:[bx+di]
    mov     [bp+var_18E], ax
    mov     di, si
    mov     ax, di
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     bx, word ptr dword_42A30
    mov     ax, es:[bx+di+2]
    mov     [bp+var_18C], ax
    mov     di, si
    mov     ax, di
    shl     di, 1
    add     di, ax
    shl     di, 1
    mov     ax, es:[bx+di+4]
    mov     [bp+var_18A], ax
    lea     ax, [bp+var_18E]
    push    ax
    mov     ax, 5EAh
    push    ax
    lea     ax, [bp+var_11A]
    push    ax
    mov     ax, [bp+arg_2]
    add     ax, 0C8h ; 'È'
    push    ax
    push    cs
    call    near ptr sub_19074
    add     sp, 8
    or      al, al
    jz      short loc_16710
    mov     byte ptr [si-6EE2h], 1
    mov     ax, 3C00h
    cwd
    push    dx
    push    ax
    mov     ax, 580h
    cwd
    push    dx
    push    ax
    mov     bx, [bp+arg_0]
    sub     ax, ax
    push    ax
    push    word ptr [bx+2Ch]
    call    __aFlmul
    push    dx
    push    ax
    call    __aFuldiv
    push    ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+18h]
    neg     ax
    push    ax
    lea     ax, [si+2]
    push    ax
    push    cs
    call    near ptr sub_19A2C
    add     sp, 6
loc_16710:
    mov     al, byte_4499A
    cbw
    mov     di, ax
    cmp     [bp+var_FC], di
    jz      short loc_1671F
    jmp     loc_16840
loc_1671F:
    mov     al, byte_4499E
    cbw
    cmp     [bp+var_F8], ax
    jz      short loc_1672C
    jmp     loc_16840
loc_1672C:
    mov     ax, 7Eh ; '~'
    push    ax
    mov     ax, word_45516
    add     ah, 1
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     bx, di
    shl     bx, 1
    mov     cx, [bx-599Eh]
    add     cx, ax
    mov     [bp+var_18E], cx
    mov     al, byte_4499B
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+178h]
    mov     [bp+var_18C], ax
    mov     ax, 7Eh ; '~'
    push    ax
    mov     ax, word_45516
    add     ah, 1
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, ax
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-55EAh]
    mov     [bp+var_18A], cx
    lea     ax, [bp+var_18E]
    push    ax
    mov     ax, 5F2h
    push    ax
    lea     ax, [bp+var_11A]
    push    ax
    mov     ax, [bp+arg_2]
    add     ax, 0C8h ; 'È'
    push    ax
    push    cs
    call    near ptr sub_19074
    add     sp, 8
    cbw
    mov     [bp+var_138], ax
    or      ax, ax
    jnz     short loc_16836
    mov     ax, 7Eh ; '~'
    push    ax
    mov     ax, word_45516
    add     ah, 3
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-599Eh]
    mov     [bp+var_18E], cx
    mov     ax, 7Eh ; '~'
    push    ax
    mov     ax, word_45516
    add     ah, 3
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, ax
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-55EAh]
    mov     [bp+var_18A], cx
    lea     ax, [bp+var_18E]
    push    ax
    mov     ax, 5F2h
    push    ax
    lea     ax, [bp+var_11A]
    push    ax
    mov     ax, [bp+arg_2]
    add     ax, 0C8h ; 'È'
    push    ax
    push    cs
    call    near ptr sub_19074
    add     sp, 8
    cbw
    mov     [bp+var_138], ax
loc_16836:
    cmp     [bp+var_138], 0
    jz      short loc_16840
    jmp     loc_16648
loc_16840:
    mov     bx, [bp+arg_0]
    mov     ax, word_449BE
    mov     dx, word_449C0
    mov     [bx], ax
    mov     [bx+2], dx
    mov     bx, [bp+arg_0]
    mov     ax, word_449C6
    mov     dx, word_449C8
    mov     [bx+4], ax
    mov     [bx+6], dx
    mov     bx, [bp+arg_0]
    mov     ax, word_449CA
    mov     dx, word_449CC
    mov     [bx+8], ax
    mov     [bx+0Ah], dx
    mov     bx, [bp+arg_0]
    mov     ax, word_44998
    mov     [bx+1Ch], ax
    mov     bx, [bp+arg_0]
    mov     ax, word_44986
    mov     [bx+1Ah], ax
    mov     bx, [bp+arg_0]
    mov     ax, word_4435E
    mov     [bx+18h], ax
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C8h], 0
loc_16892:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_14DA2 endp
sub_16898 proc far
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = byte ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14
    arg_A = word ptr 16
    arg_C = word ptr 18
    arg_E = word ptr 20
    arg_10 = word ptr 22
    arg_12 = word ptr 24

    push    bp
    mov     bp, sp
    sub     sp, 0Eh
    push    di
    push    si
    sub     si, si
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_6]
    mov     dx, [bp+arg_8]
    mov     [bx], ax
    mov     [bx+2], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_6]
    mov     dx, [bp+arg_8]
    mov     [bx+0Ch], ax
    mov     [bx+0Eh], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_A]
    mov     dx, [bp+arg_C]
    add     ax, 200h
    adc     dx, si
    mov     [bx+4], ax
    mov     [bx+6], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_A]
    mov     dx, [bp+arg_C]
    mov     [bx+10h], ax
    mov     [bx+12h], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_E]
    mov     dx, [bp+arg_10]
    mov     [bx+8], ax
    mov     [bx+0Ah], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_E]
    mov     dx, [bp+arg_10]
    mov     [bx+14h], ax
    mov     [bx+16h], dx
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_12]
    mov     [bx+18h], ax
    mov     bx, [bp+arg_0]
    mov     [bx+1Ah], si
    mov     bx, [bp+arg_0]
    mov     [bx+1Ch], si
    mov     bx, [bp+arg_0]
    mov     [bx+36h], si
    mov     bx, [bp+arg_0]
    mov     [bx+1Eh], si
    mov     bx, [bp+arg_0]
    mov     [bx+20h], si
    mov     bx, [bp+arg_0]
    sub     al, al
    mov     [bx+0BCh], al
    mov     bx, [bp+arg_0]
    mov     [bx+0BDh], al
    mov     bx, [bp+arg_2]
    mov     ax, [bx+6]
    mov     bx, [bp+arg_0]
    mov     [bx+22h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+22h]
    mov     [bx+24h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+22h]
    mov     [bx+26h], ax
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0BEh], 1
    mov     bx, [bp+arg_0]
    mov     [bx+28h], si
    mov     bx, [bp+arg_0]
    mov     [bx+2Ah], si
    mov     bx, [bp+arg_0]
    mov     [bx+2Ch], si
    mov     bx, [bp+arg_0]
    mov     [bx+2Eh], si
    mov     bx, [bp+arg_2]
    mov     ax, [bx+10h]
    mov     bx, [bp+arg_0]
    mov     [bx+30h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+30h]
    mov     cl, 8
    shr     ax, cl
    mov     [bx+32h], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+20h]
    mov     bx, [bp+arg_0]
    mov     [bx+34h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+34h]
    mov     [bx+3Ah], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+22h]
    mov     bx, [bp+arg_0]
    mov     [bx+38h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+38h]
    mov     [bx+3Ch], ax
    mov     bx, [bp+arg_0]
    mov     [bx+3Eh], si
    mov     bx, [bp+arg_0]
    mov     [bx+40h], si
    mov     bx, [bp+arg_0]
    mov     [bx+42h], si
    mov     bx, [bp+arg_0]
    mov     [bx+48h], si
    mov     bx, [bp+arg_0]
    mov     [bx+4Ah], si
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0BFh], 2
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C0h], 2
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C1h], 4
    mov     bx, [bp+arg_0]
    mov     [bx+44h], si
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+46h], 3E8h
    mov     ax, [bp+arg_6]
    mov     dx, [bp+arg_8]
    mov     cl, 6
loc_16A04:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16A04
    mov     [bp+var_A], ax
    mov     ax, [bp+arg_A]
    mov     dx, [bp+arg_C]
    mov     cl, 6
loc_16A17:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16A17
    mov     [bp+var_8], ax
    mov     ax, [bp+arg_E]
    mov     dx, [bp+arg_10]
    mov     cl, 6
loc_16A2A:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_16A2A
    mov     [bp+var_6], ax
    sub     di, di
loc_16A37:
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+di+0C2h], 1
    mov     ax, di
    shl     ax, 1
    mov     [bp+var_C], ax
    mov     bx, [bp+arg_0]
    add     bx, ax
    mov     [bx+4Ch], si
    mov     bx, [bp+arg_0]
    add     bx, [bp+var_C]
    mov     [bx+54h], si
    mov     bx, [bp+arg_0]
    add     bx, [bp+var_C]
    mov     [bx+5Ch], si
    mov     bx, [bp+arg_0]
    add     bx, [bp+var_C]
    mov     [bx+64h], si
    mov     bx, [bp+arg_0]
    add     bx, [bp+var_C]
    mov     [bx+6Ch], si
    mov     ax, di
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     [bp+var_E], ax
    mov     bx, [bp+arg_0]
    add     bx, ax
    push    si
    push    di
    lea     di, [bx+74h]
    lea     si, [bp+var_A]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    mov     bx, [bp+arg_0]
    add     bx, [bp+var_E]
    push    si
    push    di
    lea     di, [bx+8Ch]
    lea     si, [bp+var_A]
    movsw
    movsw
    movsw
    pop     di
    pop     si
    inc     di
    cmp     di, 4
    jl      short loc_16A37
    mov     bx, [bp+arg_0]
    mov     ax, si
    mov     [bx+0C6h], al
    mov     bx, [bp+arg_0]
    mov     [bx+0C7h], al
    mov     bx, [bp+arg_0]
    mov     [bx+0C8h], al
    mov     bx, [bp+arg_0]
    mov     [bx+0C9h], al
    mov     bx, [bp+arg_0]
    mov     [bx+0CAh], al
    mov     bx, [bp+arg_0]
    mov     [bx+0CBh], al
    mov     bx, [bp+arg_0]
    mov     al, [bp+arg_4]
    mov     [bx+0CCh], al
    mov     bx, [bp+arg_0]
    mov     ax, si
    mov     [bx+0CDh], al
    mov     bx, [bp+arg_0]
    mov     [bx+0CEh], al
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0CFh], 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_16898 endp
init_game_state proc far
    var_A = word ptr -10
    var_8 = word ptr -8
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    sub     si, si
    cmp     [bp+arg_0], 0FFFFh
    jnz     short loc_16B4D
    mov     word_45A24, si
    sub     di, di
loc_16B18:
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, di
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, 3F4h
    adc     dx, 0
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     es, dx
    mov     bx, ax
    mov     ax, si
    mov     es:[bx], al
    inc     di
    cmp     di, 14h
    jl      short loc_16B18
loc_16B4D:
    cmp     word_449D0, 0Ah
    jnz     short loc_16B5C
    mov     word_449E8, 64Ah
    jmp     short loc_16B62
loc_16B5C:
    mov     word_449E8, 60Ah
loc_16B62:
    mov     ax, 1Eh
    imul    word_449D0
    mov     word_45A00, ax
    mov     ax, 64h ; 'd'
    cwd
    mov     cx, word_449D0
    idiv    cx
    mov     word_4499C, ax
    cmp     [bp+arg_0], 0FFFDh
    jnz     short loc_16B82
    jmp     loc_16F34
loc_16B82:
    call    sub_22532
    mov     byte_44888, 1
    mov     word_445CE, 1
    mov     ax, si
    mov     byte_44889, al
    mov     byte_4488A, al
    mov     word_445CC, si
    mov     word_44788, si
    mov     byte_4488B, al
    mov     byte_4488C, al
    mov     di, si
    jmp     short loc_16BB3
loc_16BAC:
    mov     ax, si
    mov     [di-6EE2h], al
    inc     di
loc_16BB3:
    cmp     di, 30h ; '0'
    jl      short loc_16BAC
    mov     di, si
    jmp     short loc_16BC5
loc_16BBC:
    mov     bx, di
    shl     bx, 1
    mov     [bx-6F4Eh], si
    inc     di
loc_16BC5:
    cmp     di, 18h
    jl      short loc_16BBC
    mov     ax, 200h
    push    ax
    mov     ax, word_45516
    add     ah, 3
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 1000h
    push    cx
    mov     cx, word_45516
    add     ch, 2
    push    cx
    mov     [bp+var_A], ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, [bp+var_A]
    mov     cx, ax
    mov     al, byte_4499A
    cbw
    mov     dx, cx
    mov     cl, 0Ah
    shl     ax, cl
    add     dx, ax
    mov     word_445B4, dx
    mov     al, byte_4499B
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+178h]
    add     ax, 3C0h
    mov     word_445B6, ax
    mov     ax, 200h
    push    ax
    mov     ax, word_45516
    add     ah, 3
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 1000h
    push    cx
    mov     cx, word_45516
    add     ch, 2
    push    cx
    mov     [bp+var_A], ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, ax
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    add     cx, [bx-7E84h]
    add     cx, [bp+var_A]
    mov     word_445B8, cx
    push    si
    push    di
    mov     di, 8E4Ah
    mov     si, 8E44h
    push    ds
    pop     es
    movsw
    movsw
    movsw
    pop     di
    pop     si
    push    si
    push    di
    mov     di, 8E50h
    mov     si, 8E44h
    movsw
    movsw
    movsw
    pop     di
    pop     si
    push    si
    push    di
    mov     di, 8E56h
    mov     si, 8E44h
    movsw
    movsw
    movsw
    pop     di
    pop     si
    sub     ax, ax
    mov     word_445D2, ax
    mov     word_445D0, ax
    mov     word_445D4, si
    mov     word_445D6, si
    mov     word_445D8, si
    mov     word_445DA, si
    mov     word_445DC, si
    mov     word_445DE, si
    mov     word_445E0, si
    mov     word_445E2, si
    mov     word_445E4, si
    mov     ax, 0D2h ; 'Ò'
    push    ax
    mov     ax, word_45516
    add     ah, 2
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 24h ; '$'
    push    cx
    mov     cx, word_45516
    add     ch, 1
    push    cx
    mov     [bp+var_A], ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, [bp+var_A]
    mov     [bp+var_8], ax
    mov     ax, 0D2h ; 'Ò'
    push    ax
    mov     ax, word_45516
    add     ah, 2
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 24h ; '$'
    push    cx
    mov     cx, word_45516
    add     ch, 1
    push    cx
    mov     [bp+var_A], ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, [bp+var_A]
    mov     [bp+var_2], ax
    mov     ax, word_45516
    neg     ax
    push    ax
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    add     ax, [bp+var_2]
    cwd
    mov     cl, 6
loc_16D6F:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16D6F
    push    dx
    push    ax
    mov     al, byte_4499B
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+178h]
    cwd
    mov     cl, 6
loc_16D88:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16D88
    push    dx
    push    ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    add     ax, [bp+var_8]
    cwd
    mov     cl, 6
loc_16DA4:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16DA4
    push    dx
    push    ax
    mov     al, byte_449A9
    cbw
    push    ax
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    push    cs
    call    near ptr sub_16898
    add     sp, 14h
    mov     word_44786, si
    mov     ax, si
    mov     byte_448F1, al
    mov     byte_448F2, al
    mov     byte_448EF, al
    mov     byte_448F0, al
    mov     al, byte_4499A
    cbw
    mov     word_4478A, ax
    mov     word_4478C, ax
    mov     al, byte_4499E
    cbw
    mov     word_4478E, ax
    mov     word_44790, ax
    cmp     [bp+arg_0], 0FFFEh
    jz      short loc_16E0A
    sub     ax, ax
    push    ax
    mov     al, byte_446B4
    inc     byte_446B4
    sub     ah, ah
    push    ax
    mov     ax, 8F1Ah
    push    ax
    push    word_44630
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
loc_16E0A:
    mov     ax, 0D2h ; 'Ò'
    push    ax
    mov     ax, word_45516
    add     ah, 2
    push    ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 24h ; '$'
    push    cx
    mov     cx, word_45516
    add     ch, 3
    push    cx
    mov     [bp+var_A], ax
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, [bp+var_A]
    mov     [bp+var_8], ax
    mov     ax, 0D2h ; 'Ò'
    push    ax
    mov     ax, word_45516
    add     ah, 2
    push    ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     cx, 24h ; '$'
    push    cx
    mov     cx, word_45516
    add     ch, 3
    push    cx
    mov     [bp+var_A], ax
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, [bp+var_A]
    mov     [bp+var_2], ax
    mov     ax, word_45516
    neg     ax
    push    ax
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    add     ax, [bp+var_2]
    cwd
    mov     cl, 6
loc_16EA6:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16EA6
    push    dx
    push    ax
    mov     al, byte_4499B
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx+178h]
    cwd
    mov     cl, 6
loc_16EBF:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16EBF
    push    dx
    push    ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    add     ax, [bp+var_8]
    cwd
    mov     cl, 6
loc_16EDB:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_16EDB
    push    dx
    push    ax
    mov     ax, 1
    push    ax
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    push    cs
    call    near ptr sub_16898
    add     sp, 14h
    cmp     byte_449AA, 0
    jz      short loc_16F2F
    cmp     [bp+arg_0], 0FFFEh
    jz      short loc_16F2F
    mov     ax, 911Dh
    push    ax
    mov     al, byte_44784
    inc     byte_44784
    sub     ah, ah
    push    ax
    mov     ax, 8FEAh
    push    ax
    mov     bx, word_44700
    shl     bx, 1
    add     bx, word ptr dword_43942
    mov     es, word ptr dword_43942+2
    push    word ptr es:[bx]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
loc_16F2F:
    mov     ax, si
    mov     byte_448BE, al
loc_16F34:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
init_game_state endp
sub_16F3A proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    cmp     [bp+arg_0], 0
    jnz     short loc_16F59
    cmp     word_45A24, 0
    jnz     short loc_16F59
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr init_game_state
    add     sp, 2
loc_16F59:
    mov     ax, [bp+arg_0]
    cwd
    mov     cx, word_45A00
    idiv    cx
    mov     si, ax
    cmp     si, 14h
    jnz     short loc_16F6B
    dec     si
loc_16F6B:
    mov     ax, word_445D4
    cmp     [bp+arg_0], ax
    jb      short loc_16FB1
loc_16F73:
    mov     ax, word_45A00
    imul    si
    cmp     ax, word_445D4
    ja      short loc_16F81
    jmp     loc_17002
loc_16F81:
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, si
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, 3F4h
    adc     dx, 0
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     es, dx
    mov     bx, ax
    cmp     byte ptr es:[bx], 0
    jz      short loc_16FFE
loc_16FB1:
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, si
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    push    si
    mov     di, 8D24h
    mov     si, ax
    push    ds
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 230h
    repne movsw
    pop     ds
    pop     si
    mov     ax, 9112h
    push    ax
    call    init_kevinrandom
    add     sp, 2
    mov     ax, word_445D4
    mov     word_42D02, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    db 144
    db 144
    db 144
loc_16FFE:
    dec     si
    jmp     loc_16F73
loc_17002:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_16F3A endp
sub_17008 proc far
    var_4 = byte ptr -4
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    mov     bx, word_445D4
    les     di, dword_4562E
    mov     al, es:[bx+di]
    mov     [bp+var_4], al
    or      al, al
    jz      short loc_17027
    mov     byte_44889, 1
loc_17027:
    mov     ax, bx
    sub     dx, dx
    div     word_45A00
    or      dx, dx
    jnz     short loc_17079
    mov     ax, bx
    sub     dx, dx
    div     word_45A00
    mov     si, ax
    mov     ax, 9112h
    push    ax
    call    sub_19E4E
    add     sp, 2
    mov     ax, 460h
    cwd
    push    dx
    push    ax
    mov     ax, si
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, word_45A20
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, word_45A22
    mov     bx, ax
    mov     es, dx
    push    si
    mov     di, bx
    mov     si, 8D24h
    mov     cx, 230h
    repne movsw
    pop     si
loc_17079:
    inc     word_445D4
    cmp     byte_4488A, 0
    jz      short loc_170BE
    mov     ax, word_445CE
    cmp     word_445CC, ax
    jge     short loc_170BE
    inc     word_445CC
    cmp     word_445CC, ax
    jnz     short loc_170BE
    cmp     byte_449DA, 0
    jnz     short loc_170BE
    cmp     byte_446AF, 1
    jnz     short loc_170B2
    cmp     word_44612, 0
    jz      short loc_170B2
    inc     word_445CE
    jmp     short loc_170BE
loc_170B2:
    cmp     byte_45DB2, 0
    jnz     short loc_170BE
    mov     byte_449DA, 1
loc_170BE:
    cmp     byte_44889, 0
    jz      short loc_170F6
    mov     al, [bp+var_4]
    cbw
    push    ax
    push    cs
    call    near ptr sub_171E8
    add     sp, 2
    cmp     byte_449AA, 0
    jz      short loc_170DC
    push    cs
    call    near ptr sub_14712
loc_170DC:
    call    sub_2298C
    cmp     byte_448BE, 0
    jz      short loc_170EC
    push    cs
    call    near ptr sub_19BA0
loc_170EC:
    push    cs
    call    near ptr sub_188A4
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_170F6:
    cmp     byte_45DB2, 1
    jz      short loc_17100
    jmp     loc_171E1
loc_17100:
    push    cs
    call    near ptr sub_188A4
    cmp     byte_4393C, 0
    jnz     short loc_1710E
    jmp     loc_171E1
loc_1710E:
    cmp     word_44DCA, 1C2h
    jge     short loc_1711B
    add     word_44DCA, 8
loc_1711B:
    cmp     byte_4393C, 1
    jnz     short loc_1712E
    cmp     word_44DCA, 180h
    jle     short loc_1712E
    inc     byte_4393C
loc_1712E:
    cmp     byte_4393C, 2
    jz      short loc_17138
    jmp     loc_171E1
loc_17138:
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    mov     cx, word_445EE
    mov     bx, word_445F0
    mov     dx, cx
    mov     cl, 6
loc_17150:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_17150
    sub     ax, dx
    push    ax
    push    word_45516
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     si, ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    mov     cx, word_445E6
    mov     bx, word_445E8
    mov     dx, cx
    mov     cl, 6
loc_1718A:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_1718A
    sub     ax, dx
    push    ax
    push    word_45516
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     si, ax
    cmp     si, 0E4h ; 'ä'
    jle     short loc_171D0
    cmp     word_44610, 500h
    jnb     short loc_171CC
    mov     ax, 1
loc_171BD:
    push    ax
    push    cs
    call    near ptr sub_171E8
    add     sp, 2
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_171CC:
    sub     ax, ax
    jmp     short loc_171BD
loc_171D0:
    cmp     word_44610, 0
    jz      short loc_171DC
    mov     ax, 2
    jmp     short loc_171BD
loc_171DC:
    mov     byte_4393C, 0
loc_171E1:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_17008 endp
sub_171E8 proc far
    var_52 = word ptr -82
    var_4E = word ptr -78
    var_3A = byte ptr -58
    var_38 = word ptr -56
    var_34 = word ptr -52
    var_32 = word ptr -50
    var_30 = word ptr -48
    var_2E = word ptr -46
    var_2C = byte ptr -44
    var_2A = byte ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = byte ptr -28
    var_1A = word ptr -26
    var_16 = word ptr -22
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 52h
    push    di
    push    si
    cmp     byte_4499F, 0
    jz      short loc_171FB
    dec     byte_4499F
loc_171FB:
    mov     byte_446B5, 1
    cmp     byte_446AF, 0
    jz      short loc_17242
    mov     byte_448F1, 0
    mov     [bp+arg_0], 2
    cmp     word_44612, 0
    jnz     short loc_17242
    mov     byte_446B5, 0
    cmp     word_44610, 0
    jnz     short loc_17242
    cmp     word_44632, 0
    jnz     short loc_17242
    cmp     word_44634, 0
    jnz     short loc_17242
    cmp     word_44636, 0
    jnz     short loc_17242
    cmp     word_44638, 0
    jnz     short loc_17242
    jmp     loc_17810
loc_17242:
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    sub     ax, ax
    push    ax
    mov     al, [bp+arg_0]
    cbw
    push    ax
    push    cs
    call    near ptr sub_17A78
    add     sp, 8
    mov     al, [bp+arg_0]
    cbw
    sar     ax, 1
    sar     ax, 1
smart
    and     ax, 3
nosmart
    push    ax
    push    cs
    call    near ptr sub_187B2
    add     sp, 2
    mov     ax, 1
    push    ax
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    push    cs
    call    near ptr sub_17FD6
    add     sp, 6
    sub     ax, ax
    push    ax
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    mov     ax, 0A6EAh
    push    ax
    mov     ax, 8E76h
    push    ax
    push    cs
    call    near ptr sub_14DA2
    add     sp, 0Ah
    mov     ax, word_44612
    sub     dx, dx
    add     word_445D0, ax
    adc     word_445D2, dx
    mov     al, byte_448EF
    mov     [bp+var_1C], al
    mov     ax, word_44786
    mov     [bp+var_2], ax
    lea     ax, [bp+var_1E]
    push    ax
    lea     ax, [bp+var_2]
    push    ax
    push    cs
    call    near ptr sub_17816
    add     sp, 4
    cbw
    mov     si, ax
    or      si, si
    jnz     short loc_172CB
    jmp     loc_173B3
loc_172CB:
    cmp     [bp+var_1E], 0FFFEh
    jnz     short loc_172D8
    mov     byte_448EF, 1
    jmp     short loc_172E4
loc_172D8:
    cmp     byte_448EF, 1
    jnz     short loc_172E9
    mov     byte_448EF, 0
loc_172E4:
    mov     byte_448F0, 0
loc_172E9:
    cmp     byte_448EF, 0
    jz      short loc_172F3
    jmp     loc_173AD
loc_172F3:
    cmp     [bp+var_2], 0
    jnz     short loc_17308
    cmp     word_44788, 0
    jz      short loc_17308
    inc     byte_446B3
    jmp     short loc_1737B
    ; align 4
    db 144
    db 144
loc_17308:
    cmp     [bp+var_1E], 0
    jl      short loc_17322
    cmp     [bp+var_1E], 3
    jge     short loc_17322
    mov     byte_448F0, 0
    mov     ax, [bp+var_2]
    mov     word_44786, ax
    jmp     loc_173AD
loc_17322:
    cmp     [bp+var_1E], 0FFFFh
    jz      short loc_1732E
    cmp     [bp+var_1E], 3
    jle     short loc_173AD
loc_1732E:
    mov     di, word_44788
    shl     di, 1
    les     bx, dword_42D22
    mov     ax, [bp+var_2]
    cmp     es:[bx+di], ax
    jz      short loc_17349
    les     bx, dword_438E8
    cmp     es:[bx+di], ax
    jnz     short loc_17350
loc_17349:
    inc     byte_448F0
    jmp     short loc_17374
    ; align 2
    db 144
loc_17350:
    mov     di, [bp+var_2]
    shl     di, 1
    les     bx, dword_42D22
    mov     ax, word_44788
    cmp     es:[bx+di], ax
    jz      short loc_1736A
    les     bx, dword_438E8
    cmp     es:[bx+di], ax
    jnz     short loc_1736F
loc_1736A:
    mov     byte_448EF, 2
loc_1736F:
    mov     byte_448F0, 1
loc_17374:
    cmp     byte_448F0, 3
    jl      short loc_173AD
loc_1737B:
    mov     ax, [bp+var_2]
    mov     word_44786, ax
    mov     byte_448F0, 0
    cmp     [bp+var_1E], 0
    jle     short loc_173AD
    mov     ax, [bp+var_1E]
    imul    word_449D0
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    mov     word_461CA, ax
    mov     al, byte ptr word_449D0
    shl     al, 1
    shl     al, 1
    mov     byte_4499F, al
    mov     ax, word_461CA
    add     word_445DE, ax
loc_173AD:
    mov     ax, [bp+var_2]
    mov     word_44788, ax
loc_173B3:
    mov     byte_448F1, 0
    cmp     byte_448EF, 1
    jnz     short loc_173C2
    jmp     loc_17810
loc_173C2:
    mov     ax, 1
    push    ax
    push    word_445FE
    push    word_44600
    push    word_44602
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_20], ax
    cmp     byte_448EF, 2
    jnz     short loc_173F6
    cmp     byte_446AF, 0
    jnz     short loc_173F0
    mov     byte_448F1, 3
loc_173F0:
    mov     ax, word_44788
    jmp     loc_174C9
loc_173F6:
    cmp     word_44630, 0FFFFh
    jnz     short loc_17402
loc_173FD:
    sub     si, si
    jmp     loc_174B3
loc_17402:
    cmp     [bp+var_1C], 0
    jz      short loc_1740F
    cmp     byte_448EF, 0
    jz      short loc_17431
loc_1740F:
    mov     ax, word_44786
    cmp     word_44630, ax
    jz      short loc_1743A
    mov     di, ax
    shl     di, 1
    les     bx, dword_42D22
    mov     ax, word_44630
    cmp     es:[bx+di], ax
    jz      short loc_1743A
    les     bx, dword_438E8
    cmp     es:[bx+di], ax
    jz      short loc_1743A
loc_17431:
    mov     word_44630, 0FFFFh
    jmp     short loc_173FD
    ; align 2
    db 144
loc_1743A:
    mov     ax, word_4468A
    mov     cx, word_445E6
    mov     bx, word_445E8
    mov     dx, cx
    mov     cl, 6
loc_17449:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_17449
    sub     ax, dx
    mov     [bp+var_32], ax
    cmp     word_4468C, 0FFFFh
    jz      short loc_1747C
    mov     ax, word_4468C
    mov     cx, word_445EA
    mov     bx, word_445EC
    mov     dx, cx
    mov     cl, 6
loc_1746C:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_1746C
    sub     ax, dx
    mov     [bp+var_30], ax
    jmp     short loc_17481
    ; align 2
    db 144
loc_1747C:
    mov     [bp+var_30], 0
loc_17481:
    mov     ax, word_4468E
    mov     cx, word_445EE
    mov     bx, word_445F0
    mov     dx, cx
    mov     cl, 6
loc_17490:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_17490
    sub     ax, dx
    mov     [bp+var_2E], ax
    lea     ax, [bp+var_38]
    push    ax
    push    [bp+var_20]
    lea     ax, [bp+var_32]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     si, [bp+var_34]
loc_174B3:
    cmp     si, 113h
    jl      short loc_174BC
    jmp     loc_17699
loc_174BC:
    cmp     word_44630, 0FFFFh
    jz      short loc_174C6
    jmp     loc_1764C
loc_174C6:
    mov     ax, word_44786
loc_174C9:
    mov     [bp+var_2], ax
    mov     bx, ax
    shl     bx, 1
    les     di, dword_438E8
    cmp     word ptr es:[bx+di], 0FFFFh
    jz      short loc_174DD
    jmp     loc_17771
loc_174DD:
    mov     [bp+var_2A], 0
    mov     [bp+var_2C], 0
loc_174E5:
    sub     ax, ax
    push    ax
    mov     al, [bp+var_2C]
    sub     ah, ah
    push    ax
    mov     ax, 8F1Ah
    push    ax
    push    [bp+var_2]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    mov     [bp+var_2A], al
    push    si
    lea     di, [bp+var_28]
    mov     si, 8F1Ah
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    mov     ax, word_445E6
    mov     dx, word_445E8
    mov     cl, 6
loc_17515:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_17515
    sub     [bp+var_28], ax
    cmp     [bp+var_26], 0FFFFh
    jnz     short loc_1753E
    mov     ax, word_445EA
    mov     dx, word_445EC
    mov     cl, 6
loc_1752F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1752F
    neg     ax
    mov     [bp+var_26], ax
    jmp     short loc_17552
loc_1753E:
    mov     ax, word_445EA
    mov     dx, word_445EC
    mov     cl, 6
loc_17547:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_17547
    sub     [bp+var_26], ax
loc_17552:
    mov     ax, word_445EE
    mov     dx, word_445F0
    mov     cl, 6
loc_1755B:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1755B
    sub     [bp+var_24], ax
    lea     ax, [bp+var_38]
    push    ax
    push    [bp+var_20]
    lea     ax, [bp+var_28]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_2C], 0
    jz      short loc_1758D
    mov     ax, [bp+var_2E]
    cmp     [bp+var_34], ax
    jge     short loc_17599
    cmp     [bp+var_34], 0
    jle     short loc_17599
loc_1758D:
    mov     al, [bp+var_2C]
    mov     [bp+var_3A], al
    mov     ax, [bp+var_34]
    mov     [bp+var_2E], ax
loc_17599:
    inc     [bp+var_2C]
    cmp     [bp+var_2A], 0
    jnz     short loc_175A5
    jmp     loc_174E5
loc_175A5:
    cmp     byte_448EF, 2
    jz      short loc_175AF
    jmp     loc_17640
loc_175AF:
    cmp     [bp+var_3A], 0
    jnz     short loc_175D0
    sub     ax, ax
    push    ax
    push    ax
    lea     ax, [bp+var_52]
    push    ax
    push    [bp+var_2]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    sub     ax, ax
    push    ax
    mov     ax, 1
    jmp     short loc_175F0
    ; align 2
    db 144
loc_175D0:
    sub     ax, ax
    push    ax
    mov     al, [bp+var_3A]
    dec     al
    cbw
    push    ax
    lea     ax, [bp+var_52]
    push    ax
    push    [bp+var_2]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    sub     ax, ax
    push    ax
    mov     al, [bp+var_3A]
    sub     ah, ah
loc_175F0:
    push    ax
    lea     ax, [bp+var_1A]
    push    ax
    push    [bp+var_2]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    mov     ax, [bp+var_16]
    sub     ax, [bp+var_4E]
    push    ax
    mov     ax, [bp+var_52]
    sub     ax, [bp+var_1A]
    push    ax
    call    sub_2EA4E
    add     sp, 4
    mov     si, ax
smart
    and     si, 3FFh
nosmart
    mov     ax, word_445FE
    sub     ax, si
smart
    and     ah, 3
nosmart
    mov     si, ax
    cmp     si, 380h
    jg      short loc_17631
    cmp     si, 80h ; '€'
    jge     short loc_1764C
loc_17631:
    mov     byte_448EF, 0
    mov     byte_448F0, 1
    mov     ax, [bp+var_2]
    jmp     short loc_17643
loc_17640:
    mov     ax, word_44786
loc_17643:
    mov     word_44630, ax
    mov     al, [bp+var_3A]
    mov     byte_446B4, al
loc_1764C:
    sub     ax, ax
    push    ax
    mov     al, byte_446B4
    inc     byte_446B4
    sub     ah, ah
    push    ax
    mov     ax, 8F1Ah
    push    ax
    push    word_44630
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    or      al, al
    jz      short loc_17699
    mov     bx, word_44786
    shl     bx, 1
    les     di, dword_438E8
    cmp     word ptr es:[bx+di], 0FFFFh
    jz      short loc_17684
    mov     word_44630, 0FFFFh
    jmp     short loc_17694
loc_17684:
    mov     bx, word_44786
    shl     bx, 1
    les     di, dword_42D22
    mov     ax, es:[bx+di]
    mov     word_44630, ax
loc_17694:
    mov     byte_446B4, 0
loc_17699:
    push    si
    lea     di, [bp+var_28]
    mov     si, 8F1Ah
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    cmp     word_44630, 0FFFFh
    jnz     short loc_176B0
    jmp     loc_17771
loc_176B0:
    cmp     byte_448EF, 0
    jz      short loc_176BA
    jmp     loc_17771
loc_176BA:
    mov     ax, word_445E6
    mov     dx, word_445E8
    mov     cl, 6
loc_176C3:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_176C3
    sub     [bp+var_28], ax
    cmp     [bp+var_26], 0FFFFh
    jnz     short loc_176DC
    mov     [bp+var_26], 0
    jmp     short loc_176F0
    ; align 2
    db 144
loc_176DC:
    mov     ax, word_445EA
    mov     dx, word_445EC
    mov     cl, 6
loc_176E5:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_176E5
    sub     [bp+var_26], ax
loc_176F0:
    mov     ax, word_445EE
    mov     dx, word_445F0
    mov     cl, 6
loc_176F9:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_176F9
    sub     [bp+var_24], ax
    mov     ax, 1
    push    ax
    push    word_445FE
    push    word_44600
    push    word_44602
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_20], ax
    lea     ax, [bp+var_38]
    push    ax
    push    [bp+var_20]
    lea     ax, [bp+var_28]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_34]
    mov     ax, [bp+var_38]
    neg     ax
    push    ax
    call    sub_2EA4E
    add     sp, 4
smart
    and     ah, 3
nosmart
    mov     word_4462E, ax
    cmp     byte_446AF, 0
    jnz     short loc_17771
    add     ax, 80h ; '€'
smart
    and     ah, 3
nosmart
    mov     cl, 8
    shr     ax, cl
    cmp     ax, 1
    jz      short loc_1776C
    cmp     ax, 3
    jz      short loc_1779E
loc_17764:
    mov     byte_448F1, 0
    jmp     short loc_17771
    ; align 2
    db 144
loc_1776C:
    mov     byte_448F1, 1
loc_17771:
    cmp     byte_446B3, 0
    jnz     short loc_1777B
    jmp     loc_17810
loc_1777B:
    mov     al, byte_4499E
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-55EAh]
    mov     cx, word_445EE
    mov     bx, word_445F0
    mov     dx, cx
    mov     cl, 6
loc_17793:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jz      short loc_177AC
    jmp     short loc_17793
    ; align 2
    db 144
loc_1779E:
    cmp     word_4469C, 0
    jnz     short loc_17764
    mov     byte_448F1, 2
    jmp     short loc_17771
loc_177AC:
    sub     ax, dx
    push    ax
    push    word_45516
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     si, ax
    mov     al, byte_4499A
    cbw
    mov     bx, ax
    shl     bx, 1
    mov     ax, [bx-599Eh]
    mov     cx, word_445E6
    mov     bx, word_445E8
    mov     dx, cx
    mov     cl, 6
loc_177DE:
    sar     bx, 1
    rcr     dx, 1
    dec     cl
    jnz     short loc_177DE
    sub     ax, dx
    push    ax
    push    word_45516
    call    sin_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    add     si, ax
    jns     short loc_17810
    sub     ax, ax
    push    ax
    mov     ax, 3
    push    ax
    push    cs
    call    near ptr sub_195E6
    add     sp, 4
loc_17810:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_171E8 endp
sub_17816 proc far
    var_5A4 = word ptr -1444
    var_5A2 = byte ptr -1442
    var_21A = word ptr -538
    var_218 = byte ptr -536
    var_116 = byte ptr -278
    var_114 = byte ptr -276
    var_110 = word ptr -272
    var_E = byte ptr -14
    var_C = byte ptr -12
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = byte ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 5A4h
    push    di
    push    si
    mov     al, byte ptr word_445E8
    mov     [bp+var_4], al
    mov     al, 1Dh
    sub     al, byte ptr word_445F0
    mov     [bp+var_A], al
    mov     al, [bp+var_4]
    cbw
    mov     [bp+var_5A4], ax
    mov     ax, word_4478A
    cmp     [bp+var_5A4], ax
    jz      short loc_17848
    mov     ax, word_4478C
    cmp     [bp+var_5A4], ax
    jnz     short loc_17872
loc_17848:
    mov     al, [bp+var_A]
    cbw
    mov     [bp+var_5A4], ax
    mov     ax, word_4478E
    cmp     [bp+var_5A4], ax
    jz      short loc_17862
    mov     ax, word_44790
    cmp     [bp+var_5A4], ax
    jnz     short loc_17872
loc_17862:
    mov     bx, [bp+arg_2]
    mov     word ptr [bx], 0
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_17872:
    cmp     [bp+var_4], 0
    jge     short loc_1787B
    jmp     loc_1792E
loc_1787B:
    cmp     [bp+var_4], 1Dh
    jle     short loc_17884
    jmp     loc_1792E
loc_17884:
    cmp     [bp+var_A], 0
    jge     short loc_1788D
    jmp     loc_1792E
loc_1788D:
    cmp     [bp+var_A], 1Dh
    jle     short loc_17896
    jmp     loc_1792E
loc_17896:
    mov     [bp+var_6], 0
    mov     [bp+var_21A], 0
    sub     di, di
    sub     si, si
    jmp     short loc_178AE
    ; align 2
    db 144
loc_178A8:
    mov     [bp+si+var_5A2], 0
    inc     si
loc_178AE:
    cmp     word_45DD0, si
    jg      short loc_178A8
    mov     bx, [bp+arg_0]
    mov     si, [bx]
loc_178B9:
    mov     bx, si
    shl     bx, 1
    add     bx, word ptr dword_42D22
    mov     es, word ptr dword_42D22+2
    mov     ax, es:[bx]
    mov     [bp+var_2], ax
    mov     bx, ax
    add     bx, bp
    cmp     byte ptr [bx-5A2h], 0
    jz      short loc_17938
    cmp     [bp+var_21A], 0
    jz      short loc_178FA
    dec     [bp+var_21A]
    mov     ax, [bp+var_21A]
    shl     ax, 1
    add     ax, bp
    mov     [bp+var_5A4], ax
    mov     bx, ax
    mov     si, [bx-216h]
    mov     di, [bx-10Eh]
    jmp     short loc_178B9
    ; align 2
    db 144
loc_178FA:
    cmp     [bp+var_6], 0
    jz      short loc_1791A
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_110]
    mov     [bx], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bp+var_6]
    mov     [bx], ax
loc_17911:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_1791A:
    mov     al, [bp+var_4]
    cbw
    mov     word_4478A, ax
    mov     word_4478C, ax
    mov     al, [bp+var_A]
    cbw
    mov     word_4478E, ax
    mov     word_44790, ax
loc_1792E:
    mov     bx, [bp+arg_2]
    mov     word ptr [bx], 0FFFEh
    jmp     short loc_17911
    ; align 2
    db 144
loc_17938:
    mov     bx, [bp+var_2]
    add     bx, bp
    mov     byte ptr [bx-5A2h], 1
    mov     bx, [bp+var_2]
    add     bx, word ptr dword_459DA
    mov     es, word ptr dword_459DA+2
    mov     al, es:[bx]
    mov     [bp+var_E], al
    mov     bx, [bp+var_2]
    add     bx, word ptr dword_449D4
    mov     es, word ptr dword_449D4+2
    mov     al, es:[bx]
    sub     ah, ah
    mov     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    add     bx, ax
    shl     bx, 1
    mov     al, [bx+20A3h]
    mov     [bp+var_116], al
    test    [bp+var_116], 1
    jz      short loc_17986
    mov     al, [bp+var_E]
    inc     al
    jmp     short loc_17989
    ; align 2
    db 144
loc_17986:
    mov     al, [bp+var_E]
loc_17989:
    mov     [bp+var_218], al
    mov     bx, [bp+var_2]
    add     bx, word ptr dword_4563C
    mov     es, word ptr dword_4563C+2
    mov     al, es:[bx]
    mov     [bp+var_C], al
    test    [bp+var_116], 2
    jz      short loc_179AA
    inc     al
    jmp     short loc_179AD
    ; align 2
    db 144
loc_179AA:
    mov     al, [bp+var_C]
loc_179AD:
    mov     [bp+var_114], al
    mov     al, [bp+var_4]
    cmp     [bp+var_C], al
    jz      short loc_179BF
    cmp     [bp+var_114], al
    jnz     short loc_17A19
loc_179BF:
    mov     al, [bp+var_A]
    cmp     [bp+var_E], al
    jz      short loc_179CD
    cmp     [bp+var_218], al
    jnz     short loc_17A19
loc_179CD:
    mov     bx, si
    shl     bx, 1
    add     bx, word ptr dword_438E8
    mov     es, word ptr dword_438E8+2
    cmp     word ptr es:[bx], 0FFFFh
    jz      short loc_179E2
    mov     [bp+var_2], si
loc_179E2:
    mov     al, [bp+var_C]
    cbw
    mov     word_4478A, ax
    mov     al, [bp+var_114]
    cbw
    mov     word_4478C, ax
    mov     al, [bp+var_E]
    cbw
    mov     word_4478E, ax
    mov     al, [bp+var_218]
    cbw
    mov     word_44790, ax
    or      di, di
    jle     short loc_17A5E
    cmp     [bp+var_6], 0
    jz      short loc_17A0F
    cmp     [bp+var_6], di
    jle     short loc_17A19
loc_17A0F:
    mov     ax, [bp+var_2]
    mov     [bp+var_110], ax
    mov     [bp+var_6], di
loc_17A19:
    mov     bx, si
    shl     bx, 1
    add     bx, word ptr dword_438E8
    mov     es, word ptr dword_438E8+2
    mov     ax, es:[bx]
    mov     [bp+var_8], ax
    cmp     ax, 0FFFFh
    jz      short loc_17A4F
    mov     bx, [bp+var_21A]
    shl     bx, 1
    add     bx, bp
    mov     [bx-10Eh], di
    mov     bx, [bp+var_21A]
    inc     [bp+var_21A]
    shl     bx, 1
    add     bx, bp
    mov     ax, [bp+var_8]
    mov     [bx-216h], ax
loc_17A4F:
    cmp     [bp+var_2], 0
    jz      short loc_17A6E
    cmp     di, 0FFFFh
    jz      short loc_17A71
    inc     di
    jmp     short loc_17A71
    ; align 2
    db 144
loc_17A5E:
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_2]
    mov     [bx], ax
    mov     bx, [bp+arg_2]
    mov     [bx], di
    jmp     loc_17911
loc_17A6E:
    mov     di, 0FFFFh
loc_17A71:
    mov     si, [bp+var_2]
    jmp     loc_178B9
    ; align 2
    db 144
sub_17816 endp
sub_17A78 proc far
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = byte ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    cmp     word_449D0, 14h
    jnz     short loc_17A8E
    mov     [bp+var_2], 6
    jmp     short loc_17A93
loc_17A8E:
    mov     [bp+var_2], 0Ch
loc_17A93:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0C6h], 0
    jz      short loc_17AA1
    dec     byte ptr [bx+0C6h]
loc_17AA1:
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+2Ch]
    sub     ax, [si+2Eh]
    mov     [bx+28h], ax
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+2Ch]
    mov     [bx+2Eh], ax
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+22h]
    mov     [bx+24h], ax
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0CCh], 0
    jnz     short loc_17AE6
    cmp     byte ptr [bx+0CAh], 0
    jnz     short loc_17AE6
    test    [bp+arg_0], 10h
    jnz     short loc_17B0F
    test    [bp+arg_0], 20h
    jnz     short loc_17B2E
    jmp     loc_17B86
    ; align 2
    db 144
loc_17AE6:
    cmp     byte ptr [bx+0BEh], 0
    jnz     short loc_17AF0
    jmp     loc_17B86
loc_17AF0:
    cmp     byte ptr [bx+0CAh], 0
    jz      short loc_17AFA
    jmp     loc_17B86
loc_17AFA:
    cmp     byte ptr [bx+0C0h], 0
    jnz     short loc_17B04
    jmp     loc_17B86
loc_17B04:
    mov     si, [bp+arg_6]
    mov     ax, [si+0Ah]
    cmp     [bx+22h], ax
    jbe     short loc_17B20
loc_17B0F:
    mov     si, [bp+arg_6]
    mov     al, [si]
    cmp     [bx+0BEh], al
    jz      short loc_17B86
    inc     byte ptr [bx+0BEh]
    jmp     short loc_17B39
loc_17B20:
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_6]
    mov     ax, [si+8]
    cmp     [bx+22h], ax
    jnb     short loc_17B86
loc_17B2E:
    cmp     byte ptr [bx+0BEh], 1
    jle     short loc_17B86
    dec     byte ptr [bx+0BEh]
loc_17B39:
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0CAh], 1
    mov     al, byte ptr word_449D0
    cbw
    sar     ax, 1
    add     al, byte ptr word_449D0
    mov     bx, [bp+arg_4]
    mov     [bx+0CBh], al
    mov     bx, [bp+arg_4]
    mov     al, [bx+0BEh]
    cbw
    mov     si, ax
    shl     si, 1
    shl     si, 1
    mov     bx, [bp+arg_6]
    mov     ax, [bx+si+1Ch]
    mov     bx, [bp+arg_4]
    mov     [bx+3Ah], ax
    mov     bx, [bp+arg_4]
    mov     al, [bx+0BEh]
    cbw
    mov     si, ax
    shl     si, 1
    shl     si, 1
    mov     bx, [bp+arg_6]
    mov     ax, [bx+si+1Eh]
    mov     bx, [bp+arg_4]
    mov     [bx+3Ch], ax
loc_17B86:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0CAh], 0
    jnz     short loc_17B93
    jmp     loc_17C9E
loc_17B93:
    mov     si, bx
    mov     ax, [si+3Ah]
    cmp     [bx+34h], ax
    jnz     short loc_17C0C
    mov     ax, [bx+3Ch]
    sub     ax, [bx+38h]
    mov     [bp+var_4], ax
    or      ax, ax
    jnz     short loc_17BDA
    mov     byte ptr [bx+0CAh], 0
    mov     bx, [bp+arg_4]
    mov     al, [bx+0BEh]
    cbw
    mov     si, ax
    shl     si, 1
    mov     bx, [bp+arg_6]
    mov     ax, [bx+si+0Eh]
    mov     bx, [bp+arg_4]
    mov     [bx+30h], ax
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+30h]
    mov     cl, 8
    shr     ax, cl
    mov     [bx+32h], ax
    jmp     loc_17CAC
    ; align 2
    db 144
loc_17BDA:
    push    [bp+var_4]      ; int
    call    _abs
    add     sp, 2
    cmp     ax, [bp+var_2]
    jg      short loc_17BF6
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+3Ch]
    jmp     loc_17C84
    ; align 2
    db 144
loc_17BF6:
    cmp     [bp+var_4], 0
    jle     short loc_17BFF
    jmp     loc_17C93
loc_17BFF:
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_2]
    sub     [bx+38h], ax
    jmp     loc_17CAC
    ; align 2
    db 144
loc_17C0C:
    mov     bx, [bp+arg_6]
    mov     si, [bp+arg_4]
    mov     ax, [si+38h]
    cmp     [bx+1Eh], ax
    jnz     short loc_17C5E
    mov     bx, si
    mov     ax, [bx+3Ah]
    sub     ax, [bx+34h]
    mov     [bp+var_4], ax
    push    ax              ; int
    call    _abs
    add     sp, 2
    cmp     ax, [bp+var_2]
    jg      short loc_17C40
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+3Ah]
    mov     [bx+34h], ax
    jmp     short loc_17CAC
loc_17C40:
    cmp     [bp+var_4], 0
    jle     short loc_17C52
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_2]
    add     [bx+34h], ax
    jmp     short loc_17CAC
    ; align 2
    db 144
loc_17C52:
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_2]
    sub     [bx+34h], ax
    jmp     short loc_17CAC
    ; align 2
    db 144
loc_17C5E:
    mov     bx, [bp+arg_6]
    mov     ax, [bx+1Eh]
    mov     bx, [bp+arg_4]
    sub     ax, [bx+38h]
    mov     [bp+var_4], ax
    push    ax              ; int
    call    _abs
    add     sp, 2
    cmp     ax, [bp+var_2]
    jg      short loc_17C8A
    mov     bx, [bp+arg_4]
    mov     si, [bp+arg_6]
    mov     ax, [si+1Eh]
loc_17C84:
    mov     [bx+38h], ax
    jmp     short loc_17CAC
    ; align 2
    db 144
loc_17C8A:
    cmp     [bp+var_4], 0
    jg      short loc_17C93
    jmp     loc_17BFF
loc_17C93:
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_2]
    add     [bx+38h], ax
    jmp     short loc_17CAC
loc_17C9E:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0CBh], 0
    jz      short loc_17CAC
    dec     byte ptr [bx+0CBh]
loc_17CAC:
    mov     bx, [bp+arg_4]
    mov     ax, [bx+2Ah]
    mov     [bp+var_6], ax
    mov     si, ax
    mov     cl, 0Ah
    shr     si, cl
    shl     si, 1
    mov     bx, [bp+arg_6]
    les     bx, [bx+304h]
    mov     di, [bp+arg_4]
    mov     ax, [di+1Eh]
    sub     ax, es:[bx+si]
    mov     [bp+var_8], ax
    mov     bx, di
    mov     si, [bp+arg_6]
    mov     ax, [si+0Ch]
    cmp     [bx+22h], ax
    jbe     short loc_17CEA
    dec     ax
    mov     [bx+22h], ax
loc_17CE1:
    mov     bx, [bp+arg_6]
    mov     ax, [bx+4]
    jmp     short loc_17D36
    ; align 2
    db 144
loc_17CEA:
    mov     al, [bp+arg_0]
smart
    and     ax, 3
nosmart
    cmp     ax, 1
    jnz     short loc_17CF8
    jmp     loc_17D82
loc_17CF8:
    cmp     ax, 2
    jz      short loc_17D10
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BDh], 0
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BCh], 0
    jmp     short loc_17D39
    ; align 2
    db 144
loc_17D10:
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BDh], 0
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0C6h], 0
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BCh], 1
    cmp     [bp+arg_2], 0
    jz      short loc_17CE1
    mov     bx, [bp+arg_6]
    mov     ax, [bx+4]
    shl     ax, 1
loc_17D36:
    sub     [bp+var_8], ax
loc_17D39:
    cmp     word_449D0, 0Ah
    jnz     short loc_17D46
    mov     ax, [bp+var_8]
    add     [bp+var_8], ax
loc_17D46:
    cmp     [bp+var_8], 0
    jge     short loc_17D4F
    jmp     loc_17EE2
loc_17D4F:
    cmp     [bp+var_6], 8000h
    jb      short loc_17D59
    jmp     loc_17EC2
loc_17D59:
    mov     ax, [bp+var_8]
    add     [bp+var_6], ax
loc_17D5F:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0C0h], 0
    jnz     short loc_17D6C
    jmp     loc_17F3C
loc_17D6C:
    mov     ax, [bx+2Ch]
    sub     ax, [bp+var_6]
    mov     [bp+var_4], ax
    or      ax, ax
    jl      short loc_17D7C
    jmp     loc_17EF8
loc_17D7C:
    neg     ax
    jmp     loc_17EFB
    ; align 2
    db 144
loc_17D82:
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BCh], 0
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0BDh], 1
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0CAh], 0
    jz      short loc_17DBC
    mov     byte ptr [bx+0C6h], 0
    cmp     word_449D0, 0Ah
    jnz     short loc_17DB2
    mov     bx, [bp+arg_4]
    sub     word ptr [bx+22h], 50h ; 'P'
    jmp     short loc_17D39
    ; align 2
    db 144
loc_17DB2:
    mov     bx, [bp+arg_4]
    sub     word ptr [bx+22h], 28h ; '('
    jmp     loc_17D39
loc_17DBC:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0C0h], 0
    jnz     short loc_17DE6
    mov     si, [bp+arg_6]
    mov     ax, [si+0Ch]
    cmp     [bx+22h], ax
    jb      short loc_17DD4
    jmp     loc_17D39
loc_17DD4:
    cmp     [bp+var_6], 0FA00h
    jb      short loc_17DDE
    jmp     loc_17D39
loc_17DDE:
    add     byte ptr [bp+var_8+1], 3
    jmp     loc_17D39
    ; align 2
    db 144
loc_17DE6:
    cmp     byte ptr [bx+0BEh], 1
    jg      short loc_17DFC
    cmp     word ptr [bx+22h], 0A28h
    jge     short loc_17DFC
    mov     bx, [bp+arg_6]
    mov     al, [bx+3Ah]
    jmp     short loc_17E0C
loc_17DFC:
    mov     bx, [bp+arg_4]
    mov     si, [bx+22h]
    mov     cl, 7
    shr     si, cl
    mov     bx, [bp+arg_6]
    mov     al, [bx+si+3Bh]
loc_17E0C:
    mov     [bp+var_A], al
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0C6h], 0
    jz      short loc_17E34
    cmp     word ptr [bx+22h], 1388h
    jge     short loc_17E34
    mov     bx, [bp+arg_6]
    mov     al, [bx+3Ah]
    sub     ah, ah
    mov     cl, [bp+var_A]
    sub     ch, ch
    add     ax, cx
    shr     ax, 1
    mov     [bp+var_A], al
loc_17E34:
    mov     al, [bp+var_A]
    sub     ah, ah
    mov     cx, ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+32h]
    mul     cx
    mov     cl, 4
    shr     ax, cl
    add     [bp+var_8], ax
    mov     bx, [bp+arg_6]
    sub     ax, ax
    push    ax
    push    word ptr [bx+2]
    mov     ax, 19h
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_8]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFuldiv
    sar     ax, 1
    mov     [bp+var_8], ax
    cmp     [bp+arg_2], 0
    jz      short loc_17EAD
    mov     al, byte_44D50
    sub     ah, ah
    sub     ax, 0C8h ; 'È'
    neg     ax
    shr     ax, 1
    mov     [bp+var_A], al
    or      al, al
    jz      short loc_17EAD
    mov     ax, 0C8h ; 'È'
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_8]
    cwd
    push    dx
    push    ax
    mov     al, [bp+var_A]
    sub     ah, ah
    sub     cx, cx
    push    cx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    sub     [bp+var_8], ax
loc_17EAD:
    cmp     [bp+var_8], 128h
    jg      short loc_17EB7
    jmp     loc_17D39
loc_17EB7:
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0C6h], 5
    jmp     loc_17D39
loc_17EC2:
    mov     ax, [bp+var_8]
    add     [bp+var_6], ax
    cmp     [bp+var_6], 8000h
    jb      short loc_17ED9
    cmp     [bp+var_6], 0F500h
    ja      short loc_17ED9
    jmp     loc_17D5F
loc_17ED9:
    mov     [bp+var_6], 0F500h
    jmp     loc_17D5F
    ; align 2
    db 144
loc_17EE2:
    mov     ax, [bp+var_8]
    neg     ax
    cmp     ax, [bp+var_6]
    ja      short loc_17EEF
    jmp     loc_17D59
loc_17EEF:
    mov     [bp+var_6], 0
    jmp     loc_17D5F
    ; align 2
    db 144
loc_17EF8:
    mov     ax, [bp+var_4]
loc_17EFB:
    cmp     ax, 1400h
    jle     short loc_17F28
    mov     si, bx
    mov     ax, [si+2Ah]
    sub     dx, dx
    add     ax, [si+2Ch]
    adc     dx, dx
    shr     dx, 1
    rcr     ax, 1
    mov     [bx+2Ah], ax
    mov     bx, [bp+arg_4]
    mov     si, bx
    mov     ax, [si+2Ah]
    mov     [bx+2Ch], ax
    mov     bx, [bp+arg_4]
    mov     byte ptr [bx+0C6h], 5
    jmp     short loc_17F45
loc_17F28:
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_6]
    mov     [bx+2Ah], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_6]
    mov     [bx+2Ch], ax
    jmp     short loc_17F45
loc_17F3C:
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_6]
    mov     [bx+2Ah], ax
loc_17F45:
    mov     bx, [bp+arg_6]
    push    word ptr [bx+6]
    mov     bx, [bp+arg_4]
    mov     al, [bx+0CAh]
    cbw
    push    ax
    push    word ptr [bx+30h]
    push    word ptr [bx+2Ah]
    push    word ptr [bx+22h]
    call    sub_19DC6
    add     sp, 0Ah
    mov     bx, [bp+arg_4]
    mov     [bx+22h], ax
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx+0C1h], 0
    jz      short loc_17FBF
    mov     si, bx
    mov     ax, [si+22h]
    cmp     [bx+24h], ax
    jle     short loc_17FBF
    mov     ax, [bx+24h]
    sub     ax, [bx+22h]
    cmp     ax, 7D0h
    jle     short loc_17FA4
    mov     bx, [bp+arg_6]
    mov     al, [bx+3Ah]
    sub     ah, ah
    mov     bx, si
    mul     word ptr [bx+32h]
    cmp     ax, 2EE0h
    jle     short loc_17FBF
    mov     byte ptr [bx+0C6h], 1Eh
    jmp     short loc_17FBF
    ; align 2
    db 144
loc_17FA4:
    mov     bx, [bp+arg_4]
    mov     ax, [bx+22h]
    sub     ax, [bx+24h]
    cmp     ax, 7D0h
    jle     short loc_17FBF
    mov     byte ptr [bx+0C6h], 0Ah
    mov     bx, [bp+arg_4]
    sub     word ptr [bx+2Ch], 500h
loc_17FBF:
    mov     bx, [bp+arg_4]
    mov     ax, word_445E2
    cmp     [bx+2Ch], ax
    jbe     short loc_17FD0
    mov     ax, [bx+2Ch]
    mov     word_445E2, ax
loc_17FD0:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_17A78 endp
sub_17FD6 proc far
    var_10 = word ptr -16
    var_E = byte ptr -14
    var_C = word ptr -12
    var_A = byte ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 10h
    push    di
    push    si
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C1h], 0
    jnz     short loc_17FFC
    mov     word ptr [bx+40h], 0
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C7h], 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_17FFC:
    mov     [bp+var_8], 0
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C2h], 4
    jnz     short loc_1800E
    inc     [bp+var_8]
loc_1800E:
    cmp     byte ptr [bx+0C3h], 4
    jnz     short loc_18018
    inc     [bp+var_8]
loc_18018:
    cmp     byte ptr [bx+0C4h], 4
    jnz     short loc_18022
    inc     [bp+var_8]
loc_18022:
    cmp     byte ptr [bx+0C5h], 4
    jnz     short loc_1802C
    inc     [bp+var_8]
loc_1802C:
    cmp     [bp+var_8], 0
    jz      short loc_18051
    mov     ax, [bx+2Ch]
    sub     dx, dx
    mov     bx, [bp+var_8]
    shl     bx, 1
    div     word ptr [bx+688h]
    mov     bx, [bp+arg_0]
    sub     [bx+2Ch], ax
    mov     bx, [bp+arg_0]
    mov     si, bx
    mov     ax, [si+2Ch]
    mov     [bx+2Ah], ax
loc_18051:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+20h]
    add     ax, [bx+36h]
    mov     [bp+var_10], ax
    mov     [bp+var_C], ax
    mov     ax, [bx+2Ah]
    mov     cl, 8
    shr     ax, cl
    mov     [bp+var_2], ax
    cmp     [bp+var_C], 0
    jge     short loc_18078
    mov     ax, [bp+var_C]
    neg     ax
    jmp     short loc_1807B
    ; align 2
    db 144
loc_18078:
    mov     ax, [bp+var_C]
loc_1807B:
    mov     cl, 3
    sar     ax, cl
    mov     [bp+var_8], ax
    mov     ax, [bp+var_2]
    mul     ax
    mov     cl, 6
    shr     ax, cl
    mul     [bp+var_8]
    mov     [bp+var_4], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+0A4h]
    shl     ax, 1
    mov     [bp+var_6], ax
    cwd
    push    dx
    push    ax
    mov     bx, [bp+arg_0]
    mov     al, [bx+0C5h]
    cbw
    mov     si, ax
    shl     si, 1
    mov     bx, [bp+arg_2]
    mov     di, [bp+arg_0]
    mov     al, [di+0C4h]
    cbw
    mov     di, ax
    shl     di, 1
    mov     ax, [bx+si+0B4h]
    mov     bx, [bp+arg_0]
    mov     cx, ax
    mov     al, [bx+0C3h]
    cbw
    mov     si, ax
    shl     si, 1
    mov     bx, [bp+arg_2]
    mov     ax, [bx+si+0B4h]
    mov     bx, [bp+arg_0]
    mov     dx, ax
    mov     al, [bx+0C2h]
    cbw
    mov     si, ax
    shl     si, 1
    mov     bx, [bp+arg_2]
    mov     ax, [bx+si+0B4h]
    add     ax, dx
    add     ax, [bx+di+0B4h]
    add     ax, cx
    cwd
    push    dx
    push    ax
    call    __aFlmul
    mov     cl, 0Ah
loc_180FB:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_180FB
    mov     [bp+var_6], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_4]
    mov     [bx+44h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_6]
    mov     [bx+46h], ax
    cmp     [bp+arg_4], 0
    jnz     short loc_18121
    jmp     loc_1835E
loc_18121:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+20h], 0
    jnz     short loc_18168
    mov     al, [bx+18h]
    sub     ah, ah
    mov     [bp+var_8], ax
    cmp     ax, 7Fh ; ''
    jle     short loc_1813C
    sub     [bp+var_8], 100h
loc_1813C:
    cmp     [bp+var_8], 0
    jz      short loc_18168
    jge     short loc_1814C
    mov     ax, [bp+var_8]
    neg     ax
    jmp     short loc_1814F
    ; align 2
    db 144
loc_1814C:
    mov     ax, [bp+var_8]
loc_1814F:
    cmp     ax, 8
    jge     short loc_18168
    cmp     [bp+var_8], 0
    jle     short loc_18162
    mov     bx, [bp+arg_0]
    dec     word ptr [bx+18h]
    jmp     short loc_18168
loc_18162:
    mov     bx, [bp+arg_0]
    inc     word ptr [bx+18h]
loc_18168:
    mov     ax, [bp+var_6]
    cmp     [bp+var_4], ax
    jle     short loc_181CE
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C7h], 1
    mov     ax, [bp+var_2]
    cwd
    push    dx
    push    ax
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    mov     ax, [bp+var_6]
    cwd
    mov     dh, dl
    mov     dl, ah
    mov     ah, al
    sub     al, al
    push    dx
    push    ax
    call    __aFldiv
    mov     [bp+var_C], ax
    cmp     [bp+var_10], 0
    jge     short loc_181AD
    mov     ax, 0FFFFh
    imul    [bp+var_C]
    mov     [bp+var_C], ax
loc_181AD:
    mov     ax, [bp+var_C]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    add     ax, [bp+var_10]
    sar     ax, 1
    sar     ax, 1
    mov     [bp+var_C], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_10]
    sub     ax, [bp+var_C]
    mov     [bx+42h], ax
    jmp     short loc_18207
loc_181CE:
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C7h], 0
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+42h], 0
    jz      short loc_18207
    mov     si, bx
    mov     ax, [si+42h]
    mov     cl, 4
    sar     ax, cl
    sub     [bx+42h], ax
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+42h], 0
    jge     short loc_181FC
    mov     ax, [bx+42h]
    neg     ax
    jmp     short loc_181FF
    ; align 2
    db 144
loc_181FC:
    mov     ax, [bx+42h]
loc_181FF:
    cmp     ax, 10h
    jge     short loc_18207
    sar     word ptr [bx+42h], 1
loc_18207:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+3Eh], 0
    jnz     short loc_18220
    cmp     byte ptr [bx+0C9h], 1
    jz      short loc_18220
    mov     ax, [bp+var_C]
    mov     [bx+40h], ax
    jmp     short loc_18228
    ; align 2
    db 144
loc_18220:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+40h], 0
loc_18228:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+1Ch], 0
    jnz     short loc_18234
    jmp     loc_182BD
loc_18234:
    jge     short loc_1823E
    mov     ax, [bx+1Ch]
    neg     ax
    jmp     short loc_18241
    ; align 2
    db 144
loc_1823E:
    mov     ax, [bx+1Ch]
loc_18241:
    cmp     ax, 4
    jle     short loc_182BD
    mov     al, [bx+2]
    mov     [bp+var_A], al
    mov     al, [bx+0Ah]
    mov     [bp+var_E], al
    mov     bl, al
    sub     bh, bh
    shl     bx, 1
    mov     bx, [bx-73C4h]
    mov     al, [bp+var_A]
    sub     ah, ah
    add     bx, ax
    les     si, dword_44D42
    mov     al, es:[bx+si]
    cmp     ax, 0FDh ; 'ý'
    jz      short loc_1827C
    cmp     ax, 0FEh ; 'þ'
    jz      short loc_1827F
    cmp     ax, 0FFh
    jz      short loc_182A8
    jmp     short loc_18282
    ; align 2
    db 144
loc_1827C:
    dec     [bp+var_A]
loc_1827F:
    inc     [bp+var_E]
loc_18282:
    mov     bl, [bp+var_E]
    sub     bh, bh
    shl     bx, 1
    mov     bx, [bx-73C4h]
    mov     al, [bp+var_A]
    sub     ah, ah
    add     bx, ax
    les     si, dword_44D42
    mov     al, es:[bx+si]
    cmp     ax, 34h ; '4'
    jb      short loc_182BD
    cmp     ax, 37h ; '7'
    jbe     short loc_182AE
    jmp     short loc_182BD
    ; align 2
    db 144
loc_182A8:
    dec     [bp+var_A]
    jmp     short loc_18282
    ; align 2
    db 144
loc_182AE:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+1Ch]
    cwd
    mov     cx, 5
    idiv    cx
    add     [bx+40h], ax
loc_182BD:
    mov     ax, [bp+var_6]
    add     ax, 3E8h
    cmp     ax, [bp+var_4]
    jge     short loc_182EA
    mov     ax, [bp+var_C]
    sub     ax, [bp+var_10]
    cwd
    mov     cx, 0Eh
    idiv    cx
    mov     bx, [bp+arg_0]
    add     [bx+3Eh], ax
    mov     bx, [bp+arg_0]
    mov     cx, 2
    mov     ax, [bx+3Eh]
    cwd
    idiv    cx
    jmp     loc_1838B
    ; align 2
    db 144
loc_182EA:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+3Eh], 0
    jnz     short loc_182F6
    jmp     loc_1838E
loc_182F6:
    mov     ax, [bp+var_C]
    sub     ax, [bp+var_10]
    cwd
    mov     cx, 0Eh
    idiv    cx
    add     [bx+3Eh], ax
    mov     bx, [bp+arg_0]
    mov     cx, 2
    mov     ax, [bx+3Eh]
    cwd
    idiv    cx
    mov     [bx+3Eh], ax
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+3Eh], 0
    jnz     short loc_1838E
    push    word ptr [bx+2Ch]
    push    word ptr [bx+36h]
    call    cos_fast
    add     sp, 2
    push    ax
    call    sub_30044
    add     sp, 4
    mov     bx, [bp+arg_0]
    mov     [bx+2Ch], ax
    mov     bx, [bp+arg_0]
    push    word ptr [bx+36h]
    call    cos_fast
    add     sp, 2
    or      ax, ax
    jge     short loc_18354
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+2Ch], 0
loc_18354:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+36h], 0
    jmp     short loc_1838E
loc_1835E:
    mov     bx, [bp+arg_0]
    mov     si, bx
    mov     ax, [si+20h]
    shl     ax, 1
    shl     ax, 1
    mov     [bx+40h], ax
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+3Eh], 0
    jz      short loc_1838E
    mov     si, bx
    mov     ax, [si+3Eh]
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    sub     ax, cx
    mov     cl, 4
    sar     ax, cl
loc_1838B:
    mov     [bx+3Eh], ax
loc_1838E:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+36h], 0
    jz      short loc_183B5
    cmp     word ptr [bx+3Eh], 0
    jnz     short loc_183B5
    mov     si, bx
    mov     ax, [si+36h]
    mov     cx, ax
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    sub     ax, cx
    mov     cl, 4
    sar     ax, cl
    mov     [bx+36h], ax
loc_183B5:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+3Eh], 0
    jz      short loc_183C6
    mov     si, bx
    mov     ax, [si+3Eh]
    sub     [bx+36h], ax
loc_183C6:
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C7h], 0
    jnz     short loc_183D3
    jmp     loc_18458
loc_183D3:
    cmp     word ptr [bx+42h], 0
    jge     short loc_183E0
    mov     ax, [bx+42h]
    neg     ax
    jmp     short loc_183E3
loc_183E0:
    mov     ax, [bx+42h]
loc_183E3:
    shl     ax, 1
    mov     [bp+var_8], ax
    cmp     [bx+2Ah], ax
    jbe     short loc_18448
    cmp     [bx+2Ch], ax
    jbe     short loc_18400
    sub     [bx+2Ah], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_8]
    sub     [bx+2Ch], ax
    jmp     short loc_18410
loc_18400:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+2Ah], 0
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+2Ch], 0
loc_18410:
    mov     bx, [bp+arg_0]
    cmp     byte ptr [bx+0C9h], 0
    jnz     short loc_18458
    cmp     byte ptr [bx+0C2h], 1
    jz      short loc_18436
    cmp     byte ptr [bx+0C3h], 1
    jz      short loc_18436
    cmp     byte ptr [bx+0C4h], 1
    jz      short loc_18436
    cmp     byte ptr [bx+0C5h], 1
    jnz     short loc_1843E
loc_18436:
smart
    or      byte ptr [bx+0CFh], 2
nosmart
    jmp     short loc_18458
    ; align 2
    db 144
loc_1843E:
    mov     bx, [bp+arg_0]
smart
    or      byte ptr [bx+0CFh], 4
nosmart
    jmp     short loc_18458
loc_18448:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+2Ah], 0
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+2Ch], 0
loc_18458:
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+42h], 0
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_17FD6 endp
sub_18466 proc far
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
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
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 18h
    push    si
    mov     bx, [bp+arg_0]
    mov     byte ptr [bx+0C8h], 1
    mov     bx, [bp+arg_2]
    mov     byte ptr [bx+0C8h], 1
    mov     bx, [bp+arg_0]
    mov     ax, [bx+2Ch]
    mov     [bp+var_6], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+2Ch]
    mov     [bp+var_C], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+18h]
    mov     [bp+var_2], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+18h]
    mov     [bp+var_4], ax
    push    [bp+var_2]
    call    sin_fast
    add     sp, 2
    push    ax
    mov     ax, [bp+var_6]
    mov     cl, 8
    shr     ax, cl
    push    ax
    call    sub_30044
    add     sp, 4
    mov     [bp+var_10], ax
    push    [bp+var_4]
    call    sin_fast
    add     sp, 2
    push    ax
    mov     ax, [bp+var_C]
    mov     cl, 8
    shr     ax, cl
    push    ax
    call    sub_30044
    add     sp, 4
    mov     [bp+var_14], ax
    push    [bp+var_2]
    call    cos_fast
    add     sp, 2
    push    ax
    mov     ax, [bp+var_6]
    mov     cl, 8
    shr     ax, cl
    push    ax
    call    sub_30044
    add     sp, 4
    mov     [bp+var_12], ax
    push    [bp+var_4]
    call    cos_fast
    add     sp, 2
    push    ax
    mov     ax, [bp+var_C]
    mov     cl, 8
    shr     ax, cl
    push    ax
    call    sub_30044
    add     sp, 4
    mov     [bp+var_16], ax
    sub     ax, [bp+var_12]
    push    ax
    mov     ax, [bp+var_14]
    sub     ax, [bp+var_10]
    push    ax
    call    sub_300B6
    add     sp, 4
    mov     [bp+var_A], ax
    cmp     ax, 0Ah
    jge     short loc_1853D
    mov     [bp+var_A], 0Ah
loc_1853D:
    mov     ax, [bp+var_2]
    sub     ax, [bp+var_4]
smart
    and     ah, 3
nosmart
    mov     [bp+var_8], ax
    mov     ah, byte ptr [bp+var_A]
    sub     al, al
    mov     [bp+var_E], ax
    mov     ax, 300h
    imul    [bp+var_A]
    sar     ax, 1
    sar     ax, 1
    mov     [bp+var_18], ax
    mov     bx, [bp+arg_0]
    cmp     [bx+2Ch], ax
    jnb     short loc_1856E
    mov     word ptr [bx+2Ch], 0
    jmp     short loc_18577
    ; align 2
    db 144
loc_1856E:
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_18]
    sub     [bx+2Ch], ax
loc_18577:
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_4]
    sub     ax, [bp+var_2]
    mov     [bx+36h], ax
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+36h], 200h
    jl      short loc_18592
    sub     word ptr [bx+36h], 400h
loc_18592:
    mov     bx, [bp+arg_0]
    cmp     word ptr [bx+36h], 0FE00h
    jg      short loc_185A0
    add     byte ptr [bx+37h], 4
loc_185A0:
    mov     bx, [bp+arg_2]
    mov     ax, [bp+var_2]
    sub     ax, [bp+var_4]
    mov     [bx+36h], ax
    mov     bx, [bp+arg_2]
    cmp     word ptr [bx+36h], 200h
    jl      short loc_185BB
    sub     word ptr [bx+36h], 400h
loc_185BB:
    mov     bx, [bp+arg_2]
    cmp     word ptr [bx+36h], 0FE00h
    jg      short loc_185C9
    add     byte ptr [bx+37h], 4
loc_185C9:
    mov     bx, [bp+arg_0]
    mov     si, bx
    mov     ax, [si+2Ch]
    mov     [bx+2Ah], ax
    mov     bx, [bp+arg_2]
    mov     si, bx
    mov     ax, [si+2Ch]
    mov     [bx+2Ah], ax
    cmp     [bp+var_A], 1Eh
    jle     short loc_185EE
    mov     ax, 1
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_185EE:
    sub     ax, ax
    pop     si
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_18466 endp
sub_185F6 proc far
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    mov     ax, [si+54h]
    mov     [bp+var_2], ax
    mov     [bp+var_4], 0
    mov     [bp+var_6], 0
    cmp     word ptr [si+6Ch], 0
    jz      short loc_18659
    jge     short loc_18636
    add     word ptr [si+6Ch], 4
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    add     si, 6Ch ; 'l'
    mov     ax, [si]
    cmp     [bp+var_6], ax
    jge     short loc_18659
    jmp     short loc_18654
loc_18636:
    mov     di, [bp+arg_4]
    shl     di, 1
    mov     bx, [bp+arg_0]
    sub     word ptr [bx+di+6Ch], 4
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    add     si, 6Ch ; 'l'
    mov     ax, [si]
    cmp     [bp+var_6], ax
    jle     short loc_18659
loc_18654:
    mov     ax, [bp+var_6]
    mov     [si], ax
loc_18659:
    mov     di, [bp+arg_4]
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     si, [bx+di+6Ch]
    mov     di, [bp+arg_4]
    shl     di, 1
    mov     [bx+di+6Ch], si
    cmp     [bp+arg_2], 0
    jge     short loc_18689
    mov     di, [bp+arg_4]
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_2]
    neg     ax
    cmp     [bx+di+54h], ax
    jle     short loc_18689
    mov     [bp+arg_2], 0
loc_18689:
    cmp     [bp+arg_2], 0
    jnz     short loc_186FA
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    mov     ax, [si+6Ch]
    cmp     [si+54h], ax
    jle     short loc_186C8
    sub     word ptr [si+54h], 80h ; '€'
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    mov     ax, [si+6Ch]
    cmp     [si+54h], ax
    jge     short loc_186B7
    mov     [si+54h], ax
loc_186B7:
    mov     ax, [bp+var_2]
    mov     si, [bp+arg_4]
    shl     si, 1
    mov     bx, [bp+arg_0]
    sub     ax, [bx+si+54h]
    jmp     loc_187A2
loc_186C8:
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    mov     ax, [si+6Ch]
    cmp     [si+54h], ax
    jl      short loc_186DB
    jmp     loc_187A5
loc_186DB:
    add     word ptr [si+54h], 80h ; '€'
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    mov     ax, [si+6Ch]
    cmp     [si+54h], ax
    jg      short loc_186F3
    jmp     loc_187A5
loc_186F3:
    mov     [si+54h], ax
    jmp     loc_187A5
    ; align 2
    db 144
loc_186FA:
    cmp     [bp+arg_2], 0
    jle     short loc_18748
    cmp     [bp+arg_2], 0C0h ; 'À'
    jle     short loc_18716
    mov     si, [bp+arg_4]
    shl     si, 1
    mov     bx, [bp+arg_0]
    add     word ptr [bx+si+54h], 0C0h ; 'À'
    jmp     short loc_18724
loc_18716:
    mov     si, [bp+arg_4]
    shl     si, 1
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_2]
    add     [bx+si+54h], ax
loc_18724:
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    add     si, 54h ; 'T'
    cmp     word ptr [si], 180h
    jle     short loc_18739
    mov     word ptr [si], 180h
loc_18739:
    mov     si, [bp+arg_4]
    shl     si, 1
    mov     bx, [bp+arg_0]
    mov     word ptr [bx+si+64h], 0
    jmp     short loc_187A5
loc_18748:
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    add     si, 54h ; 'T'
    mov     ax, [bp+arg_2]
    add     ax, [si]
    cmp     ax, 0FEE0h
    jle     short loc_18764
    mov     ax, [bp+arg_2]
    add     [si], ax
    jmp     short loc_18791
loc_18764:
    mov     di, [bp+arg_4]
    shl     di, 1
    mov     bx, [bp+arg_0]
    mov     ax, [bp+arg_2]
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    sar     ax, 1
    sar     ax, 1
    add     [bx+di+54h], ax
    mov     si, [bp+arg_4]
    shl     si, 1
    add     si, [bp+arg_0]
    add     si, 54h ; 'T'
    cmp     word ptr [si], 0FE80h
    jge     short loc_18791
    mov     word ptr [si], 0FE80h
loc_18791:
    mov     ax, [bp+var_2]
    mov     si, [bp+arg_4]
    shl     si, 1
    mov     bx, [bp+arg_0]
    sub     ax, [bx+si+54h]
    add     ax, [bp+arg_2]
loc_187A2:
    mov     [bp+var_4], ax
loc_187A5:
    mov     ax, [bp+var_2]
    add     ax, [bp+var_4]
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_185F6 endp
sub_187B2 proc far
    var_4 = byte ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    mov     di, word_44606
    mov     ax, word_44612
    mov     cl, 0Ah
    shr     ax, cl
smart
    and     al, 0FCh
nosmart
    mov     [bp+var_4], al
    cbw
    mov     bx, ax
    mov     al, [bp+arg_0]
    cbw
    add     bx, ax
    add     bx, word_449E8
    mov     al, [bx]
    cbw
    mov     si, ax
    or      si, si
    jle     short loc_187E8
    cmp     di, 0FFFFh
    jge     short loc_187F5
    jmp     short loc_187F1
    ; align 2
    db 144
loc_187E8:
    or      si, si
    jz      short loc_187F5
    cmp     di, 1
    jle     short loc_187F5
loc_187F1:
    mov     cl, 2
    shl     si, cl
loc_187F5:
    or      si, si
    jnz     short loc_18835
    cmp     word_44612, 0
    jz      short loc_18835
    or      di, di
    jz      short loc_18835
    mov     al, [bp+var_4]
    cbw
    mov     bx, ax
    add     bx, word_449E8
    mov     al, [bx+1]
    cbw
    mov     si, ax
    shl     si, 1
    or      di, di
    jge     short loc_18820
    mov     ax, di
    neg     ax
    jmp     short loc_18822
loc_18820:
    mov     ax, di
loc_18822:
    cmp     ax, si
    jle     short loc_1882E
    or      di, di
    jle     short loc_18835
    mov     ax, si
    jmp     short loc_18831
loc_1882E:
    mov     ax, word_44606
loc_18831:
    neg     ax
    mov     si, ax
loc_18835:
    cmp     word_449D0, 0Ah
    jnz     short loc_18850
    cmp     si, 0A0h ; ' '
    jle     short loc_18845
    mov     si, 0A0h ; ' '
loc_18845:
    cmp     si, 0FF60h
    jge     short loc_18860
    mov     si, 0FF60h
    jmp     short loc_18860
loc_18850:
    cmp     si, 50h ; 'P'
    jle     short loc_18858
    mov     si, 50h ; 'P'
loc_18858:
    cmp     si, 0FFB0h
    jge     short loc_18860
    mov     si, 0FFB0h
loc_18860:
    add     di, si
    cmp     di, 0F0h ; 'ð'
    jle     short loc_1886B
    mov     di, 0F0h ; 'ð'
loc_1886B:
    cmp     di, 0FF10h
    jge     short loc_18874
    mov     di, 0FF10h
loc_18874:
    mov     al, [bp+var_4]
    cbw
    mov     bx, ax
    mov     al, [bp+arg_0]
    cbw
    add     bx, ax
    add     bx, word_449E8
    cmp     byte ptr [bx], 0
    jnz     short loc_18899
    push    di              ; int
    call    _abs
    add     sp, 2
    cmp     ax, 8
    jge     short loc_18899
    sub     di, di
loc_18899:
    mov     word_44606, di
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_187B2 endp
sub_188A4 proc far
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
    var_16 = byte ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
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

    push    bp
    mov     bp, sp
    sub     sp, 34h
    push    di
    push    si
    cmp     byte_454B8, 0
    jnz     short loc_188B6
    jmp     loc_1893A
loc_188B6:
    cmp     byte_459D8, 0
    jz      short loc_18925
    mov     ax, word_449E4
    mov     word_44D1E, ax
    test    byte_42D26, 6
    jz      short loc_188D6
    push    word_43964
    call    sub_27466
    add     sp, 2
loc_188D6:
    test    byte_42D26, 1
    jz      short loc_188E9
    push    word_43964
    call    sub_26F6D
    add     sp, 2
loc_188E9:
    cmp     byte_449AA, 0
    jz      short loc_18916
    test    byte_42D2A, 6
    jz      short loc_18903
    push    word_4408C
    call    sub_27466
    add     sp, 2
loc_18903:
    test    byte_42D2A, 1
    jz      short loc_18916
    push    word_4408C
    call    sub_26F6D
    add     sp, 2
loc_18916:
    mov     byte_459D8, 0
    mov     byte_42D26, 0
    mov     byte_42D2A, 0
loc_18925:
    mov     al, byte_3BE02
    cmp     byte_454B8, al
    jnz     short loc_18931
    jmp     loc_18CCC
loc_18931:
    call    sub_38178
    jmp     loc_18CCC
    ; align 2
    db 144
loc_1893A:
    mov     ax, word_445F2
    mov     dx, word_445F4
    mov     cl, 6
loc_18943:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_18943
    mov     [bp+var_1C], ax
    mov     ax, word_445F6
    mov     dx, word_445F8
    mov     cl, 6
loc_18957:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_18957
    mov     [bp+var_1A], ax
    mov     ax, word_445FA
    mov     dx, word_445FC
    mov     cl, 6
loc_1896B:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1896B
    mov     [bp+var_18], ax
    mov     ax, word_445E6
    mov     dx, word_445E8
    mov     cl, 6
loc_1897F:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_1897F
    mov     [bp+var_8], ax
    mov     ax, word_445EA
    mov     dx, word_445EC
    mov     cl, 6
loc_18993:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_18993
    mov     [bp+var_6], ax
    mov     ax, word_445EE
    mov     dx, word_445F0
    mov     cl, 6
loc_189A7:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_189A7
    mov     [bp+var_4], ax
    cmp     byte_449AA, 0
    jz      short loc_18A31
    mov     ax, word_446C2
    mov     dx, word_446C4
    mov     cl, 6
loc_189C2:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_189C2
    mov     [bp+var_24], ax
    mov     ax, word_446C6
    mov     dx, word_446C8
    mov     cl, 6
loc_189D6:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_189D6
    mov     [bp+var_22], ax
    mov     ax, word_446CA
    mov     dx, word_446CC
    mov     cl, 6
loc_189EA:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_189EA
    mov     [bp+var_20], ax
    mov     ax, word_446B6
    mov     dx, word_446B8
    mov     cl, 6
loc_189FE:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_189FE
    mov     [bp+var_E], ax
    mov     ax, word_446BA
    mov     dx, word_446BC
    mov     cl, 6
loc_18A12:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_18A12
    mov     [bp+var_C], ax
    mov     ax, word_446BE
    mov     dx, word_446C0
    mov     cl, 6
loc_18A26:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_18A26
    mov     [bp+var_A], ax
loc_18A31:
    mov     al, byte_3B8F5
    cbw
    or      ax, ax
    jz      short loc_18A52
    cmp     ax, 1
    jnz     short loc_18A41
    jmp     loc_18B42
loc_18A41:
    cmp     ax, 2
    jz      short loc_18A52
    cmp     ax, 3
    jnz     short loc_18A4E
    jmp     loc_18B6E
loc_18A4E:
    jmp     short loc_18A82
    db 144
    db 144
loc_18A52:
    cmp     byte_463E0, 0
    jz      short loc_18A6E
    lea     di, [bp+var_14]
    lea     si, [bp+var_E]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    lea     di, [bp+var_2A]
    lea     si, [bp+var_24]
loc_18A6A:
    push    ss
    pop     es
    jmp     short loc_18A7F
loc_18A6E:
    lea     di, [bp+var_14]
    lea     si, [bp+var_8]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    lea     di, [bp+var_2A]
    lea     si, [bp+var_1C]
loc_18A7F:
    movsw
    movsw
    movsw
loc_18A82:
    mov     ax, 22h ; '"'
    imul    word_449E4
    add     ax, 97DCh
    mov     [bp+var_2], ax
    mov     bx, ax
    mov     ax, [bp+var_2A]
    sub     ax, [bp+var_1C]
    mov     [bx+6], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_28]
    sub     ax, [bp+var_1A]
    mov     [bx+8], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_26]
    sub     ax, [bp+var_18]
    mov     [bx+0Ah], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_14]
    sub     ax, [bp+var_8]
    mov     [bx+0Ch], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_12]
    sub     ax, [bp+var_6]
    mov     [bx+0Eh], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_10]
    sub     ax, [bp+var_4]
    mov     [bx+10h], ax
    mov     bx, [bp+var_2]
    mov     ax, word_44608
    mov     [bx+1Eh], ax
    cmp     byte_449AA, 0
    jnz     short loc_18AE9
    jmp     loc_18BB6
loc_18AE9:
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_2A]
    sub     ax, [bp+var_24]
    mov     [bx+12h], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_28]
    sub     ax, [bp+var_22]
    mov     [bx+14h], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_26]
    sub     ax, [bp+var_20]
    mov     [bx+16h], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_14]
    sub     ax, [bp+var_E]
    mov     [bx+18h], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_12]
    sub     ax, [bp+var_C]
    mov     [bx+1Ah], ax
    mov     bx, [bp+var_2]
    mov     ax, [bp+var_10]
    sub     ax, [bp+var_A]
    mov     [bx+1Ch], ax
    mov     bx, [bp+var_2]
    mov     ax, word_446D8
    mov     [bx+20h], ax
    mov     [bp+var_30], 2
    jmp     short loc_18BBB
    ; align 2
    db 144
loc_18B42:
    mov     al, byte_463E0
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     si, ax
    push    si
    lea     di, [bp+var_14]
    lea     si, [si-71BCh]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    push    si
    lea     di, [bp+var_2A]
    lea     si, [si-71B0h]
    movsw
    movsw
    movsw
    pop     si
    jmp     loc_18A82
    ; align 2
    db 144
loc_18B6E:
    mov     al, byte_463E0
    cbw
    mov     bx, ax
    mov     al, [bx-6EE5h]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, word_449DC
    mov     dx, word_449DE
    mov     word ptr [bp+var_34], ax
    mov     word ptr [bp+var_34+2], dx
    les     bx, [bp+var_34]
    mov     ax, es:[bx]
    mov     [bp+var_14], ax
    mov     ax, es:[bx+2]
    add     ax, word_44D20
    add     ax, 5Ah ; 'Z'
    mov     [bp+var_12], ax
    mov     ax, es:[bx+4]
    mov     [bp+var_10], ax
    lea     di, [bp+var_2A]
    lea     si, [bp+var_14]
    jmp     loc_18A6A
loc_18BB6:
    mov     [bp+var_30], 1
loc_18BBB:
    mov     [bp+var_1E], 0
    jmp     loc_18C93
    ; align 2
    db 144
loc_18BC4:
    mov     [bp+var_2C], 8E76h
    mov     ax, word_43964
    mov     [bp+var_2E], ax
    mov     al, byte_42D26
loc_18BD2:
    mov     [bp+var_16], al
    mov     bx, [bp+var_2C]
    test    byte ptr [bx+0CFh], 1
    jz      short loc_18BF4
    test    [bp+var_16], 1
    jnz     short loc_18C08
smart
    or      [bp+var_16], 1
nosmart
    push    [bp+var_2E]
    call    sub_26EF4
    jmp     short loc_18C05
    ; align 2
    db 144
loc_18BF4:
    test    [bp+var_16], 1
    jz      short loc_18C08
    dec     [bp+var_16]
    push    [bp+var_2E]
    call    sub_26F6D
loc_18C05:
    add     sp, 2
loc_18C08:
    mov     bx, [bp+var_2C]
    test    byte ptr [bx+0CFh], 6
    jz      short loc_18C56
    mov     al, [bp+var_16]
smart
    and     al, 6
nosmart
    mov     byte ptr [bp+var_34], al
    mov     al, [bx+0CFh]
smart
    and     al, 6
nosmart
    cmp     al, byte ptr [bp+var_34]
    jz      short loc_18C7B
    test    [bp+var_16], 6
    jnz     short loc_18C5C
    test    byte ptr [bx+0CFh], 2
    jz      short loc_18C44
    push    [bp+var_2E]
    call    sub_273B0
    add     sp, 2
    add     [bp+var_16], 2
    jmp     short loc_18C7B
    ; align 2
    db 144
loc_18C44:
    push    [bp+var_2E]
    call    sub_2740B
    add     sp, 2
    add     [bp+var_16], 4
    jmp     short loc_18C7B
    ; align 2
    db 144
loc_18C56:
    test    [bp+var_16], 6
    jz      short loc_18C7B
loc_18C5C:
    test    [bp+var_16], 2
    jz      short loc_18C66
    sub     [bp+var_16], 2
loc_18C66:
    test    [bp+var_16], 4
    jz      short loc_18C70
    sub     [bp+var_16], 4
loc_18C70:
    push    [bp+var_2E]
    call    sub_27466
    add     sp, 2
loc_18C7B:
    cmp     [bp+var_1E], 0
    jz      short loc_18C8A
    mov     al, [bp+var_16]
    mov     byte_42D2A, al
    jmp     short loc_18C90
    ; align 2
    db 144
loc_18C8A:
    mov     al, [bp+var_16]
    mov     byte_42D26, al
loc_18C90:
    inc     [bp+var_1E]
loc_18C93:
    mov     ax, [bp+var_30]
    cmp     [bp+var_1E], ax
    jge     short loc_18CB6
    cmp     [bp+var_1E], 0
    jnz     short loc_18CA4
    jmp     loc_18BC4
loc_18CA4:
    mov     [bp+var_2C], 8F46h
    mov     ax, word_4408C
    mov     [bp+var_2E], ax
    mov     al, byte_42D2A
    jmp     loc_18BD2
    ; align 2
    db 144
loc_18CB6:
    mov     byte_459D8, 1
    inc     word_449E4
    cmp     word_449E4, 28h ; '('
    jnz     short loc_18CCC
    mov     word_449E4, 0
loc_18CCC:
    mov     al, byte_454B8
    mov     byte_3BE02, al
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_188A4 endp
sub_18CD8 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = byte ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    cmp     byte_459D8, 0
    jz      short loc_18D04
    test    [bp+arg_0], 10h
    jz      short loc_18CF3
    push    [bp+arg_2]
    call    sub_27374
    add     sp, 2
loc_18CF3:
    test    [bp+arg_0], 20h
    jz      short loc_18D04
    push    [bp+arg_2]
    call    sub_27338
    add     sp, 2
loc_18D04:
    pop     bp
    retf
sub_18CD8 endp
sub_18D06 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    push    [bp+arg_2]
    mov     bx, [bp+arg_0]
    push    word ptr [bx+10h]
    push    word ptr [bx+0Eh]
    push    word ptr [bx+0Ch]
    push    word ptr [bx+0Ah]
    push    word ptr [bx+8]
    push    word ptr [bx+6]
    push    word ptr [bx+1Eh]
    push    word_43964
    call    sub_2712E
    add     sp, 12h
    cmp     byte_449AA, 0
    jz      short loc_18D5E
    push    [bp+arg_2]
    mov     bx, [bp+arg_0]
    push    word ptr [bx+1Ch]
    push    word ptr [bx+1Ah]
    push    word ptr [bx+18h]
    push    word ptr [bx+16h]
    push    word ptr [bx+14h]
    push    word ptr [bx+12h]
    push    word ptr [bx+20h]
    push    word_4408C
    call    sub_2712E
    add     sp, 12h
loc_18D5E:
    pop     bp
    retf
sub_18D06 endp
sub_18D60 proc far
    var_30 = word ptr -48
    var_2E = word ptr -46
    var_2C = word ptr -44
    var_2A = word ptr -42
    var_28 = byte ptr -40
    var_26 = word ptr -38
    var_24 = word ptr -36
    var_22 = word ptr -34
    var_20 = word ptr -32
    var_1E = word ptr -30
    var_1C = byte ptr -28
    var_1A = word ptr -26
    var_18 = byte ptr -24
    var_16 = byte ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = byte ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = dword ptr -6
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = byte ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 30h
    push    di
    push    si
    mov     bx, [bp+arg_0]
    les     si, dword_449D4
    mov     al, es:[bx+si]
    mov     [bp+var_2], al
    les     si, dword_454D6
    mov     al, es:[bx+si]
smart
    and     al, 0Fh
nosmart
    mov     [bp+var_1C], al
    mov     al, es:[bx+si]
smart
    and     al, 10h
nosmart
    mov     [bp+var_28], al
    mov     al, [bp+var_2]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, 2098h
    mov     [bp+var_26], ax
    mov     bx, ax
    mov     ax, [bx]
    mov     [bp+var_14], ax
    mov     [bp+var_12], ds
    mov     al, [bp+var_1C]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_14]
    mov     dx, ds
    mov     word ptr [bp+var_6], ax
    mov     word ptr [bp+var_6+2], dx
    mov     [bp+var_24], 0
    les     bx, [bp+var_6]
    mov     al, es:[bx+5]
    mov     [bp+var_18], al
    cmp     [bp+var_28], 0
    jnz     short loc_18DE2
    mov     al, [bp+arg_4]
    shl     al, 1
    jmp     short loc_18DEC
loc_18DE2:
    mov     al, [bp+var_18]
    sub     al, [bp+arg_4]
    shl     al, 1
    sub     al, 2
loc_18DEC:
    mov     [bp+var_10], al
    cmp     [bp+arg_6], 0
    jz      short loc_18E1A
    mov     al, es:[bx+0Dh]
    mov     [bp+var_16], al
    mov     bx, [bp+var_26]
    mov     al, [bx+9]
    mov     byte ptr [bp+var_1A], al
    mov     si, [bp+var_1A]
smart
    and     si, 0FFh
nosmart
    mov     bl, [bp+var_16]
    sub     bh, bh
    mov     al, [bx+si-6A20h]
    mov     bx, [bp+arg_6]
    mov     [bx], al
loc_18E1A:
    les     bx, [bp+var_6]
    cmp     word ptr es:[bx+0Ah], 0
    jz      short loc_18E29
    mov     [bp+var_24], 1
loc_18E29:
    cmp     [bp+var_28], 0
    jz      short loc_18E76
    cmp     [bp+var_24], 0
    jz      short loc_18E3C
    mov     ax, es:[bx+0Ah]
    jmp     short loc_18E7D
    ; align 2
    db 144
loc_18E3C:
    les     bx, [bp+var_6]
    mov     ax, es:[bx+8]
    mov     [bp+var_2C], ax
    mov     [bp+var_2A], ds
    mov     al, [bp+var_10]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_2C]
    mov     dx, ds
    mov     [bp+var_30], ax
    mov     [bp+var_2E], dx
    add     ax, 6
    lea     di, [bp+var_C]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    movsw
    movsw
    movsw
    pop     ds
    mov     ax, [bp+var_30]
    jmp     short loc_18EAA
    ; align 2
    db 144
loc_18E76:
    les     bx, [bp+var_6]
    mov     ax, es:[bx+8]
loc_18E7D:
    mov     [bp+var_2C], ax
    mov     [bp+var_2A], ds
    mov     al, [bp+var_10]
    sub     ah, ah
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    add     ax, [bp+var_2C]
    mov     dx, ds
    mov     [bp+var_30], ax
    mov     [bp+var_2E], dx
    lea     di, [bp+var_C]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    movsw
    movsw
    movsw
    pop     ds
    add     ax, 6
loc_18EAA:
    lea     di, [bp+var_22]
    mov     si, ax
    push    ds
    mov     ds, dx
    movsw
    movsw
    movsw
    pop     ds
    les     bx, [bp+var_6]
    mov     ax, es:[bx+6]
    cmp     ax, 100h
    jnz     short loc_18EC5
    jmp     loc_18F74
loc_18EC5:
    cmp     ax, 200h
    jnz     short loc_18ECD
    jmp     loc_18F52
loc_18ECD:
    cmp     ax, 300h
    jnz     short loc_18EFA
    mov     ax, [bp+var_C]
    mov     [bp+var_E], ax
    mov     ax, [bp+var_8]
    neg     ax
    mov     [bp+var_C], ax
    mov     ax, [bp+var_E]
    mov     [bp+var_8], ax
    mov     ax, [bp+var_22]
    mov     [bp+var_E], ax
    mov     ax, [bp+var_1E]
    neg     ax
    mov     [bp+var_22], ax
    mov     ax, [bp+var_E]
loc_18EF7:
    mov     [bp+var_1E], ax
loc_18EFA:
    mov     bx, [bp+arg_0]
    les     si, dword_4563C
    mov     al, es:[bx+si]
    mov     [bp+var_16], al
    les     si, dword_459DA
    mov     al, es:[bx+si]
    mov     byte ptr [bp+var_1A], al
    cmp     [bp+var_A], 0FFFFh
    jz      short loc_18F3B
    mov     bl, al
    sub     bh, bh
    shl     bx, 1
    mov     bx, [bx-73C4h]
    mov     al, [bp+var_16]
    sub     ah, ah
    add     bx, ax
    les     si, dword_454BC
    cmp     byte ptr es:[bx+si], 6
    jnz     short loc_18F3B
    mov     ax, word_3B8EA
    add     [bp+var_A], ax
    add     [bp+var_20], ax
loc_18F3B:
    mov     bx, [bp+var_26]
    test    byte ptr [bx+0Bh], 1
    jz      short loc_18F9C
    mov     bl, byte ptr [bp+var_1A]
    sub     bh, bh
    shl     bx, 1
    mov     si, [bx-7E84h]
    jmp     short loc_18FA7
    ; align 2
    db 144
loc_18F52:
    mov     ax, [bp+var_8]
    neg     ax
    mov     [bp+var_8], ax
    mov     ax, [bp+var_C]
    neg     ax
    mov     [bp+var_C], ax
    mov     ax, [bp+var_1E]
    neg     ax
    mov     [bp+var_1E], ax
    mov     ax, [bp+var_22]
    neg     ax
    mov     [bp+var_22], ax
    jmp     short loc_18EFA
loc_18F74:
    mov     ax, [bp+var_C]
    mov     [bp+var_E], ax
    mov     ax, [bp+var_8]
    mov     [bp+var_C], ax
    mov     ax, [bp+var_E]
    neg     ax
    mov     [bp+var_8], ax
    mov     ax, [bp+var_22]
    mov     [bp+var_E], ax
    mov     ax, [bp+var_1E]
    mov     [bp+var_22], ax
    mov     ax, [bp+var_E]
    neg     ax
    jmp     loc_18EF7
loc_18F9C:
    mov     bl, byte ptr [bp+var_1A]
    sub     bh, bh
    shl     bx, 1
    mov     si, [bx-55EAh]
loc_18FA7:
    add     [bp+var_8], si
    add     [bp+var_1E], si
    mov     bx, [bp+var_26]
    test    byte ptr [bx+0Bh], 2
    jz      short loc_18FC4
    mov     bl, [bp+var_16]
    sub     bh, bh
    shl     bx, 1
    mov     si, [bx+7558h]
    jmp     short loc_18FCF
    ; align 2
    db 144
loc_18FC4:
    mov     bl, [bp+var_16]
    sub     bh, bh
    shl     bx, 1
    mov     si, [bx-599Eh]
loc_18FCF:
    add     [bp+var_C], si
    add     [bp+var_22], si
    mov     ax, [bp+var_22]
    cwd
    mov     cx, ax
    mov     ax, [bp+var_C]
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     bx, [bp+arg_2]
    mov     [bx], ax
    cmp     [bp+var_A], 0FFFFh
    jnz     short loc_18FFE
    mov     bx, [bp+arg_2]
    mov     word ptr [bx+2], 0FFFFh
    jmp     short loc_19018
loc_18FFE:
    mov     ax, [bp+var_20]
    cwd
    mov     cx, ax
    mov     ax, [bp+var_A]
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     bx, [bp+arg_2]
    mov     [bx+2], ax
loc_19018:
    mov     ax, [bp+var_1E]
    cwd
    mov     cx, ax
    mov     ax, [bp+var_8]
    mov     bx, dx
    cwd
    add     ax, cx
    adc     dx, bx
    sar     dx, 1
    rcr     ax, 1
    mov     bx, [bp+arg_2]
    mov     [bx+4], ax
    mov     bx, [bp+arg_2]
    lea     di, [bx+6]
    lea     si, [bp+var_C]
    push    ds
    pop     es
    movsw
    movsw
    movsw
    mov     bx, [bp+arg_2]
    lea     di, [bx+0Ch]
    lea     si, [bp+var_22]
    movsw
    movsw
    movsw
    mov     bx, [bp+arg_2]
    mov     ax, [bp+var_24]
    mov     [bx+12h], ax
    mov     al, [bp+arg_4]
    cbw
    mov     cl, [bp+var_18]
    sub     ch, ch
    dec     cx
    cmp     cx, ax
    jnz     short loc_1906C
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_1906C:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_18D60 endp
sub_19074 proc far
    var_2A = word ptr -42
    var_28 = word ptr -40
    var_26 = word ptr -38
    var_24 = byte ptr -36
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_4 = word ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 2Ah
    push    di
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bx+6]
    mov     bx, [bp+arg_4]
    add     si, [bx+6]
    mov     bx, [bp+arg_2]
    mov     di, [bp+arg_6]
    mov     ax, [di]
    cmp     [bx], ax
    jl      short loc_19098
    mov     ax, [bx]
    mov     bx, di
loc_19098:
    sub     ax, [bx]
    cmp     ax, si
    jg      short loc_190D2
    mov     bx, [bp+arg_2]
    mov     di, [bp+arg_6]
    mov     ax, [di+4]
    cmp     [bx+4], ax
    jl      short loc_190B1
    mov     ax, [bx+4]
    mov     bx, di
loc_190B1:
    sub     ax, [bx+4]
    cmp     ax, si
    jg      short loc_190D2
    mov     bx, [bp+arg_2]
    mov     di, [bp+arg_6]
    mov     ax, [di+2]
    cmp     [bx+2], ax
    jl      short loc_190CB
    mov     ax, [bx+2]
    mov     bx, di
loc_190CB:
    sub     ax, [bx+2]
    cmp     ax, si
    jle     short loc_190DA
loc_190D2:
    sub     ax, ax
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_190DA:
    mov     bx, [bp+arg_2]
    mov     ax, [bx]
    mov     bx, [bp+arg_6]
    sub     ax, [bx]
    mov     [bp+var_2A], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+2]
    mov     bx, [bp+arg_6]
    sub     ax, [bx+2]
    mov     [bp+var_28], ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+4]
    mov     bx, [bp+arg_6]
    sub     ax, [bx+4]
    mov     [bp+var_26], ax
    lea     ax, [bp+var_2A]
    push    ax
    call    sub_2EA08
    add     sp, 2
    cmp     ax, si
    ja      short loc_190D2
    sub     ax, ax
    push    ax
    mov     bx, [bp+arg_2]
    mov     ax, [bx+0Ah]
    neg     ax
    push    ax
    mov     ax, [bx+8]
    neg     ax
    push    ax
    mov     ax, [bx+6]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_6], 0
    jmp     short loc_191B2
loc_1913E:
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    neg     ax
loc_19145:
    mov     [bp+var_C], ax
    mov     [bp+var_A], 0
    mov     al, [bp+var_6]
    cbw
    mov     bx, ax
    shl     bx, 1
    cmp     word ptr [bx+69Ch], 0
    jnz     short loc_19164
    mov     bx, [bp+arg_0]
    mov     ax, [bx+4]
    jmp     short loc_1916C
loc_19164:
    mov     bx, [bp+arg_0]
    mov     ax, [bx+4]
    neg     ax
loc_1916C:
    mov     [bp+var_8], ax
    lea     ax, [bp+var_2A]
    push    ax
    push    [bp+var_4]
    lea     ax, [bp+var_C]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     bx, [bp+arg_2]
    mov     ax, [bx]
    add     [bp+var_2A], ax
    mov     ax, [bx+2]
    add     [bp+var_28], ax
    mov     ax, [bx+4]
    add     [bp+var_26], ax
    mov     al, [bp+var_6]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    push    si
    lea     di, [bp+di+var_24]
    lea     si, [bp+var_2A]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    inc     [bp+var_6]
loc_191B2:
    cmp     [bp+var_6], 4
    jge     short loc_191D2
    mov     al, [bp+var_6]
    cbw
    mov     bx, ax
    shl     bx, 1
    cmp     word ptr [bx+694h], 0
    jz      short loc_191CA
    jmp     loc_1913E
loc_191CA:
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    jmp     loc_19145
loc_191D2:
    mov     ax, 1
    push    ax
    mov     bx, [bp+arg_6]
    push    word ptr [bx+0Ah]
    push    word ptr [bx+8]
    push    word ptr [bx+6]
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_6], 0
    jmp     short loc_191F7
    ; align 2
    db 144
loc_191F4:
    inc     [bp+var_6]
loc_191F7:
    cmp     [bp+var_6], 4
    jge     short loc_1927A
    mov     al, [bp+var_6]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    mov     bx, [bp+arg_6]
    mov     ax, [bx]
    sub     ax, [di-24h]
    mov     [bp+var_C], ax
    mov     ax, [bx+2]
    sub     ax, [di-22h]
    mov     [bp+var_A], ax
    mov     ax, [bx+4]
    sub     ax, [di-20h]
    mov     [bp+var_8], ax
    lea     ax, [bp+var_2A]
    push    ax
    push    [bp+var_4]
    lea     ax, [bp+var_C]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_28], 0
    jl      short loc_191F4
    mov     bx, [bp+arg_4]
    mov     ax, [bp+var_28]
    cmp     [bx+2], ax
    jl      short loc_191F4
    mov     di, [bx]
    mov     ax, di
    neg     ax
    cmp     [bp+var_2A], ax
    jl      short loc_191F4
    cmp     [bp+var_2A], di
    jg      short loc_191F4
    mov     ax, [bx+4]
    neg     ax
    cmp     ax, [bp+var_26]
    jg      short loc_191F4
    mov     ax, [bp+var_26]
    cmp     [bx+4], ax
    jl      short loc_191F4
loc_19270:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_1927A:
    sub     ax, ax
    push    ax
    mov     bx, [bp+arg_6]
    mov     ax, [bx+0Ah]
    neg     ax
    push    ax
    mov     ax, [bx+8]
    neg     ax
    push    ax
    mov     ax, [bx+6]
    neg     ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_6], 0
    jmp     short loc_19318
    ; align 2
    db 144
loc_192A4:
    mov     bx, [bp+arg_4]
    mov     ax, [bx]
    neg     ax
loc_192AB:
    mov     [bp+var_C], ax
    mov     [bp+var_A], 0
    mov     al, [bp+var_6]
    cbw
    mov     bx, ax
    shl     bx, 1
    cmp     word ptr [bx+69Ch], 0
    jnz     short loc_192CA
    mov     bx, [bp+arg_4]
    mov     ax, [bx+4]
    jmp     short loc_192D2
loc_192CA:
    mov     bx, [bp+arg_4]
    mov     ax, [bx+4]
    neg     ax
loc_192D2:
    mov     [bp+var_8], ax
    lea     ax, [bp+var_2A]
    push    ax
    push    [bp+var_4]
    lea     ax, [bp+var_C]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     bx, [bp+arg_6]
    mov     ax, [bx]
    add     [bp+var_2A], ax
    mov     ax, [bx+2]
    add     [bp+var_28], ax
    mov     ax, [bx+4]
    add     [bp+var_26], ax
    mov     al, [bp+var_6]
    cbw
    mov     di, ax
    shl     di, 1
    add     di, ax
    shl     di, 1
    push    si
    lea     di, [bp+di+var_24]
    lea     si, [bp+var_2A]
    push    ss
    pop     es
    movsw
    movsw
    movsw
    pop     si
    inc     [bp+var_6]
loc_19318:
    cmp     [bp+var_6], 4
    jge     short loc_19338
    mov     al, [bp+var_6]
    cbw
    mov     bx, ax
    shl     bx, 1
    cmp     word ptr [bx+694h], 0
    jz      short loc_19330
    jmp     loc_192A4
loc_19330:
    mov     bx, [bp+arg_4]
    mov     ax, [bx]
    jmp     loc_192AB
loc_19338:
    mov     ax, 1
    push    ax
    mov     bx, [bp+arg_2]
    push    word ptr [bx+0Ah]
    push    word ptr [bx+8]
    push    word ptr [bx+6]
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_6], 0
    jmp     short loc_1935D
    ; align 2
    db 144
loc_1935A:
    inc     [bp+var_6]
loc_1935D:
    cmp     [bp+var_6], 4
    jl      short loc_19366
    jmp     loc_190D2
loc_19366:
    mov     al, [bp+var_6]
    cbw
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    add     di, bp
    mov     bx, [bp+arg_2]
    mov     ax, [bx]
    sub     ax, [di-24h]
    mov     [bp+var_C], ax
    mov     ax, [bx+2]
    sub     ax, [di-22h]
    mov     [bp+var_A], ax
    mov     ax, [bx+4]
    sub     ax, [di-20h]
    mov     [bp+var_8], ax
    lea     ax, [bp+var_2A]
    push    ax
    push    [bp+var_4]
    lea     ax, [bp+var_C]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    cmp     [bp+var_28], 0
    jl      short loc_1935A
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_28]
    cmp     [bx+2], ax
    jl      short loc_1935A
    mov     di, [bx]
    mov     ax, di
    neg     ax
    cmp     [bp+var_2A], ax
    jl      short loc_1935A
    cmp     [bp+var_2A], di
    jg      short loc_1935A
    mov     ax, [bx+4]
    neg     ax
    cmp     ax, [bp+var_26]
    jg      short loc_1935A
    mov     ax, [bp+var_26]
    cmp     [bx+4], ax
    jl      short loc_193DC
    jmp     loc_19270
loc_193DC:
    jmp     loc_1935A
    ; align 2
    db 144
sub_19074 endp
sub_193E0 proc far
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    mov     ax, 0FFFDh
    push    ax
    push    cs
    call    near ptr init_game_state
    add     sp, 2
    sub     si, si
    mov     byte_44889, 2
    mov     word_454C2, offset unk_3B1C0
    mov     word_454C4, seg seg038
    mov     byte_4499A, 1
    mov     byte_4499E, 1Ch
    les     bx, dword_449D4
    mov     byte ptr es:[bx], 7
    les     bx, dword_4563C
    mov     byte ptr es:[bx], 1
    les     bx, dword_459DA
    mov     al, byte_4499E
    mov     es:[bx], al
    les     bx, dword_454D6
    mov     byte ptr es:[bx], 0
    les     bx, dword_449D4
    mov     byte ptr es:[bx+1], 6
    les     bx, dword_4563C
    mov     byte ptr es:[bx+1], 0
    les     bx, dword_459DA
    mov     al, byte_4499E
    mov     es:[bx+1], al
    les     bx, dword_454D6
    mov     byte ptr es:[bx+1], 0
    les     bx, dword_449D4
    mov     byte ptr es:[bx+2], 8
    les     bx, dword_4563C
    mov     byte ptr es:[bx+2], 0
    les     bx, dword_459DA
    mov     al, byte_4499E
    inc     al
    mov     es:[bx+2], al
    les     bx, dword_454D6
    mov     byte ptr es:[bx+2], 0
    les     bx, dword_449D4
    mov     byte ptr es:[bx+3], 9
    les     bx, dword_4563C
    mov     byte ptr es:[bx+3], 1
    les     bx, dword_459DA
    mov     al, byte_4499E
    inc     al
    mov     es:[bx+3], al
    les     bx, dword_454D6
    mov     byte ptr es:[bx+3], 0
    les     bx, dword_449D4
    mov     byte ptr es:[bx+4], 7
    les     bx, dword_4563C
    mov     byte ptr es:[bx+4], 1
    les     bx, dword_459DA
    mov     al, byte_4499E
    mov     es:[bx+4], al
    les     bx, dword_454D6
    mov     byte ptr es:[bx+4], 0
    les     bx, dword_43942
    mov     es:[bx], si
    les     bx, dword_43942
    mov     word ptr es:[bx+2], 1
    les     bx, dword_43942
    mov     word ptr es:[bx+4], 2
    les     bx, dword_43942
    mov     word ptr es:[bx+6], 3
    les     bx, dword_43942
    mov     word ptr es:[bx+8], 4
    les     bx, dword_43942
    mov     word ptr es:[bx+0Ah], 1
    les     bx, dword_43942
    mov     word ptr es:[bx+0Ch], 2
    les     bx, dword_43942
    mov     word ptr es:[bx+0Eh], 3
    les     bx, dword_43942
    mov     word ptr es:[bx+10h], 4
    les     bx, dword_43942
    mov     word ptr es:[bx+12h], 1
    les     bx, dword_43942
    mov     word ptr es:[bx+14h], 2
    les     bx, dword_43942
    mov     word ptr es:[bx+16h], 3
    les     bx, dword_43942
    mov     word ptr es:[bx+18h], 4
    les     bx, dword_43942
    mov     es:[bx+1Ah], si
    les     bx, dword_43942
    mov     word ptr es:[bx+1Ch], 1
    les     bx, dword_43942
    mov     word ptr es:[bx+1Eh], 2
    les     bx, dword_43942
    mov     word ptr es:[bx+20h], 3
    les     bx, dword_43942
    mov     es:[bx+22h], si
    mov     byte_44D50, 0C8h ; 'È'
    sub     ax, ax
    push    ax
    mov     ax, word_43924
    add     ax, 12Eh
    cwd
    mov     cl, 6
loc_1958C:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jnz     short loc_1958C
    push    dx
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    mov     ax, 7700h
    mov     dx, 1
    push    dx
    push    ax
    mov     ax, dx
    push    ax
    mov     ax, 9ED0h
    push    ax
    mov     ax, 8F46h
    push    ax
    push    cs
    call    near ptr sub_16898
    add     sp, 14h
    mov     ax, 911Dh
    push    ax
    mov     al, byte_44784
    inc     byte_44784
    sub     ah, ah
    push    ax
    mov     ax, 8FEAh
    push    ax
    mov     bx, word_44700
    shl     bx, 1
    les     di, dword_43942
    push    word ptr es:[bx+di]
    push    cs
    call    near ptr sub_18D60
    add     sp, 8
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_193E0 endp
sub_195E0 proc far

    push    cs
    call    near ptr sub_14712
    retf
    ; align 2
    db 144
sub_195E0 endp
sub_195E6 proc far
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    mov     ax, [bp+arg_2]
    or      ax, ax
    jz      short loc_195FC
    cmp     ax, 1
    jz      short loc_19612
    jmp     short loc_19601
loc_195FC:
    mov     [bp+var_4], 8E76h
loc_19601:
    mov     bx, [bp+var_4]
    cmp     byte ptr [bx+0C9h], 0
    jz      short loc_1961A
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_19612:
    mov     [bp+var_4], 8F46h
    jmp     short loc_19601
    ; align 2
    db 144
loc_1961A:
    mov     [bp+var_2], 0
    mov     ax, [bp+arg_0]
    cmp     ax, 1
    jz      short loc_1967F
    cmp     ax, 2
    jnz     short loc_1962E
    jmp     loc_196DE
loc_1962E:
    cmp     ax, 3
    jnz     short loc_19636
    jmp     loc_19730
loc_19636:
    cmp     ax, 4
    jz      short loc_19642
    cmp     ax, 5
    jz      short loc_19676
    jmp     short loc_1964E
loc_19642:
    mov     word_445CC, 1
    mov     word_445CE, 1
loc_1964E:
    cmp     [bp+var_2], 0
    jz      short loc_19664
    mov     bx, [bp+var_4]
    mov     word ptr [bx+2Ch], 0
    mov     bx, [bp+var_4]
    mov     word ptr [bx+2Ah], 0
loc_19664:
    cmp     [bp+arg_2], 0
    jnz     short loc_1966D
    jmp     loc_19760
loc_1966D:
    mov     ax, word_445D4
    mov     word_445DC, ax
    jmp     loc_19766
loc_19676:
    mov     [bp+arg_0], 1
    mov     [bp+var_2], 1
loc_1967F:
    mov     bx, [bp+var_4]
    mov     byte ptr [bx+0C9h], 1
    sub     ax, ax
    push    ax
    mov     bx, [bp+var_4]
    push    word ptr [bx+18h]
    push    [bp+arg_2]
    push    cs
    call    near ptr sub_19A2C
    add     sp, 6
    cmp     [bp+arg_2], 0
    jnz     short loc_196B3
    mov     bx, [bp+var_4]
    mov     ax, [bx+2Ch]
    mov     word_445E0, ax
    mov     ax, word_449D0
    shl     ax, 1
    shl     ax, 1
    mov     word_445CE, ax
loc_196B3:
    cmp     byte_454B8, 0
    jnz     short loc_1964E
    cmp     byte_459D8, 0
    jz      short loc_1964E
    cmp     [bp+arg_2], 0
    jnz     short loc_196CE
    push    word_43964
    jmp     short loc_196D2
    ; align 2
    db 144
loc_196CE:
    push    word_4408C
loc_196D2:
    call    sub_272F4
    add     sp, 2
    jmp     loc_1964E
    ; align 2
    db 144
loc_196DE:
    cmp     byte_454B8, 0
    jnz     short loc_19704
    cmp     byte_459D8, 0
    jz      short loc_19704
    cmp     [bp+arg_2], 0
    jnz     short loc_196F8
    push    word_43964
    jmp     short loc_196FC
loc_196F8:
    push    word_4408C
loc_196FC:
    call    sub_272F4
    add     sp, 2
loc_19704:
    mov     bx, [bp+var_4]
    mov     byte ptr [bx+0C9h], 2
    mov     [bp+var_2], 1
    cmp     [bp+arg_2], 0
    jz      short loc_19719
    jmp     loc_1964E
loc_19719:
    mov     bx, [bp+var_4]
    mov     ax, [bx+2Ch]
    mov     word_445E0, ax
    mov     ax, word_449D0
    shl     ax, 1
    shl     ax, 1
loc_19729:
    mov     word_445CE, ax
    jmp     loc_1964E
    ; align 2
    db 144
loc_19730:
    mov     bx, [bp+var_4]
    mov     byte ptr [bx+0C9h], 3
    cmp     [bp+arg_2], 0
    jnz     short loc_19752
    mov     ax, word_445D4
    add     ax, word_445DE
    add     ax, word_45A24
    mov     word_445D6, ax
    mov     ax, word_449D0
    jmp     short loc_19729
    ; align 2
    db 144
loc_19752:
    mov     ax, word_445D4
    add     ax, word_45A24
    mov     word_445D8, ax
    jmp     loc_1964E
    ; align 2
    db 144
loc_19760:
    mov     ax, word_445D4
    mov     word_445DA, ax
loc_19766:
    cmp     byte_4488A, 0
    jnz     short loc_19779
    cmp     [bp+arg_2], 0
    jnz     short loc_19779
    mov     al, byte ptr [bp+arg_0]
    mov     byte_4488A, al
loc_19779:
    test    byte_43966, 4
    jnz     short loc_1978D
    mov     di, 8BD8h
    mov     si, 8E60h
    push    ds
    pop     es
    mov     cx, 0Bh
    repne movsw
loc_1978D:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_195E6 endp
sub_19794 proc far
    var_36 = dword ptr -54
    var_32 = word ptr -50
    var_2E = word ptr -46
    var_2C = byte ptr -44
    var_1A = byte ptr -26
    var_8 = byte ptr -8
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 36h
    push    di
    push    si
    cmp     word_44F4A, 0FFFFh
    jnz     short loc_197A6
    jmp     loc_198C2
loc_197A6:
    mov     ax, 22h ; '"'
    imul    word_44F4A
    add     ax, word_454C2
    mov     dx, word_454C4
    mov     word ptr [bp+var_36], ax
    mov     word ptr [bp+var_36+2], dx
    les     bx, [bp+var_36]
    mov     ax, word_44F44
    cmp     es:[bx+2], ax
    jnz     short loc_197D6
    mov     ax, word_44F42
    cmp     es:[bx], ax
    jnz     short loc_197D6
    mov     si, word_44F46
    jmp     short loc_19845
    ; align 2
    db 144
loc_197D6:
    lea     ax, [bp+var_8]
    push    ax
    mov     ax, 8B62h
    push    ax
    mov     ax, 97C6h
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, 22h ; '"'
    imul    word_44F4A
    add     ax, word_454C2
    mov     dx, word_454C4
    add     ax, 10h
    push    si
    lea     di, [bp+var_1A]
    mov     si, ax
    push    ss
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 9
    repne movsw
    pop     ds
    pop     si
    lea     ax, [bp+var_2C]
    push    ax
    lea     ax, [bp+var_1A]
    push    ax
    call    mat_copy
    add     sp, 4
    lea     ax, [bp+var_32]
    push    ax
    lea     ax, [bp+var_2C]
    push    ax
    lea     ax, [bp+var_8]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    push    [bp+var_2E]
    mov     ax, [bp+var_32]
    neg     ax
    push    ax
    call    sub_2EA4E
    add     sp, 4
    mov     si, ax
loc_19845:
    add     si, word_44F48
    jz      short loc_198A4
    cmp     word_3BE16, si
    jz      short loc_19866
    mov     ax, si
    neg     ax
    push    ax
    mov     ax, 55E8h
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     word_3BE16, si
loc_19866:
    lea     ax, [bp+var_32]
    push    ax
    mov     ax, 55E8h
    push    ax
    mov     ax, 97C6h
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, 97CCh
    push    ax
    mov     ax, 22h ; '"'
    imul    word_44F4A
    add     ax, word_454C2
    mov     dx, word_454C4
    add     ax, 10h
    push    dx
    push    ax
    lea     ax, [bp+var_32]
loc_19895:
    push    ax
    push    cs
    call    near ptr sub_14D6C
    add     sp, 8
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
loc_198A4:
    mov     ax, 97CCh
    push    ax
    mov     ax, 22h ; '"'
    imul    word_44F4A
    add     ax, word_454C2
    mov     dx, word_454C4
    add     ax, 10h
    push    dx
    push    ax
    mov     ax, 97C6h
    jmp     short loc_19895
    ; align 2
    db 144
loc_198C2:
    cmp     word_44F48, 0
    jz      short loc_1990C
    mov     ax, word_3BE14
    cmp     word_44F48, ax
    jz      short loc_198EA
    mov     ax, word_44F48
    neg     ax
    push    ax
    mov     ax, 55D6h
    push    ax
    call    mat_rot_y
    add     sp, 4
    mov     ax, word_44F48
    mov     word_3BE14, ax
loc_198EA:
    lea     ax, [bp+var_32]
    push    ax
    mov     ax, 55D6h
    push    ax
    mov     ax, 97C6h
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     ax, 97CCh
    push    ax
    mov     ax, 8B62h
    push    ax
    lea     ax, [bp+var_32]
    jmp     short loc_19917
    ; align 2
    db 144
loc_1990C:
    mov     ax, 97CCh
    push    ax
    mov     ax, 8B62h
    push    ax
    mov     ax, 97C6h
loc_19917:
    push    ax
    call    mat_mul_vector
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_19794 endp
sub_19926 proc far
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = dword ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 10h
    mov     ax, word_44DC6
    cmp     [bp+arg_0], ax
    jnz     short loc_1993E
    mov     ax, word_44D64
    mov     dx, word_44D66
    jmp     short loc_1994C
    ; align 2
    db 144
loc_1993E:
    mov     ax, 22h ; '"'
    imul    [bp+arg_0]
    add     ax, word_454C2
    mov     dx, word_454C4
loc_1994C:
    mov     word ptr [bp+var_4], ax
    mov     word ptr [bp+var_4+2], dx
    les     bx, [bp+var_4]
    mov     ax, es:[bx+6]
    add     ax, word_449EC
    mov     [bp+var_E], ax
    mov     ax, [bp+arg_4]
    sub     ax, [bp+var_E]
    mov     [bp+var_8], ax
    cmp     [bp+arg_0], 4
    jl      short loc_199AE
    mov     ax, es:[bx+4]
    add     ax, word_449E0
    mov     [bp+var_10], ax
    mov     ax, es:[bx+8]
    add     ax, word_449F6
    mov     [bp+var_C], ax
    mov     ax, [bp+arg_2]
    sub     ax, [bp+var_10]
    mov     [bp+var_A], ax
    mov     ax, [bp+arg_6]
    sub     ax, [bp+var_C]
    mov     [bp+var_6], ax
    mov     ax, bx
    add     ax, 0Ah
    push    dx
    push    ax
    push    [bp+var_6]
    push    [bp+var_8]
    push    [bp+var_A]
    push    cs
    call    near ptr sub_199B2
    add     sp, 0Ah
loc_199AE:
    mov     sp, bp
    pop     bp
    retf
sub_19926 endp
sub_199B2 proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = dword ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    di
    push    si
    mov     ax, 2000h
    cwd
    push    dx
    push    ax
    mov     ax, [bp+arg_0]
    cwd
    push    dx
    push    ax
    les     bx, [bp+arg_6]
    mov     ax, es:[bx]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    mov     cx, ax
    mov     ax, [bp+arg_4]
    mov     bx, dx
    cwd
    push    dx
    push    ax
    les     si, [bp+arg_6]
    mov     ax, es:[si+4]
    cwd
    push    dx
    push    ax
    mov     si, cx
    mov     di, bx
    call    __aFlmul
    mov     cx, ax
    mov     ax, [bp+arg_2]
    mov     bx, dx
    cwd
    push    dx
    push    ax
    mov     ax, bx
    les     bx, [bp+arg_6]
    mov     dx, ax
    mov     ax, es:[bx+2]
    mov     [bp+var_4], cx
    mov     [bp+var_2], dx
    cwd
    push    dx
    push    ax
    call    __aFlmul
    add     ax, [bp+var_4]
    adc     dx, [bp+var_2]
    add     ax, si
    adc     dx, di
    push    dx
    push    ax
    call    __aFldiv
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_199B2 endp
sub_19A2C proc far
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 18h
    push    di
    push    si
    cmp     [bp+arg_0], 2
    jge     short loc_19A5E
    mov     ax, [bp+arg_2]
    mov     [bp+var_6], ax
    mov     [bp+var_2], 400h
    mov     [bp+var_E], 12h
    mov     ax, [bp+arg_0]
    shl     ax, 1
    shl     ax, 1
    add     ax, 4
    mov     [bp+var_10], ax
    mov     [bp+var_8], 6
    jmp     short loc_19A7B
loc_19A5E:
    mov     ax, [bp+arg_2]
    sub     ax, 60h ; '`'
    mov     [bp+var_6], ax
    mov     [bp+var_2], 0C0h ; 'À'
    mov     [bp+var_E], 8
    mov     [bp+var_10], 0
    mov     [bp+var_8], 1
loc_19A7B:
    mov     byte_448BE, 1
    mov     [bp+var_12], 0
    sub     si, si
loc_19A87:
    mov     bx, si
    shl     bx, 1
    cmp     word ptr [bx-6F4Eh], 0
    jnz     short loc_19A95
    inc     [bp+var_12]
loc_19A95:
    inc     si
    cmp     si, 18h
    jl      short loc_19A87
    mov     ax, [bp+var_E]
    cmp     [bp+var_12], ax
    jle     short loc_19AA6
    mov     [bp+var_12], ax
loc_19AA6:
    mov     [bp+var_C], 0
    sub     si, si
    jmp     short loc_19AB1
    ; align 2
    db 144
loc_19AB0:
    inc     si
loc_19AB1:
    cmp     si, 18h
    jl      short loc_19AB9
    jmp     loc_19B99
loc_19AB9:
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_14], ax
    mov     bx, ax
    cmp     word ptr [bx-6F4Eh], 0
    jnz     short loc_19AB0
    mov     al, byte ptr [bp+arg_0]
    mov     [si-6E99h], al
    mov     al, byte ptr [bp+var_C]
smart
    and     al, 3
nosmart
    add     al, byte ptr [bp+var_10]
    mov     [si-6EB1h], al
    mov     ax, si
    shl     ax, 1
    shl     ax, 1
    mov     [bp+var_16], ax
    mov     bx, ax
    sub     ax, ax
    mov     [bx-72DAh], ax
    mov     [bx-72DCh], ax
    mov     bx, [bp+var_16]
    mov     [bx-727Ah], ax
    mov     [bx-727Ch], ax
    mov     bx, [bp+var_16]
    mov     [bx-721Ah], ax
    mov     [bx-721Ch], ax
    call    sub_19E7B
    shl     ax, 1
    shl     ax, 1
    mov     bx, [bp+var_14]
    mov     [bx-6FDEh], ax
    call    sub_19E7B
    shl     ax, 1
    shl     ax, 1
    mov     bx, si
    shl     bx, 1
    mov     [bx-6FAEh], ax
    mov     ax, [bp+var_12]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_C]
    cwd
    push    dx
    push    ax
    mov     ax, [bp+var_2]
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFldiv
    add     ax, [bp+var_6]
smart
    and     ah, 3
nosmart
    mov     bx, si
    shl     bx, 1
    mov     [bx-6F7Eh], ax
    call    sub_19E7B
    mov     cx, ax
    shl     ax, 1
    add     ax, cx
    shl     ax, 1
    mov     di, ax
    mov     cl, 2
    sar     di, cl
    add     di, [bp+arg_4]
    add     di, 180h
    mov     ax, si
    shl     ax, 1
    mov     [bp+var_18], ax
    mov     bx, ax
    mov     [bx-6F4Eh], di
    mov     ax, [bp+var_8]
    imul    di
    sar     ax, 1
    sar     ax, 1
    mov     bx, [bp+var_18]
    mov     [bx-6F1Eh], ax
    inc     [bp+var_C]
    mov     ax, [bp+var_12]
    cmp     [bp+var_C], ax
    jz      short loc_19B99
    jmp     loc_19AB0
loc_19B99:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_19A2C endp
sub_19BA0 proc far
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_4 = word ptr -4
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2

    push    bp
    mov     bp, sp
    sub     sp, 14h
    push    di
    push    si
    mov     [bp+var_2], 0
    sub     si, si
    jmp     short loc_19BC3
loc_19BB0:
    mov     [bp+var_2], 1
    mov     di, si
    shl     di, 1
    add     word ptr [di-6FDEh], 10h
    add     word ptr [di-6FAEh], 10h
loc_19BC2:
    inc     si
loc_19BC3:
    cmp     si, 18h
    jl      short loc_19BCB
    jmp     loc_19C96
loc_19BCB:
    mov     di, si
    shl     di, 1
    cmp     word ptr [di-6F4Eh], 0
    jz      short loc_19BC2
    mov     ax, 1
    push    ax
    push    word ptr [di-6F7Eh]
    sub     ax, ax
    push    ax
    push    ax
    call    mat_rot_zxy
    add     sp, 8
    mov     [bp+var_4], ax
    mov     [bp+var_C], 0
    mov     [bp+var_A], 0
    mov     bx, si
    shl     bx, 1
    mov     ax, [bx-6F4Eh]
    mov     [bp+var_8], ax
    lea     ax, [bp+var_12]
    push    ax
    push    [bp+var_4]
    lea     ax, [bp+var_C]
    push    ax
    call    mat_mul_vector
    add     sp, 6
    mov     di, si
    mov     cl, 2
    shl     di, cl
    mov     ax, [bp+var_12]
    cwd
    add     [di-72DCh], ax
    adc     [di-72DAh], dx
    mov     ax, [bp+var_E]
    cwd
    add     [di-721Ch], ax
    adc     [di-721Ah], dx
    mov     ax, si
    shl     ax, 1
    add     ax, 90E2h
    mov     [bp+var_14], ax
    mov     bx, ax
    sub     word ptr [bx], 13h
    mov     bx, [bp+var_14]
    mov     ax, [bx]
    cwd
    add     [di-727Ch], ax
    adc     [di-727Ah], dx
    cmp     word_449D0, 0Ah
    jnz     short loc_19C6B
    mov     bx, [bp+var_14]
    sub     word ptr [bx], 13h
    mov     bx, [bp+var_14]
    mov     ax, [bx]
    cwd
    add     [di-727Ch], ax
    adc     [di-727Ah], dx
loc_19C6B:
    mov     bx, si
    shl     bx, 1
    shl     bx, 1
    mov     ax, [bx-727Ch]
    mov     dx, [bx-727Ah]
    add     ax, word_445EA
    adc     dx, word_445EC
    or      dx, dx
    jl      short loc_19C88
    jmp     loc_19BB0
loc_19C88:
    mov     bx, si
    shl     bx, 1
    mov     word ptr [bx-6F4Eh], 0
    jmp     loc_19BC2
    ; align 2
    db 144
loc_19C96:
    mov     al, [bp+var_2]
    mov     byte_448BE, al
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_19BA0 endp
sub_19CA2 proc far
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
    cmp     [bp+arg_4], 0
    jz      short loc_19CB3
    jmp     loc_19D36
loc_19CB3:
    mov     ax, 6A8h
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_28A8C
    add     sp, 6
    push    si
    mov     di, 0A6EAh
    mov     si, ax
    push    ds
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 184h
    repne movsw
    pop     ds
    pop     si
    mov     ax, word ptr dword_44D3E
    mov     dx, word ptr dword_44D3E+2
    mov     word_4615E, ax
    mov     word_46160, dx
    sub     si, si
loc_19CE7:
    mov     ax, si
    cwd
    push    dx
    push    ax
    cwd
    push    dx
    push    ax
    mov     ax, word_45E92
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFlmul
    mov     cl, 9
loc_19D03:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_19D03
    mov     bx, si
    shl     bx, 1
    les     di, dword_44D3E
    mov     es:[bx+di], ax
    inc     si
    cmp     si, 40h ; '@'
    jl      short loc_19CE7
    mov     ax, 6ADh
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_28A8C
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 8C14h
    jmp     loc_19DB6
loc_19D36:
    mov     ax, 6B2h
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_28A8C
    add     sp, 6
    push    si
    mov     di, 9ED0h
    mov     si, ax
    push    ds
    pop     es
    push    ds
    mov     ds, dx
    mov     cx, 184h
    repne movsw
    pop     ds
    pop     si
    mov     ax, word ptr dword_44D4A
    mov     dx, word ptr dword_44D4A+2
    mov     word_45944, ax
    mov     word_45946, dx
    sub     si, si
loc_19D6A:
    mov     ax, si
    cwd
    push    dx
    push    ax
    cwd
    push    dx
    push    ax
    mov     ax, word_45678
    cwd
    push    dx
    push    ax
    call    __aFlmul
    push    dx
    push    ax
    call    __aFlmul
    mov     cl, 9
loc_19D86:
    sar     dx, 1
    rcr     ax, 1
    dec     cl
    jnz     short loc_19D86
    mov     bx, si
    shl     bx, 1
    les     di, dword_44D4A
    mov     es:[bx+di], ax
    inc     si
    cmp     si, 40h ; '@'
    jl      short loc_19D6A
    mov     ax, 6B7h
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_28A8C
    add     sp, 6
    push    dx
    push    ax
    mov     ax, 81F7h
loc_19DB6:
    push    ax
    call    sub_28AD6
    add     sp, 6
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_19CA2 endp
seg001 ends
end
