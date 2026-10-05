#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include "../c/legacy.h"

#define TEST_ARGUMENT_CAPACITY 6
#define TEST_ARGUMENT_TEXT_CAPACITY 64
#define TEST_EXIT_HANDLER_CAPACITY 2
#define TEST_GAME_RESULT 7
#define TEST_ARGUMENT_COUNT(arguments)                                                             \
	((legacy_int)(sizeof(arguments) / sizeof((arguments)[0]) - 1U))

static bool SDLCALL test_sdl_init(SDL_InitFlags flags);
static const legacy_char *SDLCALL test_sdl_error(void);
static legacy_int test_chdir(const legacy_char *directory);
static legacy_int test_atexit(void (*handler)(void));
static void *test_malloc(size_t size);
static void test_free(void *allocation);

#define RESTUNTS_FULL
#define main test_entry_main
#define SDL_Init test_sdl_init
#define SDL_GetError test_sdl_error
#define chdir test_chdir
#define atexit test_atexit
#define malloc test_malloc
#define free test_free
#include "../platform/sdl3/main.c"
#undef free
#undef malloc
#undef atexit
#undef chdir
#undef SDL_GetError
#undef SDL_Init
#undef main

static legacy_char **original_arguments;
static legacy_char *original_pointers[TEST_ARGUMENT_CAPACITY];
static legacy_char original_text[TEST_ARGUMENT_CAPACITY][TEST_ARGUMENT_TEXT_CAPACITY];
static legacy_int original_count;
static legacy_int expected_count;
static const legacy_char *const *expected_arguments;
static legacy_s16 information_result;
static legacy_int directory_calls;
static legacy_int initialization_calls;
static legacy_int game_calls;
static legacy_int fail_allocation;
static legacy_int fail_registration;
static legacy_int fail_initialization;
static legacy_int exit_from_game;
static void *live_allocation;
static void (*exit_handlers[TEST_EXIT_HANDLER_CAPACITY])(void);
static legacy_int exit_handler_count;
static jmp_buf exit_target;

static void assert_original_arguments(void)
{
	for (legacy_int index = 0; index <= original_count; index++) {
		assert(original_arguments[index] == original_pointers[index]);
		if (index < original_count) {
			assert(strcmp(original_arguments[index], original_text[index]) == 0);
		}
	}
}

static void reset_test(legacy_int argc, legacy_char **argv, legacy_int game_argc,
					   const legacy_char *const *game_argv)
{
	assert(live_allocation == NULL);
	assert(exit_handler_count == 0);
	assert(argc < TEST_ARGUMENT_CAPACITY);
	original_count = argc;
	original_arguments = argv;
	for (legacy_int index = 0; index <= argc; index++) {
		original_pointers[index] = argv[index];
		if (index < argc) {
			assert(strlen(argv[index]) < TEST_ARGUMENT_TEXT_CAPACITY);
			strcpy(original_text[index], argv[index]);
		}
	}
	expected_count = game_argc;
	expected_arguments = game_argv;
	information_result = COMMAND_LINE_CONTINUE;
	directory_calls = 0;
	initialization_calls = 0;
	game_calls = 0;
	fail_allocation = 0;
	fail_registration = 0;
	fail_initialization = 0;
	exit_from_game = 0;
}

static void run_exit_handlers(void)
{
	while (exit_handler_count != 0) {
		exit_handlers[--exit_handler_count]();
	}
	assert(live_allocation == NULL);
	assert_original_arguments();
}

static void *test_malloc(size_t size)
{
	assert(live_allocation == NULL);
	live_allocation = fail_allocation ? NULL : malloc(size);
	return live_allocation;
}

static void test_free(void *allocation)
{
	assert(allocation == live_allocation);
	free(allocation);
	live_allocation = NULL;
}

static legacy_int test_atexit(void (*handler)(void))
{
	if (fail_registration) {
		return -1;
	}
	assert(exit_handler_count < TEST_EXIT_HANDLER_CAPACITY);
	exit_handlers[exit_handler_count++] = handler;
	return 0;
}

static legacy_int test_chdir(const legacy_char *directory)
{
	directory_calls++;
	assert(strcmp(directory, "data-directory") == 0);
	return 0;
}

static bool SDLCALL test_sdl_init(SDL_InitFlags flags)
{
	assert(flags == 0);
	assert_original_arguments();
	initialization_calls++;
	return fail_initialization == 0;
}

static const legacy_char *SDLCALL test_sdl_error(void)
{
	return "expected test failure";
}

legacy_s16 command_line_print_info(legacy_s32 argc, legacy_s8 *argv[])
{
	(void)argc;
	assert((legacy_char **)argv == original_arguments);
	assert_original_arguments();
	return information_result;
}

void asset_path_initialize(const legacy_char *executable)
{
	assert(executable == (original_count > 0 ? original_arguments[0] : NULL));
	assert(directory_calls == 0);
}

