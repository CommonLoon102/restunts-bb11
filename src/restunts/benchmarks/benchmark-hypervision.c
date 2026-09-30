/* Reuse the production replay setup and state-preservation checks. */
#define stuntsmain benchmark_replay_test_main
#include "../tests/test-render-replay.c"
#undef stuntsmain
#include "../c/render_workers.h"
#include "../platform/sdl3/sdl3.h"
#include <errno.h>
#include <sys/resource.h>

#define BENCH_DEFAULT_FRAMES 120U
#define BENCH_WARMUP_FRAMES 8U
#define BENCH_LAST_TICK 160U
#define BENCH_TICK_INTERVAL 80U
#define BENCH_MILLISECONDS_PER_SECOND 1000.0
#define BENCH_MICROSECONDS_PER_MILLISECOND 1000.0
#define BENCH_PERCENTILE_95 95U
#define BENCH_PERCENTILE_100 100U
#define BENCH_ARGUMENTS_REQUIRED 2
#define BENCH_ARGUMENTS_WITH_COUNT 3
#define BENCH_REPLAY_ARGUMENT 1
#define BENCH_COUNT_ARGUMENT 2
#define BENCH_COUNT_BASE 10
#define BENCH_CAMERA_ELEVATION 64
#define BENCH_CAMERA_AZIMUTH 128
#define BENCH_CAMERA_DISTANCE 450

static void benchmark_usage(const legacy_s8 *program)
{
	fprintf(stderr,
			"Usage: %s [--data-dir DIR] REPLAY [FRAMES]\n"
			"  REPLAY is a replay name without .rpl; FRAMES is a positive integer\n"
			"  (default %u). Measures full-track geometry at 320x200, 640x400\n"
			"  and 1280x800 in cockpit and external views at ticks 0, 80, 160.\n",
			program, BENCH_DEFAULT_FRAMES);
}

static legacy_s32 benchmark_parse_count(const legacy_s8 *argument, legacy_u32 *count)
{
	if (argument[0] < '0' || argument[0] > '9') {
		return 0;
	}
	legacy_char *end;
	errno = 0;
	legacy_u64 value = strtoull((const legacy_char *)argument, &end, BENCH_COUNT_BASE);
	if (errno == ERANGE || *end != 0 || value == 0 || value > LEGACY_U32_MAX ||
		value > (size_t)-1 / sizeof(legacy_f64)) {
		return 0;
	}
	*count = (legacy_u32)value;
	return 1;
}

static legacy_int benchmark_compare_times(const void *first, const void *second)
{
	legacy_f64 a = *(const legacy_f64 *)first;
	legacy_f64 b = *(const legacy_f64 *)second;
	return (a > b) - (a < b);
}

static legacy_f64 benchmark_cpu_milliseconds(const struct rusage *start, const struct rusage *end)
{
	return (end->ru_utime.tv_sec - start->ru_utime.tv_sec + end->ru_stime.tv_sec -
			start->ru_stime.tv_sec) *
			   BENCH_MILLISECONDS_PER_SECOND +
		   (end->ru_utime.tv_usec - start->ru_utime.tv_usec + end->ru_stime.tv_usec -
			start->ru_stime.tv_usec) /
			   BENCH_MICROSECONDS_PER_MILLISECOND;
}

