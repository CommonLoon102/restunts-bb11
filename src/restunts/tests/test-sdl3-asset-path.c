#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include "../c/asset_path.h"

#define TEST_PROGRAM "restunts-test"
#define TEST_DIRECTORY_MODE (S_IRUSR | S_IWUSR | S_IXUSR)
#define TEST_FILE_MODE (S_IRUSR | S_IWUSR)
#define TEST_EXECUTABLE_MODE (TEST_FILE_MODE | S_IXUSR)

static const legacy_char *sdl_base_path;

static const legacy_char *test_sdl_base_path(void)
{
	return sdl_base_path;
}

/* Exercise the same fallback on hosts where SDL normally knows the path. */
#define SDL_GetBasePath test_sdl_base_path
#include "../platform/sdl3/asset_path.c"
#undef SDL_GetBasePath

static void write_executable(const legacy_char *path, legacy_s32 executable)
{
	FILE *file = fopen(path, "wb");
	assert(file != NULL);
	assert(fclose(file) == 0);
	assert(chmod(path, executable ? TEST_EXECUTABLE_MODE : TEST_FILE_MODE) == 0);
}

static void test_after_directory_change(const legacy_char *executable, const legacy_char *expected,
										const legacy_char *data_directory)
{
	legacy_char *original_directory = realpath(".", NULL);
	assert(original_directory != NULL);
	asset_path_initialize(executable);
	assert(chdir(data_directory) == 0);
	assert(asset_path_base() != NULL);
	assert(strcmp(asset_path_base(), expected) == 0);
	/* Reinitialization cannot replace the captured path after --data-dir. */
	asset_path_initialize("missing-program");
	assert(strcmp(asset_path_base(), expected) == 0);
	assert(chdir(original_directory) == 0);
	free(original_directory);
	asset_path_cleanup();
}

legacy_int main(void)
{
	legacy_char *original_directory = realpath(".", NULL);
	const legacy_char *environment_path = getenv("PATH");
	legacy_char *original_path = environment_path != NULL ? strdup(environment_path) : NULL;
	assert(original_directory != NULL);
	assert(environment_path == NULL || original_path != NULL);
	legacy_char temporary[] = "/tmp/restunts asset path-XXXXXX";
	assert(mkdtemp(temporary) != NULL);
	assert(chdir(temporary) == 0);
	assert(mkdir("bin", TEST_DIRECTORY_MODE) == 0);
	assert(mkdir("data", TEST_DIRECTORY_MODE) == 0);
	assert(mkdir("blocked", TEST_DIRECTORY_MODE) == 0);
	write_executable("bin/" TEST_PROGRAM, 1);
	write_executable("blocked/" TEST_PROGRAM, 0);
	assert(symlink("bin/" TEST_PROGRAM, "linked-program") == 0);
	legacy_char *root = realpath(".", NULL);
	assert(root != NULL);
	legacy_char expected[PATH_MAX];
	legacy_char absolute_executable[PATH_MAX];
	legacy_char data_directory[PATH_MAX];
	assert(snprintf(expected, sizeof(expected), "%s/bin/", root) < (legacy_s32)sizeof(expected));
	assert(snprintf(absolute_executable, sizeof(absolute_executable), "%s/bin/%s", root,
					TEST_PROGRAM) < (legacy_s32)sizeof(absolute_executable));
	assert(snprintf(data_directory, sizeof(data_directory), "%s/data", root) <
		   (legacy_s32)sizeof(data_directory));

	/* A native SDL result takes priority and survives the cwd change too. */
	sdl_base_path = expected;
	test_after_directory_change("missing-program", expected, data_directory);
	sdl_base_path = NULL;
	test_after_directory_change(absolute_executable, expected, data_directory);
	test_after_directory_change("./bin/" TEST_PROGRAM, expected, data_directory);
	test_after_directory_change("./linked-program", expected, data_directory);

	/* PATH entries may be relative, empty, or point at non-executable files. */
	assert(setenv("PATH", "blocked:bin", 1) == 0);
	test_after_directory_change(TEST_PROGRAM, expected, data_directory);
	assert(setenv("PATH", ".", 1) == 0);
	test_after_directory_change("linked-program", expected, data_directory);
	assert(chdir("bin") == 0);
	assert(setenv("PATH", "/missing-directory:", 1) == 0);
	test_after_directory_change(TEST_PROGRAM, expected, data_directory);
	assert(setenv("PATH", "", 1) == 0);
	test_after_directory_change(TEST_PROGRAM, expected, data_directory);
	assert(chdir("..") == 0);

	assert(unsetenv("PATH") == 0);
	asset_path_initialize(TEST_PROGRAM);
	assert(asset_path_base() == NULL);
	asset_path_initialize(NULL);
	assert(asset_path_base() == NULL);
	asset_path_initialize("");
	assert(asset_path_base() == NULL);
	asset_path_initialize("./blocked/" TEST_PROGRAM);
	assert(asset_path_base() == NULL);
	asset_path_initialize("./bin");
	assert(asset_path_base() == NULL);

	assert(remove("linked-program") == 0);
	assert(remove("blocked/" TEST_PROGRAM) == 0);
	assert(remove("bin/" TEST_PROGRAM) == 0);
	assert(rmdir("bin") == 0);
	assert(rmdir("blocked") == 0);
	assert(rmdir("data") == 0);
	assert(chdir(original_directory) == 0);
	assert(rmdir(temporary) == 0);
	if (original_path != NULL) {
		assert(setenv("PATH", original_path, 1) == 0);
	} else {
		assert(unsetenv("PATH") == 0);
	}
	free(root);
	free(original_directory);
	free(original_path);
	puts("Executable asset directory fallback tests passed.");
	return 0;
}
