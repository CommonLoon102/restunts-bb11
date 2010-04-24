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
seg015 segment byte public 'STUNTSC' use16
    assume cs:seg015
    assume es:nothing, ss:nothing, ds:dseg
    public sub_36412
sub_36412 proc far
    var_1A = word ptr -26
    var_18 = word ptr -24
    var_16 = word ptr -22
    var_14 = word ptr -20
    var_12 = word ptr -18
    var_10 = word ptr -16
    var_E = word ptr -14
    var_C = word ptr -12
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
loc_36413:
    mov     bp, sp
loc_36415:
    sub     sp, 1Ah
loc_36418:
    push    di
loc_36419:
    push    si
loc_3641A:
    mov     bx, [bp+arg_0]
loc_3641D:
    mov     si, [bx]
loc_3641F:
    mov     ax, [bx+4]
loc_36422:
    sub     ax, si
loc_36424:
    mov     bx, [bp+arg_2]
loc_36427:
    mov     [bx], ax
loc_36429:
    mov     bx, [bp+arg_0]
loc_3642C:
    mov     ax, [bx+6]
loc_3642F:
    sub     ax, [bx+2]
loc_36432:
    mov     bx, [bp+arg_2]
loc_36435:
    mov     [bx+2], ax
loc_36438:
    mov     bx, [bp+arg_0]
loc_3643B:
    mov     ax, [bx+8]
loc_3643E:
    sub     ax, si
loc_36440:
    mov     bx, [bp+arg_2]
loc_36443:
    mov     [bx+20h], ax
loc_36446:
    mov     bx, [bp+arg_0]
loc_36449:
    mov     ax, [bx+0Ah]
loc_3644C:
    sub     ax, [bx+2]
loc_3644F:
    mov     bx, [bp+arg_2]
loc_36452:
    mov     [bx+22h], ax
loc_36455:
    mov     ax, [bx]
loc_36457:
    sar     ax, 1
loc_36459:
    mov     [bp+var_2], ax
loc_3645C:
    sar     ax, 1
    mov     [bp+var_A], ax
    mov     ax, [bp+var_2]
loc_36464:
    add     ax, [bp+var_A]
    mov     [bp+var_6], ax
    mov     ax, [bx+2]
    sar     ax, 1
    mov     [bp+var_4], ax
    sar     ax, 1
    mov     [bp+var_C], ax
    mov     ax, [bp+var_4]
loc_3647A:
    add     ax, [bp+var_C]
    mov     [bp+var_8], ax
    mov     ax, [bx+20h]
loc_36483:
    sar     ax, 1
loc_36485:
    mov     [bp+var_10], ax
loc_36488:
    sar     ax, 1
    mov     [bp+var_18], ax
    mov     ax, [bp+var_10]
    add     ax, [bp+var_18]
    mov     [bp+var_14], ax
    mov     ax, [bx+22h]
    sar     ax, 1
loc_3649B:
    mov     [bp+var_12], ax
    sar     ax, 1
    mov     [bp+var_1A], ax
    mov     ax, [bp+var_12]
    add     ax, [bp+var_1A]
    mov     [bp+var_16], ax
    mov     ax, (offset sceneshapes2.ss_loShapePtr+0E1h)
    push    ax
    mov     ax, [bx+20h]
    add     ax, [bx]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+10h], ax
loc_364C4:
    mov     ax, (offset sceneshapes2.ss_loShapePtr+0E1h)
    push    ax
    mov     ax, [bx+2]
loc_364CB:
    add     ax, [bx+22h]
loc_364CE:
    push    ax
loc_364CF:
    call    multiply_and_scale
loc_364D4:
    add     sp, 4
loc_364D7:
    mov     bx, [bp+arg_2]
loc_364DA:
    mov     [bx+12h], ax
loc_364DD:
    mov     ax, 393Eh
    push    ax
loc_364E1:
    mov     ax, [bx]
    add     ax, [bp+var_10]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+8], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+2]
    add     ax, [bp+var_12]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Ah], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+20h]
    add     ax, [bp+var_2]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+18h], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+22h]
    add     ax, [bp+var_4]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+1Ah], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx]
    add     ax, [bp+var_18]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+4], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx+2]
    add     ax, [bp+var_1A]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+6], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx+20h]
    add     ax, [bp+var_A]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
