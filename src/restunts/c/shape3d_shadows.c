#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "shape3d_shadows.h"

#if defined(RESTUNTS_SDL3)

#include <float.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#if defined(_WIN32)
#include <io.h>
#include <sys/stat.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>

#define SHADOW_MAP_SIZE 512
#define SHADOW_TEXEL_SIZE 8.0F
#define SHADOW_MAP_HALF_EXTENT (SHADOW_MAP_SIZE * SHADOW_TEXEL_SIZE * 0.5F)
#define SHADOW_FADE_START (SHADOW_MAP_HALF_EXTENT * 0.75F)
#define SHADOW_LIGHT_X 0.45F
#define SHADOW_LIGHT_Z 0.30F
#define SHADOW_GRILLE_PERIOD 32.0F
#define SHADOW_GRILLE_BAR 8.0F
#define SHADOW_DEPTH_BIAS 2.5F
#define SHADOW_MAX_OPACITY 78.0F
#define SHADOW_AMBIENT_OPACITY 34.0F
#define SHADOW_CONTACT_HEIGHT 96.0F
#define SHADOW_CONTACT_TILE_SIZE 16
#define SHADOW_CONTACT_TILE_COUNT (SHADOW_MAP_SIZE / SHADOW_CONTACT_TILE_SIZE)

struct SHADOW_TEXEL {
	legacy_f32 height;
};

struct SHADOW_CONTACT {
	legacy_f32 low, high;
};

struct SHADOW_VERTEX {
	legacy_f64 x, y, z;
};

static struct SHADOW_TEXEL *light_map;
static struct SHADOW_CONTACT *contact_map;
static legacy_u16 *light_generations, *contact_generations;
static legacy_u16 generation;
/* Conservative bounds reject empty or distant contact neighborhoods before
 * their eight taps. Each tile covers 128 world units. */
static struct SHADOW_CONTACT contact_tiles[SHADOW_CONTACT_TILE_COUNT * SHADOW_CONTACT_TILE_COUNT];
static struct VECTOR camera;
static legacy_f64 map_left, map_top;
static legacy_s32 active;

static legacy_s32 cache_collecting, cache_ready;
static legacy_u32 dynamic_polygons;
static legacy_f64 dynamic_light_low[2], dynamic_light_high[2];
static legacy_f64 dynamic_contact_low[3], dynamic_contact_high[3];
static void cache_add_polygon(const struct SHAPE3D_HIRES_VECTOR *vertices, legacy_u32 count,
							  legacy_s32 grille);

static legacy_f64 shadow_absolute(legacy_f64 value)
{
	return value < 0 ? -value : value;
}

static legacy_s32 shadow_floor(legacy_f64 value)
{
	legacy_s32 result = (legacy_s32)value;
	return result > value ? result - 1 : result;
}

void shape3d_shadows_reset(void)
{
	active = 0;
}

static void shadow_release_dynamic(void)
{
	free(light_map);
	free(contact_map);
	free(light_generations);
	free(contact_generations);
	light_generations = NULL;
	contact_generations = NULL;
	generation = 0;
	light_map = NULL;
	contact_map = NULL;
	active = 0;
}

void shape3d_shadows_shutdown(void)
{
	shape3d_shadows_invalidate();
	shadow_release_dynamic();
}

