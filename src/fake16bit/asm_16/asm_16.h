#ifndef ASM_16_H_INCLUDE
#define ASM_16_H_INCLUDE

const int DOS_SEG = 0x60;
const int MCGA_SEG = 0xa000;
const int MEMORY_SIZE = 0x100000; // 2^20
const int PARAGRAPH_SIZE = 0x10;

const int DOS_API = 0x21;

const int DOS_FUNCTION_MEM_ALLOC = 0x48;
const int DOS_FUNCTION_MEM_FREE = 0x49;
const int DOS_FUNCTION_MEM_REALLOC = 0x4a;

//TODO
const int DOS_FUNCTION_FILE_CREATE = 0x3c; // http://stanislavs.org/helppc/int_21-3c.html
const int DOS_FUNCTION_FILE_OPEN = 0x3d; // http://stanislavs.org/helppc/int_21-3d.html
const int DOS_FUNCTION_FILE_CLOSE = 0x3e; // http://stanislavs.org/helppc/int_21-3e.html
const int DOS_FUNCTION_FILE_READ = 0x3f; // http://stanislavs.org/helppc/int_21-3f.html
const int DOS_FUNCTION_FILE_WRITE = 0x40; // http://stanislavs.org/helppc/int_21-40.html
const int DOS_FUNCTION_FILE_SEEK = 0x42; // http://stanislavs.org/helppc/int_21-42.html
const int DOS_FUNCTION_FILE_UNLINK = 0x41; // http://stanislavs.org/helppc/int_21-41.html
//
//const int DOS_FUNCTION_SET_DTA = 0x1a; // http://stanislavs.org/helppc/int_21-1a.html
//const int DOS_FUNCTION_FILE_FIND = 0x4e; // http://stanislavs.org/helppc/int_21-4e.html
//const int DOS_FUNCTION_FILE_FIND_NEXT = 0x4f; // http://stanislavs.org/helppc/int_21-4f.html

const int NOT_ENOUGH_MEMORY = 0x08;
const int INVALID_MEMORY_BLOCK_ADDRESS = 0x09;

const int BIOS_VIDEO_API = 0x10;
const int BIOS_KEYBOARD_API = 0x16;

#ifdef ASM_16_EXPORTS
#   define ASM_16_API __declspec(dllexport)
#else
#   define ASM_16_API __declspec(dllimport)
#endif

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

  //union _REGS {             
  //  struct _WORDREGS x;     
  //  struct _BYTEREGS h;     
  //};

  //struct _WORDREGS {        
  //  unsigned int ax;
  //  unsigned int bx;
  //  unsigned int cx;
  //  unsigned int dx;
  //  unsigned int si;
  //  unsigned int di;
  //  unsigned int cflag;
  //};

  //struct _BYTEREGS {        
  //  unsigned char al, ah;
  //  unsigned char bl, bh;
  //  unsigned char cl, ch;
  //  unsigned char dl, dh;
  //};

typedef struct global_struct_s
{
    word ax;
	word bx;
	word cx;
	word dx;

	word cs;
	word ds;
	word ss;
	word es;

	word di;
	word si;

	word ip;
	word sp;
	word bp;

    void* memory;

	// TODO
	byte cf;
	byte ah;
	byte al;

} global_struct_t;

