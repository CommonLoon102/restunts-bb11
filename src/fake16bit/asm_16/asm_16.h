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
//const int DOS_FUNCTION_FILE_OPEN = 0x3d; // http://stanislavs.org/helppc/int_21-3d.html
//const int DOS_FUNCTION_FILE_CLOSE = 0x3e; // http://stanislavs.org/helppc/int_21-3e.html
//const int DOS_FUNCTION_FILE_READ = 0x3f; // http://stanislavs.org/helppc/int_21-3f.html
//const int DOS_FUNCTION_FILE_WRITE = 0x40; // http://stanislavs.org/helppc/int_21-40.html
//const int DOS_FUNCTION_FILE_SEEK = 0x42; // http://stanislavs.org/helppc/int_21-42.html
//const int DOS_FUNCTION_FILE_UNLINK = 0x41; // http://stanislavs.org/helppc/int_21-41.html
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

#ifdef __cplusplus
}
#endif


#endif // ASM_16_H_INCLUDE
