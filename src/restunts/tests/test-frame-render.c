#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define plane_signed_distance frame_test_plane_distance
#define subst_hillroad_track frame_test_subst_hillroad_track
#define transform_wheel_travel_to_world frame_test_transform_wheel_travel
#include "../c/frame.c"

#undef printf

struct TRACKOBJECT trkObjectList[215];
legacy_s16 camera_track_height_offset;
legacy_u8 supersight_enabled;

static legacy_u64 trace_hash = UINT64_C(1469598103934665603);
static legacy_s16 fixture_plane_distance;
static legacy_s16 transform_stop_at;
static legacy_s16 transform_count;
static legacy_s16 transform_capacity;
static legacy_s16 queue_resets;
static legacy_s16 check_retry_brake_paint;
static legacy_s16 rejected_shape;
static struct VECTOR flag_vertices[4];
static legacy_u8 terrain_map[900];
static legacy_u8 element_map[900];
static legacy_u8 sign_map[900];

static struct CARSTATE ghost_fixture;
static struct SIMD ghost_simd_fixture;
static struct GHOST_CAMERA_STATE ghost_camera_fixture;
static legacy_s16 cockpit_effect_frame;
static legacy_s16 cockpit_effect_kind;
static legacy_s16 ghost_fixture_active;
static legacy_s16 ghost_wheel_updates;

#if defined(RESTUNTS_SDL3)
legacy_u16 frame_callback_count;
legacy_s8 *lookahead_tiles_tables[8];

struct FRAME_SHADOW_FIXTURE {
	struct VECTOR position;
	legacy_s16 heading, half_width, half_length;
	const struct SHAPE3D *model;
};
static struct FRAME_SHADOW_FIXTURE shadow_cars[2];
static struct VECTOR shadow_camera;
static legacy_s16 shadow_count, shadow_begin_count, shadow_projection_count;
static legacy_s16 shadow_frame_active;

void shape3d_hires_shadows_begin(const struct VECTOR *camera_position)
{
	assert(shadow_projection_count == 1);
	shadow_camera = *camera_position;
	shadow_count = 0;
	shadow_begin_count++;
}

void shape3d_hires_shadow_car(const struct VECTOR *position, legacy_s16 heading,
							  legacy_s16 half_width, legacy_s16 half_length)
{
	assert(shadow_begin_count == 1 && shadow_count < 2);
	shadow_cars[shadow_count++] =
		(struct FRAME_SHADOW_FIXTURE){*position, heading, half_width, half_length, NULL};
}

void shape3d_hires_shadow_model(const struct SHAPE3D *shape)
{
	assert(shadow_count > 0 && shadow_count <= 2);
	assert(shadow_cars[shadow_count - 1].model == NULL);
	shadow_cars[shadow_count - 1].model = shape;
}

legacy_u16 select_cliprect_rotate(legacy_s16 roll, legacy_s16 pitch, legacy_s16 yaw,
								  struct RECTANGLE *cliprect, legacy_s16 half_scale)
{
	(void)cliprect;
	(void)half_scale;
	mat_temp = *mat_rot_zxy(roll, pitch, yaw, MATRIX_ROTATION_ORDER_YXZ);
	shadow_projection_count++;
	return 0;
}

void polyinfo_set_supersight(legacy_u8 enabled)
{
	(void)enabled;
}

void shape3d_render_queued_primitives(void)
{
	assert(shadow_frame_active != 0);
}

legacy_s16 skybox_render(legacy_s16 view_index, struct RECTANGLE *clip, legacy_s16 direction,
						 struct MATRIX *rotation, legacy_s16 roll, legacy_s16 angle,
						 legacy_s16 camera_y)
{
	(void)view_index;
	(void)clip;
	(void)direction;
	(void)rotation;
	(void)roll;
	(void)angle;
	(void)camera_y;
	return 0;
}

struct RECTANGLE *draw_ingame_text(void)
{
	return &empty_rect;
}

void format_frame_as_string(legacy_s8 *destination, legacy_s16 frames, legacy_s16 hundredths)
{
	(void)destination;
	(void)frames;
	(void)hundredths;
	assert(0);
}

struct RECTANGLE *intro_draw_text(legacy_s8 *text, legacy_s16 x, legacy_s16 y, legacy_s16 color,
								  legacy_s16 shadow_color)
{
	(void)text;
	(void)x;
	(void)y;
	(void)color;
	(void)shadow_color;
	assert(0);
	return &empty_rect;
}

void shape2d_draw_scaled_transparent_clipped(legacy_s16 scale, struct SHAPE2D far *shape,
											 legacy_s16 x, legacy_s16 y)
{
	(void)scale;
	(void)shape;
	(void)x;
	(void)y;
	assert(0);
}
#endif

struct CARSTATE *ghost_car_state(void)
{
	return ghost_fixture_active != 0 ? &ghost_fixture : 0;
}

const struct GHOST_CAMERA_STATE *ghost_camera_state(void)
{
	return ghost_fixture_active != 0 ? &ghost_camera_fixture : 0;
}

void fatal_error(const legacy_s8 *format, ...)
{
	(void)format;
	assert(0);
}

void sprite_set_target_clip_bounds(legacy_u16 left, legacy_u16 right, legacy_u16 top,
								   legacy_u16 bottom)
{
	assert(left == 0 && right == FRAME_SCREEN_WIDTH);
	assert(top == 0 && bottom == 200);
}

struct RECTANGLE *init_crak(legacy_s16 frame, legacy_s16 top, legacy_s16 height)
{
	static struct RECTANGLE rect;
	assert(top == 0 && height == 200);
	cockpit_effect_frame = frame;
	cockpit_effect_kind = CRASH_EVENT_COLLISION;
	return &rect;
}

struct RECTANGLE *do_sinking(legacy_s16 frame, legacy_s16 top, legacy_s16 height)
{
	struct RECTANGLE *rect = init_crak(frame, top, height);
	cockpit_effect_kind = CRASH_EVENT_WATER;
	return rect;
}

const struct SIMD *ghost_car_simd(void)
{
	return &ghost_simd_fixture;
}

legacy_u8 ghost_car_material(void)
{
	return 2;
}

void shape3d_update_car_wheel_vertices(struct SHAPE3D *shape, legacy_u16 first_vertex,
									   legacy_s16 steering_angle,
									   const legacy_s16 *suspension_offsets,
									   legacy_s16 *cached_wheel_state, struct VECTOR *base_vertices,
									   struct VECTOR *front_wheel_centers)
{
#if defined(RESTUNTS_SDL3)
	if (shadow_frame_active != 0) {
		assert(shape == &game3dshapes[PLAYER_CAR_WHEEL_SHAPE] ||
			   shape == &game3dshapes[OPPONENT_CAR_WHEEL_SHAPE]);
		return;
	}
#endif
	assert(shape == &game3dshapes[127]);
	assert(first_vertex == FRAME_STEERED_WHEEL_FIRST_VERTEX);
	assert(steering_angle == ghost_fixture.car_steeringAngle);
	assert(suspension_offsets == ghost_fixture.car_suspension_deflection);
	assert(cached_wheel_state == opponent_wheel_vertex_state);
	assert(base_vertices == opponent_base_wheel_vertices);
	assert(front_wheel_centers == opponent_front_wheel_centers);
	ghost_wheel_updates++;
}