static legacy_s32 benchmark_frames(legacy_u16 tick, legacy_u32 count)
{
	legacy_f64 *times = malloc((size_t)count * sizeof(*times));
	if (times == NULL) {
		fputs("Cannot allocate benchmark frame samples\n", stderr);
		return 0;
	}
	for (legacy_u32 frame = 0; frame < BENCH_WARMUP_FRAMES; frame++) {
		full_redraw_frames_remaining = 1;
		render_and_check(NULL);
	}
	struct rusage cpu_start, cpu_end;
	if (getrusage(RUSAGE_SELF, &cpu_start) != 0) {
		perror("Cannot read process CPU time");
		free(times);
		return 0;
	}
	legacy_u64 frequency = SDL_GetPerformanceFrequency();
	legacy_u64 started = SDL_GetPerformanceCounter();
	for (legacy_u32 frame = 0; frame < count; frame++) {
		legacy_u64 before = SDL_GetPerformanceCounter();
		full_redraw_frames_remaining = 1;
		render_and_check(NULL);
		times[frame] =
			(SDL_GetPerformanceCounter() - before) * BENCH_MILLISECONDS_PER_SECOND / frequency;
	}
	legacy_f64 total =
		(SDL_GetPerformanceCounter() - started) * BENCH_MILLISECONDS_PER_SECOND / frequency;
	if (getrusage(RUSAGE_SELF, &cpu_end) != 0) {
		perror("Cannot read process CPU time");
		free(times);
		return 0;
	}
	legacy_f64 cpu = benchmark_cpu_milliseconds(&cpu_start, &cpu_end) / count;
	qsort(times, count, sizeof(*times), benchmark_compare_times);
	legacy_u32 percentile =
		(legacy_u32)((legacy_u64)count * BENCH_PERCENTILE_95 / BENCH_PERCENTILE_100);
	printf("tick=%u camera=%d resolution=%dx%d workers=%d n=%u "
		   "cpu=%.3f mean=%.3f p50=%.3f p95=%.3f ms\n",
		   tick, cameramode, hires_render_width(), hires_render_height(), render_workers_count(),
		   count, cpu, total / count, times[count / 2U], times[percentile]);
	fflush(stdout);
	free(times);
	return 1;
}

legacy_s16 stuntsmain(legacy_s16 argc, legacy_s8 *argv[])
{
	legacy_u32 count = BENCH_DEFAULT_FRAMES;
	if (argc == BENCH_ARGUMENTS_REQUIRED &&
		SDL_strcmp((const legacy_char *)argv[BENCH_REPLAY_ARGUMENT], "--help") == 0) {
		benchmark_usage(argv[0]);
		return EXIT_SUCCESS;
	}
	if ((argc != BENCH_ARGUMENTS_REQUIRED && argc != BENCH_ARGUMENTS_WITH_COUNT) ||
		argv[BENCH_REPLAY_ARGUMENT][0] == 0 ||
		(argc == BENCH_ARGUMENTS_WITH_COUNT &&
		 !benchmark_parse_count(argv[BENCH_COUNT_ARGUMENT], &count))) {
		benchmark_usage(argv[0]);
		return EXIT_FAILURE;
	}
	init_main(argc, argv);
	init_div0();
	init_row_tables();
	mainresptr = file_load_resfile("main");
	fontdefptr = file_load_resource(0, "fontdef.fnt");
	fontnptr = file_load_resource(0, "fontn.fnt");
	font_set_fontdef();
	init_polyinfo();
	init_trackdata();
	reset_race_loop_state();
	init_kevinrandom("kevin");
	initialize_replay(argv[BENCH_REPLAY_ARGUMENT]);
	assert(handle_ingame_kb_shortcuts(KEY_F12) != 0);
	frame_adaptive_set_preset(&frame_adaptive, FRAME_ADAPTIVE_PRESET_FULL);
	custom_camera.elevation_angle = BENCH_CAMERA_ELEVATION;
	custom_camera.azimuth_angle = BENCH_CAMERA_AZIMUTH;
	custom_camera.distance = BENCH_CAMERA_DISTANCE;
	static const legacy_s8 cameras[] = {CAMERA_MODE_COCKPIT, CAMERA_MODE_CUSTOM};
	static const legacy_s32 scales[] = {HIRES_MINIMUM_SCALE, HIRES_MEDIUM_SCALE, HIRES_SCALE};
	legacy_u16 limit = gameconfig.game_recordedframes < BENCH_LAST_TICK
						   ? gameconfig.game_recordedframes
						   : BENCH_LAST_TICK;
	puts("HyperVision full-track renderer benchmark: process CPU and elapsed milliseconds/frame.");
	puts("Includes render/state checks and framebuffer copies; excludes display, pacing and "
		 "physics.");
	for (legacy_u16 tick = 0; tick <= limit; tick++) {
		if (tick % BENCH_TICK_INTERVAL == 0 || tick == limit) {
			for (legacy_u32 camera = 0; camera < SDL_arraysize(cameras); camera++) {
				cameramode = cameras[camera];
				for (legacy_u32 scale = 0; scale < SDL_arraysize(scales); scale++) {
					hires_set_render_scale(scales[scale]);
					if (!benchmark_frames(tick, count)) {
						call_exitlist();
						return EXIT_FAILURE;
					}
				}
			}
		}
		if (tick < limit) {
			update_gamestate();
		}
	}
	call_exitlist();
	return EXIT_SUCCESS;
}
