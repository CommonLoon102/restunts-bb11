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
seg017 segment byte public 'STUNTSC' use16
    assume cs:seg017
    assume es:nothing, ss:nothing, ds:dseg
    public mouse_set_pixratio
    public mouse_init
    public mouse_set_minmax
    public mouse_set_position
    public mouse_get_state
    public nopsub_36A9A
    public nopsub_36ACA
mouse_set_pixratio proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     regs_x86, 0Fh
    mov     ax, [bp+arg_0]
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_2]
    mov     regs_x86+6, ax
    mov     ax, offset regs_x86
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33,f = set mouse mickey pixel ratio
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
mouse_set_pixratio endp
mouse_init proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 2
    mov     regs_x86, 0C201h
    mov     ax, offset regs_x86
    push    ax
    push    ax              ; union REGS *
    mov     ax, 15h         ; int 15,c201 = enable ps2 pointing device
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     regs_x86, 0
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33,0 = mouse reset/get mouse installed
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     ax, regs_x86
    mov     [bp+var_2], ax
    mov     ax, regs_x86+2
    mov     word_45D7C, ax
    cmp     [bp+var_2], 0
    jz      short loc_36955
    cmp     [bp+arg_0], 140h
    jnz     short loc_36928
    mov     mousehorscale, 1
    jmp     short loc_3692E
    ; align 2
    db 144
loc_36928:
    mov     mousehorscale, 0
loc_3692E:
    mov     ax, [bp+arg_2]
    dec     ax
    push    ax
    mov     ax, [bp+arg_0]
    dec     ax
    push    ax
    sub     ax, ax
    push    ax
    push    ax
    push    cs
    call    near ptr mouse_set_minmax
    add     sp, 8
    mov     ax, 10h
    push    ax
    push    ax
    push    cs
    call    near ptr mouse_set_pixratio
    add     sp, 4
    mov     word_40318, 0FFFFh
loc_36955:
    mov     ax, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
mouse_init endp
mouse_set_minmax proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    mov     regs_x86, 7
    mov     ax, [bp+arg_0]
    mov     cl, byte ptr mousehorscale
    shl     ax, cl
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_4]
    shl     ax, cl
    mov     regs_x86+6, ax
    mov     ax, offset regs_x86
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33,0 = mouse reset/get mouse installed
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     regs_x86, 8
    mov     ax, [bp+arg_2]
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_6]
    mov     regs_x86+6, ax
    mov     ax, offset regs_x86
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33, 8 = set mouse vert min/max
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
    ; align 2
    db 144
    mov     regs_x86, 3
    mov     ax, offset regs_x86
    push    ax
    push    ax
    mov     ax, 33h         ; int 33,3 = get mouse pos/but status
    push    ax
    call    _int86
    add     sp, 6
    mov     ax, regs_x86+2
    mov     word_45D7C, ax
    mov     ax, regs_x86+4
    mov     cl, byte ptr mousehorscale
    shr     ax, cl
    mov     word_44D3C, ax
    mov     ax, regs_x86+6
    mov     word_44D62, ax
    mov     ax, word_45D7C
    retf
    ; align 2
    db 144
    inc     showmouse
    cmp     showmouse, 1
    jl      short locret_36A0C
    mov     showmouse, 1
    mov     regs_x86, 1
    mov     ax, offset regs_x86
    push    ax
    push    ax
    mov     ax, 33h         ; int 33,1 = show mouse cursor
    push    ax
    call    _int86
    add     sp, 6
locret_36A0C:
    retf
    ; align 2
    db 144
    dec     showmouse
    jnz     short locret_36A2B
    mov     regs_x86, 2
    mov     ax, offset regs_x86
    push    ax
    push    ax
    mov     ax, 33h         ; int 33,2 = hide mouse cursor
    push    ax
    call    _int86
    add     sp, 6
locret_36A2B:
    retf
mouse_set_minmax endp
mouse_set_position proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     regs_x86, 4
    mov     ax, [bp+arg_0]
    mov     word_44D3C, ax
    mov     cl, byte ptr mousehorscale
    shl     ax, cl
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_2]
    mov     regs_x86+6, ax
    mov     word_44D62, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33, 4 = set mouse cursor
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
mouse_set_position endp
mouse_get_state proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    mov     regs_x86, 3
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h         ; int 33, 3 = get mouse position and button
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     bx, [bp+arg_0]
    mov     ax, regs_x86+2
    mov     [bx], ax
    mov     bx, [bp+arg_2]
    mov     ax, regs_x86+4
    mov     cl, byte ptr mousehorscale
    shr     ax, cl
    mov     [bx], ax
    mov     bx, [bp+arg_4]
    mov     ax, regs_x86+6
    mov     [bx], ax
    pop     bp
    retf
mouse_get_state endp
nopsub_36A9A proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     regs_x86, 7
    mov     ax, [bp+arg_0]
    mov     cl, byte ptr mousehorscale
    sar     ax, cl
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_2]
    sar     ax, cl
    mov     regs_x86+6, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
nopsub_36A9A endp
nopsub_36ACA proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     regs_x86, 8
    mov     ax, [bp+arg_0]
    mov     regs_x86+4, ax
    mov     ax, [bp+arg_2]
    mov     regs_x86+6, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
nopsub_36ACA endp
seg017 ends
end
