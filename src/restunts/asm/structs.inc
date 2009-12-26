FILE struc
_ptr dd ?
_cnt dw ?
_base dd ?
_flag db ?
_file db ?
ends
MATRIX struc
_11 dw ?
_21 dw ?
_31 dw ?
_12 dw ?
_22 dw ?
_32 dw ?
_13 dw ?
_23 dw ?
_33 dw ?
ends
VECTOR struc
x dw ?
y dw ?
z dw ?
ends
MEMCHUNK struc
resname db 12 dup (?)
ressize dw ?
resofs dw ?
resunk dw ?
ends
VECTORLONG struc
long_x dd ?
long_y dd ?
long_z dd ?
ends
PLANE struc
plane_yz dw ?
plane_xy dw ?
plane_origin VECTOR ?
plane_normal VECTOR ?
plane_rotation MATRIX ?
ends
POINT2D struc
x2 dw ?
y2 dw ?
ends
SIMD struc
num_gears db ?
simd_unk db ?
car_mass dw ?
braging_eff dw ?
idle_rpm dw ?
downshift_rpm dw ?
upshift_rpm dw ?
max_rpm dw ?
gear_ratios dw 7 dup (?)
knob_points POINT2D 7 dup (?)
aero_resistance dw ?
torque_curve db 104 dup (?)
grip dw ?
simd_unk2 db 16 dup (?)
sliding dw ?
surface_grip dw 4 dup (?)
simd_unk3 db 10 dup (?)
collide_points POINT2D 2 dup (?)
car_height dw ?
wheel_coords VECTOR 4 dup (?)
steeringdots db 62 dup (?)
spdcenter POINT2D ?
spdnumpoints dw ?
spdpoints db 208 dup (?)
revcenter POINT2D ?
revnumpoints dw ?
revpoints db 256 dup (?)
aerorestable dw ?
ends
SHAPE3DHEADER struc
header_numverts db ?
header_numprimitives db ?
header_numpaints db ?
header_reserved db ?
ends
CARSTATE struc
car_longvec1 VECTORLONG ?
car_longvec2 VECTORLONG ?
car_rotate VECTOR ?
field_1E dw ?
field_20 dw ?
car_currpm dw ?
car_lastrpm dw ?
car_idlerpm2 dw ?
car_gripdiff dw ?
car_trackgrip dw ?
car_trackgrip2 dw ?
car_lasttrackgrip dw ?
car_gearratio dw ?
car_gearratioshr8 dw ?
car_knob_x dw ?
field_36 dw ?
car_knob_y dw ?
car_knob_x2 dw ?
car_knob_y2 dw ?
car_angle_z dw ?
field_40 dw ?
field_42 dw ?
field_44 dw ?
car_surfacegrip_sum dw ?
field_48 dw ?
car_trackdata3_index dw ?
car_rc1 dw 4 dup (?)
car_rc2 dw 4 dup (?)
car_rc3 dw 4 dup (?)
car_rc4 dw 4 dup (?)
car_rc5 dw 4 dup (?)
car_vec_unk VECTOR ?
field_7A dw ?
field_7C dw ?
field_7E dw ?
field_80 dw ?
field_82 dw ?
field_84 dw ?
field_86 dw ?
field_88 dw ?
field_8A dw ?
car_vec_unk2 VECTOR 4 dup (?)
car_vec_unk3 VECTOR ?
car_vec_unk4 VECTOR ?
car_vec_unk5 VECTOR ?
field_B6 dw ?
field_B8 dw ?
field_BA dw ?
car_is_braking db ?
car_is_accelerating db ?
car_current_gear db ?
field_BF db ?
field_C0 db ?
field_C1 db ?
car_surfacegrip_idx1 db ?
car_surfacegrip_idx2 db ?
car_surfacegrip_idx3 db ?
car_surfacegrip_idx4 db ?
field_C6 db ?
field_C7 db ?
field_C8 db ?
field_C9 db ?
car_changing_gear db ?
car_fpsmul2 db ?
car_transmission db ?
field_CD db ?
field_CE db ?
field_CF db ?
ends
SHAPE3D struc
shape3d_numverts dw ?
shape3d_verts dd ?
shape3d_numprimitives dw ?
shape3d_numpaints dw ?
shape3d_primitives dd ?
shape3d_cull1 dd ?
shape3d_cull2 dd ?
ends
PRIMITIVE struc
primitive_type db ?
primitive_flags db ?
ends
SCENESHAPE struc
scene_unk dw ?
scene_rot_y dw ?
scene_shapeptr dw ?
scene_loshapeptr dw ?
scene_unk2 db ?
scene_material db ?
scene_unk3 db ?
scene_translateflag db ?
scene_unk4 db ?
scene_unk5 db ?
ends
TRANSFORMEDSHAPE struc
ts_pos VECTOR ?
ts_shapeptr dw ?
ts_rectptr dw ?
ts_rotvec VECTOR ?
ts_unk dw ?
ts_flags db ?
ts_material db ?
ends
SHAPEINFO struc
si_unk1 db 2 dup (?)
si_unk2 dw ?
si_unk3 dw ?
si_unk4 dw ?
si_unk5 dw ?
si_unk6 dw ?
si_unk7 dw ?
ends
SHAPE2D struc
s2d_width dw ?
s2d_height dw ?
s2d_unk1 dw ?
s2d_unk2 dw ?
s2d_pos_x dw ?
s2d_pos_y dw ?
s2d_unk3 db ?
s2d_unk4 db ?
s2d_unk5 db ?
s2d_unk6 db ?
ends
GAMESTATE struc
game_longvecs1 VECTORLONG 8 dup (?)
game_longvecs2 VECTORLONG 8 dup (?)
game_longvecs3 VECTORLONG 8 dup (?)
game_vec1 VECTOR ?
game_vec2 VECTOR ?
game_vec3 VECTOR ?
game_vec4 VECTOR ?
game_frame_in_sec dw ?
game_frames_per_sec dw ?
field_13C dw ?
field_13E dw ?
game_frame dw ?
field_142 dw ?
field_144 dw ?
field_146 dw ?
game_frame2 dw ?
game_fps_counter dw ?
field_14C dw ?
field_14E dw ?
field_150 dw ?
playerstate CARSTATE ?
opponentstate CARSTATE ?
field_2F2 dw ?
field_2F4 dw ?
game_startcol dw ?
game_startcol2 dw ?
game_startrow dw ?
game_startrow2 dw ?
field_2FE dw 24 dup (?)
field_32E dw 24 dup (?)
field_35E dw 24 dup (?)
field_38E db 48 dup (?)
field_3BE db 48 dup (?)
kevinseed db 6 dup (?)
field_3F4 db ?
field_3F5 db ?
field_3F6 db ?
field_3F7 db ?
field_3F8 db ?
field_3F9 db ?
field_3FA db 48 dup (?)
field_42A db ?
field_42B db 24 dup (?)
field_443 db 24 dup (?)
field_45B db ?
field_45C db ?
field_45D db ?
field_45E db ?
field_45F db ?
ends
GAMEINFO struc
game_playercarid db 4 dup (?)
game_playermaterial db ?
game_playertransmission db ?
game_opponenttype db ?
game_opponentcarid db 4 dup (?)
game_opponentmaterial db ?
game_opponenttransmission db ?
game_trackname db 9 dup (?)
game_framespersec dw ?
game_recordedframes dw ?
ends
SPRITE struc
sprite_bitmapptr dd ?
sprite_unk1 dw ?
sprite_unk2 dw ?
sprite_unk3 dw ?
sprite_lineofs dw ?
sprite_left dw ?
sprite_width dw ?
sprite_top dw ?
sprite_height dw ?
sprite_height2 dw ?
sprite_unk4 dw ?
sprite_width2 dw ?
sprite_left2 dw ?
sprite_widthsum dw ?
ends
