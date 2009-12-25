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
    public mat_rot_x
    public mat_rot_y
    public mat_rot_z
    ; align 2
    db 144
mat_rot_x proc far
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
    mov     [bx+MATRIX._11], 4000h
    mov     [bx+MATRIX._21], 0
    mov     [bx+MATRIX._31], 0
    mov     [bx+MATRIX._12], 0
    mov     ax, [bp+var_2]
    mov     [bx+MATRIX._22], ax
    mov     ax, [bp+var_4]
    mov     [bx+MATRIX._32], ax
    mov     [bx+MATRIX._13], 0
    neg     ax
    mov     [bx+MATRIX._23], ax
    mov     ax, [bp+var_2]
    mov     [bx+MATRIX._33], ax
    mov     sp, bp
    pop     bp
    retf
    ; align 2
    db 144
mat_rot_x endp
mat_rot_y proc far
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
    mov     [bx+MATRIX._11], ax
    mov     [bx+MATRIX._21], 0
    mov     ax, [bp+var_4]
    neg     ax
    mov     [bx+MATRIX._31], ax
    mov     [bx+MATRIX._12], 0
    mov     [bx+MATRIX._22], 4000h
    mov     [bx+MATRIX._32], 0
    mov     ax, [bp+var_4]
    mov     [bx+MATRIX._13], ax
    mov     [bx+MATRIX._23], 0
    mov     ax, [bp+var_2]
    mov     [bx+MATRIX._33], ax
    mov     sp, bp
    pop     bp
    retf
mat_rot_y endp
mat_rot_z proc far
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
    mov     [bx+MATRIX._11], ax
    mov     ax, [bp+var_4]
    mov     [bx+MATRIX._21], ax
    mov     [bx+MATRIX._31], 0
    neg     ax
    mov     [bx+MATRIX._12], ax
    mov     ax, [bp+var_2]
loc_37012:
    mov     [bx+MATRIX._22], ax
    mov     [bx+MATRIX._32], 0
loc_3701A:
    mov     [bx+MATRIX._13], 0
loc_3701F:
    mov     [bx+MATRIX._23], 0
loc_37024:
    mov     [bx+MATRIX._33], 4000h
    mov     sp, bp
    pop     bp
    retf
mat_rot_z endp
seg024 ends
end
