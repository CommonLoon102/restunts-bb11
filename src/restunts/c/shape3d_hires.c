#include "shape3d_hires.h"

#if defined(RESTUNTS_SDL3)

#include "hires.h"
#include "projection.h"
#include "shape3d_internal.h"

extern legacy_s8 is_facing_camera(struct POINT2D far *points);

#define HIRES_NEAR_CLIP_Z 12
#define HIRES_MAX_POLYGON_POINTS 20
#define HIRES_ROUND_POINTS 64
#define HIRES_WHEEL_INNER_SCALE (9472.0 / TRIG_FIXED_ONE)

struct HIRES_PRIMITIVE {
	struct SHAPE3D_HIRES_POINT points[HIRES_MAX_POLYGON_POINTS];
	unsigned int count;
	double size;
};

struct HIRES_PAINT {
	legacy_u16 color;
	legacy_u16 alternate;
	legacy_u16 pattern;
	legacy_u16 mode;
};

static struct HIRES_PRIMITIVE primitives[POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY];

static void project_coordinates(double x, double y, double z, struct SHAPE3D_HIRES_POINT *point)
{
	point->x = (legacy_s16)projection_center_x * HIRES_SCALE +
			   x * projection_focal_length_x * HIRES_SCALE / z;
	point->y = (legacy_s16)projection_center_y * HIRES_SCALE -
			   y * projection_focal_length_y * HIRES_SCALE / z;
}

void shape3d_hires_project(const struct VECTOR *vector, struct SHAPE3D_HIRES_POINT *point)
{
	project_coordinates(vector->x, vector->y, vector->z > 0 ? vector->z : 1, point);
}

void shape3d_hires_reset(void)
{
	for (unsigned int index = 0; index < POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY; index++) {
		primitives[index].count = 0;
	}
}

static void project_intersection(const struct VECTOR *first, const struct VECTOR *second,
								 struct SHAPE3D_HIRES_POINT *point)
{
	/* Near-plane intersections need the same subpixel precision as ordinary
	 * vertices; rounding back into a legacy VECTOR would discard it. */
	double fraction = (HIRES_NEAR_CLIP_Z - first->z) / (double)(second->z - first->z);
	project_coordinates(first->x + (second->x - first->x) * fraction,
						first->y + (second->y - first->y) * fraction, HIRES_NEAR_CLIP_Z, point);
}

static void queue_polygon(struct HIRES_PRIMITIVE *primitive, unsigned int count,
						  const legacy_u8 *indices, const struct VECTOR *vertices)
{
	const struct VECTOR *previous = &vertices[indices[count - 1]];
	for (unsigned int index = 0; index < count; index++) {
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

void shape3d_hires_queue(legacy_u16 index, legacy_u8 type, legacy_u16 vertex_count,
						 const legacy_u8 *indices, const struct VECTOR *vertices,
						 const struct POINT2D *projected)
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
		struct POINT2D face[3];
		for (unsigned int vertex = 0; vertex < 3; vertex++) {
			face[vertex] = projected[indices[vertex]];
		}
		unsigned int start = is_facing_camera(face) != 0 ? 0 : 3;
		for (unsigned int vertex = 0; vertex < 4; vertex++) {
			shape3d_hires_project(&vertices[indices[(start + vertex) % 6]],
								  &primitive->points[vertex]);
		}
		primitive->count = 4;
		return;
	}
	for (unsigned int vertex = 0; vertex < vertex_count; vertex++) {
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
		primitive->size =
			(double)projection_focal_length_x * polarRadius3D(&radius) * HIRES_SCALE / center->z;
	}
}

static int ceil_coordinate(double coordinate)
{
	int result = (int)coordinate;
	return result < coordinate ? result + 1 : result;
}

static double absolute_coordinate(double value)
{
	return value < 0 ? -value : value;
}

