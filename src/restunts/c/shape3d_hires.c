#include "shape3d_hires.h"

#if defined(RESTUNTS_SDL3)

#include "hires.h"
#include "projection.h"
#include "shape3d_internal.h"

#define HIRES_NEAR_CLIP_Z 12
#define HIRES_MAX_POLYGON_POINTS 20
#define HIRES_ROUND_POINTS 64
#define HIRES_WHEEL_INNER_SCALE (9472.0 / TRIG_FIXED_ONE)

struct HIRES_PRIMITIVE {
	struct SHAPE3D_HIRES_POINT points[HIRES_MAX_POLYGON_POINTS];
	legacy_u32 count;
	legacy_u32 wheel_face;
	legacy_u16 shape;
	legacy_u16 family;
	legacy_s32 attached;
	legacy_f64 size;
};

struct HIRES_PAINT {
	legacy_u16 color;
	legacy_u16 alternate;
	legacy_u16 pattern;
	legacy_u16 mode;
	legacy_s32 depth_test;
	legacy_u16 family;
	legacy_s32 attached;
};

struct HIRES_SHAPE {
	struct RECTANGLE bounds;
	legacy_s32 depth_test;
};

static struct HIRES_PRIMITIVE primitives[POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY];
static struct HIRES_SHAPE shapes[POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY];
static legacy_u16 current_shape;
static legacy_u16 current_family;
static legacy_u16 rendered_shape = LEGACY_U16_MAX;
static legacy_u32 rendered_generation;

static void project_coordinates(legacy_f64 x, legacy_f64 y, legacy_f64 z,
								struct SHAPE3D_HIRES_POINT *point)
{
	point->x = (legacy_s16)projection_center_x * HIRES_SCALE +
			   x * projection_focal_length_x * HIRES_SCALE / z;
	point->y = (legacy_s16)projection_center_y * HIRES_SCALE -
			   y * projection_focal_length_y * HIRES_SCALE / z;
	point->inverse_z = 1.0 / z;
}

void shape3d_hires_project(const struct VECTOR *vector, struct SHAPE3D_HIRES_POINT *point)
{
	project_coordinates(vector->x, vector->y, vector->z > 0 ? vector->z : 1, point);
}

static legacy_u8 point_clip_flags(const struct SHAPE3D_HIRES_POINT *point)
{
	legacy_u8 flags = 0;
	if (point->y < select_rect_rc.top * HIRES_SCALE) {
		flags |= 1;
	} else if (point->y >= (select_rect_rc.bottom + 1) * HIRES_SCALE) {
		flags |= 2;
	}
	if (point->x < select_rect_rc.left * HIRES_SCALE) {
		flags |= 4;
	} else if (point->x >= (select_rect_rc.right + 1) * HIRES_SCALE) {
		flags |= 8;
	}
	return flags;
}

legacy_u8 shape3d_hires_clip_flags(const struct VECTOR *vector)
{
	struct SHAPE3D_HIRES_POINT point;
	shape3d_hires_project(vector, &point);
	return point_clip_flags(&point);
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

legacy_s32 shape3d_hires_polygon_visible(legacy_u16 index, legacy_s32 cull_backface)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY) {
		return 0;
	}
	const struct HIRES_PRIMITIVE *primitive = &primitives[index];
	if (primitive->count < 3) {
		return 0;
	}
	legacy_u8 flags = 15;
	for (legacy_u32 point = 0; point < primitive->count; point++) {
		flags &= point_clip_flags(&primitive->points[point]);
	}
	return flags == 0 &&
		   (!cull_backface || polygon_faces_camera(primitive->points, primitive->count));
}

legacy_u32 shape3d_hires_wheel_face(legacy_u16 index)
{
	return index < POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY ? primitives[index].wheel_face : 0;
}

void shape3d_hires_begin_shape(legacy_u16 index, legacy_s32 depth_test)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY) {
		return;
	}
	current_shape = index;
	current_family = index + 1;
	shapes[index].bounds.left = HIRES_WIDTH / HIRES_SCALE;
	shapes[index].bounds.right = 0;
	shapes[index].bounds.top = HIRES_HEIGHT / HIRES_SCALE;
	shapes[index].bounds.bottom = 0;
	shapes[index].depth_test = depth_test;
}