static void trace_word(legacy_u16 value)
{
	trace_hash = (trace_hash ^ (value & 255U)) * UINT64_C(1099511628211);
	trace_hash = (trace_hash ^ (value >> 8)) * UINT64_C(1099511628211);
}

static void trace_vector(const struct VECTOR *vector)
{
	trace_word(vector->x);
	trace_word(vector->y);
	trace_word(vector->z);
}

void build_track_object(struct VECTOR *first, struct VECTOR *second)
{
	assert(first == second);
	trace_word(1);
	trace_vector(first);
}

legacy_s16 plane_signed_distance(legacy_s16 plane, legacy_s16 x, legacy_s16 y, legacy_s16 z)
{
	trace_word(2);
	trace_word(plane);
	trace_word(x);
	trace_word(y);
	trace_word(z);
	return fixture_plane_distance;
}

void transform_wheel_travel_to_world(void)
{
	trace_word(3);
	trace_vector(&wheel_forward_travel);
	trace_word(planindex_copy);
	trace_word(wheel_heading_offset);
	trace_word(car_initial_pitch);
	trace_word(car_initial_roll);
	trace_word(car_initial_yaw);
	wheel_world_travel.x = -5;
	wheel_world_travel.y = wheel_forward_travel.y;
	wheel_world_travel.z = 7;
}

static void trace_shape(const struct TRANSFORMEDSHAPE3D *shape)
{
	trace_word(shape->shapeptr == 0 ? 65535U : (legacy_u16)(shape->shapeptr - game3dshapes));
	trace_vector(&shape->pos);
	trace_vector(&shape->rotvec);
	trace_word(shape->culling_distance);
	trace_word(shape->ts_flags);
	trace_word(shape->material);
	trace_word(shape->rectptr == &frame_unsorted_shapes_rect);
	trace_word(shape->rectptr == &frame_sorted_shapes_rect);
}

legacy_u16 shape3d_transform_and_queue(struct TRANSFORMEDSHAPE3D *shape)
{
	trace_word(4);
	trace_shape(shape);
	trace_word(backlights_paint_override);
	transform_count++;
	if (check_retry_brake_paint != 0 && transform_count == 1) {
		assert(backlights_paint_override == BACKLIGHT_PAINT_DEFAULT);
	}
	if (transform_count == transform_stop_at ||
		(transform_capacity != 0 && transform_count > transform_capacity)) {
		if (check_retry_brake_paint != 0) {
			backlights_paint_override = BACKLIGHT_PAINT_BRAKING;
		}
		return 1;
	}
	return transform_count == rejected_shape ? 65535U : 0;
}

void shape3d_vertex_read(const struct SHAPE3D *shape, legacy_u16 index, struct VECTOR *destination)
{
	assert(shape == &game3dshapes[111]);
	trace_word(5);
	trace_word(index);
	*destination = flag_vertices[index - FRAME_START_FLAG_FIRST_VERTEX];
}

void shape3d_vertex_write(struct SHAPE3D *shape, legacy_u16 index, const struct VECTOR *source)
{
	assert(shape == &game3dshapes[111]);
	trace_word(6);
	trace_word(index);
	trace_vector(source);
	flag_vertices[index - FRAME_START_FLAG_FIRST_VERTEX] = *source;
}

legacy_u8 subst_hillroad_track(legacy_u8 terrain, legacy_u8 element)
{
	trace_word(7);
	trace_word(terrain);
	trace_word(element);
	return element;
}

void polyinfo_reset(void)
{
	transform_count = 0;
	queue_resets++;
}

static void test_camera_modes(void)
{
	gameconfig.game_opponenttype = 1;
	memset(&state, 0, sizeof(state));
	state.playerstate.car_position.lx = -80000;
	state.playerstate.car_position.ly = 1280;
	state.playerstate.car_position.lz = 96000;
	state.opponentstate.car_position.lx = 120000;
	state.opponentstate.car_position.ly = -640;
	state.opponentstate.car_position.lz = -40000;
	static struct VECTOR track_cameras[] = {{200, -20, 700}, {-400, 40, -300}};
	state.game_follow_camera_position[0] = track_cameras[1];
	state.game_follow_camera_position[1] = track_cameras[0];
	state.game_trackside_camera_index[0] = 0;
	state.game_trackside_camera_index[1] = 1;
	trackside_camera_positions = track_cameras;
	simd_player.car_height = 60;
	simd_opponent.car_height = 90;
	camera_track_height_offset = 25;
	planindex = 2;
	struct FRAME_CAMERA camera;
	for (legacy_u32 scenario = 0; scenario < 64U; scenario++) {
		memset(&camera, 0, sizeof(camera));
		followOpponentFlag = (scenario / 4U) % 2U;
		cameramode = scenario % 4U;
		state.playerstate.car_rotate.x = (legacy_s16)(scenario * 73U);
		state.playerstate.car_rotate.y = (legacy_s16)(scenario * 7U);
		state.playerstate.car_rotate.z = (legacy_s16)(scenario % 4U);
		state.opponentstate.car_rotate.x = (legacy_s16)(-300 + scenario * 37U);
		state.opponentstate.car_rotate.y = -20;
		state.opponentstate.car_rotate.z = (legacy_s16)(1023 - scenario % 4U);
		custom_camera.distance = 900 + scenario;
		custom_camera.elevation_angle = 50 + scenario;
		custom_camera.azimuth_angle = -20 + scenario;
		terrainHeight = scenario % 2U == 0 ? -100 : 600;
		track_wall_collision_enabled = (scenario / 8U) % 2U;
		fixture_plane_distance = (scenario / 16U) * 10;
		frame_setup_camera(&camera);
		trace_vector(&camera.position);
		trace_word(camera.pitch);
		trace_word(camera.yaw);
		trace_word(camera.roll);
	}
}

static void test_covered_tiles(void)
{
	struct FRAME_TILE tile;
	memset(&tile, 0, sizeof(tile));
	static const legacy_s8 offsets[] = {-128, -1, 0, 126, 127};
	struct FRAME_LOOKAHEAD_TILE lookahead[24];
	struct FRAME_TILE_SELECTION tiles;
	for (legacy_u32 flag = 0; flag < 5U; flag++) {
		trkObjectList[1].ss_multiTileFlag = flag;
		for (legacy_u32 east = 0; east < 5U; east++) {
			for (legacy_u32 south = 0; south < 5U; south++) {
				memset(&tiles, 0, sizeof(tiles));
				tiles.lookahead = lookahead;
				tile.element = 1;
				tile.east = offsets[east];
				tile.south = offsets[south];
				for (legacy_u32 index = 0; index < 24U; index++) {
					lookahead[index].east = LEGACY_S8_WRAP_ADD(offsets[east], (index % 3U) - 1U);
					lookahead[index].south =
						LEGACY_S8_WRAP_ADD(offsets[south], (index / 3U) % 3U - 1U);
					tiles.markers[index] = index % 3U;
				}
				frame_mark_covered_tiles(&tiles, &tile, 22);
				for (legacy_u32 index = 0; index < 24U; index++) {
					trace_word(tiles.markers[index]);
				}
			}
		}
	}
}

