#include "shape3d_hires.h"

#if defined(RESTUNTS_SDL3)

#include <SDL3/SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>
#include "fatal.h"
#include "hires.h"
#include "projection.h"
#include "render_workers.h"
#include "shape3d_internal.h"

#define HIRES_NEAR_CLIP_Z 12
#define HIRES_BAND_HEIGHT 32
#define HIRES_BAND_COUNT (HIRES_HEIGHT / HIRES_BAND_HEIGHT)
#define HIRES_PARALLEL_MIN_AREA 65536U
#define HIRES_INITIAL_COMMAND_CAPACITY 1024U
/* Keep the original displayed pixel width through medium-close views,
 * then let perspective narrow the stroke at greater distances. */
#define HIRES_LINE_DIAMETER 1.5
/* A subpixel decal still needs coverage across diagonal sample gaps. */
#define HIRES_MIN_DECAL_WIDTH 1.5
#define HIRES_DECAL_WIDTH_REDUCTION 2
#define HIRES_MAX_POLYGON_POINTS 20
#define HIRES_ROUND_POINTS 64
#define HIRES_WHEEL_INNER_SCALE (9472.0 / TRIG_FIXED_ONE)

struct HIRES_PRIMITIVE {
	struct SHAPE3D_HIRES_POINT points[HIRES_MAX_POLYGON_POINTS];
	legacy_u32 count;
	legacy_u32 wheel_face;
	legacy_u32 shape;
	legacy_u32 family;
	legacy_s32 attached;
	legacy_f64 size;
	legacy_f64 depth;
};

struct HIRES_PAINT {
	struct HIRES_RASTER_CONTEXT *context;
	legacy_u16 color;
	legacy_u16 alternate;
	legacy_u16 pattern;
	legacy_u16 mode;
	legacy_s32 depth_test;
	legacy_u32 family;
	legacy_s32 depth_mode;
};

struct HIRES_SHAPE {
	legacy_s32 depth_mode;
	legacy_s32 shadow_receiver;
};

#define HIRES_CAR_SHADOW_COUNT 2
#define HIRES_CAR_SHADOW_REACH 256.0
#define HIRES_CAR_SHADOW_HEIGHT_SCALE 0.6
/* Cotangent of the 70-degree sun elevation; preserve the authored approximation. */
#define HIRES_CAR_SHADOW_SUN_COTANGENT 0.363970
#define HIRES_CAR_SHADOW_OFFSET_LIMIT_SCALE 0.7
#define HIRES_CAR_SHADOW_RECEIVER_TOLERANCE 0.5
#define HIRES_CAR_SHADOW_SHRINK_HEIGHT 128
#define HIRES_CAR_SHADOW_FALLBACK_HEIGHT_SCALE 0.5
#define HIRES_CAR_SHADOW_FALLBACK_DIAGONAL 1.7
#define HIRES_CAR_SHADOW_FALLBACK_EDGE_SCALE 8
#define HIRES_CAR_SHADOW_FADE_HEIGHT 64
#define HIRES_CAR_SHADOW_MAX_OPACITY 96
#define HIRES_CAR_SHADOW_BOUNDS_CORNERS 8
#define HIRES_CAR_SHADOW_BOUNDS_EDGES 12
#define HIRES_CAR_SHADOW_CORNER_MAX_X 1
#define HIRES_CAR_SHADOW_CORNER_MAX_Z 2
#define HIRES_CAR_SHADOW_CORNER_BOTTOM 4

struct HIRES_CAR_SHADOW {
	struct VECTOR position;
	legacy_f64 cosine, sine;
	legacy_f64 half_width, half_length;
	legacy_f64 min_x, max_x, min_z, max_z, height;
	const struct SHADOW_SILHOUETTE *silhouette;
};

static struct HIRES_CAR_SHADOW car_shadows[HIRES_CAR_SHADOW_COUNT];
static legacy_s32 car_shadow_count;
static legacy_s32 shadow_model_pending;
static struct MATRIX shadow_view;
static legacy_f64 shadow_inverse[3][3];
static legacy_f64 shadow_ground_y;

static struct HIRES_PRIMITIVE *primitives;
static struct HIRES_SHAPE *shapes;
static legacy_u32 primitive_capacity;
static legacy_u32 primitive_count;
static legacy_u32 current_shape;
static legacy_u32 current_family;
static legacy_s32 rendered_depth_valid;
static legacy_u32 rendered_generation;
static legacy_f64 model_scale = 1;

struct HIRES_COMMAND {
	legacy_u32 index;
	legacy_u8 type;
	legacy_u16 color, second_color, third_color, pattern_type, pattern;
	legacy_s32 top, bottom;
};

static struct HIRES_COMMAND *commands;
static size_t command_capacity;
static size_t command_count;
static legacy_u32 command_area;
static legacy_s32 batching;

static void reserve_primitives(legacy_u32 index)
{
	if (index < primitive_capacity) {
		return;
	}
	size_t capacity =
		primitive_capacity != 0 ? primitive_capacity : POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY;
	while (capacity <= index) {
		if (capacity > LEGACY_S32_MAX / 2U) {
			fatal_error("SuperSight scene has too many primitives");
			return;
		}
		capacity *= 2U;
	}
	if (capacity > (size_t)-1 / sizeof(*primitives) || capacity > (size_t)-1 / sizeof(*shapes)) {
		fatal_error("SuperSight scene exceeds addressable memory");
		return;
	}
	struct HIRES_PRIMITIVE *new_primitives = calloc(capacity, sizeof(*new_primitives));
	struct HIRES_SHAPE *new_shapes = calloc(capacity, sizeof(*new_shapes));
	if (new_primitives == NULL || new_shapes == NULL) {
		fatal_error("Cannot allocate SuperSight scene geometry");
		return;
	}
	if (primitive_capacity != 0) {
		memcpy(new_primitives, primitives, (size_t)primitive_capacity * sizeof(*new_primitives));
		memcpy(new_shapes, shapes, (size_t)primitive_capacity * sizeof(*new_shapes));
	}
	free(primitives);
	free(shapes);
	primitives = new_primitives;
	shapes = new_shapes;
	primitive_capacity = (legacy_u32)capacity;
}

static void project_coordinates(legacy_f64 x, legacy_f64 y, legacy_f64 z,
								struct SHAPE3D_HIRES_POINT *point)
{
	point->x = (legacy_s16)projection_center_x * HIRES_SCALE +
			   x * projection_focal_length_x * HIRES_SCALE / z;
	point->y = (legacy_s16)projection_center_y * HIRES_SCALE -
			   y * projection_focal_length_y * HIRES_SCALE / z;
	point->inverse_z = 1.0 / z;
}

void shape3d_hires_project(const struct SHAPE3D_HIRES_VECTOR *vector,
						   struct SHAPE3D_HIRES_POINT *point)
{
	project_coordinates(vector->x, vector->y, vector->z > 0 ? vector->z : 1, point);
}

static legacy_u8 point_clip_flags(const struct SHAPE3D_HIRES_POINT *point, legacy_f64 padding)
{
	legacy_u8 flags = 0;
	if (point->y < select_rect_rc.top * HIRES_SCALE - padding) {
		flags |= SHAPE3D_RECT_CLIP_TOP;
	} else if (point->y >= (select_rect_rc.bottom + 1) * HIRES_SCALE + padding) {
		flags |= SHAPE3D_RECT_CLIP_BOTTOM;
	}
	if (point->x < select_rect_rc.left * HIRES_SCALE - padding) {
		flags |= SHAPE3D_RECT_CLIP_LEFT;
	} else if (point->x >= (select_rect_rc.right + 1) * HIRES_SCALE + padding) {
		flags |= SHAPE3D_RECT_CLIP_RIGHT;
	}
	return flags;
}

legacy_u8 shape3d_hires_clip_flags(const struct SHAPE3D_HIRES_VECTOR *vector)
{
	struct SHAPE3D_HIRES_POINT point;
	shape3d_hires_project(vector, &point);
	/* The shared early cull must retain lines whose wider stroke reaches
	 * into the viewport even when their centerline is just outside it. */
	return point_clip_flags(&point, HIRES_SCALE / 2.0 + 0.5);
}

static legacy_s32 polygon_faces_camera(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count)
{
	/* Use every edge: the first three vertices can be collinear even when
	 * the complete polygon covers visible pixels at the higher resolution. */
	legacy_f64 area = 0;
	for (legacy_u32 index = 1; index + 1 < count; index++) {
		area += (points[index].x - points[0].x) * (points[index + 1].y - points[0].y) -
				(points[index].y - points[0].y) * (points[index + 1].x - points[0].x);
	}
	return area > 0;
}

static legacy_f64 decal_width(legacy_f64 inverse_z)
{
	legacy_f64 width =
		HIRES_LINE_DIAMETER * model_scale * projection_focal_length_x * HIRES_SCALE * inverse_z;
	if (width >= HIRES_SCALE) {
		return HIRES_SCALE;
	}
	if (width <= HIRES_MIN_DECAL_WIDTH) {
		return HIRES_MIN_DECAL_WIDTH;
	}
	/* Reduce mid-range coverage while preserving the near cap and distant visibility floor. */
	legacy_f64 excess = width - HIRES_MIN_DECAL_WIDTH;
	legacy_f64 reduction = excess - excess * excess / (HIRES_SCALE - HIRES_MIN_DECAL_WIDTH);
	width -= HIRES_DECAL_WIDTH_REDUCTION * reduction;
	return width > HIRES_MIN_DECAL_WIDTH ? width : HIRES_MIN_DECAL_WIDTH;
}

/* Measure the complete projected polygon across each edge normal. The coverage
 * target follows perspective, with a small floor to keep distant decals solid. */
