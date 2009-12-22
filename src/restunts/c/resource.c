#include <dos.h>

struct RESOURCE {
	char resname[12];
	short ressize;
	short resofs;
	short resunk;
};

extern unsigned short pspofs;
extern unsigned short pspseg;
extern unsigned short word_3FF82;
extern unsigned short word_3FF84;
extern struct RESOURCE* resptr1;
extern struct RESOURCE* resptr2;
extern struct RESOURCE* resendptr1;
extern struct RESOURCE* resendptr2;
extern short resmaxsize;
extern char* aReservememoryO;
extern char* aReservememoryOutOfMemory;
extern char* aMemoryManagerB;

extern void fatal_error();
extern void sub_311D5();

#define pushregs()\
	_asm {\
		push dx\
	}\


#define popregs()\
	_asm {\
		pop dx\
	}

char* mmgr_path_to_name(char* filename) {
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

void mmgr_alloc_pages(unsigned short arg_0, unsigned short arg_2) {
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
call    far ptr mmgr_path_to_name
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

void mmgr_alloc_resmem(unsigned short arg_0) {

	    //arg_0 = word ptr 6
__asm {
    push    si
    push    di
    mov     ax, 3000h
    int     21h             // DOS - GET DOS VERSION
    mov     bx, cs
    cmp     al, 3
    jl      short loc_3108E
    mov     ah, 62h
    int     21h             // DOS - 3+ - GET PSP ADDRESS
loc_3108E:
    mov     pspofs, bx
    mov     pspseg, ds
    cmp     word_3FF82, 0
    jnz     short loc_310CD
    mov     bx, 64h
    mov     ah, 48h
    int     21h             // DOS - 2+ - ALLOCATE MEMORY
    mov     si, resptr1
    mov     [si+.resofs], ax
    mov     word_3FF84, ax
    mov     es, ax
    mov     bx, [arg_0]
    sub     bx, [si+.resofs]
    mov     ah, 4Ah
    int     21h             // DOS - 2+ - ADJUST MEMORY BLOCK SIZE (SETBLOCK)
    mov     ah, 4Ah
    int     21h             // DOS - 2+ - ADJUST MEMORY BLOCK SIZE (SETBLOCK)
    mov     ax, word_3FF84
    add     ax, bx
    mov     si, resendptr2
    mov     [si+.resofs], ax
    mov     word_3FF82, ax
loc_310CD:
    mov     si, resendptr2
    mov     resendptr1, si
    mov     si, resptr1
    mov     resptr2, si
loc_310DD:
    add     si, 12h
    cmp     si, resendptr2
    jz      short loc_310ED
    mov     word ptr [si+.resunk], 0
    jmp     short loc_310DD
loc_310ED:
    pop     di
    pop     si
}
}

void mmgr_alloc_a000() {
	return mmgr_alloc_resmem(0xa000);
}

unsigned short mmgr_get_ofs_diff() {
	return resendptr2->resofs - resptr2->resofs - resptr2->ressize;
}

void mmgr_free(unsigned short arg_0, unsigned short arg_2) {
	__asm {
    push    si
    push    di
    mov     ax, [arg_2]
    mov     si, resptr2
loc_31488:
    cmp     si, resptr1
    jz      short loc_31498
    cmp     ax, [si+.resofs]
    jz      short loc_314A4
    sub     si, 12h
    jmp     short loc_31488
loc_31498:
    push    [arg_2]
    mov     ax, offset aMemoryManagerB // "memory manager - BLOCK NOT FOUND at SEG"...
    push    ax
    call    far ptr fatal_error
loc_314A4:
    mov     [arg_2], 0
    mov     word ptr [si+.resunk], 0
    cmp     si, resptr2
    jz      short loc_314CE
    mov     bx, resptr2
    mov     di, resendptr1
    cmp     si, di
    jz      short loc_31508
    mov     ax, [di+.resofs]
    sub     ax, [bx+.resofs]
    sub     ax, [bx+.ressize]
    cmp     ax, [si+.ressize]
    jb      short loc_31508
loc_314CE:
    mov     bx, [si+.ressize]
    push    bx
    mov     di, resendptr1
    mov     ax, [di+.resofs]
    sub     ax, bx
    push    ax
    mov     [arg_2], ax
    push    word ptr [si+.resofs]
    sub     di, 12h
    mov     resendptr1, di
    mov     [di+.resofs], ax
    mov     [di+.ressize], bx
    mov     word ptr [di+.resunk], 1
    mov     cx, 0Ch
    xor     bx, bx
loc_314F9:
    mov     al, byte ptr [bx+si+.resname]
    mov     byte ptr [bx+di+.resname], al
    inc     bx
    loop    loc_314F9
    call    far ptr sub_311D5
    add     sp, 6
loc_31508:
    cmp     si, resptr2
    jnz     short loc_3151B
loc_3150E:
    sub     si, 12h
    cmp     word ptr [si+.resunk], 0
    jz      short loc_3150E
    mov     resptr2, si
loc_3151B:
    mov     ax, [arg_0]
    mov     dx, [arg_2]
    pop     di
    pop     si
}
}

void mmgr_copy_paras(unsigned short srcseg, unsigned short destseg, short paras) {
	unsigned short count; // number of words to copy
	unsigned short far * srcptr;
	unsigned short far * destptr;
	
	while (paras != 0) {
		count = 0x8000; // 64k in words
		paras -= 0x1000; // 64k in paras
		if (paras < 0) {
			count = (paras + 0x1000) << 3;  // count = remaining paras < 0x1000 in words
			paras = 0;
		}
		srcptr = MK_FP(srcseg, 0);
		destptr = MK_FP(destseg, 0);

		while (count) {
			*destptr = *srcptr;
			srcptr++;
			destptr++;
			count--;
		}

		srcseg += 0x1000;
		destseg += 0x1000;
	}
}
