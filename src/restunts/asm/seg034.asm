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
    include seg031.inc
    include seg032.inc
    include seg033.inc
    include seg035.inc
    include seg036.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg034 segment byte public 'STUNTSC' use16
    assume cs:seg034
    assume es:nothing, ss:nothing, ds:dseg
    public load_2dshape_1
    public load_2dshape_0
    public load_2dshape
    public sub_3ACB0
    ; align 2
    db 144
load_2dshape_1 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    mov     ax, 1
    push    ax
    push    [bp+arg_0]
    push    cs
    call    near ptr load_2dshape
    add     sp, 4
    pop     bp
    retf
    ; align 2
    db 144
load_2dshape_1 endp
load_2dshape_0 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    sub     ax, ax
    push    ax
    push    [bp+arg_0]
    push    cs
    call    near ptr load_2dshape
    add     sp, 4
    pop     bp
    retf
load_2dshape_0 endp
load_2dshape proc far
    var_80 = byte ptr -128
    var_7E = word ptr -126
    var_7C = byte ptr -124
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = dword ptr -16
    var_C = dword ptr -12
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 80h
    push    si
    push    [bp+arg_0]
    lea     ax, [bp+var_7C]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    lea     ax, [bp+var_7C]
    mov     word ptr [bp+var_C], ax
    jmp     short loc_3AA25
    ; align 2
    db 144
loc_3AA1C:
    cmp     [bp+var_80], 2Eh ; '.'
    jz      short loc_3AA31
    inc     word ptr [bp+var_C]
loc_3AA25:
    mov     bx, word ptr [bp+var_C]
    mov     al, [bx]
    mov     [bp+var_80], al
    or      al, al
    jnz     short loc_3AA1C
loc_3AA31:
    mov     bx, word ptr [bp+var_C]
    cmp     byte ptr [bx], 0
    jnz     short loc_3AA92
    mov     [bp+var_14], 0
    jmp     short loc_3AA53
loc_3AA40:
    lea     ax, [bp+var_7C]
    push    ax
    call    find_filename
    add     sp, 2
    or      ax, ax
    jnz     short loc_3AAA8
    inc     [bp+var_14]
loc_3AA53:
    mov     bx, [bp+var_14]
    shl     bx, 1
    mov     si, [bx+5466h]
    cmp     byte ptr [si], 0
    jz      short loc_3AAA8
    mov     ax, word ptr [bp+var_C]
    mov     [bp+var_12], ax
    push    si
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    lea     ax, [bp+var_7C]
    push    ax
    call    mmgr_get_unk
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      dx, ax
    jz      short loc_3AA40
loc_3AA87:
    mov     ax, [bp+var_4]
    mov     dx, [bp+var_2]
    pop     si
    mov     sp, bp
    pop     bp
    retf
loc_3AA92:
    lea     ax, [bp+var_7C]
    push    ax
    call    mmgr_get_unk
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      dx, ax
    jnz     short loc_3AA87
loc_3AAA8:
    push    word ptr [bp+var_C]
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _strcpy
    add     sp, 4
    mov     ax, offset a_pvs; ".PVS"
    push    ax
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _stricmp
    add     sp, 4
    or      ax, ax
    jnz     short loc_3AB24
    push    [bp+arg_2]
    lea     ax, [bp+var_7C]
    push    ax
    call    sub_30DE6
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_3AA87
    push    dx
    push    [bp+var_4]
    call    get_unflip_size
    add     sp, 4
    push    ax
    mov     ax, offset aUnflip_0; "UNFLIP"
    push    ax
    call    mmgr_alloc_pages
    add     sp, 4
    mov     word ptr [bp+var_10], ax
    mov     word ptr [bp+var_10+2], dx
    push    dx
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    sub_32AE2
    add     sp, 8
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    call    mmgr_op_unk2
    add     sp, 4
    jmp     loc_3AA87