static void reset_shapes(void)
{
	memset(currenttransshape, 0, sizeof(currenttransshape));
	curtransshape_ptr = currenttransshape;
	transformedshape_counter = 0;
	transform_count = 0;
	transform_stop_at = 0;
	rejected_shape = 0;
	backlights_paint_override = BACKLIGHT_PAINT_DEFAULT;
	memset(&mat_temp, 0, sizeof(mat_temp));
	mat_temp.m._11 = mat_temp.m._22 = mat_temp.m._33 = 16384;
}

static void configure_track(void)
{
	memset(terrain_map, 0, sizeof(terrain_map));
	memset(element_map, 0, sizeof(element_map));
	memset(sign_map, 255, sizeof(sign_map));
	track_terrain_map = terrain_map;
	track_element_map = element_map;
	roadside_sign_indices_by_tile = sign_map;
	for (legacy_u32 index = 0; index < 30U; index++) {
		terrainrows[index] = trackrows[index] = index * 30U;
		track_column_centers[index] = index * 1024U + 512U;
		track_row_centers[index] = 30000 - index * 1024U;
		track_column_positions[index] = index * 1024U;
		track_row_positions[index] = 30512 - index * 1024U;
	}
}

static void test_tile_selection(void)
{
	struct FRAME_LOOKAHEAD_TILE lookahead[24];
	struct FRAME_CAMERA camera;
	struct FRAME_TILE_SELECTION tiles;
	for (legacy_u32 scenario = 0; scenario < 30U; scenario++) {
		memset(&camera, 0, sizeof(camera));
		memset(&tiles, 0x35, sizeof(tiles));
		configure_track();
		detail_level = scenario % 5U;
		camera.position.x = scenario % 2U == 0 ? 10240 : -512;
		camera.position.z = 19456;
		state.playerstate.car_position.lx = 10L * 65536L;
		state.playerstate.car_position.lz = 19L * 65536L;
		tiles.lookahead = lookahead;
		for (legacy_u32 index = 0; index < 24U; index++) {
			lookahead[index].east = (legacy_s8)(index % 5U - 2U);
			lookahead[index].south = (legacy_s8)(index / 5U - 2U);
			lookahead[index].detail = index % 3U;
		}
		trkObjectList[1].ss_multiTileFlag = scenario % 4U;
		trkObjectList[1].ss_physicalModel = scenario % 2U == 0 ? 64 : 63;
		element_map[10 + 10 * 30] = 1;
		element_map[11 + 10 * 30] = TRACK_TILE_CONTINUATION_EAST;
		element_map[10 + 11 * 30] = TRACK_TILE_CONTINUATION_SOUTH;
		element_map[11 + 11 * 30] = TRACK_TILE_CONTINUATION_SOUTHEAST;
		terrain_map[10 + 10 * 30] = scenario % 3U == 0 ? 7 : 0;
		frame_select_tiles(&tiles, &camera);
		for (legacy_u32 index = 0; index < 24U; index++) {
			trace_word(tiles.markers[index]);
			trace_word(tiles.east[index]);
			trace_word(tiles.south[index]);
			trace_word(tiles.elements[index]);
			trace_word(tiles.terrain[index]);
			trace_word(tiles.detail[index]);
		}
	}
}

static void test_terrain_exhaustion(void)
{
	struct FRAME_CAMERA camera;
	memset(&camera, 0, sizeof(camera));
	camera.position.x = -100;
	camera.position.y = 250;
	camera.position.z = 400;
	struct FRAME_TILE tile;
	for (legacy_u32 scenario = 0; scenario < 24U; scenario++) {
		configure_track();
		reset_shapes();
		memset(&tile, 0, sizeof(tile));
		tile.east = 4;
		tile.south = 5;
		tile.element = scenario % 3U == 0 ? 105 : 0;
		tile.terrain = scenario % 3U == 1 ? TERRAIN_RAISED_TILE : 1;
		terrain_map[4 + 5 * 30] = 1;
		terrain_map[5 + 5 * 30] = 2;
		terrain_map[4 + 6 * 30] = 0;
		terrain_map[5 + 6 * 30] = 3;
		transform_stop_at = scenario / 3U;
		trace_word(frame_draw_terrain(&tile, &camera, 8));
		trace_word(tile.terrain);
		trace_word(tile.height);
		trace_word(tile.last_east);
		trace_word(tile.last_south);
	}
}

static void test_sorted_shapes(void)
{
	struct FRAME_CAR_RENDER cars[2];
	for (legacy_u32 scenario = 0; scenario < 36U; scenario++) {
		reset_shapes();
		memset(cars, 0, sizeof(cars));
		state.playerstate.car_is_braking = scenario % 2U;
		state.opponentstate.car_is_braking = (scenario / 2U) % 2U;
		state.playerstate.car_crashBmpFlag = CRASH_EVENT_COLLISION;
		state.opponentstate.car_crashBmpFlag = scenario % 2U == 0 ? CRASH_EVENT_COLLISION : 0;
		transformedshape_counter = scenario % 5U;
		for (legacy_u32 index = 0; index < 5U; index++) {
			currenttransshape[index].shapeptr = &game3dshapes[index];
			transformed_shape_sort_types[index] = index % 4U;
			transformedshape_indices[index] = index;
			transformedshape_zarray[index] = (legacy_s16)((index * 7U + scenario) % 4U);
		}
		transform_stop_at = (scenario / 5U) % 4U;
		rejected_shape = scenario % 4U;
		frame_draw_sorted_shapes(cars);
		trace_word(cars[0].explosion_visible);
		trace_word(cars[1].explosion_visible);
	}
}

static void test_track_elements_and_flags(void)
{
	struct FRAME_CAMERA camera;
	memset(&camera, 0, sizeof(camera));
	camera.position.x = 100;
	camera.position.y = 200;
	camera.position.z = -100;
	struct FRAME_TILE tile;
	struct FRAME_CAR_RENDER cars[2];
	legacy_s8 overlay;
	for (legacy_u32 scenario = 0; scenario < 32U; scenario++) {
		configure_track();
		reset_shapes();
		memset(&tile, 0, sizeof(tile));
		memset(cars, 0, sizeof(cars));
		tile.east = 4;
		tile.south = 5;
		tile.element = scenario % 8U == 0 ? 0 : 1;
		tile.detail = scenario % 2U;
		tile.height = scenario % 3U == 0 ? 450 : 0;
		trkObjectList[1].ss_shapePtr = &game3dshapes[1];
		trkObjectList[1].ss_loShapePtr = &game3dshapes[2];
		trkObjectList[1].ss_multiTileFlag = scenario % 4U;
		trkObjectList[1].ss_ignoreZBias = (scenario / 4U) % 2U;
		trkObjectList[1].ss_surfaceType = scenario % 2U == 0 ? -1 : 2;
		trkObjectList[1].ss_ssOvelay = 0;
		trkObjectList[1].ss_rotY = scenario * 31U;
		start_finish_column = 4;
		start_finish_row = scenario % 2U == 0 ? 5 : 6;
		overlay = scenario % 2U;
		cars[0].depth_adjustment = cars[1].depth_adjustment = 2048;
		transform_stop_at = (scenario / 8U) % 3U;
		trace_word(frame_add_track_element(&tile, &camera, cars, 8, 3, &overlay));
		trace_vector(&tile.position);
		trace_word(tile.last_east);
		trace_word(tile.last_south);
		trace_word(tile.depth_mask);
		trace_word(overlay);
		trace_word(cars[0].depth_adjustment);
		trace_word(cars[1].depth_adjustment);
		state.game_inputmode = GAME_INPUT_MODE_WAITING;
		start_flag_animation = scenario * 33U;
		track_angle = scenario * 17U;
		hillFlag = scenario % 2U;
		for (legacy_u32 index = 0; index < 4U; index++) {
			flag_vertices[index].x = index;
			flag_vertices[index].y = index * 30U;
			flag_vertices[index].z = -10;
		}
		frame_add_start_flag(&tile, &camera, 8);
		trace_word(transformedshape_counter);
		for (legacy_u32 index = 0; index < (legacy_u32)transformedshape_counter; index++) {
			trace_shape(&currenttransshape[index]);
			trace_word(transformedshape_zarray[index]);
		}
	}
}