static legacy_f64 polygon_padding(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count)
{
	legacy_f64 minimum_width = 0;
	legacy_s32 has_edge = 0;
	legacy_f64 nearest = points[0].inverse_z;
	legacy_f64 farthest = nearest;
	for (legacy_u32 edge = 0; edge < count; edge++) {
		const struct SHAPE3D_HIRES_POINT *first = &points[edge];
		const struct SHAPE3D_HIRES_POINT *last = &points[(edge + 1) % count];
		if (first->inverse_z > nearest) {
			nearest = first->inverse_z;
		}
		if (first->inverse_z < farthest) {
			farthest = first->inverse_z;
		}
		legacy_f64 dx = last->x - first->x;
		legacy_f64 dy = last->y - first->y;
		legacy_f64 length = SDL_sqrt(dx * dx + dy * dy);
		if (length == 0) {
			continue;
		}
		legacy_f64 minimum = 0;
		legacy_f64 maximum = 0;
		for (legacy_u32 point = 0; point < count; point++) {
			legacy_f64 distance =
				(points[point].x - first->x) * dy - (points[point].y - first->y) * dx;
			if (distance < minimum) {
				minimum = distance;
			}
			if (distance > maximum) {
				maximum = distance;
			}
		}
		legacy_f64 width = (maximum - minimum) / length;
		if (!has_edge || width < minimum_width) {
			minimum_width = width;
		}
		has_edge = 1;
	}
	if (minimum_width >= HIRES_SCALE) {
		return 0;
	}
	legacy_f64 expansion = decal_width(nearest) - minimum_width;
	/* A sloping dash can taper below a sample at its far end even when its
	 * near end is wide enough. Account for that narrowing before adding a border. */
	legacy_f64 far_expansion = decal_width(farthest) - minimum_width * farthest / nearest;
	if (far_expansion > expansion) {
		expansion = far_expansion;
	}
	/* Keep the original nearby weight and a smooth transition to unexpanded surfaces. */
	if (expansion > HIRES_SCALE - minimum_width) {
		expansion = HIRES_SCALE - minimum_width;
	}
	return expansion > 0 ? expansion * 0.5 : 0;
}

legacy_s32 shape3d_hires_polygon_visible(legacy_u32 index, legacy_s32 cull_backface)
{
	if (index >= primitive_capacity) {
		return 0;
	}
	const struct HIRES_PRIMITIVE *primitive = &primitives[index];
	if (primitive->count < 3) {
		return 0;
	}
	legacy_u8 flags = SHAPE3D_ALL_RECT_CLIP_FLAGS;
	for (legacy_u32 point = 0; point < primitive->count; point++) {
		flags &= point_clip_flags(&primitive->points[point], primitive->size);
	}
	return flags == 0 &&
		   (!cull_backface || polygon_faces_camera(primitive->points, primitive->count));
}

legacy_u32 shape3d_hires_wheel_face(legacy_u32 index)
{
	return index < primitive_capacity ? primitives[index].wheel_face : 0;
}

legacy_f64 shape3d_hires_depth(legacy_u32 index)
{
	return index < primitive_capacity ? primitives[index].depth : 0;
}

void shape3d_hires_begin_shape(legacy_u32 index, legacy_s32 depth_mode)
{
	reserve_primitives(index);
	current_shape = index;
	current_family = index + 1;
	shapes[index].depth_mode = depth_mode;
	shapes[index].shadow_receiver = 1;
}

void shape3d_hires_set_shadow_receiver(legacy_s32 enabled)
{
	shapes[current_shape].shadow_receiver = enabled;
}

void shape3d_hires_set_model_scale(legacy_f64 scale)
{
	model_scale = scale;
}

void shape3d_hires_reset(void)
{
	car_shadow_count = 0;
	shadow_model_pending = 0;
	model_scale = 1;
	shape3d_hires_begin_shape(0, SHAPE3D_HIRES_DEPTH_SORTED);
	rendered_depth_valid = 0;
	for (legacy_u32 index = 0; index < primitive_count; index++) {
		primitives[index].count = 0;
	}
	primitive_count = 0;
}

static void project_intersection(const struct SHAPE3D_HIRES_VECTOR *first,
								 const struct SHAPE3D_HIRES_VECTOR *second,
								 struct SHAPE3D_HIRES_POINT *point)
{
	/* Evaluate shared edges in the same direction so clipping neighboring
	 * polygons cannot round the same intersection to opposite pixel sides. */
	if (first->z > second->z) {
		const struct SHAPE3D_HIRES_VECTOR *temporary = first;
		first = second;
		second = temporary;
	}
	/* Near-plane intersections need the same subpixel precision as ordinary
	 * vertices; rounding back into a legacy VECTOR would discard it. */
	legacy_f64 fraction = (HIRES_NEAR_CLIP_Z - first->z) / (legacy_f64)(second->z - first->z);
	project_coordinates(first->x + (second->x - first->x) * fraction,
						first->y + (second->y - first->y) * fraction, HIRES_NEAR_CLIP_Z, point);
}

static void queue_polygon(struct HIRES_PRIMITIVE *primitive, legacy_u32 count,
						  const legacy_u8 *indices, const struct SHAPE3D_HIRES_VECTOR *vertices)
{
	const struct SHAPE3D_HIRES_VECTOR *previous = &vertices[indices[count - 1]];
	for (legacy_u32 index = 0; index < count; index++) {
		const struct SHAPE3D_HIRES_VECTOR *current = &vertices[indices[index]];
		if ((previous->z < HIRES_NEAR_CLIP_Z) != (current->z < HIRES_NEAR_CLIP_Z)) {
			project_intersection(previous, current, &primitive->points[primitive->count++]);
		}
		if (current->z >= HIRES_NEAR_CLIP_Z) {
			shape3d_hires_project(current, &primitive->points[primitive->count++]);
		}
		previous = current;
	}
}

static void queue_primitive(legacy_u32 index, legacy_u8 type, legacy_u16 vertex_count,
							const legacy_u8 *indices, const struct SHAPE3D_HIRES_VECTOR *vertices)
{
	reserve_primitives(index);
	if (primitive_count <= index) {
		primitive_count = index + 1U;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	primitive->count = 0;
	primitive->size = 0;
	if (!hires_enabled() || vertex_count == 0 || vertex_count > HIRES_MAX_POLYGON_POINTS / 2) {
		return;
	}
	/* Keep scene sorting independent of the serialized 16-bit depth word.
	 * Track corners can be farther than 32767 units from the camera. */
	primitive->depth = 0;
	for (legacy_u32 vertex = 0; vertex < vertex_count; vertex++) {
		primitive->depth += vertices[indices[vertex]].z;
	}
	primitive->depth /= vertex_count;
	if (type == RENDER_PRIMITIVE_POLYGON) {
		queue_polygon(primitive, vertex_count, indices, vertices);
		return;
	}
	if (type == RENDER_PRIMITIVE_WHEEL) {
		struct SHAPE3D_HIRES_POINT face[SHAPE3D_WHEEL_RIM_VERTEX_COUNT];
		for (legacy_u32 vertex = 0; vertex < SHAPE3D_WHEEL_RIM_VERTEX_COUNT; vertex++) {
			shape3d_hires_project(&vertices[indices[vertex]], &face[vertex]);
		}
		legacy_u32 start = polygon_faces_camera(face, SHAPE3D_WHEEL_RIM_VERTEX_COUNT)
							   ? 0
							   : SHAPE3D_WHEEL_RIM_VERTEX_COUNT;
		primitive->wheel_face = start;
		primitive->depth = vertices[indices[start]].z;
		for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
			shape3d_hires_project(&vertices[indices[(start + vertex) % 6]],
								  &primitive->points[vertex]);
		}
		primitive->count = 4;
		return;
	}
	for (legacy_u32 vertex = 0; vertex < vertex_count; vertex++) {
		const struct SHAPE3D_HIRES_VECTOR *current = &vertices[indices[vertex]];
		if (type == RENDER_PRIMITIVE_LINE && current->z < HIRES_NEAR_CLIP_Z) {
			project_intersection(current, &vertices[indices[1 - vertex]],
								 &primitive->points[vertex]);
		} else {
			shape3d_hires_project(current, &primitive->points[vertex]);
		}
	}
	primitive->count = vertex_count;
	if (type == RENDER_PRIMITIVE_LINE) {
		/* Capture projection and authored model units with the queued geometry. */
		primitive->size =
			HIRES_LINE_DIAMETER * model_scale * projection_focal_length_x * HIRES_SCALE;
	}
	if (type == RENDER_PRIMITIVE_SPHERE) {
		const struct SHAPE3D_HIRES_VECTOR *center = &vertices[indices[0]];
		const struct SHAPE3D_HIRES_VECTOR *endpoint = &vertices[indices[1]];
		legacy_f64 radius_x = center->x - endpoint->x;
		legacy_f64 radius_y = center->y - endpoint->y;
		legacy_f64 radius_z = center->z - endpoint->z;
		legacy_f64 radius =
			SDL_sqrt(radius_x * radius_x + radius_y * radius_y + radius_z * radius_z);
		primitive->size = projection_focal_length_x * radius * HIRES_SCALE / center->z;
	}
}

