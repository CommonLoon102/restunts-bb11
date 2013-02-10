#include <dos.h>
//#include <stddef.h>

#pragma pack (push, 1)
struct GAMEINFO {
	char game_playercarid[4];
	char game_playermaterial;
	char game_playertransmission;
	char game_opponenttype;
	char game_opponentcarid[4];
	char game_opponentmaterial;
	char game_opponenttransmission;
	char game_trackname[9];
	unsigned short game_framespersec;
	unsigned short game_recordedframes;
};
#pragma pack (pop)

#pragma pack (push, 1)
struct GAMESTATE {
	unsigned char data1[0x140];
	unsigned short frame;
	unsigned char data2[0x31E];
};
#pragma pack (pop)

extern struct GAMEINFO gameconfig;
extern struct GAMEINFO gameconfigcopy;
extern struct GAMESTATE state;

extern void far* mainresptr;
extern void far* fontnptr;
extern void far* fontdefptr;
extern void far* cvxptr;

extern int trackrows[];
extern int terrainrows[];
extern int trackpos[];
extern int trackcenterpos[];
extern int terrainpos[];
extern int terraincenterpos[];
extern int trackpos2[];
extern int trackcenterpos2[];
extern char far* td01_track_file_cpy; //trackdata1;
extern char far* td02_penalty_related; //trackdata2;
extern char far* trackdata3;
extern char far* td04_aerotable_pl; //trackdata4;
extern char far* td05_aerotable_op; //trackdata5;
extern char far* trackdata6;
extern char far* trackdata7;
extern char far* td08_direction_related; //trackdata8;
extern char far* trackdata9;
extern char far* td10_track_check_rel;// trackdata10;
extern char far* td11_highscores; //trackdata11;
extern char far* trackdata12;
extern char far* td13_rpl_header; //trackdata13;
extern char far* td14_elem_map_main; //trackdata14;
extern char far* td15_terr_map_main; //trackdata15;
extern char far* td16_rpl_buffer; //trackdata16;
extern char far* td17_trk_elem_ordered; //trackdata17;
extern char far* trackdata18;
extern char far* trackdata19;
extern char far* td20_trk_file_appnd; //trackdata20;
extern char far* td21_col_from_path; //trackdata21;
extern char far* td22_row_from_path; //trackdata22;
extern char far* trackdata23;

extern char kbormouse;
extern char passed_security;
extern char byte_3B80C[];
extern char byte_3B85E[];
extern unsigned short word_449EA;
extern unsigned short word_42D28;
extern unsigned char byte_454B8;
extern unsigned char byte_44AE2;
extern unsigned char byte_449E6;
extern unsigned char byte_449DA;
extern unsigned char byte_461C8;
extern unsigned char byte_44346;
extern unsigned char byte_4432A;
extern unsigned char byte_46467;
extern unsigned char dashb_toggle;
extern unsigned char replaybar_toggle;
extern unsigned char cameramode;
extern unsigned char byte_45DB2;
extern unsigned short word_44DCA;
extern unsigned short framespersec;

extern void init_video(int argc, char* argv[]);
extern void init_div0(void);
extern void far* file_load_resfile(const char* filename);
extern void far* file_load_resource(int type, const char* filename);
extern void font_set_fontdef(void);
extern void init_polyinfo(void);
extern void init_trackdata(void);
extern void sub_22532(void);
extern void init_kevinrandom(const char* seed);
extern unsigned short get_kevinrandom(void);
extern void load_replay_file(const char* dir, const char* name);
extern char setup_track(void);
extern void init_game_state(unsigned short unk);
extern void restore_gamestate(unsigned short frame);
extern void update_gamestate(void);
extern unsigned short input_do_checking(unsigned short val);
extern void setup_player_cars(void);
extern void set_frame_callback(void);
extern void far* mmgr_alloc_resbytes(const char* name, long int size);
extern void audio_stop_unk(void);
extern void audiodrv_atexit(void);
extern void kb_exit_handler(void);
extern void kb_shift_checking1(void);
extern void video_set_mode7(void);
extern void fatal_error(const char*, ...);