static void test_invalid_multitile_flags(void)
{
	struct FRAME_TILE tile = {0};
	struct FRAME_TILE_SELECTION tiles = {0};
	struct FRAME_CAMERA camera = {0};
	configure_track();
	tile.element = 1;
	tile.height = 450;
	/* Unknown footprint codes must never select an indeterminate offset table. */
	for (legacy_s16 flag = -128; flag <= 127; flag++) {
		if (flag >= FRAME_MULTITILE_NONE && flag <= FRAME_MULTITILE_BOTH) {
			continue;
		}
		reset_shapes();
		trkObjectList[1].ss_multiTileFlag = (legacy_s8)flag;
		assert(frame_draw_fences(&tile, &tiles, &camera, 0) == 0);
		assert(frame_draw_hill_fill(&tile, &trkObjectList[1], 0) == 0);
		assert(transform_count == 0);
	}
}

static void test_ghost_uses_independent_visual_state(void)
{
	struct FRAME_LOOKAHEAD_TILE lookahead[FRAME_LOOKAHEAD_TILE_COUNT] = {{0}};
	struct FRAME_TILE_SELECTION tiles = {0};
	struct FRAME_CAR_RENDER cars[2] = {{0}};
	struct FRAME_CAMERA camera = {0};
	struct FRAME_TILE tile = {0};
	struct GAMESTATE original_state = state;
	gameconfig.game_opponenttype = 0;
	ghost_fixture_active = 1;
	ghost_fixture.car_position.lx = 10L * 65536L;
	ghost_fixture.car_position.lz = 20L * 65536L;
	ghost_fixture.car_steeringAngle = 29;
	ghost_fixture.car_is_braking = 1;
	ghost_fixture.car_crashBmpFlag = CRASH_EVENT_COLLISION;
	cameramode = CAMERA_MODE_COCKPIT;
	followOpponentFlag = 0;
	detail_level = 0;
	slow_video_mgmt_copy = 0;
	lookahead[0].east = 10;
	lookahead[0].south = 9;
	tiles.lookahead = lookahead;
	tiles.count = FRAME_LOOKAHEAD_TILE_COUNT;
	for (legacy_u32 index = 1; index < FRAME_LOOKAHEAD_TILE_COUNT; index++) {
		tiles.markers[index] = FRAME_TILE_UNAVAILABLE_MARKER;
	}
	reset_shapes();
	frame_place_cars(&tiles, cars);
	assert(cars[PLAYER_CAR_INDEX].east == -1);
	assert(cars[OPPONENT_CAR_INDEX].east == 10);
	assert(cars[OPPONENT_CAR_INDEX].south == 9);
	tile.east = tile.last_east = 10;
	tile.south = tile.last_south = 9;
	trkObjectList[FRAME_OPPONENT_SORT_ID].ss_shapePtr = &game3dshapes[OPPONENT_CAR_HIGH_SHAPE];
	trkObjectList[FRAME_OPPONENT_SORT_ID].ss_loShapePtr = &game3dshapes[OPPONENT_CAR_LOW_SHAPE];
	frame_add_tile_cars(&tile, &camera, cars, 0);
	assert(transformedshape_counter == 1);
	assert(ghost_wheel_updates == 1);
	assert(currenttransshape[0].shapeptr == &game3dshapes[OPPONENT_CAR_HIGH_SHAPE]);
	assert((currenttransshape[0].ts_flags & SHAPE3D_GHOST_FLAG) != 0U);
	assert(currenttransshape[0].pos.x == position_to_word(ghost_fixture.car_position.lx));
	frame_draw_sorted_shapes(cars);
	assert(cars[OPPONENT_CAR_INDEX].explosion_visible == 0);
	assert(memcmp(&state, &original_state, sizeof(state)) == 0);
	assert(gameconfig.game_opponenttype == 0);
	ghost_fixture_active = 0;
	frame_place_cars(&tiles, cars);
	assert(cars[OPPONENT_CAR_INDEX].east == -1);
}

static void test_ghost_camera_modes(void)
{
	struct GAMESTATE live_state = {0};
	struct VECTOR track_cameras[] = {{500, 10, -800}, {2500, 60, 3400}};
	trackside_camera_positions = track_cameras;
	live_state.playerstate.car_position.lx = -100000;
	live_state.playerstate.car_position.lz = -60000;
	live_state.opponentstate.car_position.lx = 900000;
	live_state.opponentstate.car_position.lz = 800000;
	live_state.game_follow_camera_position[0] = track_cameras[0];
	live_state.game_follow_camera_position[1] = track_cameras[0];
	live_state.game_frame = 2000;
	ghost_fixture_active = 1;
	ghost_fixture.car_position.lx = 320000;
	ghost_fixture.car_position.ly = 6400;
	ghost_fixture.car_position.lz = 480000;
	ghost_fixture.car_rotate.x = 190;
	ghost_fixture.car_rotate.y = 17;
	ghost_fixture.car_rotate.z = 9;
	ghost_camera_fixture.follow_position = track_cameras[1];
	ghost_camera_fixture.trackside_index = 1;
	ghost_camera_fixture.frame = 120;
	ghost_camera_fixture.crash_frame = 100;
	ghost_simd_fixture.car_height = 82;
	custom_camera.distance = 1100;
	custom_camera.azimuth_angle = 30;
	custom_camera.elevation_angle = 70;
	terrainHeight = -1000;
	track_wall_collision_enabled = 0;
	camera_track_height_offset = 20;
	for (cameramode = CAMERA_MODE_COCKPIT; cameramode < CAMERA_MODE_COUNT; cameramode++) {
		struct FRAME_CAMERA expected = {0};
		struct FRAME_CAMERA actual = {0};
		/* The ghost view must match an opponent at the recorded pose and camera,
		 * regardless of the live player's or unused opponent's positions. */
		state = live_state;
		state.opponentstate = ghost_fixture;
		state.game_follow_camera_position[1] = ghost_camera_fixture.follow_position;
		state.game_trackside_camera_index[1] = ghost_camera_fixture.trackside_index;
		gameconfig.game_opponenttype = 1;
		followOpponentFlag = 1;
		simd_player.car_height = ghost_simd_fixture.car_height;
		frame_setup_camera(&expected);
		state = live_state;
		gameconfig.game_opponenttype = 0;
		simd_player.car_height = 40;
		frame_setup_camera(&actual);
		assert(memcmp(&actual, &expected, sizeof(actual)) == 0);
		assert(memcmp(&state, &live_state, sizeof(state)) == 0);
		/* T back to the player uses the live camera, and a missing ghost
		 * also falls back to that camera instead of the unused AI state. */
		followOpponentFlag = 0;
		frame_setup_camera(&expected);
		ghost_fixture_active = 0;
		followOpponentFlag = 1;
		frame_setup_camera(&actual);
		assert(memcmp(&actual, &expected, sizeof(actual)) == 0);
		ghost_fixture_active = 1;
	}
	cameramode = CAMERA_MODE_COCKPIT;
	slow_video_mgmt_copy = 0;
	struct RECTANGLE cliprect = {0, 320, 0, 200};
	ghost_fixture.car_crashBmpFlag = CRASH_EVENT_COLLISION;
	frame_draw_cockpit_effects(&cliprect);
	assert(cockpit_effect_kind == CRASH_EVENT_COLLISION && cockpit_effect_frame == 20);
	ghost_fixture.car_crashBmpFlag = CRASH_EVENT_WATER;
	frame_draw_cockpit_effects(&cliprect);
	assert(cockpit_effect_kind == CRASH_EVENT_WATER && cockpit_effect_frame == 20);
	assert(memcmp(&state, &live_state, sizeof(state)) == 0);
	ghost_fixture_active = 0;
	followOpponentFlag = 0;
}

