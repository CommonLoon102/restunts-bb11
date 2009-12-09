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
seg013 segment byte public 'CODE' use16
    assume cs:seg013
    assume es:nothing, ss:nothing, ds:dseg
    public sub_361BC
sub_361BC proc far
    var_4 = word ptr -4
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    sub     sp, 4
    mov     ax, [bp+arg_4]
    sub     ax, [bp+arg_0]
    inc     ax
    mov     [bp+var_2], ax
    mov     ax, [bp+arg_6]
    sub     ax, [bp+arg_2]
    mov     [bp+var_4], ax
    cmp     [bp+var_2], 0
    jle     short loc_3620B
    push    [bp+arg_8]
    mov     ax, 1
    push    ax
    push    [bp+var_2]
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_33578
    add     sp, 0Ah
    push    [bp+arg_8]
    mov     ax, 1
    push    ax
    push    [bp+var_2]
    push    [bp+arg_6]
    push    [bp+arg_0]
    call    sub_33578
    add     sp, 0Ah
loc_3620B:
    cmp     [bp+var_4], 0
    jle     short loc_36241
    push    [bp+arg_8]
    push    [bp+var_4]
    mov     ax, 1
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_33578
    add     sp, 0Ah
    push    [bp+arg_8]
    push    [bp+var_4]
    mov     ax, 1
    push    ax
    push    [bp+arg_2]
    push    [bp+arg_4]
    call    sub_33578
    add     sp, 0Ah
loc_36241:
    mov     sp, bp
    pop     bp
    retf
sub_361BC endp
seg013 ends
end
