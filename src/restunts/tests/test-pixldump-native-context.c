#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../c/legacy.h"

#define RESTUNTS_SDL3 1

static const legacy_char *test_psp_setting;
static const legacy_char *test_path_setting;

static legacy_char *test_getenv(const legacy_char *name)
{
	if (strcmp(name, "RESTUNTS_ORACLE_PSP_SEGMENT") == 0) {
		return (legacy_char *)test_psp_setting;
	}
	assert(strcmp(name, "RESTUNTS_ORACLE_PROGRAM_PATH") == 0);
	return (legacy_char *)test_path_setting;
}

#define getenv test_getenv
#include "../pixldump/legacy_context.c"
#undef getenv
#undef strcmp

static struct SHAPE2D test_shape_headers[3];
static struct SPRITE test_sprites[3];
static legacy_u8 test_resources[3];

struct SPRITE far *mouse_small_sprite = &test_sprites[0];
struct SPRITE far *mouse_medium_sprite = &test_sprites[1];
struct SPRITE far *mouse_background_sprite = &test_sprites[2];
void far *mainresptr = &test_resources[0];
void far *fontdefptr = &test_resources[1];
void far *fontnptr = &test_resources[2];

legacy_u16 file_paras_fatal(const legacy_s8 *filename)
{
	assert(strcmp((const legacy_char *)filename, "pc15.drv") == 0);
	return 140U;
}

legacy_u16 mmgr_get_chunk_size(legacy_s8 far *pointer)
{
	for (legacy_u16 index = 0; index < 3U; index++) {
		if ((void *)pointer == &test_shape_headers[index]) {
			return 12U;
		}
	}
	if ((void *)pointer == mainresptr) {
		return 94U;
	}
	if ((void *)pointer == fontdefptr) {
		return 115U;
	}
	assert((void *)pointer == fontnptr);
	return 91U;
}

legacy_int main(void)
{
	legacy_s8 *arguments[] = {(legacy_s8 *)"/unrelated/native/build/pixldump",
							  (legacy_s8 *)"my0000", (legacy_s8 *)"2", (legacy_s8 *)"0",
							  (legacy_s8 *)"170"};
	for (legacy_u16 index = 0; index < 3U; index++) {
		test_sprites[index].sprite_bitmapptr = &test_shape_headers[index];
	}
	assert(pixldump_legacy_load_segment() == 0x029e);
	assert(pixldump_legacy_polygon_code_segment() == 0x1774);
	assert(pixldump_legacy_polyinfo_segment() == 0x3e61);
	assert((legacy_u16)pixldump_legacy_argv_si(4, arguments) == 0xcc38);
	assert((legacy_u16)pixldump_legacy_argv_si(5, arguments) == 0xcc32);
	arguments[0] = (legacy_s8 *)"C:\\another\\pixldump.exe";
	assert((legacy_u16)pixldump_legacy_argv_si(5, arguments) == 0xcc32);

	test_psp_setting = "0x029e";
	assert(pixldump_legacy_load_segment() == 0x02ae);
	assert(pixldump_legacy_polyinfo_segment() == 0x3e71);
	test_path_setting = "C:\\GAMES\\STUNTS\\PIXLDUMP.EXE";
	assert((legacy_u16)pixldump_legacy_argv_si(5, arguments) == 0xcc26);
	test_psp_setting = "65536";
	assert(pixldump_legacy_load_segment() == 0x029e);
	test_psp_setting = "invalid";
	assert(pixldump_legacy_load_segment() == 0x029e);
	test_psp_setting = "0";
	assert(pixldump_legacy_load_segment() == 0x029e);
	return 0;
}