void shape3d_hires_queue(legacy_u32 index, legacy_u8 type, legacy_u16 vertex_count,
						 const legacy_u8 *indices, const struct SHAPE3D_HIRES_VECTOR *vertices,
						 legacy_u16 flags)
{
	queue_primitive(index, type, vertex_count, indices, vertices);
	if (index >= primitive_capacity || primitives[index].count == 0) {
		return;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	primitive->shape = current_shape;
	primitive->attached = (flags & SHAPE3D_PRIMITIVE_SKIP_DEPTH_SORT_FLAG) != 0;
	if (type == RENDER_PRIMITIVE_POLYGON && primitive->attached) {
		primitive->size = polygon_padding(primitive->points, primitive->count);
	}
	if (!primitive->attached) {
		current_family = index + 1;
	}
	primitive->family = current_family;
}

static legacy_s32 ceil_coordinate(legacy_f64 coordinate)
{
	legacy_s32 result = (legacy_s32)coordinate;
	return result < coordinate ? result + 1 : result;
}

static legacy_f64 absolute_coordinate(legacy_f64 value)
{
	return value < 0 ? -value : value;
}

void shape3d_hires_update_bounds(legacy_u32 index, legacy_u8 type, struct RECTANGLE *rectangle)
{
	if (index >= primitive_capacity || primitives[index].count == 0) {
		return;
	}
	const struct HIRES_PRIMITIVE *primitive = &primitives[index];
	legacy_f64 minimum_x = primitive->points[0].x;
	legacy_f64 maximum_x = minimum_x;
	legacy_f64 minimum_y = primitive->points[0].y;
	legacy_f64 maximum_y = minimum_y;
	if (type == RENDER_PRIMITIVE_SPHERE) {
		minimum_x -= primitive->size * 0.5;
		maximum_x += primitive->size * 0.5;
		minimum_y -= primitive->size * (13.0 / 32.0);
		maximum_y += primitive->size * (13.0 / 32.0);
	} else if (type == RENDER_PRIMITIVE_WHEEL) {
		/* The sum of the two axis magnitudes bounds every perimeter point,
		 * including tilted ellipses and the far edge of the tread. */
		legacy_f64 width = absolute_coordinate(primitive->points[1].x - minimum_x) +
						   absolute_coordinate(primitive->points[2].x - minimum_x);
		legacy_f64 height = absolute_coordinate(primitive->points[1].y - minimum_y) +
							absolute_coordinate(primitive->points[2].y - minimum_y);
		if (primitive->points[3].x < minimum_x) {
			minimum_x = primitive->points[3].x;
		} else {
			maximum_x = primitive->points[3].x;
		}
		if (primitive->points[3].y < minimum_y) {
			minimum_y = primitive->points[3].y;
		} else {
			maximum_y = primitive->points[3].y;
		}
		minimum_x -= width;
		maximum_x += width;
		minimum_y -= height;
		maximum_y += height;
	} else {
		for (legacy_u32 point = 1; point < primitive->count; point++) {
			if (primitive->points[point].x < minimum_x) {
				minimum_x = primitive->points[point].x;
			}
			if (primitive->points[point].x > maximum_x) {
				maximum_x = primitive->points[point].x;
			}
			if (primitive->points[point].y < minimum_y) {
				minimum_y = primitive->points[point].y;
			}
			if (primitive->points[point].y > maximum_y) {
				maximum_y = primitive->points[point].y;
			}
		}
	}
	if (type == RENDER_PRIMITIVE_LINE || type == RENDER_PRIMITIVE_POLYGON) {
		/* Include the maximum stroke and rounding to its nearest sample
		 * in the sprite copy rectangle. */
		legacy_f64 padding =
			type == RENDER_PRIMITIVE_LINE ? HIRES_SCALE / 2.0 + 0.5 : primitive->size;
		minimum_x -= padding;
		maximum_x += padding;
		minimum_y -= padding;
		maximum_y += padding;
	}
	if (maximum_x < 0 || minimum_x >= HIRES_WIDTH || maximum_y < 0 || minimum_y >= HIRES_HEIGHT) {
		return;
	}
	legacy_s32 left = minimum_x <= 0 ? 0 : (legacy_s32)(minimum_x / HIRES_SCALE);
	legacy_s32 top = minimum_y <= 0 ? 0 : (legacy_s32)(minimum_y / HIRES_SCALE);
	legacy_s32 right = maximum_x >= HIRES_WIDTH - HIRES_SCALE
						   ? HIRES_WIDTH / HIRES_SCALE
						   : ceil_coordinate(maximum_x / HIRES_SCALE) + 1;
	legacy_s32 bottom = maximum_y >= HIRES_HEIGHT - HIRES_SCALE
							? HIRES_HEIGHT / HIRES_SCALE
							: ceil_coordinate(maximum_y / HIRES_SCALE) + 1;
	if (left < rectangle->left) {
		rectangle->left = (legacy_s16)left;
	}
	if (right > rectangle->right) {
		rectangle->right = (legacy_s16)right;
	}
	if (top < rectangle->top) {
		rectangle->top = (legacy_s16)top;
	}
	if (bottom > rectangle->bottom) {
		rectangle->bottom = (legacy_s16)bottom;
	}
}

static legacy_s32 polygon_covers_sample(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
										legacy_f64 x, legacy_f64 y)
{
	legacy_s32 inside = 0;
	const struct SHAPE3D_HIRES_POINT *previous = &points[count - 1];
	for (legacy_u32 index = 0; index < count; index++) {
		const struct SHAPE3D_HIRES_POINT *current = &points[index];
		if ((previous->y <= y && current->y > y) || (current->y <= y && previous->y > y)) {
			/* Match the fill's half-open spans and shared-edge arithmetic exactly. */
			const struct SHAPE3D_HIRES_POINT *lower = previous->y < current->y ? previous : current;
			const struct SHAPE3D_HIRES_POINT *upper = previous->y < current->y ? current : previous;
			legacy_f64 fraction = (y - lower->y) / (upper->y - lower->y);
			if (lower->x + fraction * (upper->x - lower->x) <= x) {
				inside = !inside;
			}
		}
		previous = current;
	}
	return inside;
}

static void paint_pixel(legacy_s32 x, legacy_s32 y, legacy_f64 inverse_z,
						const struct HIRES_PAINT *paint)
{
	struct HIRES_RASTER_CONTEXT *context = paint->context;
	if (context != NULL && (y < context->top || y >= context->bottom)) {
		return;
	}
	legacy_u16 color = paint->color;
	if (paint->mode != 0) {
		legacy_u32 bit = ((y & 1) == 0 ? 8U : 0U) + 7U - (x & 7);
		if ((paint->pattern & (1U << bit)) != 0) {
			color = paint->mode == 2 ? paint->alternate : paint->color;
		} else if (paint->mode != 2) {
			return;
		}
	}
	if (context != NULL) {
		if (!paint->depth_test ||
			hires_raster_depth_test(context, x, y, inverse_z, paint->family, paint->depth_mode)) {
			hires_raster_pixel(context, x, y, (legacy_u8)color);
		}
	} else if (!paint->depth_test ||
			   hires_depth_test(x, y, inverse_z, paint->family, paint->depth_mode)) {
		hires_pixel(x, y, (legacy_u8)color);
	}
}

static legacy_s32 clip_line_edge(legacy_f64 direction, legacy_f64 distance, legacy_f64 *first,
								 legacy_f64 *last)
{
	if (direction == 0) {
		return distance >= 0;
	}
	legacy_f64 ratio = distance / direction;
	if (direction < 0) {
		if (ratio > *last) {
			return 0;
		}
		if (ratio > *first) {
			*first = ratio;
		}
	} else {
		if (ratio < *first) {
			return 0;
		}
		if (ratio < *last) {
			*last = ratio;
		}
	}
	return 1;
}

static void paint_line_stroke(legacy_s32 x, legacy_s32 y, const struct SHAPE3D_HIRES_POINT *first,
							  const struct SHAPE3D_HIRES_POINT *last, legacy_f64 projected_width,
							  legacy_f64 inverse_length_squared, const struct HIRES_PAINT *paint)
{
	legacy_f64 delta_x = last->x - first->x;
	legacy_f64 delta_y = last->y - first->y;
	/* Endpoint rounding can shift the Bresenham path by almost one sample
	 * from the fractional segment; include that in the candidate search. */
	legacy_s32 extent = HIRES_SCALE / 2 + 1;
	legacy_s32 top = y - extent;
	legacy_s32 bottom = y + extent + 1;
	if (paint->context != NULL) {
		if (top < paint->context->top) {
			top = paint->context->top;
		}
		if (bottom > paint->context->bottom) {
			bottom = paint->context->bottom;
		}
	}
	for (legacy_s32 row = top; row < bottom; row++) {
		for (legacy_s32 column = x - extent; column <= x + extent; column++) {
			legacy_f64 offset_x = column + 0.5 - first->x;
			legacy_f64 offset_y = row + 0.5 - first->y;
			legacy_f64 fraction =
				inverse_length_squared == 0
					? (first->inverse_z < last->inverse_z ? 1 : 0)
					: (offset_x * delta_x + offset_y * delta_y) * inverse_length_squared;
			if (fraction < 0) {
				fraction = 0;
			} else if (fraction > 1) {
				fraction = 1;
			}
			legacy_f64 inverse_z =
				first->inverse_z + (last->inverse_z - first->inverse_z) * fraction;
			legacy_f64 width = projected_width * inverse_z;
			if (width <= 1) {
				continue;
			}
			if (width > HIRES_SCALE) {
				width = HIRES_SCALE;
			}
			offset_x -= delta_x * fraction;
			offset_y -= delta_y * fraction;
			/* Fractional coverage gives a round stroke that tapers with depth,
			 * without rounding the entire primitive to an integer brush size. */
			if (offset_x * offset_x + offset_y * offset_y < width * width * 0.25) {
				paint_pixel(column, row, inverse_z, paint);
			}
		}
	}
}

static void draw_line(const struct SHAPE3D_HIRES_POINT *first,
					  const struct SHAPE3D_HIRES_POINT *last, legacy_f64 projected_width,
					  const struct HIRES_PAINT *paint)
{
	legacy_f64 padding = (projected_width > 0 ? HIRES_SCALE / 2.0 : 0) + 0.5;
	legacy_f64 delta_x = last->x - first->x;
	legacy_f64 delta_y = last->y - first->y;
	legacy_f64 start = 0;
	legacy_f64 end = 1;
	if (!clip_line_edge(-delta_x, first->x + padding, &start, &end) ||
		!clip_line_edge(delta_x, HIRES_WIDTH - 1 + padding - first->x, &start, &end) ||
		!clip_line_edge(-delta_y, first->y + padding, &start, &end) ||
		!clip_line_edge(delta_y, HIRES_HEIGHT - 1 + padding - first->y, &start, &end)) {
		return;
	}
	legacy_s32 x = (legacy_s32)SDL_floor(first->x + delta_x * start + 0.5);
	legacy_s32 y = (legacy_s32)SDL_floor(first->y + delta_y * start + 0.5);
	legacy_s32 end_x = (legacy_s32)SDL_floor(first->x + delta_x * end + 0.5);
	legacy_s32 end_y = (legacy_s32)SDL_floor(first->y + delta_y * end + 0.5);
	legacy_s32 step_x = x < end_x ? 1 : -1;
	legacy_s32 step_y = y < end_y ? 1 : -1;
	legacy_s32 width = x < end_x ? end_x - x : x - end_x;
	legacy_s32 height = y < end_y ? y - end_y : end_y - y;
	legacy_s32 error = width + height;
	legacy_s32 steps = width > -height ? width : -height;
	legacy_f64 inverse_z = first->inverse_z + (last->inverse_z - first->inverse_z) * start;
	legacy_f64 depth_step =
		steps == 0 ? 0 : (last->inverse_z - first->inverse_z) * (end - start) / steps;
	legacy_f64 length_squared = delta_x * delta_x + delta_y * delta_y;
	legacy_f64 inverse_length_squared = length_squared == 0 ? 0 : 1 / length_squared;
	legacy_f64 nearest_depth =
		first->inverse_z > last->inverse_z ? first->inverse_z : last->inverse_z;
	if (steps == 0) {
		inverse_z = nearest_depth;
	}
	for (;;) {
		/* Retain a continuous one-pixel spine for distant or edge-on details. */
		paint_pixel(x, y, inverse_z, paint);
		if (projected_width * nearest_depth > 1) {
			paint_line_stroke(x, y, first, last, projected_width, inverse_length_squared, paint);
		}
		inverse_z += depth_step;
		if (x == end_x && y == end_y) {
			break;
		}
		legacy_s32 twice_error = error * 2;
		if (twice_error >= height) {
			error += height;
			x += step_x;
		}
		if (twice_error <= width) {
			error += width;
			y += step_y;
		}
	}
}

static void fill_polygon(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
						 const struct HIRES_PAINT *paint)
{
	if (count == 0) {
		return;
	}
	if (count < 3) {
		draw_line(&points[0], &points[count - 1], 0, paint);
		return;
	}
	legacy_f64 minimum_y = points[0].y;
	legacy_f64 maximum_y = minimum_y;
	for (legacy_u32 index = 1; index < count; index++) {
		if (points[index].y < minimum_y) {
			minimum_y = points[index].y;
		}
		if (points[index].y > maximum_y) {
			maximum_y = points[index].y;
		}
	}
	if (maximum_y < 0 || minimum_y >= HIRES_HEIGHT) {
		return;
	}
	legacy_s32 top = minimum_y < 0 ? 0 : ceil_coordinate(minimum_y - 0.5);
	legacy_s32 bottom = maximum_y >= HIRES_HEIGHT ? HIRES_HEIGHT : ceil_coordinate(maximum_y - 0.5);
	if (paint->context != NULL) {
		if (top < paint->context->top) {
			top = paint->context->top;
		}
		if (bottom > paint->context->bottom) {
			bottom = paint->context->bottom;
		}
	}
	for (legacy_s32 y = top; y < bottom; y++) {
		struct SHAPE3D_HIRES_POINT intersections[HIRES_ROUND_POINTS];
		legacy_u32 intersection_count = 0;
		legacy_f64 sample_y = y + 0.5;
		const struct SHAPE3D_HIRES_POINT *previous = &points[count - 1];
		for (legacy_u32 index = 0; index < count; index++) {
			const struct SHAPE3D_HIRES_POINT *current = &points[index];
			if ((previous->y <= sample_y && current->y > sample_y) ||
				(current->y <= sample_y && previous->y > sample_y)) {
				/* Opposite polygon windings must produce bit-identical shared
				 * edges before pixel-center coverage rounds the intersection. */
				const struct SHAPE3D_HIRES_POINT *lower =
					previous->y < current->y ? previous : current;
				const struct SHAPE3D_HIRES_POINT *upper =
					previous->y < current->y ? current : previous;
				legacy_f64 fraction = (sample_y - lower->y) / (upper->y - lower->y);
				struct SHAPE3D_HIRES_POINT intersection;
				intersection.x = lower->x + fraction * (upper->x - lower->x);
				intersection.y = sample_y;
				intersection.inverse_z =
					lower->inverse_z + fraction * (upper->inverse_z - lower->inverse_z);
				legacy_u32 position = intersection_count++;
				while (position != 0 && intersections[position - 1].x > intersection.x) {
					intersections[position] = intersections[position - 1];
					position--;
				}
				intersections[position] = intersection;
			}
			previous = current;
		}
		for (legacy_u32 index = 0; index + 1 < intersection_count; index += 2) {
			const struct SHAPE3D_HIRES_POINT *first = &intersections[index];
			const struct SHAPE3D_HIRES_POINT *last = &intersections[index + 1];
			if (last->x < 0 || first->x >= HIRES_WIDTH || last->x <= first->x) {
				continue;
			}
			legacy_s32 left = first->x < 0 ? 0 : ceil_coordinate(first->x - 0.5);
			legacy_s32 right =
				last->x >= HIRES_WIDTH ? HIRES_WIDTH : ceil_coordinate(last->x - 0.5);
			legacy_f64 depth_step = (last->inverse_z - first->inverse_z) / (last->x - first->x);
			legacy_f64 inverse_z = first->inverse_z + (left + 0.5 - first->x) * depth_step;
			for (legacy_s32 x = left; x < right; x++) {
				paint_pixel(x, y, inverse_z, paint);
				inverse_z += depth_step;
			}
		}
	}
}

static void draw_polygon_border(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
								legacy_u32 edge, legacy_f64 radius, const struct HIRES_PAINT *paint)
{
	const struct SHAPE3D_HIRES_POINT *first = &points[edge];
	const struct SHAPE3D_HIRES_POINT *last = &points[(edge + 1) % count];
	legacy_f64 dx = last->x - first->x;
	legacy_f64 dy = last->y - first->y;
	legacy_f64 minimum_y = (dy < 0 ? last->y : first->y) - radius;
	legacy_f64 maximum_y = (dy < 0 ? first->y : last->y) + radius;
	if (maximum_y < 0 || minimum_y >= HIRES_HEIGHT) {
		return;
	}
	legacy_s32 top = minimum_y < 0 ? 0 : ceil_coordinate(minimum_y - 0.5);
	legacy_s32 bottom = maximum_y >= HIRES_HEIGHT ? HIRES_HEIGHT : ceil_coordinate(maximum_y - 0.5);
	if (paint->context != NULL) {
		if (top < paint->context->top) {
			top = paint->context->top;
		}
		if (bottom > paint->context->bottom) {
			bottom = paint->context->bottom;
		}
	}
	legacy_f64 length_squared = dx * dx + dy * dy;
	for (legacy_s32 y = top; y < bottom; y++) {
		legacy_f64 sample_y = y + 0.5;
		legacy_f64 start = 0;
		legacy_f64 end = 1;
		/* Restrict each row to the part of the edge within one radius. Unlike
		 * stamping a brush along the edge, this visits each candidate only once. */
		if (!clip_line_edge(-dy, first->y - sample_y + radius, &start, &end) ||
			!clip_line_edge(dy, sample_y + radius - first->y, &start, &end)) {
			continue;
		}
		legacy_f64 minimum_x = first->x + dx * (dx < 0 ? end : start) - radius;
		legacy_f64 maximum_x = first->x + dx * (dx < 0 ? start : end) + radius;
		if (maximum_x < 0 || minimum_x >= HIRES_WIDTH) {
			continue;
		}
		legacy_s32 left = minimum_x < 0 ? 0 : ceil_coordinate(minimum_x - 0.5);
		legacy_s32 right =
			maximum_x >= HIRES_WIDTH ? HIRES_WIDTH : ceil_coordinate(maximum_x - 0.5);
		for (legacy_s32 x = left; x < right; x++) {
			legacy_f64 offset_x = x + 0.5 - first->x;
			legacy_f64 offset_y = sample_y - first->y;
			legacy_f64 fraction = length_squared == 0
									  ? (first->inverse_z < last->inverse_z ? 1 : 0)
									  : (offset_x * dx + offset_y * dy) / length_squared;
			if (fraction < 0) {
				fraction = 0;
			} else if (fraction > 1) {
				fraction = 1;
			}
			offset_x -= dx * fraction;
			offset_y -= dy * fraction;
			if (offset_x * offset_x + offset_y * offset_y < radius * radius &&
				!polygon_covers_sample(points, count, x + 0.5, sample_y)) {
				legacy_f64 inverse_z =
					first->inverse_z + (last->inverse_z - first->inverse_z) * fraction;
				paint_pixel(x, y, inverse_z, paint);
			}
		}
	}
}

static void draw_polygon(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
						 legacy_f64 padding, const struct HIRES_PAINT *paint)
{
	fill_polygon(points, count, paint);
	/* Only add coverage outside the original polygon. Interior samples must
	 * keep their exact fill depth, including when another surface occludes them. */
	for (legacy_u32 edge = 0; padding > 0 && edge < count; edge++) {
		draw_polygon_border(points, count, edge, padding, paint);
	}
}

static void build_perimeter(const struct SHAPE3D_HIRES_POINT *center,
							const struct SHAPE3D_HIRES_POINT *first_axis,
							const struct SHAPE3D_HIRES_POINT *second_axis, legacy_f64 scale,
							struct SHAPE3D_HIRES_POINT *points)
{
	for (legacy_u32 index = 0; index < HIRES_ROUND_POINTS; index++) {
		legacy_s16 angle = (legacy_s16)(index * ANGLE_FULL_TURN / HIRES_ROUND_POINTS);
		legacy_f64 cosine = cos_fast(angle) * scale / TRIG_FIXED_ONE;
		legacy_f64 sine = sin_fast(angle) * scale / TRIG_FIXED_ONE;
		points[index].x =
			center->x + (first_axis->x - center->x) * cosine + (second_axis->x - center->x) * sine;
		points[index].y =
			center->y + (first_axis->y - center->y) * cosine + (second_axis->y - center->y) * sine;
		points[index].inverse_z = center->inverse_z +
								  (first_axis->inverse_z - center->inverse_z) * cosine +
								  (second_axis->inverse_z - center->inverse_z) * sine;
	}
}

static void draw_sphere(const struct HIRES_PRIMITIVE *primitive, const struct HIRES_PAINT *paint)
{
	struct SHAPE3D_HIRES_POINT points[HIRES_ROUND_POINTS];
	struct SHAPE3D_HIRES_POINT horizontal = primitive->points[0];
	struct SHAPE3D_HIRES_POINT vertical = horizontal;
	horizontal.x += primitive->size * 0.5;
	vertical.y += primitive->size * (13.0 / 32.0);
	build_perimeter(&primitive->points[0], &horizontal, &vertical, 1, points);
	draw_polygon(points, HIRES_ROUND_POINTS, 0, paint);
}

static void draw_wheel(const struct HIRES_PRIMITIVE *primitive, struct HIRES_PAINT paint,
					   legacy_u16 side_color, legacy_u16 inner_color)
{
	struct SHAPE3D_HIRES_POINT outer[HIRES_ROUND_POINTS];
	struct SHAPE3D_HIRES_POINT inner[HIRES_ROUND_POINTS];
	build_perimeter(&primitive->points[0], &primitive->points[1], &primitive->points[2], 1, outer);
	build_perimeter(&primitive->points[0], &primitive->points[1], &primitive->points[2],
					HIRES_WHEEL_INNER_SCALE, inner);
	legacy_f64 depth_x = primitive->points[3].x - primitive->points[0].x;
	legacy_f64 depth_y = primitive->points[3].y - primitive->points[0].y;
	legacy_f64 depth_z = primitive->points[3].inverse_z - primitive->points[0].inverse_z;
	for (legacy_u32 index = 0; index < HIRES_ROUND_POINTS; index++) {
		legacy_u32 next = (index + 1) % HIRES_ROUND_POINTS;
		struct SHAPE3D_HIRES_POINT side[4] = {outer[index], outer[next], outer[next], outer[index]};
		side[2].x += depth_x;
		side[2].y += depth_y;
		side[2].inverse_z += depth_z;
		side[3].x += depth_x;
		side[3].y += depth_y;
		side[3].inverse_z += depth_z;
		draw_polygon(side, 4, 0, &paint);
	}
	paint.color = side_color;
	for (legacy_u32 index = 0; index < HIRES_ROUND_POINTS; index++) {
		legacy_u32 next = (index + 1) % HIRES_ROUND_POINTS;
		struct SHAPE3D_HIRES_POINT rim[4] = {outer[index], outer[next], inner[next], inner[index]};
		draw_polygon(rim, 4, 0, &paint);
	}
	paint.color = inner_color;
	draw_polygon(inner, HIRES_ROUND_POINTS, 0, &paint);
}

static void render_primitive(legacy_u32 index, legacy_u8 type, legacy_u16 color,
							 legacy_u16 second_color, legacy_u16 third_color,
							 legacy_u16 pattern_type, legacy_u16 pattern,
							 struct HIRES_RASTER_CONTEXT *context)
{
	if (index >= primitive_capacity || primitives[index].count == 0) {
		return;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	const struct HIRES_SHAPE *shape = &shapes[primitive->shape];
	if (context == NULL && (!rendered_depth_valid || rendered_generation != hires_generation())) {
		hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
		rendered_depth_valid = 1;
		rendered_generation = hires_generation();
	}
	legacy_s32 ordered = shape->depth_mode == SHAPE3D_HIRES_DEPTH_ORDERED;
	struct HIRES_PAINT paint = {context,
								color,
								second_color,
								pattern,
								pattern_type,
								shape->depth_mode != SHAPE3D_HIRES_DEPTH_BACKGROUND,
								ordered ? primitive->shape + 1 : primitive->family,
								ordered && !primitive->attached ? HIRES_DEPTH_ORDERED
																: primitive->attached};
	if (pattern_type == 2) {
		/* The legacy two-color helper receives the secondary material first;
		 * set pattern bits still select the primary material color. */
		paint.color = second_color;
		paint.alternate = color;
	}
	if ((type & RENDER_PRIMITIVE_GHOST_FLAG) != 0) {
		type &= ~RENDER_PRIMITIVE_GHOST_FLAG;
		paint.color = 0;
		paint.pattern = PRERENDER_BLACK_GRILLE_PATTERN;
		paint.mode = 1;
		second_color = 0;
		third_color = 0;
	}
	if (type == RENDER_PRIMITIVE_POLYGON) {
		draw_polygon(primitive->points, primitive->count, primitive->size, &paint);
	} else if (type == RENDER_PRIMITIVE_LINE) {
		draw_line(&primitive->points[0], &primitive->points[1], primitive->size, &paint);
	} else if (type == RENDER_PRIMITIVE_POINT) {
		draw_line(&primitive->points[0], &primitive->points[0], 0, &paint);
	} else if (type == RENDER_PRIMITIVE_SPHERE) {
		draw_sphere(primitive, &paint);
	} else if (type == RENDER_PRIMITIVE_WHEEL) {
		draw_wheel(primitive, paint, second_color, third_color);
	}
}

void shape3d_hires_batch_begin(void)
{
	command_count = 0;
	command_area = 0;
#if !defined(__DJGPP__)
	batching = 1;
#endif
}

void shape3d_hires_render(legacy_u32 index, legacy_u8 type, legacy_u16 color,
						  legacy_u16 second_color, legacy_u16 third_color, legacy_u16 pattern_type,
						  legacy_u16 pattern)
{
	if (!batching) {
		render_primitive(index, type, color, second_color, third_color, pattern_type, pattern,
						 NULL);
		return;
	}
	if (index >= primitive_capacity || primitives[index].count == 0) {
		return;
	}
	struct RECTANGLE bounds = {HIRES_WIDTH / HIRES_SCALE, 0, HIRES_HEIGHT / HIRES_SCALE, 0};
	legacy_u8 bounds_type = type & ~RENDER_PRIMITIVE_GHOST_FLAG;
	/* Points and degenerate polygons use rounded line endpoints. Their
	 * fractional centers can lie just outside the screen and still hit it. */
	if (bounds_type == RENDER_PRIMITIVE_POINT ||
		(bounds_type == RENDER_PRIMITIVE_POLYGON && primitives[index].count < 3)) {
		bounds_type = RENDER_PRIMITIVE_LINE;
	}
	shape3d_hires_update_bounds(index, bounds_type, &bounds);
	if (bounds.left >= bounds.right || bounds.top >= bounds.bottom) {
		return;
	}
	if (command_count == command_capacity) {
		size_t capacity =
			command_capacity != 0 ? command_capacity * 2 : HIRES_INITIAL_COMMAND_CAPACITY;
		if (capacity < command_capacity || capacity > (size_t)-1 / sizeof(*commands)) {
			fatal_error("SuperSight drawing commands exceed addressable memory");
			return;
		}
		struct HIRES_COMMAND *buffer = realloc(commands, capacity * sizeof(*commands));
		if (buffer == NULL) {
			fatal_error("Cannot allocate SuperSight drawing commands");
			return;
		}
		commands = buffer;
		command_capacity = capacity;
	}
	commands[command_count++] = (struct HIRES_COMMAND){index,
													   type,
													   color,
													   second_color,
													   third_color,
													   pattern_type,
													   pattern,
													   bounds.top * HIRES_SCALE,
													   bounds.bottom * HIRES_SCALE};
	if (command_area < HIRES_PARALLEL_MIN_AREA) {
		command_area += (legacy_u32)(bounds.right - bounds.left) * (bounds.bottom - bounds.top) *
						HIRES_SCALE * HIRES_SCALE;
	}
}

struct HIRES_BATCH {
	struct HIRES_RASTER_TARGET target;
	struct HIRES_RASTER_CONTEXT bands[HIRES_BAND_COUNT];
};

static void render_band(void *argument, legacy_s32 index)
{
	struct HIRES_BATCH *batch = argument;
	struct HIRES_RASTER_CONTEXT *context = &batch->bands[index];
	if (context->top >= context->bottom) {
		return;
	}
	for (size_t command = 0; command < command_count; command++) {
		const struct HIRES_COMMAND *entry = &commands[command];
		if (entry->top >= context->bottom || entry->bottom <= context->top) {
			continue;
		}
		render_primitive(entry->index, entry->type, entry->color, entry->second_color,
						 entry->third_color, entry->pattern_type, entry->pattern, context);
	}
}

legacy_s32 shape3d_hires_batch_end(void)
{
	batching = 0;
	struct HIRES_BATCH batch;
	if (command_area >= HIRES_PARALLEL_MIN_AREA && render_workers_count() != 0) {
		/* Allocate and clear depth on the caller before any workers can read it. */
		if (!rendered_depth_valid || rendered_generation != hires_generation()) {
			hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
			rendered_depth_valid = 1;
			rendered_generation = hires_generation();
		}
		if (hires_raster_prepare(&batch.target)) {
			for (legacy_s32 index = 0; index < HIRES_BAND_COUNT; index++) {
				batch.bands[index] = (struct HIRES_RASTER_CONTEXT){
					&batch.target, index * HIRES_BAND_HEIGHT, (index + 1) * HIRES_BAND_HEIGHT, 0};
				if (batch.bands[index].top < batch.target.top) {
					batch.bands[index].top = batch.target.top;
				}
				if (batch.bands[index].bottom > batch.target.bottom) {
					batch.bands[index].bottom = batch.target.bottom;
				}
			}
			legacy_s32 workers = render_workers_run(HIRES_BAND_COUNT, render_band, &batch);
			legacy_u32 cleared = 0;
			for (legacy_s32 index = 0; index < HIRES_BAND_COUNT; index++) {
				cleared += batch.bands[index].cleared_argb_cells;
			}
			hires_raster_finish(&batch.target, cleared);
			command_count = 0;
			command_area = 0;
			return workers;
		}
	}
	for (size_t index = 0; index < command_count; index++) {
		const struct HIRES_COMMAND *entry = &commands[index];
		render_primitive(entry->index, entry->type, entry->color, entry->second_color,
						 entry->third_color, entry->pattern_type, entry->pattern, NULL);
	}
	command_count = 0;
	command_area = 0;
	return 0;
}

#define SHADOW_SILHOUETTE_WIDTH 128
#define SHADOW_SILHOUETTE_LENGTH 256
#define SHADOW_SILHOUETTE_ROUND_POINTS 12
#define SHADOW_SILHOUETTE_LINE_RADIUS 0.75
#define SHADOW_SILHOUETTE_BORDER_TEXELS 2
#define SHADOW_SILHOUETTE_INITIAL_BOUND (LEGACY_U16_MAX + 1.0)

struct SHADOW_SILHOUETTE {
	const struct SHAPE3D *shape;
	const legacy_u8 *vertex_bytes;
	legacy_f64 min_x, min_z, max_x, max_z, height, scale_x, scale_z;
	legacy_u8 pixels[SHADOW_SILHOUETTE_WIDTH * SHADOW_SILHOUETTE_LENGTH];
};

struct SHADOW_SILHOUETTE_POINT {
	legacy_f64 x, z;
};

/* Build a tiny top-down union of the authored primitives once per loaded car.
 * Keeping separate polygons preserves the gaps beside open-wheel suspension. */
static void shadow_silhouette_polygon(struct SHADOW_SILHOUETTE *silhouette,
									  const struct SHADOW_SILHOUETTE_POINT *points,
									  legacy_s32 count, legacy_s32 rasterize)
{
	if (!rasterize) {
		for (legacy_s32 index = 0; index < count; index++) {
			if (points[index].x < silhouette->min_x) {
				silhouette->min_x = points[index].x;
			}
			if (points[index].x > silhouette->max_x) {
				silhouette->max_x = points[index].x;
			}
			if (points[index].z < silhouette->min_z) {
				silhouette->min_z = points[index].z;
			}
			if (points[index].z > silhouette->max_z) {
				silhouette->max_z = points[index].z;
			}
		}
		return;
	}
	for (legacy_s32 row = 1; row < SHADOW_SILHOUETTE_LENGTH - 1; row++) {
		legacy_f64 z = silhouette->min_z + (row + 0.5) / silhouette->scale_z;
		legacy_f64 crossings[SHADOW_SILHOUETTE_ROUND_POINTS];
		legacy_s32 crossing_count = 0;
		for (legacy_s32 index = 0; index < count; index++) {
			const struct SHADOW_SILHOUETTE_POINT *first = &points[index];
			const struct SHADOW_SILHOUETTE_POINT *last = &points[(index + 1) % count];
			if ((first->z > z) != (last->z > z)) {
				legacy_f64 x =
					first->x + (last->x - first->x) * (z - first->z) / (last->z - first->z);
				legacy_s32 insertion = crossing_count++;
				while (insertion > 0 && crossings[insertion - 1] > x) {
					crossings[insertion] = crossings[insertion - 1];
					insertion--;
				}
				crossings[insertion] = x;
			}
		}
		for (legacy_s32 index = 0; index + 1 < crossing_count; index += 2) {
			legacy_s32 left = (legacy_s32)SDL_ceil(
				(crossings[index] - silhouette->min_x) * silhouette->scale_x - 0.5);
			legacy_s32 right = (legacy_s32)SDL_ceil(
				(crossings[index + 1] - silhouette->min_x) * silhouette->scale_x - 0.5);
			if (left < 1) {
				left = 1;
			}
			if (right > SHADOW_SILHOUETTE_WIDTH - 1) {
				right = SHADOW_SILHOUETTE_WIDTH - 1;
			}
			if (left < right) {
				memset(&silhouette->pixels[row * SHADOW_SILHOUETTE_WIDTH + left], LEGACY_U8_MAX,
					   (size_t)(right - left));
			}
		}
	}
}

static void shadow_silhouette_line(struct SHADOW_SILHOUETTE *silhouette, const struct VECTOR *first,
								   const struct VECTOR *last, legacy_s32 rasterize)
{
	legacy_f64 dx = last->x - first->x;
	legacy_f64 dz = last->z - first->z;
	legacy_f64 length = SDL_sqrt(dx * dx + dz * dz);
	if (length == 0) {
		return;
	}
	legacy_f64 radius = SHADOW_SILHOUETTE_LINE_RADIUS;
	if (rasterize) {
		legacy_f64 texel = 0.5 / silhouette->scale_x + 0.5 / silhouette->scale_z;
		if (radius < texel) {
			radius = texel;
		}
	}
	legacy_f64 x = dz * radius / length;
	legacy_f64 z = -dx * radius / length;
	struct SHADOW_SILHOUETTE_POINT points[4] = {{first->x - x, first->z - z},
												{last->x - x, last->z - z},
												{last->x + x, last->z + z},
												{first->x + x, first->z + z}};
	shadow_silhouette_polygon(silhouette, points, 4, rasterize);
}

static void shadow_silhouette_primitives(struct SHADOW_SILHOUETTE *silhouette,
										 const struct SHAPE3D *shape, legacy_s32 rasterize)
{
	static const legacy_u8 vertex_counts[SHAPE3D_PRIMITIVE_TYPE_COUNT] = {0, 1, 2,	3, 4, 5, 6, 7,
																		  8, 9, 10, 2, 6, 3, 0, 0};
	const legacy_u8 *primitive = shape->shape3d_primitives;
	for (legacy_u16 index = 0; index < shape->shape3d_numprimitives; index++) {
		legacy_u8 type = primitive[0];
		if (type >= SHAPE3D_PRIMITIVE_TYPE_COUNT) {
			return;
		}
		legacy_u8 count = vertex_counts[type];
		const legacy_u8 *indices =
			primitive + SHAPE3D_PRIMITIVE_HEADER_SIZE + shape->shape3d_numpaints;
		primitive = indices + count;
		struct VECTOR vertices[SHAPE3D_POLYGON_MAX_VERTICES];
		legacy_s32 valid = 1;
		for (legacy_u8 vertex = 0; vertex < count; vertex++) {
			if (indices[vertex] >= shape->shape3d_numverts) {
				valid = 0;
				break;
			}
			shape3d_vertex_read(shape, indices[vertex], &vertices[vertex]);
			if (!rasterize && vertices[vertex].y > silhouette->height) {
				silhouette->height = vertices[vertex].y;
			}
		}
		if (!valid) {
			continue;
		}
		struct SHADOW_SILHOUETTE_POINT points[SHADOW_SILHOUETTE_ROUND_POINTS];
		if (type >= SHAPE3D_PRIMITIVE_POLYGON_FIRST && type <= SHAPE3D_PRIMITIVE_POLYGON_LAST) {
			for (legacy_u8 vertex = 0; vertex < count; vertex++) {
				points[vertex].x = vertices[vertex].x;
				points[vertex].z = vertices[vertex].z;
			}
			shadow_silhouette_polygon(silhouette, points, count, rasterize);
		} else if (type == SHAPE3D_PRIMITIVE_LINE) {
			shadow_silhouette_line(silhouette, &vertices[0], &vertices[1], rasterize);
		} else if (type == SHAPE3D_PRIMITIVE_WHEEL) {
			/* A wheel is an ellipse extruded from vertex 0 to vertex 3.
			 * Its two authored radial axes also support steered/custom wheels. */
			struct SHADOW_SILHOUETTE_POINT other[SHADOW_SILHOUETTE_ROUND_POINTS];
			for (legacy_s32 vertex = 0; vertex < SHADOW_SILHOUETTE_ROUND_POINTS; vertex++) {
				legacy_s16 angle =
					(legacy_s16)(vertex * ANGLE_FULL_TURN / SHADOW_SILHOUETTE_ROUND_POINTS);
				legacy_f64 cosine = cos_fast(angle) / (legacy_f64)TRIG_FIXED_ONE;
				legacy_f64 sine = sin_fast(angle) / (legacy_f64)TRIG_FIXED_ONE;
				points[vertex].x = vertices[0].x +
								   (vertices[SHAPE3D_WHEEL_FIRST_AXIS].x - vertices[0].x) * cosine +
								   (vertices[SHAPE3D_WHEEL_SECOND_AXIS].x - vertices[0].x) * sine;
				points[vertex].z = vertices[0].z +
								   (vertices[SHAPE3D_WHEEL_FIRST_AXIS].z - vertices[0].z) * cosine +
								   (vertices[SHAPE3D_WHEEL_SECOND_AXIS].z - vertices[0].z) * sine;
				other[vertex].x =
					points[vertex].x + vertices[SHAPE3D_WHEEL_RIM_VERTEX_COUNT].x - vertices[0].x;
				other[vertex].z =
					points[vertex].z + vertices[SHAPE3D_WHEEL_RIM_VERTEX_COUNT].z - vertices[0].z;
			}
			shadow_silhouette_polygon(silhouette, points, SHADOW_SILHOUETTE_ROUND_POINTS,
									  rasterize);
			shadow_silhouette_polygon(silhouette, other, SHADOW_SILHOUETTE_ROUND_POINTS, rasterize);
			for (legacy_s32 vertex = 0; vertex < SHADOW_SILHOUETTE_ROUND_POINTS; vertex++) {
				legacy_s32 next = (vertex + 1) % SHADOW_SILHOUETTE_ROUND_POINTS;
				struct SHADOW_SILHOUETTE_POINT side[4] = {points[vertex], points[next], other[next],
														  other[vertex]};
				shadow_silhouette_polygon(silhouette, side, 4, rasterize);
			}
		} else if (type == SHAPE3D_PRIMITIVE_SPHERE) {
			legacy_f64 dx = vertices[1].x - vertices[0].x;
			legacy_f64 dy = vertices[1].y - vertices[0].y;
			legacy_f64 dz = vertices[1].z - vertices[0].z;
			legacy_f64 radius = SDL_sqrt(dx * dx + dy * dy + dz * dz);
			if (!rasterize && vertices[0].y + radius > silhouette->height) {
				silhouette->height = vertices[0].y + radius;
			}
			for (legacy_s32 vertex = 0; vertex < SHADOW_SILHOUETTE_ROUND_POINTS; vertex++) {
				legacy_s16 angle =
					(legacy_s16)(vertex * ANGLE_FULL_TURN / SHADOW_SILHOUETTE_ROUND_POINTS);
				points[vertex].x = vertices[0].x + radius * cos_fast(angle) / TRIG_FIXED_ONE;
				points[vertex].z = vertices[0].z + radius * sin_fast(angle) / TRIG_FIXED_ONE;
			}
			shadow_silhouette_polygon(silhouette, points, SHADOW_SILHOUETTE_ROUND_POINTS,
									  rasterize);
		}
	}
}

static legacy_s32 shadow_silhouette_build(struct SHADOW_SILHOUETTE *silhouette,
										  const struct SHAPE3D *shape)
{
	memset(silhouette, 0, sizeof(*silhouette));
	if (shape == NULL || shape->shape3d_numverts == 0 || shape->shape3d_numprimitives == 0) {
		return 0;
	}
	silhouette->min_x = silhouette->min_z = SHADOW_SILHOUETTE_INITIAL_BOUND;
	silhouette->max_x = silhouette->max_z = -SHADOW_SILHOUETTE_INITIAL_BOUND;
	shadow_silhouette_primitives(silhouette, shape, 0);
	legacy_f64 width = silhouette->max_x - silhouette->min_x;
	legacy_f64 length = silhouette->max_z - silhouette->min_z;
	if (width <= 0 || length <= 0) {
		return 0;
	}
	/* Leave clear texels around the model so bilinear filtering softens only
	 * the silhouette edge and cannot wrap onto the opposite side. */
	silhouette->scale_x = (SHADOW_SILHOUETTE_WIDTH - 2 * SHADOW_SILHOUETTE_BORDER_TEXELS) / width;
	silhouette->scale_z = (SHADOW_SILHOUETTE_LENGTH - 2 * SHADOW_SILHOUETTE_BORDER_TEXELS) / length;
	silhouette->min_x -= SHADOW_SILHOUETTE_BORDER_TEXELS / silhouette->scale_x;
	silhouette->max_x += SHADOW_SILHOUETTE_BORDER_TEXELS / silhouette->scale_x;
	silhouette->min_z -= SHADOW_SILHOUETTE_BORDER_TEXELS / silhouette->scale_z;
	silhouette->max_z += SHADOW_SILHOUETTE_BORDER_TEXELS / silhouette->scale_z;
	shadow_silhouette_primitives(silhouette, shape, 1);
	silhouette->shape = shape;
	silhouette->vertex_bytes = shape->shape3d_vertex_bytes;
	return 1;
}

static legacy_f64 shadow_silhouette_sample(const struct SHADOW_SILHOUETTE *silhouette, legacy_f64 x,
										   legacy_f64 z)
{
	x = (x - silhouette->min_x) * silhouette->scale_x - 0.5;
	z = (z - silhouette->min_z) * silhouette->scale_z - 0.5;
	if (x < 0 || z < 0 || x >= SHADOW_SILHOUETTE_WIDTH - 1 || z >= SHADOW_SILHOUETTE_LENGTH - 1) {
		return 0;
	}
	legacy_s32 column = (legacy_s32)x;
	legacy_s32 row = (legacy_s32)z;
	x -= column;
	z -= row;
	const legacy_u8 *pixels = &silhouette->pixels[row * SHADOW_SILHOUETTE_WIDTH + column];
	legacy_f64 first = pixels[0] + (pixels[1] - pixels[0]) * x;
	legacy_f64 last = pixels[SHADOW_SILHOUETTE_WIDTH] +
					  (pixels[SHADOW_SILHOUETTE_WIDTH + 1] - pixels[SHADOW_SILHOUETTE_WIDTH]) * x;
	return (first + (last - first) * z) / LEGACY_U8_MAX;
}

static struct SHADOW_SILHOUETTE shadow_silhouettes[HIRES_CAR_SHADOW_COUNT];

void shape3d_hires_shadows_begin(const struct VECTOR *camera_position)
{
	car_shadow_count = 0;
	shadow_model_pending = 0;
	shadow_ground_y = -camera_position->y;
	shadow_view = mat_temp;
	/* Invert the fixed-point view exactly: transposing its rounded rotation
	 * can move a small distant footprint away from its supporting surface. */
	legacy_f64 a = shadow_view.m._11, b = shadow_view.m._12, c = shadow_view.m._13;
	legacy_f64 d = shadow_view.m._21, e = shadow_view.m._22, f = shadow_view.m._23;
	legacy_f64 g = shadow_view.m._31, h = shadow_view.m._32, i = shadow_view.m._33;
	legacy_f64 determinant = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
	legacy_f64 scale = determinant != 0 ? TRIG_FIXED_ONE / determinant : 0;
	shadow_inverse[0][0] = (e * i - f * h) * scale;
	shadow_inverse[0][1] = (c * h - b * i) * scale;
	shadow_inverse[0][2] = (b * f - c * e) * scale;
	shadow_inverse[1][0] = (f * g - d * i) * scale;
	shadow_inverse[1][1] = (a * i - c * g) * scale;
	shadow_inverse[1][2] = (c * d - a * f) * scale;
	shadow_inverse[2][0] = (d * h - e * g) * scale;
	shadow_inverse[2][1] = (b * g - a * h) * scale;
	shadow_inverse[2][2] = (a * e - b * d) * scale;
}

void shape3d_hires_shadow_car(const struct VECTOR *relative_position, legacy_s16 heading,
							  legacy_s16 half_width, legacy_s16 half_length)
{
	shadow_model_pending = 0;
	if (!hires_enabled() || car_shadow_count == HIRES_CAR_SHADOW_COUNT || half_width <= 0 ||
		half_length <= 0) {
		return;
	}
	struct HIRES_CAR_SHADOW *shadow = &car_shadows[car_shadow_count++];
	shadow->position = *relative_position;
	shadow->cosine = cos_fast((legacy_u16)heading) / (legacy_f64)TRIG_FIXED_ONE;
	shadow->sine = sin_fast((legacy_u16)heading) / (legacy_f64)TRIG_FIXED_ONE;
	shadow->half_width = half_width;
	shadow->half_length = half_length;
	shadow->min_x = -half_width;
	shadow->max_x = half_width;
	shadow->min_z = -half_length;
	shadow->max_z = half_length;
	shadow->height = SDL_min(half_width, half_length) * HIRES_CAR_SHADOW_FALLBACK_HEIGHT_SCALE;
	shadow->silhouette = NULL;
	shadow_model_pending = 1;
}

void shape3d_hires_shadow_models_reset(void)
{
	car_shadow_count = 0;
	shadow_model_pending = 0;
	for (legacy_s32 index = 0; index < HIRES_CAR_SHADOW_COUNT; index++) {
		shadow_silhouettes[index].shape = NULL;
	}
}

void shape3d_hires_shadow_model(const struct SHAPE3D *shape)
{
	if (!shadow_model_pending || shape == NULL) {
		return;
	}
	shadow_model_pending = 0;
	struct HIRES_CAR_SHADOW *shadow = &car_shadows[car_shadow_count - 1];
	struct SHADOW_SILHOUETTE *silhouette = &shadow_silhouettes[car_shadow_count - 1];
	if (silhouette->shape != shape || silhouette->vertex_bytes != shape->shape3d_vertex_bytes) {
		if (!shadow_silhouette_build(silhouette, shape)) {
			return;
		}
		silhouette->shape = shape;
		silhouette->vertex_bytes = shape->shape3d_vertex_bytes;
	}
	shadow->silhouette = silhouette;
	shadow->min_x = silhouette->min_x;
	shadow->max_x = silhouette->max_x;
	shadow->min_z = silhouette->min_z;
	shadow->max_z = silhouette->max_z;
	shadow->height = silhouette->height;
	shadow->half_width = (silhouette->max_x - silhouette->min_x) * 0.5;
	shadow->half_length = (silhouette->max_z - silhouette->min_z) * 0.5;
}

static legacy_f64 shadow_north_offset(const struct HIRES_CAR_SHADOW *shadow, legacy_f64 gap)
{
	/* North is +world Z. A 70-degree southern sun leaves a visible, short
	 * lean even on the ground; limit travel as the car rises into the air. */
	legacy_f64 offset =
		(gap + shadow->height * HIRES_CAR_SHADOW_HEIGHT_SCALE) * HIRES_CAR_SHADOW_SUN_COTANGENT;
	legacy_f64 limit =
		SDL_min(shadow->half_width, shadow->half_length) * HIRES_CAR_SHADOW_OFFSET_LIMIT_SCALE;
	return offset < limit ? offset : limit;
}

static struct RECTANGLE shadow_bounds(const struct HIRES_CAR_SHADOW *shadow)
{
	struct RECTANGLE bounds = {HIRES_WIDTH, 0, HIRES_HEIGHT, 0};
	/* The flat ground bounds the bottom of the small receiving volume. */
	legacy_f64 bottom = shadow->position.y - HIRES_CAR_SHADOW_REACH;
	if (bottom < shadow_ground_y) {
		bottom = shadow_ground_y;
	}
	legacy_f64 extension = shadow_north_offset(shadow, HIRES_CAR_SHADOW_REACH);
	legacy_f64 extend_x = -extension * shadow->sine;
	legacy_f64 extend_z = extension * shadow->cosine;
	struct SHAPE3D_HIRES_VECTOR vertices[HIRES_CAR_SHADOW_BOUNDS_CORNERS];
	for (legacy_s32 corner = 0; corner < HIRES_CAR_SHADOW_BOUNDS_CORNERS; corner++) {
		legacy_f64 x = (corner & HIRES_CAR_SHADOW_CORNER_MAX_X)
						   ? shadow->max_x + SDL_max(0, extend_x)
						   : shadow->min_x + SDL_min(0, extend_x);
		legacy_f64 z = (corner & HIRES_CAR_SHADOW_CORNER_MAX_Z)
						   ? shadow->max_z + SDL_max(0, extend_z)
						   : shadow->min_z + SDL_min(0, extend_z);
		legacy_f64 world_x = shadow->position.x + x * shadow->cosine + z * shadow->sine;
		legacy_f64 world_y =
			(corner & HIRES_CAR_SHADOW_CORNER_BOTTOM) ? bottom : shadow->position.y;
		legacy_f64 world_z = shadow->position.z - x * shadow->sine + z * shadow->cosine;
		struct SHAPE3D_HIRES_VECTOR view = {
			(world_x * shadow_view.m._11 + world_y * shadow_view.m._12 +
			 world_z * shadow_view.m._13) /
				TRIG_FIXED_ONE,
			(world_x * shadow_view.m._21 + world_y * shadow_view.m._22 +
			 world_z * shadow_view.m._23) /
				TRIG_FIXED_ONE,
			(world_x * shadow_view.m._31 + world_y * shadow_view.m._32 +
			 world_z * shadow_view.m._33) /
				TRIG_FIXED_ONE};
		vertices[corner] = view;
	}
	/* Clip the twelve prism edges, including near-camera cars, rather than
	 * turning a single behind-camera corner into a full-screen scan. */
	struct SHAPE3D_HIRES_POINT
		points[HIRES_CAR_SHADOW_BOUNDS_CORNERS + HIRES_CAR_SHADOW_BOUNDS_EDGES];
	legacy_s32 count = 0;
	for (legacy_s32 corner = 0; corner < HIRES_CAR_SHADOW_BOUNDS_CORNERS; corner++) {
		if (vertices[corner].z >= HIRES_NEAR_CLIP_Z) {
			shape3d_hires_project(&vertices[corner], &points[count++]);
		}
		for (legacy_s32 edge = HIRES_CAR_SHADOW_CORNER_MAX_X;
			 edge <= HIRES_CAR_SHADOW_CORNER_BOTTOM; edge *= 2) {
			if ((corner & edge) == 0 && ((vertices[corner].z < HIRES_NEAR_CLIP_Z) !=
										 (vertices[corner | edge].z < HIRES_NEAR_CLIP_Z))) {
				project_intersection(&vertices[corner], &vertices[corner | edge], &points[count++]);
			}
		}
	}
	for (legacy_s32 index = 0; index < count; index++) {
		struct SHAPE3D_HIRES_POINT point = points[index];
		legacy_s16 px = (legacy_s16)(point.x < 0			 ? 0
									 : point.x > HIRES_WIDTH ? HIRES_WIDTH
															 : point.x);
		legacy_s16 py = (legacy_s16)(point.y < 0			  ? 0
									 : point.y > HIRES_HEIGHT ? HIRES_HEIGHT
															  : point.y);
		if (px < bounds.left) {
			bounds.left = px;
		}
		if (px + 1 > bounds.right) {
			bounds.right = px + 1;
		}
		if (py < bounds.top) {
			bounds.top = py;
		}
		if (py + 1 > bounds.bottom) {
			bounds.bottom = py + 1;
		}
	}
	return bounds;
}

static legacy_u8 shadow_opacity(const struct HIRES_CAR_SHADOW *shadow, legacy_f64 x, legacy_f64 y,
								legacy_f64 z)
{
	legacy_f64 gap = shadow->position.y - y;
	if (gap < -HIRES_CAR_SHADOW_RECEIVER_TOLERANCE || gap >= HIRES_CAR_SHADOW_REACH) {
		return 0;
	}
	if (gap < 0) {
		gap = 0;
	}
	x -= shadow->position.x;
	z -= shadow->position.z + shadow_north_offset(shadow, gap);
	legacy_f64 shrink = 1 + gap / HIRES_CAR_SHADOW_SHRINK_HEIGHT;
	legacy_f64 u = (x * shadow->cosine - z * shadow->sine) * shrink;
	legacy_f64 v = (x * shadow->sine + z * shadow->cosine) * shrink;
	legacy_f64 coverage;
	if (shadow->silhouette != NULL) {
		coverage = shadow_silhouette_sample(shadow->silhouette, u, v);
	} else {
		/* Retain a modest footprint if a custom model has no usable geometry. */
		u = absolute_coordinate(u / shadow->half_width);
		v = absolute_coordinate(v / shadow->half_length);
		legacy_f64 contour = SDL_max(SDL_max(u, v), (u + v) / HIRES_CAR_SHADOW_FALLBACK_DIAGONAL);
		coverage = SDL_min(1, (1 - contour) * HIRES_CAR_SHADOW_FALLBACK_EDGE_SCALE);
	}
	if (coverage <= 0) {
		return 0;
	}
	/* Fade the last part of the short reach so a high jump has no cutoff. */
	legacy_f64 fade = (HIRES_CAR_SHADOW_REACH - gap) / HIRES_CAR_SHADOW_FADE_HEIGHT;
	if (fade > 1) {
		fade = 1;
	}
	return (legacy_u8)(HIRES_CAR_SHADOW_MAX_OPACITY * coverage * fade);
}

void shape3d_hires_draw_shadows(void)
{
	if (!hires_enabled() || car_shadow_count == 0 || projection_focal_length_x == 0 ||
		projection_focal_length_y == 0) {
		return;
	}
	struct HIRES_RASTER_TARGET target;
	if (!hires_raster_prepare(&target)) {
		return;
	}
	legacy_f64 focal_x = projection_focal_length_x * HIRES_SCALE;
	legacy_f64 focal_y = projection_focal_length_y * HIRES_SCALE;
	legacy_f64 ray_step[3];
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		ray_step[axis] = shadow_inverse[axis][0] / focal_x;
	}
	legacy_s32 started = 0;
	for (legacy_s32 car = 0; car < car_shadow_count; car++) {
		const struct HIRES_CAR_SHADOW *shadow = &car_shadows[car];
		struct RECTANGLE bounds = shadow_bounds(shadow);
		if (bounds.left < target.left) {
			bounds.left = (legacy_s16)target.left;
		}
		if (bounds.right > target.right) {
			bounds.right = (legacy_s16)target.right;
		}
		if (bounds.top < target.top) {
			bounds.top = (legacy_s16)target.top;
		}
		if (bounds.bottom > target.bottom) {
			bounds.bottom = (legacy_s16)target.bottom;
		}
		for (legacy_s32 y = bounds.top; y < bounds.bottom; y++) {
			legacy_f64 view_x =
				(bounds.left + 0.5 - (legacy_s16)projection_center_x * HIRES_SCALE) / focal_x;
			legacy_f64 view_y = ((legacy_s16)projection_center_y * HIRES_SCALE - y - 0.5) / focal_y;
			legacy_f64 ray[3];
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				ray[axis] = shadow_inverse[axis][0] * view_x + shadow_inverse[axis][1] * view_y +
							shadow_inverse[axis][2];
			}
			for (legacy_s32 x = bounds.left; x < bounds.right; x++) {
				legacy_u32 family = 0;
				size_t pixel = (size_t)y * HIRES_WIDTH + x;
				if (x >= target.depth_left && x < target.depth_right && y >= target.depth_top &&
					y < target.depth_bottom) {
					family = target.depth_family[pixel];
				}
				legacy_f64 depth = 0;
				if (family != 0) {
					if (family <= primitive_count &&
						shapes[primitives[family - 1].shape].shadow_receiver) {
						depth = 1.0 / target.inverse_depth[pixel];
					}
				} else if (ray[1] < 0 && shadow_ground_y < 0) {
					/* Flat grass is the skybox's ground fill, not queued geometry. */
					depth = shadow_ground_y / ray[1];
				}
				if (depth >= HIRES_NEAR_CLIP_Z) {
					legacy_u8 opacity =
						shadow_opacity(shadow, ray[0] * depth, ray[1] * depth, ray[2] * depth);
					if (opacity != 0) {
						if (!started) {
							if (!hires_shadow_begin()) {
								return;
							}
							started = 1;
						}
						hires_shadow_pixel(x, y, opacity);
					}
				}
				for (legacy_s32 axis = 0; axis < 3; axis++) {
					ray[axis] += ray_step[axis];
				}
			}
		}
	}
}

#endif
