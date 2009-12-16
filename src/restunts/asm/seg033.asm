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
    include seg024.inc
    include seg025.inc
    include seg026.inc
    include seg027.inc
    include seg028.inc
    include seg029.inc
    include seg030.inc
    include seg031.inc
    include seg032.inc
    include seg034.inc
    include seg035.inc
    include seg036.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg033 segment byte public 'STUNTSC' use16
    assume cs:seg033
    assume es:nothing, ss:nothing, ds:dseg
    public sub_3A958
    public sub_3A9A0
sub_3A958 proc far

    mov     ax, word ptr dword_4392E
    or      ax, word ptr dword_4392E+2
    jnz     short loc_3A97C
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    sub_34C0C
    add     sp, 6
    mov     word ptr dword_4392E, ax
    mov     word ptr dword_4392E+2, dx
loc_3A97C:
    mov     ax, 5F3Eh
    mov     dx, seg seg012
    push    dx
    push    ax
    call    sub_35AF6
    add     sp, 4
    les     bx, dword_4392E
    push    word ptr es:[bx+2]
    push    word ptr es:[bx]
    call    sub_33BDA
    add     sp, 4
    retf
sub_3A958 endp
sub_3A9A0 proc far

    mov     ax, word ptr dword_4392E
    or      ax, word ptr dword_4392E+2
    jnz     short loc_3A9C4
    mov     ax, 0Fh
    push    ax
    mov     ax, 0C8h ; 'È'
    push    ax
    mov     ax, 140h
    push    ax
    call    sub_34C0C
    add     sp, 6
    mov     word ptr dword_4392E, ax
    mov     word ptr dword_4392E+2, dx
loc_3A9C4:
    push    word ptr dword_4392E+2
    push    word ptr dword_4392E
    call    sub_35AF6
    add     sp, 4
    retf
sub_3A9A0 endp
seg033 ends
end
