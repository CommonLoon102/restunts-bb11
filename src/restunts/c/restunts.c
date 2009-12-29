#include <dos.h>
#include "memmgr.h"
#include "fileio.h"
#include "externs.h"

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

extern struct GAMEINFO gameconfig;
extern struct GAMEINFO gameconfigcopy;
extern void far* fontnptr;
extern void far* fontdefptr;
extern void far* mainresptr;
extern void far* cvxptr;
extern int trackrows[];
extern int terrainrows[];
extern int trackpos[];
extern int trackcenterpos[];
extern int terrainpos[];
extern int terraincenterpos[];
extern int trackpos2[];
extern int trackcenterpos2[];
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
extern char kbormouse;
extern char passed_security;
extern char byte_3B80C[];
extern char track_full_path[];
extern char byte_44AE2;
extern unsigned short dialogarg2;
extern char byte_3B85E[];
extern char byte_43966;
extern char aMain[];
extern char aFontdef_fnt[];
extern char aFontn_fnt[];
extern char aTrakdata[];
extern char aKevin[];
extern char aDefault_0[];
extern char aCvx[];
extern char aTedit__0[];
extern char a_trk[];
extern char aSlct[];
extern char aSkidms_0[];
extern char aSkidslct[];
extern char aDos[];

extern void init_video(int argc, char* argv);
extern void init_div0(void);
extern void set_fontdef(void);
extern void init_polyinfo(void);
extern void sub_28DB6(void);
extern void sub_22532(void);
extern void input_do_checking(int unk);
extern void init_kevinrandom(const char* seed);
extern void set_default_car(void);
extern void combine_file_path(const char* str1, const char* str2, const char* str3, const char* str4);
extern void ensure_file_exists(int unk);
extern unsigned short run_intro_looped(void);
extern void sub_28DB6(void);
extern void audio_stop_unk(void);
extern void audiodrv_atexit(void);
extern void sub_30883(void);
extern void kb_shift_checking1(void);
extern void video_set_mode7(void);
extern void far* locate_text_res(void far* ptr, const char* resname);
extern unsigned short show_dialog(int unk1, int unk2, void far* textresptr, unsigned short unk3, unsigned short unk4, int arg, int unk5, int unk6);
extern void audio_unload(void);
extern char run_menu(void);
extern void random_wait(void);
extern char setup_track(void);
extern void run_tracks_menu(int unk);
extern void run_opponent_menu(void);
extern void check_input(void);
extern void show_waiting(void);
extern void run_car_menu(struct GAMEINFO* unk, char* unk2, char* unk3, unsigned int unk4);
extern void get_super_random();
extern void run_game(void);
extern unsigned end_hiscore(void);
extern unsigned run_option_menu(void);
extern void init_game_state(unsigned short unk);
extern void security_check();


