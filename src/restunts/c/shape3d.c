#include <dos.h>
#include "externs.h"
#include "fileio.h"
#include "memmgr.h"

struct SHAPE3D {
	unsigned short shape3d_numverts;
	char far* shape3d_verts;
	unsigned short shape3d_numprimitives;
	unsigned short shape3d_numpaints;
	char far* shape3d_primitives;
	char far* shape3d_cull1;
	char far* shape3d_cull2;
};

struct SHAPE3DHEADER {
	unsigned char header_numverts;
	unsigned char header_numprimitives;
	unsigned char header_numpaints;
	unsigned char header_reserved;
};

extern char far* game1ptr;
extern char far* game2ptr;
extern char far* curshapeptr;
extern struct SHAPE3D game3dshapes[130];

extern void shape3d_init_shape(char far* shapeptr, struct SHAPE3D* gameshape);
extern unsigned long mmgr_get_res_ofs_diff_scaled(void);

extern char aBarn[];

int shape3d_load_all() {
	int i;
	unsigned long mmgrofsdiff;
	char* shapename;

	game1ptr = 0;
	game2ptr = 0;
	
	mmgrofsdiff = mmgr_get_res_ofs_diff_scaled();
	
	if (mmgrofsdiff < 0xFDE8)	// ??
		return 1;

	game1ptr = file_load_3dres("game1");
	game2ptr = file_load_3dres("game2");
	
	for (i = 0; i < 0x74; i++) {
		shapename = &aBarn[i * 5];
		curshapeptr = locate_shape_nofatal(game1ptr, shapename);
		if (curshapeptr == 0)
			curshapeptr = locate_shape_fatal(game2ptr, shapename);
		shape3d_init_shape(curshapeptr, &game3dshapes[i]);
	}
	return 0;
}

void shape3d_free_all() {
	if (game1ptr != 0)
		mmgr_free(FP_OFF(game1ptr), FP_SEG(game1ptr));
	if (game2ptr != 0)
		mmgr_free(FP_OFF(game2ptr), FP_SEG(game2ptr));
}

void shape3d_init_shape(char far* shapeptr, struct SHAPE3D* gameshape) {
	struct SHAPE3DHEADER far* hdr;
	gameshape->shape3d_numverts = hdr->header_numverts;
	gameshape->shape3d_numprimitives = hdr->header_numprimitives;
	gameshape->shape3d_numpaints = hdr->header_numpaints;
	gameshape->shape3d_verts = shapeptr + 4;
	gameshape->shape3d_cull1 = shapeptr + hdr->header_numverts * 6 + 4;
	gameshape->shape3d_cull2 = shapeptr + hdr->header_numprimitives * 4 + hdr->header_numverts * 6 + 4;
	gameshape->shape3d_primitives = shapeptr + hdr->header_numprimitives * 8 + hdr->header_numverts * 6 + 4;
}
