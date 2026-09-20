#include "owoot.h"
#include "externs.h"
#include "shape3d.h"
#include "fileio.h"
#include "memmgr.h"
#include "fatal.h"

#define OWOOT_FIRST_WHEEL_VERTEX 8U
#define OWOOT_WHEEL_CONTROL_COUNT 6U
#define OWOOT_WHEEL_RING_COUNT 16U
#define OWOOT_POSITION_SCALE 64L

/* Preserve the unsteered model, independently of the renderer's mutable copy.
 * Each wheel has two circles, each specified by a centre and two radii. */
static struct VECTOR wheel_controls[CARSTATE_WHEEL_COUNT][OWOOT_WHEEL_CONTROL_COUNT];

void owoot_read_wheel_shape(const legacy_u8 far *shape)
{
	if (!owoot_enabled) {
		return;
	}
	if (shape == 0 ||
		shape[SHAPE3D_VERTEX_COUNT_OFFSET] <
			OWOOT_FIRST_WHEEL_VERTEX + CARSTATE_WHEEL_COUNT * OWOOT_WHEEL_CONTROL_COUNT) {
		fatal_error("OWOOT: invalid player wheel shape");
		return;
	}
	for (legacy_u16 wheel = 0; wheel < CARSTATE_WHEEL_COUNT; wheel++) {
		for (legacy_u16 point = 0; point < OWOOT_WHEEL_CONTROL_COUNT; point++) {
			legacy_u16 index = OWOOT_FIRST_WHEEL_VERTEX + wheel * OWOOT_WHEEL_CONTROL_COUNT + point;
			const legacy_u8 far *vertex = shape + SHAPE3D_HEADER_SIZE + index * SHAPE3D_VERTEX_SIZE;
			wheel_controls[wheel][point].x = LEGACY_S16_FROM_BITS(LEGACY_READ_U16_LE(vertex));
			wheel_controls[wheel][point].y = LEGACY_S16_FROM_BITS(LEGACY_READ_U16_LE(vertex + 2));
			wheel_controls[wheel][point].z = LEGACY_S16_FROM_BITS(LEGACY_READ_U16_LE(vertex + 4));
		}
	}
}

/* Physics dumps do not load car graphics. Load just long enough to retain the
 * same wheel controls as the game, including custom cars. */
void owoot_load_player_wheels(void)
{
	if (!owoot_enabled) {
		return;
	}
	legacy_s8 filename[] = "stxxxx.p3s";
	for (legacy_u16 index = 0; index < 4; index++) {
		filename[index + 2] = gameconfig.game_playercarid[index];
	}
	void far *resource = file_decomp_nofatal(filename);
	if (resource == 0) {
		filename[7] = '3';
		filename[8] = 's';
		filename[9] = 'h';
		resource = file_load_binary_fatal(filename);
	}
	owoot_read_wheel_shape((const legacy_u8 far *)locate_shape_fatal(resource, "car1"));
	mmgr_free(resource);
}

/* The renderer uses sixteen points with these Q14 coefficients, rather than
 * a bounding rectangle. This preserves small overlaps when a car is banked,
 * pitched or upside down. */
static const legacy_s16 ring_cos[OWOOT_WHEEL_RING_COUNT] = {
	16384,	14654,	11585,	7327,  0, -7327, -11585, -14654,
	-16384, -14654, -11585, -7327, 0, 7327,	 11585,	 14654};
static const legacy_s16 ring_sin[OWOOT_WHEEL_RING_COUNT] = {
	0, 7327,  11585,  14654,  16384,  14654,  11585,  7327,
	0, -7327, -11585, -14654, -16384, -14654, -11585, -7327};

static legacy_s16 ring_coordinate(legacy_s16 center, legacy_s16 first, legacy_s16 second,
								  legacy_u16 point)
{
	legacy_s32 value = ((legacy_s32)first - center) * ring_cos[point] +
					   ((legacy_s32)second - center) * ring_sin[point];
	return (legacy_s16)(center + value / TRIG_FIXED_ONE);
}

legacy_u16 owoot_wheel_footprint(const struct CARSTATE *car, legacy_u16 wheel,
								 struct VECTOR output[OWOOT_WHEEL_VERTEX_COUNT])
{
	struct MATRIX rotation = *mat_rot_zxy(-car->car_rotate.z, -car->car_rotate.y,
										  -car->car_rotate.x, MATRIX_ROTATION_ORDER_ZXY);
	legacy_s16 sine = sin_fast(LEGACY_S16_SAR(car->car_steeringAngle, 1U));
	legacy_s16 cosine = cos_fast(LEGACY_S16_SAR(car->car_steeringAngle, 1U));
	struct VECTOR controls[OWOOT_WHEEL_CONTROL_COUNT];
	legacy_s16 center_x = LEGACY_S16_SAR(
		LEGACY_S16_WRAP_ADD(wheel_controls[wheel][0].x, wheel_controls[wheel][3].x), 1U);
	legacy_s16 center_z = wheel_controls[wheel][0].z;
	for (legacy_u16 point = 0; point < OWOOT_WHEEL_CONTROL_COUNT; point++) {
		controls[point] = wheel_controls[wheel][point];
		if (wheel < 2 && car->car_steeringAngle != 0) {
			/* Match shape3d_steer_car_wheel_vertices: its cached control
			 * offsets are centre-minus-vertex, and both sine terms add.
			 * Transform controls before constructing the rim so rounding
			 * cannot move the tire edge differently from the car model. */
			legacy_s16 dx = LEGACY_S16_WRAP_SUB(center_x, controls[point].x);
			legacy_s16 dz = LEGACY_S16_WRAP_SUB(center_z, controls[point].z);
			controls[point].x =
				LEGACY_S16_WRAP_ADD(LEGACY_S16_WRAP_ADD(center_x, multiply_and_scale(dz, sine)),
									multiply_and_scale(dx, cosine));
			controls[point].z =
				LEGACY_S16_WRAP_ADD(LEGACY_S16_WRAP_ADD(center_z, multiply_and_scale(dx, sine)),
									multiply_and_scale(dz, cosine));
		}
	}
	for (legacy_u16 side = 0; side < 2; side++) {
		/* The far rim's control radii can be reversed for face culling.
		 * The tire tread joins corresponding points by translating one rim,
		 * just as the renderer does, not by joining opposite windings. */
		const struct VECTOR *circle = controls;
		for (legacy_u16 i = 0; i < OWOOT_WHEEL_RING_COUNT; i++) {
			struct VECTOR local;
			local.x = ring_coordinate(circle[0].x, circle[1].x, circle[2].x, i);
			local.y = ring_coordinate(circle[0].y, circle[1].y, circle[2].y, i);
			local.z = ring_coordinate(circle[0].z, circle[1].z, circle[2].z, i);
			if (side != 0) {
				local.x += controls[3].x - controls[0].x;
				local.y += controls[3].y - controls[0].y;
				local.z += controls[3].z - controls[0].z;
			}
			local.y -= (legacy_s16)(car->car_suspension_deflection[wheel] / OWOOT_POSITION_SCALE);
			struct VECTOR *point = &output[side * OWOOT_WHEEL_RING_COUNT + i];
			mat_mul_vector(&local, &rotation, point);
			point->x += position_to_word(car->car_position.lx);
			point->y += position_to_word(car->car_position.ly);
			point->z += position_to_word(car->car_position.lz);
		}
	}
	return OWOOT_WHEEL_VERTEX_COUNT;
}
