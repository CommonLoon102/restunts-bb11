#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "../platform/sdl3/sdl3.h"
#include "../c/fileio.h"
#include "../c/highscore.h"
#include "../c/platform.h"

#define TEST_GUARD_BYTES 19U
#define TEST_GUARD_VALUE 0xa7U
#define TEST_LARGE_FILE_BYTES 98341U

static void fill_pattern(legacy_u8 *data, size_t length)
{
	for (size_t index = 0; index < length; index++) {
		data[index] = (legacy_u8)(index * 37U + (index >> 9U) + (index >> 16U));
	}
}

static void check_file_contents(const char *path, const legacy_u8 *expected, size_t length)
{
	FILE *file = fopen(path, "rb");
	assert(file != NULL);
	legacy_u8 buffer[2048];
	while (length != 0) {
		size_t count = length < sizeof(buffer) ? length : sizeof(buffer);
		assert(fread(buffer, 1, count, file) == count);
		assert(memcmp(buffer, expected, count) == 0);
		expected += count;
		length -= count;
	}
	assert(fgetc(file) == EOF);
	assert(!ferror(file));
	assert(fclose(file) == 0);
}

static void check_guards(const legacy_u8 *allocation, size_t length)
{
	for (size_t index = 0; index < TEST_GUARD_BYTES; index++) {
		assert(allocation[index] == TEST_GUARD_VALUE);
		assert(allocation[TEST_GUARD_BYTES + length + index] == TEST_GUARD_VALUE);
	}
}

static void test_highscore_stack_buffers(void)
{
	struct HIGHSCORE_ENTRY scores[HIGHSCORE_ENTRY_COUNT];
	struct {
		legacy_u8 before[TEST_GUARD_BYTES];
		struct HIGHSCORE_ENTRY scores[HIGHSCORE_ENTRY_COUNT];
		legacy_u8 after[TEST_GUARD_BYTES];
	} loaded;
	fill_pattern((legacy_u8 *)scores, sizeof(scores));
	memset(&loaded, TEST_GUARD_VALUE, sizeof(loaded));
	assert(sizeof(scores) == HIGHSCORE_TABLE_SIZE_BYTES);
	assert(dos_memory_make_pointer(dos_memory_pointer_segment(scores),
								   dos_memory_pointer_offset(scores)) == scores);
	assert(file_write_fatal((const legacy_s8 *)"casedir/ScOrE.HiG", scores, sizeof(scores)) == 0);
	check_file_contents("CaSeDir/ScOrE.HiG", (const legacy_u8 *)scores, sizeof(scores));
	const legacy_s8 *found = file_find((const legacy_s8 *)"CASEDIR/*.hig");
	assert(found != NULL);
	assert(file_read_nofatal(found, loaded.scores) == loaded.scores);
	assert(memcmp(scores, loaded.scores, sizeof(scores)) == 0);
	for (size_t index = 0; index < TEST_GUARD_BYTES; index++) {
		assert(loaded.before[index] == TEST_GUARD_VALUE);
		assert(loaded.after[index] == TEST_GUARD_VALUE);
	}
	assert(file_find_next() == NULL);
	assert(dos_file_remove((const legacy_s8 *)"CaSeDiR\\score.hig") == 0);
}

static void test_heap_buffers(void)
{
	static const size_t lengths[] = {0,		1,	   16383, 16384,
									 16385, 65535, 65536, TEST_LARGE_FILE_BYTES};
	legacy_u8 *source_allocation = malloc(TEST_LARGE_FILE_BYTES + TEST_GUARD_BYTES * 2U);
	legacy_u8 *destination_allocation = malloc(TEST_LARGE_FILE_BYTES + TEST_GUARD_BYTES * 2U);
	assert(source_allocation != NULL && destination_allocation != NULL);
	legacy_u8 *source = source_allocation + TEST_GUARD_BYTES;
	legacy_u8 *destination = destination_allocation + TEST_GUARD_BYTES;
	for (size_t index = 0; index < sizeof(lengths) / sizeof(lengths[0]); index++) {
		size_t length = lengths[index];
		memset(source_allocation, TEST_GUARD_VALUE, length + TEST_GUARD_BYTES * 2U);
		memset(destination_allocation, TEST_GUARD_VALUE, length + TEST_GUARD_BYTES * 2U);
		fill_pattern(source, length);
		assert(file_write_fatal((const legacy_s8 *)"casedir\\BuFfEr.BiN", source,
								(legacy_u32)length) == 0);
		check_file_contents("CaSeDir/BuFfEr.BiN", source, length);
		assert(file_read_nofatal((const legacy_s8 *)"CASEDIR/buffer.bin", destination) ==
			   destination);
		assert(memcmp(source, destination, length) == 0);
		check_guards(source_allocation, length);
		check_guards(destination_allocation, length);
		assert(dos_file_remove((const legacy_s8 *)"casedir/BUFFER.BIN") == 0);
	}
	free(source_allocation);
	free(destination_allocation);
}

