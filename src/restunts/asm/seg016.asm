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
seg016 segment byte public 'STUNTSC' use16
    assume cs:seg016
    assume es:nothing, ss:nothing, ds:dseg
    public sub_367B2
    ; align 2
    db 144
sub_367B2 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12

    push    bp
    mov     bp, sp
    jmp     short loc_367D9
    ; align 2
    db 144
loc_367B8:
    push    [bp+arg_4]
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_30F9D
    add     sp, 6
    mov     bx, [bp+arg_6]
    add     [bp+arg_6], 4
    mov     [bx], ax
    mov     [bx+2], dx
    add     [bp+arg_4], 4
loc_367D9:
    mov     bx, [bp+arg_4]
    cmp     byte ptr [bx], 0
    jnz     short loc_367B8
    pop     bp
    retf
    ; align 2
    db 144
    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     word ptr [bp-2], 0
    jmp     short loc_36819
loc_367F2:
    push    word ptr [bp+0Ah]
    push    word ptr [bp+8]
    push    word ptr [bp+6]
    call    sub_30F92
    add     sp, 6
    mov     bx, [bp-2]
    inc     word ptr [bp-2]
    shl     bx, 1
    shl     bx, 1
    mov     si, [bp+0Ch]
    mov     [bx+si], ax
    mov     [bx+si+2], dx
    add     word ptr [bp+0Ah], 4
loc_36819:
    mov     bx, [bp+0Ah]
    cmp     byte ptr [bx], 0
    jnz     short loc_367F2
    pop     si
    mov     sp, bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     word ptr [bp-2], 0
    jmp     short loc_3685B
loc_36834:
    push    word ptr [bp+0Ah]
    push    word ptr [bp+8]
    push    word ptr [bp+6]
    call    sub_30FA9
    add     sp, 6
    mov     bx, [bp-2]
    inc     word ptr [bp-2]
    shl     bx, 1
    shl     bx, 1
    mov     si, [bp+0Ch]
    mov     [bx+si], ax
    mov     [bx+si+2], dx
    add     word ptr [bp+0Ah], 4
loc_3685B:
    mov     bx, [bp+0Ah]
    cmp     byte ptr [bx], 0
    jnz     short loc_36834
    pop     si
    mov     sp, bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    sub     sp, 2
    push    si
    mov     word ptr [bp-2], 0
    jmp     short loc_3689D
loc_36876:
    push    word ptr [bp+0Ah]
    push    word ptr [bp+8]
    push    word ptr [bp+6]
    call    sub_30F92
    add     sp, 6
    mov     bx, [bp-2]
    inc     word ptr [bp-2]
    shl     bx, 1
    shl     bx, 1
    mov     si, [bp+0Ch]
    mov     [bx+si], ax
    mov     [bx+si+2], dx
    add     word ptr [bp+0Ah], 4
loc_3689D:
    mov     bx, [bp+0Ah]
    cmp     byte ptr [bx], 0
    jnz     short loc_36876
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_367B2 endp
seg016 ends
end
