#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../c/platform.h"

static legacy_u8 external_data[32];

static void test_update_streams(void)
{
	static const legacy_s8 *names[] = {(const legacy_s8 *)"UPDATE0.TST",
									   (const legacy_s8 *)"UPDATE1.TST"};
	legacy_u16 handles[2];
	char contents[16];
	for (legacy_u32 index = 0; index < 2; index++) {
		handles[index] = dos_file_open(names[index], DOS_FILE_CREATE);
		assert(handles[index] != 0);
		assert(dos_file_write(handles[index], "0123456789", 10) == 10);
	}
	assert(handles[0] != handles[1]);
	for (legacy_u32 index = 0; index < 2; index++) {
		assert(dos_file_read(handles[index], contents, 1) == 0);
		assert(dos_file_tell(handles[index]) == 10);
		assert(dos_file_error() == 0);
		assert(dos_file_seek(handles[index], 0, DOS_FILE_SEEK_BEGIN) == 0);
		assert(dos_file_read(handles[index], contents, 3) == 3);
		assert(memcmp(contents, "012", 3) == 0);
	}
	/* DOS handles allow direction changes at the current position. Exercise
	 * each transition on both files before the next to check independent state. */
	for (legacy_u32 index = 0; index < 2; index++) {
		assert(dos_file_write(handles[index], "XY", 2) == 2);
		assert(dos_file_tell(handles[index]) == 5);
	}
	for (legacy_u32 index = 0; index < 2; index++) {
		assert(dos_file_read(handles[index], contents, 5) == 5);
		assert(memcmp(contents, "56789", 5) == 0);
		assert(dos_file_tell(handles[index]) == 10);
	}
	/* Reading exactly to the end need not set the stream's EOF indicator. */
	for (legacy_u32 index = 0; index < 2; index++) {
		assert(dos_file_write(handles[index], "END", 3) == 3);
		assert(dos_file_tell(handles[index]) == 13);
	}
	for (legacy_u32 index = 0; index < 2; index++) {
		assert(dos_file_read(handles[index], contents, 1) == 0);
		assert(dos_file_error() == 0);
		assert(dos_file_close(handles[index]) == 0);
	}
	for (legacy_u32 index = 0; index < 2; index++) {
		legacy_u16 handle = dos_file_open(names[index], DOS_FILE_OPEN_EXISTING);
		assert(handle != 0);
		assert(dos_file_read(handle, contents, sizeof(contents)) == 13);
		assert(memcmp(contents, "012XY56789END", 13) == 0);
		assert(dos_file_error() == 0);
		assert(dos_file_close(handle) == 0);
		assert(dos_file_remove(names[index]) == 0);
	}
}

int main(void)
{
	legacy_u16 segment = dos_memory_allocate(100);
	assert(segment != 0);
	legacy_u8 *arena = dos_memory_make_pointer(segment, 0);
	legacy_u8 *crossing = arena + 65530;
	memset(crossing, 0x73, 32);
	assert(*(legacy_u8 *)dos_memory_make_pointer(segment + 4096, 0) == 0x73);
	for (legacy_u32 index = 0; index < 32; index++) {
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
	test_update_streams();
	puts("SDL3 memory and file regression tests passed");
	return 0;
}
