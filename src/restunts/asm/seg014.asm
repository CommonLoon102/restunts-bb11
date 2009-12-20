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
    include seg034.inc
    include seg035.inc
    include seg036.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg014 segment byte public 'STUNTSC' use16
    assume cs:seg014
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36246
    ; align 2
    db 144
sub_36246 proc far
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
    mov     bp, sp
    sub     sp, 0Ah
    push    di
    push    si
    mov     bx, [bp+arg_0]
    mov     si, [bx]
    mov     ax, [bx+4]
    sub     ax, si
    mov     bx, [bp+arg_2]
    mov     [bx], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+6]
    sub     ax, [bx+2]
    mov     bx, [bp+arg_2]
    mov     [bx+2], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+8]
    sub     ax, si
    mov     bx, [bp+arg_2]
    mov     [bx+10h], ax
    mov     bx, [bp+arg_0]
    mov     ax, [bx+0Ah]
    sub     ax, [bx+2]
    mov     bx, [bp+arg_2]
    mov     [bx+12h], ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+10h]
    add     ax, [bx]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+8], ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+2]
    add     ax, [bx+12h]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Ah], ax
    mov     si, [bx+10h]
    sar     si, 1
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx]
    add     ax, si
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+4], ax
    mov     di, [bx+12h]
    sar     di, 1
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+2]
    add     ax, di
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+6], ax
    mov     ax, [bx]
    sar     ax, 1
    mov     [bp+var_4], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+10h]
    add     ax, [bp+var_4]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Ch], ax
    mov     ax, [bx+2]
    sar     ax, 1
    mov     [bp+var_6], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+12h]
    add     ax, [bp+var_6]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Eh], ax
    mov     ax, [bx]
    neg     ax
    mov     [bp+var_8], ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+10h]
    add     ax, [bp+var_8]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+18h], ax
    mov     ax, [bx+2]
    neg     ax
    mov     [bp+var_A], ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+12h]
    add     ax, [bp+var_A]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+1Ah], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bp+var_8]
    add     ax, si
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+1Ch], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bp+var_A]
    add     ax, di
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+1Eh], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+10h]
    sub     ax, [bp+var_4]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+14h], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+12h]
    sub     ax, [bp+var_6]
    push    ax
    call    scale_value
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+16h], ax
    mov     [bp+var_2], 0
loc_363DC:
    mov     si, [bp+var_2]
    mov     cl, 2
    shl     si, cl
    add     si, [bp+arg_2]
    mov     bx, [bp+arg_0]
    mov     di, [bx]
    mov     ax, di
    sub     ax, [si]
    mov     [si+20h], ax
    mov     ax, [bx+2]
    sub     ax, [si+2]
    mov     [si+22h], ax
    add     [si], di
    mov     ax, [bx+2]
    add     [si+2], ax
    inc     [bp+var_2]
    cmp     [bp+var_2], 8
    jl      short loc_363DC
    pop     si
    pop     di
    mov     sp, bp
    pop     bp
    retf
sub_36246 endp
seg014 ends
end
