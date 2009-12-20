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
