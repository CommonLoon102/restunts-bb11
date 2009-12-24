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

extern void fatal_error(char*, ...);
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

void far* dos_get_psp() {
	unsigned short resseg, resofs;
	__asm {
		push ds
		mov ah, 62h
		int 21h
		mov resseg, ds
		mov resofs, bx
		pop ds
	}
	return MK_FP(resseg, resofs);
}

unsigned short dos_alloc(unsigned short paras) {
	unsigned short resseg;
	__asm {
		mov bx, paras
		mov ah, 48h
		int 21h
		mov resseg, ax
	}
	return resseg;
}

unsigned short dos_setblock(unsigned short blockseg, unsigned short newsize) {
	unsigned short res;
	__asm {
		mov bx, newsize
		mov es, blockseg
		mov ah, 4ah
		int 21h
		mov res, bx	// bx = max blocks
	}
	return res;
}

void mmgr_alloc_resmem(unsigned short arg_0) {

	void far* psp;
	unsigned short maxblocks;
	struct RESOURCE* rp;

	psp = dos_get_psp();
	pspseg = FP_SEG(psp);
	pspofs = FP_OFF(psp);
	
	if (word_3FF82 == 0) {
		resptr1->resofs = dos_alloc(0x64);
		word_3FF84 = resptr1->resofs;
		maxblocks = dos_setblock(resptr1->resofs, arg_0 - resptr1->resofs);
		maxblocks = dos_setblock(resptr1->resofs, maxblocks);
		resendptr2->resofs = word_3FF84 + maxblocks;
		word_3FF82 = resendptr2->resofs;
	}
	resendptr1 = resendptr2;
	resptr2 = resptr1;
	
	rp = resptr1;
	for (;;) {
		rp++;
		if (rp == resendptr2) break;
		rp->resunk = 0;
	}
}

void mmgr_alloc_a000() {
	return mmgr_alloc_resmem(0xa000);
}

unsigned short mmgr_get_ofs_diff() {
	return resendptr2->resofs - resptr2->resofs - resptr2->ressize;
}

void far* mmgr_free(unsigned short arg_0, unsigned short arg_2) {
	int i;
	unsigned short ax, bx, cx, dx, di;
	struct RESOURCE* ressi;
	struct RESOURCE* resbx;

	ressi = resptr2;

	while (1) {
		if (ressi == resptr1) 
			fatal_error(aMemoryManagerB, arg_2);
		if (ressi->resofs == arg_2) break;
		ressi--;
	}

	arg_2 = 0;
	ressi->resunk = 0;
	if (ressi != resptr2) {
		if (ressi == resendptr1) goto loc_31508;
		ax = resendptr1->resofs - resptr2->resofs - resptr2->ressize;
		if (ax < ressi->ressize) goto loc_31508;
	}

	arg_2 = resendptr1->resofs - ressi->ressize;
	resendptr1--;
	resendptr1->resofs = arg_2;
	resendptr1->ressize = ressi->ressize;
	resendptr1->resunk = 1;

	for (i = 0; i < 0xC; i++) {
		resendptr1->resname[i] = ressi->resname[i];
	}

	sub_311D5(ressi->resofs, arg_2, ressi->ressize);

loc_31508:
	if (ressi == resptr2) {
		do {
			ressi--;
		} while (ressi->resunk == 0);
		resptr2 = ressi;
	}

	return MK_FP(arg_0, arg_2);
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
