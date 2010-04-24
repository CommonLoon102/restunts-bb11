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
    include seg033.inc
    include seg034.inc
    include seg035.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg036 segment byte public 'STUNTSC' use16
    assume cs:seg036
    assume es:nothing, ss:nothing, ds:dseg
    public get_unflip_size
get_unflip_size proc far
    var_C = word ptr -12
    var_A = word ptr -10
    var_8 = word ptr -8
    var_6 = word ptr -6
    var_4 = dword ptr -4
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8

    push    bp
loc_3B0BB:
    mov     bp, sp
loc_3B0BD:
    sub     sp, 0Ch
loc_3B0C0:
    push    [bp+arg_2]
loc_3B0C3:
    push    [bp+arg_0]
loc_3B0C6:
    call    sub_3264A
loc_3B0CB:
    add     sp, 4
loc_3B0CE:
    mov     [bp+var_A], ax
loc_3B0D1:
    mov     [bp+var_6], 0
loc_3B0D6:
    mov     [bp+var_8], 0
loc_3B0DB:
    jmp     short loc_3B11B
    ; align 2
    db 144
loc_3B0DE:
    push    [bp+var_8]
loc_3B0E1:
    push    [bp+arg_2]
loc_3B0E4:
    push    [bp+arg_0]
loc_3B0E7:
    call    sub_3265B
loc_3B0EC:
    add     sp, 6
loc_3B0EF:
    mov     word ptr [bp+var_4], ax
loc_3B0F2:
    mov     word ptr [bp+var_4+2], dx
loc_3B0F5:
    les     bx, [bp+var_4]
loc_3B0F8:
    mov     ax, es:[bx+2]
loc_3B0FC:
    imul    word ptr es:[bx]
    add     ax, 20h ; ' '
loc_3B102:
    mov     [bp+var_C], ax
    mov     cl, 4
loc_3B107:
    shr     [bp+var_C], cl
loc_3B10A:
    mov     ax, [bp+var_6]
loc_3B10D:
    cmp     [bp+var_C], ax
loc_3B110:
    jbe     short loc_3B118
    mov     ax, [bp+var_C]
loc_3B115:
    mov     [bp+var_6], ax
loc_3B118:
    inc     [bp+var_8]
loc_3B11B:
    mov     ax, [bp+var_A]
loc_3B11E:
    cmp     [bp+var_8], ax
loc_3B121:
    jl      short loc_3B0DE
loc_3B123:
    mov     ax, [bp+var_6]
loc_3B126:
    mov     sp, bp
loc_3B128:
    pop     bp
locret_3B129:
    retf
get_unflip_size endp
seg036 ends
end