void shape3d_hires_update_bounds(legacy_u16 index, legacy_u8 type, struct RECTANGLE *rectangle)
{
	if (index >= POLYINFO_SUPERSIGHT_PRIMITIVE_CAPACITY || primitives[index].count == 0) {
		return;
	}
	const struct HIRES_PRIMITIVE *primitive = &primitives[index];
	double minimum_x = primitive->points[0].x;
	double maximum_x = minimum_x;
	double minimum_y = primitive->points[0].y;
	double maximum_y = minimum_y;
	if (type == RENDER_PRIMITIVE_SPHERE) {
		minimum_x -= primitive->size * 0.5;
		maximum_x += primitive->size * 0.5;
		minimum_y -= primitive->size * (13.0 / 32.0);
		maximum_y += primitive->size * (13.0 / 32.0);
	} else if (type == RENDER_PRIMITIVE_WHEEL) {
		/* The sum of the two axis magnitudes bounds every perimeter point,
		 * including tilted ellipses and the far edge of the tread. */
		double width = absolute_coordinate(primitive->points[1].x - minimum_x) +
					   absolute_coordinate(primitive->points[2].x - minimum_x);
		double height = absolute_coordinate(primitive->points[1].y - minimum_y) +
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
		for (unsigned int point = 1; point < primitive->count; point++) {
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
	int left = minimum_x <= 0 ? 0 : (int)(minimum_x / HIRES_SCALE);
	int top = minimum_y <= 0 ? 0 : (int)(minimum_y / HIRES_SCALE);
	int right = maximum_x >= HIRES_WIDTH - HIRES_SCALE
					? HIRES_WIDTH / HIRES_SCALE
					: ceil_coordinate(maximum_x / HIRES_SCALE) + 1;
	int bottom = maximum_y >= HIRES_HEIGHT - HIRES_SCALE
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

static void paint_pixel(int x, int y, const struct HIRES_PAINT *paint)
{
	if (paint->mode == 0) {
		hires_pixel(x, y, (unsigned char)paint->color);
		return;
	}
	unsigned int bit = ((y & 1) == 0 ? 8U : 0U) + 7U - (x & 7);
	if ((paint->pattern & (1U << bit)) != 0) {
		hires_pixel(x, y, (unsigned char)(paint->mode == 2 ? paint->alternate : paint->color));
	} else if (paint->mode == 2) {
		hires_pixel(x, y, (unsigned char)paint->color);
	}
}

static int clip_line_edge(double direction, double distance, double *first, double *last)
{
	if (direction == 0) {
		return distance >= 0;
	}
	double ratio = distance / direction;
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
	double delta_x = last->x - first->x;
	double delta_y = last->y - first->y;
	double start = 0;
	double end = 1;
	if (!clip_line_edge(-delta_x, first->x, &start, &end) ||
		!clip_line_edge(delta_x, HIRES_WIDTH - 1 - first->x, &start, &end) ||
		!clip_line_edge(-delta_y, first->y, &start, &end) ||
		!clip_line_edge(delta_y, HIRES_HEIGHT - 1 - first->y, &start, &end)) {
		return;
	}
	int x = (int)(first->x + delta_x * start + 0.5);
	int y = (int)(first->y + delta_y * start + 0.5);
	int end_x = (int)(first->x + delta_x * end + 0.5);
	int end_y = (int)(first->y + delta_y * end + 0.5);
	int step_x = x < end_x ? 1 : -1;
	int step_y = y < end_y ? 1 : -1;
	int width = x < end_x ? end_x - x : x - end_x;
	int height = y < end_y ? y - end_y : end_y - y;
	int error = width + height;
	for (;;) {
		paint_pixel(x, y, paint);
		if (x == end_x && y == end_y) {
			break;
		}
		int twice_error = error * 2;
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

static void draw_polygon(const struct SHAPE3D_HIRES_POINT *points, unsigned int count,
						 const struct HIRES_PAINT *paint)
{
	if (count == 0) {
		return;
	}
	if (count < 3) {
		draw_line(&points[0], &points[count - 1], paint);
		return;
	}
	double minimum_y = points[0].y;
	double maximum_y = minimum_y;
	for (unsigned int index = 1; index < count; index++) {
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
	int top = minimum_y < 0 ? 0 : ceil_coordinate(minimum_y - 0.5);
	int bottom = maximum_y >= HIRES_HEIGHT ? HIRES_HEIGHT : ceil_coordinate(maximum_y - 0.5);
	for (int y = top; y < bottom; y++) {
		double intersections[HIRES_ROUND_POINTS];
		unsigned int intersection_count = 0;
		double sample_y = y + 0.5;
		const struct SHAPE3D_HIRES_POINT *previous = &points[count - 1];
		for (unsigned int index = 0; index < count; index++) {
			const struct SHAPE3D_HIRES_POINT *current = &points[index];
			if ((previous->y <= sample_y && current->y > sample_y) ||
				(current->y <= sample_y && previous->y > sample_y)) {
				double x = previous->x + (sample_y - previous->y) * (current->x - previous->x) /
											 (current->y - previous->y);
				unsigned int position = intersection_count++;
				while (position != 0 && intersections[position - 1] > x) {
					intersections[position] = intersections[position - 1];
					position--;
				}
				intersections[position] = x;
			}
			previous = current;
		}
		for (unsigned int index = 0; index + 1 < intersection_count; index += 2) {
			if (intersections[index + 1] < 0 || intersections[index] >= HIRES_WIDTH) {
				continue;
			}
			int left = intersections[index] < 0 ? 0 : ceil_coordinate(intersections[index] - 0.5);
			int right = intersections[index + 1] >= HIRES_WIDTH
							? HIRES_WIDTH
							: ceil_coordinate(intersections[index + 1] - 0.5);
			for (int x = left; x < right; x++) {
				paint_pixel(x, y, paint);
			}
		}
	}
}

static void build_perimeter(const struct SHAPE3D_HIRES_POINT *center,
							const struct SHAPE3D_HIRES_POINT *first_axis,
							const struct SHAPE3D_HIRES_POINT *second_axis, double scale,
							struct SHAPE3D_HIRES_POINT *points)
{
	for (unsigned int index = 0; index < HIRES_ROUND_POINTS; index++) {
		legacy_s16 angle = (legacy_s16)(index * ANGLE_FULL_TURN / HIRES_ROUND_POINTS);
		double cosine = cos_fast(angle) * scale / TRIG_FIXED_ONE;
		double sine = sin_fast(angle) * scale / TRIG_FIXED_ONE;
		points[index].x =
			center->x + (first_axis->x - center->x) * cosine + (second_axis->x - center->x) * sine;
		points[index].y =
			center->y + (first_axis->y - center->y) * cosine + (second_axis->y - center->y) * sine;
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
	double depth_x = primitive->points[3].x - primitive->points[0].x;
	double depth_y = primitive->points[3].y - primitive->points[0].y;
	for (unsigned int index = 0; index < HIRES_ROUND_POINTS; index++) {
		unsigned int next = (index + 1) % HIRES_ROUND_POINTS;
		struct SHAPE3D_HIRES_POINT side[4] = {outer[index], outer[next], outer[next], outer[index]};
		side[2].x += depth_x;
		side[2].y += depth_y;
		side[3].x += depth_x;
		side[3].y += depth_y;
		draw_polygon(side, 4, &paint);
	}
	paint.color = side_color;
	for (unsigned int index = 0; index < HIRES_ROUND_POINTS; index++) {
		unsigned int next = (index + 1) % HIRES_ROUND_POINTS;
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
	struct HIRES_PAINT paint = {color, second_color, pattern, pattern_type};
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
