#include <assert.h>
#include <stdio.h>
#include <SDL3/SDL.h>

static int detected_cores;
static int thread_budget;

static int detect_cores(void)
{
	return detected_cores;
}

static SDL_Thread *create_thread(SDL_ThreadFunction function, const char *name, void *argument)
{
	if (thread_budget == 0) {
		return NULL;
	}
	thread_budget--;
	return SDL_CreateThread(function, name, argument);
}

#undef SDL_CreateThread
#define SDL_CreateThread create_thread
#define SDL_GetNumLogicalCPUCores detect_cores
#include "../c/render_workers.c"

struct JOB_RESULTS {
	int visits[97];
	SDL_ThreadID threads[97];
};

static void record_job(void *argument, legacy_s32 index)
{
	struct JOB_RESULTS *results = argument;
	assert(index >= 0 && index < 97);
	results->visits[index]++;
	results->threads[index] = SDL_GetCurrentThreadID();
	SDL_Delay(1);
}

static void check_jobs(int expected_workers)
{
	for (int pass = 0; pass < 3; pass++) {
		struct JOB_RESULTS results = {0};
		assert(render_workers_run(97, record_job, &results) == expected_workers);
		int background_jobs = 0;
		for (int index = 0; index < 97; index++) {
			assert(results.visits[index] == 1);
			background_jobs += results.threads[index] != SDL_GetCurrentThreadID();
		}
		assert((background_jobs != 0) == (expected_workers != 0));
	}
}

int main(void)
{
	assert(SDL_Init(0));
	const int cores[] = {0, 1, 2, 4, 64};
	const int expected[] = {0, 0, 1, 3, 7};
	assert(SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS", "", true));
	for (unsigned int index = 0; index < sizeof(cores) / sizeof(cores[0]); index++) {
		detected_cores = cores[index];
		thread_budget = 100;
		assert(render_workers_count() == expected[index]);
		check_jobs(expected[index]);
		render_workers_shutdown();
		render_workers_shutdown();
	}
	/* Resource exhaustion keeps the workers already created, or runs serially. */
	for (int available = 0; available < 3; available++) {
		detected_cores = 8;
		thread_budget = available;
		check_jobs(available);
		render_workers_shutdown();
	}
	const char *settings[] = {"0", "1", "2", "999", "invalid", "-2"};
	const int configured[] = {0, 1, 2, 7, 3, 3};
	detected_cores = 4;
	for (unsigned int index = 0; index < sizeof(settings) / sizeof(settings[0]); index++) {
		assert(SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS",
										  settings[index], true));
		thread_budget = 100;
		assert(render_workers_count() == configured[index]);
		check_jobs(configured[index]);
		render_workers_shutdown();
	}
	struct JOB_RESULTS single = {0};
	assert(render_workers_run(0, record_job, &single) == 0);
	assert(single.visits[0] == 0);
	assert(render_workers_run(1, record_job, &single) == 0);
	assert(single.visits[0] == 1 && single.threads[0] == SDL_GetCurrentThreadID());
	SDL_Quit();
	puts("Renderer CPU detection, worker limits, job ownership and lifecycle passed.");
	return 0;
}
