#include "memmgr.h"
#include "fileio.h"

extern void far* fontnptr;
extern void far* fontdefptr;
extern void far* mainresptr;
extern int* trackrows;
extern int* terrainrows;
extern int* trackpos;
extern int* trackcenterpos;
extern int* terrainpos;
extern int* terraincenterpos;
extern int* trackpos2;
extern int* trackcenterpos2;
extern char far* trackdata1;
extern char far* trackdata2;
extern char far* trackdata3;
extern char far* trackdata4;
extern char far* trackdata5;
extern char far* trackdata6;
extern char far* trackdata7;
extern char far* trackdata8;
extern char far* trackdata9;
extern char far* trackdata10;
extern char far* trackdata11;
extern char far* trackdata12;
extern char far* trackdata13;
extern char far* trackdata14;
extern char far* trackdata15;
extern char far* trackdata16;
extern char far* trackdata17;
extern char far* trackdata18;
extern char far* trackdata19;
extern char far* trackdata20;
extern char far* trackdata21;
extern char far* trackdata22;
extern char far* trackdata23;

int stuntsmain(int argc, char* argv, char* env) {
	return ported_stuntsmain_(argc, argv, env);
/*	
	int i;
	char far* trkptr;

	init_video(argc, argv);
	init_div0();
	
	for (i = 0; i < 30; i++) {
		trackrows[i] = 30 * (29 - i);
		terrainrows[i] = 30 * i;
		trackpos[i] = (29 - i) << 10;
		trackcenterpos[i] = ((29 - i) << 10) + 0x200;
		terrainpos[i] = i << 10;
		terraincenterpos[i] = (i << 10) + 0x200;
	}
	
	for (i = 0; i < 30; i++) {
		trackpos2[i] = i << 10;
		trackcenterpos2[i] = (i << 10) + 0x200;
	}
	
	mainresptr = file_load_resfile("main");

	fontdefptr = file_load_resource(0, "fontdef.fnt");
	fontnptr = file_load_resource(0, "fontn.fnt");

	set_fontdef();
	init_polyinfo();

	trkptr = mmgr_alloc_resbytes("trakdata", 0x6BF3);

	trackdata1 = trkptr;
	
	trkptr += 0x70a;
	trackdata2 = trkptr;
	
	trkptr += 0x70a;
	trackdata3 = trkptr;

	trkptr += 0x70a;
	trackdata4 = trkptr;

	trkptr += 0x80;
	trackdata5 = trkptr;

	trkptr += 0x80;
	trackdata6 = trkptr;

	trkptr += 0x80;
	trackdata7 = trkptr;

	trkptr += 0x80;
	trackdata8 = trkptr;

	trkptr += 0x60;
	trackdata9 = trkptr;

	trkptr += 0x180;
	trackdata10 = trkptr;

	trkptr += 0x120;
	trackdata11 = trkptr;

	trkptr += 0x16c;
	trackdata12 = trkptr;

	trkptr += 0x0f0;
	trackdata13 = trkptr;

	trkptr += 0x1a;
	trackdata14 = trkptr;

	trkptr += 0x385;
	trackdata15 = trkptr;

	trkptr += 0x385;
	trackdata16 = trkptr;

	trkptr += 0x2ee0;
	trackdata17 = trkptr;

	trkptr += 0x385;
	trackdata18 = trkptr;

	trkptr += 0x385;
	trackdata19 = trkptr;

	trkptr += 0x385;
	trackdata20 = trkptr;

	trkptr += 0x7ac;
	trackdata21 = trkptr;

	trkptr += 0x385;
	trackdata22 = trkptr;

	trkptr += 0x385;
	trackdata23 = trkptr;

	trkptr += 0x30;

	return 0;
*/
}
