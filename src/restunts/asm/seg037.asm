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
    include seg036.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg037 segment byte public 'STUNTSC' use16
    assume cs:seg037
    assume es:nothing, ss:nothing, ds:dseg
    public load_2dshape_helper5
load_2dshape_helper5 proc far
    var_E = word ptr -14
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
    mov     bp, sp
    sub     sp, 0Eh
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_3264A
    add     sp, 4
    mov     [bp+var_A], ax
    mov     cl, 3
    shl     ax, cl
    add     ax, 10h
    cwd
    mov     [bp+var_E], ax
    mov     [bp+var_C], dx
    mov     [bp+var_8], 0
    jmp     short loc_3B191
loc_3B156:
    push    [bp+var_8]
    push    [bp+arg_2]
    push    [bp+arg_0]
    call    sub_3265B
    add     sp, 6
    mov     word ptr [bp+var_4], ax
    mov     word ptr [bp+var_4+2], dx
    les     bx, [bp+var_4]
    mov     ax, es:[bx+2]
    imul    word ptr es:[bx]
    mov     cl, 3
    shl     ax, cl
    mov     [bp+var_6], ax
    sub     dx, dx
    add     [bp+var_E], ax
    adc     [bp+var_C], dx
    add     [bp+var_E], 10h
    adc     [bp+var_C], 0
    inc     [bp+var_8]
loc_3B191:
    mov     ax, [bp+var_A]
    cmp     [bp+var_8], ax
    jl      short loc_3B156
    add     [bp+var_E], 10h
    adc     [bp+var_C], 0
    mov     al, 4
    push    ax
    lea     ax, [bp+var_E]
    push    ax
    call    unknown_libname_4; MS Quick C v1.0/v2.01 & MSC v5.1 DOS run-time & graphic
    mov     ax, [bp+var_E]
    mov     sp, bp
    pop     bp
    retf
    ; align 10h
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
    db 0
load_2dshape_helper5 endp
seg037 ends
end
