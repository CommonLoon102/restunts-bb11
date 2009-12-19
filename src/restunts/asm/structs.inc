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
