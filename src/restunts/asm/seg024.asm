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
seg024 segment byte public 'STUNTSC' use16
    assume cs:seg024
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36F2A
    public sub_36F80
    public sub_36FD8
    ; align 2
    db 144
sub_36F2A proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    [bp+arg_2]
    call    cos_fast
    add     sp, 2
    mov     [bp+var_2], ax
    push    [bp+arg_2]
    call    sin_fast
    mov     [bp+var_4], ax
    mov     bx, [bp+arg_0]
    mov     word ptr [bx], 4000h
    mov     word ptr [bx+2], 0
    mov     word ptr [bx+4], 0
    mov     word ptr [bx+6], 0
    mov     ax, [bp+var_2]
    mov     [bx+8], ax
    mov     ax, [bp+var_4]
    mov     [bx+0Ah], ax
    mov     word ptr [bx+0Ch], 0
    neg     ax
    mov     [bx+0Eh], ax
    mov     ax, [bp+var_2]
    mov     [bx+10h], ax
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
sub_36F2A endp
sub_36F80 proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    [bp+arg_2]
    call    cos_fast
    add     sp, 2
    mov     [bp+var_2], ax
    push    [bp+arg_2]
    call    sin_fast
    mov     [bp+var_4], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_2]
    mov     [bx], ax
    mov     word ptr [bx+2], 0
    mov     ax, [bp+var_4]
    neg     ax
    mov     [bx+4], ax
    mov     word ptr [bx+6], 0
    mov     word ptr [bx+8], 4000h
    mov     word ptr [bx+0Ah], 0
    mov     ax, [bp+var_4]
    mov     [bx+0Ch], ax
    mov     word ptr [bx+0Eh], 0
    mov     ax, [bp+var_2]
    mov     [bx+10h], ax
    mov     sp, bp
    pop     bp
    retf
sub_36F80 endp
sub_36FD8 proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 4
    push    [bp+arg_2]
    call    cos_fast
    add     sp, 2
    mov     [bp+var_2], ax
    push    [bp+arg_2]
    call    sin_fast
    mov     [bp+var_4], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bp+var_2]
    mov     [bx], ax
    mov     ax, [bp+var_4]
    mov     [bx+2], ax
    mov     word ptr [bx+4], 0
    neg     ax
    mov     [bx+6], ax
    mov     ax, [bp+var_2]
    mov     [bx+8], ax
    mov     word ptr [bx+0Ah], 0
    mov     word ptr [bx+0Ch], 0
    mov     word ptr [bx+0Eh], 0
    mov     word ptr [bx+10h], 4000h
    mov     sp, bp
    pop     bp
    retf
sub_36FD8 endp
seg024 ends
end
