#include <dos.h>
#include <stddef.h>
#include "memmgr.h"
#include "fileio.h"
#include "externs.h"
#include "keyboard.h"
#include "shape3d.h"
#include "shape2d.h"

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
extern char byte_44AE2;
extern unsigned short dialogarg2;
extern char byte_3B85E[];
extern char byte_43966;
extern char aMain[];
extern char aFontdef_fnt[];
extern char aFontn_fnt[];
extern char aTrakdata[];
extern char aDefault_0[];
extern char aCvx[];
extern char aTedit__0[];
extern char aSlct[];
extern char aSkidms_0[];
extern char aSkidslct[];
extern char aDos[];

extern void init_div0(void);
extern void font_set_fontdef(void);
extern void init_polyinfo(void);
extern void mouse_draw_opaque_check(void);
extern void sub_22532(void);
extern void input_do_checking(int unk);
extern void ensure_file_exists(int unk);
extern unsigned short run_intro_looped(void);
extern void audio_stop_unk(void);
extern void audiodrv_atexit(void);
extern void kb_exit_handler(void);
extern void kb_shift_checking1(void);
extern void video_set_mode7(void);
extern void far* locate_text_res(void far* ptr, const char* resname);
extern unsigned short show_dialog(int unk1, int unk2, void far* textresptr, unsigned short unk3, unsigned short unk4, int arg, int unk5, int unk6);
extern void audio_unload(void);
extern char run_menu(void);
extern char setup_track(void);
extern void run_tracks_menu(int unk);
extern void run_opponent_menu(void);
extern void check_input(void);
extern void show_waiting(void);
extern void run_car_menu(struct GAMEINFO* unk, char* unk2, char* unk3, unsigned int unk4);
extern void run_game(void);
extern unsigned end_hiscore(void);
extern unsigned run_option_menu(void);
extern void init_game_state(unsigned short unk);
extern void security_check(void);

int get_0(void)
{
	return 0;
}

unsigned long timer_get_counter(void)
{
	/*
	unsigned long val;

	disable();
	val = timer_callback_counter;
	enable();

	return val;
	*/

	// Compiler produces too sloppy code; stick to asm for now...
	__asm {
		cli
		mov     ax, word ptr timer_callback_counter
		mov     dx, word ptr timer_callback_counter+2
		sti
	}
}

unsigned long timer_get_delta(void)
{
    /*
	unsigned long last, curr;
	
	last = last_timer_callback_counter;
	
	disable();
	curr = timer_get_counter();
	enable();
	
	last_timer_callback_counter = curr;

	return curr - last;
	*/
	
	// Compiler produces too sloppy code; stick to asm for now...
	__asm {
		mov     bx, word ptr last_timer_callback_counter
		mov     cx, word ptr last_timer_callback_counter+2
		cli
		mov     ax, word ptr timer_callback_counter
		mov     dx, word ptr timer_callback_counter+2
		sti
		mov     word ptr last_timer_callback_counter, ax
		mov     word ptr last_timer_callback_counter+2, dx
		sub     ax, bx
		sbb     dx, cx
	}
}

unsigned long timer_get_delta_alt(void)
{
	return timer_get_delta();
}

unsigned long timer_custom_delta(unsigned long ticks)
{
	return timer_get_counter() - ticks;
}

void timer_reset()
{
	timer_callback_counter = 0;
}

#define KEVINRANDOM_SEED_LEN 6
void init_kevinrandom(const char* seed)
{
	int i;

	for (i = 0; i < KEVINRANDOM_SEED_LEN; ++i) {
		g_kevinrandom_seed[i] = seed[i];
	}
}

void get_kevinrandom_seed(char* seed)
{
	int i;

	for (i = 0; i < KEVINRANDOM_SEED_LEN; ++i) {
		seed[i] = g_kevinrandom_seed[i];
	}
}

int get_kevinrandom(void)
{
	g_kevinrandom_seed[4] += g_kevinrandom_seed[5];
	g_kevinrandom_seed[3] += g_kevinrandom_seed[4];
	g_kevinrandom_seed[2] += g_kevinrandom_seed[3];
	g_kevinrandom_seed[1] += g_kevinrandom_seed[2];
	g_kevinrandom_seed[0] += g_kevinrandom_seed[1];
	
	!++g_kevinrandom_seed[5] 
	&& !++g_kevinrandom_seed[4]
	&& !++g_kevinrandom_seed[3]
	&& !++g_kevinrandom_seed[2]
	&& !++g_kevinrandom_seed[1]
	&& ++g_kevinrandom_seed[0];

	return g_kevinrandom_seed[0];
}