static void shadow_begin_map(const struct VECTOR *camera_origin)
{
	active = 0;
	dynamic_polygons = 0;
	if (camera_origin == NULL) {
		return;
	}
	if (light_map == NULL) {
		light_map = malloc((size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE * sizeof(*light_map));
	}
	if (contact_map == NULL) {
		contact_map = malloc((size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE * sizeof(*contact_map));
	}
	if (light_generations == NULL) {
		light_generations =
			calloc((size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE, sizeof(*light_generations));
	}
	if (contact_generations == NULL) {
		contact_generations =
			calloc((size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE, sizeof(*contact_generations));
	}
	if (light_map == NULL || contact_map == NULL || light_generations == NULL ||
		contact_generations == NULL) {
		shadow_release_dynamic();
		active = cache_ready;
		return;
	}
	camera = *camera_origin;
	/* Snap the light grid to the world, not the moving camera. */
	map_left = shadow_floor((camera.x + SHADOW_LIGHT_X * camera.y) / SHADOW_TEXEL_SIZE) *
				   SHADOW_TEXEL_SIZE -
			   SHADOW_MAP_HALF_EXTENT;
	map_top = shadow_floor((camera.z + SHADOW_LIGHT_Z * camera.y) / SHADOW_TEXEL_SIZE) *
				  SHADOW_TEXEL_SIZE -
			  SHADOW_MAP_HALF_EXTENT;
	/* Sparse scenes touch a fraction of the map. Tags avoid clearing the
	 * geometry buffers for every presented frame. */
	generation++;
	if (generation == 0) {
		memset(light_generations, 0,
			   (size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE * sizeof(*light_generations));
		memset(contact_generations, 0,
			   (size_t)SHADOW_MAP_SIZE * SHADOW_MAP_SIZE * sizeof(*contact_generations));
		generation = 1;
	}
	for (size_t index = 0; index < sizeof(contact_tiles) / sizeof(contact_tiles[0]); index++) {
		contact_tiles[index].low = FLT_MAX;
		contact_tiles[index].high = -FLT_MAX;
	}
	active = 1;
}

void shape3d_shadows_begin(const struct VECTOR *camera_origin)
{
	for (legacy_s32 axis = 0; axis < 2; axis++) {
		dynamic_light_low[axis] = FLT_MAX;
		dynamic_light_high[axis] = -FLT_MAX;
	}
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		dynamic_contact_low[axis] = FLT_MAX;
		dynamic_contact_high[axis] = -FLT_MAX;
	}
	dynamic_polygons = 0;
	active = 0;
	if (camera_origin == NULL) {
		return;
	}
	camera = *camera_origin;
	if (cache_ready) {
		active = 1;
		return;
	}
	shadow_begin_map(camera_origin);
}

legacy_s32 shape3d_shadows_active(void)
{
	return active;
}

legacy_f64 shape3d_shadows_ground_height(void)
{
	return -camera.y;
}

static legacy_s32 grille_covers(legacy_f64 x, legacy_f64 z)
{
	legacy_f64 phase_x = x - shadow_floor(x / SHADOW_GRILLE_PERIOD) * SHADOW_GRILLE_PERIOD;
	legacy_f64 phase_z = z - shadow_floor(z / SHADOW_GRILLE_PERIOD) * SHADOW_GRILLE_PERIOD;
	return phase_x < SHADOW_GRILLE_BAR || phase_z < SHADOW_GRILLE_BAR;
}

static void contact_write(legacy_s32 x, legacy_s32 z, legacy_f64 low, legacy_f64 high)
{
	if (x < 0 || z < 0 || x >= SHADOW_MAP_SIZE || z >= SHADOW_MAP_SIZE) {
		return;
	}
	struct SHADOW_CONTACT *tile =
		&contact_tiles[(z / SHADOW_CONTACT_TILE_SIZE) * SHADOW_CONTACT_TILE_COUNT +
					   x / SHADOW_CONTACT_TILE_SIZE];
	if (low < tile->low) {
		tile->low = (legacy_f32)low;
	}
	if (high > tile->high) {
		tile->high = (legacy_f32)high;
	}
	size_t index = (size_t)z * SHADOW_MAP_SIZE + x;
	struct SHADOW_CONTACT *texel = &contact_map[index];
	if (contact_generations[index] != generation) {
		contact_generations[index] = generation;
		texel->low = (legacy_f32)low;
		texel->high = (legacy_f32)high;
		return;
	}
	/* Keep the lowest separate layer for ground contact. Joining disjoint
	 * decks would invent a solid wall between them and darken clear space. */
	if (low > texel->high + SHADOW_TEXEL_SIZE && texel->high != -FLT_MAX) {
		return;
	}
	if (high < texel->low - SHADOW_TEXEL_SIZE) {
		texel->low = (legacy_f32)low;
		texel->high = (legacy_f32)high;
		return;
	}
	if (low < texel->low) {
		texel->low = (legacy_f32)low;
	}
	if (high > texel->high) {
		texel->high = (legacy_f32)high;
	}
}

static legacy_s32 triangle_bounds(const struct SHADOW_VERTEX *a, const struct SHADOW_VERTEX *b,
								  const struct SHADOW_VERTEX *c, legacy_s32 *left,
								  legacy_s32 *right, legacy_s32 *top, legacy_s32 *bottom)
{
	legacy_f64 min_x = a->x < b->x ? a->x : b->x;
	legacy_f64 max_x = a->x > b->x ? a->x : b->x;
	legacy_f64 min_z = a->z < b->z ? a->z : b->z;
	legacy_f64 max_z = a->z > b->z ? a->z : b->z;
	if (c->x < min_x) {
		min_x = c->x;
	}
	if (c->x > max_x) {
		max_x = c->x;
	}
	if (c->z < min_z) {
		min_z = c->z;
	}
	if (c->z > max_z) {
		max_z = c->z;
	}
	if (max_x < 0 || max_z < 0 || min_x >= SHADOW_MAP_SIZE || min_z >= SHADOW_MAP_SIZE) {
		return 0;
	}
	*left = min_x > 0 ? shadow_floor(min_x) : 0;
	*right = max_x < SHADOW_MAP_SIZE - 1 ? shadow_floor(max_x) : SHADOW_MAP_SIZE - 1;
	*top = min_z > 0 ? shadow_floor(min_z) : 0;
	*bottom = max_z < SHADOW_MAP_SIZE - 1 ? shadow_floor(max_z) : SHADOW_MAP_SIZE - 1;
	return 1;
}

static void light_triangle(struct SHADOW_VERTEX a, struct SHADOW_VERTEX b, struct SHADOW_VERTEX c,
						   legacy_s32 grille)
{
	a.x = (a.x + SHADOW_LIGHT_X * a.y - map_left) / SHADOW_TEXEL_SIZE;
	a.z = (a.z + SHADOW_LIGHT_Z * a.y - map_top) / SHADOW_TEXEL_SIZE;
	b.x = (b.x + SHADOW_LIGHT_X * b.y - map_left) / SHADOW_TEXEL_SIZE;
	b.z = (b.z + SHADOW_LIGHT_Z * b.y - map_top) / SHADOW_TEXEL_SIZE;
	c.x = (c.x + SHADOW_LIGHT_X * c.y - map_left) / SHADOW_TEXEL_SIZE;
	c.z = (c.z + SHADOW_LIGHT_Z * c.y - map_top) / SHADOW_TEXEL_SIZE;
	legacy_f64 determinant = (b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x);
	if (shadow_absolute(determinant) < 0.00001) {
		return;
	}
	legacy_f64 gradient_x = ((b.y - a.y) * (c.z - a.z) - (c.y - a.y) * (b.z - a.z)) / determinant;
	legacy_f64 gradient_z = ((c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x)) / determinant;
	legacy_s32 left, right, top, bottom;
	if (!triangle_bounds(&a, &b, &c, &left, &right, &top, &bottom)) {
		return;
	}
	legacy_f64 inverse = 1.0 / determinant;
	legacy_f64 bary_b_x = (c.z - a.z) * inverse;
	legacy_f64 bary_b_z = -(c.x - a.x) * inverse;
	legacy_f64 bary_c_x = -(b.z - a.z) * inverse;
	legacy_f64 bary_c_z = (b.x - a.x) * inverse;
	for (legacy_s32 z = top; z <= bottom; z++) {
		legacy_f64 dz = z + 0.5 - a.z;
		legacy_f64 dx = left + 0.5 - a.x;
		legacy_f64 bary_b = dx * bary_b_x + dz * bary_b_z;
		legacy_f64 bary_c = dx * bary_c_x + dz * bary_c_z;
		legacy_f64 height = a.y + dx * gradient_x + dz * gradient_z;
		for (legacy_s32 x = left; x <= right; x++) {
			if (bary_b >= -0.00001 && bary_c >= -0.00001 && bary_b + bary_c <= 1.00001) {
				size_t index = (size_t)z * SHADOW_MAP_SIZE + x;
				struct SHADOW_TEXEL *texel = &light_map[index];
				if ((light_generations[index] != generation || height > texel->height) &&
					(!grille ||
					 grille_covers(
						 map_left + (x + 0.5) * SHADOW_TEXEL_SIZE - SHADOW_LIGHT_X * height,
						 map_top + (z + 0.5) * SHADOW_TEXEL_SIZE - SHADOW_LIGHT_Z * height))) {
					light_generations[index] = generation;
					texel->height = (legacy_f32)height;
				}
			}
			bary_b += bary_b_x;
			bary_c += bary_c_x;
			height += gradient_x;
		}
	}
}

static void contact_triangle(struct SHADOW_VERTEX a, struct SHADOW_VERTEX b, struct SHADOW_VERTEX c,
							 legacy_s32 grille)
{
	legacy_f64 min_y = a.y < b.y ? a.y : b.y;
	legacy_f64 max_y = a.y > b.y ? a.y : b.y;
	if (c.y < min_y) {
		min_y = c.y;
	}
	if (c.y > max_y) {
		max_y = c.y;
	}
	/* Flat terrain is a receiver, not an ambient occluder. */
	if (max_y <= SHADOW_DEPTH_BIAS) {
		return;
	}
	a.x = (a.x - map_left) / SHADOW_TEXEL_SIZE;
	a.z = (a.z - map_top) / SHADOW_TEXEL_SIZE;
	b.x = (b.x - map_left) / SHADOW_TEXEL_SIZE;
	b.z = (b.z - map_top) / SHADOW_TEXEL_SIZE;
	c.x = (c.x - map_left) / SHADOW_TEXEL_SIZE;
	c.z = (c.z - map_top) / SHADOW_TEXEL_SIZE;
	legacy_s32 left, right, top, bottom;
	if (!triangle_bounds(&a, &b, &c, &left, &right, &top, &bottom)) {
		return;
	}
	legacy_f64 determinant = (b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x);
	if (max_y - min_y > SHADOW_TEXEL_SIZE) {
		/* Upright faces are nearly lines from above. Their height interval
		 * gives nearby ground and wall junctions a short contact shadow. */
		legacy_f64 horizontal_x =
			shadow_absolute((b.y - a.y) * (c.z - a.z) - (c.y - a.y) * (b.z - a.z));
		legacy_f64 horizontal_z =
			shadow_absolute((c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x));
		if (shadow_absolute(determinant) * SHADOW_TEXEL_SIZE >
			(horizontal_x + horizontal_z) * 0.12) {
			return;
		}
		const struct SHADOW_VERTEX edges[] = {a, b, c, a};
		for (legacy_s32 edge = 0; edge < 3; edge++) {
			const struct SHADOW_VERTEX *first = &edges[edge];
			const struct SHADOW_VERTEX *last = &edges[edge + 1];
			legacy_f64 dx = last->x - first->x;
			legacy_f64 dz = last->z - first->z;
			legacy_f64 length = shadow_absolute(dx) > shadow_absolute(dz) ? shadow_absolute(dx)
																		  : shadow_absolute(dz);
			if (length > SHADOW_MAP_SIZE * 4) {
				length = SHADOW_MAP_SIZE * 4;
			}
			legacy_s32 steps = (legacy_s32)length + 1;
			for (legacy_s32 step = 0; step <= steps; step++) {
				legacy_f64 fraction = (legacy_f64)step / steps;
				legacy_s32 x = shadow_floor(first->x + dx * fraction);
				legacy_s32 z = shadow_floor(first->z + dz * fraction);
				contact_write(x, z, min_y, max_y);
			}
		}
		return;
	}
	if (shadow_absolute(determinant) < 0.00001) {
		return;
	}
	legacy_f64 inverse = 1.0 / determinant;
	for (legacy_s32 z = top; z <= bottom; z++) {
		for (legacy_s32 x = left; x <= right; x++) {
			legacy_f64 dx = x + 0.5 - a.x;
			legacy_f64 dz = z + 0.5 - a.z;
			legacy_f64 bary_b = (dx * (c.z - a.z) - dz * (c.x - a.x)) * inverse;
			legacy_f64 bary_c = (dz * (b.x - a.x) - dx * (b.z - a.z)) * inverse;
			if (bary_b >= -0.00001 && bary_c >= -0.00001 && bary_b + bary_c <= 1.00001 &&
				(!grille || grille_covers(map_left + (x + 0.5) * SHADOW_TEXEL_SIZE,
										  map_top + (z + 0.5) * SHADOW_TEXEL_SIZE))) {
				legacy_f64 height = a.y + bary_b * (b.y - a.y) + bary_c * (c.y - a.y);
				contact_write(x, z, height, height);
			}
		}
	}
}

void shape3d_shadows_add_polygon(const struct SHAPE3D_HIRES_VECTOR *vertices, legacy_u32 count,
								 legacy_s32 grille)
{
	if (!active || vertices == NULL || count < 3 || count > 20) {
		return;
	}
	if (cache_collecting) {
		cache_add_polygon(vertices, count, grille);
		return;
	}
	if (cache_ready && dynamic_polygons == 0) {
		shadow_begin_map(&camera);
		if (light_map == NULL) {
			return;
		}
	}
	struct SHADOW_VERTEX world[20];
	legacy_f64 maximum_y = -FLT_MAX;
	for (legacy_u32 index = 0; index < count; index++) {
		world[index] =
			(struct SHADOW_VERTEX){vertices[index].x + camera.x, vertices[index].y + camera.y,
								   vertices[index].z + camera.z};
		if (world[index].y > maximum_y) {
			maximum_y = world[index].y;
		}
	}
	/* Base ground cannot cast onto an above-ground receiver. */
	if (maximum_y <= SHADOW_DEPTH_BIAS) {
		return;
	}
	dynamic_polygons++;
	for (legacy_u32 index = 0; index < count; index++) {
		legacy_f64 point[3] = {world[index].x, world[index].y, world[index].z};
		legacy_f64 light[2] = {point[0] + SHADOW_LIGHT_X * point[1],
							   point[2] + SHADOW_LIGHT_Z * point[1]};
		for (legacy_s32 axis = 0; axis < 2; axis++) {
			if (light[axis] < dynamic_light_low[axis]) {
				dynamic_light_low[axis] = light[axis];
			}
			if (light[axis] > dynamic_light_high[axis]) {
				dynamic_light_high[axis] = light[axis];
			}
		}
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			if (point[axis] < dynamic_contact_low[axis]) {
				dynamic_contact_low[axis] = point[axis];
			}
			if (point[axis] > dynamic_contact_high[axis]) {
				dynamic_contact_high[axis] = point[axis];
			}
		}
	}
	/* Authored shape faces are convex. Fan triangles also accept reversed
	 * winding; camera backface culling must not affect shadow casting. */
	for (legacy_u32 index = 1; index + 1 < count; index++) {
		light_triangle(world[0], world[index], world[index + 1], grille);
		contact_triangle(world[0], world[index], world[index + 1], grille);
	}
}

static legacy_f32 sample_light(legacy_f32 x, legacy_f32 y, legacy_f32 z, legacy_f32 gradient_x,
							   legacy_f32 gradient_z)
{
	legacy_s32 left = shadow_floor(x - 0.5F);
	legacy_s32 top = shadow_floor(z - 0.5F);
	legacy_f32 fraction_x = x - 0.5F - left;
	legacy_f32 fraction_z = z - 0.5F - top;
	legacy_f32 coverage = 0;
	for (legacy_s32 dz = 0; dz < 2; dz++) {
		for (legacy_s32 dx = 0; dx < 2; dx++) {
			legacy_s32 column = left + dx;
			legacy_s32 row = top + dz;
			if (column < 0 || row < 0 || column >= SHADOW_MAP_SIZE || row >= SHADOW_MAP_SIZE) {
				continue;
			}
			size_t index = (size_t)row * SHADOW_MAP_SIZE + column;
			const struct SHADOW_TEXEL *texel = &light_map[index];
			if (light_generations[index] != generation) {
				continue;
			}
			/* Compare at the texel center on the receiver's plane. Extending
			 * the caster's plane instead would project neighboring roofs past
			 * their edges and produce striped self shadows on ramp walls. */
			legacy_f32 receiver =
				y + (column + 0.5F - x) * gradient_x + (row + 0.5F - z) * gradient_z;
			legacy_f32 weight =
				(dx != 0 ? fraction_x : 1 - fraction_x) * (dz != 0 ? fraction_z : 1 - fraction_z);
			if (texel->height > receiver + SHADOW_DEPTH_BIAS) {
				coverage += weight;
			}
		}
	}
	return coverage;
}

static legacy_f32 sample_contact(legacy_f32 x, legacy_f32 y, legacy_f32 z)
{
	/* Fixed short taps bound cost and keep contact darkening local. Corners
	 * use the same approximate radius as the four cardinal directions. */
	static const legacy_s8 offsets[8][2] = {{-3, 0},  {3, 0},  {0, -3}, {0, 3},
											{-2, -2}, {2, -2}, {-2, 2}, {2, 2}};
	legacy_s32 column = shadow_floor(x);
	legacy_s32 row = shadow_floor(z);
	legacy_s32 first_x = column > 3 ? (column - 3) / SHADOW_CONTACT_TILE_SIZE : 0;
	legacy_s32 first_z = row > 3 ? (row - 3) / SHADOW_CONTACT_TILE_SIZE : 0;
	legacy_s32 last_x = (column + 3) / SHADOW_CONTACT_TILE_SIZE;
	legacy_s32 last_z = (row + 3) / SHADOW_CONTACT_TILE_SIZE;
	if (last_x >= SHADOW_CONTACT_TILE_COUNT) {
		last_x = SHADOW_CONTACT_TILE_COUNT - 1;
	}
	if (last_z >= SHADOW_CONTACT_TILE_COUNT) {
		last_z = SHADOW_CONTACT_TILE_COUNT - 1;
	}
	legacy_s32 nearby = 0;
	for (legacy_s32 tz = first_z; tz <= last_z; tz++) {
		for (legacy_s32 tx = first_x; tx <= last_x; tx++) {
			const struct SHADOW_CONTACT *tile = &contact_tiles[tz * SHADOW_CONTACT_TILE_COUNT + tx];
			nearby |= tile->high > y + SHADOW_DEPTH_BIAS && tile->low < y + SHADOW_CONTACT_HEIGHT;
		}
	}
	if (!nearby) {
		return 0;
	}
	legacy_f32 coverage = 0;
	for (legacy_s32 index = 0; index < 8; index++) {
		legacy_s32 sx = column + offsets[index][0];
		legacy_s32 sz = row + offsets[index][1];
		if (sx < 0 || sz < 0 || sx >= SHADOW_MAP_SIZE || sz >= SHADOW_MAP_SIZE) {
			continue;
		}
		size_t sample = (size_t)sz * SHADOW_MAP_SIZE + sx;
		const struct SHADOW_CONTACT *texel = &contact_map[sample];
		if (contact_generations[sample] != generation || texel->high <= y + SHADOW_DEPTH_BIAS) {
			continue;
		}
		legacy_f32 gap = texel->low - y;
		if (gap < -SHADOW_DEPTH_BIAS) {
			continue;
		}
		if (gap < 0) {
			gap = 0;
		}
		if (gap < SHADOW_CONTACT_HEIGHT) {
			coverage += 1 - gap / SHADOW_CONTACT_HEIGHT;
		}
	}
	return coverage / 8;
}

legacy_u8 shape3d_shadows_sample_plane(legacy_f64 world_x, legacy_f64 world_y, legacy_f64 world_z,
									   legacy_f64 normal_x, legacy_f64 normal_y,
									   legacy_f64 normal_z)
{
	if (!active || (cache_ready && dynamic_polygons == 0)) {
		return 0;
	}
	legacy_f32 x = (legacy_f32)(world_x + camera.x);
	legacy_f32 y = (legacy_f32)(world_y + camera.y);
	legacy_f32 z = (legacy_f32)(world_z + camera.z);
	legacy_f32 light_x = x + SHADOW_LIGHT_X * y;
	legacy_f32 light_z = z + SHADOW_LIGHT_Z * y;
	legacy_f32 distance_x = shadow_absolute(light_x - map_left - SHADOW_MAP_HALF_EXTENT);
	legacy_f32 distance_z = shadow_absolute(light_z - map_top - SHADOW_MAP_HALF_EXTENT);
	legacy_f32 distance = distance_x > distance_z ? distance_x : distance_z;
	if (distance >= SHADOW_MAP_HALF_EXTENT) {
		return 0;
	}
	legacy_f64 denominator = normal_y - SHADOW_LIGHT_X * normal_x - SHADOW_LIGHT_Z * normal_z;
	legacy_f64 magnitude =
		shadow_absolute(normal_x) + shadow_absolute(normal_y) + shadow_absolute(normal_z);
	legacy_f32 shade = 0;
	if (shadow_absolute(denominator) > magnitude * 0.0001) {
		legacy_f32 gradient_x = (legacy_f32)(-normal_x * SHADOW_TEXEL_SIZE / denominator);
		legacy_f32 gradient_z = (legacy_f32)(-normal_z * SHADOW_TEXEL_SIZE / denominator);
		shade = sample_light((light_x - map_left) / SHADOW_TEXEL_SIZE, y,
							 (light_z - map_top) / SHADOW_TEXEL_SIZE, gradient_x, gradient_z) *
				SHADOW_MAX_OPACITY;
	}
	legacy_f32 contact =
		sample_contact((x - map_left) / SHADOW_TEXEL_SIZE, y, (z - map_top) / SHADOW_TEXEL_SIZE) *
		SHADOW_AMBIENT_OPACITY;
	/* Contact darkening follows supporting surfaces. Upright faces already
	 * receive cast shadows; their thin top-down footprint is unsuitable for
	 * the inexpensive contact taps. */
	contact *= magnitude > 0 ? (legacy_f32)(shadow_absolute(normal_y) / magnitude) : 0;
	/* Layer contact darkness without letting overlapping casters accumulate. */
	shade += contact * (1 - shade / 255.0F);
	if (distance > SHADOW_FADE_START) {
		shade *= (SHADOW_MAP_HALF_EXTENT - distance) / (SHADOW_MAP_HALF_EXTENT - SHADOW_FADE_START);
	}
	return (legacy_u8)(shade + 0.5F);
}

legacy_u8 shape3d_shadows_sample(legacy_f64 x, legacy_f64 y, legacy_f64 z)
{
	return shape3d_shadows_sample_plane(x, y, z, 0, 1, 0);
}

/* Static receiver textures are baked once. The BSP holds straddling faces at
 * each split, so its children occupy disjoint half spaces. Runtime sampling
 * normally stays on the previous receiver and only traverses the tree at edges. */
#define CACHE_TEXEL 2.0F
#define CACHE_COARSE_TEXEL 8.0F
#define CACHE_PAGE_SIZE 256
#define CACHE_MAX_BYTES (128U * 1024U * 1024U)
#define CACHE_MAX_MIPS 16
#define CACHE_GRILLE_PERIOD 8.0F
#define CACHE_GRILLE_BAR 4.0F
#define CACHE_RECEIVER_TOLERANCE 2.0F
#define CACHE_NONE (~0U)
#define CACHE_GROUND_HINT 0x80000000U
#define CACHE_EMPTY_HINT 0xC0000000U
#define CACHE_HINT_MASK 0xC0000000U
#define CACHE_EDGE_BYTES (8U * 1024U * 1024U)

struct CACHE_TEXTURE {
	legacy_u8 *pixels;
	legacy_u32 width, height, levels;
	legacy_u32 offsets[CACHE_MAX_MIPS];
	legacy_f32 u, v, texel, inverse_texel;
	legacy_s32 uniform;
};
struct CACHE_FACE {
	struct SHADOW_VERTEX vertices[20];
	legacy_u32 count;
	legacy_s32 grille, axis, u_axis, v_axis, rectangular, axial;
	legacy_u32 first_edge;
	legacy_f64 normal[3], plane;
	legacy_f64 low[3], high[3];
	legacy_f64 light_low[2], light_high[2];
	struct CACHE_TEXTURE texture;
};
struct CACHE_NODE {
	legacy_f64 low[3], high[3], split;
	legacy_u32 first, count, left, right;
	legacy_s32 axis;
};
struct CACHE_PAGE {
	legacy_s32 x, z, dense;
	struct CACHE_TEXTURE texture;
};
struct CACHE_EDGE {
	legacy_f64 u, v, du, dv, margin;
};
static struct CACHE_EDGE *cache_edges;
static legacy_u32 cache_edge_count;
static struct CACHE_FACE *cache_faces;
static legacy_u32 cache_face_count, cache_face_capacity;
static struct CACHE_NODE *cache_nodes;
static legacy_u32 cache_node_count;
static legacy_u32 *cache_indices;
static struct CACHE_PAGE *cache_pages;
static legacy_u32 cache_page_count, cache_page_capacity;
static legacy_u32 *cache_page_hash;
static legacy_u32 cache_hash_capacity;
static legacy_u32 cache_bytes;

static legacy_f64 cache_component(const struct SHADOW_VERTEX *vertex, legacy_s32 axis)
{
	return axis == 0 ? vertex->x : axis == 1 ? vertex->y : vertex->z;
}
static legacy_s32 cache_grille_covered(legacy_f64 u, legacy_f64 v)
{
	/* Consistent rounding keeps shared caster edges on the same lattice bar. */
	u += 0.0000001;
	v += 0.0000001;
	u -= shadow_floor(u / CACHE_GRILLE_PERIOD) * CACHE_GRILLE_PERIOD;
	v -= shadow_floor(v / CACHE_GRILLE_PERIOD) * CACHE_GRILLE_PERIOD;
	return u < CACHE_GRILLE_BAR || v < CACHE_GRILLE_BAR;
}
static legacy_u32 cache_hash(legacy_s32 x, legacy_s32 z)
{
	return (legacy_u32)x * 0x9E3779B1U ^ (legacy_u32)z * 0x85EBCA77U;
}
static void cache_discard_lighting(void)
{
	free(cache_edges);
	cache_edges = NULL;
	cache_edge_count = 0;
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		free(cache_faces[i].texture.pixels);
		memset(&cache_faces[i].texture, 0, sizeof(cache_faces[i].texture));
	}
	for (legacy_u32 i = 0; i < cache_page_count; i++) {
		free(cache_pages[i].texture.pixels);
	}
	free(cache_nodes);
	free(cache_indices);
	free(cache_pages);
	free(cache_page_hash);
	cache_nodes = NULL;
	cache_indices = NULL;
	cache_pages = NULL;
	cache_page_hash = NULL;
	cache_node_count = cache_page_count = cache_page_capacity = cache_hash_capacity = 0;
	cache_bytes = 0;
	cache_ready = 0;
}
void shape3d_shadows_invalidate(void)
{
	active = 0;
	cache_discard_lighting();
	free(cache_faces);
	cache_faces = NULL;
	cache_face_count = cache_face_capacity = 0;
	cache_collecting = 0;
	dynamic_polygons = 0;
}
legacy_s32 shape3d_shadows_bake_begin(void)
{
	shape3d_shadows_invalidate();
	cache_collecting = 1;
	camera = (struct VECTOR){0, 0, 0};
	active = 1;
	return 1;
}
legacy_s32 shape3d_shadows_baked(void)
{
	return cache_ready;
}
legacy_u32 shape3d_shadows_baked_bytes(void)
{
	return cache_bytes + cache_edge_count * sizeof(*cache_edges) +
		   cache_face_capacity * sizeof(*cache_faces) +
		   (cache_nodes != NULL ? cache_face_count * 2 * sizeof(*cache_nodes) : 0) +
		   (cache_indices != NULL ? cache_face_count * sizeof(*cache_indices) : 0) +
		   cache_page_capacity * sizeof(*cache_pages) +
		   cache_hash_capacity * sizeof(*cache_page_hash);
}
static void cache_add_polygon(const struct SHAPE3D_HIRES_VECTOR *vertices, legacy_u32 count,
							  legacy_s32 grille)
{
	if (cache_face_count == cache_face_capacity) {
		legacy_u32 capacity = cache_face_capacity != 0 ? cache_face_capacity * 2 : 256;
		if (capacity > 131072) {
			return;
		}
		struct CACHE_FACE *faces = realloc(cache_faces, (size_t)capacity * sizeof(*faces));
		if (faces == NULL) {
			return;
		}
		cache_faces = faces;
		cache_face_capacity = capacity;
	}
	struct CACHE_FACE *face = &cache_faces[cache_face_count];
	memset(face, 0, sizeof(*face));
	face->count = count;
	face->grille = grille;
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		face->low[axis] = FLT_MAX;
		face->high[axis] = -FLT_MAX;
	}
	for (legacy_s32 axis = 0; axis < 2; axis++) {
		face->light_low[axis] = FLT_MAX;
		face->light_high[axis] = -FLT_MAX;
	}
	for (legacy_u32 i = 0; i < count; i++) {
		struct SHADOW_VERTEX *vertex = &face->vertices[i];
		*vertex = (struct SHADOW_VERTEX){vertices[i].x, vertices[i].y, vertices[i].z};
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			legacy_f64 value = cache_component(vertex, axis);
			if (value < face->low[axis]) {
				face->low[axis] = value;
			}
			if (value > face->high[axis]) {
				face->high[axis] = value;
			}
		}
		legacy_f64 light[2] = {vertex->x + SHADOW_LIGHT_X * vertex->y,
							   vertex->z + SHADOW_LIGHT_Z * vertex->y};
		for (legacy_s32 axis = 0; axis < 2; axis++) {
			if (light[axis] < face->light_low[axis]) {
				face->light_low[axis] = light[axis];
			}
			if (light[axis] > face->light_high[axis]) {
				face->light_high[axis] = light[axis];
			}
		}
	}
	if (face->high[1] <= SHADOW_DEPTH_BIAS) {
		return;
	}
	/* Newell's normal also accepts convex faces with collinear first edges. */
	for (legacy_u32 i = 0; i < count; i++) {
		const struct SHADOW_VERTEX *a = &face->vertices[i];
		const struct SHADOW_VERTEX *b = &face->vertices[(i + 1) % count];
		face->normal[0] += (a->y - b->y) * (a->z + b->z);
		face->normal[1] += (a->z - b->z) * (a->x + b->x);
		face->normal[2] += (a->x - b->x) * (a->y + b->y);
	}
	legacy_f64 length = shadow_absolute(face->normal[0]) + shadow_absolute(face->normal[1]) +
						shadow_absolute(face->normal[2]);
	if (length < 0.00001) {
		return;
	}
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		face->normal[axis] /= length;
	}
	face->axis = 0;
	if (shadow_absolute(face->normal[1]) > shadow_absolute(face->normal[face->axis])) {
		face->axis = 1;
	}
	if (shadow_absolute(face->normal[2]) > shadow_absolute(face->normal[face->axis])) {
		face->axis = 2;
	}
	face->u_axis = (face->axis + 1) % 3;
	face->v_axis = (face->axis + 2) % 3;
	face->plane = face->normal[0] * vertices[0].x + face->normal[1] * vertices[0].y +
				  face->normal[2] * vertices[0].z;
	cache_face_count++;
}
static legacy_s32 cache_contains(const struct CACHE_FACE *face, const legacy_f64 *point,
								 legacy_f64 tolerance)
{
	legacy_f64 sign = 0;
	for (legacy_u32 i = 0; i < face->count; i++) {
		const struct SHADOW_VERTEX *a = &face->vertices[i];
		const struct SHADOW_VERTEX *b = &face->vertices[(i + 1) % face->count];
		legacy_f64 au = cache_component(a, face->u_axis);
		legacy_f64 av = cache_component(a, face->v_axis);
		legacy_f64 du = cache_component(b, face->u_axis) - au;
		legacy_f64 dv = cache_component(b, face->v_axis) - av;
		legacy_f64 cross = du * (point[face->v_axis] - av) - dv * (point[face->u_axis] - au);
		legacy_f64 margin = tolerance * (shadow_absolute(du) + shadow_absolute(dv));
		if (cross > margin) {
			if (sign < 0) {
				return 0;
			}
			sign = 1;
		} else if (cross < -margin) {
			if (sign > 0) {
				return 0;
			}
			sign = -1;
		}
	}
	return 1;
}
static legacy_u32 cache_build_node(legacy_u32 first, legacy_u32 count, legacy_u32 depth)
{
	legacy_u32 result = cache_node_count++;
	struct CACHE_NODE *node = &cache_nodes[result];
	node->first = first;
	node->count = count;
	node->left = node->right = CACHE_NONE;
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		node->low[axis] = FLT_MAX;
		node->high[axis] = -FLT_MAX;
	}
	for (legacy_u32 i = first; i < first + count; i++) {
		const struct CACHE_FACE *face = &cache_faces[cache_indices[i]];
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			if (face->low[axis] < node->low[axis]) {
				node->low[axis] = face->low[axis];
			}
			if (face->high[axis] > node->high[axis]) {
				node->high[axis] = face->high[axis];
			}
		}
	}
	if (count <= 8 || depth >= 24) {
		return result;
	}
	node->axis = 0;
	for (legacy_s32 axis = 1; axis < 3; axis++) {
		if (node->high[axis] - node->low[axis] > node->high[node->axis] - node->low[node->axis]) {
			node->axis = axis;
		}
	}
	node->split = (node->low[node->axis] + node->high[node->axis]) * 0.5;
	legacy_u32 left_end = first;
	for (legacy_u32 i = first; i < first + count; i++) {
		if (cache_faces[cache_indices[i]].high[node->axis] < node->split) {
			legacy_u32 temporary = cache_indices[left_end];
			cache_indices[left_end++] = cache_indices[i];
			cache_indices[i] = temporary;
		}
	}
	legacy_u32 right_end = left_end;
	for (legacy_u32 i = left_end; i < first + count; i++) {
		if (cache_faces[cache_indices[i]].low[node->axis] > node->split) {
			legacy_u32 temporary = cache_indices[right_end];
			cache_indices[right_end++] = cache_indices[i];
			cache_indices[i] = temporary;
		}
	}
	node->first = right_end;
	node->count = first + count - right_end;
	if (left_end > first) {
		node->left = cache_build_node(first, left_end - first, depth + 1);
	}
	if (right_end > left_end) {
		node->right = cache_build_node(left_end, right_end - left_end, depth + 1);
	}
	return result;
}
static legacy_s32 cache_ray_box(const struct CACHE_NODE *node, const legacy_f64 *point,
								const legacy_f64 *direction, const legacy_f64 *inverse,
								legacy_f64 maximum)
{
	legacy_f64 near = SHADOW_DEPTH_BIAS;
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		if (shadow_absolute(direction[axis]) < 0.00001) {
			if (point[axis] < node->low[axis] || point[axis] > node->high[axis]) {
				return 0;
			}
		} else {
			legacy_f64 a = (node->low[axis] - point[axis]) * inverse[axis];
			legacy_f64 b = (node->high[axis] - point[axis]) * inverse[axis];
			if (a > b) {
				legacy_f64 temporary = a;
				a = b;
				b = temporary;
			}
			if (a > near) {
				near = a;
			}
			if (b < maximum) {
				maximum = b;
			}
			if (near > maximum) {
				return 0;
			}
		}
	}
	return 1;
}
static legacy_s32 cache_ray_blocked(const legacy_f64 *point, const legacy_f64 *direction,
									legacy_f64 maximum, legacy_u32 receiver)
{
	if (cache_node_count == 0) {
		return 0;
	}
	legacy_f64 inverse[3];
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		inverse[axis] = shadow_absolute(direction[axis]) > 0.00001 ? 1.0 / direction[axis] : 0;
	}
	legacy_f64 minimum_y = point[1] + direction[1] * SHADOW_DEPTH_BIAS;
	legacy_f64 maximum_y = point[1] + direction[1] * maximum;
	if (minimum_y > maximum_y) {
		legacy_f64 temporary = minimum_y;
		minimum_y = maximum_y;
		maximum_y = temporary;
	}
	legacy_u32 stack[64], count = 1;
	stack[0] = 0;
	while (count != 0) {
		const struct CACHE_NODE *node = &cache_nodes[stack[--count]];
		if (!cache_ray_box(node, point, direction, inverse, maximum)) {
			continue;
		}
		for (legacy_u32 i = node->first; i < node->first + node->count; i++) {
			legacy_u32 index = cache_indices[i];
			if (index == receiver) {
				continue;
			}
			const struct CACHE_FACE *face = &cache_faces[index];
			/* Most candidates lie entirely below an upward light/contact ray.
			 * The interval also handles horizontal and downward test rays. */
			if (face->high[1] < minimum_y - 0.0001 || face->low[1] > maximum_y + 0.0001) {
				continue;
			}
			legacy_f64 denominator = face->normal[0] * direction[0] +
									 face->normal[1] * direction[1] +
									 face->normal[2] * direction[2];
			if (shadow_absolute(denominator) < 0.00001) {
				continue;
			}
			legacy_f64 distance = (face->plane - face->normal[0] * point[0] -
								   face->normal[1] * point[1] - face->normal[2] * point[2]) /
								  denominator;
			if (distance < SHADOW_DEPTH_BIAS || distance > maximum) {
				continue;
			}
			legacy_f64 hit[3] = {point[0] + direction[0] * distance,
								 point[1] + direction[1] * distance,
								 point[2] + direction[2] * distance};
			/* Retain a small edge tolerance, but reject hits outside the face
			 * bounds before its more expensive polygon edge tests. */
			legacy_s32 inside = 1;
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				if (hit[axis] < face->low[axis] - 0.0001 || hit[axis] > face->high[axis] + 0.0001) {
					inside = 0;
					break;
				}
			}
			if (!inside || !cache_contains(face, hit, 0.00001)) {
				continue;
			}
			if (face->grille && !cache_grille_covered(hit[face->u_axis], hit[face->v_axis])) {
				continue;
			}
			return 1;
		}
		if (node->left != CACHE_NONE) {
			stack[count++] = node->left;
		}
		if (node->right != CACHE_NONE) {
			stack[count++] = node->right;
		}
	}
	return 0;
}
static legacy_u8 cache_contact_lighting(const legacy_f64 *point, legacy_u32 receiver)
{
	legacy_f64 shade = 0;
	legacy_f64 support =
		receiver != CACHE_NONE ? shadow_absolute(cache_faces[receiver].normal[1]) : 1;
	if (support > 0.1) {
		static const legacy_f64 rays[4][3] = {{1, 0.5, 0}, {-1, 0.5, 0}, {0, 0.5, 1}, {0, 0.5, -1}};
		legacy_f64 contact = 0;
		for (legacy_s32 i = 0; i < 4; i++) {
			contact +=
				cache_ray_blocked(point, rays[i], 32, receiver) ? SHADOW_AMBIENT_OPACITY * 0.25 : 0;
		}
		shade += contact * support * (1 - shade / 255.0);
	}
	return (legacy_u8)(shade + 0.5);
}
static legacy_s32 cache_texture_allocate(struct CACHE_TEXTURE *texture, legacy_u32 width,
										 legacy_u32 height, legacy_f64 u, legacy_f64 v,
										 legacy_f64 texel)
{
	legacy_u32 bytes = 0, w = width, h = height;
	texture->levels = 0;
	if (width == 0 || height == 0 || width > 8192 || height > 8192 ||
		(legacy_u64)width * height > 4U * 1024U * 1024U) {
		return 0;
	}
	for (;;) {
		texture->offsets[texture->levels++] = bytes;
		bytes += w * h;
		if ((w == 1 && h == 1) || texture->levels == CACHE_MAX_MIPS) {
			break;
		}
		w = (w + 1) / 2;
		h = (h + 1) / 2;
	}
	if (bytes > CACHE_MAX_BYTES - cache_bytes) {
		return 0;
	}
	texture->pixels = calloc(bytes, 1);
	if (texture->pixels == NULL) {
		return 0;
	}
	texture->width = width;
	texture->height = height;
	texture->u = (legacy_f32)u;
	texture->v = (legacy_f32)v;
	texture->texel = (legacy_f32)texel;
	cache_bytes += bytes;
	return 1;
}
static void cache_texture_finish(struct CACHE_TEXTURE *texture, legacy_s32 occupied)
{
	if (!occupied) {
		legacy_u32 last = texture->levels - 1;
		legacy_u32 w = (texture->width + (1U << last) - 1) >> last;
		legacy_u32 h = (texture->height + (1U << last) - 1) >> last;
		cache_bytes -= texture->offsets[last] + w * h;
		free(texture->pixels);
		texture->pixels = NULL;
		return;
	}
	legacy_u32 width = texture->width, height = texture->height;
	for (legacy_u32 level = 1; level < texture->levels; level++) {
		const legacy_u8 *source = texture->pixels + texture->offsets[level - 1];
		legacy_u8 *target = texture->pixels + texture->offsets[level];
		legacy_u32 next_width = (width + 1) / 2, next_height = (height + 1) / 2;
		for (legacy_u32 y = 0; y < next_height; y++) {
			for (legacy_u32 x = 0; x < next_width; x++) {
				legacy_u32 total = 0, samples = 0;
				for (legacy_u32 dy = 0; dy < 2 && y * 2 + dy < height; dy++) {
					for (legacy_u32 dx = 0; dx < 2 && x * 2 + dx < width; dx++) {
						total += source[(y * 2 + dy) * width + x * 2 + dx];
						samples++;
					}
				}
				target[y * next_width + x] = (legacy_u8)((total + samples / 2) / samples);
			}
		}
		width = next_width;
		height = next_height;
	}
}
static legacy_s32 cache_rehash(legacy_u32 capacity)
{
	legacy_u32 *table = calloc(capacity, sizeof(*table));
	if (table == NULL) {
		return 0;
	}
	for (legacy_u32 i = 0; i < cache_page_count; i++) {
		legacy_u32 slot = cache_hash(cache_pages[i].x, cache_pages[i].z) & (capacity - 1);
		while (table[slot] != 0) {
			slot = (slot + 1) & (capacity - 1);
		}
		table[slot] = i + 1;
	}
	free(cache_page_hash);
	cache_page_hash = table;
	cache_hash_capacity = capacity;
	return 1;
}
static struct CACHE_PAGE *cache_page(legacy_s32 x, legacy_s32 z, legacy_s32 create)
{
	if (cache_hash_capacity == 0 || (create && cache_page_count * 2 >= cache_hash_capacity)) {
		if (!create || !cache_rehash(cache_hash_capacity != 0 ? cache_hash_capacity * 2 : 256)) {
			return NULL;
		}
	}
	legacy_u32 slot = cache_hash(x, z) & (cache_hash_capacity - 1);
	while (cache_page_hash[slot] != 0) {
		struct CACHE_PAGE *page = &cache_pages[cache_page_hash[slot] - 1];
		if (page->x == x && page->z == z) {
			return page;
		}
		slot = (slot + 1) & (cache_hash_capacity - 1);
	}
	if (!create || cache_page_count >= 16384) {
		return NULL;
	}
	if (cache_page_count == cache_page_capacity) {
		legacy_u32 capacity = cache_page_capacity != 0 ? cache_page_capacity * 2 : 128;
		struct CACHE_PAGE *pages = realloc(cache_pages, (size_t)capacity * sizeof(*pages));
		if (pages == NULL) {
			return NULL;
		}
		cache_pages = pages;
		cache_page_capacity = capacity;
	}
	struct CACHE_PAGE *page = &cache_pages[cache_page_count];
	memset(page, 0, sizeof(*page));
	page->x = x;
	page->z = z;
	cache_page_hash[slot] = ++cache_page_count;
	return page;
}
static void cache_mark_ground(legacy_f64 left, legacy_f64 top, legacy_f64 right, legacy_f64 bottom,
							  legacy_s32 dense)
{
	legacy_s32 x0 = shadow_floor((left - 2) / CACHE_PAGE_SIZE);
	legacy_s32 z0 = shadow_floor((top - 2) / CACHE_PAGE_SIZE);
	legacy_s32 x1 = shadow_floor((right + 2) / CACHE_PAGE_SIZE);
	legacy_s32 z1 = shadow_floor((bottom + 2) / CACHE_PAGE_SIZE);
	for (legacy_s32 z = z0; z <= z1; z++) {
		for (legacy_s32 x = x0; x <= x1; x++) {
			struct CACHE_PAGE *page = cache_page(x, z, 1);
			if (page != NULL) {
				page->dense |= dense;
			}
		}
	}
}
static void cache_ground_triangle(const struct SHADOW_VERTEX *first,
								  const struct SHADOW_VERTEX *second,
								  const struct SHADOW_VERTEX *third, legacy_s32 grille,
								  legacy_s32 grille_u_axis, legacy_s32 grille_v_axis)
{
	struct SHADOW_VERTEX a = {first->x + SHADOW_LIGHT_X * first->y, first->y,
							  first->z + SHADOW_LIGHT_Z * first->y};
	struct SHADOW_VERTEX b = {second->x + SHADOW_LIGHT_X * second->y, second->y,
							  second->z + SHADOW_LIGHT_Z * second->y};
	struct SHADOW_VERTEX c = {third->x + SHADOW_LIGHT_X * third->y, third->y,
							  third->z + SHADOW_LIGHT_Z * third->y};
	legacy_f64 determinant = (b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x);
	if (shadow_absolute(determinant) < 0.00001) {
		return;
	}
	legacy_f64 inverse = 1 / determinant;
	legacy_f64 bx = (c.z - a.z) * inverse, bz = -(c.x - a.x) * inverse;
	legacy_f64 cx = -(b.z - a.z) * inverse, cz = (b.x - a.x) * inverse;
	legacy_f64 left = a.x < b.x ? a.x : b.x, right = a.x > b.x ? a.x : b.x;
	legacy_f64 top = a.z < b.z ? a.z : b.z, bottom = a.z > b.z ? a.z : b.z;
	if (c.x < left) {
		left = c.x;
	}
	if (c.x > right) {
		right = c.x;
	}
	if (c.z < top) {
		top = c.z;
	}
	if (c.z > bottom) {
		bottom = c.z;
	}
	legacy_s32 page_left = shadow_floor((left - 0.00001) / CACHE_PAGE_SIZE);
	legacy_s32 page_right = shadow_floor(right / CACHE_PAGE_SIZE);
	legacy_s32 page_top = shadow_floor((top - 0.00001) / CACHE_PAGE_SIZE);
	legacy_s32 page_bottom = shadow_floor(bottom / CACHE_PAGE_SIZE);
	for (legacy_s32 pz = page_top; pz <= page_bottom; pz++) {
		for (legacy_s32 px = page_left; px <= page_right; px++) {
			struct CACHE_PAGE *page = cache_page(px, pz, 0);
			if (page == NULL || page->texture.pixels == NULL) {
				continue;
			}
			struct CACHE_TEXTURE *texture = &page->texture;
			legacy_s32 x0 = shadow_floor((left - texture->u) / texture->texel);
			legacy_s32 x1 = shadow_floor((right - texture->u) / texture->texel);
			legacy_s32 z0 = shadow_floor((top - texture->v) / texture->texel);
			legacy_s32 z1 = shadow_floor((bottom - texture->v) / texture->texel);
			if (x0 < 0) {
				x0 = 0;
			}
			if (z0 < 0) {
				z0 = 0;
			}
			if (x1 >= (legacy_s32)texture->width) {
				x1 = texture->width - 1;
			}
			if (z1 >= (legacy_s32)texture->height) {
				z1 = texture->height - 1;
			}
			for (legacy_s32 z = z0; z <= z1; z++) {
				legacy_f64 world_z = texture->v + z * texture->texel;
				legacy_f64 dz = world_z - a.z;
				legacy_f64 dx = texture->u + x0 * texture->texel - a.x;
				legacy_f64 wb = dx * bx + dz * bz, wc = dx * cx + dz * cz;
				for (legacy_s32 x = x0; x <= x1;
					 x++, wb += texture->texel * bx, wc += texture->texel * cx) {
					if (wb < -0.00001 || wc < -0.00001 || wb + wc > 1.00001) {
						continue;
					}
					legacy_f64 height = a.y + wb * (b.y - a.y) + wc * (c.y - a.y);
					if (height <= SHADOW_DEPTH_BIAS) {
						continue;
					}
					if (grille) {
						legacy_f64 gx = texture->u + x * texture->texel - SHADOW_LIGHT_X * height;
						legacy_f64 gz = world_z - SHADOW_LIGHT_Z * height;
						legacy_f64 source[3] = {gx, height, gz};
						if (!cache_grille_covered(source[grille_u_axis], source[grille_v_axis])) {
							continue;
						}
					}
					texture->pixels[(size_t)z * texture->width + x] = (legacy_u8)SHADOW_MAX_OPACITY;
				}
			}
		}
	}
}
static legacy_f64 cache_ground_distance_squared(const struct CACHE_FACE *face, legacy_f64 x,
												legacy_f64 z)
{
	legacy_f64 minimum = 32 * 32;
	legacy_s32 positive = 0, negative = 0;
	legacy_f64 area = 0;
	for (legacy_u32 i = 0; i < face->count; i++) {
		const struct SHADOW_VERTEX *a = &face->vertices[i];
		const struct SHADOW_VERTEX *b = &face->vertices[(i + 1) % face->count];
		legacy_f64 dx = b->x - a->x, dz = b->z - a->z;
		legacy_f64 cross = dx * (z - a->z) - dz * (x - a->x);
		positive |= cross > 0.00001;
		negative |= cross < -0.00001;
		area += a->x * b->z - a->z * b->x;
		legacy_f64 length = dx * dx + dz * dz;
		legacy_f64 t = length > 0 ? ((x - a->x) * dx + (z - a->z) * dz) / length : 0;
		if (t < 0) {
			t = 0;
		}
		if (t > 1) {
			t = 1;
		}
		legacy_f64 sx = a->x + t * dx - x, sz = a->z + t * dz - z;
		legacy_f64 distance = sx * sx + sz * sz;
		if (distance < minimum) {
			minimum = distance;
		}
	}
	return shadow_absolute(area) > 0.00001 && !(positive && negative) ? 0 : minimum;
}
static void cache_ground_contact(const struct CACHE_FACE *face)
{
	if (face->low[1] >= 32) {
		return;
	}
	legacy_f64 strength = SHADOW_AMBIENT_OPACITY * (1 - (face->low[1] > 0 ? face->low[1] : 0) / 32);
	legacy_f64 left = face->low[0] - 32, right = face->high[0] + 32;
	legacy_f64 top = face->low[2] - 32, bottom = face->high[2] + 32;
	legacy_s32 page_left = shadow_floor((left - 0.00001) / CACHE_PAGE_SIZE);
	legacy_s32 page_right = shadow_floor(right / CACHE_PAGE_SIZE);
	legacy_s32 page_top = shadow_floor((top - 0.00001) / CACHE_PAGE_SIZE);
	legacy_s32 page_bottom = shadow_floor(bottom / CACHE_PAGE_SIZE);
	for (legacy_s32 pz = page_top; pz <= page_bottom; pz++) {
		for (legacy_s32 px = page_left; px <= page_right; px++) {
			struct CACHE_PAGE *page = cache_page(px, pz, 0);
			if (page == NULL || page->texture.pixels == NULL) {
				continue;
			}
			struct CACHE_TEXTURE *texture = &page->texture;
			legacy_s32 x0 = shadow_floor((left - texture->u) / texture->texel);
			legacy_s32 x1 = shadow_floor((right - texture->u) / texture->texel);
			legacy_s32 z0 = shadow_floor((top - texture->v) / texture->texel);
			legacy_s32 z1 = shadow_floor((bottom - texture->v) / texture->texel);
			if (x0 < 0) {
				x0 = 0;
			}
			if (z0 < 0) {
				z0 = 0;
			}
			if (x1 >= (legacy_s32)texture->width) {
				x1 = texture->width - 1;
			}
			if (z1 >= (legacy_s32)texture->height) {
				z1 = texture->height - 1;
			}
			for (legacy_s32 z = z0; z <= z1; z++) {
				for (legacy_s32 x = x0; x <= x1; x++) {
					legacy_f64 distance = cache_ground_distance_squared(
						face, texture->u + x * texture->texel, texture->v + z * texture->texel);
					legacy_f64 contact = strength * (1 - distance / (32 * 32));
					legacy_u8 *pixel = &texture->pixels[(size_t)z * texture->width + x];
					legacy_u8 shade =
						(legacy_u8)(*pixel >= SHADOW_MAX_OPACITY
										? SHADOW_MAX_OPACITY +
											  contact * (1 - SHADOW_MAX_OPACITY / 255.0F) + 0.5
										: contact + 0.5);
					if (shade > *pixel) {
						*pixel = shade;
					}
				}
			}
		}
	}
}
static void cache_bake_ground(void)
{
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		const struct CACHE_FACE *face = &cache_faces[i];
		cache_mark_ground(face->light_low[0], face->light_low[1], face->light_high[0],
						  face->light_high[1], face->grille);
		if (face->low[1] < 32) {
			cache_mark_ground(face->low[0] - 32, face->low[2] - 32, face->high[0] + 32,
							  face->high[2] + 32, 0);
		}
	}
	for (legacy_u32 i = 0; i < cache_page_count; i++) {
		struct CACHE_PAGE *page = &cache_pages[i];
		legacy_f64 texel = page->dense ? CACHE_TEXEL : CACHE_COARSE_TEXEL;
		legacy_u32 samples = (legacy_u32)(CACHE_PAGE_SIZE / texel) + 1;
		cache_texture_allocate(&page->texture, samples, samples, page->x * CACHE_PAGE_SIZE,
							   page->z * CACHE_PAGE_SIZE, texel);
	}
	/* Ground receives the union of projected faces. Rasterizing this once is
	 * much cheaper than tracing every ground texel through the entire scene. */
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		const struct CACHE_FACE *face = &cache_faces[i];
		for (legacy_u32 vertex = 1; vertex + 1 < face->count; vertex++) {
			cache_ground_triangle(&face->vertices[0], &face->vertices[vertex],
								  &face->vertices[vertex + 1], face->grille, face->u_axis,
								  face->v_axis);
		}
	}
	/* A local footprint halo approximates ground contact occlusion. Its
	 * maximum operator keeps adjoining faces from accumulating darkness. */
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		cache_ground_contact(&cache_faces[i]);
	}
	for (legacy_u32 i = 0; i < cache_page_count; i++) {
		struct CACHE_TEXTURE *texture = &cache_pages[i].texture;
		if (texture->pixels == NULL) {
			continue;
		}
		legacy_s32 occupied = 0;
		for (legacy_u32 pixel = 0; pixel < texture->width * texture->height; pixel++) {
			occupied |= texture->pixels[pixel];
		}
		cache_texture_finish(texture, occupied);
	}
}