static void write_track_fixture(const legacy_u8 *bytes, size_t length)
{
	FILE *file = fopen("CaSeDir/MiXeD.tRk", "wb");
	assert(file != NULL);
	assert(fwrite(bytes, 1, length, file) == length);
	assert(fclose(file) == 0);
}

static void test_track_lightmap_identity(void)
{
	legacy_u8 bytes[1807] = {0};
	legacy_u8 elements[901];
	legacy_u8 terrain[901];
	for (legacy_u32 index = 0; index < 900; index++) {
		bytes[index] = (legacy_u8)(index % 180);
		bytes[901 + index] = (legacy_u8)(index % 9);
	}
	bytes[17] = 182;
	bytes[18] = 252;
	bytes[19] = 253;
	bytes[20] = 255;
	bytes[900] = 3;
	bytes[1801] = 2;
	memcpy(bytes + 1802, "EXTRA", 5);
	memcpy(elements, bytes, sizeof(elements));
	memcpy(terrain, bytes + 901, sizeof(terrain));
	elements[17] = elements[18] = 4;
	/* These independently computed digests cover the original bytes, before
	 * track_setup normalizes unsupported element IDs to ordinary road. */
	const legacy_u8 expected[16] = {0xD4, 0xC1, 0x25, 0xB0, 0xD2, 0xC4, 0x40, 0x30,
									0x64, 0x13, 0x6E, 0x42, 0x36, 0x86, 0xC2, 0x01};
	const legacy_u8 extended[16] = {0xA9, 0x3A, 0x53, 0x38, 0x02, 0xB2, 0x3B, 0x59,
									0x66, 0xF3, 0xB6, 0x1D, 0x59, 0x00, 0x0D, 0xE3};
	legacy_u8 digest[16];
	char path[256];
	write_track_fixture(bytes, 1802);
	assert(dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
								   elements, terrain, path, sizeof(path), digest));
	assert(strcmp(path, "CaSeDir/MiXeD.LMP") == 0);
	assert(memcmp(digest, expected, sizeof(digest)) == 0);

	/* A case-sensitive host must reuse an existing differently cased sibling
	 * instead of creating two lightmaps for the same DOS track name. */
	FILE *cache = fopen("CaSeDir/mIxEd.lMp", "wb");
	assert(cache != NULL && fclose(cache) == 0);
	assert(dos_track_lightmap_path((const legacy_s8 *)"CASEDIR", (const legacy_s8 *)"MIXED",
								   elements, terrain, path, sizeof(path), digest));
	assert(strcmp(path, "CaSeDir/mIxEd.lMp") == 0);
	assert(memcmp(digest, expected, sizeof(digest)) == 0);

	elements[0] ^= 1;
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
									elements, terrain, path, sizeof(path), digest));
	elements[0] ^= 1;
	terrain[123] ^= 1;
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
									elements, terrain, path, sizeof(path), digest));
	terrain[123] ^= 1;
	legacy_s8 unterminated_name[9];
	legacy_s8 unterminated_directory[81];
	memset(unterminated_name, 'X', sizeof(unterminated_name));
	memset(unterminated_directory, 'X', sizeof(unterminated_directory));
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", unterminated_name, elements,
									terrain, path, sizeof(path), digest));
	assert(!dos_track_lightmap_path(unterminated_directory, (const legacy_s8 *)"mixed", elements,
									terrain, path, sizeof(path), digest));
	struct {
		char path[4];
		legacy_u8 guard[8];
	} small;
	memset(&small, TEST_GUARD_VALUE, sizeof(small));
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
									elements, terrain, small.path, sizeof(small.path), digest));
	for (legacy_u32 index = 0; index < sizeof(small.guard); index++) {
		assert(small.guard[index] == TEST_GUARD_VALUE);
	}
	write_track_fixture(bytes, 1801);
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
									elements, terrain, path, sizeof(path), digest));
	write_track_fixture(bytes, sizeof(bytes));
	assert(dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
								   elements, terrain, path, sizeof(path), digest));
	assert(memcmp(digest, extended, sizeof(digest)) == 0);
	assert(remove("CaSeDir/MiXeD.tRk") == 0);
	assert(!dos_track_lightmap_path((const legacy_s8 *)"casedir", (const legacy_s8 *)"mixed",
									elements, terrain, path, sizeof(path), digest));
	assert(remove("CaSeDir/mIxEd.lMp") == 0);
}

int main(void)
{
	sdl3_batch_mode = 1;
#ifdef _WIN32
	legacy_s32 result = _mkdir("CaSeDir");
#else
	legacy_s32 result = mkdir("CaSeDir", 0700);
#endif
	assert(result == 0 || errno == EEXIST);
	test_highscore_stack_buffers();
	test_heap_buffers();
	test_track_lightmap_identity();
	assert(rmdir("CaSeDir") == 0);
	puts("SDL3 core file I/O stack, heap, page-boundary and path regressions passed");
	return 0;
}
