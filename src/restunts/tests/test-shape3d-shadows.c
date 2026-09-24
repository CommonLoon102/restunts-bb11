#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../c/shape3d_shadows.h"

static const struct VECTOR origin = {0, 0, 0};

static void add_roof(legacy_f64 x, legacy_f64 y, legacy_f64 z, legacy_f64 radius, legacy_s32 grille)
{
	const struct SHAPE3D_HIRES_VECTOR vertices[] = {{x - radius, y, z - radius},
													{x + radius, y, z - radius},
													{x + radius, y, z + radius},
													{x - radius, y, z + radius}};
	shape3d_shadows_add_polygon(vertices, 4, grille);
}

static void test_ground_and_roof(void)
{
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_active());
	assert(shape3d_shadows_sample(0, 0, 0) == 0);
	add_roof(0, 0, 0, 1024, 0);
	assert(shape3d_shadows_sample(0, 0, 0) == 0);
	add_roof(0, 64, 0, 128, 0);
	assert(shape3d_shadows_sample(32, 0, 24) >= 78);
	assert(shape3d_shadows_sample(32, 64, 24) == 0);
	assert(shape3d_shadows_sample(32, 80, 24) == 0);
	assert(shape3d_shadows_sample(300, 0, 300) == 0);
}

static void test_slope_and_winding(void)
{
	const struct SHAPE3D_HIRES_VECTOR slope[] = {
		{-128, 16, -128}, {128, 144, -128}, {128, 144, 128}, {-128, 16, 128}};
	legacy_u8 first[13];
	for (legacy_s32 reversed = 0; reversed < 2; reversed++) {
		struct SHAPE3D_HIRES_VECTOR vertices[4];
		for (legacy_s32 index = 0; index < 4; index++) {
			vertices[index] = slope[reversed ? 3 - index : index];
		}
		shape3d_shadows_begin(&origin);
		shape3d_shadows_add_polygon(vertices, 4, 0);
		for (legacy_s32 sample = 0; sample < 13; sample++) {
			legacy_f64 x = sample * 16 - 96;
			assert(shape3d_shadows_sample_plane(x, 80 + x * 0.5, 0, -0.5, 1, 0) == 0);
			legacy_u8 shade = shape3d_shadows_sample(x, 0, 0);
			assert(shade >= 78);
			if (reversed) {
				assert(shade == first[sample]);
			} else {
				first[sample] = shade;
			}
		}
	}
}

static void test_grille_and_camera_stability(void)
{
	const struct VECTOR shifted = {123, 40, -81};
	legacy_u8 first[128];
	legacy_u8 minimum = 255;
	legacy_u8 maximum = 0;
	for (legacy_s32 moved = 0; moved < 2; moved++) {
		const struct VECTOR *camera = moved ? &shifted : &origin;
		shape3d_shadows_begin(camera);
		assert(shape3d_shadows_ground_height() == -camera->y);
		add_roof(-camera->x, 128 - camera->y, -camera->z, 256, 1);
		for (legacy_s32 sample = 0; sample < 128; sample++) {
			legacy_u8 shade = shape3d_shadows_sample((sample % 16) * 2 - camera->x, -camera->y,
													 40 + (sample / 16) * 2 - camera->z);
			if (moved) {
				assert(shade == first[sample]);
			} else {
				first[sample] = shade;
				if (shade < minimum) {
					minimum = shade;
				}
				if (shade > maximum) {
					maximum = shade;
				}
			}
		}
	}
	assert(minimum == 0);
	assert(maximum >= 60);
}