int stuntsmain(int argc, char* argv) {
//	return ported_stuntsmain_(argc, argv, envp);

	int i, result;
	int regax, regsi;
	char var_A;
	char far* trkptr;
	char far* textresptr;


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

	sub_22532();
	
	init_kevinrandom("kevin");
	
	strcpy(gameconfig.game_trackname, "DEFAULT");
	
	input_do_checking(1);
	input_do_checking(1);
	sub_28DB6();
	
	kbormouse = 0;
	passed_security = 1;  // set to 0 for the original copy protection
	set_default_car();

	regsi = 1;
	goto _do_intro;

_tracks_menu0:
	regax = 0;
_tracks_menu_ax:
	run_tracks_menu(regax);
	goto _show_menu;
_do_opp_menu:
	check_input();
	show_waiting();
	run_opponent_menu();
	goto _show_menu;
_do_option_menu:
	check_input();
	show_waiting();
	result = run_option_menu();
	if (result == 0) goto _show_menu;
	// goto replay-mode if option-menu-result != 0
_goto_game1:
	var_A = 1;
	goto _do_game1;
_do_car_menu:
	check_input();
	show_waiting();
	run_car_menu(&gameconfig, &gameconfig.game_playermaterial, &gameconfig.game_playertransmission, 0);
	goto _show_menu;

_do_game0:
	var_A = 0;
_do_game1:
	_memcpy(&gameconfigcopy, &gameconfig, sizeof(struct GAMEINFO));
	for (i = 0; i < 0x70A; i++) {
		trackdata20[i] = trackdata14[i];
	}
	for (i = 0; i < 0x51; i++) {
		trackdata20[i + 0x70A] = byte_3B80C[i];
		trackdata20[i + 0x75B] = byte_3B85E[i];
	}
	
	if (byte_44AE2 != 0) {
		if (file_find("tedit.*") != 0) goto _init_replay;
		goto _prepare_intro;
	}
	result = setup_track();
	if (result != 0) {
		regax = 1;
		goto _tracks_menu_ax;
	}
	random_wait();
	if (passed_security == 0) {
		fatal_error("security check");
		//get_super_random();
		//security_check();
	}

_init_replay:
	audio_unload();

	cvxptr = mmgr_alloc_resbytes("cvx", 0x5780);
	init_game_state(0xFFFF);
	
	if (var_A == 0) goto loc_104A6;
	byte_43966 = 0;
	goto loc_104AC;
	


_prepare_intro:
	audio_unload();
_do_intro0:
	regsi = 0;
_do_intro:
	ensure_file_exists(2);
	
	if (regsi != 0) {
		combine_file_path(byte_3B80C, gameconfig.game_trackname, ".trk", track_full_path);
		file_read_fatal(track_full_path, trackdata14);
	}
	
	byte_44AE2 = 0;
	result = run_intro_looped();
	if (result == 27) goto _ask_dos;

_show_menu:

	ensure_file_exists(2);
	if (is_audioloaded == 0) {
		file_load_audiores("skidslct", "skidms", "SLCT");
	}
	result = run_menu();
	if (result == -1) 
		goto _prepare_intro; else
	if (result == 0)
		goto _do_game0; else
	if (result == 1)
		goto _do_car_menu; else
	if (result == 2)
		goto _do_opp_menu; else
	if (result == 3)
		goto _tracks_menu0; else
	if (result == 4)
		goto _do_option_menu; else
		goto _show_menu;
loc_104A6:
	gameconfig.game_recordedframes = 0;
loc_104AC:
	show_waiting();
	run_game();
	//if (byte_44AE2 != 0) goto loc_104D2;
	//if (byte_43966 == 0) goto loc_104D2;
	if (byte_44AE2 == 0 && byte_43966 != 0) {
		result = end_hiscore();
		if (result == 0) {
			byte_43966 = 4;
			goto loc_104AC;
		}
		if (result == 1) goto loc_104A6;
	}

loc_104D2:
	_memcpy(&gameconfigcopy, &gameconfig, sizeof(struct GAMEINFO));
	for (i = 0; i < 0x70A; i++) {
		trackdata14[i] = trackdata20[i];
	}
	for (i = 0; i < 0x51; i++) {
		byte_3B80C[i] = trackdata20[i + 0x70A];
		byte_3B85E[i] = trackdata20[i + 0x75B];
	}
	mmgr_op_unk(FP_OFF(cvxptr), FP_SEG(cvxptr));
	
	if (byte_44AE2 != 0) goto _do_intro0;
	goto _show_menu;

_ask_dos:
	textresptr = locate_text_res(mainresptr, "dos");
	result = show_dialog(2, 1, textresptr, 0xFFFF, 0xFFFF, dialogarg2, 0, 0);
	if (result >= 1) {
		sub_28DB6();
		audio_stop_unk();
		audiodrv_atexit();
		sub_30883();
		kb_shift_checking1();
		video_set_mode7();
		return result;
	}
	goto _do_intro0;
}

