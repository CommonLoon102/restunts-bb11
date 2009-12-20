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
RESOURCE struc
resname db 12 dup (?)
ressize dw ?
resofs dw ?
resunk dw ?
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
simd_unk4 db 536 dup (?)
next_simd dw ?
trackdata5_ptr dw ?
ends
CARSTATE struc
field_0 dw ?
field_2 dw ?
field_4 dw ?
field_6 dw ?
field_8 dw ?
field_A dw ?
field_C dw ?
field_E dw ?
field_10 dw ?
field_12 dw ?
field_14 dw ?
field_16 dw ?
field_18 dw ?
field_1A dw ?
field_1C dw ?
field_1E dw ?
field_20 dw ?
field_22 dw ?
field_24 dw ?
field_26 dw ?
field_28 dw ?
field_2A dw ?
field_2C dw ?
field_2E dw ?
field_30 dw ?
field_32 dw ?
field_34 dw ?
field_36 dw ?
field_38 dw ?
field_3A dw ?
field_3C dw ?
field_3E dw ?
field_40 dw ?
field_42 dw ?
field_44 dw ?
field_46 dw ?
field_48 dw ?
field_4A dw ?
field_4C dw ?
field_4E dw ?
field_50 dw ?
field_52 dw ?
field_54 dw ?
field_56 dw ?
field_58 dw ?
field_5A dw ?
field_5C dw ?
field_5E dw ?
field_60 dw ?
field_62 dw ?
field_64 dw ?
field_66 dw ?
field_68 dw ?
field_6A dw ?
field_6C dw ?
field_6E dw ?
field_70 dw ?
field_72 dw ?
field_74 dw ?
field_76 dw ?
field_78 dw ?
field_7A dw ?
field_7C dw ?
field_7E dw ?
field_80 dw ?
field_82 dw ?
field_84 dw ?
field_86 dw ?
field_88 dw ?
field_8A dw ?
field_8C dw ?
field_8E dw ?
field_90 dw ?
field_92 dw ?
field_94 dw ?
field_96 dw ?
field_98 dw ?
field_9A dw ?
field_9C dw ?
field_9E dw ?
field_A0 dw ?
field_A2 dw ?
field_A4 dw ?
field_A6 dw ?
field_A8 dw ?
field_AA dw ?
field_AC dw ?
field_AE dw ?
field_B0 dw ?
field_B2 dw ?
field_B4 dw ?
field_B6 dw ?
field_B8 dw ?
field_BA dw ?
field_BC db ?
field_BD db ?
field_BE db ?
field_BF db ?
field_C0 db ?
field_C1 db ?
field_C2 dw ?
field_C4 db ?
field_C5 db ?
field_C6 db ?
field_C7 db ?
field_C8 db ?
field_C9 db ?
field_CA db ?
field_CB db ?
field_CC db ?
field_CD db ?
field_CE db ?
field_CF db ?
ends
GAMESTATE struc
field_0 dw ?
field_2 dw ?
field_4_2 db 284 dup (?)
field_120 VECTOR ?
field_126 db 18 dup (?)
field_138 dw ?
field_13A dw ?
field_13C dw ?
field_13E dw ?
field_140 dw ?
field_142 dw ?
field_144 dw ?
field_146 dw ?
field_148 dw ?
field_14A dw ?
field_14C dw ?
field_14E dw ?
field_150 dw ?
playerstate CARSTATE ?
opponentstate CARSTATE ?
field_2F2 dw ?
field_2F4 dw ?
field_2F6 dw ?
field_2F8 dw ?
field_2FA dw ?
field_2FC dw ?
field_2FE db 240 dup (?)
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
