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
    sub     sp, 8
    push    di
    push    si
    mov     ax, [bp+arg_0]
    cwd
    sub     ax, dx
    sar     ax, 1
    jmp     short loc_36C6B
loc_36BFA:
    mov     si, [bp+var_8]
    shl     si, 1
    mov     bx, [bp+arg_2]
    mov     ax, [bx+si]
    mov     [bp+var_2], ax
    mov     di, [bp+var_8]
    add     di, [bp+var_4]
    shl     di, 1
    mov     ax, [bx+di]
    mov     [bx+si], ax
    mov     ax, [bp+var_2]
    mov     [bx+di], ax
    mov     bx, [bp+arg_4]
    mov     ax, [bx+si]
    mov     [bp+var_2], ax
    mov     ax, [bx+di]
    mov     [bx+si], ax
    mov     ax, [bp+var_2]
    mov     [bx+di], ax
    mov     ax, [bp+var_4]
    sub     [bp+var_8], ax
loc_36C2F:
    cmp     [bp+var_8], 0
    jl      short loc_36C4B
    mov     bx, [bp+var_8]
    shl     bx, 1
    mov     si, [bp+arg_2]
    mov     ax, [bx+si]
    mov     bx, [bp+var_8]
    add     bx, [bp+var_4]
    shl     bx, 1
    cmp     [bx+si], ax
    jg      short loc_36BFA
loc_36C4B:
    inc     [bp+var_6]
loc_36C4E:
    mov     ax, [bp+arg_0]
    cmp     [bp+var_6], ax
    jge     short loc_36C62
    mov     ax, [bp+var_6]
    sub     ax, [bp+var_4]
    mov     [bp+var_8], ax
    jmp     short loc_36C2F
    ; align 2
    db 144
loc_36C62:
    mov     cx, 2
    mov     ax, [bp+var_4]
loc_36C68:
    cwd
    idiv    cx
loc_36C6B:
    mov     [bp+var_4], ax
loc_36C6E:
    or      ax, ax
loc_36C70:
    jle     short loc_36C78
    mov     [bp+var_6], ax
loc_36C75:
    jmp     short loc_36C4E
    ; align 2
    db 144
loc_36C78:
    pop     si
    pop     di
loc_36C7A:
    mov     sp, bp
    pop     bp
    retf
sub_36BE8 endp
seg021 ends
end