static void test_contact_wall_and_overlapping_casters(void)
{
	const struct SHAPE3D_HIRES_VECTOR wall[] = {
		{0, 0, -128}, {0, 96, -128}, {0, 96, 128}, {0, 0, 128}};
	shape3d_shadows_begin(&origin);
	shape3d_shadows_add_polygon(wall, 4, 0);
	/* The light falls toward +X; negative X isolates the ambient contact. */
	legacy_u8 contact = shape3d_shadows_sample(-20, 0, 0);
	assert(contact > 0 && contact < 34);
	assert(shape3d_shadows_sample(-80, 0, 0) == 0);
	assert(shape3d_shadows_sample(-20, 128, 0) == 0);
	add_roof(0, 64, 0, 128, 0);
	legacy_u8 single = shape3d_shadows_sample(32, 0, 24);
	add_roof(0, 64, 0, 128, 0);
	assert(shape3d_shadows_sample(32, 0, 24) == single);
}

static void test_separate_contact_levels(void)
{
	shape3d_shadows_begin(&origin);
	/* Different overhead layers must not become a fictional vertical wall.
	 * This point lies beyond their directional shadows, near their edge. */
	add_roof(0, 32, 0, 128, 0);
	add_roof(0, 512, 0, 128, 0);
	assert(shape3d_shadows_sample(-140, 128, 0) == 0);
	shape3d_shadows_begin(&origin);
	add_roof(0, 512, 0, 128, 0);
	add_roof(0, 32, 0, 128, 0);
	assert(shape3d_shadows_sample(-140, 128, 0) == 0);
}

static void test_receiver_planes_and_grille_holes(void)
{
	const struct SHAPE3D_HIRES_VECTOR left[] = {
		{-64, 0, -128}, {-64, 128, -128}, {-64, 128, 128}, {-64, 0, 128}};
	const struct SHAPE3D_HIRES_VECTOR right[] = {
		{64, 0, -128}, {64, 128, -128}, {64, 128, 128}, {64, 0, 128}};
	shape3d_shadows_begin(&origin);
	add_roof(0, 128, 0, 64, 0);
	shape3d_shadows_add_polygon(left, 4, 0);
	shape3d_shadows_add_polygon(right, 4, 0);
	for (legacy_s32 y = 8; y < 120; y++) {
		for (legacy_s32 z = -48; z <= 48; z += 7) {
			assert(shape3d_shadows_sample_plane(-64, y, z, -1, 0, 0) == 0);
		}
	}
	shape3d_shadows_begin(&origin);
	add_roof(0, 64, 0, 256, 0);
	add_roof(0, 192, 0, 256, 1);
	legacy_u8 minimum = 255;
	legacy_u8 maximum = 0;
	for (legacy_s32 x = 0; x < 128; x++) {
		legacy_u8 shade = shape3d_shadows_sample((x % 16) * 2, 64, 40 + (x / 16) * 2);
		if (shade < minimum) {
			minimum = shade;
		}
		if (shade > maximum) {
			maximum = shade;
		}
	}
	assert(minimum == 0 && maximum >= 60);
}

static void test_generation_wrap(void)
{
	shape3d_shadows_shutdown();
	shape3d_shadows_begin(&origin);
	add_roof(0, 128, 0, 64, 0);
	assert(shape3d_shadows_sample(32, 0, 24) > 0);
	for (legacy_u32 generation = 0; generation < 65535U; generation++) {
		shape3d_shadows_begin(&origin);
	}
	/* Generation one is reused. The old caster must not reappear. */
	assert(shape3d_shadows_sample(32, 0, 24) == 0);
}

static void test_map_bounds_and_reset(void)
{
	shape3d_shadows_begin(&origin);
	add_roof(0, 128, 0, 8192, 0);
	legacy_u8 central = shape3d_shadows_sample(0, 0, 0);
	legacy_u8 edge = shape3d_shadows_sample(1900, 0, 0);
	assert(central >= 78);
	assert(edge > 0 && edge < central);
	assert(shape3d_shadows_sample(2048, 0, 0) == 0);
	assert(shape3d_shadows_sample(-5000, 0, 0) == 0);
	shape3d_shadows_reset();
	assert(!shape3d_shadows_active());
	assert(shape3d_shadows_sample(0, 0, 0) == 0);
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_sample(0, 0, 0) == 0);
	shape3d_shadows_shutdown();
	assert(!shape3d_shadows_active());
	shape3d_shadows_begin(NULL);
	assert(!shape3d_shadows_active());
}

