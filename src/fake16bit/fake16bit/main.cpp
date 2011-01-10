#include <asm_16/asm_16.h>

#include <iostream>
#include <string>

#include <cassert>
#include <list>

/*
the backend:
-is(should be) an "release" mode dll - to gain max. speed for stuff that is always the same
-holds the "memory" which is used as an 1MB dos lower memory fake (all seg:ofs accesses are directed into this one)
-a stack that also uses the 1MB fake

missing (im working on that)
-dos file access
-some keyboard, video bios functions
-redirect port access
-(will) load the game.exe into the 1MB fake - so there is inital no need for faking the data-content - you just use the linear ida offset to access the variable
-some asm commands like jnz(), jmp, call etc.

[transform]

proc sub_1234
  mov dx,seg31
  mov ds,dx
  mov [word ptr ds:bx+10],23
  mov ax,[ds:bx+20]
  mov ax,0
  mov bx,10
  mov [word123],10
  cmp ax,0
  mul bx
  mul bh
  jnz lableX
  mov ax,bx
lableX:
  call sub_4321
  ret
endp

[into]

word* word123 = (word*)ptrl( linear-ida-offset );

void sub_1234()
{
  dx = seg31;
  dx = ds;
  *wptr(ds,bx+10)=23;
  ax = *wptr(ds,bx+20);
  ax = 0;
  bx = 10;
  *word123 = 10;
  cmp(ax,0);
  mul16(bx); // dx:ax = ax * bx --> dxax() = ax * bx; set flags???
  mul8(bh); // ax = al * bh; --> ax() = al() * bh(); set flags???
  if(jnz()) goto lableX;
  ax = bx;
lableX:
  sub_4321();
  return;
}

benefit?

you can mix 16bit asm semantic with 32bit C/C++ code :)

mul bl                          ;8-Bit-Multiplication, Result in BX
mul [MyWord]                    ;16-Bit-Multiplication, Result in DX:AX
mul edx                         ;32-Bit-Multiplication, Result in EDX:EAX

*/

int main( int argc, char* argv[] )
{
	initialize();

	global_struct_t& x = *get_global();

	x.ax = 100;
	subw( x.ax, 20 );

	x.bx = 1;
	x.ah = DOS_FUNCTION_MEM_ALLOC;
	intcall(DOS_API);
	if( !x.cf )
	{
		int seg = x.ax;
		word* yyy = wptr( seg, 0 );

		*yyy=100;
		subw( *yyy, 20 );
		assert( *yyy == 80 );

		int ddd=1;

		x.ah = DOS_FUNCTION_MEM_FREE;
		x.es = seg;
		intcall(DOS_API);
		assert( x.cf == 0 );
	}

	//dos mem test

	x.bx = 0xFFFF;
	x.ah = DOS_FUNCTION_MEM_ALLOC;
	intcall(DOS_API);

	if( x.cf && x.ax == NOT_ENOUGH_MEMORY )
	{
		x.ah = DOS_FUNCTION_MEM_ALLOC;
		intcall(DOS_API);

		int seg = x.ax;

		x.es = seg;
		x.bx = 100;
		x.ah = DOS_FUNCTION_MEM_REALLOC;
		intcall(DOS_API);

		int brk = 1;

		x.ah = DOS_FUNCTION_MEM_ALLOC;
		x.bx = 100;
		intcall(DOS_API);

		int seg2 = x.ax;

		x.es = seg;
		x.bx = 0xFFFF;
		x.ah = DOS_FUNCTION_MEM_REALLOC;
		intcall(DOS_API);

		brk = 1;

		x.ah = DOS_FUNCTION_MEM_FREE;
		x.es = seg;
		intcall(DOS_API);

		brk = 1;

		x.ah = DOS_FUNCTION_MEM_FREE;
		x.es = seg;
		intcall(DOS_API);

		brk = 1;
	}

	int brk = 1;

	return 0;
}