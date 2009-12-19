struct RESOURCE {
	char resname[12];
	short ressize;
	short resofs;
	short resunk;
};

extern void* resptr2;
extern void* resendptr1;
extern void* resendptr2;
extern short resmaxsize;
extern char* aReservememoryO;
extern char* aReservememoryOutOfMemory;

extern void fatal_error();

#define pushregs()\
	_asm {\
		push dx\
	}\


#define popregs()\
	_asm {\
		pop dx\
	}

char* check_pathdrive(char* filename) {
	char* c;
	char* result;

	pushregs();
	
	result = filename;
	for (c = filename; *c; c++) {
		if (*c == ':' || *c == '\\') 
			result = c + 1;
	}
	
	popregs();
	return result;
}

void alloc_respages(unsigned short arg_0, unsigned short arg_2) {
	__asm {
    push    si
    push    di
    mov     di, resptr2
    mov     si, resendptr1
    mov     dx, [di+.resofs]
    add     dx, [di+.ressize]
    add     di, 12h
    cmp     si, di
    jbe     short loc_312A7
loc_31262:
    mov     resptr2, di
    push    [arg_0]
//call    check_pathdrive
call    far ptr check_pathdrive
    add     sp, 2
    mov     si, ax
    xor     bx, bx
    mov     cx, 0Ch
loc_31278:
    mov     al, byte ptr [bx+si+.resname]
    mov     byte ptr [bx+di+.resname], al
    inc     bx
    loop    loc_31278
    mov     si, resendptr1
    mov     ax, [arg_2]
    mov     [di+.resofs], dx
    mov     [di+.ressize], ax
    mov     word ptr [di+.resunk], 2
    add     ax, dx
    cmp     ax, resmaxsize
    jb      short loc_3129C
    mov     resmaxsize, ax
loc_3129C:
    cmp     ax, [si+.resofs]
    ja      short loc_312C2
loc_312A1:
    xor     ax, ax
    pop     di
    pop     si
    jmp endfunc
    //pop bp
    //retf
loc_312A7:
    cmp     si, resendptr2
    jz      short loc_312B6
    add     si, 12h
    mov     resendptr1, si
    jmp     short loc_31262
loc_312B6:
    push    [arg_0]
    mov     ax, offset aReservememoryO
    push    ax
    call    far ptr fatal_error
loc_312C2:
    mov     si, resendptr1
    mov     di, resptr2
    mov     ax, [di+.resofs]
    add     ax, [di+.ressize]
loc_312D0:
    cmp     ax, [si+.resofs]
    jbe     short loc_312A1
    cmp     si, resendptr2
    jz      short loc_312E9
    mov     word ptr [si+.resunk], 0
    add     si, 12h
    mov     resendptr1, si
    jmp     short loc_312D0
loc_312E9:
    mov     bx, resmaxsize
    push    bx
    push    word ptr [di+.ressize]
    push    [arg_0]
    mov     ax, offset aReservememoryOutOfMemory
    push    ax
    call    far ptr fatal_error
}
endfunc:
}

