#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include "scheduling.h"
#include "../../c/legacy.h"

#if defined(__linux__) || defined(_WIN32)
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sched.h>
#include <sys/resource.h>
#endif

#define PROCESS_CPU_AUTOMATIC (-1)
#define PROCESS_CPU_UNCHANGED (-2)
#define PROCESS_CPU_DECIMAL_BASE 10
#define PROCESS_NICE_ABOVE_NORMAL (-5)
#define PROCESS_CPU_MASK_GROWTH 2

static legacy_s32 requested_cpu(void)
{
	const legacy_char *setting = getenv("RESTUNTS_CPU_AFFINITY");
	if (setting == NULL || *setting == '\0' || strcmp(setting, "off") == 0) {
		return PROCESS_CPU_UNCHANGED;
	}
	if (strcmp(setting, "auto") == 0) {
		return PROCESS_CPU_AUTOMATIC;
	}
	legacy_char *end;
	errno = 0;
	legacy_s64 cpu = strtol(setting, &end, PROCESS_CPU_DECIMAL_BASE);
	if (*setting >= '0' && *setting <= '9' && *end == '\0' && errno == 0 &&
		cpu <= (legacy_s64)LEGACY_S32_MAX) {
		return (legacy_s32)cpu;
	}
	fputs("Invalid RESTUNTS_CPU_AFFINITY: use auto, off, or a logical CPU number; "
		  "keeping inherited affinity.\n",
		  stderr);
	return PROCESS_CPU_UNCHANGED;
}

static legacy_s32 requested_high_priority(void)
{
	const legacy_char *setting = getenv("RESTUNTS_HIGH_PRIORITY");
	if (setting == NULL || *setting == '\0' || strcmp(setting, "1") == 0) {
		return 1;
	}
	if (strcmp(setting, "0") != 0) {
		fputs("Invalid RESTUNTS_HIGH_PRIORITY: use 0 or 1; keeping inherited priority.\n", stderr);
	}
	return 0;
}

#if defined(_WIN32)
static void configure_affinity(legacy_s32 requested)
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR allowed;
	DWORD_PTR system;
	if (!GetProcessAffinityMask(process, &allowed, &system)) {
		fprintf(stderr, "Cannot read CPU affinity (Windows error %lu).\n", GetLastError());
		return;
	}
	if (allowed == 0) {
		fputs("Cannot select a CPU from the process's current processor group.\n", stderr);
		return;
	}
	const legacy_s32 cpu_limit = (legacy_s32)(sizeof(allowed) * CHAR_BIT);
	legacy_s32 cpu = requested;
	if (cpu == PROCESS_CPU_AUTOMATIC) {
		cpu = (legacy_s32)GetCurrentProcessorNumber();
		if (cpu >= cpu_limit || !(allowed & ((DWORD_PTR)1 << cpu))) {
			for (cpu = 0; cpu < cpu_limit; cpu++) {
				if (allowed & ((DWORD_PTR)1 << cpu)) {
					break;
				}
			}
		}
	}
	if (cpu >= cpu_limit || !(allowed & ((DWORD_PTR)1 << cpu))) {
		fprintf(stderr,
				"Logical CPU %" LEGACY_PRId32
				" is outside the allowed affinity; keeping it unchanged.\n",
				cpu);
		return;
	}
	if (!SetProcessAffinityMask(process, (DWORD_PTR)1 << cpu)) {
		fprintf(stderr, "Cannot set CPU affinity (Windows error %lu).\n", GetLastError());
		return;
	}
	fprintf(stderr, "CPU affinity: logical CPU %" LEGACY_PRId32 ".\n", cpu);
}