#ifdef __cplusplus
extern "C" {
#endif

// tools
ASM_16_API int linear_offset( int p_segment, int p_offset );

// memory access
ASM_16_API void* ptrl( int p_linear_offset );
ASM_16_API void* ptr( int p_segment, int p_offset );
ASM_16_API byte* bptr( int p_segment, int p_offset );
ASM_16_API word* wptr( int p_segment, int p_offset );
ASM_16_API dword* dptr( int p_segment, int p_offset );

//mulb --> ax, mulw --> dx:ax
//dword value = rdxax(); (dx:ax)
//wdxax( value ) --> dx:ax

// stack access
ASM_16_API void pushb( byte p_value );
ASM_16_API void pushw( word p_value );
ASM_16_API void pushd( dword p_value );

ASM_16_API byte popb();
ASM_16_API word popw();
ASM_16_API dword popd();

ASM_16_API void intcall( int p_nr ); // 10h, 16h, 21h

ASM_16_API void doscall(); // 21h

ASM_16_API void initialize( void );

// globals
ASM_16_API global_struct_t* get_global( void );

//http://unixwiz.net/techtips/x86-jumps.html
//http://ref.x86asm.net/coder32.html
//http://www.strchr.com/machine_code_redundancy

//for segment override (0x26) or other prefixes
//http://code.google.com/p/distorm/wiki/x86_x64_Machine_Code 

/*
    [lahf] => 1 http://www.fermi.mn.it/linux/quarta/x86/lahf.htm
    [jno] => 1
    [pushf] => 1 http://www.fermi.mn.it/linux/quarta/x86/pushf.htm // void pushf(){ pushw( flags ); }
    [xlat] => 1 http://www.fermi.mn.it/linux/quarta/x86/xlat.htm
    [nop] => 1 // void nop(){}
    [std] => 2 // void std(){ flags.df = 1; }
    [sahf] => 2
    [cmps] => 3
    [iret] => 3 // return;
    [clc] => 5 // void clc(){ flags.cf = 0; }
    [js] => 5  // bool js(){ return flags.sf == 1; }
    [jcxz] => 7
    [jo] => 7 // bool jo(){ return flags.of == 1; }
    [stc] => 8 // void std(){ flags.cf = 1; }
    [lds] => 8
    [jns] => 10 // bool jns(){ return !js() }
    [rol] => 10
    [cli] => 15 // void cli(){}
    [not] => 17
    [sti] => 18 // void sti(){}
    [in] => 19
    [retn] => 27 // return;
    [scas] => 28 // repne_scasb(){ cx,... }
    [out] => 30
    [idiv] => 36
    [div] => 46
    [sbb] => 47
    [cld] => 54 // void cld(){ flags.df = 0; }
    [ja] => 56 // bool ja(){ return ( ( flags.cf == 0 ) && ( flags.zf == 0 ) ); }
    [mul] => 60
    [rcl] => 69
    [lods] => 71 http://www.fermi.mn.it/linux/quarta/x86/lods.htm
    [int] => 75 void int(){ MAPPING }
    [xchg] => 77
    [jbe] => 81 bool jbe(){ return ( ( flags.cf == 1 ) || ( flags.zf == 1 ) ); }
    [stos] => 101
    [jnb] => 119
    [loop] => 133
    [jb] => 144 bool jb(){ return flags.cf == 1; }
    [imul] => 145
    [and] => 159
    [adc] => 160
    [shr] => 161
    [rcr] => 171
    [test] => 201
    [neg] => 228
    [sar] => 233
    [jg] => 245
    [cwd] => 294 void cwd(){ set_dxax( ax ); }
    [xor] => 318
    [jl] => 360
    [jge] => 360
    [movs] => 396
    [jle] => 399
    [les] => 411
    [dec] => 478
    [or] => 625
    [cbw] => 676 // void cbw(){ regs.ax = regs.al; } (no eax in use)
    [lea] => 678
    [retf] => 691
    [inc] => 827
    [jnz] => 1413
    [jz] => 1527
    [shl] => 1539 ... shlw cl,r/m16 --> shl cl,ax --> shl_r16(cl,ax);
    [pop] => 1796
    [sub] => 2331
    [jmp] => 2653
    [call] => 3066
    [cmp] => 3461
    [add] => 4836
    [push] => 9437
    [mov] => 20408

	cmp ax,0
	jnz lableX
	ret

	cmp(ax,0);
	if(jnz()) goto lableX;
	return;
*/

#ifdef __cplusplus
}
#endif


#endif // ASM_16_H_INCLUDE
