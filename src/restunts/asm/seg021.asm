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
seg021 segment byte public 'STUNTSC' use16
    assume cs:seg021
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36BE8
sub_36BE8 proc far
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
loc_36BE9:
    mov     bp, sp
loc_36BEB:
    sub     sp, 8
loc_36BEE:
    push    di
    push    si
loc_36BF0:
    mov     ax, [bp+arg_0]
    cwd
loc_36BF4:
    sub     ax, dx
loc_36BF6:
    sar     ax, 1
loc_36BF8:
    jmp     short loc_36C6B
loc_36BFA:
    mov     si, [bp+var_8]
loc_36BFD:
    shl     si, 1
loc_36BFF:
    mov     bx, [bp+arg_2]
loc_36C02:
    mov     ax, [bx+si]
loc_36C04:
    mov     [bp+var_2], ax
loc_36C07:
    mov     di, [bp+var_8]
loc_36C0A:
    add     di, [bp+var_4]
loc_36C0D:
    shl     di, 1
loc_36C0F:
    mov     ax, [bx+di]
loc_36C11:
    mov     [bx+si], ax
loc_36C13:
    mov     ax, [bp+var_2]
loc_36C16:
    mov     [bx+di], ax
loc_36C18:
    mov     bx, [bp+arg_4]
loc_36C1B:
    mov     ax, [bx+si]
loc_36C1D:
    mov     [bp+var_2], ax
loc_36C20:
    mov     ax, [bx+di]
loc_36C22:
    mov     [bx+si], ax
loc_36C24:
    mov     ax, [bp+var_2]
loc_36C27:
    mov     [bx+di], ax
loc_36C29:
    mov     ax, [bp+var_4]
loc_36C2C:
    sub     [bp+var_8], ax
loc_36C2F:
    cmp     [bp+var_8], 0
loc_36C33:
    jl      short loc_36C4B
loc_36C35:
    mov     bx, [bp+var_8]
loc_36C38:
    shl     bx, 1
    mov     si, [bp+arg_2]
loc_36C3D:
    mov     ax, [bx+si]
    mov     bx, [bp+var_8]
loc_36C42:
    add     bx, [bp+var_4]
    shl     bx, 1
loc_36C47:
    cmp     [bx+si], ax
loc_36C49:
    jg      short loc_36BFA
loc_36C4B:
    inc     [bp+var_6]
loc_36C4E:
    mov     ax, [bp+arg_0]
loc_36C51:
    cmp     [bp+var_6], ax
loc_36C54:
    jge     short loc_36C62
loc_36C56:
    mov     ax, [bp+var_6]
loc_36C59:
    sub     ax, [bp+var_4]
loc_36C5C:
    mov     [bp+var_8], ax
loc_36C5F:
    jmp     short loc_36C2F
    ; align 2
    db 144
loc_36C62:
    mov     cx, 2
loc_36C65:
    mov     ax, [bp+var_4]
loc_36C68:
    cwd
loc_36C69:
    idiv    cx
loc_36C6B:
    mov     [bp+var_4], ax
loc_36C6E:
    or      ax, ax
loc_36C70:
    jle     short loc_36C78
loc_36C72:
    mov     [bp+var_6], ax
loc_36C75:
    jmp     short loc_36C4E
    ; align 2
    db 144
loc_36C78:
    pop     si
loc_36C79:
    pop     di
loc_36C7A:
    mov     sp, bp
loc_36C7C:
    pop     bp
locret_36C7D:
    retf
sub_36BE8 endp
seg021 ends
end
