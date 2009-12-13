.model large
nosmart
    include structs.inc
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
seg030 segment byte public 'CODE' use16
    assume cs:seg030
    assume es:nothing, ss:nothing, ds:dseg
    public sub_39CCE
    db 144
    db 144
    db 0
    db 0
sub_39CCE proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10

    push    bp
    mov     bp, sp
    sub     sp, 2
    push    di
    push    si
    mov     si, 75BEh
    push    ds
    pop     es
    mov     di, word ptr [bp+arg_0]
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    sub     di, cx
    xchg    di, si
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
    mov     ax, 5Ch ; '\'
    push    ax
    mov     ax, 75BEh
    push    ax              ; char *
    call    _strrchr
    add     sp, 4
    mov     [bp+var_2], ax
    or      ax, ax
    jz      short loc_39D16
    inc     [bp+var_2]
    mov     bx, [bp+var_2]
    mov     byte ptr [bx], 0
    jmp     short loc_39D1B
    ; align 2
    db 144
loc_39D16:
    mov     byte_42D2E, 0
loc_39D1B:
    mov     si, 75BEh
    push    ds
    pop     es
    mov     di, [bp+arg_4]
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    sub     di, cx
    mov     bx, cx
    xchg    di, si
    mov     cx, 0FFFFh
    repne scasb
    dec     di
    mov     cx, bx
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
    mov     ax, 5Ch ; '\'
    push    ax
    push    word ptr [bp+arg_0]; char *
    call    _strrchr
    add     sp, 4
    mov     [bp+var_2], ax
    or      ax, ax
    jz      short loc_39D6A
    inc     [bp+var_2]
    mov     bx, [bp+var_2]
    mov     di, bx
    mov     si, 75BEh
    mov     ax, ds
    mov     es, ax
    jmp     short loc_39D72
    ; align 2
    db 144
loc_39D6A:
    mov     si, 75BEh
    push    ds
    pop     es
    mov     di, word ptr [bp+arg_0]
loc_39D72:
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    sub     di, cx
    mov     bx, cx
    xchg    di, si
    mov     cx, 0FFFFh
    repne scasb
    dec     di
    mov     cx, bx
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
    mov     di, 75BEh
    mov     ax, ds
    mov     es, ax
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    dec     cx
    mov     bx, cx
    cmp     byte ptr [bx+75BAh], 2Eh ; '.'
    jnz     short loc_39DBF
    mov     di, 75BEh
    mov     ax, ds
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    dec     cx
    cmp     cx, 4
    ja      short loc_39E0B
loc_39DBF:
    mov     di, 503Ch
    mov     si, 75BEh
    mov     ax, ds
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    sub     di, cx
    mov     bx, cx
    xchg    di, si
    mov     cx, 0FFFFh
    repne scasb
    dec     di
    mov     cx, bx
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
    mov     si, 75BEh
    mov     di, word ptr [bp+arg_0+2]
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    sub     di, cx
    mov     bx, cx
    xchg    di, si
    mov     cx, 0FFFFh
    repne scasb
    dec     di
    mov     cx, bx
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
loc_39E0B:
    mov     ax, 75BEh
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_39CCE endp
seg030 ends
end