/* Speculative collision or water flags must not start irreversible effects.
 * Confirmed events must still be visible on every intervening presentation. */
static void test_prediction_uses_authoritative_events(void)
{
	struct FRAME_LOOKAHEAD_TILE lookahead = {11, 9, 0};
	struct FRAME_TILE_SELECTION tiles = {0};
	struct FRAME_CAMERA camera = {0};
	struct FRAME_TILE tile = {0};
	struct RECTANGLE cliprect = {0, 320, 0, 200};
	tiles.lookahead = &lookahead;
	tiles.count = 1;
	tile.east = tile.last_east = 11;
	tile.south = tile.last_south = 9;
	tile.detail = 1;
	memset(&simd_player, 0, sizeof(simd_player));
	memset(&simd_opponent, 0, sizeof(simd_opponent));
	memset(&ghost_simd_fixture, 0, sizeof(ghost_simd_fixture));
	trkObjectList[FRAME_PLAYER_SORT_ID].ss_loShapePtr = &game3dshapes[PLAYER_CAR_LOW_SHAPE];
	trkObjectList[FRAME_OPPONENT_SORT_ID].ss_loShapePtr = &game3dshapes[OPPONENT_CAR_LOW_SHAPE];
	slow_video_mgmt_copy = 0;
	for (legacy_s16 viewed = 0; viewed < 3; viewed++) {
		for (legacy_s16 real_event = CRASH_EVENT_NONE; real_event <= CRASH_EVENT_WATER;
			 real_event++) {
			for (legacy_s16 phantom_event = CRASH_EVENT_NONE; phantom_event <= CRASH_EVENT_WATER;
				 phantom_event++) {
				memset(&state, 0, sizeof(state));
				memset(&ghost_fixture, 0, sizeof(ghost_fixture));
				state.game_frame = 2000;
				state.game_pEndFrame = 1980;
				state.game_oEndFrame = 1960;
				ghost_camera_fixture.frame = 120;
				ghost_camera_fixture.crash_frame = 100;
				gameconfig.game_opponenttype = viewed == 1;
				ghost_fixture_active = viewed == 2;
				followOpponentFlag = viewed != 0;
				legacy_s16 car_index = viewed == 0 ? PLAYER_CAR_INDEX : OPPONENT_CAR_INDEX;
				struct CARSTATE *real_car = viewed == 0	  ? &state.playerstate
											: viewed == 1 ? &state.opponentstate
														  : &ghost_fixture;
				real_car->car_crashBmpFlag = real_event;
				real_car->car_position.lx = 10L * 65536L;
				real_car->car_position.lz = 20L * 65536L;
				struct GAMESTATE real_state = state;
				struct CARSTATE real_ghost = ghost_fixture;
				struct GHOST_CAMERA_STATE real_ghost_camera = ghost_camera_fixture;
				struct GAMESTATE phantom = state;
				struct CARSTATE phantom_ghost = ghost_fixture;
				struct GHOST_CAMERA_STATE phantom_ghost_camera = ghost_camera_fixture;
				phantom.game_pEndFrame = phantom.game_oEndFrame = 1999;
				phantom_ghost_camera.frame = 999;
				phantom_ghost_camera.crash_frame = 119;
				struct CARSTATE *phantom_car = viewed == 0	 ? &phantom.playerstate
											   : viewed == 1 ? &phantom.opponentstate
															 : &phantom_ghost;
				phantom_car->car_crashBmpFlag = phantom_event;
				phantom_car->car_position.lx = 11L * 65536L;
				struct GAMESTATE presentation = phantom;
				struct CARSTATE presentation_ghost = phantom_ghost;
				struct GHOST_CAMERA_STATE presentation_ghost_camera = phantom_ghost_camera;
				frame_preserve_authoritative_events(&presentation, &presentation_ghost,
													&presentation_ghost_camera);
				frame_state = &presentation;
				frame_ghost = ghost_fixture_active != 0 ? &presentation_ghost : 0;
				frame_ghost_camera = ghost_fixture_active != 0 ? &presentation_ghost_camera : 0;
				frame_uses_snapshot = 1;

				cameramode = CAMERA_MODE_COCKPIT;
				cockpit_effect_kind = cockpit_effect_frame = -1;
				frame_draw_cockpit_effects(&cliprect);
				if (real_event == CRASH_EVENT_NONE) {
					assert(cockpit_effect_kind == -1 && cockpit_effect_frame == -1);
				} else {
					assert(cockpit_effect_kind == real_event);
					assert(cockpit_effect_frame == (viewed == 1 ? 40 : 20));
				}

				cameramode = CAMERA_MODE_FOLLOW;
				struct FRAME_CAR_RENDER cars[2] = {{0}};
				reset_shapes();
				frame_place_cars(&tiles, cars);
				if (real_event == CRASH_EVENT_WATER) {
					assert(cars[car_index].east == -1);
				} else {
					assert(cars[car_index].east == 11 && cars[car_index].south == 9);
					cars[1 - car_index].east = -1;
					frame_add_tile_cars(&tile, &camera, cars, 0);
					assert(transformedshape_counter == 1);
					assert(currenttransshape[0].pos.x ==
						   position_to_word(phantom_car->car_position.lx));
					legacy_s8 expected_flags = real_event == CRASH_EVENT_COLLISION
												   ? FRAME_TRANSFORM_FLAGS_CLIPPED
												   : FRAME_TRANSFORM_FLAGS_DEFAULT;
					if (viewed == 2) {
						expected_flags |= SHAPE3D_GHOST_FLAG;
					}
					assert(currenttransshape[0].ts_flags == expected_flags);
					frame_draw_sorted_shapes(cars);
					assert(cars[car_index].explosion_visible ==
						   (real_event == CRASH_EVENT_COLLISION && viewed != 2));
				}
				assert(memcmp(&state, &real_state, sizeof(state)) == 0);
				assert(memcmp(&ghost_fixture, &real_ghost, sizeof(ghost_fixture)) == 0);
				assert(memcmp(&ghost_camera_fixture, &real_ghost_camera,
							  sizeof(ghost_camera_fixture)) == 0);
				assert(phantom_car->car_crashBmpFlag == phantom_event);
				assert(phantom.game_pEndFrame == 1999 && phantom.game_oEndFrame == 1999);
				assert(phantom_ghost_camera.frame == 999 &&
					   phantom_ghost_camera.crash_frame == 119);
				frame_uses_snapshot = 0;
				frame_state = &state;
				frame_ghost = 0;
				frame_ghost_camera = 0;
			}
		}
	}
	ghost_fixture_active = 0;
	followOpponentFlag = 0;
}

