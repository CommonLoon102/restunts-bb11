#include <cassert>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <iostream>

#include "types.hpp"
#include "emulator.hpp"
#include "emulator_test.hpp"

void check_type_size()
{
	assert( sizeof( byte_t ) == 1 );
	assert( sizeof( word_t ) == 2 );
	assert( sizeof( dword_t ) == 4 );
}

int main()
{
	emulator_t e;

	//emulator_test_t et( e );
	//et.test();

	//e.MOVb( e.DS, e.BX, 0x00 );

	//some "unit" tests

	e.AL = -127;
	e.CBW();
	short sAX = e.AX;
	assert( sAX == -127 );

	e.AX = 0x1;
	e.ROL( e.AX, 2 );
	assert( e.AX == 4 );

	e.AX = 20;
	e.BX = 10;
	e.CMP( e.AX, e.BX );
	bool jg = e.JG();
	bool jz = e.JZ();
	bool jl = e.JL();
	bool jle = e.JLE();

	e.AX = 0x1234;
	e.BX = 0x4321;
	e.AND( e.AX, e.BX );
	assert( word_t( 0x1234 & 0x4321 ) == e.AX );

	e.AX = 0x1234;
	e.BX = 0x4321;
	e.XOR( e.AX, e.BX );
	assert( word_t( 0x1234 ^ 0x4321 ) == e.AX );

	e.AX = 0x1234;
	e.BX = 0x4321;
	e.XCHG( e.AX, e.BX );
	assert( e.AX == 0x4321 && e.BX == 0x1234 );

	e.AL = 0x12;
	e.BL = 0x43;
	e.XCHG( e.AL, e.BL );
	assert( e.AL == 0x43 && e.BL == 0x12 );

	e.AH = 10;
	e.SUB( e.AH, 20 );
	assert( e.AH == byte_t( 10 - 20) );
	char ah = e.AH;

	e.AX = 1000;
	e.SUB( e.AX, 20 );
	assert( e.AX == word_t( 1000 - 20) );
	short ax = e.AX;

	e.AX = 1000;
	e.ADD( e.AX, 0xfffb ); // e.AX + word_t(-5)
	assert( e.AX == word_t( 1000 - 5 ) );

	e.AX = 6400;
	e.BX = 100;
	e.MUL( e.BX );
	assert( e.DXAX == dword_t( 6400 * 100 ) );
	dword_t valyyy = e.DXAX;

	e.AX = 6400;
	e.BX = 100;
	e.IMUL( e.BX );
	assert( e.DXAX == dword_t( 6400 * 100 ) );
	dword_t valxxx = e.DXAX;

	e.AH = 10;
	e.ADD( e.AH, 20 );
	assert( e.AH == byte_t( 10 + 20) );

	e.AX = 1000;
	e.ADD( e.AX, 20 );
	assert( e.AX == word_t( 1000 + 20) );

	e.AX = 101;
	e.IDIV( byte_t( 10 ) );
	byte_t xah = e.AH;
	byte_t xal = e.AL;
	assert( byte_t( 101 % 10 ) == xah );
	assert( byte_t( 101 / 10 ) == xal );
	int brk = 1;

	e.DXAX = 10001;
	e.IDIV( word_t( 1000 ) );
	word_t xdx = e.DX;
	word_t xax = e.AX;
	assert( word_t( 10001 % 1000 ) == xdx );
	assert( word_t( 10001 / 1000 ) == xax );
	int brk5 = 1;

	return 0;
}