loc_36587:
    mov     [bx+1Ch], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx+22h]
    add     ax, [bp+var_C]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+1Eh], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bx]
    add     ax, [bp+var_14]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Ch], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bx+2]
    add     ax, [bp+var_16]
    push    ax
    call    multiply_and_scale
loc_365CB:
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+0Eh], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bx+20h]
    add     ax, [bp+var_6]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
loc_365EA:
    mov     [bx+14h], ax
loc_365ED:
    mov     ax, 3333h
    push    ax
    mov     ax, [bx+22h]
    add     ax, [bp+var_8]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+16h], ax
    mov     ax, [bx]
    neg     ax
    mov     si, ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+20h]
    add     ax, si
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+30h], ax
    mov     ax, [bx+2]
    neg     ax
    mov     di, ax
    mov     ax, 2D41h
    push    ax
    mov     ax, [bx+22h]
    add     ax, di
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+32h], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bp+var_10]
    add     ax, si
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+38h], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bp+var_12]
    add     ax, di
    push    ax
    call    multiply_and_scale
    add     sp, 4
loc_3666D:
    mov     bx, [bp+arg_2]
loc_36670:
    mov     [bx+3Ah], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+20h]
    sub     ax, [bp+var_2]
    push    ax
loc_3667E:
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+28h], ax
    mov     ax, 393Eh
    push    ax
    mov     ax, [bx+22h]
loc_36693:
    sub     ax, [bp+var_4]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
loc_366A2:
    mov     [bx+2Ah], ax
    mov     ax, [bx]
    neg     ax
    mov     si, ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bp+var_18]
    add     ax, si
    push    ax
loc_366B5:
    call    multiply_and_scale
    add     sp, 4
loc_366BD:
    mov     bx, [bp+arg_2]
    mov     [bx+3Ch], ax
    mov     ax, [bx+2]
    neg     ax
    mov     di, ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bp+var_1A]
loc_366D1:
    add     ax, di
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+3Eh], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx+20h]
    sub     ax, [bp+var_A]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+24h], ax
    mov     ax, 3E17h
    push    ax
    mov     ax, [bx+22h]
    sub     ax, [bp+var_C]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+26h], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bp+var_14]
    add     ax, si
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+34h], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bp+var_16]
    add     ax, di
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+36h], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bx+20h]
    sub     ax, [bp+var_6]
    push    ax
    call    multiply_and_scale
    add     sp, 4
    mov     bx, [bp+arg_2]
    mov     [bx+2Ch], ax
    mov     ax, 3333h
    push    ax
    mov     ax, [bx+22h]
loc_36764:
    sub     ax, [bp+var_8]
    push    ax
    call    multiply_and_scale
    add     sp, 4
loc_36770:
    mov     bx, [bp+arg_2]
loc_36773:
    mov     [bx+2Eh], ax
loc_36776:
    mov     [bp+var_E], 0
loc_3677B:
    mov     si, [bp+var_E]
loc_3677E:
    mov     cl, 2
    shl     si, cl
loc_36782:
    add     si, [bp+arg_2]
    mov     bx, [bp+arg_0]
loc_36788:
    mov     di, [bx]
loc_3678A:
    mov     ax, di
loc_3678C:
    sub     ax, [si]
    mov     [si+40h], ax
loc_36791:
    mov     ax, [bx+2]
    sub     ax, [si+2]
loc_36797:
    mov     [si+42h], ax
loc_3679A:
    add     [si], di
loc_3679C:
    mov     ax, [bx+2]
loc_3679F:
    add     [si+2], ax
    inc     [bp+var_E]
loc_367A5:
    cmp     [bp+var_E], 10h
loc_367A9:
    jl      short loc_3677B
    pop     si
loc_367AC:
    pop     di
loc_367AD:
    mov     sp, bp
    pop     bp
locret_367B0:
    retf
sub_36412 endp
seg015 ends
end
