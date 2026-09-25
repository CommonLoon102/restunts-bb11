#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/legacy.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <sched.h>
#include <sys/resource.h>
#endif

enum {
	TEST_FIRST_CPU = 3,
	TEST_CURRENT_CPU = 11,
	TEST_UNAVAILABLE_CPU = 7,
	TEST_CPU_UNSET = -1,
	TEST_EXPECTED_NICE = -5,
	TEST_STRONGER_NICE = -10,
	TEST_CPU_TEXT_SIZE = sizeof(legacy_int) * CHAR_BIT + 1
};

static const legacy_char *affinity_setting;
static const legacy_char *priority_setting;
static legacy_s32 allowed_cpus[2];
static legacy_s32 allowed_count;
static legacy_s32 current_cpu;
static legacy_s32 selected_cpu;
static legacy_s32 affinity_queries;
static legacy_s32 affinity_updates;
static legacy_s32 priority_queries;
static legacy_s32 priority_updates;
static legacy_s32 affinity_query_error;
static legacy_s32 affinity_update_error;
static legacy_s32 priority_query_error;
static legacy_s32 priority_update_error;
static legacy_s32 diagnostic_count;

static legacy_char *test_getenv(const legacy_char *name)
{
	if (strcmp(name, "RESTUNTS_CPU_AFFINITY") == 0) {
		return (legacy_char *)affinity_setting;
	}
	assert(strcmp(name, "RESTUNTS_HIGH_PRIORITY") == 0);
	return (legacy_char *)priority_setting;
}

static legacy_int test_fprintf(FILE *stream, const legacy_char *format, ...)
{
	assert(stream == stderr && *format != '\0');
	diagnostic_count++;
	return 0;
}

static legacy_int test_fputs(const legacy_char *message, FILE *stream)
{
	return test_fprintf(stream, message);
}

#if defined(_WIN32)
static DWORD inherited_priority;

static HANDLE test_GetCurrentProcess(void)
{
	return NULL;
}

static DWORD test_GetLastError(void)
{
	return ERROR_ACCESS_DENIED;
}

static DWORD test_GetCurrentProcessorNumber(void)
{
	return (DWORD)current_cpu;
}

static BOOL test_GetProcessAffinityMask(HANDLE process, PDWORD_PTR allowed, PDWORD_PTR system)
{
	assert(process == NULL);
	affinity_queries++;
	if (affinity_query_error) {
		return FALSE;
	}
	*allowed = 0;
	*system = ~(DWORD_PTR)0;
	for (legacy_s32 index = 0; index < allowed_count; index++) {
		*allowed |= (DWORD_PTR)1 << allowed_cpus[index];
	}
	return TRUE;
}

static BOOL test_SetProcessAffinityMask(HANDLE process, DWORD_PTR mask)
{
	assert(process == NULL && mask != 0 && (mask & (mask - 1)) == 0);
	affinity_updates++;
	for (legacy_s32 cpu = 0; cpu < (legacy_s32)(sizeof(mask) * CHAR_BIT); cpu++) {
		if (mask & ((DWORD_PTR)1 << cpu)) {
			selected_cpu = cpu;
		}
	}
	return !affinity_update_error;
}

static DWORD test_GetPriorityClass(HANDLE process)
{
	assert(process == NULL);
	priority_queries++;
	return priority_query_error ? 0 : inherited_priority;
}

static BOOL test_SetPriorityClass(HANDLE process, DWORD priority)
{
	assert(process == NULL && priority == ABOVE_NORMAL_PRIORITY_CLASS);
	priority_updates++;
	return !priority_update_error;
}

#define GetCurrentProcess test_GetCurrentProcess
#define GetLastError test_GetLastError
#define GetCurrentProcessorNumber test_GetCurrentProcessorNumber
#define GetProcessAffinityMask test_GetProcessAffinityMask
#define SetProcessAffinityMask test_SetProcessAffinityMask
#define GetPriorityClass test_GetPriorityClass
#define SetPriorityClass test_SetPriorityClass
#else
static legacy_s32 inherited_priority;
static legacy_s32 mask_capacity;

static legacy_int test_sched_getcpu(void)
{
	return current_cpu;
}

static legacy_int test_sched_getaffinity(pid_t process, size_t size, cpu_set_t *allowed)
{
	assert(process == 0);
	affinity_queries++;
	if (affinity_query_error || size < CPU_ALLOC_SIZE(mask_capacity)) {
		errno = affinity_query_error ? EACCES : EINVAL;
		return -1;
	}
	CPU_ZERO_S(size, allowed);
	for (legacy_s32 index = 0; index < allowed_count; index++) {
		CPU_SET_S(allowed_cpus[index], size, allowed);
	}
	return 0;
}

