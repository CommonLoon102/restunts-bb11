#include "render_workers.h"

#if !defined(__DJGPP__)
#include <SDL3/SDL_atomic.h>
#include <SDL3/SDL_cpuinfo.h>
#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>
#include <stdlib.h>

/* Bound scheduling overhead for a one-megapixel software target. This is a
 * ceiling, not a CPU assumption: smaller machines use their detected count. */
#define RENDER_MAX_WORKERS 7

struct RENDER_WORKER {
	SDL_Thread *thread;
	SDL_Semaphore *start;
};

static struct RENDER_WORKER workers[RENDER_MAX_WORKERS];
static SDL_Semaphore *completed;
static SDL_AtomicInt next_job;
static legacy_s32 initialized;
static legacy_s32 worker_count;
static legacy_s32 stopping;
static legacy_s32 job_count;
static void (*job_function)(void *, legacy_s32);
static void *job_context;

static void run_jobs(void)
{
	for (;;) {
		legacy_s32 index = SDL_AddAtomicInt(&next_job, 1);
		if (index >= job_count) {
			return;
		}
		job_function(job_context, index);
	}
}

static int SDLCALL worker_main(void *argument)
{
	struct RENDER_WORKER *worker = argument;
	for (;;) {
		SDL_WaitSemaphore(worker->start);
		if (stopping) {
			return 0;
		}
		run_jobs();
		SDL_SignalSemaphore(completed);
	}
}

static void initialize_workers(void)
{
	if (initialized) {
		return;
	}
	initialized = 1;
	legacy_s32 requested = SDL_GetNumLogicalCPUCores() - 1;
	const char *setting = SDL_getenv("RESTUNTS_RENDER_WORKERS");
	if (setting != NULL && *setting != 0) {
		char *end;
		legacy_s64 value = strtol(setting, &end, 10);
		if (*end == 0 && value >= 0) {
			requested = value > RENDER_MAX_WORKERS ? RENDER_MAX_WORKERS : (legacy_s32)value;
		}
	}
	if (requested > RENDER_MAX_WORKERS) {
		requested = RENDER_MAX_WORKERS;
	}
	if (requested <= 0) {
		return;
	}
	completed = SDL_CreateSemaphore(0);
	if (completed == NULL) {
		return;
	}
	for (legacy_s32 index = 0; index < requested; index++) {
		struct RENDER_WORKER *worker = &workers[index];
		worker->start = SDL_CreateSemaphore(0);
		if (worker->start == NULL) {
			break;
		}
		worker->thread = SDL_CreateThread(worker_main, "renderer", worker);
		if (worker->thread == NULL) {
			SDL_DestroySemaphore(worker->start);
			worker->start = NULL;
			break;
		}
		worker_count++;
	}
}

legacy_s32 render_workers_count(void)
{
	initialize_workers();
	return worker_count;
}

void render_workers_shutdown(void)
{
	stopping = 1;
	for (legacy_s32 index = 0; index < worker_count; index++) {
		SDL_SignalSemaphore(workers[index].start);
	}
	for (legacy_s32 index = 0; index < worker_count; index++) {
		SDL_WaitThread(workers[index].thread, NULL);
		SDL_DestroySemaphore(workers[index].start);
		workers[index].thread = NULL;
		workers[index].start = NULL;
	}
	/* Some semaphore backends initialize their dispatch table on first creation. */
	if (completed != NULL) {
		SDL_DestroySemaphore(completed);
		completed = NULL;
	}
	worker_count = 0;
	initialized = 0;
	stopping = 0;
}

legacy_s32 render_workers_run(legacy_s32 count, void (*job)(void *, legacy_s32), void *context)
{
	if (count <= 1) {
		if (count == 1) {
			job(context, 0);
		}
		return 0;
	}
	initialize_workers();
	legacy_s32 used = worker_count < count ? worker_count : count - 1;
	if (used == 0) {
		for (legacy_s32 index = 0; index < count; index++) {
			job(context, index);
		}
		return 0;
	}
	job_count = count;
	job_function = job;
	job_context = context;
	SDL_SetAtomicInt(&next_job, 0);
	for (legacy_s32 index = 0; index < used; index++) {
		SDL_SignalSemaphore(workers[index].start);
	}
	run_jobs();
	for (legacy_s32 index = 0; index < used; index++) {
		SDL_WaitSemaphore(completed);
	}
	return used;
}
#else
legacy_s32 render_workers_count(void)
{
	return 0;
}

legacy_s32 render_workers_run(legacy_s32 count, void (*job)(void *, legacy_s32), void *context)
{
	for (legacy_s32 index = 0; index < count; index++) {
		job(context, index);
	}
	return 0;
}

void render_workers_shutdown(void)
{
}
#endif