void shape3d_hires_reset(void)
{
	shape3d_hires_begin_shape(0, 1);
	rendered_shape = LEGACY_U16_MAX;
	for (legacy_u32 index = 0; index < POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY; index++) {
		primitives[index].count = 0;
	}
}

static void project_intersection(const struct VECTOR *first, const struct VECTOR *second,
								 struct SHAPE3D_HIRES_POINT *point)
{
	/* Near-plane intersections need the same subpixel precision as ordinary
	 * vertices; rounding back into a legacy VECTOR would discard it. */
	legacy_f64 fraction = (HIRES_NEAR_CLIP_Z - first->z) / (legacy_f64)(second->z - first->z);
	project_coordinates(first->x + (second->x - first->x) * fraction,
						first->y + (second->y - first->y) * fraction, HIRES_NEAR_CLIP_Z, point);
}

static void queue_polygon(struct HIRES_PRIMITIVE *primitive, legacy_u32 count,
						  const legacy_u8 *indices, const struct VECTOR *vertices)
{
	const struct VECTOR *previous = &vertices[indices[count - 1]];
	for (legacy_u32 index = 0; index < count; index++) {
		const struct VECTOR *current = &vertices[indices[index]];
		if ((previous->z < HIRES_NEAR_CLIP_Z) != (current->z < HIRES_NEAR_CLIP_Z)) {
			project_intersection(previous, current, &primitive->points[primitive->count++]);
		}
		if (current->z >= HIRES_NEAR_CLIP_Z) {
			shape3d_hires_project(current, &primitive->points[primitive->count++]);
		}
		previous = current;
	}
}

static void queue_primitive(legacy_u16 index, legacy_u8 type, legacy_u16 vertex_count,
							const legacy_u8 *indices, const struct VECTOR *vertices)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY) {
		return;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	primitive->count = 0;
	if (!hires_enabled() || vertex_count == 0 || vertex_count > HIRES_MAX_POLYGON_POINTS / 2) {
		return;
	}
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
		for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
			shape3d_hires_project(&vertices[indices[(start + vertex) % 6]],
								  &primitive->points[vertex]);
		}
		primitive->count = 4;
		return;
	}
	for (legacy_u32 vertex = 0; vertex < vertex_count; vertex++) {
		const struct VECTOR *current = &vertices[indices[vertex]];
		if (type == RENDER_PRIMITIVE_LINE && current->z < HIRES_NEAR_CLIP_Z) {
			project_intersection(current, &vertices[indices[1 - vertex]],
								 &primitive->points[vertex]);
		} else {
			shape3d_hires_project(current, &primitive->points[vertex]);
		}
	}
	primitive->count = vertex_count;
	if (type == RENDER_PRIMITIVE_SPHERE) {
		const struct VECTOR *center = &vertices[indices[0]];
		const struct VECTOR *endpoint = &vertices[indices[1]];
		struct VECTOR radius;
		radius.x = LEGACY_S16_WRAP_SUB(center->x, endpoint->x);
		radius.y = LEGACY_S16_WRAP_SUB(center->y, endpoint->y);
		radius.z = LEGACY_S16_WRAP_SUB(center->z, endpoint->z);
		primitive->size = (legacy_f64)projection_focal_length_x * polarRadius3D(&radius) *
						  HIRES_SCALE / center->z;
	}
}

void shape3d_hires_queue(legacy_u16 index, legacy_u8 type, legacy_u16 vertex_count,
						 const legacy_u8 *indices, const struct VECTOR *vertices, legacy_u16 flags)
{
	queue_primitive(index, type, vertex_count, indices, vertices);
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY || primitives[index].count == 0) {
		return;
	}
	struct HIRES_PRIMITIVE *primitive = &primitives[index];
	primitive->shape = current_shape;
	primitive->attached = (flags & 2U) != 0;
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

