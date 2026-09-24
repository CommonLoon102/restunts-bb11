#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lz4.h>
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
	legacy_s64 size = ftell(file);
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

static legacy_u32 disk_read_u32(const legacy_u8 *bytes)
{
	legacy_u32 value = 0;
	for (legacy_u32 index = 0; index < 4; index++) {
		value |= (legacy_u32)bytes[index] << (index * 8);
	}
	return value;
}

static legacy_u32 disk_texture_bytes(const legacy_u8 *metadata)
{
	legacy_u32 width = disk_read_u32(metadata + 4);
	legacy_u32 height = disk_read_u32(metadata + 8);
	legacy_u32 bytes = 0;
	assert(width != 0 && height != 0);
	for (;;) {
		bytes += width * height;
		if (width == 1 && height == 1) {
			return bytes;
		}
		width = (width + 1) / 2;
		height = (height + 1) / 2;
	}
}

struct DISK_FIXTURE {
	legacy_u8 *bytes;
	size_t length, capacity;
};

static legacy_u8 *disk_append(struct DISK_FIXTURE *fixture, const void *bytes, size_t length)
{
	assert(length <= 4U * 1024U * 1024U);
	assert(fixture->length <= 4U * 1024U * 1024U - length);
	size_t needed = fixture->length + length;
	if (needed > fixture->capacity) {
		size_t capacity = fixture->capacity != 0 ? fixture->capacity : 4096;
		while (capacity < needed) {
			capacity *= 2;
		}
		legacy_u8 *replacement = realloc(fixture->bytes, capacity);
		assert(replacement != NULL);
		fixture->bytes = replacement;
		fixture->capacity = capacity;
	}
	legacy_u8 *destination = fixture->bytes + fixture->length;
	if (bytes != NULL) {
		memcpy(destination, bytes, length);
	}
	fixture->length = needed;
	return destination;
}

/* Derive independent v1 and raw-v2 fixtures from one valid compressed file.
 * This also lets migration compare every stored texel, including all mipmaps,
 * instead of relying only on the sampled lighting colors. */
static legacy_u8 *disk_expand(const legacy_u8 *bytes, size_t length, legacy_u32 version,
							  size_t *expanded_length)
{
	assert(length >= 52 && memcmp(bytes, "RSLMAP02", 8) == 0);
	assert(disk_read_u32(bytes + 8) == 2);
	assert(version == 1 || version == 2);
	struct DISK_FIXTURE fixture = {0};
	disk_append(&fixture, bytes, 44);
	memcpy(fixture.bytes, version == 1 ? "RSLMAP01" : "RSLMAP02", 8);
	disk_u32(fixture.bytes + 8, version);
	legacy_u32 faces = disk_read_u32(bytes + 36);
	legacy_u32 pages = disk_read_u32(bytes + 40);
	size_t cursor = 44;
	for (legacy_u32 index = 0; index < faces + pages; index++) {
		if (index >= faces) {
			assert(cursor + 12 <= length - 8);
			disk_append(&fixture, bytes + cursor, 12);
			cursor += 12;
		}
		assert(cursor + 4 <= length - 8);
		legacy_u32 present = disk_read_u32(bytes + cursor);
		assert(present <= 1);
		if (!present) {
			disk_append(&fixture, bytes + cursor, 4);
			cursor += 4;
			continue;
		}
		assert(cursor + 28 <= length - 8);
		legacy_u32 decoded = disk_texture_bytes(bytes + cursor);
		disk_append(&fixture, bytes + cursor, 24);
		legacy_u32 stored = disk_read_u32(bytes + cursor + 24);
		cursor += 28;
		assert(stored != 0 && stored <= decoded && stored <= length - 8 - cursor);
		if (version == 2) {
			legacy_u8 size[4];
			disk_u32(size, decoded);
			disk_append(&fixture, size, sizeof(size));
		}
		legacy_u8 *pixels = disk_append(&fixture, NULL, decoded);
		if (stored == decoded) {
			memcpy(pixels, bytes + cursor, decoded);
		} else {
			assert(LZ4_decompress_safe((const char *)bytes + cursor, (char *)pixels,
									   (legacy_s32)stored,
									   (legacy_s32)decoded) == (legacy_s32)decoded);
		}
		cursor += stored;
	}
	assert(cursor == length - 8);
	disk_append(&fixture, NULL, 8);
	disk_repair_checksum(fixture.bytes, fixture.length);
	*expanded_length = fixture.length;
	return fixture.bytes;
}

