char* check_pathdrive(char* arg_0) {
	__asm {
		mov	si, [arg_0]
		mov	bx, si
		cld

loc_1EA2_2812:

		lodsb
		or	al, al
		jz	short loc_1EA2_2823
		cmp	al, ':'
		jz	short loc_1EA2_281F
		cmp	al, '\'
		jnz	short loc_1EA2_2812

loc_1EA2_281F:
		mov	bx, si
		jmp	short loc_1EA2_2812
; ---------------------------------------------------------------------------

loc_1EA2_2823:
		mov	ax, bx
	}
}

