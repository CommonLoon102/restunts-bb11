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
									   legacy_s16 steering_angle, legacy_s16 *suspension_offsets,
									   legacy_s16 *cached_wheel_state, struct VECTOR *base_vertices,
									   struct VECTOR *front_wheel_centers)
{
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
	test_ghost_uses_independent_visual_state();
	test_ghost_camera_modes();
	test_supersight_selection();
	test_supersight_capacity_retries();
	puts("Frame rendering snapshots, ghost isolation and SuperSight passed.");
	return 0;
}
