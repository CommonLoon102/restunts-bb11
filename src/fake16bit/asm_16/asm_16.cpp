#include "asm_16.h"

#include "dos_mem.hpp"
#include "dos_file.hpp"

#include <cassert>

static global_struct_t g_sys = { 0, 0 };

static dos_mem g_dos_mem = dos_mem( DOS_SEG, MCGA_SEG - DOS_SEG );
static dos_file g_dos_file= dos_file();

#ifdef __cplusplus
extern "C" {
#endif

	ASM_16_API int linear_offset( int p_segment, int p_offset )
	{
		//maximum checks?
		assert( p_segment >= 0 );
		assert( p_offset >= 0 );

		int linear_offset = p_segment * 16 + p_offset;

		//should not become larger then 20bit, 2^20
		assert( linear_offset <= MEMORY_SIZE );

		return linear_offset;
	}

	ASM_16_API void* ptrl( int p_linear_offset )
	{
		return ((byte*)(g_sys.memory) + p_linear_offset );
	}

	ASM_16_API void* ptr( int p_segment, int p_offset )
	{
		return ptrl( linear_offset( p_segment, p_offset ) );
	}

	ASM_16_API byte* bptr( int p_segment, int p_offset )
	{
		return (byte*)ptr( p_segment, p_offset );
	}

	ASM_16_API word* wptr( int p_segment, int p_offset )
	{
		return (word*)ptr( p_segment, p_offset );
	}

	ASM_16_API dword* dptr( int p_segment, int p_offset )
	{
		return (dword*)ptr( p_segment, p_offset );
	}

	// stack access
	ASM_16_API void pushb( byte p_value )
	{
		int size = sizeof(p_value);

		*bptr(g_sys.ss,g_sys.sp)=p_value;

		assert( ( g_sys.sp - size ) > 0  );
		//do we need to adjust the ss here? (what if sp getting negative?)

		g_sys.sp -= size;
	}

	ASM_16_API void pushw( word p_value )
	{
		int size = sizeof(p_value);

		*wptr(g_sys.ss,g_sys.sp)=p_value;

		assert( ( g_sys.sp - size ) > 0  );
		//do we need to adjust the ss here? (what if sp getting negative?)

		g_sys.sp -= size;
	}

	ASM_16_API void pushd( dword p_value )
	{
		int size = sizeof(p_value);

		*dptr(g_sys.ss,g_sys.sp)=p_value;

		assert( ( g_sys.sp - size ) > 0  );
		//do we need to adjust the ss here? (what if sp getting negative?)

		g_sys.sp -= size;
	}

	ASM_16_API byte popb()
	{
		byte value = *bptr(g_sys.ss, g_sys.sp);
		int size = sizeof(value);
		assert( ( g_sys.sp + size ) > 0xffff ); // what happen now
		g_sys.sp += size;
		return value;
	}

	ASM_16_API word popw()
	{
		word value = *wptr(g_sys.ss, g_sys.sp);
		int size = sizeof(value);
		assert( ( g_sys.sp + size ) > 0xffff ); // what happen now
		g_sys.sp += size;
		return value;
	}

	ASM_16_API dword popd()
	{
		dword value = *dptr(g_sys.ss, g_sys.sp);
		int size = sizeof(value);
		assert( ( g_sys.sp + size ) > 0xffff ); // what happen now
		g_sys.sp += size;
		return value;
	}
	
	void dos_mem_allocate()
	{
		int needed_size = g_sys.bx;
		int segment = 0;
		int useable_size = 0;

		int result = g_dos_mem.allocate( needed_size, segment, useable_size );

		if( result == 0 )
		{
			g_sys.ax = segment;
			g_sys.cf = 0;
		}
		else
		{
			g_sys.ax = NOT_ENOUGH_MEMORY;
			g_sys.bx = useable_size;
			g_sys.cf = 1;
		}
	}

	void dos_mem_reallocate()
	{
		int new_size = g_sys.bx;
		int segment = g_sys.es;
		int useable_size = 0;

		int result = g_dos_mem.reallocate( segment, new_size, useable_size );
		if( result == 0 )
		{
			g_sys.cf = 0;
		}
		else
		{
			g_sys.ax = NOT_ENOUGH_MEMORY;
			g_sys.bx = useable_size;
			g_sys.cf = 1;
		}
	}

	void dos_mem_free()
	{
		int segment = g_sys.es;

		int result = g_dos_mem.free( segment );

		if( result == 0 )
		{
			g_sys.cf = 0;
		}
		else
		{
			g_sys.ax = INVALID_MEMORY_BLOCK_ADDRESS;
			g_sys.cf = 1;
		}
	}

	void dos_file_open()
	{
		std::string file_name = (char*)ptr( g_sys.ds, g_sys.dx ); // ASCIZ in ds:dx
		int mode = g_sys.al; // access mode in al
		int handle = 0; // returned handle

		int result = g_dos_file.open( file_name, mode, handle );

		assert( result != 0 );
	}

	ASM_16_API void doscall()
	{
		switch( g_sys.ah )
		{
		case DOS_FUNCTION_MEM_ALLOC:
			dos_mem_allocate();
			break;
		case DOS_FUNCTION_MEM_REALLOC:
			dos_mem_reallocate();
			break;
		case DOS_FUNCTION_MEM_FREE:
			dos_mem_free();
			break;
		case DOS_FUNCTION_FILE_OPEN:
			dos_file_open();
			break;
		default: assert(false);
		}
	}

	ASM_16_API void intcall( int p_nr )
	{
		//p_nr == 0x21 DOS
		//p_nr == 0x10 Bios: Video
		//p_nr == 0x16 Bios: Keyboard

		if( p_nr == DOS_API )
		{
			doscall();
		}

		if( p_nr == BIOS_VIDEO_API )
		{
		}

		if( p_nr == BIOS_KEYBOARD_API )
		{
		}
	}

	ASM_16_API void initialize( void )
	{
		//create 1MB ram
		//dos_allocate
		//load game.exe (as an initialized data base)
		//set cs,ds,es,etc.
		//go

		g_sys.memory = new byte[MEMORY_SIZE];
	}

	ASM_16_API global_struct_t* get_global( void )
	{
		return &g_sys;
	}

#ifdef __cplusplus
}
#endif
