#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../c/platform.h"

static legacy_u8 external_data[32];

int main(void)
{
	legacy_u16 segment = dos_memory_allocate(100);
	assert(segment != 0);
	legacy_u8 *arena = dos_memory_make_pointer(segment, 0);
	legacy_u8 *crossing = arena + 65530;
	memset(crossing, 0x73, 32);
	assert(*(legacy_u8 *)dos_memory_make_pointer(segment + 4096, 0) == 0x73);
	for (unsigned int index = 0; index < 32; index++) {
		void *address = crossing + index;
		assert(dos_memory_make_pointer(dos_memory_pointer_segment(address),
									   dos_memory_pointer_offset(address)) == address);
	}
	assert(dos_memory_make_pointer(dos_memory_pointer_segment(external_data),
								   dos_memory_pointer_offset(external_data)) == external_data);
	legacy_u8 *vga = dos_memory_make_pointer(0xA000, 0);
	vga[63999] = 0xC8;
	assert(*(legacy_u8 *)dos_memory_make_pointer(0xA000, 63999) == 0xC8);
	assert(dos_memory_pointer_segment(NULL) == 0);
	assert(dos_memory_make_pointer(0, 0) == NULL);

	const legacy_s8 *name = (const legacy_s8 *)"CaSe.TST";
	legacy_u16 handle = dos_file_open(name, DOS_FILE_CREATE);
	assert(handle == 5);
	assert(dos_file_write(handle, "hello", 5) == 5);
	assert(dos_file_close(handle) == 0);
	handle = dos_file_open((const legacy_s8 *)".\\case.tst", DOS_FILE_OPEN_EXISTING);
	assert(handle == 5);
	char contents[8] = {0};
	assert(dos_file_read(handle, contents, sizeof(contents)) == 5);
	assert(strcmp(contents, "hello") == 0);
	assert(dos_file_error() == 0);
	assert(dos_file_seek(handle, -2, DOS_FILE_SEEK_END) == 0);
	assert(dos_file_tell(handle) == 3);
	assert(dos_file_read(handle, contents, 2) == 2);
	assert(contents[0] == 'l' && contents[1] == 'o');
	assert(dos_file_close(handle) == 0);
	assert(dos_file_read(handle, contents, 2) == 0);
	assert(dos_file_error() != 0);
	assert(dos_file_error() == 0);
	const legacy_s8 *found = dos_file_find_first((const legacy_s8 *)"*.tst");
	/* DOS may expose uppercase 8.3 names. The enumeration must return a
	 * usable matching name regardless of the filesystem's spelling. */
	assert(found != NULL);
	handle = dos_file_open(found, DOS_FILE_OPEN_EXISTING);
	assert(handle == 5);
	assert(dos_file_read(handle, contents, 5) == 5);
	assert(memcmp(contents, "hello", 5) == 0);
	assert(dos_file_close(handle) == 0);
	assert(dos_file_find_next() == NULL);
	assert(dos_file_remove((const legacy_s8 *)"CASE.tst") == 0);
	assert(dos_file_open(name, DOS_FILE_OPEN_EXISTING) == 0);
	assert(dos_file_error() != 0);
	puts("SDL3 memory and file regression tests passed");
	return 0;
}