static void test_disk_cache_roundtrip_and_recovery(void)
{
	legacy_u8 track_md5[16] = {0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,
							   0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72};
	char path[128];
	legacy_s32 written = snprintf(
		path, sizeof(path), "restunts-shadow-cache-%" LEGACY_PRIu32 ".LMP", (legacy_u32)getpid());
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
	assert(memcmp(valid, "RSLMAP02", 8) == 0 && disk_read_u32(valid + 8) == 2);
	legacy_u32 first_decoded = disk_texture_bytes(valid + 44);
	legacy_u32 first_stored = disk_read_u32(valid + 68);
	assert(first_stored > 1 && first_stored < first_decoded);
	size_t legacy_length, raw_length;
	legacy_u8 *legacy = disk_expand(valid, length, 1, &legacy_length);
	legacy_u8 *raw = disk_expand(valid, length, 2, &raw_length);
	/* This grille-and-contact fixture has enough texture data for the on-disk
	 * size target to be meaningful, rather than dominated by a tiny header. */
	assert(length * 10 <= legacy_length);
	disk_write(path, raw, raw_length);
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);
	assert(shape3d_shadows_baked_bytes() == bytes_before);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);

	/* A valid legacy file is a cache hit and is rewritten without rebaking. */
	disk_write(path, legacy, legacy_length);
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(path, track_md5) == 1);
	assert(shape3d_shadows_baked_bytes() == bytes_before);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);
	size_t migrated_length, restored_length;
	legacy_u8 *migrated = disk_read(path, &migrated_length);
	assert(memcmp(migrated, "RSLMAP02", 8) == 0);
	assert(migrated_length * 10 <= legacy_length);
	legacy_u8 *restored = disk_expand(migrated, migrated_length, 1, &restored_length);
	assert(restored_length == legacy_length);
	assert(memcmp(restored, legacy, legacy_length) == 0);
	free(restored);
	free(migrated);

#if defined(__linux__)
	/* /proc permits reading this descriptor but cannot accept the temporary
	 * sibling required by an atomic save, even when the tests run as root. */
	disk_write(path, legacy, legacy_length);
	FILE *read_only = fopen(path, "rb");
	assert(read_only != NULL);
	char descriptor_path[64];
	written =
		snprintf(descriptor_path, sizeof(descriptor_path), "/proc/self/fd/%d", fileno(read_only));
	assert(written > 0 && (size_t)written < sizeof(descriptor_path));
	disk_scene(0);
	assert(shape3d_shadows_bake_end_cached(descriptor_path, track_md5) == 1);
	disk_samples(loaded);
	assert(memcmp(reference, loaded, sizeof(reference)) == 0);
	assert(fclose(read_only) == 0);
	legacy_u8 *unchanged = disk_read(path, &restored_length);
	assert(restored_length == legacy_length);
	assert(memcmp(unchanged, legacy, legacy_length) == 0);
	free(unchanged);