static legacy_s32 cache_receiver_region(legacy_u32 index, legacy_f64 *low, legacy_f64 *high,
										legacy_s32 *dense)
{
	*dense = 0;
	const struct CACHE_FACE *receiver = &cache_faces[index];
	low[0] = low[1] = FLT_MAX;
	high[0] = high[1] = -FLT_MAX;
	legacy_f64 denominator = receiver->normal[0] * SHADOW_LIGHT_X - receiver->normal[1] +
							 receiver->normal[2] * SHADOW_LIGHT_Z;
	legacy_u32 stack[64], pending = 0;
	if (cache_node_count != 0) {
		stack[pending++] = 0;
	}
	while (pending != 0) {
		const struct CACHE_NODE *node = &cache_nodes[stack[--pending]];
		if (node->high[1] <= receiver->low[1] + SHADOW_DEPTH_BIAS) {
			continue;
		}
		/* A node can affect this receiver through either sunlight or the
		 * short contact rays. Projecting its world AABB is conservative even
		 * when its faces straddle a BSP split or have sloping silhouettes. */
		legacy_s32 sunlight = shadow_absolute(denominator) > 0.00001;
		const legacy_f64 light[2] = {SHADOW_LIGHT_X, SHADOW_LIGHT_Z};
		const legacy_s32 world_axes[2] = {0, 2};
		for (legacy_s32 axis = 0; axis < 2; axis++) {
			legacy_f64 light_low = node->low[world_axes[axis]] + light[axis] * node->low[1];
			legacy_f64 light_high = node->high[world_axes[axis]] + light[axis] * node->high[1];
			sunlight &=
				light_high >= receiver->light_low[axis] && light_low <= receiver->light_high[axis];
		}
		legacy_s32 contact = shadow_absolute(receiver->normal[1]) > 0.1;
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			contact &= node->high[axis] + 32 >= receiver->low[axis] &&
					   node->low[axis] - 32 <= receiver->high[axis];
		}
		if (!sunlight && !contact) {
			continue;
		}
		if (node->left != CACHE_NONE) {
			stack[pending++] = node->left;
		}
		if (node->right != CACHE_NONE) {
			stack[pending++] = node->right;
		}
		for (legacy_u32 candidate = node->first; candidate < node->first + node->count;
			 candidate++) {
			legacy_u32 i = cache_indices[candidate];
			if (i == index) {
				continue;
			}
			const struct CACHE_FACE *caster = &cache_faces[i];
			if (caster->high[1] <= receiver->low[1] + SHADOW_DEPTH_BIAS) {
				continue;
			}
			legacy_s32 intersects = 1;
			for (legacy_s32 axis = 0; axis < 2; axis++) {
				intersects &= caster->light_high[axis] >= receiver->light_low[axis] &&
							  caster->light_low[axis] <= receiver->light_high[axis];
			}
			if (intersects && shadow_absolute(denominator) > 0.00001) {
				*dense |= caster->grille;
				for (legacy_u32 v = 0; v < caster->count; v++) {
					const struct SHADOW_VERTEX *vertex = &caster->vertices[v];
					legacy_f64 t =
						(receiver->plane - receiver->normal[0] * vertex->x -
						 receiver->normal[1] * vertex->y - receiver->normal[2] * vertex->z) /
						denominator;
					legacy_f64 point[3] = {vertex->x + SHADOW_LIGHT_X * t, vertex->y - t,
										   vertex->z + SHADOW_LIGHT_Z * t};
					legacy_f64 uv[2] = {point[receiver->u_axis], point[receiver->v_axis]};
					for (legacy_s32 axis = 0; axis < 2; axis++) {
						if (uv[axis] < low[axis]) {
							low[axis] = uv[axis];
						}
						if (uv[axis] > high[axis]) {
							high[axis] = uv[axis];
						}
					}
				}
			}
			intersects = shadow_absolute(receiver->normal[1]) > 0.1;
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				intersects &= caster->high[axis] + 32 >= receiver->low[axis] &&
							  caster->low[axis] - 32 <= receiver->high[axis];
			}
			if (intersects) {
				legacy_s32 axes[2] = {receiver->u_axis, receiver->v_axis};
				for (legacy_s32 a = 0; a < 2; a++) {
					if (caster->low[axes[a]] - 32 < low[a]) {
						low[a] = caster->low[axes[a]] - 32;
					}
					if (caster->high[axes[a]] + 32 > high[a]) {
						high[a] = caster->high[axes[a]] + 32;
					}
				}
			}
		}
	}
	legacy_s32 axes[2] = {receiver->u_axis, receiver->v_axis};
	for (legacy_s32 a = 0; a < 2; a++) {
		if (low[a] > high[a]) {
			return 0;
		}
		if (low[a] < receiver->low[axes[a]]) {
			low[a] = receiver->low[axes[a]];
		}
		if (high[a] > receiver->high[axes[a]]) {
			high[a] = receiver->high[axes[a]];
		}
		if (low[a] > high[a]) {
			return 0;
		}
	}
	return 1;
}
struct CACHE_PROJECTED_VERTEX {
	legacy_f64 u, v, grille_u, grille_v, distance;
};
static void cache_receiver_triangle(struct CACHE_TEXTURE *texture,
									const struct CACHE_PROJECTED_VERTEX *a,
									const struct CACHE_PROJECTED_VERTEX *b,
									const struct CACHE_PROJECTED_VERTEX *c, legacy_s32 grille)
{
	legacy_f64 determinant = (b->u - a->u) * (c->v - a->v) - (b->v - a->v) * (c->u - a->u);
	if (shadow_absolute(determinant) < 0.00001) {
		return;
	}
	legacy_f64 inverse = 1 / determinant;
	legacy_f64 bx = (c->v - a->v) * inverse, by = -(c->u - a->u) * inverse;
	legacy_f64 cx = -(b->v - a->v) * inverse, cy = (b->u - a->u) * inverse;
	legacy_f64 left = a->u < b->u ? a->u : b->u, right = a->u > b->u ? a->u : b->u;
	legacy_f64 top = a->v < b->v ? a->v : b->v, bottom = a->v > b->v ? a->v : b->v;
	if (c->u < left) {
		left = c->u;
	}
	if (c->u > right) {
		right = c->u;
	}
	if (c->v < top) {
		top = c->v;
	}
	if (c->v > bottom) {
		bottom = c->v;
	}
	legacy_s32 x0 = shadow_floor((left - texture->u) / texture->texel);
	legacy_s32 x1 = shadow_floor((right - texture->u) / texture->texel);
	legacy_s32 y0 = shadow_floor((top - texture->v) / texture->texel);
	legacy_s32 y1 = shadow_floor((bottom - texture->v) / texture->texel);
	if (x0 < 0) {
		x0 = 0;
	}
	if (y0 < 0) {
		y0 = 0;
	}
	if (x1 >= (legacy_s32)texture->width) {
		x1 = texture->width - 1;
	}
	if (y1 >= (legacy_s32)texture->height) {
		y1 = texture->height - 1;
	}
	for (legacy_s32 y = y0; y <= y1; y++) {
		legacy_f64 dy = texture->v + y * texture->texel - a->v;
		legacy_f64 dx = texture->u + x0 * texture->texel - a->u;
		legacy_f64 wb = dx * bx + dy * by, wc = dx * cx + dy * cy;
		for (legacy_s32 x = x0; x <= x1;
			 x++, wb += texture->texel * bx, wc += texture->texel * cx) {
			if (wb < -0.00001 || wc < -0.00001 || wb + wc > 1.00001) {
				continue;
			}
			if (grille) {
				legacy_f64 gu = a->grille_u + wb * (b->grille_u - a->grille_u) +
								wc * (c->grille_u - a->grille_u);
				legacy_f64 gv = a->grille_v + wb * (b->grille_v - a->grille_v) +
								wc * (c->grille_v - a->grille_v);
				if (!cache_grille_covered(gu, gv)) {
					continue;
				}
			}
			texture->pixels[(size_t)y * texture->width + x] = (legacy_u8)SHADOW_MAX_OPACITY;
		}
	}
}
static void cache_project_caster(struct CACHE_FACE *receiver, const struct CACHE_FACE *caster)
{
	legacy_f64 denominator = receiver->normal[0] * SHADOW_LIGHT_X - receiver->normal[1] +
							 receiver->normal[2] * SHADOW_LIGHT_Z;
	if (shadow_absolute(denominator) < 0.00001) {
		return;
	}
	struct CACHE_PROJECTED_VERTEX original[20], clipped[24];
	for (legacy_u32 i = 0; i < caster->count; i++) {
		const struct SHADOW_VERTEX *vertex = &caster->vertices[i];
		legacy_f64 distance = (receiver->plane - receiver->normal[0] * vertex->x -
							   receiver->normal[1] * vertex->y - receiver->normal[2] * vertex->z) /
							  denominator;
		legacy_f64 projected[3] = {vertex->x + SHADOW_LIGHT_X * distance, vertex->y - distance,
								   vertex->z + SHADOW_LIGHT_Z * distance};
		original[i] = (struct CACHE_PROJECTED_VERTEX){
			projected[receiver->u_axis], projected[receiver->v_axis],
			cache_component(vertex, caster->u_axis), cache_component(vertex, caster->v_axis),
			distance};
	}
	legacy_u32 count = 0;
	for (legacy_u32 i = 0; i < caster->count; i++) {
		const struct CACHE_PROJECTED_VERTEX *a = &original[i];
		const struct CACHE_PROJECTED_VERTEX *b = &original[(i + 1) % caster->count];
		legacy_s32 a_inside = a->distance >= SHADOW_DEPTH_BIAS;
		legacy_s32 b_inside = b->distance >= SHADOW_DEPTH_BIAS;
		if (a_inside) {
			clipped[count++] = *a;
		}
		if (a_inside != b_inside) {
			legacy_f64 t = (SHADOW_DEPTH_BIAS - a->distance) / (b->distance - a->distance);
			clipped[count++] = (struct CACHE_PROJECTED_VERTEX){
				a->u + t * (b->u - a->u), a->v + t * (b->v - a->v),
				a->grille_u + t * (b->grille_u - a->grille_u),
				a->grille_v + t * (b->grille_v - a->grille_v), SHADOW_DEPTH_BIAS};
		}
	}
	for (legacy_u32 i = 1; i + 1 < count; i++) {
		cache_receiver_triangle(&receiver->texture, &clipped[0], &clipped[i], &clipped[i + 1],
								caster->grille);
	}
}
static void cache_receiver_shadows(legacy_u32 index)
{
	struct CACHE_FACE *receiver = &cache_faces[index];
	legacy_u32 stack[64], pending = 1;
	stack[0] = 0;
	while (pending != 0) {
		const struct CACHE_NODE *node = &cache_nodes[stack[--pending]];
		if (node->high[1] <= receiver->low[1] + SHADOW_DEPTH_BIAS) {
			continue;
		}
		legacy_s32 overlaps = 1;
		for (legacy_s32 axis = 0; axis < 2; axis++) {
			legacy_s32 world_axis = axis == 0 ? 0 : 2;
			legacy_f64 light = axis == 0 ? SHADOW_LIGHT_X : SHADOW_LIGHT_Z;
			overlaps &=
				node->high[world_axis] + light * node->high[1] >= receiver->light_low[axis] &&
				node->low[world_axis] + light * node->low[1] <= receiver->light_high[axis];
		}
		if (!overlaps) {
			continue;
		}
		if (node->left != CACHE_NONE) {
			stack[pending++] = node->left;
		}
		if (node->right != CACHE_NONE) {
			stack[pending++] = node->right;
		}
		for (legacy_u32 i = node->first; i < node->first + node->count; i++) {
			legacy_u32 caster_index = cache_indices[i];
			if (caster_index == index) {
				continue;
			}
			const struct CACHE_FACE *caster = &cache_faces[caster_index];
			if (caster->high[1] <= receiver->low[1] + SHADOW_DEPTH_BIAS) {
				continue;
			}
			overlaps = 1;
			for (legacy_s32 axis = 0; axis < 2; axis++) {
				overlaps &= caster->light_high[axis] >= receiver->light_low[axis] &&
							caster->light_low[axis] <= receiver->light_high[axis];
			}
			if (overlaps) {
				cache_project_caster(receiver, caster);
			}
		}
	}
}
static void cache_receiver_contact(legacy_u32 index)
{
	struct CACHE_FACE *face = &cache_faces[index];
	struct CACHE_TEXTURE *texture = &face->texture;
	if (shadow_absolute(face->normal[1]) <= 0.1) {
		return;
	}
	legacy_u32 stride = (legacy_u32)(CACHE_COARSE_TEXEL / texture->texel);
	legacy_u32 width = (texture->width + stride - 1) / stride + 1;
	legacy_u32 height = (texture->height + stride - 1) / stride + 1;
	legacy_u8 *contact = calloc((size_t)width * height, 1);
	if (contact == NULL) {
		return;
	}
	for (legacy_u32 y = 0; y < height; y++) {
		for (legacy_u32 x = 0; x < width; x++) {
			legacy_f64 point[3];
			point[face->u_axis] = texture->u + x * CACHE_COARSE_TEXEL;
			point[face->v_axis] = texture->v + y * CACHE_COARSE_TEXEL;
			point[face->axis] = (face->plane - face->normal[face->u_axis] * point[face->u_axis] -
								 face->normal[face->v_axis] * point[face->v_axis]) /
								face->normal[face->axis];
			if (cache_contains(face, point, CACHE_COARSE_TEXEL)) {
				contact[y * width + x] = cache_contact_lighting(point, index);
			}
		}
	}
	for (legacy_u32 y = 0; y < texture->height; y++) {
		legacy_u32 iy = y / stride;
		legacy_f64 fy = (legacy_f64)(y % stride) / stride;
		for (legacy_u32 x = 0; x < texture->width; x++) {
			legacy_u32 ix = x / stride;
			legacy_f64 fx = (legacy_f64)(x % stride) / stride;
			legacy_f64 top =
				contact[iy * width + ix] * (1 - fx) + contact[iy * width + ix + 1] * fx;
			legacy_f64 bottom =
				contact[(iy + 1) * width + ix] * (1 - fx) + contact[(iy + 1) * width + ix + 1] * fx;
			legacy_f64 ambient = top * (1 - fy) + bottom * fy;
			legacy_u8 *pixel = &texture->pixels[(size_t)y * texture->width + x];
			*pixel = (legacy_u8)(*pixel + ambient * (1 - *pixel / 255.0) + 0.5);
		}
	}
	free(contact);
}
static void cache_bake_receivers(void)
{
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		struct CACHE_FACE *face = &cache_faces[i];
		legacy_f64 low[2], high[2];
		legacy_s32 dense;
		if (!cache_receiver_region(i, low, high, &dense)) {
			continue;
		}
		legacy_f64 texel = dense ? CACHE_TEXEL : CACHE_COARSE_TEXEL;
		for (legacy_s32 axis = 0; axis < 2; axis++) {
			low[axis] = shadow_floor(low[axis] / texel) * texel - texel;
			high[axis] += texel;
		}
		legacy_u32 width = (legacy_u32)((high[0] - low[0]) / texel) + 2;
		legacy_u32 height = (legacy_u32)((high[1] - low[1]) / texel) + 2;
		if (!cache_texture_allocate(&face->texture, width, height, low[0], low[1], texel)) {
			continue;
		}
		cache_receiver_shadows(i);
		cache_receiver_contact(i);
		legacy_s32 occupied = 0;
		for (legacy_u32 pixel = 0; pixel < width * height; pixel++) {
			occupied |= face->texture.pixels[pixel];
		}
		cache_texture_finish(&face->texture, occupied);
	}
}