#if !defined(RESTUNTS_SDL3)
static void test_supersight_selection(void)
{
	/* The eight original headings select four cardinal rotations and their
	 * mirrored painter order. Every one must cover the same 110 unique cells. */
	static const struct FRAME_LOOKAHEAD_TILE headings[] = {{2, -4, 2}, {-2, -4, 2}, {4, -2, 2},
														   {4, 2, 2},  {-2, 4, 2},	{2, 4, 2},
														   {-4, 2, 2}, {-4, -2, 2}};
	struct FRAME_CAMERA camera = {0};
	camera.position.x = 15 * 1024;
	camera.position.z = 14 * 1024;
	supersight_enabled = 1;
	for (legacy_u32 heading = 0; heading < 8; heading++) {
		for (detail_level = 0; detail_level < 5; detail_level++) {
			struct FRAME_TILE_SELECTION tiles = {0};
			configure_track();
			tiles.lookahead = &headings[heading];
			frame_select_tiles(&tiles, &camera);
			assert(tiles.count == FRAME_SUPERSIGHT_TILE_COUNT);
			for (legacy_u32 i = 0; i < FRAME_SUPERSIGHT_TILE_COUNT; i++) {
				assert(tiles.markers[i] == FRAME_TILE_DRAW_MARKER);
				assert(tiles.lookahead[i].detail == supersight_tiles[i].priority);
				for (legacy_u32 j = 0; j < i; j++) {
					assert(tiles.east[i] != tiles.east[j] || tiles.south[i] != tiles.south[j]);
				}
			}
			assert(tiles.east[109] == 15 && tiles.south[109] == 15);
		}
	}
	/* Expanded view still respects the scenery option and map boundaries. */
	for (legacy_u32 corner = 0; corner < 4; corner++) {
		struct FRAME_TILE_SELECTION tiles = {0};
		camera.position.x = corner & 1 ? 29 * 1024 : 0;
		camera.position.z = corner & 2 ? 29 * 1024 : 0;
		tiles.lookahead = &headings[corner * 2];
		frame_select_tiles(&tiles, &camera);
		for (legacy_u32 i = 0; i < FRAME_SUPERSIGHT_TILE_COUNT; i++) {
			if (tiles.markers[i] == FRAME_TILE_DRAW_MARKER) {
				assert(tiles.east[i] >= 0 && tiles.east[i] <= 29);
				assert(tiles.south[i] >= 0 && tiles.south[i] <= 29);
			}
		}
	}
	camera.position.x = 15 * 1024;
	camera.position.z = 14 * 1024;
	for (detail_level = 0; detail_level < 5; detail_level++) {
		struct FRAME_TILE_SELECTION tiles = {0};
		configure_track();
		element_map[15 + 14 * 30] = 1;
		trkObjectList[1].ss_multiTileFlag = 0;
		trkObjectList[1].ss_physicalModel = FRAME_SCENERY_PHYSICAL_MODEL_FIRST;
		state.playerstate.car_position.lx = 15L * 65536L;
		state.playerstate.car_position.lz = 14L * 65536L;
		tiles.lookahead = &headings[0];
		frame_select_tiles(&tiles, &camera);
		assert(tiles.elements[107] == (detail_level == 0 ? 1 : 0));
	}
	supersight_enabled = 0;
}

static void test_supersight_capacity_retries(void)
{
	struct FRAME_CAMERA camera = {0};
	struct FRAME_TILE_SELECTION tiles = {0};
	struct FRAME_CAR_RENDER cars[2] = {{0}};
	static const struct FRAME_LOOKAHEAD_TILE north = {2, -4, 2};
	configure_track();
	reset_shapes();
	memset(terrain_map, 1, sizeof(terrain_map));
	memset(&state, 0, sizeof(state));
	camera.position.x = 15 * 1024;
	camera.position.z = 14 * 1024;
	tiles.lookahead = &north;
	detail_level = 1;
	cameramode = CAMERA_MODE_COCKPIT;
	followOpponentFlag = 0;
	gameconfig.game_opponenttype = 0;
	supersight_enabled = 1;
	frame_select_tiles(&tiles, &camera);
	check_retry_brake_paint = 1;
	frame_supersight_reset();
	transform_capacity = 35;
	queue_resets = 0;
	frame_draw_supersight(&tiles, &camera, cars, 0, 0, 0);
	assert(queue_resets == 5);
	assert(tiles.first == 80);
	assert(transform_count == 30);
	/* Same-view frames skip failed levels, including paused frame zero. A full
	 * probe recovers quality within sixteen presented frames without relying
	 * on advancing simulation time. */
	transform_capacity = 200;
	for (legacy_u32 frame = 1; frame <= FRAME_SUPERSIGHT_PROBE_INTERVAL; frame++) {
		polyinfo_reset();
		legacy_s16 resets = queue_resets;
		tiles.lookahead = &north;
		frame_select_tiles(&tiles, &camera);
		frame_draw_supersight(&tiles, &camera, cars, 0, 0, 0);
		assert(queue_resets == resets);
		assert(tiles.first == (frame < FRAME_SUPERSIGHT_PROBE_INTERVAL ? 80 : 0));
	}
	assert(transform_count == 110);
	/* Camera changes and explicit replay seeks recover on their very next frame. */
	supersight_attempt_hint = 5;
	camera.yaw = 128;
	assert(frame_supersight_first_attempt(&tiles, &camera) == 0);
	supersight_attempt_hint = 5;
	select_rect_rc.right++;
	assert(frame_supersight_first_attempt(&tiles, &camera) == 0);
	supersight_attempt_hint = 5;
	frame_supersight_reset();
	assert(frame_supersight_first_attempt(&tiles, &camera) == 0);
	transform_capacity = 1;
	polyinfo_reset();
	tiles.lookahead = &north;
	frame_select_tiles(&tiles, &camera);
	frame_draw_supersight(&tiles, &camera, cars, 0, 0, 0);
	assert(tiles.first == 106);
	assert(transform_count == 2);
	supersight_enabled = 0;
	transform_capacity = 0;
	check_retry_brake_paint = 0;
}