void full_data_initialize(void)
{
	assert(initialization_calls == 1);
}

void sdl3_platform_shutdown(void)
{
	/* The argument storage survives shutdown, including a direct exit(). */
	assert(live_allocation != NULL);
	assert_original_arguments();
}

legacy_s16 run_main_menu_loop(legacy_s16 argc, legacy_s8 *argv[])
{
	game_calls++;
	assert(argc == expected_count);
	assert((legacy_char **)argv != original_arguments);
	assert(argv[argc] == NULL);
	for (legacy_int index = 0; index < argc; index++) {
		assert(strcmp((const legacy_char *)argv[index], expected_arguments[index]) == 0);
		for (legacy_int source = 0; source < original_count; source++) {
			assert((legacy_char *)argv[index] != original_arguments[source]);
		}
		argv[index][0] = '!';
	}
	/* Exercise both dump-style text edits and benchmark-style vector compaction. */
	if (argc > SDL3_FIRST_OPTION_INDEX) {
		argv[0] = argv[SDL3_FIRST_OPTION_INDEX];
		argv[SDL3_FIRST_OPTION_INDEX] = NULL;
	}
	assert_original_arguments();
	if (exit_from_game) {
		run_exit_handlers();
		longjmp(exit_target, 1);
	}
	return TEST_GAME_RESULT;
}

static void test_dispatch(legacy_int argc, legacy_char **argv, legacy_int game_argc,
						  const legacy_char *const *game_argv, legacy_int direct_exit)
{
	reset_test(argc, argv, game_argc, game_argv);
	exit_from_game = direct_exit;
	if (setjmp(exit_target) == 0) {
		assert(test_entry_main(argc, argv) == TEST_GAME_RESULT);
		assert(!direct_exit);
		run_exit_handlers();
	} else {
		assert(direct_exit);
	}
	assert(game_calls == 1);
	assert(initialization_calls == 1);
}

legacy_int main(void)
{
	legacy_char program[] = "restunts";
	legacy_char data_option[] = "--data-dir";
	legacy_char directory[] = "data-directory";
	legacy_char option[] = "--nointro";
	legacy_char replay[] = "replay.rpl";
	legacy_char *filtered[] = {program, data_option, directory, option, replay, NULL};
	legacy_char *plain[] = {program, option, replay, NULL};
	legacy_char *directory_only[] = {program, data_option, directory, NULL};
	legacy_char *missing_directory[] = {program, data_option, NULL};
	legacy_char *empty[] = {NULL};
	const legacy_char *expected[] = {"restunts", "--nointro", "replay.rpl"};
	test_dispatch(TEST_ARGUMENT_COUNT(filtered), filtered, TEST_ARGUMENT_COUNT(plain), expected, 0);
	assert(directory_calls == 1);
	test_dispatch(TEST_ARGUMENT_COUNT(plain), plain, TEST_ARGUMENT_COUNT(plain), expected, 1);
	assert(directory_calls == 0);
	test_dispatch(TEST_ARGUMENT_COUNT(directory_only), directory_only, SDL3_FIRST_OPTION_INDEX,
				  expected, 0);
	test_dispatch(TEST_ARGUMENT_COUNT(empty), empty, 0, NULL, 0);

	reset_test(TEST_ARGUMENT_COUNT(missing_directory), missing_directory, 0, NULL);
	information_result = COMMAND_LINE_SUCCESS;
	assert(test_entry_main(original_count, original_arguments) == COMMAND_LINE_SUCCESS);
	assert(directory_calls == 0 && initialization_calls == 0 && exit_handler_count == 0);
	information_result = COMMAND_LINE_CONTINUE;
	assert(test_entry_main(original_count, original_arguments) == COMMAND_LINE_FAILURE);
	assert(directory_calls == 0 && initialization_calls == 0 && exit_handler_count == 0);
	assert_original_arguments();

	reset_test(TEST_ARGUMENT_COUNT(plain), plain, 0, NULL);
	assert(test_entry_main((legacy_int)LEGACY_S16_MAX + 1, plain) == COMMAND_LINE_FAILURE);
	assert(initialization_calls == 0 && live_allocation == NULL);
	fail_allocation = 1;
	assert(test_entry_main(original_count, plain) == COMMAND_LINE_FAILURE);
	assert(initialization_calls == 0 && live_allocation == NULL);
	fail_allocation = 0;
	fail_registration = 1;
	assert(test_entry_main(original_count, plain) == COMMAND_LINE_FAILURE);
	assert(initialization_calls == 0 && live_allocation == NULL);
	fail_registration = 0;
	fail_initialization = 1;
	assert(test_entry_main(original_count, plain) == COMMAND_LINE_FAILURE);
	assert(game_calls == 0 && live_allocation == NULL);
	run_exit_handlers();
	puts("SDL3 entry-point argument tests passed");
	return 0;
}
