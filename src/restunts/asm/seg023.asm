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
seg023 segment byte public 'STUNTSC' use16
    assume cs:seg023
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36EC0
sub_36EC0 proc far
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
    sub     sp, 0Ah
loc_36EC6:
    push    si
    push    [bp+arg_4]
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_36B16
    add     sp, 6
    mov     bx, [bp+arg_0]
    mov     ax, [bx+0Ch]
    sub     ax, [bx]
    mov     [bp+var_8], ax
    mov     ax, [bx+0Eh]
    sub     ax, [bx+2]
    mov     [bp+var_A], ax
    mov     ax, [bp+arg_2]
    mov     [bp+var_2], ax
    add     ax, 80h ; '€'
    mov     [bp+var_4], ax
    mov     [bp+var_6], 0
loc_36EFD:
    mov     bx, [bp+var_4]
    mov     si, [bp+var_2]
    mov     ax, [si]
    add     ax, [bp+var_8]
    mov     [bx], ax
    mov     ax, [si+2]
    add     ax, [bp+var_A]
    mov     [bx+2], ax
loc_36F13:
    add     [bp+var_2], 4
    add     [bp+var_4], 4
    inc     [bp+var_6]
loc_36F1E:
    cmp     [bp+var_6], 10h
    jl      short loc_36EFD
    pop     si
loc_36F25:
    mov     sp, bp
    pop     bp
    retf
sub_36EC0 endp
seg023 ends
end
