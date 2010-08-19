#ifndef RESTUNTS_SHAPE3D_H
#define RESTUNTS_SHAPE3D_H

#pragma pack (push, 1)

struct RECTANGLE {
	int x1, y1;
	int x2, y2;
};

struct VECTOR {
	int x, y, z;
};

struct POINT2D {
	int px, py;
};

struct MATRIX {
	int _11, _12, _13;
	int _21, _22, _23;
	int _31, _32, _33;
};

struct SHAPE3D {
	unsigned short shape3d_numverts;
	struct VECTOR far* shape3d_verts;
	unsigned short shape3d_numprimitives;
	unsigned short shape3d_numpaints;
	char far* shape3d_primitives;
	char far* shape3d_cull1;
	char far* shape3d_cull2;
} _SHAPE3D;

struct SHAPE3DHEADER {
	unsigned char header_numverts;
	unsigned char header_numprimitives;
	unsigned char header_numpaints;
	unsigned char header_reserved;
};

struct TRANSFORMEDSHAPE3D {
	struct VECTOR pos;
	struct SHAPE3D* shapeptr;
	struct RECTANGLE* rectptr;
	struct VECTOR rotvec;
	unsigned short unk;
	unsigned char ts_flags;
	unsigned char material;
} _TRANSFORMEDSHAPE3D;

#pragma pack (pop)

int shape3d_load_all(void);
void shape3d_free_all(void);
void shape3d_init_shape(char far* shapeptr, struct SHAPE3D* gameshape);

#endif
