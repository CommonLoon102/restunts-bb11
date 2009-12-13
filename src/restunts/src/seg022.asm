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
seg022 segment byte public 'CODE' use16
    assume cs:seg022
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36C7E
sub_36C7E proc far
    var_11A = word ptr -282
    var_118 = word ptr -280
    var_116 = word ptr -278
    var_114 = word ptr -276
    var_112 = word ptr -274
    var_110 = word ptr -272
    var_10E = word ptr -270
    var_10C = word ptr -268
    var_10A = byte ptr -266
    var_D2 = byte ptr -210
    var_CE = byte ptr -206
    var_92 = byte ptr -146
    var_8E = word ptr -142
    var_8C = word ptr -140
    var_4E = byte ptr -78
    var_A = byte ptr -10
    var_6 = word ptr -6
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
    sub     sp, 11Ah
    push    si
    push    [bp+arg_2]
    lea     ax, [bp+var_10E]
    push    ax
    push    [bp+arg_0]
    call    sub_36EC0
    add     sp, 6
    lea     ax, [bp+var_10E]
    mov     [bp+var_2], ax
    mov     [bp+var_110], 0
loc_36CA6:
    mov     bx, [bp+var_2]
    push    word ptr [bx+82h]
    push    word ptr [bx+80h]
    push    word ptr [bx+86h]
    push    word ptr [bx+84h]
    push    word ptr [bx+6]
    push    word ptr [bx+4]
    push    word ptr [bx+2]
    push    word ptr [bx]
    mov     ax, 4
    push    ax
    push    [bp+arg_4]
    call    sub_317EE
    add     sp, 14h
    add     [bp+var_2], 4
    inc     [bp+var_110]
    cmp     [bp+var_110], 0Fh
    jl      short loc_36CA6
    mov     bx, [bp+var_2]
    push    word ptr [bx+82h]
    push    word ptr [bx+80h]
    push    [bp+var_8C]
    push    [bp+var_8E]
    push    [bp+var_10C]
    push    [bp+var_10E]
    push    word ptr [bx+2]
    push    word ptr [bx]
    mov     ax, 4
    push    ax
    push    [bp+arg_4]
    call    sub_317EE
    add     sp, 14h
    lea     ax, [bp+var_10A]
    mov     [bp+var_4], ax
    mov     ax, [bp+var_10C]
    mov     [bp+var_118], ax
    mov     [bp+var_11A], 0
    mov     [bp+var_110], 1
loc_36D2C:
    mov     bx, [bp+var_4]
    mov     ax, [bp+var_118]
    cmp     [bx+2], ax
    jge     short loc_36D47
    mov     ax, [bx+2]
    mov     [bp+var_118], ax
    mov     ax, [bp+var_110]
    mov     [bp+var_11A], ax
loc_36D47:
    add     [bp+var_4], 4
    inc     [bp+var_110]
    cmp     [bp+var_110], 10h
    jl      short loc_36D2C
    mov     si, [bp+var_11A]
    mov     cl, 2
    shl     si, cl
    add     si, bp
    lea     ax, [si-10Eh]
    mov     [bp+var_4], ax
    lea     ax, [si-0CEh]
    mov     [bp+var_114], ax
    lea     ax, [bp+var_4E]
    mov     [bp+var_6], ax
    lea     ax, [bp+var_A]
    mov     [bp+var_116], ax
    mov     ax, [bp+var_11A]
    mov     [bp+var_112], ax
    mov     [bp+var_110], 0
    jmp     short loc_36DA2
loc_36D8C:
    add     [bp+var_4], 4
    add     [bp+var_114], 4
loc_36D95:
    add     [bp+var_6], 4
    sub     [bp+var_116], 4
    inc     [bp+var_110]
loc_36DA2:
    cmp     [bp+var_110], 8
    jg      short loc_36DEE
    mov     bx, [bp+var_6]
    mov     si, [bp+var_4]
    mov     ax, [si]
    mov     [bx], ax
    mov     ax, [si+2]
    mov     [bx+2], ax
    mov     bx, [bp+var_116]
    mov     si, [bp+var_114]
    mov     ax, [si]
    mov     [bx], ax
    mov     ax, [si+2]
    mov     [bx+2], ax
    inc     [bp+var_112]
    cmp     [bp+var_112], 10h
    jl      short loc_36D8C
    lea     ax, [bp+var_10E]
    mov     [bp+var_4], ax
    lea     ax, [bp+var_CE]
    mov     [bp+var_114], ax
    mov     [bp+var_112], 0
    jmp     short loc_36D95
    ; align 2
    db 144
loc_36DEE:
    lea     ax, [bp+var_4E]
    push    ax
    mov     ax, 12h
    push    ax
    push    [bp+arg_6]
    call    sub_317C1
    add     sp, 6
    mov     si, [bp+var_11A]
    mov     cl, 2
    shl     si, cl
    add     si, bp
    lea     ax, [si-10Eh]
    mov     [bp+var_4], ax
    lea     ax, [si-0CEh]
    mov     [bp+var_114], ax
    lea     ax, [bp+var_4E]
    mov     [bp+var_6], ax
    lea     ax, [bp+var_A]
    mov     [bp+var_116], ax
    mov     ax, [bp+var_11A]
    mov     [bp+var_112], ax
    mov     [bp+var_110], 0
    jmp     short loc_36E4E
    ; align 2
    db 144
loc_36E38:
    sub     [bp+var_4], 4
    sub     [bp+var_114], 4
loc_36E41:
    add     [bp+var_6], 4
    sub     [bp+var_116], 4
    inc     [bp+var_110]
loc_36E4E:
    cmp     [bp+var_110], 9
    jge     short loc_36E94
    mov     bx, [bp+var_6]
    mov     si, [bp+var_4]
    mov     ax, [si]
    mov     [bx], ax
    mov     ax, [si+2]
    mov     [bx+2], ax
    mov     bx, [bp+var_116]
    mov     si, [bp+var_114]
    mov     ax, [si]
    mov     [bx], ax
    mov     ax, [si+2]
    mov     [bx+2], ax
    dec     [bp+var_112]
    jns     short loc_36E38
    lea     ax, [bp+var_D2]
    mov     [bp+var_4], ax
    lea     ax, [bp+var_92]
    mov     [bp+var_114], ax
    mov     [bp+var_112], 10h
    jmp     short loc_36E41
loc_36E94:
    lea     ax, [bp+var_4E]
    push    ax
    mov     ax, 12h
    push    ax
    push    [bp+arg_6]
    call    sub_317C1
    add     sp, 6
    lea     ax, [bp+var_CE]
    push    ax
    mov     ax, 10h
    push    ax
    push    [bp+arg_8]
    call    sub_317C1
    add     sp, 6
    pop     si
    mov     sp, bp
    pop     bp
    retf
sub_36C7E endp
seg022 ends
end