static legacy_u8 cached_sample(legacy_f64 x, legacy_f64 y, legacy_f64 z)
{
	legacy_u32 hint = 0;
	return shape3d_shadows_sample_cached(x, y, z, 1, &hint);
}

static void test_static_cache_lifetime_and_camera_stability(void)
{
	const struct VECTOR cameras[] = {{0, 0, 0}, {1081, 73, 947}, {8000, 200, 8000}};
	shape3d_shadows_invalidate();
	assert(!shape3d_shadows_baked());
	assert(shape3d_shadows_bake_begin());
	add_roof(1024, 128, 1024, 128, 0);
	shape3d_shadows_bake_end();
	assert(shape3d_shadows_baked());
	legacy_u8 first = 0;
	/* All presentations reuse the static bake. Moving the camera outside the
	 * old local map must not remove shadows from this world-space receiver. */
	for (legacy_u32 frame = 0; frame < 12; frame++) {
		const struct VECTOR *camera = &cameras[frame % 3];
		shape3d_shadows_begin(camera);
		legacy_u8 shade = cached_sample(1056 - camera->x, -camera->y, 1048 - camera->z);
		assert(shade > 0);
		if (frame == 0) {
			first = shade;
		} else {
			assert(shade == first);
		}
		assert(cached_sample(1500 - camera->x, -camera->y, 1500 - camera->z) == 0);
		shape3d_shadows_reset();
		assert(!shape3d_shadows_active());
		assert(shape3d_shadows_baked());
	}
	/* Loading an empty replacement track cannot retain the previous roof. */
	assert(shape3d_shadows_bake_begin());
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	assert(cached_sample(1056, 0, 1048) == 0);
	shape3d_shadows_invalidate();
	assert(!shape3d_shadows_baked());
}

static void test_cached_static_and_animated_layers(void)
{
	shape3d_shadows_invalidate();
	assert(shape3d_shadows_bake_begin());
	add_roof(1024, 128, 1024, 128, 0);
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	legacy_u8 fixed = cached_sample(1056, 0, 1048);
	assert(fixed > 0);
	assert(cached_sample(1568, 0, 1048) == 0);
	/* An animated blade contributes only to this presentation. Moving it
	 * neither erases static shade nor leaves its old dynamic silhouette. */
	add_roof(1536, 128, 1024, 128, 0);
	assert(cached_sample(1568, 0, 1048) > 0);
	assert(cached_sample(1056, 0, 1048) == fixed);
	shape3d_shadows_begin(&origin);
	add_roof(512, 128, 1024, 128, 0);
	assert(cached_sample(1568, 0, 1048) == 0);
	assert(cached_sample(544, 0, 1048) > 0);
	assert(cached_sample(1056, 0, 1048) == fixed);
	shape3d_shadows_invalidate();
}

static void test_dense_cached_grille_on_elevated_receiver(void)
{
	shape3d_shadows_invalidate();
	assert(shape3d_shadows_bake_begin());
	add_roof(1024, 64, 1024, 256, 0);
	add_roof(1024, 192, 1024, 256, 1);
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	legacy_u32 total = 0;
	legacy_u32 transitions = 0;
	legacy_u8 minimum = 255;
	legacy_u8 maximum = 0;
	legacy_u8 previous = cached_sample(1024, 64, 1084);
	for (legacy_s32 x = 0; x < 64; x++) {
		legacy_u8 shade = cached_sample(1024 + x, 64, 1084);
		total += shade;
		transitions += (shade >= 40) != (previous >= 40);
		previous = shade;
		if (shade < minimum) {
			minimum = shade;
		}
		if (shade > maximum) {
			maximum = shade;
		}
	}
	/* The lattice is visibly finer than the former 32-unit cells and must
	 * retain holes instead of turning into an unbroken solid shadow. */
	assert(transitions >= 8);
	assert(minimum < 35 && maximum >= 55);
	assert(total > 64 * 30);
	legacy_u32 hint = 0;
	legacy_u8 distant = shape3d_shadows_sample_cached(1056, 64, 1084, 32, &hint);
	assert(distant > 30 && distant < 78);
	/* Adjacent filtered resolutions must blend continuously. Crossing a mip
	 * boundary while approaching a bridge must not draw horizontal bands. */
	for (legacy_s32 footprint = 2; footprint <= 16; footprint *= 2) {
		for (legacy_s32 x = 0; x < 16; x++) {
			legacy_u8 before =
				shape3d_shadows_sample_cached(1024 + x, 64, 1084, footprint - 0.001, &hint);
			legacy_u8 after =
				shape3d_shadows_sample_cached(1024 + x, 64, 1084, footprint + 0.001, &hint);
			legacy_s32 difference = (legacy_s32)after - before;
			assert(difference >= -1 && difference <= 1);
		}
	}
	shape3d_shadows_invalidate();
}

