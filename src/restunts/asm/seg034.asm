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
    public load_2dshape_fatal
    public load_2dshape_nofatal
    public load_2dshape
    public load_2dshape_helper
algn_3A9D5:
    ; align 2
    db 144
load_2dshape_fatal proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
loc_3A9D7:
    mov     bp, sp
loc_3A9D9:
    mov     ax, 1
loc_3A9DC:
    push    ax
loc_3A9DD:
    push    [bp+arg_0]
loc_3A9E0:
    push    cs
loc_3A9E1:
    call near ptr load_2dshape
loc_3A9E4:
    add     sp, 4
    pop     bp
locret_3A9E8:
    retf
    ; align 2
    db 144
load_2dshape_fatal endp
load_2dshape_nofatal proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
loc_3A9EB:
    mov     bp, sp
loc_3A9ED:
    sub     ax, ax
    push    ax
loc_3A9F0:
    push    [bp+arg_0]
    push    cs
loc_3A9F4:
    call near ptr load_2dshape
loc_3A9F7:
    add     sp, 4
loc_3A9FA:
    pop     bp
    retf
load_2dshape_nofatal endp
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
    arg_fatal = word ptr 8

    push    bp
loc_3A9FD:
    mov     bp, sp
loc_3A9FF:
    sub     sp, 80h
    push    si
loc_3AA04:
    push    [bp+arg_0]
loc_3AA07:
    lea     ax, [bp+var_7C]
loc_3AA0A:
    push    ax              ; char *
loc_3AA0B:
    call    _strcpy
loc_3AA10:
    add     sp, 4
loc_3AA13:
    lea     ax, [bp+var_7C]
loc_3AA16:
    mov     word ptr [bp+var_C], ax
loc_3AA19:
    jmp     short loc_3AA25
    ; align 2
    db 144
loc_3AA1C:
    cmp     [bp+var_80], 2Eh ; '.'
    jz      short loc_3AA31
loc_3AA22:
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
loc_3AA39:
    mov     [bp+var_14], 0
    jmp     short loc_3AA53
loc_3AA40:
    lea     ax, [bp+var_7C]
    push    ax
loc_3AA44:
    call    file_find
    add     sp, 2
    or      ax, ax
    jnz     short loc_3AAA8
    inc     [bp+var_14]
loc_3AA53:
    mov     bx, [bp+var_14]
    shl     bx, 1
    mov     si, shapeexts[bx]
loc_3AA5C:
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
    call    mmgr_get_chunk_by_name
    add     sp, 2
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_3AA83:
    or      dx, ax
    jz      short loc_3AA40
loc_3AA87:
    mov     ax, [bp+var_4]
loc_3AA8A:
    mov     dx, [bp+var_2]
loc_3AA8D:
    pop     si
loc_3AA8E:
    mov     sp, bp
loc_3AA90:
    pop     bp
locret_3AA91:
    retf
loc_3AA92:
    lea     ax, [bp+var_7C]
loc_3AA95:
    push    ax
loc_3AA96:
    call    mmgr_get_chunk_by_name
loc_3AA9B:
    add     sp, 2
loc_3AA9E:
    mov     [bp+var_4], ax
loc_3AAA1:
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
loc_3AABA:
    push    ax
    lea     ax, [bp+var_C+2]
    push    ax              ; char *
    call    _stricmp
    add     sp, 4
    or      ax, ax
    jnz     short loc_3AB24
    push    [bp+arg_fatal]
    lea     ax, [bp+var_7C]
    push    ax
    call    file_decomp
    add     sp, 4
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
    or      ax, dx
    jz      short loc_3AA87
    push    dx
    push    [bp+var_4]
loc_3AAE8:
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
loc_3AB08:
    push    [bp+var_4]
loc_3AB0B:
    call    sub_32AE2
    add     sp, 8
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    call    mmgr_release
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
    push    [bp+arg_fatal]
    lea     ax, [bp+var_7C]
    push    ax
    call    file_decomp
loc_3AB44:
    add     sp, 4
loc_3AB47:
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
    push    [bp+arg_fatal]
    lea     ax, [bp+var_7C]
    push    ax
    call    file_decomp
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
loc_3AB8B:
    call    mmgr_alloc_pages
loc_3AB90:
    add     sp, 4
    mov     word ptr [bp+var_10], ax
    mov     word ptr [bp+var_10+2], dx
    push    dx
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
loc_3ABA1:
    call    sub_360F6
    add     sp, 8
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
    call    mmgr_release
loc_3ABB4:
    add     sp, 4
loc_3ABB7:
    push    [bp+var_2]
    push    [bp+var_4]
    call    sub_3B12A
loc_3ABC2:
    add     sp, 4
    mov     [bp+var_7E], ax
    mov     ax, offset aMga ; "!MGA"
    push    ax
    push    [bp+var_2]
    push    [bp+var_4]
loc_3ABD2:
    call    locate_shape_nofatal
    add     sp, 6
    mov     [bp+var_18], ax
loc_3ABDD:
    mov     [bp+var_16], dx
    or      ax, dx
    jz      short loc_3ABF3
    mov     ax, [bp+var_18]
    add     ax, 10h
    push    dx
    push    ax
    push    cs
    call near ptr load_2dshape_helper
loc_3ABF0:
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
    push    [bp+arg_fatal]
    lea     ax, [bp+var_7C]
    push    ax
    call    file_load_binary
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
    push    [bp+arg_fatal]
    lea     ax, [bp+var_7C]
    push    ax
    call    file_load_binary
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
loc_3AC81:
    call    mmgr_release
    add     sp, 4
    push    word ptr [bp+var_10+2]
    push    word ptr [bp+var_10]
loc_3AC8F:
    call    mmgr_op_unk
loc_3AC94:
    add     sp, 4
loc_3AC97:
    mov     [bp+var_4], ax
    mov     [bp+var_2], dx
loc_3AC9D:
    mov     ax, offset shapenums
    push    ax
    push    dx
loc_3ACA2:
    push    [bp+var_4]
loc_3ACA5:
    call    sub_35F48
loc_3ACAA:
    add     sp, 6
    jmp     loc_3AA87
load_2dshape endp
load_2dshape_helper proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6

    push    bp
loc_3ACB1:
    mov     bp, sp
    sub     sp, 2
    push    si
loc_3ACB7:
    mov     [bp+var_2], 0
loc_3ACBC:
    mov     bx, [bp+var_2]
loc_3ACBF:
    les     si, [bp+arg_0]
    mov     al, es:[bx+si]
loc_3ACC5:
    mov     shapenums[bx], al
loc_3ACC9:
    inc     [bp+var_2]
loc_3ACCC:
    cmp     [bp+var_2], 10h
loc_3ACD0:
    jl      short loc_3ACBC
loc_3ACD2:
    pop     si
loc_3ACD3:
    mov     sp, bp
loc_3ACD5:
    pop     bp
locret_3ACD6:
    retf
load_2dshape_helper endp
seg034 ends
end
