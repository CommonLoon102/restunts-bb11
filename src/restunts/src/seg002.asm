.model large
nosmart
    include structs.inc
    include custom.inc
    include seg000.inc
    include seg001.inc
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
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include dseg.inc
    include seg041.inc
seg002 segment byte public 'STUNTSC' use16
    assume cs:seg002
    assume es:nothing, ss:nothing, ds:dseg
    public sub_19DC6
    public sub_19E21
    public sub_19E4E
    public sub_19E7B
    public sub_19EE8
    ; align 2
    db 144
sub_19DC6 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6
    arg_2 = word ptr 8
    arg_4 = word ptr 10
    arg_6 = word ptr 12
    arg_8 = word ptr 14

    push    bp
    mov     bp, sp
    push    bp
    mov     cx, [bp+arg_0]
    mov     ax, [bp+arg_2]
    cmp     [bp+arg_6], 0
    jnz     short loc_19DDB
    mul     [bp+arg_4]
    mov     cx, dx
loc_19DDB:
    cmp     cx, [bp+arg_8]
    jnb     short loc_19DE3
    mov     cx, [bp+arg_8]
loc_19DE3:
    mov     ax, cx
    pop     bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    push    bp
    xor     ax, ax
    mov     bx, [bp+6]
    or      bx, bx
    jz      short loc_19DFC
    jl      short loc_19DFB
    inc     ax
    pop     bp
    pop     bp
    retf
loc_19DFB:
    dec     ax
loc_19DFC:
    pop     bp
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     ax, [bp+6]
    int     61h             ; reserved for user interrupt
    pop     bp
    retf
    push    bp
    mov     bp, sp
    mov     ax, [bp+6]
    int     60h
    pop     bp
    retf
    push    bp
    mov     bp, sp
    push    bp
    push    si
    mov     si, [bp+6]
    int     62h             ; reserved for user interrupt
    pop     si
    pop     bp
    pop     bp
    retf
sub_19DC6 endp
sub_19E21 proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    push    bp
    mov     bx, [bp+arg_0]
    mov     al, [bx]
    mov     byte_4594A, al
    mov     al, [bx+1]
    mov     byte_4594B, al
    mov     al, [bx+2]
    mov     byte_4594C, al
    mov     al, [bx+3]
    mov     byte_4594D, al
    mov     al, [bx+4]
    mov     byte_4594E, al
    mov     al, [bx+5]
    mov     byte_4594F, al
    pop     bp
    pop     bp
    retf
sub_19E21 endp
sub_19E4E proc far
     s = byte ptr 0
     r = byte ptr 2
    arg_0 = word ptr 6

    push    bp
    mov     bp, sp
    push    bp
    mov     bx, [bp+arg_0]
    mov     al, byte_4594A
    mov     [bx], al
    mov     al, byte_4594B
    mov     [bx+1], al
    mov     al, byte_4594C
    mov     [bx+2], al
    mov     al, byte_4594D
    mov     [bx+3], al
    mov     al, byte_4594E
    mov     [bx+4], al
    mov     al, byte_4594F
    mov     [bx+5], al
    pop     bp
    pop     bp
    retf
sub_19E4E endp
sub_19E7B proc far

    mov     al, byte_4594F
    add     al, byte_4594E
    mov     byte_4594E, al
    add     al, byte_4594D
    mov     byte_4594D, al
    add     al, byte_4594C
    mov     byte_4594C, al
    add     al, byte_4594B
    mov     byte_4594B, al
    add     al, byte_4594A
    mov     byte_4594A, al
    inc     byte_4594F
    jnz     short loc_19EC3
    inc     byte_4594E
    jnz     short loc_19EC3
    inc     byte_4594D
    jnz     short loc_19EC3
    inc     byte_4594C
    jnz     short loc_19EC3
    inc     byte_4594B
    jnz     short loc_19EC3
    inc     byte_4594A
loc_19EC3:
    mov     al, byte_4594A
    xor     ah, ah
    retf
    push    bp
    mov     bp, sp
    push    ds
    mov     ax, seg dseg
    mov     ds, ax
    mov     ax, [bp+4]
    mov     word_3BE30, ax
    mov     ax, [bp+2]
    mov     word_3BE32, ax
    inc     ax
    inc     ax
    mov     [bp+2], ax
    xor     ax, ax
    pop     ds
    pop     bp
    iret
sub_19E7B endp
sub_19EE8 proc far

    push    ds
    mov     ah, 35h ; '5'
    mov     al, 0
    int     21h             ; DOS - 2+ - GET INTERRUPT VECTOR
    mov     word ptr dword_3BE2C+2, es
    mov     word ptr dword_3BE2C, bx
    mov     dx, seg seg002
    mov     ds, dx
    mov     dx, 109h
    mov     ah, 25h ; '%'
    mov     al, 0
    int     21h             ; DOS - SET INTERRUPT VECTOR
    pop     ds
    retf
    db 30
    db 197
    db 22
    db 188
    db 6
    db 180
    db 37
    db 176
    db 0
    db 205
    db 33
sub_19EE8 endp
seg002 ends
end