#else
static void test_supersight_selection(void)
{
	struct FRAME_CAMERA camera = {0};
	supersight_enabled = 1;
	for (legacy_s16 pose = 0; pose < 24; pose++) {
		configure_track();
		camera.position.x = pose & 1 ? 0 : 29 * 1024;
		camera.position.z = pose & 2 ? 0 : 29 * 1024;
		camera.position.y = 2000;
		mat_temp = *mat_rot_zxy(pose * 43, pose * 73, pose * 127, MATRIX_ROTATION_ORDER_ZXY);
		for (detail_level = 0; detail_level < 5; detail_level++) {
			struct FRAME_TILE_SELECTION tiles = {0};
			legacy_u8 seen[900] = {0};
			frame_select_tiles(&tiles, &camera);
			assert(tiles.count == 900 && tiles.first == 0);
			legacy_s32 previous_depth = INT32_MAX;
			for (legacy_s16 i = 0; i < tiles.count; i++) {
				assert(tiles.markers[i] == FRAME_TILE_DRAW_MARKER);
				assert(tiles.detail[i] == FRAME_TILE_DETAIL_FULL);
				assert(tiles.east[i] >= 0 && tiles.east[i] < 30);
				assert(tiles.south[i] >= 0 && tiles.south[i] < 30);
				assert(seen[tiles.south[i] * 30 + tiles.east[i]]++ == 0);
				legacy_s32 x = (legacy_s32)track_column_centers[tiles.east[i]] - camera.position.x;
				legacy_s32 z = (legacy_s32)track_row_centers[tiles.south[i]] - camera.position.z;
				legacy_s32 depth =
					(legacy_s32)(((legacy_s64)x * mat_temp.m._31 + (legacy_s64)z * mat_temp.m._33) /
								 TRIG_FIXED_ONE);
				assert(depth <= previous_depth);
				previous_depth = depth;
			}
		}
	}
	/* A distant multi-tile object is resolved once, even when a continuation
	 * is closer than its owner. Scenery remains an explicit graphics option. */
	for (detail_level = 0; detail_level < 5; detail_level++) {
		configure_track();
		struct FRAME_TILE_SELECTION tiles = {0};
		element_map[0] = 1;
		element_map[1] = TRACK_TILE_CONTINUATION_EAST;
		element_map[30] = TRACK_TILE_CONTINUATION_SOUTH;
		element_map[31] = TRACK_TILE_CONTINUATION_SOUTHEAST;
		trkObjectList[1].ss_multiTileFlag = FRAME_MULTITILE_BOTH;
		trkObjectList[1].ss_physicalModel = 0;
		element_map[29] = 2;
		trkObjectList[2].ss_multiTileFlag = 0;
		trkObjectList[2].ss_physicalModel = FRAME_SCENERY_PHYSICAL_MODEL_FIRST;
		state.playerstate.car_position.lx = 15L * 65536;
		state.playerstate.car_position.lz = 15L * 65536;
		frame_select_tiles(&tiles, &camera);
		legacy_s16 objects = 0, covered = 0, scenery = 0;
		for (legacy_s16 i = 0; i < tiles.count; i++) {
			if (tiles.markers[i] == FRAME_TILE_MULTITILE_COVERED_MARKER) {
				covered++;
			} else if (tiles.markers[i] == FRAME_TILE_DRAW_MARKER) {
				objects += tiles.elements[i] == 1;
				scenery += tiles.elements[i] == 2;
			}
		}
		assert(objects == 1 && covered == 3);
		assert(scenery == (detail_level == 0));
	}
	supersight_enabled = 0;
}

static void test_supersight_capacity_retries(void)
{
	struct FRAME_CAMERA camera = {0};
	struct FRAME_TILE_SELECTION tiles = {0};
	struct FRAME_CAR_RENDER cars[2] = {{0}};
	configure_track();
	reset_shapes();
	memset(terrain_map, 1, sizeof(terrain_map));
	memset(&state, 0, sizeof(state));
	state.playerstate.car_position.lx = 15L * 65536;
	state.playerstate.car_position.lz = 15L * 65536;
	detail_level = 1;
	cameramode = CAMERA_MODE_COCKPIT;
	followOpponentFlag = 0;
	gameconfig.game_opponenttype = 0;
	start_finish_column = -1;
	supersight_enabled = 1;
	frame_select_tiles(&tiles, &camera);
	transform_capacity = 0;
	queue_resets = 0;
	frame_draw_supersight(&tiles, &camera, cars, 0, 0, 0);
	assert(queue_resets == 0 && tiles.first == 0);
	assert(transform_count == 900);
	/* Full diagonal depths and signed biases must preserve far-to-near order. */
	reset_shapes();
	mat_temp = *mat_rot_zxy(0, 0, 128, MATRIX_ROTATION_ORDER_ZXY);
	curtransshape_ptr = currenttransshape;
	for (legacy_s16 i = 0; i < 3; i++) {
		curtransshape_ptr->pos.x = -29000 + i * 1000;
		curtransshape_ptr->pos.y = 0;
		curtransshape_ptr->pos.z = 29000 - i * 1000;
		curtransshape_ptr->shapeptr = &game3dshapes[0];
		transformed_shape_add_for_sort(i == 0 ? -2048 : 0, 0);
	}
	assert(supersight_shape_depths[1] > 32767);
	frame_draw_sorted_shapes(cars);
	for (legacy_s16 i = 1; i < 3; i++) {
		assert(supersight_shape_depths[transformedshape_indices[i - 1]] >=
			   supersight_shape_depths[transformedshape_indices[i]]);
	}
	supersight_enabled = 0;
}
#endif

