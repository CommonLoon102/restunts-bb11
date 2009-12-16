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
seg029 segment byte public 'STUNTSC' use16
    assume cs:seg029
    assume es:nothing, ss:nothing, ds:dseg
    public sub_39AD4
    public sub_39B5A
    public sub_39BDC
    public sub_39C84
    ; align 4
    db 144
    db 0
    db 0
sub_39AD4 proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = dword ptr 8
    arg_6 = dword ptr 12
    arg_A = word ptr 16

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    cmp     [bp+arg_A], 0
    jz      short loc_39B50
    mov     di, [bp+arg_0]
    mov     si, [bp+arg_A]
loc_39AE8:
    les     bx, [bp+arg_2]
    cmp     byte ptr es:[bx], 0
    jz      short loc_39B4D
    les     bx, [bp+arg_6]
    cmp     byte ptr es:[bx], 0
    jz      short loc_39B4D
    or      di, di
    jz      short loc_39B16
    mov     al, es:[bx]
    les     bx, [bp+arg_2]
    cmp     es:[bx], al
    jz      short loc_39B16
loc_39B09:
    sub     ax, ax
    mov     [bp+arg_A], si
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    db 144
    db 144
loc_39B16:
    or      di, di
    jnz     short loc_39B44
    les     bx, [bp+arg_6]
    mov     al, es:[bx]
    sub     ah, ah
    push    ax
    call    sub_370BA
    add     sp, 2
    les     bx, [bp+arg_2]
    mov     cl, es:[bx]
    sub     ch, ch
    push    cx
    mov     [bp+var_2], ax
    call    sub_370BA
    add     sp, 2
    cmp     ax, [bp+var_2]
    jnz     short loc_39B09
loc_39B44:
    inc     word ptr [bp+arg_2]
    inc     word ptr [bp+arg_6]
    dec     si
    jnz     short loc_39AE8
loc_39B4D:
    mov     [bp+arg_A], si
loc_39B50:
    mov     ax, 1
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_39AD4 endp
sub_39B5A proc far
    var_E = word ptr -14
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = byte ptr -6
    var_2 = byte ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = dword ptr 12

    push    bp
    mov     bp, sp
    sub     sp, 0Eh
    push    di
    push    si
    mov     [bp+var_2], 0
    mov     [bp+var_8], 0
    cmp     [bp+arg_2], 0
    jle     short loc_39BD3
    mov     ax, [bp+arg_4]
    mov     [bp+var_E], ax
    mov     [bp+var_C], ds
    mov     si, [bp+var_8]
loc_39B7D:
    sub     cx, cx
    les     di, [bp+arg_6]
loc_39B82:
    mov     bx, cx
    add     bx, bp
    mov     al, es:[di]
    mov     [bx-6], al
    inc     di
    inc     cx
    cmp     cx, 4
    jl      short loc_39B82
    mov     word ptr [bp+arg_6], di
    mov     word ptr [bp+arg_6+2], es
    mov     [bp+var_A], cx
    mov     ax, 4
    push    ax
    push    [bp+var_C]
    push    [bp+var_E]
    lea     ax, [bp+var_6]
    push    ss
    push    ax
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_39AD4
    add     sp, 0Ch
    or      ax, ax
    jz      short loc_39BC4
    mov     ax, si
    mov     [bp+var_8], si
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
loc_39BC4:
    mov     ax, [bp+arg_0]
    add     word ptr [bp+arg_6], ax
    inc     si
    cmp     si, [bp+arg_2]
    jl      short loc_39B7D
    mov     [bp+var_8], si
loc_39BD3:
    mov     ax, 0FFFFh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_39B5A endp
sub_39BDC proc far
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
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 10h
    sub     ax, ax
    mov     [bp+var_8], ax
    mov     [bp+var_A], ax
    mov     ax, 4
    cwd
    add     ax, [bp+arg_0]
    adc     dx, 0
    mov     cx, 0Ch
    shl     dx, cl
    add     dx, [bp+arg_2]
    mov     es, dx
    mov     bx, ax
    mov     ax, es:[bx]
    mov     [bp+var_2], ax
    mov     ax, 6
    cwd
    add     ax, [bp+arg_0]
    adc     dx, 0
    shl     dx, cl
    add     dx, [bp+arg_2]
    push    dx
    push    ax
    push    [bp+arg_4]
    push    [bp+var_2]
    sub     ax, ax
    push    ax
    push    cs
    call    near ptr sub_39B5A
    add     sp, 0Ah
    mov     [bp+var_10], ax
    or      ax, ax
    jl      short loc_39C7A
    mov     ax, [bp+arg_0]
    mov     dx, [bp+arg_2]
    mov     cx, [bp+var_2]
    shl     cx, 1
    shl     cx, 1
    add     ax, cx
    mov     cx, [bp+var_10]
    shl     cx, 1
    shl     cx, 1
    add     ax, cx
    add     ax, 6
    mov     [bp+var_6], ax
    mov     [bp+var_4], dx
    push    dx
    push    ax
    call    sub_384FA
    add     sp, 4
    mov     [bp+var_E], ax
    mov     [bp+var_C], dx
    mov     ax, [bp+arg_0]
    mov     dx, [bp+arg_2]
    mov     bx, [bp+var_2]
    mov     cl, 3
    shl     bx, cl
    add     ax, bx
    add     ax, [bp+var_E]
    add     ax, 6
    mov     [bp+var_A], ax
    mov     [bp+var_8], dx
loc_39C7A:
    mov     ax, [bp+var_A]
    mov     dx, [bp+var_8]
    mov     sp, bp
    pop     bp
    retf
sub_39BDC endp
sub_39C84 proc far
    var_6 = word ptr -6
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = dword ptr 10
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 6
    push    di
    push    si
    mov     [bp+var_2], 0
    cmp     [bp+arg_8], 0
    jle     short loc_39CC4
    mov     cx, [bp+arg_8]
    mov     ax, cx
    add     [bp+var_2], ax
    les     di, [bp+arg_4]
    mov     [bp+var_6], ds
    lds     si, [bp+arg_0]
loc_39CA8:
    mov     al, [si]
    mov     es:[di], al
    mov     ax, si
    mov     dx, ds
    inc     si
    inc     di
    loop    loc_39CA8
    mov     word ptr [bp+arg_0], si
    mov     word ptr [bp+arg_0+2], ds
    mov     ds, [bp+var_6]
    mov     word ptr [bp+arg_4], di
    mov     word ptr [bp+arg_4+2], es
loc_39CC4:
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_39C84 endp
seg029 ends
end