static legacy_int test_sched_setaffinity(pid_t process, size_t size, const cpu_set_t *mask)
{
	assert(process == 0 && CPU_COUNT_S(size, mask) == 1);
	affinity_updates++;
	for (legacy_s32 cpu = 0; cpu < mask_capacity; cpu++) {
		if (CPU_ISSET_S(cpu, size, mask)) {
			selected_cpu = cpu;
		}
	}
	if (affinity_update_error) {
		errno = EPERM;
		return -1;
	}
	return 0;
}

static legacy_int test_getpriority(legacy_int which, id_t process)
{
	assert(which == PRIO_PROCESS && process == 0);
	priority_queries++;
	if (priority_query_error) {
		errno = EACCES;
		return -1;
	}
	return inherited_priority;
}

static legacy_int test_setpriority(legacy_int which, id_t process, legacy_int priority)
{
	assert(which == PRIO_PROCESS && process == 0 && priority == TEST_EXPECTED_NICE);
	priority_updates++;
	if (priority_update_error) {
		errno = EACCES;
		return -1;
	}
	return 0;
}

#define sched_getcpu test_sched_getcpu
#define sched_getaffinity test_sched_getaffinity
#define sched_setaffinity test_sched_setaffinity
#define getpriority test_getpriority
#define setpriority test_setpriority
#endif

/* Every operating-system operation is mocked: this test never changes its own
 * or its parent's scheduling policy, and needs no elevated privileges. */
#define getenv test_getenv
#define fprintf test_fprintf
#define fputs test_fputs
#include "../platform/sdl3/scheduling.c"

static void reset_settings(void)
{
	affinity_setting = NULL;
	priority_setting = NULL;
	allowed_cpus[0] = TEST_FIRST_CPU;
	allowed_cpus[1] = TEST_CURRENT_CPU;
	allowed_count = sizeof(allowed_cpus) / sizeof(allowed_cpus[0]);
	current_cpu = TEST_CURRENT_CPU;
	selected_cpu = TEST_CPU_UNSET;
	affinity_queries = affinity_updates = priority_queries = priority_updates = 0;
	affinity_query_error = affinity_update_error = priority_query_error = priority_update_error = 0;
	diagnostic_count = 0;
#if defined(_WIN32)
	inherited_priority = NORMAL_PRIORITY_CLASS;
#else
	inherited_priority = 0;
	mask_capacity = CPU_SETSIZE;
#endif
}

static void test_affinity_selection(void)
{
	const legacy_char *unpinned[] = {NULL, "", "off"};
	for (legacy_u32 index = 0; index < sizeof(unpinned) / sizeof(unpinned[0]); index++) {
		reset_settings();
		affinity_setting = unpinned[index];
		sdl3_configure_process();
		assert(affinity_queries == 0 && affinity_updates == 0 && selected_cpu == TEST_CPU_UNSET);
		assert(priority_queries == 0 && priority_updates == 0 && diagnostic_count == 0);
	}

	reset_settings();
	affinity_setting = "auto";
	priority_setting = "0";
	sdl3_configure_process();
	assert(affinity_updates == 1 && selected_cpu == TEST_CURRENT_CPU);
	assert(priority_queries == 0);

	reset_settings();
	affinity_setting = "auto";
	current_cpu = TEST_UNAVAILABLE_CPU;
	sdl3_configure_process();
	assert(affinity_updates == 1 && selected_cpu == TEST_FIRST_CPU);

	legacy_char explicit_cpu[TEST_CPU_TEXT_SIZE];
	snprintf(explicit_cpu, sizeof(explicit_cpu), "%d", TEST_FIRST_CPU);
	reset_settings();
	affinity_setting = explicit_cpu;
	sdl3_configure_process();
	assert(affinity_updates == 1 && selected_cpu == TEST_FIRST_CPU);

	reset_settings();
	affinity_setting = "auto";
	priority_setting = "1";
	allowed_count = 0;
	sdl3_configure_process();
	assert(affinity_updates == 0 && priority_updates == 1);

	reset_settings();
	affinity_setting = "off";
	priority_setting = "0";
	sdl3_configure_process();
	assert(affinity_queries == 0 && priority_queries == 0 && diagnostic_count == 0);
}

