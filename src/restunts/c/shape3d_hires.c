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
#include "shape3d_shadows.h"

#define HIRES_NEAR_CLIP_Z 12
#define HIRES_BAND_HEIGHT 32
#define HIRES_BAND_COUNT (HIRES_HEIGHT / HIRES_BAND_HEIGHT)
#define HIRES_PARALLEL_MIN_AREA 65536U
/* Keep the original displayed pixel width through medium-close views,
 * then let perspective narrow the stroke at greater distances. */
#define HIRES_LINE_DIAMETER 1.5
/* A subpixel decal still needs coverage across diagonal sample gaps. */
#define HIRES_MIN_DECAL_WIDTH 1.5
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
};

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
		if (capacity > 0x3FFFFFFFUL) {
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
		flags |= 1;
	} else if (point->y >= (select_rect_rc.bottom + 1) * HIRES_SCALE + padding) {
		flags |= 2;
	}
	if (point->x < select_rect_rc.left * HIRES_SCALE - padding) {
		flags |= 4;
	} else if (point->x >= (select_rect_rc.right + 1) * HIRES_SCALE + padding) {
		flags |= 8;
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
	width -= 2 * reduction;
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
	legacy_u8 flags = 15;
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
}

void shape3d_hires_set_model_scale(legacy_f64 scale)
{
	model_scale = scale;
}