loc_3AB24:
    mov     ax, offset a_xvs; ".XVS"
    push    ax
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _stricmp
    add     sp, 4
    or      ax, ax
    jnz     short loc_3AB50
    push    [bp+arg_2]
    lea     ax, [bp+var_7C]
    push    ax
    call    sub_30DE6
loc_3AB44:
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    jmp     loc_3AA87
loc_3AB50:
    mov     ax, offset a_pes; ".PES"
    push    ax
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _stricmp
    add     sp, 4
    or      ax, ax
    jz      short loc_3AB67
    jmp     loc_3AC1C
loc_3AB67:
    push    [bp+arg_2]
    lea     ax, [bp+var_7C]
    push    ax
    call    sub_30DE6
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jnz     short loc_3AB83
    jmp     loc_3AA87
loc_3AB83:
    mov     ax, 3E8h
    push    ax
    mov     ax, offset aUnflip; "UNFLIP"
    push    ax
    call    mmgr_alloc_pages
    add     sp, 4
    mov     word ptr [bp+var_10], ax
    mov     word ptr [bp+var_10+2], dx
    push    dx
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    sub_360F6
    add     sp, 8
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    call    mmgr_op_unk2
    add     sp, 4
loc_3ABB7:
    push    [bp+var_2]
    push    [bp+var_4]
    call    sub_3B12A
    add     sp, 4
    mov     [bp+var_7E], ax
    mov     ax, offset aMga ; "!MGA"
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
    call    locate_shape_0
    add     sp, 6
    mov     [bp+var_18], ax
    mov     [bp+var_16], dx
    or      ax, dx
    jz      short loc_3ABF3
    mov     ax, [bp+var_18]
    add     ax, 10h
    push    dx
    push    ax
    push    cs
    call    near ptr sub_3ACB0
    add     sp, 4
loc_3ABF3:
    push    [bp+var_7E]
    lea     ax, [bp+var_7C]
    push    ax
    call    mmgr_alloc_pages
    add     sp, 4
    mov     word ptr [bp+var_10], ax
    mov     word ptr [bp+var_10+2], dx
    les     bx, [bp+var_10]
    mov     ax, [bp+var_7E]
    sub     dx, dx
    mov     cl, 4
loc_3AC12:
    shl     ax, 1
    rcl     dx, 1
    dec     cl
    jz      short loc_3AC60
    jmp     short loc_3AC12
loc_3AC1C:
    mov     ax, offset a_esh; ".ESH"
    push    ax
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _stricmp
    add     sp, 4
    or      ax, ax
    jnz     short loc_3AC50
    push    [bp+arg_2]
    lea     ax, [bp+var_7C]
    push    ax
    call    load_res0_1_alt
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_3AC4C
    jmp     loc_3ABB7
loc_3AC4C:
    jmp     loc_3AA87
    ; align 2
    db 144
loc_3AC50:
    push    [bp+arg_2]
    lea     ax, [bp+var_7C]
    push    ax
    call    load_res0_1_alt
    jmp     loc_3AB44
    ; align 2
    db 144
loc_3AC60:
    mov     es:[bx], ax
    mov     es:[bx+2], dx
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    push    [bp+var_2]
    push    [bp+var_4]
    call    sub_35FA2
    add     sp, 8
    push    [bp+var_2]
    push    [bp+var_4]
    call    mmgr_op_unk2
    add     sp, 4
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    call    mmgr_op_unk
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    mov     ax, offset shapenums
    push    ax
    push    dx
    push    [bp+var_4]
    call    sub_35F48
    add     sp, 6
    jmp     loc_3AA87
load_2dshape endp
sub_3ACB0 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     [bp+var_2], 0
loc_3ACBC:
    mov     bx, [bp+var_2]
    les     si, [bp+arg_0]
    mov     al, es:[bx+si]
    mov     shapenums[bx], al
    inc     [bp+var_2]
    cmp     [bp+var_2], 10h
    jl      short loc_3ACBC
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_3ACB0 endp
seg034 ends
end