static legacy_s32 cache_build_tree(void)
{
	if (cache_face_count == 0) {
		return 1;
	}
	cache_nodes = calloc((size_t)cache_face_count * 2, sizeof(*cache_nodes));
	cache_indices = malloc((size_t)cache_face_count * sizeof(*cache_indices));
	if (cache_nodes == NULL || cache_indices == NULL) {
		return 0;
	}
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		cache_indices[i] = i;
	}
	cache_build_node(0, cache_face_count, 0);
	return 1;
}
/* Derived runtime data is deliberately absent from the persistent format.
 * Rectangular faces need only the bounds/plane checks. Other faces retain the
 * original edge-test arithmetic with their invariant values computed once. */
static void cache_prepare_texture(struct CACHE_TEXTURE *texture)
{
	texture->uniform = -1;
	texture->inverse_texel = texture->texel > 0 ? 1.0F / texture->texel : 0;
	if (texture->pixels == NULL) {
		return;
	}
	legacy_u8 value = texture->pixels[0];
	legacy_u32 last = texture->levels - 1;
	legacy_u32 width = (texture->width + (1U << last) - 1) >> last;
	legacy_u32 height = (texture->height + (1U << last) - 1) >> last;
	legacy_u32 bytes = texture->offsets[last] + width * height;
	for (legacy_u32 i = 1; i < bytes; i++) {
		if (texture->pixels[i] != value) {
			return;
		}
	}
	texture->uniform = value;
}
static void cache_prepare_runtime(void)
{
	legacy_u32 maximum_edges = CACHE_EDGE_BYTES / sizeof(*cache_edges);
	for (legacy_u32 index = 0; index < cache_face_count; index++) {
		struct CACHE_FACE *face = &cache_faces[index];
		face->axial = face->low[face->axis] == face->high[face->axis] &&
					  shadow_absolute(face->normal[face->axis]) == 1 &&
					  face->normal[face->u_axis] == 0 && face->normal[face->v_axis] == 0 &&
					  face->plane == face->normal[face->axis] * face->low[face->axis];
		legacy_u32 corners = 0;
		face->rectangular = face->count == 4;
		for (legacy_u32 i = 0; i < face->count && face->rectangular; i++) {
			const struct SHADOW_VERTEX *a = &face->vertices[i];
			const struct SHADOW_VERTEX *b = &face->vertices[(i + 1) % face->count];
			legacy_f64 u = cache_component(a, face->u_axis);
			legacy_f64 v = cache_component(a, face->v_axis);
			if ((u != face->low[face->u_axis] && u != face->high[face->u_axis]) ||
				(v != face->low[face->v_axis] && v != face->high[face->v_axis]) ||
				(u != cache_component(b, face->u_axis) && v != cache_component(b, face->v_axis))) {
				face->rectangular = 0;
			} else {
				corners |= 1U << ((u == face->high[face->u_axis]) |
								  ((v == face->high[face->v_axis]) << 1));
			}
		}
		face->rectangular &= corners == 15;
		face->first_edge = CACHE_NONE;
		if (!face->rectangular && face->count <= maximum_edges - cache_edge_count) {
			face->first_edge = cache_edge_count;
			cache_edge_count += face->count;
		}
		cache_prepare_texture(&face->texture);
	}
	if (cache_edge_count != 0) {
		cache_edges = malloc((size_t)cache_edge_count * sizeof(*cache_edges));
	}
	if (cache_edges == NULL) {
		cache_edge_count = 0;
	}
	for (legacy_u32 index = 0; index < cache_face_count; index++) {
		struct CACHE_FACE *face = &cache_faces[index];
		if (cache_edges == NULL || face->first_edge == CACHE_NONE) {
			continue;
		}
		for (legacy_u32 i = 0; i < face->count; i++) {
			const struct SHADOW_VERTEX *a = &face->vertices[i];
			const struct SHADOW_VERTEX *b = &face->vertices[(i + 1) % face->count];
			struct CACHE_EDGE *edge = &cache_edges[face->first_edge + i];
			edge->u = cache_component(a, face->u_axis);
			edge->v = cache_component(a, face->v_axis);
			edge->du = cache_component(b, face->u_axis) - edge->u;
			edge->dv = cache_component(b, face->v_axis) - edge->v;
			edge->margin =
				CACHE_RECEIVER_TOLERANCE * (shadow_absolute(edge->du) + shadow_absolute(edge->dv));
		}
	}
	for (legacy_u32 index = 0; index < cache_page_count; index++) {
		cache_prepare_texture(&cache_pages[index].texture);
	}
}