void shape3d_hires_reset(void)
{
	/* A scene reset only clears transient casters. Track lightmaps survive
	 * camera changes, menus and the F12 toggle until track resources unload. */
	shape3d_shadows_reset();
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
		struct SHAPE3D_HIRES_POINT face[3];
		for (legacy_u32 vertex = 0; vertex < 3; vertex++) {
			shape3d_hires_project(&vertices[indices[vertex]], &face[vertex]);
		}
		legacy_u32 start = polygon_faces_camera(face, 3) ? 0 : 3;
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
	primitive->attached = (flags & 2U) != 0;
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

static legacy_u32 polygon_row_crossings(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
										legacy_f64 y, legacy_f64 *crossings)
{
	legacy_u32 crossing_count = 0;
	const struct SHAPE3D_HIRES_POINT *previous = &points[count - 1];
	for (legacy_u32 index = 0; index < count; index++) {
		const struct SHAPE3D_HIRES_POINT *current = &points[index];
		if ((previous->y <= y && current->y > y) || (current->y <= y && previous->y > y)) {
			/* Match the fill's half-open spans and shared-edge arithmetic exactly. */
			const struct SHAPE3D_HIRES_POINT *lower = previous->y < current->y ? previous : current;
			const struct SHAPE3D_HIRES_POINT *upper = previous->y < current->y ? current : previous;
			legacy_f64 fraction = (y - lower->y) / (upper->y - lower->y);
			crossings[crossing_count++] = lower->x + fraction * (upper->x - lower->x);
		}
		previous = current;
	}
	return crossing_count;
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
			if (paint->context != NULL) {
				hires_raster_span(paint->context, left, right, y, inverse_z, depth_step,
								  paint->family, paint->depth_mode, paint->color, paint->alternate,
								  paint->pattern, paint->mode, paint->depth_test);
			} else {
				for (legacy_s32 x = left; x < right; x++) {
					paint_pixel(x, y, inverse_z, paint);
					inverse_z += depth_step;
				}
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
		legacy_f64 crossings[HIRES_ROUND_POINTS];
		legacy_u32 crossing_count = 0;
		legacy_s32 crossings_valid = 0;
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
			if (offset_x * offset_x + offset_y * offset_y < radius * radius) {
				/* An edge can cover several samples on the same row. Compute the
				 * polygon intersections once, while preserving exact fill coverage. */
				if (!crossings_valid) {
					crossing_count = polygon_row_crossings(points, count, sample_y, crossings);
					crossings_valid = 1;
				}
				legacy_s32 inside = 0;
				for (legacy_u32 crossing = 0; crossing < crossing_count; crossing++) {
					inside ^= crossings[crossing] <= x + 0.5;
				}
				if (inside) {
					continue;
				}
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
	struct RECTANGLE bounds = {320, 0, 200, 0};
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
		size_t capacity = command_capacity != 0 ? command_capacity * 2 : 1024;
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

/* The depth pass has finished before these jobs start. Reconstruct receivers
 * from the visible depth, so shadows follow roads, banked surfaces and scenery.
 * The horizon's plain ground has no polygon depth; intersect its world plane. */
struct HIRES_SHADOW_BATCH {
	struct HIRES_RASTER_TARGET target;
	legacy_f64 inverse_view[3][3];
	legacy_f64 view_transpose[3][3];
	legacy_f64 ray_step_x[3], ray_step_y[3];
	legacy_f64 footprint_scale;
	legacy_s32 baked;
	legacy_u32 added[HIRES_BAND_COUNT];
};

static legacy_s32 shadow_depth_derivative(const struct HIRES_RASTER_TARGET *target, legacy_s32 x,
										  legacy_s32 y, legacy_u32 family, legacy_f64 depth,
										  legacy_s32 dx, legacy_s32 dy, legacy_f64 *derivative)
{
	*derivative = 0;
	legacy_s32 found = 0;
	for (legacy_s32 direction = -1; direction <= 1; direction += 2) {
		legacy_s32 sx = x + direction * dx;
		legacy_s32 sy = y + direction * dy;
		if (sx < target->depth_left || sx >= target->depth_right || sy < target->depth_top ||
			sy >= target->depth_bottom) {
			continue;
		}
		size_t offset = (size_t)sy * HIRES_WIDTH + sx;
		if (target->depth_family[offset] == family) {
			legacy_f64 candidate = (target->inverse_depth[offset] - depth) * direction;
			/* An ordered shape can contain a crease. Prefer the neighbor whose
			 * depth stays closest to this receiver, never an unrelated silhouette. */
			if (!found || SDL_fabs(candidate) < SDL_fabs(*derivative)) {
				*derivative = candidate;
				found = 1;
			}
		}
	}
	return found;
}

static legacy_u8 shade_receiver(const struct HIRES_SHADOW_BATCH *batch, const legacy_f64 *ray,
								legacy_f64 view_x, legacy_f64 view_y, legacy_f64 inverse_depth,
								legacy_f64 derivative_x, legacy_f64 derivative_y,
								legacy_s32 plane_valid, legacy_f64 ground,
								legacy_f64 depth_variation, legacy_f64 sample_scale,
								legacy_u32 *surface_hint)
{
	legacy_f64 depth;
	legacy_f64 normal[3] = {0, 1, 0};
	if (inverse_depth > 0) {
		depth = 1.0 / inverse_depth;
		if (batch->baked) {
			/* Inverse-depth variation accounts for grazing surfaces without
			 * reconstructing a normal or taking additional depth samples. */
			legacy_f64 footprint =
				depth * (batch->footprint_scale * sample_scale + depth_variation * depth);
			return shape3d_shadows_sample_cached_view(ray[0] * depth, ray[1] * depth,
													  ray[2] * depth, footprint, surface_hint);
		}
		if (!plane_valid) {
			return 0;
		}
		/* Inverse depth is affine across a projected plane. Its derivatives
		 * recover the receiver normal without three world-space reconstructions.
		 * Transform normals with the view transpose, not the position inverse. */
		legacy_f64 nx = derivative_x * projection_focal_length_x * HIRES_SCALE;
		legacy_f64 ny = -derivative_y * projection_focal_length_y * HIRES_SCALE;
		legacy_f64 nz = inverse_depth - nx * view_x - ny * view_y;
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			normal[axis] = batch->view_transpose[axis][0] * nx +
						   batch->view_transpose[axis][1] * ny +
						   batch->view_transpose[axis][2] * nz;
		}
	} else if (ground < 0 && ray[1] < -0.0001) {
		depth = ground / ray[1];
	} else {
		return 0;
	}
	if (batch->baked) {
		/* Ground spans grow toward the horizon; include that footprint so a
		 * distant dense grille averages instead of shimmering between holes. */
		legacy_f64 footprint = depth * batch->footprint_scale * sample_scale;
		if (ray[1] < -0.0001) {
			footprint += depth * SDL_fabs(batch->ray_step_y[1] / ray[1]) * 2 * sample_scale;
		}
		return shape3d_shadows_sample_cached_view(ray[0] * depth, ray[1] * depth, ray[2] * depth,
												  footprint, surface_hint);
	}
	return shape3d_shadows_sample_plane(ray[0] * depth, ray[1] * depth, ray[2] * depth, normal[0],
										normal[1], normal[2]);
}

static legacy_u32 shade_block2(const struct HIRES_SHADOW_BATCH *batch,
							   struct HIRES_RASTER_CONTEXT *context, legacy_s32 x, legacy_s32 y,
							   const legacy_f64 *ray, legacy_f64 view_x, legacy_f64 view_y,
							   legacy_f64 ground, legacy_u32 *surface_hint)
{
	const struct HIRES_RASTER_TARGET *target = &batch->target;
	legacy_f64 step_x = 1.0 / (projection_focal_length_x * HIRES_SCALE);
	legacy_f64 step_y = -1.0 / (projection_focal_length_y * HIRES_SCALE);
	legacy_u32 added = 0;
	size_t index = (size_t)y * HIRES_WIDTH + x;
	legacy_u32 family = target->depth_family[index];
	legacy_s32 shared = x + 1 < target->depth_right && y + 1 < context->bottom;
	legacy_f64 depths[4] = {0, 0, 0, 0};
	legacy_f64 average = 0;
	legacy_f64 depth_variation = 0;
	if (shared) {
		shared = target->depth_family[index + 1] == family &&
				 target->depth_family[index + HIRES_WIDTH] == family &&
				 target->depth_family[index + HIRES_WIDTH + 1] == family;
	}
	if (shared && family != 0) {
		depths[0] = target->inverse_depth[index];
		depths[1] = target->inverse_depth[index + 1];
		depths[2] = target->inverse_depth[index + HIRES_WIDTH];
		depths[3] = target->inverse_depth[index + HIRES_WIDTH + 1];
		legacy_f64 minimum = depths[0], maximum = depths[0];
		for (legacy_s32 sample = 1; sample < 4; sample++) {
			if (depths[sample] < minimum) {
				minimum = depths[sample];
			}
			if (depths[sample] > maximum) {
				maximum = depths[sample];
			}
		}
		depth_variation = maximum - minimum;
		shared = depth_variation <= minimum * 0.01;
		average = (depths[0] + depths[1] + depths[2] + depths[3]) * 0.25;
	}
	legacy_u8 opacity = 0;
	if (shared) {
		legacy_f64 derivative_x = 0, derivative_y = 0;
		if (!batch->baked) {
			derivative_x = (depths[1] - depths[0] + depths[3] - depths[2]) * 0.5;
			derivative_y = (depths[2] - depths[0] + depths[3] - depths[1]) * 0.5;
		}
		opacity = shade_receiver(batch, ray, view_x, view_y, average, derivative_x, derivative_y, 1,
								 ground, depth_variation, 1, surface_hint);
	}
	if (shared && opacity != 0) {
		added += hires_raster_shadow_block2(context, x, y, opacity);
	}
	if (!shared) {
		for (legacy_s32 sample = 0; sample < 4; sample++) {
			legacy_s32 sx = x + sample % 2;
			legacy_s32 sy = y + sample / 2;
			if (sx >= target->depth_right || sy >= context->bottom) {
				continue;
			}
			legacy_f64 sample_ray[3];
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				sample_ray[axis] = ray[axis] + (sample % 2 - 0.5) * batch->ray_step_x[axis] +
								   (sample / 2 - 0.5) * batch->ray_step_y[axis];
			}
			size_t offset = (size_t)sy * HIRES_WIDTH + sx;
			legacy_u32 sample_family = target->depth_family[offset];
			legacy_f64 depth = sample_family != 0 ? target->inverse_depth[offset] : 0;
			legacy_f64 derivative_x = 0;
			legacy_f64 derivative_y = 0;
			legacy_s32 plane_valid = 1;
			if (sample_family != 0 && !batch->baked) {
				plane_valid = shadow_depth_derivative(target, sx, sy, sample_family, depth, 1, 0,
													  &derivative_x);
				plane_valid &= shadow_depth_derivative(target, sx, sy, sample_family, depth, 0, 1,
													   &derivative_y);
			}
			opacity = shade_receiver(batch, sample_ray, view_x + (sample % 2 - 0.5) * step_x,
									 view_y + (sample / 2 - 0.5) * step_y, depth, derivative_x,
									 derivative_y, plane_valid, ground, 0, 1, surface_hint);
			if (opacity != 0) {
				added += hires_raster_shadow(context, sx, sy, opacity);
			}
		}
	}
	return added;
}

static void shade_band(void *argument, legacy_s32 band)
{
	struct HIRES_SHADOW_BATCH *batch = argument;
	const struct HIRES_RASTER_TARGET *target = &batch->target;
	struct HIRES_RASTER_CONTEXT context = {target, band * HIRES_BAND_HEIGHT,
										   (band + 1) * HIRES_BAND_HEIGHT, 0};
	if (context.top < target->depth_top) {
		context.top = target->depth_top;
	}
	if (context.bottom > target->depth_bottom) {
		context.bottom = target->depth_bottom;
	}
	legacy_f64 step_x = 1.0 / (projection_focal_length_x * HIRES_SCALE);
	legacy_f64 step_y = -1.0 / (projection_focal_length_y * HIRES_SCALE);
	legacy_f64 first_x =
		(target->depth_left + 2.0 - (legacy_s16)projection_center_x * HIRES_SCALE) * step_x;
	legacy_f64 ground = shape3d_shadows_ground_height();
	legacy_u32 added = 0, surface_hint = 0;
	/* Contact shading changes slowly inside a surface. Share one filtered
	 * lookup across a legacy 4x4 cell; retain 2x2 and individual lookups at
	 * silhouettes and depth breaks, where a coarse sample would bleed. */
	for (legacy_s32 y = context.top; y < context.bottom; y += 4) {
		legacy_f64 view_y = (y + 2.0 - (legacy_s16)projection_center_y * HIRES_SCALE) * step_y;
		legacy_f64 ray[3], view_x = first_x;
		for (legacy_s32 axis = 0; axis < 3; axis++) {
			ray[axis] = batch->inverse_view[axis][0] * first_x +
						batch->inverse_view[axis][1] * view_y + batch->inverse_view[axis][2];
		}
		for (legacy_s32 x = target->depth_left; x < target->depth_right; x += 4) {
			legacy_s32 shared =
				batch->baked && x + 3 < target->depth_right && y + 3 < context.bottom;
			legacy_f64 average = 0, minimum = 0, maximum = 0;
			if (shared) {
				size_t index = (size_t)y * HIRES_WIDTH + x;
				legacy_u32 family = target->depth_family[index];
				for (legacy_s32 row = 0; row < 4 && shared; row++) {
					const legacy_u32 *families = target->depth_family + index + row * HIRES_WIDTH;
					shared = families[0] == family && families[1] == family &&
							 families[2] == family && families[3] == family;
				}
				if (shared && family != 0) {
					minimum = maximum = target->inverse_depth[index];
					for (legacy_s32 row = 0; row < 4; row++) {
						const legacy_f32 *depths =
							target->inverse_depth + index + row * HIRES_WIDTH;
						for (legacy_s32 column = 0; column < 4; column++) {
							legacy_f64 depth = depths[column];
							average += depth;
							if (depth < minimum) {
								minimum = depth;
							}
							if (depth > maximum) {
								maximum = depth;
							}
						}
					}
					average *= 0.0625;
					shared = maximum - minimum <= minimum * 0.03;
				}
			}
			if (shared) {
				legacy_u8 opacity = shade_receiver(batch, ray, view_x, view_y, average, 0, 0, 1,
												   ground, maximum - minimum, 2, &surface_hint);
				if (opacity != 0) {
					added += hires_raster_shadow_block4(&context, x, y, opacity);
				}
			} else {
				for (legacy_s32 row = 0; row < 4 && y + row < context.bottom; row += 2) {
					for (legacy_s32 column = 0; column < 4 && x + column < target->depth_right;
						 column += 2) {
						legacy_f64 sample_ray[3];
						for (legacy_s32 axis = 0; axis < 3; axis++) {
							sample_ray[axis] = ray[axis] + (column - 1) * batch->ray_step_x[axis] +
											   (row - 1) * batch->ray_step_y[axis];
						}
						added += shade_block2(batch, &context, x + column, y + row, sample_ray,
											  view_x + (column - 1) * step_x,
											  view_y + (row - 1) * step_y, ground, &surface_hint);
					}
				}
			}
			for (legacy_s32 axis = 0; axis < 3; axis++) {
				ray[axis] += batch->ray_step_x[axis] * 4;
			}
			view_x += step_x * 4;
		}
	}
	batch->added[band] = added;
}

static legacy_s32 prepare_shadow_batch(struct HIRES_SHADOW_BATCH *batch)
{
	if (!shape3d_shadows_active() || projection_focal_length_x == 0 ||
		projection_focal_length_y == 0 || !hires_shadow_prepare()) {
		return 0;
	}
	if (!rendered_depth_valid || rendered_generation != hires_generation()) {
		hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
		rendered_depth_valid = 1;
		rendered_generation = hires_generation();
	}
	if (!hires_raster_prepare(&batch->target)) {
		return 0;
	}
	/* Invert the actual fixed-point view matrix, including its rounding.
	 * A transpose alone drifts far enough to cause acne on distant slopes. */
	legacy_f64 view[3][3];
	for (legacy_s32 row = 0; row < 3; row++) {
		for (legacy_s32 column = 0; column < 3; column++) {
			view[row][column] = mat_temp.vals[column * 3 + row] / (legacy_f64)TRIG_FIXED_ONE;
			batch->view_transpose[column][row] = view[row][column];
		}
	}
	legacy_f64 determinant = 0;
	for (legacy_s32 row = 0; row < 3; row++) {
		for (legacy_s32 column = 0; column < 3; column++) {
			batch->inverse_view[column][row] =
				view[(row + 1) % 3][(column + 1) % 3] * view[(row + 2) % 3][(column + 2) % 3] -
				view[(row + 1) % 3][(column + 2) % 3] * view[(row + 2) % 3][(column + 1) % 3];
		}
		determinant += view[row][0] * batch->inverse_view[0][row];
	}
	if (SDL_fabs(determinant) < 0.0001) {
		return 0;
	}
	for (legacy_s32 row = 0; row < 3; row++) {
		for (legacy_s32 column = 0; column < 3; column++) {
			batch->inverse_view[row][column] /= determinant;
		}
	}
	batch->baked = shape3d_shadows_baked();
	legacy_f64 step_x = 1.0 / (projection_focal_length_x * HIRES_SCALE);
	legacy_f64 step_y = -1.0 / (projection_focal_length_y * HIRES_SCALE);
	batch->footprint_scale = 2 * (step_x > -step_y ? step_x : -step_y);
	for (legacy_s32 axis = 0; axis < 3; axis++) {
		batch->ray_step_x[axis] = batch->inverse_view[axis][0] * step_x;
		batch->ray_step_y[axis] = batch->inverse_view[axis][1] * step_y;
	}
	return 1;
}

static void finish_shadow_batch(struct HIRES_SHADOW_BATCH *batch)
{
	legacy_u32 added = 0;
	for (legacy_s32 band = 0; band < HIRES_BAND_COUNT; band++) {
		added += batch->added[band];
	}
	hires_raster_shadow_finish(&batch->target, added);
}

static void shade_scene(void)
{
	struct HIRES_SHADOW_BATCH batch;
	if (!prepare_shadow_batch(&batch)) {
		return;
	}
	render_workers_run(HIRES_BAND_COUNT, shade_band, &batch);
	finish_shadow_batch(&batch);
}

struct HIRES_SHADED_BATCH {
	struct HIRES_BATCH *geometry;
	struct HIRES_SHADOW_BATCH *lighting;
};

static void render_shaded_band(void *argument, legacy_s32 band)
{
	struct HIRES_SHADED_BATCH *batch = argument;
	render_band(batch->geometry, band);
	/* Cached lighting reads only this band's finished depth. Uncached dynamic
	 * lighting needs neighboring normal samples and keeps the separate pass. */
	shade_band(batch->lighting, band);
}

legacy_s32 shape3d_hires_batch_end(void)
{
	batching = 0;
	struct HIRES_BATCH batch;
	legacy_s32 workers = 0;
	legacy_s32 combined_shading = 0;
	struct HIRES_SHADOW_BATCH lighting;
	/* The prepared span path benefits serial drawing too. Initialize depth
	 * once before either path, including the low-area and zero-worker cases. */
	if (!rendered_depth_valid || rendered_generation != hires_generation()) {
		hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
		rendered_depth_valid = 1;
		rendered_generation = hires_generation();
	}
	if (hires_raster_prepare(&batch.target)) {
		legacy_u32 cleared = 0;
		if (command_area >= HIRES_PARALLEL_MIN_AREA && render_workers_count() != 0) {
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
			combined_shading = shape3d_shadows_baked() && prepare_shadow_batch(&lighting);
			if (combined_shading) {
				struct HIRES_SHADED_BATCH shaded = {&batch, &lighting};
				workers = render_workers_run(HIRES_BAND_COUNT, render_shaded_band, &shaded);
			} else {
				workers = render_workers_run(HIRES_BAND_COUNT, render_band, &batch);
			}
			for (legacy_s32 index = 0; index < HIRES_BAND_COUNT; index++) {
				cleared += batch.bands[index].cleared_argb_cells;
			}
		} else {
			struct HIRES_RASTER_CONTEXT context = {&batch.target, batch.target.top,
												   batch.target.bottom, 0};
			for (size_t index = 0; index < command_count; index++) {
				const struct HIRES_COMMAND *entry = &commands[index];
				render_primitive(entry->index, entry->type, entry->color, entry->second_color,
								 entry->third_color, entry->pattern_type, entry->pattern, &context);
			}
			cleared = context.cleared_argb_cells;
		}
		hires_raster_finish(&batch.target, cleared);
	} else {
		for (size_t index = 0; index < command_count; index++) {
			const struct HIRES_COMMAND *entry = &commands[index];
			render_primitive(entry->index, entry->type, entry->color, entry->second_color,
							 entry->third_color, entry->pattern_type, entry->pattern, NULL);
		}
	}
	if (combined_shading) {
		finish_shadow_batch(&lighting);
	} else {
		shade_scene();
	}
	command_count = 0;
	command_area = 0;
	return workers;
}

#endif