#endif
	free(raw);
	free(legacy);

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
	for (legacy_u32 fault = 0; fault < 22; fault++) {
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
		} else if (fault < 15) {
			/* The first face has a texture. Keep the checksum valid so these
			 * exercise record validation rather than checksum rejection. */
			const legacy_u32 offsets[] = {48, 52, 56, 60, 64, 44};
			const legacy_u32 values[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0, 0x7FC00000U, 0x7F800000U, 2};
			disk_u32(damaged + offsets[fault - 9], values[fault - 9]);
			disk_repair_checksum(damaged, length);
		} else if (fault == 15 || fault == 16) {
			/* No zero-length payload or larger-than-decoded allocation request. */
			disk_u32(damaged + 68, fault == 15 ? 0 : first_decoded + 1);
			disk_repair_checksum(damaged, length);
		} else if (fault == 17) {
			/* A valid empty LZ4 block with correctly framed following records
			 * cannot satisfy the declared mip byte count. */
			memmove(damaged + 73, damaged + 72 + first_stored, length - 72 - first_stored);
			damaged_length -= first_stored - 1;
			disk_u32(damaged + 68, 1);
			damaged[72] = 0;
			disk_repair_checksum(damaged, damaged_length);
		} else if (fault == 18 || fault == 19) {
			/* Malformed match tokens and truncated literal-length extensions. */
			memset(damaged + 72, fault == 18 ? 0 : 255, first_stored);
			disk_repair_checksum(damaged, length);
		} else if (fault == 20) {
			/* Remove the final compressed byte but retain correctly framed later
			 * records and a matching checksum: exact decoded length is required. */
			memmove(damaged + 72 + first_stored - 1, damaged + 72 + first_stored,
					length - 72 - first_stored);
			damaged_length--;
			disk_u32(damaged + 68, first_stored - 1);
			disk_repair_checksum(damaged, damaged_length);
		} else {
			damaged[length - 1] ^= 1; /* Valid compressed data, incorrect checksum. */
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

static legacy_f32 gpu_float(legacy_u32 word)
{
	legacy_f32 value;
	memcpy(&value, &word, sizeof(value));
	return value;
}

static void check_gpu_texture(const struct SHAPE3D_SHADOWS_GPU_DATA *export, legacy_u32 offset)
{
	legacy_u32 words = export->static_bytes / sizeof(legacy_u32);
	assert(offset <= words && words - offset >= 28);
	const legacy_u32 *texture = export->static_words + offset;
	if (texture[2] == 0) {
		return;
	}
	assert(texture[0] != 0 && texture[1] != 0 && texture[2] <= 16);
	assert(gpu_float(texture[6]) > 0 && gpu_float(texture[7]) > 0);
	legacy_u32 width = texture[0], height = texture[1];
	for (legacy_u32 level = 0; level < texture[2]; level++) {
		legacy_u32 bytes = texture[9 + level] + width * height;
		assert(texture[8] <= words && (bytes + 3) / 4 <= words - texture[8]);
		width = (width + 1) / 2;
		height = (height + 1) / 2;
	}
}

static void test_gpu_export_lifetime_and_sparse_maps(void)
{
	struct SHAPE3D_SHADOWS_GPU_DATA exported;
	shape3d_shadows_shutdown();
	assert(!shape3d_shadows_gpu_export(NULL));
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.static_bytes == 16 * sizeof(legacy_u32));
	assert(exported.frame_bytes == 32 * sizeof(legacy_u32));
	assert(exported.frame_words[0] == 0 && exported.static_revision != 0);
	legacy_u32 initial_revision = exported.static_revision;
	shape3d_shadows_begin(&origin);
	assert(!shape3d_shadows_gpu_export(&exported));
	assert(exported.static_words == NULL && exported.frame_words == NULL);
	assert(shape3d_shadows_bake_begin());
	add_roof(0, 64, 0, 128, 1);
	/* An irregular receiver exercises edge packing; the upper roof has no
	 * occluders, so missing texture storage must also export safely. */
	const struct SHAPE3D_HIRES_VECTOR triangle[] = {
		{-128, 32, -128}, {128, 48, -128}, {0, 40, 128}};
	shape3d_shadows_add_polygon(triangle, 3, 0);
	shape3d_shadows_bake_end();
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.static_revision != initial_revision);
	assert(exported.static_words[1] == 2 && exported.static_words[3] != 0);
	const legacy_u32 *static_words = exported.static_words;
	legacy_u32 revision = exported.static_revision;
	legacy_u32 bytes = exported.static_bytes;
	legacy_u32 *snapshot = malloc(bytes);
	assert(snapshot != NULL);
	memcpy(snapshot, static_words, bytes);
	for (legacy_u32 i = 0; i < static_words[1]; i++) {
		legacy_u32 face = static_words[0] + i * 20;
		check_gpu_texture(&exported, static_words[face + 17]);
		if (!static_words[face + 14]) {
			assert(static_words[face + 16] == 3);
			assert(static_words[face + 15] + 3 * 5 <= bytes / sizeof(legacy_u32));
		}
	}
	for (legacy_u32 i = 0; i < static_words[6]; i++) {
		check_gpu_texture(&exported, static_words[static_words[5] + i * 4 + 2]);
	}
	const struct VECTOR moved = {123, 40, -81};
	shape3d_shadows_begin(&moved);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.static_words == static_words && exported.static_revision == revision);
	assert(exported.frame_words[0] == 1 && exported.frame_words[1] == 0);
	assert(gpu_float(exported.frame_words[2]) == moved.x);
	assert(gpu_float(exported.frame_words[3]) == moved.y);
	assert(gpu_float(exported.frame_words[4]) == moved.z);
	add_roof(100 - moved.x, 100 - moved.y, 50 - moved.z, 32, 0);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[1] == 1);
	/* Small moving casters upload a small rectangular region, independent
	 * of the complete 512x512 shadow-map allocation. */
	assert(exported.frame_bytes > 32 * sizeof(legacy_u32) && exported.frame_bytes < 8192);
	for (legacy_u32 rectangle = 17; rectangle <= 22; rectangle += 5) {
		const legacy_u32 *map = exported.frame_words + rectangle;
		assert(map[0] + map[2] <= 512 && map[1] + map[3] <= 512);
		assert(map[2] > 0 && map[3] > 0);
		legacy_u32 stride = rectangle == 17 ? 1 : 2;
		assert(map[4] + map[2] * map[3] * stride <= exported.frame_bytes / sizeof(legacy_u32));
	}
	assert(exported.static_words == static_words && exported.static_revision == revision);
	assert(exported.static_bytes == bytes && memcmp(snapshot, static_words, bytes) == 0);
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[1] == 0 && exported.frame_bytes == 32 * sizeof(legacy_u32));
	shape3d_shadows_reset();
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[0] == 0 && exported.static_revision == revision);
	shape3d_shadows_invalidate();
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.static_revision != revision &&
		   exported.static_bytes == 16 * sizeof(legacy_u32));
	free(snapshot);
	/* Empty tracks still have a valid baked cache and must not force CPU
	 * fallback or read nonexistent BSP roots. */
	assert(shape3d_shadows_bake_begin());
	shape3d_shadows_bake_end();
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[0] == 1 && exported.static_words[1] == 0);
	shape3d_shadows_shutdown();
}

