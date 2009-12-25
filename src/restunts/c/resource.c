#include <dos.h>

struct RESOURCE {
	char resname[12];
	unsigned short ressize;
	unsigned short resofs;
	unsigned short resunk;
};

extern unsigned short pspofs;
extern unsigned short pspseg;
extern unsigned short word_3FF82;
extern unsigned short word_3FF84;
extern struct RESOURCE* resptr1;
extern struct RESOURCE* resptr2;
extern struct RESOURCE* resendptr1;
extern struct RESOURCE* resendptr2;
extern unsigned short resmaxsize;
extern char* aReservememoryO;
extern char* aReservememoryOutOfMemory;
extern char* aMemoryManagerB;
extern char* aResizememoryNo;
extern char* aResizememoryCa;

extern void fatal_error(char*, ...);

void copy_paras_reverse(unsigned short srcseg, unsigned short destseg, short paras);

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

void far* mmgr_alloc_pages(char* arg_0, unsigned short arg_2) {
	int i;
	struct RESOURCE* resdi;
	struct RESOURCE* ressi;
	char* chunkname;
	unsigned short rax, rdx;

	resdi = resptr2;
	ressi = resendptr1;
	rdx = resdi->resofs + resdi->ressize;
	resdi++;
	if (ressi <= resdi) {
		if (ressi == resendptr2) 
			fatal_error(aReservememoryO, arg_0);

		ressi++;
		resendptr1 = ressi;
	}

	resptr2 = resdi;
	chunkname = mmgr_path_to_name(arg_0);
	for (i = 0; i < 0xC; i++)
		resdi->resname[i] = chunkname[i];

	rax = arg_2;
	resdi->resofs = rdx;
	resdi->ressize = rax;
	resdi->resunk = 2;

	rax += rdx;
	if (rax > resmaxsize) 
		resmaxsize = rax;

	if (rax > ressi->resofs) {
		ressi = resendptr1;
		resdi = resptr2;
		rax = resdi->resofs + resdi->ressize;
	
		while (rax > ressi->resofs) {
			if (ressi == resendptr2)
				fatal_error(aReservememoryOutOfMemory, arg_0, resdi->ressize, resmaxsize);

			ressi->resunk = 0;
			ressi++;
			resendptr1 = ressi;
		}
	}

	return MK_FP(rdx, 0);
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

	copy_paras_reverse(ressi->resofs, arg_2, ressi->ressize);

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


void copy_paras_reverse(unsigned short srcseg, unsigned short destseg, short paras) {
	unsigned short count, ofs;
	unsigned short far* destptr;
	unsigned short far* srcptr;

	pushregs();

	srcseg += paras;
	destseg += paras;

	while (paras != 0) {
		count = 0x1000;
		paras -= 0x1000;
		if (paras < 0) {
			count = paras + 0x1000;
			paras = 0;
		}
		srcseg -= count;
		destseg -= count;
		count <<= 3;
		ofs = (count << 1) - 2;

		srcptr = MK_FP(srcseg, ofs);
		destptr = MK_FP(destseg, ofs);
		while (count) {
			*destptr = *srcptr;
			srcptr--;
			destptr--;
			count--;
		}
	}
	popregs();
}



void mmgr_find_free() {
	//return mmgr_find_free2();
//     r = byte ptr 0
__asm {
    push    si
    push    di
    push    dx
    mov     si, resendptr2
    mov     di, si
    xor     dx, dx
loc_31308:
    test    word ptr [si+.resunk], 1
    jnz     short loc_31315
    add     dx, [si+.ressize]
    jmp     short loc_3131C
    db 144
loc_31315:
    or      dx, dx
    jnz     short loc_31330
loc_31319:
    sub     di, 12h
loc_3131C:
    sub     si, 12h
    cmp     si, resendptr1
    jnb     short loc_31308
    add     di, 12h
    mov     resendptr1, di
    pop     dx
    pop     di
    pop     si
    jmp endfunc

loc_31330:
    mov     bx, [si+.ressize]
    push    bx
    add     di, 12h
    mov     ax, [di+.resofs]
    sub     ax, bx
    push    ax
    push    word ptr [si+.resofs]
    sub     di, 12h
    mov     [di+.ressize], bx
    mov     [di+.resofs], ax
    mov     cx, [si+.resunk]
    mov     word ptr [si+.resunk], 0
    mov     [di+.resunk], cx
    xor     bx, bx
    mov     cx, 0Ch
loc_31359:
    mov     al, byte ptr [bx+si+.resname]
    mov     byte ptr [bx+di+.resname], al
    inc     bx
    loop    loc_31359
    call    far ptr copy_paras_reverse
    add     sp, 6
    jmp     short loc_31319
}
endfunc:
}

void mmgr_get_unk(unsigned short arg_0) {
		//return mmgr_get_unk2(arg_0);
__asm {
//     s = byte ptr 0
//     r = byte ptr 2
//    arg_0 = word ptr 6

    push    si
    push    di
    mov     si, resendptr1
    push    [arg_0]
    call    far ptr mmgr_path_to_name
    add     sp, 2
    mov     di, ax
loc_31380:
    xor     bx, bx
    cmp     word ptr [si+.resunk], 0
    jz      short loc_313AE
loc_31388:
    mov     al, [bx+di]
    or      al, al
    jz      short loc_3139B
    cmp     al, [bx+si]
    jnz     short loc_313A5
    inc     bx
    cmp     bx, 0Ch
    jl      short loc_31388
    jmp     short loc_313B6
    db 144
loc_3139B:
    cmp     byte ptr [bx+si], 2Eh ; '.'
    jz      short loc_313B6
    cmp     byte ptr [bx+si], 0
    jz      short loc_313B6
loc_313A5:
    add     si, 12h
    cmp     si, resendptr2
    jb      short loc_31380
loc_313AE:
    xor     dx, dx
loc_313B0:
    xor     ax, ax
    pop     di
    pop     si
    jmp endfunc

loc_313B6:
    mov     di, resptr2
    mov     dx, [di+.resofs]
    add     dx, [di+.ressize]
    add     di, 12h
    mov     resptr2, di
    mov     ax, [si+.ressize]
    push    ax
    push    dx
    push    word ptr [si+.resofs]
    mov     word ptr [si+.resunk], 0
    mov     [di+.resofs], dx
    mov     [di+.ressize], ax
    mov     word ptr [di+.resunk], 2
    xor     bx, bx
    mov     cx, 0Ch
loc_313E4:
    mov     al, byte ptr [bx+si+.resname]
    mov     byte ptr [bx+di+.resname], al
    inc     bx
    loop    loc_313E4
    cmp     di, resendptr1
    jnz     short loc_313F6
    add     resendptr1, 12h
loc_313F6:
    call    far ptr mmgr_copy_paras
    add     sp, 6
    mov     si, resendptr1
    mov     di, resptr2
    mov     ax, [di+.resofs]
    add     ax, [di+.ressize]
loc_3140C:
    cmp     ax, [si+.resofs]
    jbe     short loc_3141F
    mov     word ptr [si+.resunk], 0
    add     si, 12h
    mov     resendptr1, si
    jmp     short loc_3140C
loc_3141F:
    call    far ptr mmgr_find_free
    mov     dx, [di+.resofs]
    jmp     short loc_313B0
}
endfunc:
}



void mmgr_op_unk2(unsigned short arg_0, unsigned short arg_2) {
	pushregs();
	
__asm {
	push bx
    push    si
    push    di
    mov     ax, [arg_2]
    mov     si, resptr2
loc_3164D:
    cmp     si, resptr1
    jz      short loc_3165D
    cmp     ax, [si+.resofs]
    jz      short loc_31660
    sub     si, 12h
    jmp     short loc_3164D
loc_3165D:
    //jmp     loc_31498
    push    [arg_2]
    mov     ax, offset aMemoryManagerB // "memory manager - BLOCK NOT FOUND at SEG"...
    push    ax
    call    far ptr fatal_error
loc_31660:
    mov     word ptr [si+.resunk], 0
    cmp     si, resptr2
    jnz     short loc_31678
loc_3166B:
    sub     si, 12h
    cmp     word ptr [si+.resunk], 0
    jz      short loc_3166B
    mov     resptr2, si
loc_31678:
    pop     di
    pop     si
    pop bx
}
	popregs();
}

void mmgr_get_chunk_size(unsigned short arg_0, unsigned short arg_2) {
__asm {
    push    si
    push    di
    mov     ax, [arg_2]
    mov     si, resptr2
loc_31688:
    cmp     si, resptr1
    jz      short loc_31698
    cmp     ax, [si+.resofs]
    jz      short loc_3169B
    sub     si, 12h
    jmp     short loc_31688
loc_31698:
//    jmp     loc_31498
    push    [arg_2]
    mov     ax, offset aMemoryManagerB // "memory manager - BLOCK NOT FOUND at SEG"...
    push    ax
    call    far ptr fatal_error

loc_3169B:
    mov     ax, [si+.ressize]
    pop     di
    pop     si
}
}

void mmgr_resize_memory(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
__asm {
    push    si
    push    di
    mov     ax, [arg_2]
    mov     si, resptr2
loc_316AE:
    cmp     si, resptr1
    jz      short loc_316BE
    cmp     ax, [si+.resofs]
    jz      short loc_316C1
    sub     si, 12h
    jmp     short loc_316AE
loc_316BE:
    //jmp     loc_31498
    push    [arg_2]
    mov     ax, offset aMemoryManagerB // "memory manager - BLOCK NOT FOUND at SEG"...
    push    ax
    call    far ptr fatal_error

loc_316C1:
    mov     ax, [arg_4]
    cmp     ax, [si+.ressize]
    ja      short loc_316D0
    mov     [si+.ressize], ax
    pop     di
    pop     si
    jmp endfunc
loc_316D0:
    cmp     si, resptr2
    jnz     short loc_316F4
    mov     [si+.ressize], ax
    mov     di, resendptr1
    add     ax, [si+.resofs]
    cmp     ax, resmaxsize
    jb      short loc_316E9
    mov     resmaxsize, ax
loc_316E9:
    cmp     ax, [di+.resofs]
    ja      short loc_316FD
loc_316EE:
    xor     ax, ax
    pop     di
    pop     si
    jmp endfunc
loc_316F4:
    mov     ax, offset aResizememoryCa //  "resizememory - CANNOT EXPAND BLOCK NOT "...
    push    ax
    call    far ptr fatal_error
loc_316FD:
    mov     si, resendptr1
    mov     di, resptr2
    mov     ax, [di+.resofs]
    add     ax, [di+.ressize]
loc_3170B:
    cmp     ax, [si+.resofs]
    jbe     short loc_316EE
    cmp     si, resendptr2
    jz      short loc_31724
    mov     word ptr [si+.resunk], 0
    add     si, 12h
    mov     resendptr1, si
    jmp     short loc_3170B
loc_31724:
    mov     bx, resmaxsize
    push    bx
    mov     ax, offset aResizememoryNo //  "resizememory - NO MEMORY LEFT TO EXPAND"...
    push    ax
    call    far ptr fatal_error
}
endfunc:
}

void mmgr_op_unk(unsigned short arg_0, unsigned short arg_2) {
__asm {
    push    si
    push    di
    mov     ax, [arg_2]
    mov     si, resptr2
loc_3173E:
    cmp     si, resptr1
    jz      short loc_3174E
    cmp     ax, [si+.resofs]
    jz      short loc_31751
    sub     si, 12h
    jmp     short loc_3173E
loc_3174E:
//    jmp     loc_31498
    push    [arg_2]
    mov     ax, offset aMemoryManagerB // "memory manager - BLOCK NOT FOUND at SEG"...
    push    ax
    call    far ptr fatal_error

loc_31751:
    mov     di, si
    sub     di, 12h
    cmp     word ptr [di+.resunk], 0
    jnz     short loc_317AD
loc_3175C:
    sub     di, 12h
    cmp     word ptr [di+.resunk], 0
    jz      short loc_3175C
    mov     word ptr [si+.resunk], 0
    mov     bx, [si+.ressize]
    push    bx
    mov     ax, [di+.resofs]
    add     ax, [di+.ressize]
    push    ax
    push    word ptr [si+.resofs]
    add     di, 12h
    cmp     si, resptr2
    jnz     short loc_31785
    mov     resptr2, di
loc_31785:
    mov     [di+.resofs], ax
    mov     [di+.ressize], bx
    mov     word ptr [di+.resunk], 2
    mov     cx, 0Ch
    xor     bx, bx
loc_31795:
    mov     al, byte ptr [bx+si+.resname]
    mov     byte ptr [bx+di+.resname], al
    inc     bx
    loop    loc_31795
    call    far ptr mmgr_copy_paras
    add     sp, 6
loc_317A4:
    mov     dx, [di+.resofs]
    xor     ax, ax
    pop     di
    pop     si
    jmp endfunc
loc_317AD:
    mov     di, si
    jmp     short loc_317A4
}
endfunc:
}