static void test_invalid_settings(void)
{
	const legacy_char *invalid_cpu[] = {
		"AUTO", "-1", "+1", " 1", "1 ", "1x", "1.0", "0x1", "999999999999999999999999999999999999"};
	for (legacy_u32 index = 0; index < sizeof(invalid_cpu) / sizeof(invalid_cpu[0]); index++) {
		reset_settings();
		affinity_setting = invalid_cpu[index];
		priority_setting = "0";
		sdl3_configure_process();
		assert(affinity_queries == 0 && affinity_updates == 0 && diagnostic_count != 0);
	}
	legacy_char overflow_cpu[TEST_CPU_TEXT_SIZE];
	snprintf(overflow_cpu, sizeof(overflow_cpu), "%" LEGACY_PRIu32, (legacy_u32)INT_MAX + 1U);
	reset_settings();
	affinity_setting = overflow_cpu;
	priority_setting = "0";
	sdl3_configure_process();
	assert(affinity_queries == 0 && affinity_updates == 0 && diagnostic_count != 0);

	legacy_char unavailable_cpu[TEST_CPU_TEXT_SIZE];
	snprintf(unavailable_cpu, sizeof(unavailable_cpu), "%d", TEST_UNAVAILABLE_CPU);
	reset_settings();
	affinity_setting = unavailable_cpu;
	priority_setting = "1";
	sdl3_configure_process();
	assert(affinity_queries != 0 && affinity_updates == 0 && priority_updates == 1);

	snprintf(unavailable_cpu, sizeof(unavailable_cpu), "%d", INT_MAX);
	reset_settings();
	affinity_setting = unavailable_cpu;
	priority_setting = "1";
	sdl3_configure_process();
	assert(affinity_updates == 0 && priority_updates == 1);

	const legacy_char *invalid_priority[] = {"auto", "yes", "2", "-1", " 1", "1 "};
	for (legacy_u32 index = 0; index < sizeof(invalid_priority) / sizeof(invalid_priority[0]);
		 index++) {
		reset_settings();
		affinity_setting = "off";
		priority_setting = invalid_priority[index];
		sdl3_configure_process();
		assert(priority_queries == 0 && priority_updates == 0 && diagnostic_count != 0);
	}
}

static void test_priority_selection(void)
{
	const legacy_char *disabled[] = {NULL, "", "0"};
	for (legacy_u32 index = 0; index < sizeof(disabled) / sizeof(disabled[0]); index++) {
		reset_settings();
		priority_setting = disabled[index];
		sdl3_configure_process();
		assert(priority_queries == 0 && priority_updates == 0 && diagnostic_count == 0);
	}
	reset_settings();
	priority_setting = "1";
	sdl3_configure_process();
	assert(priority_queries == 1 && priority_updates == 1);
#if defined(_WIN32)
	const DWORD elevated[] = {ABOVE_NORMAL_PRIORITY_CLASS, HIGH_PRIORITY_CLASS,
							  REALTIME_PRIORITY_CLASS};
#else
	const legacy_s32 elevated[] = {TEST_EXPECTED_NICE, TEST_STRONGER_NICE};
#endif
	for (legacy_u32 index = 0; index < sizeof(elevated) / sizeof(elevated[0]); index++) {
		reset_settings();
		affinity_setting = "off";
		priority_setting = "1";
		inherited_priority = elevated[index];
		sdl3_configure_process();
		assert(priority_queries == 1 && priority_updates == 0 && diagnostic_count == 0);
	}
}

static void test_independent_failures(void)
{
	reset_settings();
	affinity_setting = "auto";
	priority_setting = "1";
	affinity_query_error = 1;
	sdl3_configure_process();
	assert(affinity_updates == 0 && priority_updates == 1);

	reset_settings();
	affinity_setting = "auto";
	priority_setting = "1";
	affinity_update_error = 1;
	sdl3_configure_process();
	assert(affinity_updates == 1 && priority_updates == 1);

	reset_settings();
	affinity_setting = "auto";
	priority_setting = "1";
	priority_query_error = 1;
	sdl3_configure_process();
	assert(affinity_updates == 1 && priority_queries == 1 && priority_updates == 0);

	reset_settings();
	affinity_setting = "auto";
	priority_setting = "1";
	priority_update_error = 1;
	sdl3_configure_process();
	assert(affinity_updates == 1 && priority_updates == 1);
}

static void test_platform_boundaries(void)
{
	reset_settings();
	affinity_setting = "auto";
#if defined(_WIN32)
	/* Exercise a pointer-sized shift, including bit 63 in a 64-bit build. */
	current_cpu = (legacy_s32)(sizeof(DWORD_PTR) * CHAR_BIT) - 1;
#else
	/* A sparse CPU ID beyond cpu_set_t requires a larger kernel mask. */
	current_cpu = CPU_SETSIZE + TEST_CURRENT_CPU;
	mask_capacity = CPU_SETSIZE * 2;
#endif
	allowed_cpus[1] = current_cpu;
	sdl3_configure_process();
	assert(affinity_updates == 1 && selected_cpu == current_cpu);
#if !defined(_WIN32)
	assert(affinity_queries > 1);
	reset_settings();
	affinity_setting = "auto";
	current_cpu = TEST_CPU_UNSET;
	sdl3_configure_process();
	assert(selected_cpu == TEST_FIRST_CPU);

	/* A valid nice value of -1 must not inherit a stale errno from another call. */
	reset_settings();
	affinity_setting = "off";
	priority_setting = "1";
	inherited_priority = -1;
	errno = EACCES;
	sdl3_configure_process();
	assert(priority_queries == 1 && priority_updates == 1);
#endif
}

legacy_int main(void)
{
	test_affinity_selection();
	test_invalid_settings();
	test_priority_selection();
	test_independent_failures();
	test_platform_boundaries();
	puts("SDL3 affinity, priority, overrides and nonfatal scheduling failures passed.");
	return 0;
}