void shape3d_shadows_bake_end(void)
{
	if (!cache_collecting) {
		return;
	}
	cache_collecting = 0;
	active = 0;
	if (cache_face_count != 0) {
		if (!cache_build_tree()) {
			shape3d_shadows_invalidate();
			return;
		}
		cache_bake_ground();
		cache_bake_receivers();
	}
	cache_prepare_runtime();
	cache_ready = 1;
}
/* The disk cache is a versioned byte format, never a dump of native structs.
 * Bump the version when lighting or filtering algorithms change. Geometry is
 * rebuilt from trusted track resources; only its matching textures are read. */
#define CACHE_DISK_VERSION 1U
#define CACHE_HASH_OFFSET 14695981039346656037ULL
#define CACHE_HASH_PRIME 1099511628211ULL
#define CACHE_MAX_DISK_BYTES (CACHE_MAX_BYTES + 8U * 1024U * 1024U)

struct CACHE_STREAM {
	FILE *file;
	legacy_u64 checksum;
	size_t remaining;
	legacy_s32 failed;
};
static legacy_u64 cache_hash_bytes(legacy_u64 hash, const void *buffer, size_t count)
{
	const legacy_u8 *bytes = buffer;
	for (size_t i = 0; i < count; i++) {
		hash = (hash ^ bytes[i]) * CACHE_HASH_PRIME;
	}
	return hash;
}
static legacy_u64 cache_hash_integer(legacy_u64 hash, legacy_u64 value, legacy_u32 count)
{
	legacy_u8 bytes[8];
	for (legacy_u32 i = 0; i < count; i++) {
		bytes[i] = (legacy_u8)(value >> (i * 8));
	}
	return cache_hash_bytes(hash, bytes, count);
}
static legacy_u64 cache_geometry_fingerprint(void)
{
	legacy_u64 hash = cache_hash_integer(CACHE_HASH_OFFSET, CACHE_DISK_VERSION, 4);
	const legacy_f64 settings[] = {SHADOW_LIGHT_X,	   SHADOW_LIGHT_Z,			SHADOW_DEPTH_BIAS,
								   SHADOW_MAX_OPACITY, SHADOW_AMBIENT_OPACITY,	CACHE_TEXEL,
								   CACHE_COARSE_TEXEL, CACHE_GRILLE_PERIOD,		CACHE_GRILLE_BAR,
								   CACHE_PAGE_SIZE,	   CACHE_RECEIVER_TOLERANCE};
	for (size_t i = 0; i < sizeof(settings) / sizeof(settings[0]); i++) {
		legacy_u64 bits;
		memcpy(&bits, &settings[i], sizeof(bits));
		hash = cache_hash_integer(hash, bits, 8);
	}
	hash = cache_hash_integer(hash, cache_face_count, 4);
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		const struct CACHE_FACE *face = &cache_faces[i];
		hash = cache_hash_integer(hash, face->count, 4);
		hash = cache_hash_integer(hash, face->grille != 0, 4);
		for (legacy_u32 vertex = 0; vertex < face->count; vertex++) {
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				legacy_f64 value = cache_component(&face->vertices[vertex], axis);
				legacy_u64 bits;
				memcpy(&bits, &value, sizeof(bits));
				hash = cache_hash_integer(hash, bits, 8);
			}
		}
	}
	return hash;
}
static void cache_read_bytes(struct CACHE_STREAM *stream, void *buffer, size_t count)
{
	if (stream->failed) {
		return;
	}
	if (count > stream->remaining || fread(buffer, 1, count, stream->file) != count) {
		stream->failed = 1;
		return;
	}
	stream->remaining -= count;
	stream->checksum = cache_hash_bytes(stream->checksum, buffer, count);
}
static legacy_u64 cache_read_integer(struct CACHE_STREAM *stream, legacy_u32 count)
{
	legacy_u8 bytes[8] = {0};
	cache_read_bytes(stream, bytes, count);
	legacy_u64 value = 0;
	for (legacy_u32 i = 0; i < count; i++) {
		value |= (legacy_u64)bytes[i] << (i * 8);
	}
	return value;
}
static void cache_write_bytes(struct CACHE_STREAM *stream, const void *buffer, size_t count)
{
	if (stream->failed) {
		return;
	}
	if (fwrite(buffer, 1, count, stream->file) != count) {
		stream->failed = 1;
		return;
	}
	stream->checksum = cache_hash_bytes(stream->checksum, buffer, count);
}
static void cache_write_integer(struct CACHE_STREAM *stream, legacy_u64 value, legacy_u32 count)
{
	legacy_u8 bytes[8];
	for (legacy_u32 i = 0; i < count; i++) {
		bytes[i] = (legacy_u8)(value >> (i * 8));
	}
	cache_write_bytes(stream, bytes, count);
}
static legacy_u32 cache_texture_bytes(const struct CACHE_TEXTURE *texture)
{
	legacy_u32 level = texture->levels - 1;
	legacy_u32 width = (texture->width + (1U << level) - 1) >> level;
	legacy_u32 height = (texture->height + (1U << level) - 1) >> level;
	return texture->offsets[level] + width * height;
}
static legacy_s32 cache_read_texture(struct CACHE_STREAM *stream, struct CACHE_TEXTURE *texture,
									 const struct CACHE_FACE *face, const struct CACHE_PAGE *page)
{
	legacy_u32 present = (legacy_u32)cache_read_integer(stream, 4);
	if (stream->failed || present > 1) {
		return 0;
	}
	if (present == 0) {
		return 1;
	}
	legacy_u32 width = (legacy_u32)cache_read_integer(stream, 4);
	legacy_u32 height = (legacy_u32)cache_read_integer(stream, 4);
	legacy_u32 texel = (legacy_u32)cache_read_integer(stream, 4);
	legacy_u32 u_bits = (legacy_u32)cache_read_integer(stream, 4);
	legacy_u32 v_bits = (legacy_u32)cache_read_integer(stream, 4);
	legacy_f32 u, v;
	memcpy(&u, &u_bits, sizeof(u));
	memcpy(&v, &v_bits, sizeof(v));
	if (stream->failed || (texel != CACHE_TEXEL && texel != CACHE_COARSE_TEXEL) ||
		(u_bits & 0x7F800000U) == 0x7F800000U || (v_bits & 0x7F800000U) == 0x7F800000U ||
		width == 0 || height == 0 || width > 8192 || height > 8192 ||
		(legacy_u64)width * height > 4U * 1024U * 1024U) {
		return 0;
	}
	if (page != NULL) {
		legacy_u32 expected = CACHE_PAGE_SIZE / texel + 1;
		if (width != expected || height != expected ||
			texel != (page->dense ? CACHE_TEXEL : CACHE_COARSE_TEXEL) ||
			u != (legacy_f64)page->x * CACHE_PAGE_SIZE ||
			v != (legacy_f64)page->z * CACHE_PAGE_SIZE) {
			return 0;
		}
	} else if (face != NULL) {
		if (u < face->low[face->u_axis] - 2 * texel || v < face->low[face->v_axis] - 2 * texel ||
			u > face->high[face->u_axis] + texel || v > face->high[face->v_axis] + texel ||
			u + (legacy_f64)(width - 1) * texel > face->high[face->u_axis] + 3 * texel ||
			v + (legacy_f64)(height - 1) * texel > face->high[face->v_axis] + 3 * texel) {
			return 0;
		}
	}
	/* Check the complete mip payload before allocating, including truncation. */
	legacy_u32 bytes = 0, w = width, h = height;
	for (;;) {
		bytes += w * h;
		if (w == 1 && h == 1) {
			break;
		}
		w = (w + 1) / 2;
		h = (h + 1) / 2;
	}
	if (bytes > stream->remaining || !cache_texture_allocate(texture, width, height, u, v, texel)) {
		return 0;
	}
	cache_read_bytes(stream, texture->pixels, bytes);
	return !stream->failed;
}
static void cache_write_texture(struct CACHE_STREAM *stream, const struct CACHE_TEXTURE *texture)
{
	cache_write_integer(stream, texture->pixels != NULL, 4);
	if (texture->pixels == NULL) {
		return;
	}
	legacy_u32 u_bits, v_bits;
	memcpy(&u_bits, &texture->u, sizeof(u_bits));
	memcpy(&v_bits, &texture->v, sizeof(v_bits));
	cache_write_integer(stream, texture->width, 4);
	cache_write_integer(stream, texture->height, 4);
	cache_write_integer(stream, (legacy_u32)texture->texel, 4);
	cache_write_integer(stream, u_bits, 4);
	cache_write_integer(stream, v_bits, 4);
	cache_write_bytes(stream, texture->pixels, cache_texture_bytes(texture));
}
static legacy_s32 cache_load_file(const char *path, const legacy_u8 track_md5[16],
								  legacy_u64 geometry)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return 0;
	}
	legacy_s32 valid = 0;
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return 0;
	}
	long length = ftell(file);
	if (length < 52 || length > CACHE_MAX_DISK_BYTES || fseek(file, 0, SEEK_SET) != 0) {
		fclose(file);
		return 0;
	}
	struct CACHE_STREAM stream = {file, CACHE_HASH_OFFSET, (size_t)length - 8, 0};
	legacy_u8 magic[8] = {0}, md5[16] = {0};
	cache_read_bytes(&stream, magic, sizeof(magic));
	legacy_u32 version = (legacy_u32)cache_read_integer(&stream, 4);
	cache_read_bytes(&stream, md5, sizeof(md5));
	legacy_u64 fingerprint = cache_read_integer(&stream, 8);
	legacy_u32 faces = (legacy_u32)cache_read_integer(&stream, 4);
	legacy_u32 pages = (legacy_u32)cache_read_integer(&stream, 4);
	if (stream.failed || memcmp(magic, "RSLMAP01", 8) != 0 || version != CACHE_DISK_VERSION ||
		memcmp(md5, track_md5, 16) != 0 || fingerprint != geometry || faces != cache_face_count ||
		pages > 16384) {
		goto finished;
	}
	for (legacy_u32 i = 0; i < faces; i++) {
		if (!cache_read_texture(&stream, &cache_faces[i].texture, &cache_faces[i], NULL)) {
			goto finished;
		}
	}
	for (legacy_u32 i = 0; i < pages; i++) {
		legacy_u32 x_bits = (legacy_u32)cache_read_integer(&stream, 4);
		legacy_u32 z_bits = (legacy_u32)cache_read_integer(&stream, 4);
		legacy_u32 dense = (legacy_u32)cache_read_integer(&stream, 4);
		legacy_s32 x = x_bits <= 0x7FFFFFFFU ? (legacy_s32)x_bits : -1 - (legacy_s32)(~x_bits);
		legacy_s32 z = z_bits <= 0x7FFFFFFFU ? (legacy_s32)z_bits : -1 - (legacy_s32)(~z_bits);
		if (stream.failed || dense > 1 || x < -4096 || x > 4096 || z < -4096 || z > 4096 ||
			cache_page(x, z, 0) != NULL) {
			goto finished;
		}
		struct CACHE_PAGE *page = cache_page(x, z, 1);
		if (page == NULL) {
			goto finished;
		}
		page->dense = (legacy_s32)dense;
		if (!cache_read_texture(&stream, &page->texture, NULL, page)) {
			goto finished;
		}
	}
	if (stream.failed || stream.remaining != 0) {
		goto finished;
	}
	legacy_u8 footer[8];
	if (fread(footer, 1, sizeof(footer), file) != sizeof(footer)) {
		goto finished;
	}
	legacy_u64 checksum = 0;
	for (legacy_u32 i = 0; i < 8; i++) {
		checksum |= (legacy_u64)footer[i] << (i * 8);
	}
	valid = checksum == stream.checksum;
