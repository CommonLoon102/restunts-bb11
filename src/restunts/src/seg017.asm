.model large
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
seg017 segment byte public 'CODE' use16
    assume cs:seg017
    assume es:nothing, ss:nothing, ds:dseg
    public sub_368AA
    public sub_368D2
    public sub_3695C
    public sub_36A2C
    public sub_36A60
sub_368AA proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     word_4498A, 0Fh
    mov     ax, [bp+arg_0]
    mov     word_4498E, ax
    mov     ax, [bp+arg_2]
    mov     word_44990, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
sub_368AA endp
sub_368D2 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 2
    mov     word_4498A, 0C201h
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 15h
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     word_4498A, 0
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     ax, word_4498A
    mov     [bp+var_2], ax
    mov     ax, word_4498C
    mov     word_45D7C, ax
    cmp     [bp+var_2], 0
    jz      short loc_36955
    cmp     [bp+arg_0], 140h
    jnz     short loc_36928
    mov     word_4031A, 1
    jmp     short loc_3692E
    ; align 2
    db 144
loc_36928:
    mov     word_4031A, 0
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
    call    near ptr sub_3695C
    add     sp, 8
    mov     ax, 10h
    push    ax
    push    ax
    push    cs
    call    near ptr sub_368AA
    add     sp, 4
    mov     word_40318, 0FFFFh
loc_36955:
    mov     ax, [bp+var_2]
    mov     sp, bp
    pop     bp
    retf
sub_368D2 endp
sub_3695C proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    mov     word_4498A, 7
    mov     ax, [bp+arg_0]
    mov     cl, byte ptr word_4031A
    shl     ax, cl
    mov     word_4498E, ax
    mov     ax, [bp+arg_4]
    shl     ax, cl
    mov     word_44990, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     word_4498A, 8
    mov     ax, [bp+arg_2]
    mov     word_4498E, ax
    mov     ax, [bp+arg_6]
    mov     word_44990, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
    ; align 2
    db 144
    mov     word_4498A, 3
    mov     ax, 921Ah
    push    ax
    push    ax
    mov     ax, 33h ; '3'
    push    ax
    call    _int86
    add     sp, 6
    mov     ax, word_4498C
    mov     word_45D7C, ax
    mov     ax, word_4498E
    mov     cl, byte ptr word_4031A
    shr     ax, cl
    mov     word_44D3C, ax
    mov     ax, word_44990
    mov     word_44D62, ax
    mov     ax, word_45D7C
    retf
    ; align 2
    db 144
    inc     word_4031C
    cmp     word_4031C, 1
    jl      short locret_36A0C
    mov     word_4031C, 1
    mov     word_4498A, 1
    mov     ax, 921Ah
    push    ax
    push    ax
    mov     ax, 33h ; '3'
    push    ax
    call    _int86
    add     sp, 6
locret_36A0C:
    retf
    ; align 2
    db 144
    dec     word_4031C
    jnz     short locret_36A2B
    mov     word_4498A, 2
    mov     ax, 921Ah
    push    ax
    push    ax
    mov     ax, 33h ; '3'
    push    ax
    call    _int86
    add     sp, 6
locret_36A2B:
    retf
sub_3695C endp
sub_36A2C proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    mov     word_4498A, 4
    mov     ax, [bp+arg_0]
    mov     word_44D3C, ax
    mov     cl, byte ptr word_4031A
    shl     ax, cl
    mov     word_4498E, ax
    mov     ax, [bp+arg_2]
    mov     word_44990, ax
    mov     word_44D62, ax
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    pop     bp
    retf
sub_36A2C endp
sub_36A60 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    mov     word_4498A, 3
    mov     ax, 921Ah
    push    ax
    push    ax              ; union REGS *
    mov     ax, 33h ; '3'
    push    ax              ; int
    call    _int86
    add     sp, 6
    mov     bx, [bp+arg_0]
    mov     ax, word_4498C
    mov     [bx], ax
    mov     bx, [bp+arg_2]
    mov     ax, word_4498E
    mov     cl, byte ptr word_4031A
    shr     ax, cl
    mov     [bx], ax
    mov     bx, [bp+arg_4]
    mov     ax, word_44990
    mov     [bx], ax
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     word_4498A, 7
    mov     ax, [bp+6]
    mov     cl, byte ptr word_4031A
    sar     ax, cl
    mov     word_4498E, ax
    mov     ax, [bp+8]
    sar     ax, cl
    mov     word_44990, ax
    mov     ax, 921Ah
    push    ax
    push    ax
    mov     ax, 33h ; '3'
    push    ax
    call    _int86
    add     sp, 6
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     word_4498A, 8
    mov     ax, [bp+6]
    mov     word_4498E, ax
    mov     ax, [bp+8]
    mov     word_44990, ax
    mov     ax, 921Ah
    push    ax
    push    ax
    mov     ax, 33h ; '3'
    push    ax
    call    _int86
    add     sp, 6
    pop     bp
    retf
sub_36A60 endp
seg017 ends
end