int get_super_random(void)
{
	int val = rand() + get_kevinrandom() + timer_get_counter() + gState_game_frame;
	return val < 0 ? -val : val;
}

int video_get_status(void)
{
	return inport(0x3DA) & 0x8;
}

int random_wait(void)
{
	int status1, i;
	
	status1 = video_get_status();
	
	for (i = 0; status1 == video_get_status() && i < 12000; ++i);
	
	if (i == 1024) {
		i = *(char near*)0x46C; // TODO: Debug this bugger...
	}
	
	while (1) {
		if (!(--i)) {
			break;
		}
		rand();
		get_kevinrandom();
	}
	
	i &= 0xFF;
	
	while (1) {
		if (!(--i)) {
			break;
		}
		
		get_kevinrandom();
		rand();
	}
	
	return i;
}

char toupper(char* ch)
{
	if (*ch >= 'a' && *ch <= 'z') {
		*ch -= ' ';
	}
	
	return *ch;
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

void set_default_car(void) {
	gameconfig.game_playercarid[0]     = 'C';
	gameconfig.game_playercarid[1]     = 'O';
	gameconfig.game_playercarid[2]     = 'U';
	gameconfig.game_playercarid[3]     = 'N';
	gameconfig.game_playermaterial     = 0;
	gameconfig.game_playertransmission = 1;
	gameconfig.game_opponenttype       = 0;
	gameconfig.game_opponentmaterial   = 0;
	gameconfig.game_opponentcarid[0]   = 0xFF;
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

extern struct SHAPE3D game3dshapes[];

//extern void shape3d_load_car_shapes(char* carid, char* oppcarid);
extern unsigned select_cliprect_rotate(int angX, int angY, int angZ, struct RECTANGLE* cliprect, int unk);
//extern void transformed_shape_op(struct TRANSFORMSHAPE3D* shape);

extern void sprite_blit_to_video(struct SPRITE far* sprite);
extern void sub_29772(void);
extern void set_projection(int, int, int, int);

struct RECTANGLE shaperect = { 0, 320, 0, 200 };
struct TRANSFORMEDSHAPE3D transshape;
struct RECTANGLE cliprect = { 0, 0x140, 0, 0x5F };
struct VECTOR carpos = { 0, 0x0FCB8, 0x0B40 }; // from the original
//struct VECTOR carpos = { 0, 0, 320 };

extern struct SPRITE far* sprite_make_wnd(int width, int height);
extern struct SPRITE far* wndsprite;

extern struct RECTANGLE cliprect_unk;
//cliprect_unk    RECTANGLE <270Fh, 0FFFFh, 270Fh, 0FFFFh>


extern int polyinfonumpolys;
extern unsigned char far* polyinfoptrs[]; // array size = 0x190 
extern unsigned int word_40ED6[]; // array size = 0x190

extern void preRender_default(int color, int vertlinecount, int* vertlines);

void copy_material_list_pointers(void* clrlist, void* clrlist2, void* patlist, void* patlist2, unsigned short videoConst)
{
	material_clrlist_ptr_cpy = clrlist;
	material_clrlist2_ptr_cpy = clrlist2;
	material_patlist_ptr_cpy = patlist;
	material_patlist2_ptr_cpy = patlist2;
	someZeroVideoConst = videoConst;
}

/*
void init_main(int argc, char* argv[])
{
	//ported_init_main_(argc, argv);
	unsigned char argmode4, argnosound, var2;
	unsigned short timerdelta1, timerdelta2, timerdelta3, var4, var6, var8, varA, var18, var1A;

	// Keyboard
	kb_init_interrupt();
	kb_shift_checking2();
	kb_call_readchar_callback();

	kb_reg_callback(0x0007, &do_mrl_textres);
	kb_reg_callback(0x000A, &do_joy_restext);
	kb_reg_callback(0x000B, &do_key_restext);
	kb_reg_callback(0x3200, &do_mof_restext);
	kb_reg_callback(0x0010, &do_pau_restext);
	kb_reg_callback('p', &do_pau_restext);
	kb_reg_callback(0x0011, &do_dos_restext);
	kb_reg_callback(0x0013, &do_sonsof_restext);
	kb_reg_callback(0x0018, &do_dos_restext);
	
	// Video
	video_flag1_is1 = 1;
	video_flag2_is1 = 1;
	video_flag3_isFFFF = 0xFFFF;
	video_flag4_is1 = 1;
	
	mmgr_alloc_a000();
	
	video_flag5_is0 = 0;
	video_flag6_is1 = 1;
	
	textresprefix = 'e';
	
	argmode4 = 0;
	argnosound = 0;
	var2 = 0;
	
	// TODO: Loop?
loc_39F63:
	for (j = 1; argc > j; ++j) {
		argcmd = argv[j];
		
		if (argcmd[0] == '/') {
			if (argcmd[1] == 'h') {
				argmode = 4;
			}
			else {
				if (argcmd[1] == 'n' && argcmd[2] == 's') {
					
				}
				else if (argcmd[1]) == 's' {
				...
				}
				
			}
		}
		// TODO: parse args...
	}

	{
		video_set_mode_13h();
		if (!argmode4) {
			video_set_mode4();
		}
		timer_setup_interrupt();
		sprite_copy_2_to_1_clear();
		mouse_init(0x140, 0xC8);
		
		if (!audio_load_driver(audiodriverstring, 0, 0)) {
			audio_stop_unk();
			libsub_quit_to_dos_alt(1);
		}
		
		if (!argnosound) {
			audio_toggle_flag2();
			audio_toggle_flag6();
		}
		
		set_criterr_handler(&do_dea_textres);
		
		load_palandcursor();
		
		// Timing measures?
		sprite_copy_1_to_1);
		sprite_set_1_size(0, 0x140, 0, 0x78);

		timer_get_delta_alt();
		for (i = 0; i < 0xF; ++i) {
			sprite_clear_1_color(0);
		}
		timerdelta1 = timer_get_delta_alt();
		
		sprite_set_1_size(0, 0x140, 0, 0x3C);
	}

	for (j = 0; j < 0xF; ++i) {
		varA = 0;
		var8 = 0;
		var6 = 0;
		var4 = 0;
		
		for (i = 0; i < 0x190; ++i) {
			var18 = var1A = i; // TODO: Where is var18 used?
			rect_adjust_from_point(&var1A, &varA);
		}
		
		sprite_clear_1_color(0);
	}
	
	timerdelta2 = timer_get_delta_alt();

	for (j = 0; j < 0x92; ++j) {
		for (i = 0; i < 0xff; ++i) {
			rect_adjust_from_point(&var1A, &varA);
		}
	}
	
	timerdelta1 = timerdelta3 = timer_get_delta_alt();
	
	timertestflag = (timerdelta2 >= timerdelta1);
	framespersec2 = (timerdelta3 >= 75) ? 10 : 20;

	if (timerdelta3 < 35) {
		timertestflag2 = 0;
	}
	else if (timerdelta3 < 55) {
		timertestflag2 = 1;
	}
	else if (timerdelta3 < 67) {
		timertestflag2 = 2;
	}
	else if (timerdelta3 < 100) {
		timertestflag2 = 3;
	}
	else if (timertestflag) {
		timertestflag2 = 4;
	}
	else {
		timertestflag2 = 3;
	}

	framespersec = framespersec2;
	timertestflag_copy = timertestflag;
	
	random_wait();
	
	copy_material_list_pointers(material_clrlist_ptr, material_clrlist2_ptr, material_patlist_ptr, material_patlist2_ptr, 0);
}*/

int stuntsmain2(int argc, char* argv) {
	int result;
	char far* textresptr;
	int carposangle;
	struct SPRITE far* var42wnd;
	int counter;
	int inch;
	int shapeindex;

	// initialization
	init_main(argc, argv);
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

	strcpy(gameconfig.game_trackname, "DEFAULT");
	
	input_do_checking(1);
	input_do_checking(1);
	mouse_draw_opaque_check();

	kbormouse = 0;
	passed_security = 1;  // set to 0 for the original copy protection	
	//set_default_car();
		
	// try do something
	sub_29772();
	set_projection(0x24, 0x11, 0x140, 0x64);	// would at best draw just a pixel without this - camera projection??

	wndsprite = sprite_make_wnd(320, 100);

	//run_intro_looped();
	
	carposangle = polarAngle(carpos.y, carpos.z);

	shape3d_load_all();
	shape3d_load_car_shapes("coun", "coun");
	select_cliprect_rotate(0, carposangle, 0, &cliprect, 0);

	//shaperect = cliprect;
	transshape.material = 0;
	transshape.rotvec.x = 0;
	transshape.rotvec.y = 0;
	transshape.pos = carpos;

	transshape.unk = 0;//0x7530;
	transshape.ts_flags = 0;
	transshape.rectptr = &shaperect;

	counter = 0;
	shapeindex = 24;
	for (; ; counter++) {

		transshape.rotvec.z = 0; //counter + 0x230;
		
		// seg000:1C58                 mov     [bp+var_transshape.ts_shapeptr], (offset game3dshapes.shape3d_numverts+0AA8h)
		// 0xAA8 / sizeof(SHAPE3D) = 0xAA8 / 0x16 = 124, points at where car0 is loaded during shape3d_load_car_shapes();

		transshape.shapeptr = &game3dshapes[shapeindex];

		//transshape.shapeptr = &game3dshapes[124];
		//transshape.shapeptr = &game3dshapes[124];
		
		transformed_shape_op(&transshape);
		
		sprite_copy_wnd_to_1();
		sprite_clear_1_color(3);
		
		//sprite_set_1_size(50, 200, 50, 100);
		get_a_poly_info(); // renders to sprite1
	
		//sprite_copy_2_to_1_2();
		sprite_blit_to_video(wndsprite); // sprite_blit_to_video(wndsprite);
		
		inch = get_kb_or_joy_flags();//kb_get_char();
		if (inch == 4) { // right
			shapeindex++;
			shapeindex = (shapeindex + 0x74) % 0x74;
		} else
		if (inch == 8) { // left
			shapeindex--;
			shapeindex = (shapeindex + 0x74) % 0x74;
		} else
		if (inch != 0) {
			textresptr = locate_text_res(mainresptr, "dos");
			//result = show_dialog(2, 1, textresptr, 0xFFFF, 0xFFFF, dialogarg2, 0, 0); // center
			result = show_dialog(2, 1, textresptr, 0, 170, dialogarg2, 0, 0);
			if (result >= 1)
				break;
		}
	}

	//var42wnd = sprite_make_wnd(320, 200);
	//setup_mcgawnd2();
	//sprite_set_1_size(0, 320, 0, 200);
	//sprite_copy_2_to_1_2();
	//sprite_clear_1_color(2);
		//sprite_copy_wnd_to_1();
		//sprite_copy_2_to_1_2();
	
		//sprite_putimage(wndsprite->sprite_bitmapptr);
		//sprite_putimage(var42wnd->sprite_bitmapptr);
	
	//fatal_error("happy yet?");


	// shutdown
	mouse_draw_opaque_check();
	audio_stop_unk();
	audiodrv_atexit();
	kb_exit_handler();
	kb_shift_checking1();
	video_set_mode7();
	
	fatal_error("err %i", inch);

	return 0;
}


int stuntsmain(int argc, char* argv) {

	int i, result;
	int regax, regsi;
	char var_A;
	char far* trkptr;
	char far* textresptr;

	//return ported_stuntsmain_(argc, argv);

	init_main(argc, argv);
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
	
	strcpy(gameconfig.game_trackname, "DEFAULT");
	
	//fatal_error("ai");
	
	input_do_checking(1);
	input_do_checking(1);
	mouse_draw_opaque_check();
	
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
		td20_trk_file_appnd[i] = td14_elem_map_main[i];
	}
	for (i = 0; i < 0x51; i++) {
		td20_trk_file_appnd[i + 0x70A] = byte_3B80C[i];
		td20_trk_file_appnd[i + 0x75B] = byte_3B85E[i];
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
		file_build_path(byte_3B80C, gameconfig.game_trackname, ".trk", g_path_buf);
		file_read_fatal(g_path_buf, td14_elem_map_main);
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
		td14_elem_map_main[i] = td20_trk_file_appnd[i];
	}
	for (i = 0; i < 0x51; i++) {
		byte_3B80C[i] = td20_trk_file_appnd[i + 0x70A];
		byte_3B85E[i] = td20_trk_file_appnd[i + 0x75B];
	}
	mmgr_release(cvxptr);
	
	if (byte_44AE2 != 0) goto _do_intro0;
	goto _show_menu;

_ask_dos:
	textresptr = locate_text_res(mainresptr, "dos");
	result = show_dialog(2, 1, textresptr, 0xFFFF, 0xFFFF, dialogarg2, 0, 0);
	if (result >= 1) {
		mouse_draw_opaque_check();
		audio_stop_unk();
		audiodrv_atexit();
		kb_exit_handler();
		kb_shift_checking1();
		video_set_mode7();
		return result;
	}
	goto _do_intro0;
}