static void assert_shadow_samples_disabled(legacy_f64 x, legacy_f64 y, legacy_f64 z)
{
	legacy_u32 hint = 123;
	assert(shape3d_shadows_sample(x, y, z) == 0);
	assert(shape3d_shadows_sample_plane(x, y, z, 0, 1, 0) == 0);
	assert(shape3d_shadows_sample_cached(x, y, z, 1, &hint) == 0);
	assert(shape3d_shadows_sample_cached_view(x, y, z, 1, &hint) == 0);
	assert(hint == 123);
}

static void test_presentation_setting_retains_cache(void)
{
	struct SHAPE3D_SHADOWS_GPU_DATA exported;
	shape3d_shadows_shutdown();
	assert(shape3d_shadows_enabled());
	shape3d_shadows_begin(&origin);
	add_roof(0, 64, 0, 128, 0);
	assert(shape3d_shadows_sample(32, 0, 24) >= 78);
	shape3d_shadows_set_enabled(0);
	assert(!shape3d_shadows_enabled() && !shape3d_shadows_active());
	assert_shadow_samples_disabled(32, 0, 24);
	shape3d_shadows_begin(&origin);
	add_roof(0, 64, 0, 128, 0);
	assert(!shape3d_shadows_active());
	assert_shadow_samples_disabled(32, 0, 24);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[0] == 0 && exported.frame_words[1] == 0);
	shape3d_shadows_set_enabled(2);
	assert(shape3d_shadows_enabled() == 1 && !shape3d_shadows_active());
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_sample(32, 0, 24) == 0);
	add_roof(0, 64, 0, 128, 0);
	assert(shape3d_shadows_sample(32, 0, 24) >= 78);

	/* Explicit preloading remains possible without enabling frame shadows.
	 * Its capture-active state must not make runtime sampling active. */
	shape3d_shadows_set_enabled(0);
	assert(shape3d_shadows_bake_begin());
	assert(shape3d_shadows_active() && !shape3d_shadows_enabled());
	add_roof(0, 128, 0, 128, 0);
	shape3d_shadows_set_enabled(0);
	assert(shape3d_shadows_active());
	add_roof(512, 128, 0, 128, 0);
	assert_shadow_samples_disabled(32, 0, 24);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[0] == 0 && exported.frame_words[1] == 0);
	shape3d_shadows_bake_end();
	assert(shape3d_shadows_baked() && !shape3d_shadows_active());
	assert(shape3d_shadows_gpu_export(&exported));
	const legacy_u32 *static_words = exported.static_words;
	legacy_u32 revision = exported.static_revision;
	legacy_u32 static_bytes = exported.static_bytes;
	legacy_u32 baked_bytes = shape3d_shadows_baked_bytes();
	assert(exported.static_words[1] == 2);
	shape3d_shadows_set_enabled(1);
	assert(!shape3d_shadows_active());
	shape3d_shadows_begin(&origin);
	legacy_u8 fixed = shape3d_shadows_sample_cached_view(32, 0, 24, 1, NULL);
	assert(fixed >= 78);
	assert(shape3d_shadows_sample_cached_view(544, 0, 24, 1, NULL) >= 78);
	add_roof(1024, 128, 0, 128, 0);
	assert(shape3d_shadows_sample_cached_view(1056, 0, 24, 1, NULL) >= 78);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[1] == 1);

	shape3d_shadows_set_enabled(0);
	assert_shadow_samples_disabled(32, 0, 24);
	assert_shadow_samples_disabled(1056, 0, 24);
	for (legacy_s32 frame = 0; frame < 3; frame++) {
		struct VECTOR moved = {123, 40, -81};
		shape3d_shadows_begin(&moved);
		add_roof(1536 - moved.x, 128 - moved.y, -moved.z, 128, 0);
		assert(!shape3d_shadows_active());
		assert(shape3d_shadows_gpu_export(&exported));
		assert(exported.frame_words[0] == 0 && exported.frame_words[1] == 0);
		assert(exported.frame_bytes == 32 * sizeof(legacy_u32));
		assert(exported.static_words == static_words && exported.static_revision == revision);
		assert(exported.static_bytes == static_bytes &&
			   shape3d_shadows_baked_bytes() == baked_bytes);
	}
	shape3d_shadows_set_enabled(1);
	assert_shadow_samples_disabled(32, 0, 24);
	shape3d_shadows_begin(&origin);
	assert(shape3d_shadows_sample_cached_view(32, 0, 24, 1, NULL) == fixed);
	assert(shape3d_shadows_sample_cached_view(1056, 0, 24, 1, NULL) == 0);
	assert(shape3d_shadows_sample_cached_view(1568, 0, 24, 1, NULL) == 0);
	assert(shape3d_shadows_gpu_export(&exported));
	assert(exported.frame_words[0] == 1 && exported.frame_words[1] == 0);
	assert(exported.static_words == static_words && exported.static_revision == revision);
	shape3d_shadows_shutdown();
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
	test_gpu_export_lifetime_and_sparse_maps();
	test_presentation_setting_retains_cache();
	puts("shape3d shadows tests passed");
	return 0;
}