static void test_cached_view_range_and_dynamic_fade(void)
{
	assert(shape3d_shadows_bake_begin());
	add_roof(0, 128, 0, 128, 0);
	add_roof(1850, 128, 0, 128, 0);
	add_roof(2550, 128, 0, 128, 0);
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	legacy_u32 hint = 0;
	legacy_u8 center = shape3d_shadows_sample_cached_view(0, 0, 0, 1, &hint);
	legacy_u8 edge = shape3d_shadows_sample_cached_view(1900, 0, 0, 1, &hint);
	assert(center > 0 && edge > 0 && edge < center);
	assert(shape3d_shadows_sample_cached_view(2600, 0, 0, 1, &hint) == 0);
	assert(cached_sample(2600, 0, 0) > 0);
	const struct VECTOR moved = {2600, 0, 0};
	shape3d_shadows_begin(&moved);
	assert(shape3d_shadows_sample_cached_view(0, 0, 0, 1, &hint) == cached_sample(0, 0, 0));
	/* The transient map already fades its edge. Combining it with the static
	 * view sampler must not attenuate a moving sail's shadow a second time. */
	assert(shape3d_shadows_bake_begin());
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	add_roof(1850, 128, 0, 128, 0);
	legacy_u8 moving = shape3d_shadows_sample(1900, 0, 0);
	assert(moving > 0);
	assert(shape3d_shadows_sample_cached_view(1900, 0, 0, 1, &hint) == moving);
	shape3d_shadows_invalidate();
}

static void test_vertical_grille_projects_holes(void)
{
	const struct SHAPE3D_HIRES_VECTOR wall[] = {
		{0, 0, -128}, {0, 192, -128}, {0, 192, 128}, {0, 0, 128}};
	legacy_u8 solid[41 * 41];
	legacy_u32 holes = 0;
	for (legacy_s32 grille = 0; grille < 2; grille++) {
		assert(shape3d_shadows_bake_begin());
		shape3d_shadows_add_polygon(wall, 4, grille);
		shape3d_shadows_bake_end();
		shape3d_shadows_begin(&origin);
		legacy_u32 index = 0;
		for (legacy_s32 z = -40; z <= 40; z += 2) {
			for (legacy_s32 x = 20; x <= 60; x++) {
				legacy_u8 shade = cached_sample(x, 0, z);
				if (!grille) {
					assert(shade >= 70);
					solid[index] = shade;
				} else {
					holes += shade + 10 < solid[index];
				}
				index++;
			}
		}
	}
	/* A vertical grille needs a lattice in its own plane. Sampling only X/Z
	 * would collapse this wall's fixed X coordinate into one solid bar. */
	assert(holes > 100);
	shape3d_shadows_invalidate();
}

#define DISK_SAMPLE_COUNT (3U * 3U * 3U * 16U)

static void disk_scene(legacy_s32 revision)
{
	assert(shape3d_shadows_bake_begin());
	add_roof(1024, 64, 1024, 256, 0);
	add_roof(1024 + revision * 512, 192, 1024, 256, 1);
}