finished:
	if (fclose(file) != 0) {
		valid = 0;
	}
	if (!valid || !cache_build_tree()) {
		cache_discard_lighting();
		return 0;
	}
	cache_collecting = 0;
	cache_prepare_runtime();
	cache_ready = 1;
	active = 0;
	return 1;
}
static FILE *cache_create_temporary(const char *path, char **temporary)
{
	static legacy_u32 sequence;
	size_t length = strlen(path);
	if (length > 4096) {
		return NULL;
	}
	char *name = malloc(length + 64);
	if (name == NULL) {
		return NULL;
	}
	for (legacy_u32 attempt = 0; attempt < 16; attempt++) {
#if defined(_WIN32)
		unsigned long process = GetCurrentProcessId();
#else
		unsigned long process = (unsigned long)getpid();
#endif
		snprintf(name, length + 64, "%s.tmp-%lu-%lu", path, process, (unsigned long)++sequence);
#if defined(_WIN32)
		int descriptor = _open(name, _O_BINARY | _O_WRONLY | _O_CREAT | _O_EXCL | _O_NOINHERIT,
							   _S_IREAD | _S_IWRITE);
#else
		int descriptor = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
#endif
		if (descriptor < 0) {
			if (errno == EEXIST) {
				continue;
			}
			break;
		}
#if defined(_WIN32)
		FILE *file = _fdopen(descriptor, "wb");
#else
		FILE *file = fdopen(descriptor, "wb");
#endif
		if (file != NULL) {
			*temporary = name;
			return file;
		}
#if defined(_WIN32)
		_close(descriptor);
#else
		close(descriptor);
#endif
		remove(name);
		break;
	}
	free(name);
	return NULL;
}
static void cache_save_file(const char *path, const legacy_u8 track_md5[16], legacy_u64 geometry)
{
	char *temporary = NULL;
	FILE *file = cache_create_temporary(path, &temporary);
	if (file == NULL) {
		return;
	}
	struct CACHE_STREAM stream = {file, CACHE_HASH_OFFSET, 0, 0};
	cache_write_bytes(&stream, "RSLMAP01", 8);
	cache_write_integer(&stream, CACHE_DISK_VERSION, 4);
	cache_write_bytes(&stream, track_md5, 16);
	cache_write_integer(&stream, geometry, 8);
	cache_write_integer(&stream, cache_face_count, 4);
	cache_write_integer(&stream, cache_page_count, 4);
	for (legacy_u32 i = 0; i < cache_face_count; i++) {
		cache_write_texture(&stream, &cache_faces[i].texture);
	}
	for (legacy_u32 i = 0; i < cache_page_count; i++) {
		const struct CACHE_PAGE *page = &cache_pages[i];
		cache_write_integer(&stream, (legacy_u32)page->x, 4);
		cache_write_integer(&stream, (legacy_u32)page->z, 4);
		cache_write_integer(&stream, page->dense, 4);
		cache_write_texture(&stream, &page->texture);
	}
	legacy_u64 checksum = stream.checksum;
	cache_write_integer(&stream, checksum, 8);
	legacy_s32 valid = !stream.failed;
	if (fflush(file) != 0) {
		valid = 0;
	}
	if (fclose(file) != 0) {
		valid = 0;
	}
	if (valid) {
#if defined(_WIN32)
		valid =
			MoveFileExA(temporary, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
		valid = rename(temporary, path) == 0;
#endif
	}
	if (!valid) {
		remove(temporary);
	}
	free(temporary);
}
legacy_s32 shape3d_shadows_bake_end_cached(const char *path, const legacy_u8 track_md5[16])
{
	if (!cache_collecting) {
		return 0;
	}
	if (path == NULL || *path == 0 || track_md5 == NULL || sizeof(legacy_f32) != 4 ||
		sizeof(legacy_f64) != 8) {
		shape3d_shadows_bake_end();
		return 0;
	}
	legacy_u64 geometry = cache_geometry_fingerprint();
	if (cache_load_file(path, track_md5, geometry)) {
		return 1;
	}
	shape3d_shadows_bake_end();
	if (cache_ready) {
		cache_save_file(path, track_md5, geometry);
	}
	return 0;
}

static legacy_f64 cache_texture_level_sample(const legacy_u8 *pixels, legacy_u32 width,
											 legacy_u32 height, legacy_f64 x, legacy_f64 y)
{
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if (x > width - 1) {
		x = width - 1;
	}
	if (y > height - 1) {
		y = height - 1;
	}
	legacy_u32 ix = (legacy_u32)x, iy = (legacy_u32)y;
	legacy_u32 nx = ix + 1 < width ? ix + 1 : ix, ny = iy + 1 < height ? iy + 1 : iy;
	legacy_u8 first = pixels[iy * width + ix];
	legacy_u8 second = pixels[iy * width + nx];
	legacy_u8 third = pixels[ny * width + ix];
	legacy_u8 fourth = pixels[ny * width + nx];
	if (first == second && first == third && first == fourth) {
		return first;
	}
	legacy_f64 fx = x - ix, fy = y - iy;
	legacy_f64 top = first * (1 - fx) + second * fx;
	legacy_f64 bottom = third * (1 - fx) + fourth * fx;
	return top * (1 - fy) + bottom * fy;
}

static legacy_u8 cache_texture_sample(const struct CACHE_TEXTURE *texture, legacy_f64 u,
									  legacy_f64 v, legacy_f64 footprint)
{
	if (texture->pixels == NULL) {
		return 0;
	}
	legacy_f64 x = (u - texture->u) * texture->inverse_texel;
	legacy_f64 y = (v - texture->v) * texture->inverse_texel;
	if (x < 0 || y < 0 || x > texture->width - 1 || y > texture->height - 1) {
		return 0;
	}
	if (texture->uniform >= 0) {
		return (legacy_u8)texture->uniform;
	}
	legacy_u32 level = 0, width = texture->width, height = texture->height;
	while (footprint > texture->texel * 2 && level + 1 < texture->levels) {
		x = (x - 0.5) * 0.5;
		y = (y - 0.5) * 0.5;
		footprint *= 0.5;
		width = (width + 1) / 2;
		height = (height + 1) / 2;
		level++;
	}
	legacy_f64 shade =
		cache_texture_level_sample(texture->pixels + texture->offsets[level], width, height, x, y);
	if (footprint > texture->texel && level + 1 < texture->levels) {
		/* Blend adjacent resolutions continuously. Abrupt mip changes leave
		 * visible stripes in dense grille shadows as a surface recedes. */
		legacy_f64 coarse = cache_texture_level_sample(
			texture->pixels + texture->offsets[level + 1], (width + 1) / 2, (height + 1) / 2,
			(x - 0.5) * 0.5, (y - 0.5) * 0.5);
		legacy_f64 blend = footprint * texture->inverse_texel - 1;
		shade += (coarse - shade) * blend;
	}
	return (legacy_u8)(shade + 0.5);
}
static legacy_s32 cache_contains_runtime(const struct CACHE_FACE *face, const legacy_f64 *point)
{
	if (face->rectangular) {
		return 1;
	}
	if (cache_edges == NULL || face->first_edge == CACHE_NONE) {
		return cache_contains(face, point, CACHE_RECEIVER_TOLERANCE);
	}
	legacy_s32 sign = 0;
	legacy_f64 u = point[face->u_axis], v = point[face->v_axis];
	for (legacy_u32 i = 0; i < face->count; i++) {
		const struct CACHE_EDGE *edge = &cache_edges[face->first_edge + i];
		legacy_f64 cross = edge->du * (v - edge->v) - edge->dv * (u - edge->u);
		if (cross > edge->margin) {
			if (sign < 0) {
				return 0;
			}
			sign = 1;
		} else if (cross < -edge->margin) {
			if (sign > 0) {
				return 0;
			}
			sign = -1;
		}
	}
	return 1;
}
static legacy_s32 cache_matches(legacy_u32 index, const legacy_f64 *point, legacy_f64 *distance)
{
	const struct CACHE_FACE *face = &cache_faces[index];
	if (face->axial) {
		/* On an exact coordinate plane, the plane test replaces one bounds
		 * pair and three multiplies. Reject the wrong height before testing
		 * the other coordinates; nearby unlit roads are common candidates. */
		*distance = shadow_absolute(point[face->axis] - face->low[face->axis]);
		if (!(*distance <= CACHE_RECEIVER_TOLERANCE &&
			  point[face->u_axis] >= face->low[face->u_axis] - CACHE_RECEIVER_TOLERANCE &&
			  point[face->u_axis] <= face->high[face->u_axis] + CACHE_RECEIVER_TOLERANCE &&
			  point[face->v_axis] >= face->low[face->v_axis] - CACHE_RECEIVER_TOLERANCE &&
			  point[face->v_axis] <= face->high[face->v_axis] + CACHE_RECEIVER_TOLERANCE)) {
			return 0;
		}
		return cache_contains_runtime(face, point);
	}
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		if (point[axis] < face->low[axis] - CACHE_RECEIVER_TOLERANCE ||
			point[axis] > face->high[axis] + CACHE_RECEIVER_TOLERANCE) {
			return 0;
		}
	}
	*distance = shadow_absolute(face->normal[0] * point[0] + face->normal[1] * point[1] +
								face->normal[2] * point[2] - face->plane);
	return *distance <= CACHE_RECEIVER_TOLERANCE && cache_contains_runtime(face, point);
}
static legacy_u32 cache_receiver(const legacy_f64 *point, legacy_u32 hint)
{
	legacy_f64 best_distance = CACHE_RECEIVER_TOLERANCE + 1;
	if (hint > 0 && hint <= cache_face_count && cache_matches(hint - 1, point, &best_distance)) {
		return hint - 1;
	}
	if (cache_node_count == 0) {
		return CACHE_NONE;
	}
	legacy_u32 stack[64], count = 1, best = CACHE_NONE;
	stack[0] = 0;
	while (count != 0) {
		const struct CACHE_NODE *node = &cache_nodes[stack[--count]];
		legacy_s32 inside = 1;
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			inside &= point[axis] >= node->low[axis] - CACHE_RECEIVER_TOLERANCE &&
					  point[axis] <= node->high[axis] + CACHE_RECEIVER_TOLERANCE;
		}
		if (!inside) {
			continue;
		}
		for (legacy_u32 i = node->first; i < node->first + node->count; i++) {
			legacy_u32 index = cache_indices[i];
			legacy_f64 distance;
			if (cache_matches(index, point, &distance) && distance < best_distance) {
				best_distance = distance;
				best = index;
			}
		}
		if (node->left != CACHE_NONE &&
			point[node->axis] <= node->split + CACHE_RECEIVER_TOLERANCE) {
			stack[count++] = node->left;
		}
		if (node->right != CACHE_NONE &&
			point[node->axis] >= node->split - CACHE_RECEIVER_TOLERANCE) {
			stack[count++] = node->right;
		}
	}
	return best;
}
static const struct CACHE_PAGE *cache_ground_page(legacy_f64 x, legacy_f64 z, legacy_u32 *hint)
{
	if (hint != NULL && (*hint & CACHE_HINT_MASK) == CACHE_GROUND_HINT) {
		legacy_u32 index = (*hint & ~CACHE_HINT_MASK) - 1;
		if (index < cache_page_count) {
			const struct CACHE_PAGE *page = &cache_pages[index];
			legacy_f64 left = (legacy_f64)page->x * CACHE_PAGE_SIZE;
			legacy_f64 top = (legacy_f64)page->z * CACHE_PAGE_SIZE;
			if (x >= left && x < left + CACHE_PAGE_SIZE && z >= top && z < top + CACHE_PAGE_SIZE) {
				return page;
			}
		}
	} else if (hint != NULL && (*hint & CACHE_HINT_MASK) == CACHE_EMPTY_HINT) {
		legacy_s32 px = (legacy_s32)(*hint & 8191U) - 4096;
		legacy_s32 pz = (legacy_s32)((*hint >> 13) & 8191U) - 4096;
		legacy_f64 left = (legacy_f64)px * CACHE_PAGE_SIZE;
		legacy_f64 top = (legacy_f64)pz * CACHE_PAGE_SIZE;
		if (x >= left && x < left + CACHE_PAGE_SIZE && z >= top && z < top + CACHE_PAGE_SIZE) {
			return NULL;
		}
	}
	legacy_s32 px = shadow_floor(x / CACHE_PAGE_SIZE);
	legacy_s32 pz = shadow_floor(z / CACHE_PAGE_SIZE);
	const struct CACHE_PAGE *page = cache_page(px, pz, 0);
	if (hint != NULL) {
		if (page != NULL) {
			*hint = CACHE_GROUND_HINT | ((legacy_u32)(page - cache_pages) + 1);
		} else if (px >= -4096 && px < 4096 && pz >= -4096 && pz < 4096) {
			*hint = CACHE_EMPTY_HINT | (legacy_u32)(px + 4096) | ((legacy_u32)(pz + 4096) << 13);
		} else {
			*hint = 0;
		}
	}
	return page;
}
static legacy_u8 cache_sample(legacy_f64 x, legacy_f64 y, legacy_f64 z, legacy_f64 footprint,
							  legacy_u32 *hint, legacy_f64 fade)
{
	if (!active || !cache_ready) {
		return 0;
	}
	legacy_f64 point[3] = {x + camera.x, y + camera.y, z + camera.z};
	legacy_f64 normal[3] = {0, 1, 0};
	legacy_u8 shade = 0;
	legacy_s32 receiver_valid = 0;
	if (shadow_absolute(point[1]) <= CACHE_RECEIVER_TOLERANCE) {
		receiver_valid = 1;
		const struct CACHE_PAGE *page = cache_ground_page(point[0], point[2], hint);
		if (page != NULL) {
			shade = cache_texture_sample(&page->texture, point[0], point[2], footprint);
		}
	} else {
		legacy_u32 receiver = cache_receiver(point, hint != NULL ? *hint : 0);
		if (hint != NULL) {
			*hint = receiver != CACHE_NONE ? receiver + 1 : 0;
		}
		if (receiver != CACHE_NONE) {
			receiver_valid = 1;
			const struct CACHE_FACE *face = &cache_faces[receiver];
			shade = cache_texture_sample(&face->texture, point[face->u_axis], point[face->v_axis],
										 footprint);
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				normal[axis] = face->normal[axis];
			}
		}
	}
	shade = (legacy_u8)(shade * fade + 0.5);
	if (dynamic_polygons != 0 && receiver_valid) {
		legacy_f64 light_x = point[0] + SHADOW_LIGHT_X * point[1];
		legacy_f64 light_z = point[2] + SHADOW_LIGHT_Z * point[1];
		legacy_s32 cast = light_x >= dynamic_light_low[0] - SHADOW_TEXEL_SIZE &&
						  light_x <= dynamic_light_high[0] + SHADOW_TEXEL_SIZE &&
						  light_z >= dynamic_light_low[1] - SHADOW_TEXEL_SIZE &&
						  light_z <= dynamic_light_high[1] + SHADOW_TEXEL_SIZE;
		legacy_s32 contact =
			point[0] >= dynamic_contact_low[0] - 3 * SHADOW_TEXEL_SIZE - SHADOW_TEXEL_SIZE &&
			point[0] <= dynamic_contact_high[0] + 3 * SHADOW_TEXEL_SIZE + SHADOW_TEXEL_SIZE &&
			point[2] >= dynamic_contact_low[2] - 3 * SHADOW_TEXEL_SIZE - SHADOW_TEXEL_SIZE &&
			point[2] <= dynamic_contact_high[2] + 3 * SHADOW_TEXEL_SIZE + SHADOW_TEXEL_SIZE &&
			point[1] + SHADOW_CONTACT_HEIGHT > dynamic_contact_low[1];
		if ((!cast && !contact) || point[1] + SHADOW_DEPTH_BIAS >= dynamic_contact_high[1]) {
			return shade;
		}
		legacy_u8 moving = shape3d_shadows_sample_plane(x, y, z, normal[0], normal[1], normal[2]);
		if (moving > shade) {
			shade = moving;
		}
	}
	return shade;
}

legacy_u8 shape3d_shadows_sample_cached(legacy_f64 x, legacy_f64 y, legacy_f64 z,
										legacy_f64 footprint, legacy_u32 *hint)
{
	return cache_sample(x, y, z, footprint, hint, 1);
}

legacy_u8 shape3d_shadows_sample_cached_view(legacy_f64 x, legacy_f64 y, legacy_f64 z,
											 legacy_f64 footprint, legacy_u32 *hint)
{
	/* Keep the existing view range while retaining the entire track's bake.
	 * Reject distant receivers before looking up pages or walking the BSP. */
	legacy_f64 dx = shadow_absolute(x + SHADOW_LIGHT_X * y);
	legacy_f64 dz = shadow_absolute(z + SHADOW_LIGHT_Z * y);
	legacy_f64 distance = dx > dz ? dx : dz;
	if (distance >= SHADOW_MAP_HALF_EXTENT) {
		return 0;
	}
	legacy_f64 fade =
		distance <= SHADOW_FADE_START
			? 1
			: (SHADOW_MAP_HALF_EXTENT - distance) / (SHADOW_MAP_HALF_EXTENT - SHADOW_FADE_START);
	return cache_sample(x, y, z, footprint, hint, fade);
}

#endif
