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
seg019 segment byte public 'STUNTSC' use16
    assume cs:seg019
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36B16
sub_36B16 proc far
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
    sub     sp, 0Ch
    push    si
    mov     bx, [bp+arg_0]
    mov     ax, [bx]
    mov     dx, [bx+2]
    mov     [bp+var_C], ax
    mov     [bp+var_A], dx
    mov     si, ax
    push    [bp+arg_4]
    mov     ax, [bx+4]
    sub     ax, si
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, si
    mov     [bp+var_8], ax
    push    [bp+arg_4]
    mov     bx, [bp+arg_0]
    mov     ax, [bx+6]
    sub     ax, [bx+2]
    push    ax
    call    sub_30044
    add     sp, 4
    mov     bx, [bp+arg_0]
    mov     cx, [bx+2]
    add     cx, ax
    mov     [bp+var_6], cx
    push    [bp+arg_4]
    mov     ax, [bx+8]
    sub     ax, si
    push    ax
    call    sub_30044
    add     sp, 4
    add     ax, si
    mov     [bp+var_4], ax
    push    [bp+arg_4]
    mov     bx, [bp+arg_0]
    mov     ax, [bx+0Ah]
    sub     ax, [bx+2]
    push    ax
    call    sub_30044
    add     sp, 4
    mov     bx, [bp+arg_0]
    mov     cx, [bx+2]
    add     cx, ax
    mov     [bp+var_2], cx
    push    [bp+arg_2]
    push    bx
    call    sub_36246
    add     sp, 4
    mov     ax, [bp+arg_2]
    add     ax, 40h ; '@'
    push    ax
    lea     ax, [bp+var_C]
    push    ax
    call    sub_36246
    add     sp, 4
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_36B16 endp
seg019 ends
end