static void disk_samples(legacy_u8 samples[DISK_SAMPLE_COUNT])
{
	const struct VECTOR cameras[] = {{0, 0, 0}, {1037, 91, 983}, {6000, 200, 5000}};
	const legacy_s32 heights[] = {0, 64, 192};
	const legacy_s32 footprints[] = {1, 4, 16};
	legacy_u32 index = 0;
	for (legacy_u32 camera = 0; camera < 3; camera++) {
		shape3d_shadows_begin(&cameras[camera]);
		for (legacy_u32 height = 0; height < 3; height++) {
			for (legacy_u32 level = 0; level < 3; level++) {
				legacy_u32 hint = 0;
				for (legacy_u32 point = 0; point < 16; point++) {
					samples[index++] = shape3d_shadows_sample_cached(
						1032.0 + (point % 8) * 3 - cameras[camera].x,
						heights[height] - cameras[camera].y,
						1032.0 + (point / 8) * 7 - cameras[camera].z, footprints[level], &hint);
					if (camera != 0) {
						assert(samples[index - 1] ==
							   samples[(index - 1) % (DISK_SAMPLE_COUNT / 3)]);
					}
				}
			}
		}
	}
	assert(index == DISK_SAMPLE_COUNT);
}

static legacy_u8 *disk_read(const char *path, size_t *length)
{
	FILE *file = fopen(path, "rb");
	assert(file != NULL);
	assert(fseek(file, 0, SEEK_END) == 0);
	long size = ftell(file);
	assert(size > 68 && size < 4 * 1024 * 1024);
	assert(fseek(file, 0, SEEK_SET) == 0);
	*length = (size_t)size;
	legacy_u8 *bytes = malloc(*length + 1);
	assert(bytes != NULL);
	assert(fread(bytes, 1, *length, file) == *length);
	assert(fclose(file) == 0);
	return bytes;
}

static void disk_write(const char *path, const legacy_u8 *bytes, size_t length)
{
	FILE *file = fopen(path, "wb");
	assert(file != NULL);
	assert(fwrite(bytes, 1, length, file) == length);
	assert(fclose(file) == 0);
}

static void disk_u32(legacy_u8 *bytes, legacy_u32 value)
{
	for (legacy_u32 index = 0; index < 4; index++) {
		bytes[index] = (legacy_u8)(value >> (index * 8));
	}
}

static void disk_repair_checksum(legacy_u8 *bytes, size_t length)
{
	legacy_u64 checksum = 14695981039346656037ULL;
	for (size_t index = 0; index < length - 8; index++) {
		checksum = (checksum ^ bytes[index]) * 1099511628211ULL;
	}
	for (legacy_u32 index = 0; index < 8; index++) {
		bytes[length - 8 + index] = (legacy_u8)(checksum >> (index * 8));
	}
}

