#include <dos.h>
#include "externs.h"
#include "memmgr.h"

#define pushregs()\
	_asm {\
		push dx\
	}\


#define popregs()\
	_asm {\
		pop dx\
	}

void far* dos_get_psp(void) {
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

const char* mmgr_path_to_name(const char* filename) {
	const char* c;
	const char* result;

	pushregs();
	
	result = filename;
	for (c = filename; *c; c++) {
		if (*c == ':' || *c == '\\') 
			result = c + 1;
	}
	
	popregs();
	return result;
}

void far* mmgr_alloc_pages(const char* arg_0, unsigned short arg_2) {
	int i;
	struct MEMCHUNK* resdi;
	struct MEMCHUNK* ressi;
	const char* chunkname;
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

void far* mmgr_alloc_resbytes(const char* name, long int size) {
	return mmgr_alloc_pages(name, size / 16);
}

void mmgr_alloc_resmem(unsigned short arg_0) {

	void far* psp;
	unsigned short maxblocks;
	struct MEMCHUNK* rp;

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
	mmgr_alloc_resmem(0xa000);
}

unsigned short mmgr_get_ofs_diff() {
	return resendptr2->resofs - resptr2->resofs - resptr2->ressize;
}

void far* mmgr_free(unsigned short arg_0, unsigned short arg_2) {
	int i;
	unsigned short ax, bx, cx, dx, di;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resbx;

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

void mmgr_find_free(void) {
	int i;
	unsigned short regax, regdx;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	pushregs();

	ressi = resendptr2;
	resdi = ressi;
	regdx = 0;

	do {
		if ((ressi->resunk & 1) == 0) {
			regdx += ressi->ressize;
		} else {
		
			if (regdx != 0) {
				resdi++;
				regax = resdi->resofs - ressi->ressize;
				resdi--;
				resdi->ressize = ressi->ressize;
				resdi->resofs = regax;
				ressi->resunk = 0;
				resdi->resunk = ressi->resunk;
				for (i = 0; i < 0xC; i++) {
					resdi->resname[i] = ressi->resname[i];
				}
				copy_paras_reverse(ressi->resofs, regax, ressi->ressize);
			}
		
			resdi--;
		}
		ressi--;
	} while (ressi > resendptr1);

	resdi++;
	resendptr1 = resdi;

	popregs();
}

void far* mmgr_get_unk(const char* arg_0) {
	int i;
	unsigned short regax, regbx, regcx, regdx;
	const char* chunkname;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	ressi = resendptr1;
	chunkname = mmgr_path_to_name(arg_0);

	for (ressi = resendptr1; ressi < resendptr2; ressi++) {
		regbx = 0;
		if (ressi->resunk == 0) return MK_FP(0, 0);

		for (; regbx < 0xC; regbx++) {
			if (chunkname[regbx] == 0) break;
			if (chunkname[regbx] != ressi->resname[regbx]) break;
		}

		if (regbx == 0xC || ressi->resname[regbx] == '.' || ressi->resname[regbx] == 0) {
			resdi = resptr2;
			regdx = resdi->resofs;
			regdx += resdi->ressize;
			resdi++;
			resptr2 = resdi;
			regax = ressi->ressize;
		
			ressi->resunk = 0;
			resdi->resofs = regdx;
			resdi->ressize = regax;
			resdi->resunk = 2;
			regbx = 0;
			for (i = 0; i < 0xC; i++) {
				resdi->resname[i] = ressi->resname[i];
			}
			
			if (resdi == resendptr1) {
				resendptr1++;
			}
		
			mmgr_copy_paras(ressi->resofs, regdx, ressi->ressize);
			ressi = resendptr1;
			resdi = resptr2;
			regax = resdi->resofs;
			regax += resdi->ressize;
		
			while (regax > ressi->resofs) {
				ressi->resunk = 0;
				ressi++;
				resendptr1 = ressi;
			}
		
			mmgr_find_free();
			regdx = resdi->resofs;
			return MK_FP(regdx, 0);
		}
	}

	return MK_FP(0, 0);
}



void mmgr_op_unk2(unsigned short arg_0, unsigned short arg_2) {
	int i;
	unsigned short regax, regbx, regcx, regdx;
	char* strdi;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	pushregs();
	__asm {
		push bx
	}
	
	(void)arg_0;
	regax = arg_2;
	ressi = resptr2;

	for (;;) {
		if (ressi == resptr1) 
			fatal_error(aMemoryManagerB, arg_2);
		if (regax == ressi->resofs) break;
		ressi--;
	}
	
	ressi->resunk = 0;
	if (ressi == resptr2) {
		do {
			ressi--;
		} while (ressi->resunk == 0);
		resptr2 = ressi;
	}

	__asm {
		pop bx
	}
	popregs();
}

unsigned short mmgr_get_chunk_size(unsigned short arg_0, unsigned short arg_2) {
	int i;
	unsigned short regax, regbx, regcx, regdx;
	char* strdi;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	(void)arg_0;
	regax = arg_2;
	ressi = resptr2;

	for (;;) {
		if (ressi == resptr1) 
			fatal_error(aMemoryManagerB, arg_2);
		if (regax == ressi->resofs) break;
		ressi--;
	}
	return ressi->ressize;
}

unsigned short mmgr_resize_memory(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	int i;
	unsigned short regax, regbx, regcx, regdx;
	char* strdi;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	pushregs();

	(void)arg_0;
	regax = arg_2;
	ressi = resptr2;

	for (;;) {
		if (ressi == resptr1)
			fatal_error(aMemoryManagerB, arg_2);
		if (regax == ressi->resofs) break;
		ressi--;
	}

	regax = arg_4;
	if (regax <= ressi->ressize) {
		ressi->ressize = regax;
		popregs();
		return regax;
	}

	if (ressi != resptr2)
		fatal_error(aResizememoryCa);
	ressi->ressize = regax;
	resdi = resendptr1;
	regax += ressi->resofs;
	if (regax >= resmaxsize)
		resmaxsize = regax;

	if (regax <= resdi->resofs) {
		popregs();
		return 0;
	}

	ressi = resendptr1;
	resdi = resptr2;
	regax = resdi->resofs + resdi->ressize;

	for (;;) {
		if (regax <= ressi->resofs) break;
		if (ressi == resendptr2) 
			fatal_error(aResizememoryNo, resmaxsize);
	
		ressi->resunk = 0;
		ressi++;
		resendptr1 = ressi;
	}
	popregs();
	return 0;
}

void far* mmgr_op_unk(unsigned short arg_0, unsigned short arg_2) {
	int i;
	unsigned short regax, regbx, regcx, regdx;
	char* strdi;
	struct MEMCHUNK* ressi;
	struct MEMCHUNK* resdi;

	(void)arg_0;
	regax = arg_2;
	ressi = resptr2;

	for (;;) {
		if (ressi == resptr1)
			fatal_error(aMemoryManagerB, arg_2);
		if (regax == ressi->resofs) break;
		ressi--;
	}

	resdi = ressi;
	resdi--;
	if (resdi->resunk == 0) {
	
		do {
			resdi--;
		} while (resdi->resunk == 0);
	
		ressi->resunk = 0;
		regax = resdi->resofs + resdi->ressize;
		resdi++;
		if (ressi == resptr2) {
			resptr2 = resdi;
		}
	
		resdi->resofs = regax;
		resdi->ressize = ressi->ressize;
		resdi->resunk = 2;
		for (i = 0; i < 0xC; i++) {
			resdi->resname[i] = ressi->resname[i];
		}
		mmgr_copy_paras(ressi->resofs, regax, ressi->ressize);

	} else {
		resdi = ressi;
	}

	return MK_FP(resdi->resofs, 0);
}

void far* locate_resource(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4, unsigned short fatal) {
	unsigned short oldds;
__asm {
    push    ds
    push    si
    push    di
    mov     dx, fatal
    mov oldds, ds
_alt_locate_resource:
    cld
    mov     ds, [arg_2]
    mov     si, [arg_0]
    mov     ax, oldds //seg dseg
    mov     es, ax
    mov     di, [arg_4]
    mov     cx, 4
    mov     bx, 0
loc_30FC7:
    cmp     byte ptr es:[bx+di], 0
    jz      short loc_31024
    inc     bx
    loop    loc_30FC7
loc_30FD0:
    mov     ax, [si+4]
    or      ax, ax
    jz      short _end_of_locate
    add     si, 6
    mov     bx, si
loc_30FDC:
    mov     si, bx
    mov     di, [arg_4]
    mov     cx, 4
loc_30FE4:
    cmpsb
    jnz     short loc_30FEC
    loop    loc_30FE4
    jmp     short _found_resource
    // align 2
    db 144
loc_30FEC:
    cmp     byte ptr [si-1], 0
    jnz     short loc_30FF9
    cmp     byte ptr es:[di-1], 20h // ' '
    jz      short _found_resource
loc_30FF9:
    add     bx, 4
    dec     ax
    jge     short loc_30FDC
_end_of_locate:
    cmp     dx, 1
    jl      short loc_3101F
    jg      short loc_3100C
    mov     dx, offset aLocateshape4_4sShapeNotF// "locateshape - %-4.4s SHAPE NOT FOUND\r\n"
    jmp     short loc_3100F
    // align 2
    db 144
loc_3100C:
    mov     dx, offset aLocatesound4_4sSoundNotF // "locatesound - %-4.4s SOUND NOT FOUND\r\n"
loc_3100F:
    mov     ax, oldds //seg dseg
    mov     ds, ax
    push    [arg_4]
    push    dx
    call    far ptr fatal_error
    int     20h             // DOS - PROGRAM TERMINATION
loc_3101F:
    xor     ax, ax
    jmp     short loc_31075
    // align 2
    db 144
loc_31024:
    mov     byte ptr es:[bx+di], 20h // ' '
    inc     bx
    loop    loc_31024
    jmp     short loc_30FD0
_found_resource:
    mov     si, [arg_0]
    mov     ax, [si+4]
    shl     ax, 1
    shl     ax, 1
    add     bx, ax
    shl     ax, 1
    add     ax, 6
    mov     cx, ds
    xor     dx, dx
    shl     cx, 1
    rcl     dx, 1
    shl     cx, 1
    rcl     dx, 1
    shl     cx, 1
    rcl     dx, 1
    shl     cx, 1
    rcl     dx, 1
    add     ax, cx
    adc     dx, 0
    add     ax, [bx]
    adc     dx, [bx+2]
    mov     bx, ax
smart
    and     bx, 0Fh
nosmart
    shr     dx, 1
    rcr     ax, 1
    shr     dx, 1
    rcr     ax, 1
    shr     dx, 1
    rcr     ax, 1
    shr     dx, 1
    rcr     ax, 1
    mov     dx, ax
    mov     ax, bx
loc_31075:
    pop     di
    pop     si
    pop     ds
}
}

void far* locate_shape_nofatal(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	return locate_resource(arg_0, arg_2, arg_4, 0);
}

void far* locate_shape_fatal(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	return locate_resource(arg_0, arg_2, arg_4, 1);
}

void far* locate_shape_alt(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	return locate_shape_fatal(arg_0, arg_2, arg_4);
}

void far* locate_sound_fatal(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	return locate_resource(arg_0, arg_2, arg_4, 2);
}