#if defined(RESTUNTS_SDL3)
static void test_supersight_car_shadows(void)
{
	struct RECTANGLE cliprect = {0, 320, 0, 200};
	static struct FRAME_LOOKAHEAD_TILE lookahead[24];
	lookahead_tiles_tables[0] = (legacy_s8 *)lookahead;
	configure_track();
	memset(&state, 0, sizeof(state));
	memset(&simd_player, 0, sizeof(simd_player));
	memset(&simd_opponent, 0, sizeof(simd_opponent));
	state.playerstate.car_position = (struct VECTORLONG){640000, 6400, 700000};
	state.opponentstate.car_position = (struct VECTORLONG){660000, 9600, 720000};
	state.playerstate.car_rotate.x = 179;
	state.opponentstate.car_rotate.x = -321;
	state.game_follow_camera_position[0] = (struct VECTOR){9500, 500, 10000};
	state.game_follow_camera_position[1] = (struct VECTOR){9600, 600, 10100};
	simd_player.car_height = simd_opponent.car_height = 40;
	simd_player.collide_points[0].px = 47;
	simd_player.collide_points[1].px = 96;
	simd_opponent.collide_points[0].px = 39;
	simd_opponent.collide_points[1].px = 81;
	ghost_fixture = state.opponentstate;
	ghost_fixture.car_position.lx += 10000;
	ghost_camera_fixture.follow_position = (struct VECTOR){9000, 450, 10300};
	ghost_simd_fixture.car_height = 40;
	trkObjectList[FRAME_PLAYER_SORT_ID].ss_shapePtr = &game3dshapes[PLAYER_CAR_WHEEL_SHAPE];
	trkObjectList[FRAME_OPPONENT_SORT_ID].ss_shapePtr = &game3dshapes[OPPONENT_CAR_WHEEL_SHAPE];
	trkObjectList[FRAME_PLAYER_SORT_ID].ss_loShapePtr = &game3dshapes[PLAYER_CAR_LOW_SHAPE];
	trkObjectList[FRAME_OPPONENT_SORT_ID].ss_loShapePtr = &game3dshapes[OPPONENT_CAR_LOW_SHAPE];
	start_finish_column = -1;
	detail_level = 1;
	slow_video_mgmt_copy = 0;
	game_replay_mode = REPLAY_MODE_PAUSED;
	terrainHeight = 0;
	track_wall_collision_enabled = 0;
	shadow_frame_active = 1;
	/* Exercise the actual frame entry point, including T into an AI or ghost
	 * cockpit, with both a live pose and a separate interpolated snapshot. */
	for (legacy_s16 snapshot = 0; snapshot < 2; snapshot++) {
		struct GAMESTATE presentation = state;
		presentation.playerstate.car_position.lx += 640;
		frame_state = snapshot != 0 ? &presentation : &state;
		frame_uses_snapshot = snapshot;
		frame_ghost = &ghost_fixture;
		frame_ghost_camera = &ghost_camera_fixture;
		for (legacy_s16 scenario = 0; scenario < 48; scenario++) {
			supersight_enabled = scenario & 1;
			followOpponentFlag = (scenario >> 1) & 1;
			cameramode = (scenario & 4) != 0 ? CAMERA_MODE_COCKPIT : CAMERA_MODE_FOLLOW;
			gameconfig.game_opponenttype = (scenario >> 3) & 1;
			ghost_fixture_active = gameconfig.game_opponenttype == 0;
			state.playerstate.car_crashBmpFlag = presentation.playerstate.car_crashBmpFlag =
				scenario >= 32 ? CRASH_EVENT_WATER : CRASH_EVENT_NONE;
			state.opponentstate.car_crashBmpFlag = presentation.opponentstate.car_crashBmpFlag =
				scenario >= 16 && scenario < 32 ? CRASH_EVENT_WATER : CRASH_EVENT_NONE;
			reset_shapes();
			shadow_count = shadow_begin_count = shadow_projection_count = 0;
			update_frame(0, &cliprect);
			assert(shadow_begin_count == supersight_enabled);
			legacy_s16 expected_count = 0;
			for (legacy_s16 car_index = 0; car_index < 2; car_index++) {
				const struct CARSTATE *car =
					car_index == 0 ? &frame_state->playerstate : &frame_state->opponentstate;
				const struct SIMD *simd = car_index == 0 ? &simd_player : &simd_opponent;
				if (supersight_enabled == 0 ||
					(car_index == 1 && gameconfig.game_opponenttype == 0) ||
					car->car_crashBmpFlag == CRASH_EVENT_WATER ||
					(cameramode == CAMERA_MODE_COCKPIT && car_index == followOpponentFlag)) {
					continue;
				}
				assert(expected_count < shadow_count);
				const struct FRAME_SHADOW_FIXTURE *shadow = &shadow_cars[expected_count++];
				assert(shadow->position.x ==
					   position_to_word(car->car_position.lx) - shadow_camera.x);
				assert(shadow->position.y ==
					   position_to_word(car->car_position.ly) - shadow_camera.y);
				assert(shadow->position.z ==
					   position_to_word(car->car_position.lz) - shadow_camera.z);
				assert(shadow->heading == -car->car_rotate.x);
				assert(shadow->half_width == simd->collide_points[0].px);
				assert(shadow->half_length == simd->collide_points[1].px);
				assert(shadow->model == &game3dshapes[car_index == 0 ? PLAYER_CAR_WHEEL_SHAPE
																	 : OPPONENT_CAR_WHEEL_SHAPE]);
			}
			assert(shadow_count == expected_count);
		}
	}
	frame_uses_snapshot = 0;
	frame_state = &state;
	frame_ghost = 0;
	frame_ghost_camera = 0;
	ghost_fixture_active = 0;
	/* Body, wheel model, ghost and attached debris all reject receiving;
	 * ordinary rendering retains exactly its original flag bits. */
	for (legacy_s16 enabled = 0; enabled < 2; enabled++) {
		for (legacy_s16 ghost = 0; ghost < 2; ghost++) {
			reset_shapes();
			supersight_enabled = enabled;
			state.playerstate.car_crashBmpFlag = CRASH_EVENT_NONE;
			state.game_particles_active = 1;
			state.game_particle_forward_speed[0] = 1;
			state.game_particle_owner[0] = PLAYER_CAR_INDEX;
			state.game_particle_shape_index[0] = 0;
			particle_scene_objects[0].ss_shapePtr = &game3dshapes[116];
			struct VECTOR camera_position = {0};
			struct RECTANGLE crash_rect = {0};
			frame_add_car(&state.playerstate, PLAYER_CAR_INDEX, FRAME_PLAYER_SORT_ID,
						  &game3dshapes[PLAYER_CAR_WHEEL_SHAPE], player_wheel_vertex_state,
						  player_base_wheel_vertices, player_front_wheel_centers,
						  &frame_player_car_rect, &crash_rect, &camera_position, 0,
						  ghost != 0 ? SHAPE3D_GHOST_FLAG : 0, 0, 0);
			assert(transformedshape_counter == (ghost != 0 ? 1 : 2));
			for (legacy_s16 shape = 0; shape < transformedshape_counter; shape++) {
				assert(((currenttransshape[shape].ts_flags & SHAPE3D_NO_SHADOW_RECEIVE_FLAG) !=
						0) == enabled);
			}
			legacy_s16 body = transformedshape_counter - 1;
			assert(currenttransshape[body].shapeptr == &game3dshapes[PLAYER_CAR_WHEEL_SHAPE]);
			assert((currenttransshape[body].ts_flags & ~SHAPE3D_NO_SHADOW_RECEIVE_FLAG) ==
				   (FRAME_TRANSFORM_FLAGS_DEFAULT | (ghost != 0 ? SHAPE3D_GHOST_FLAG : 0)));
		}
	}
	shadow_frame_active = 0;
	supersight_enabled = 0;
	state.game_particles_active = 0;
	followOpponentFlag = 0;
}
#endif

int main(void)
{
	test_camera_modes();
	test_covered_tiles();
	test_tile_selection();
	test_terrain_exhaustion();
	test_sorted_shapes();
	test_track_elements_and_flags();
	/* The model indices match the DOS resource table independently of native
	 * pointer width: camera modes, tile selection, queue exhaustion, sorted
	 * brake paint, component geometry and animated start-flag vertices. */
#ifdef FRAME_RECORD_BASELINE
	printf("Frame rendering fingerprint: %016" LEGACY_PRIx64 "\n", trace_hash);
#else
	assert(trace_hash == UINT64_C(0x1f0a24dd55dcfc37));
#endif
	test_invalid_multitile_flags();
	test_ghost_uses_independent_visual_state();
	test_ghost_camera_modes();
	test_prediction_uses_authoritative_events();
	test_supersight_selection();
	test_supersight_capacity_retries();
#if defined(RESTUNTS_SDL3)
	test_supersight_car_shadows();
#endif
	puts("Frame rendering snapshots, ghost isolation and SuperSight passed.");
	return 0;
}