static void test_disk_cache_roundtrip_and_recovery(void)
{
	legacy_u8 track_md5[16] = {0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,
							   0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72};
	char path[128];
	int written =
		snprintf(path, sizeof(path), "restunts-shadow-cache-%lu.LMP", (unsigned long)getpid());
	assert(written > 0 && (size_t)written < sizeof(path));
	/* Each process owns a unique fixture and never removes an existing file. */
	FILE *existing = fopen(path, "rb");
	assert(existing == NULL);
	legacy_u8 reference[DISK_SAMPLE_COUNT];
	legacy_u8 loaded[DISK_SAMPLE_COUNT];
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 0);
	assert(shape3d_shadows_baked());
	legacy_u32 bytes_before = shape3d_shadows_baked_bytes();
	assert(bytes_before != 0);
	disk_samples(reference);
	shape3d_shadows_begin(&origin);
	add_roof(1536, 128, 1024, 64, 0);
	assert(cached_sample(1568, 0, 1048) > 0);
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);
	assert(shape3d_shadows_baked_bytes() == bytes_before);
	shape3d_shadows_begin(&origin);
	assert(cached_sample(1568, 0, 1048) == 0);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);
	size_t length;
	legacy_u8 *valid = disk_read(path, &length);
	assert(memcmp(valid + 12, track_md5, sizeof(track_md5)) == 0);
	assert(valid[44] == 1 && valid[45] == 0 && valid[46] == 0 && valid[47] == 0);

	/* Equal track names do not authorize stale lighting after geometry edits. */
	disk_scene(1);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 0);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) != 0);
	disk_scene(1);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);

	/* Raw original track identity is independent of rendered geometry. A
	 * changed digest alone must invalidate an otherwise matching lightmap. */
	disk_write(path, valid, length);
	track_md5[3] ^= 0x55;
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 0);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);
	track_md5[3] ^= 0x55;

	legacy_u8 *damaged = malloc(length + 1);
	assert(damaged != NULL);
	for (legacy_u32 fault = 0; fault < 15; fault++) {
		memcpy(damaged, valid, length);
		size_t damaged_length = length;
		if (fault == 0) {
			damaged[0] ^= 0x20; /* Invalid magic. */
		} else if (fault == 1) {
			damaged[8] ^= 0x7F; /* Unsupported format version. */
		} else if (fault == 2) {
			damaged[28] ^= 0x40; /* Different geometry fingerprint. */
		} else if (fault == 3 || fault == 4) {
			/* Counts are untrusted and cannot drive unbounded allocations. */
			memset(damaged + (fault == 3 ? 36 : 40), 0xFF, 4);
		} else if (fault == 5) {
			damaged[length / 2] ^= 0x40; /* Corrupt texels or metadata. */
		} else if (fault == 6) {
			damaged_length = 12; /* Truncated header. */
		} else if (fault == 7) {
			damaged_length = length - 1; /* Truncated body/checksum. */
		} else if (fault == 8) {
			damaged[length] = 0xAA; /* Unexpected trailing data. */
			damaged_length++;
		} else {
			/* The first face has a texture. Keep the checksum valid so these
			 * exercise record validation rather than checksum rejection. */
			const legacy_u32 offsets[] = {48, 52, 56, 60, 64, 44};
			const legacy_u32 values[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0, 0x7FC00000U, 0x7F800000U, 2};
			disk_u32(damaged + offsets[fault - 9], values[fault - 9]);
			disk_repair_checksum(damaged, length);
		}
		disk_write(path, damaged, damaged_length);
		disk_scene(0);
		assert(shape3d_shadows_bake_end_cached(path, track_md5) == 0);
		assert(shape3d_shadows_baked());
		disk_samples(loaded);
		assert(memcmp(reference, loaded, sizeof(reference)) == 0);
		/* A rejected file is replaced with a usable bake, not left to fail
		 * and rebuild again at every subsequent track load. */
		disk_scene(0);
		assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);
	}

	/* A regular file cannot be used as a directory, even when tests run as
	 * root. Failure to save must still leave fully usable memory lighting. */
	char unwritable[160];
	written = snprintf(unwritable, sizeof(unwritable), "%s/blocked.LMP", path);
	assert(written > 0 && (size_t)written < sizeof(unwritable));
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(unwritable, track_md5) == 0);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);
	assert(remove(path) == 0);
	free(valid);
	free(damaged);
	shape3d_shadows_invalidate();
}

int main(void)
{
	test_ground_and_roof();
	test_slope_and_winding();
	test_grille_and_camera_stability();
	test_contact_wall_and_overlapping_casters();
	test_separate_contact_levels();
	test_receiver_planes_and_grille_holes();
	test_generation_wrap();
	test_map_bounds_and_reset();
	test_static_cache_lifetime_and_camera_stability();
	test_cached_static_and_animated_layers();
	test_dense_cached_grille_on_elevated_receiver();
	test_vertical_grille_projects_holes();
	test_cached_view_range_and_dynamic_fade();
	test_disk_cache_roundtrip_and_recovery();
	puts("shape3d shadows tests passed");
	return 0;
}
