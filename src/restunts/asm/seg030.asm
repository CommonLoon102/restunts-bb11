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
seg030 segment byte public 'STUNTSC' use16
    assume cs:seg030
    assume es:nothing, ss:nothing, ds:dseg
    public byte_39CCA
    public byte_39CCC
    public sub_39CCE
byte_39CCA     db 144
    db 144
byte_39CCC     db 0
    db 0
sub_39CCE proc far
    var_2 = word ptr -2
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = dword ptr 6
    arg_4 = word ptr 10

    push    bp
loc_39CCF:
    mov     bp, sp
loc_39CD1:
    sub     sp, 2
loc_39CD4:
    push    di
    push    si
loc_39CD6:
    mov     si, offset byte_42D2E
    push    ds
loc_39CDA:
    pop     es
loc_39CDB:
    mov     di, word ptr [bp+arg_0]
loc_39CDE:
    mov     cx, 0FFFFh
loc_39CE1:
    xor     ax, ax
loc_39CE3:
    repne scasb
loc_39CE5:
    not     cx
loc_39CE7:
    sub     di, cx
loc_39CE9:
    xchg    di, si
loc_39CEB:
    shr     cx, 1
loc_39CED:
    repne movsw
loc_39CEF:
    adc     cx, cx
loc_39CF1:
    repne movsb
loc_39CF3:
    mov     ax, 5Ch ; '\'
loc_39CF6:
    push    ax
loc_39CF7:
    mov     ax, offset byte_42D2E
loc_39CFA:
    push    ax              ; char *
loc_39CFB:
    call    _strrchr
loc_39D00:
    add     sp, 4
loc_39D03:
    mov     [bp+var_2], ax
loc_39D06:
    or      ax, ax
loc_39D08:
    jz      short loc_39D16
loc_39D0A:
    inc     [bp+var_2]
    mov     bx, [bp+var_2]
    mov     byte ptr [bx], 0
loc_39D13:
    jmp     short loc_39D1B
    ; align 2
    db 144
loc_39D16:
    mov     byte_42D2E, 0
loc_39D1B:
    mov     si, offset byte_42D2E
loc_39D1E:
    push    ds
    pop     es
    mov     di, [bp+arg_4]
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
loc_39D2C:
    sub     di, cx
    mov     bx, cx
    xchg    di, si
loc_39D32:
    mov     cx, 0FFFFh
loc_39D35:
    repne scasb
    dec     di
loc_39D38:
    mov     cx, bx
    shr     cx, 1
    repne movsw
    adc     cx, cx
    repne movsb
    mov     ax, 5Ch ; '\'
    push    ax
    push    word ptr [bp+arg_0]; char *
loc_39D49:
    call    _strrchr
    add     sp, 4
    mov     [bp+var_2], ax
    or      ax, ax
    jz      short loc_39D6A
    inc     [bp+var_2]
    mov     bx, [bp+var_2]
    mov     di, bx
    mov     si, offset byte_42D2E
    mov     ax, ds
    mov     es, ax
    jmp     short loc_39D72
    ; align 2
    db 144
loc_39D6A:
    mov     si, offset byte_42D2E
    push    ds
    pop     es
    mov     di, word ptr [bp+arg_0]
loc_39D72:
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
loc_39D7B:
    sub     di, cx
loc_39D7D:
    mov     bx, cx
loc_39D7F:
    xchg    di, si
loc_39D81:
    mov     cx, 0FFFFh
loc_39D84:
    repne scasb
loc_39D86:
    dec     di
loc_39D87:
    mov     cx, bx
loc_39D89:
    shr     cx, 1
loc_39D8B:
    repne movsw
loc_39D8D:
    adc     cx, cx
loc_39D8F:
    repne movsb
loc_39D91:
    mov     di, offset byte_42D2E
    mov     ax, ds
    mov     es, ax
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    dec     cx
    mov     bx, cx
    cmp     byte_42D2A[bx], 2Eh ; '.'
    jnz     short loc_39DBF
    mov     di, offset byte_42D2E
    mov     ax, ds
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
    dec     cx
    cmp     cx, 4
    ja      short loc_39E0B
loc_39DBF:
    mov     di, offset unk_407AC
loc_39DC2:
    mov     si, offset byte_42D2E
    mov     ax, ds
    mov     cx, 0FFFFh
    xor     ax, ax
    repne scasb
    not     cx
loc_39DD0:
    sub     di, cx
loc_39DD2:
    mov     bx, cx
loc_39DD4:
    xchg    di, si
loc_39DD6:
    mov     cx, 0FFFFh
    repne scasb
    dec     di
    mov     cx, bx
loc_39DDE:
    shr     cx, 1
    repne movsw
loc_39DE2:
    adc     cx, cx
loc_39DE4:
    repne movsb
loc_39DE6:
    mov     si, offset byte_42D2E
loc_39DE9:
    mov     di, word ptr [bp+arg_0+2]
loc_39DEC:
    mov     cx, 0FFFFh
    xor     ax, ax
loc_39DF1:
    repne scasb
    not     cx
    sub     di, cx
loc_39DF7:
    mov     bx, cx
loc_39DF9:
    xchg    di, si
    mov     cx, 0FFFFh
loc_39DFE:
    repne scasb
loc_39E00:
    dec     di
    mov     cx, bx
    shr     cx, 1
loc_39E05:
    repne movsw
loc_39E07:
    adc     cx, cx
loc_39E09:
    repne movsb
loc_39E0B:
    mov     ax, offset byte_42D2E
loc_39E0E:
    pop     si
    pop     di
loc_39E10:
    mov     sp, bp
loc_39E12:
    pop     bp
locret_39E13:
    retf
sub_39CCE endp
seg030 ends
end
