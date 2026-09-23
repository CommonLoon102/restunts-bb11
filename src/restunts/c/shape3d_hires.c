#include "shape3d_hires.h"

#if defined(RESTUNTS_SDL3)

#include <SDL3/SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>
#include "fatal.h"
#include "hires.h"
#include "projection.h"
#include "shape3d_internal.h"

#define HIRES_NEAR_CLIP_Z 12
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
	legacy_u16 color;
	legacy_u16 alternate;
	legacy_u16 pattern;
	legacy_u16 mode;
	legacy_s32 depth_test;
	legacy_u32 family;
	legacy_s32 attached;
};

struct HIRES_SHAPE {
	struct RECTANGLE bounds;
	legacy_s32 depth_test;
};

static struct HIRES_PRIMITIVE *primitives;
static struct HIRES_SHAPE *shapes;
static legacy_u32 primitive_capacity;
static legacy_u32 primitive_count;
static legacy_u32 current_shape;
static legacy_u32 current_family;
static legacy_u32 rendered_shape = LEGACY_U32_MAX;
static legacy_u32 rendered_generation;
static legacy_f64 model_scale = 1;

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

void shape3d_hires_begin_shape(legacy_u32 index, legacy_s32 depth_test)
{
	reserve_primitives(index);
	current_shape = index;
	current_family = index + 1;
	shapes[index].bounds.left = HIRES_WIDTH / HIRES_SCALE;
	shapes[index].bounds.right = 0;
	shapes[index].bounds.top = HIRES_HEIGHT / HIRES_SCALE;
	shapes[index].bounds.bottom = 0;
	shapes[index].depth_test = depth_test;
}

void shape3d_hires_set_model_scale(legacy_f64 scale)
{
	model_scale = scale;
}

void shape3d_hires_reset(void)
{
	model_scale = 1;
	shape3d_hires_begin_shape(0, 1);
	rendered_shape = LEGACY_U32_MAX;
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
	shape3d_hires_update_bounds(index, type, &shapes[current_shape].bounds);
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
		/* Include the maximum stroke and rounding to its nearest sample in both
		 * the sprite copy rectangle and the per-shape depth buffer bounds. */
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
	legacy_u16 color = paint->color;
	if (paint->mode != 0) {
		legacy_u32 bit = ((y & 1) == 0 ? 8U : 0U) + 7U - (x & 7);
		if ((paint->pattern & (1U << bit)) != 0) {
			color = paint->mode == 2 ? paint->alternate : paint->color;
		} else if (paint->mode != 2) {
			return;
		}
	}
	if (!paint->depth_test || hires_depth_test(x, y, inverse_z, paint->family, paint->attached)) {
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
	for (legacy_s32 row = y - extent; row <= y + extent; row++) {
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

void shape3d_hires_render(legacy_u32 index, legacy_u8 type, legacy_u16 color,
						  legacy_u16 second_color, legacy_u16 third_color, legacy_u16 pattern_type,
						  legacy_u16 pattern)
{
	if (index >= primitive_capacity || primitives[index].count == 0) {
		return;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	const struct HIRES_SHAPE *shape = &shapes[primitive->shape];
	if (rendered_shape != primitive->shape || rendered_generation != hires_generation()) {
		if (shape->depth_test) {
			hires_depth_begin(shape->bounds.left * HIRES_SCALE, shape->bounds.right * HIRES_SCALE,
							  shape->bounds.top * HIRES_SCALE, shape->bounds.bottom * HIRES_SCALE);
		}
		rendered_shape = primitive->shape;
		rendered_generation = hires_generation();
	}
	struct HIRES_PAINT paint = {color,
								second_color,
								pattern,
								pattern_type,
								shape->depth_test,
								primitive->family,
								primitive->attached};
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

#endif