extern void* _memcpy(void*, const void*, unsigned);
extern char* _strcpy(char* dest, const char* src);
extern char* _strcat(char* dest, const char* src);
extern int _printf(const char* fmt, ...);

#define memcpy _memcpy
#define strcpy _strcpy
#define strcat _strcat
#define printf _printf

typedef unsigned size_t;
typedef int FILE;

int g_errno;

FILE* fopen(const char* path, const char* mode)
{
	unsigned short segm = FP_SEG(path);
	unsigned short offs = FP_OFF(path);
	FILE* handle;

	g_errno = 0;

	if (mode[0] == 'w') { // Create new file for writing
		__asm {
			push ds
			mov  ah, 3Ch // Create file
			mov  cx, 0 // No attributes
			mov  ds, segm
			mov  dx, offs
			int  21h
			jnc  short create_ok
			mov  ax, 0
			mov  g_errno, 1
		create_ok:
			mov  handle, ax
			pop  ds
		}
	}
	else { // Open existing file for reading
		__asm {
			push ds
			mov  ah, 3Dh // Open file
			mov  al, 0 // Read only
			mov  ds, segm
			mov  dx, offs
			int  21h
			jnc  short open_ok
			mov  ax, 0
			mov  g_errno, 1
		open_ok:
			mov  handle, ax
			pop  ds
		}
	}

	return handle;
}

int fclose(FILE* file)
{
	int res;

	__asm {
		mov  ah, 3Eh // Close file
		mov  bx, file
		int  21h
		jnc  short close_ok
		mov  ax, 0
		mov  g_errno, 1
	close_ok:
		mov  res, ax
	}
	
	return res;
}

size_t fwrite(const void far* src, size_t size, size_t nmemb, FILE* file)
{
	unsigned short segm = FP_SEG(src);
	unsigned short offs = FP_OFF(src);

	size_t res;
	size *= nmemb;
	
	__asm {
		push ds
		mov  ah, 40h // Write to file
		mov  bx, file
		mov  ds, segm
		mov  dx, offs
		mov  cx, size
		int  21h
		jnc  short write_ok
		mov  ax, 0
		mov  g_errno, 1
	write_ok:
		mov  res, ax
		pop  ds
	}
	
	return res;
}

void init_row_tables(void) {
	int i;
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
}

void init_trackdata(void) {
	char far* trkptr;
	trkptr = mmgr_alloc_resbytes("trakdata", 0x6BF3);

	td01_track_file_cpy = trkptr;
	
	trkptr += 0x70a;
	td02_penalty_related = trkptr;
	
	trkptr += 0x70a;
	trackdata3 = trkptr;

	trkptr += 0x70a;
	td04_aerotable_pl = trkptr;

	trkptr += 0x80;
	td05_aerotable_op = trkptr;

	trkptr += 0x80;
	trackdata6 = trkptr;

	trkptr += 0x80;
	trackdata7 = trkptr;

	trkptr += 0x80;
	td08_direction_related = trkptr;

	trkptr += 0x60;
	trackdata9 = trkptr;

	trkptr += 0x180;
	td10_track_check_rel = trkptr;

	trkptr += 0x120;
	td11_highscores = trkptr;

	trkptr += 0x16c;
	trackdata12 = trkptr;

	trkptr += 0x0f0;
	td13_rpl_header = trkptr;

	trkptr += 0x1a;
	td14_elem_map_main = trkptr;

	trkptr += 0x385;
	td15_terr_map_main = trkptr;

	trkptr += 0x385;
	td16_rpl_buffer = trkptr;

	trkptr += 0x2ee0;
	td17_trk_elem_ordered = trkptr;

	trkptr += 0x385;
	trackdata18 = trkptr;

	trkptr += 0x385;
	trackdata19 = trkptr;

	trkptr += 0x385;
	td20_trk_file_appnd = trkptr;

	trkptr += 0x7ac;
	td21_col_from_path = trkptr;

	trkptr += 0x385;
	td22_row_from_path = trkptr;

	trkptr += 0x385;
	trackdata23 = trkptr;

	trkptr += 0x30;
}