void shape3d_hires_update_bounds(legacy_u16 index, legacy_u8 type, struct RECTANGLE *rectangle)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY || primitives[index].count == 0) {
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

static void draw_line(const struct SHAPE3D_HIRES_POINT *first,
					  const struct SHAPE3D_HIRES_POINT *last, const struct HIRES_PAINT *paint)
{
	legacy_f64 delta_x = last->x - first->x;
	legacy_f64 delta_y = last->y - first->y;
	legacy_f64 start = 0;
	legacy_f64 end = 1;
	if (!clip_line_edge(-delta_x, first->x, &start, &end) ||
		!clip_line_edge(delta_x, HIRES_WIDTH - 1 - first->x, &start, &end) ||
		!clip_line_edge(-delta_y, first->y, &start, &end) ||
		!clip_line_edge(delta_y, HIRES_HEIGHT - 1 - first->y, &start, &end)) {
		return;
	}
	legacy_s32 x = (legacy_s32)(first->x + delta_x * start + 0.5);
	legacy_s32 y = (legacy_s32)(first->y + delta_y * start + 0.5);
	legacy_s32 end_x = (legacy_s32)(first->x + delta_x * end + 0.5);
	legacy_s32 end_y = (legacy_s32)(first->y + delta_y * end + 0.5);
	legacy_s32 step_x = x < end_x ? 1 : -1;
	legacy_s32 step_y = y < end_y ? 1 : -1;
	legacy_s32 width = x < end_x ? end_x - x : x - end_x;
	legacy_s32 height = y < end_y ? y - end_y : end_y - y;
	legacy_s32 error = width + height;
	legacy_s32 steps = width > -height ? width : -height;
	legacy_f64 inverse_z = first->inverse_z + (last->inverse_z - first->inverse_z) * start;
	legacy_f64 depth_step =
		steps == 0 ? 0 : (last->inverse_z - first->inverse_z) * (end - start) / steps;
	for (;;) {
		paint_pixel(x, y, inverse_z, paint);
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

static void draw_polygon(const struct SHAPE3D_HIRES_POINT *points, legacy_u32 count,
						 const struct HIRES_PAINT *paint)
{
	if (count == 0) {
		return;
	}
	if (count < 3) {
		draw_line(&points[0], &points[count - 1], paint);
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
				legacy_f64 fraction = (sample_y - previous->y) / (current->y - previous->y);
				struct SHAPE3D_HIRES_POINT intersection;
				intersection.x = previous->x + fraction * (current->x - previous->x);
				intersection.y = sample_y;
				intersection.inverse_z =
					previous->inverse_z + fraction * (current->inverse_z - previous->inverse_z);
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
	draw_polygon(points, HIRES_ROUND_POINTS, paint);
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
		draw_polygon(side, 4, &paint);
	}
	paint.color = side_color;
	for (legacy_u32 index = 0; index < HIRES_ROUND_POINTS; index++) {
		legacy_u32 next = (index + 1) % HIRES_ROUND_POINTS;
		struct SHAPE3D_HIRES_POINT rim[4] = {outer[index], outer[next], inner[next], inner[index]};
		draw_polygon(rim, 4, &paint);
	}
	paint.color = inner_color;
	draw_polygon(inner, HIRES_ROUND_POINTS, &paint);
}

void shape3d_hires_render(legacy_u16 index, legacy_u8 type, legacy_u16 color,
						  legacy_u16 second_color, legacy_u16 third_color, legacy_u16 pattern_type,
						  legacy_u16 pattern)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY || primitives[index].count == 0) {
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
		draw_polygon(primitive->points, primitive->count, &paint);
	} else if (type == RENDER_PRIMITIVE_LINE) {
		draw_line(&primitive->points[0], &primitive->points[1], &paint);
	} else if (type == RENDER_PRIMITIVE_POINT) {
		draw_line(&primitive->points[0], &primitive->points[0], &paint);
	} else if (type == RENDER_PRIMITIVE_SPHERE) {
		draw_sphere(primitive, &paint);
	} else if (type == RENDER_PRIMITIVE_WHEEL) {
		draw_wheel(primitive, paint, second_color, third_color);
	}
}

#endif