static void configure_priority(void)
{
	HANDLE process = GetCurrentProcess();
	DWORD priority = GetPriorityClass(process);
	if (priority == 0) {
		fprintf(stderr, "Cannot read process priority (Windows error %lu).\n", GetLastError());
		return;
	}
	if (priority == ABOVE_NORMAL_PRIORITY_CLASS || priority == HIGH_PRIORITY_CLASS ||
		priority == REALTIME_PRIORITY_CLASS) {
		return;
	}
	if (!SetPriorityClass(process, ABOVE_NORMAL_PRIORITY_CLASS)) {
		fprintf(stderr,
				"Cannot raise process priority (Windows error %lu); keeping it unchanged.\n",
				GetLastError());
		return;
	}
	fputs("Process priority: above normal.\n", stderr);
}
#else
static void configure_affinity(legacy_s32 requested)
{
	legacy_s32 capacity = CPU_SETSIZE;
	cpu_set_t *allowed;
	size_t size;
	/* Kernel masks can exceed cpu_set_t, even when few CPUs are online. */
	for (;;) {
		allowed = CPU_ALLOC(capacity);
		if (allowed == NULL) {
			fputs("Cannot allocate CPU affinity mask.\n", stderr);
			return;
		}
		size = CPU_ALLOC_SIZE(capacity);
		if (sched_getaffinity(0, size, allowed) == 0) {
			break;
		}
		legacy_s32 error = errno;
		CPU_FREE(allowed);
		if (error != EINVAL || capacity > (legacy_s32)LEGACY_S32_MAX / PROCESS_CPU_MASK_GROWTH) {
			fprintf(stderr, "Cannot read CPU affinity: %s.\n", strerror(error));
			return;
		}
		capacity *= PROCESS_CPU_MASK_GROWTH;
	}
	legacy_s32 cpu = requested;
	if (cpu == PROCESS_CPU_AUTOMATIC) {
		cpu = sched_getcpu();
		if (cpu < 0 || cpu >= capacity || !CPU_ISSET_S(cpu, size, allowed)) {
			for (cpu = 0; cpu < capacity; cpu++) {
				if (CPU_ISSET_S(cpu, size, allowed)) {
					break;
				}
			}
		}
	}
	if (cpu >= capacity || !CPU_ISSET_S(cpu, size, allowed)) {
		fprintf(stderr,
				"Logical CPU %" LEGACY_PRId32
				" is outside the allowed affinity; keeping it unchanged.\n",
				cpu);
		CPU_FREE(allowed);
		return;
	}
	CPU_ZERO_S(size, allowed);
	CPU_SET_S(cpu, size, allowed);
	if (sched_setaffinity(0, size, allowed) != 0) {
		fprintf(stderr, "Cannot set CPU affinity: %s.\n", strerror(errno));
	} else {
		fprintf(stderr, "CPU affinity: logical CPU %" LEGACY_PRId32 ".\n", cpu);
	}
	CPU_FREE(allowed);
}

static void configure_priority(void)
{
	errno = 0;
	legacy_s32 priority = getpriority(PRIO_PROCESS, 0);
	if (priority == -1 && errno != 0) {
		fprintf(stderr, "Cannot read process priority: %s.\n", strerror(errno));
		return;
	}
	if (priority <= PROCESS_NICE_ABOVE_NORMAL) {
		return;
	}
	if (setpriority(PRIO_PROCESS, 0, PROCESS_NICE_ABOVE_NORMAL) != 0) {
		fprintf(stderr,
				"Cannot raise process priority to nice %d: %s. "
				"Keeping inherited priority (Linux requires an appropriate "
				"RLIMIT_NICE or CAP_SYS_NICE).\n",
				PROCESS_NICE_ABOVE_NORMAL, strerror(errno));
		return;
	}
	fprintf(stderr, "Process priority: nice %d.\n", PROCESS_NICE_ABOVE_NORMAL);
}
#endif
#endif

void sdl3_configure_process(void)
{
#if defined(__linux__) || defined(_WIN32)
	legacy_s32 cpu = requested_cpu();
	if (cpu != PROCESS_CPU_UNCHANGED) {
		configure_affinity(cpu);
	}
	if (requested_high_priority()) {
		configure_priority();
	}
#endif
}