int stuntsmain(int argc, char* argv[]) {
	int i;
	char outname[13], carid[5];
	FILE* fout;
	
	if (argc != 2) {
		printf("Usage: %s REPLNAME\n\n", argv[0]);
		return 1;
	}

	init_video(argc, argv);
	init_div0();
	init_row_tables();

	mainresptr = file_load_resfile("main");
	
	fontdefptr = file_load_resource(0, "fontdef.fnt");
	fontnptr = file_load_resource(0, "fontn.fnt");

	font_set_fontdef();
	init_polyinfo();
	
	init_trackdata();

	sub_22532();
	
	init_kevinrandom("kevin");

	printf("Loading replay... ");
	load_replay_file("", argv[1]);
	printf("OK\n");
	_memcpy(carid, gameconfig.game_playercarid, 4);
	carid[4] = 0;
	printf("  Track: '%s' Car: '%s'\n", gameconfig.game_trackname, carid);

	printf("Copying track... ");
	_memcpy(&gameconfigcopy, &gameconfig, sizeof(struct GAMEINFO));
	for (i = 0; i < 0x70A; i++) {
		td20_trk_file_appnd[i] = td14_elem_map_main[i];
	}
	for (i = 0; i < 0x51; i++) {
		td20_trk_file_appnd[i + 0x70A] = byte_3B80C[i];
		td20_trk_file_appnd[i + 0x75B] = byte_3B85E[i];
	}
	printf("OK\n");

	printf("Setting up track... ");
	setup_track();
	printf("OK\n");

	printf("Allocating cvx... ");
	cvxptr = mmgr_alloc_resbytes("cvx", 0x5780);
	printf("OK\n");
	
	printf("Initializing game state... ");
	init_game_state(0xFFFF);
	printf("OK\n");
	
	// Inits from run_game()...
	word_449EA = 0xFFFF;
	word_42D28 = get_kevinrandom() << 3;
	replaybar_toggle = 1;
	byte_454B8 = 0;
	byte_44AE2 = 0;
	cameramode = 0;
	byte_45DB2 = 2;
	byte_454B8 = 1;

	printf("Loading assets... ");
	setup_player_cars();
	kbormouse = 0;
	byte_449E6 = 0;
	byte_449DA = 1;
	
	set_frame_callback();
	byte_461C8 = 0xFF;
	byte_44346 = 0;
	byte_4432A = 0;
	byte_46467 = 0;
	dashb_toggle = 0;
	
	cameramode = 0;
	byte_45DB2 = 2;
	word_44DCA = 0x1F4;
	framespersec = 20;

	restore_gamestate(0);
	restore_gamestate(gameconfig.game_recordedframes);
	printf("OK\n");
	
	strcpy(outname, argv[1]);
	strcat(outname, ".BIN");
	outname[12] = 0;
	printf("Creating output file '%s'... ", outname);
	
	fout = fopen(outname, "w");
	if (!fout) {
		printf("FAIL\n");
		return 1;
	}
	printf("OK\n");
	
	fwrite(&gameconfig.game_recordedframes, sizeof(unsigned short), 1, fout);

	printf("Processing %d frames... ", gameconfig.game_recordedframes);

	while (gameconfig.game_recordedframes > state.frame) {
		input_do_checking(1);
		update_gamestate();
		fwrite(&state, sizeof(struct GAMESTATE), 1, fout);
		//printf("Current frame %d\n", state.frame);
	}
	
	printf("OK\n");
	
	fclose(fout);
	
	input_do_checking(1);

	fatal_error("\nDone.\n");

	//audio_stop_unk();
	//audiodrv_atexit();
	//kb_exit_handler();
	//kb_shift_checking1();
	//video_set_mode7();

	return 0;
}