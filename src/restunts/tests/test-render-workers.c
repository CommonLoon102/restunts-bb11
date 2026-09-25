#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <SDL3/SDL.h>

enum {
	TEST_MAX_WORKERS = 7,
	TEST_RESOURCE_BUDGET = TEST_MAX_WORKERS + 1,
	TEST_JOB_COUNT = 97,
	TEST_JOB_PASSES = 3,
	TEST_JOB_DELAY_MS = 1,
	TEST_FAILURE_LIMIT = 3
};

static int detected_cores;
static int thread_budget;
static int semaphore_budget;
static int created_threads;
static int created_semaphores;
static int live_semaphores;

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
	SDL_Thread *thread = SDL_CreateThread(function, name, argument);
	if (thread != NULL) {
		created_threads++;
	}
	return thread;
}

static SDL_Semaphore *create_semaphore(Uint32 initial_value)
{
	if (semaphore_budget == 0) {
		return NULL;
	}
	semaphore_budget--;
	SDL_Semaphore *semaphore = SDL_CreateSemaphore(initial_value);
	if (semaphore != NULL) {
		created_semaphores++;
		live_semaphores++;
	}
	return semaphore;
}

static void destroy_semaphore(SDL_Semaphore *semaphore)
{
	assert(semaphore != NULL && live_semaphores > 0);
	SDL_DestroySemaphore(semaphore);
	live_semaphores--;
}

#undef SDL_CreateThread
#define SDL_CreateThread create_thread
#define SDL_CreateSemaphore create_semaphore
#define SDL_DestroySemaphore destroy_semaphore
#define SDL_GetNumLogicalCPUCores detect_cores
#include "../c/render_workers.c"

struct JOB_RESULTS {
	int visits[TEST_JOB_COUNT];
	SDL_ThreadID threads[TEST_JOB_COUNT];
};

static void record_job(void *argument, legacy_s32 index)
{
	struct JOB_RESULTS *results = argument;
	assert(index >= 0 && index < TEST_JOB_COUNT);
	results->visits[index]++;
	results->threads[index] = SDL_GetCurrentThreadID();
	SDL_Delay(TEST_JOB_DELAY_MS);
}

static void check_jobs(int expected_workers)
{
	for (int pass = 0; pass < TEST_JOB_PASSES; pass++) {
		struct JOB_RESULTS results = {0};
		assert(render_workers_run(TEST_JOB_COUNT, record_job, &results) == expected_workers);
		int background_jobs = 0;
		for (int index = 0; index < TEST_JOB_COUNT; index++) {
			assert(results.visits[index] == 1);
			background_jobs += results.threads[index] != SDL_GetCurrentThreadID();
		}
		assert((background_jobs != 0) == (expected_workers != 0));
	}
}

static void configure_workers(const char *setting)
{
	render_workers_shutdown();
	render_workers_shutdown();
	assert(live_semaphores == 0);
	if (setting == NULL) {
		assert(SDL_UnsetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS"));
	} else {
		assert(SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS", setting,
										  true));
	}
	thread_budget = TEST_RESOURCE_BUDGET;
	semaphore_budget = TEST_RESOURCE_BUDGET;
	created_threads = 0;
	created_semaphores = 0;
}

int main(void)
{
	assert(SDL_Init(0));
	const char *defaults[] = {NULL, ""};
	const int cores[] = {INT_MIN, 0, 1, 2, 4, 64, INT_MAX};
	const int expected[] = {0, 0, 0, 1, 3, TEST_MAX_WORKERS, TEST_MAX_WORKERS};
	for (unsigned int setting = 0; setting < sizeof(defaults) / sizeof(defaults[0]); setting++) {
		for (unsigned int index = 0; index < sizeof(cores) / sizeof(cores[0]); index++) {
			configure_workers(defaults[setting]);
			detected_cores = cores[index];
			assert(render_workers_count() == 0);
			check_jobs(0);
			assert(created_threads == 0 && created_semaphores == 0);
		}
	}
	for (unsigned int index = 0; index < sizeof(cores) / sizeof(cores[0]); index++) {
		configure_workers("auto");
		detected_cores = cores[index];
		assert(render_workers_count() == expected[index]);
		check_jobs(expected[index]);
	}
	/* Resource exhaustion keeps the workers already created, or runs serially. */
	for (int available = 0; available < TEST_FAILURE_LIMIT; available++) {
		configure_workers("auto");
		detected_cores = TEST_RESOURCE_BUDGET;
		thread_budget = available;
		check_jobs(available);
		assert(render_workers_count() == available);
	}
	for (int available = 0; available < TEST_FAILURE_LIMIT; available++) {
		configure_workers("auto");
		semaphore_budget = available;
		int expected_workers = available > 0 ? available - 1 : 0;
		check_jobs(expected_workers);
		assert(render_workers_count() == expected_workers);
	}
	const char *settings[] = {
		"0",	   "1",	 "2",		 "7", "999",
		"invalid", "-2", "2workers", " ", "999999999999999999999999999999999999",
		"auto2"};
	const int configured[] = {0, 1, 2, TEST_MAX_WORKERS, TEST_MAX_WORKERS, 0, 0, 0, 0, 0, 0};
	detected_cores = 1;
	for (unsigned int index = 0; index < sizeof(settings) / sizeof(settings[0]); index++) {
		configure_workers(settings[index]);
		/* A previous library error must not reject a valid numeric override. */
		errno = ERANGE;
		assert(render_workers_count() == configured[index]);
		check_jobs(configured[index]);
	}
	configure_workers("auto");
	detected_cores = TEST_RESOURCE_BUDGET;
	struct JOB_RESULTS single = {0};
	assert(render_workers_run(0, record_job, &single) == 0);
	assert(single.visits[0] == 0);
	assert(render_workers_run(1, record_job, &single) == 0);
	assert(single.visits[0] == 1 && single.threads[0] == SDL_GetCurrentThreadID());
	assert(created_threads == 0 && created_semaphores == 0);
	check_jobs(TEST_MAX_WORKERS);
	render_workers_shutdown();
	render_workers_shutdown();
	assert(live_semaphores == 0);
	SDL_Quit();
	puts("Renderer serial defaults, overrides, CPU detection, failure recovery and ownership "
		 "passed.");
	return 0;
}
