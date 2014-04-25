#ifndef RESTUNTS_EXTERNS_H
#define RESTUNTS_EXTERNS_H

#ifdef RESTUNTS_SDL
#define far
#define huge
#endif

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

extern struct GAMEINFO gameconfig;
extern struct GAMEINFO gameconfigcopy;

extern short video_flag1_is1;
extern short video_flag2_is1;
extern short video_flag3_isFFFF;
extern short video_flag4_is1;
extern short video_flag5_is0;
extern short video_flag6_is1;

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
extern char g_is_busy;
extern char g_path_buf[];
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

extern unsigned short framespersec;
extern unsigned short framespersec2;
extern unsigned short timertestflag;
extern unsigned short timertestflag_copy;
extern unsigned char timertestflag2;

extern unsigned short pspofs;
extern unsigned short pspseg;
extern unsigned word_3FF82;
extern unsigned word_3FF84;

extern struct MEMCHUNK* resptr1;
extern struct MEMCHUNK* resptr2;
extern struct MEMCHUNK* resendptr1;
extern struct MEMCHUNK* resendptr2;
extern unsigned short resmaxsize;

extern unsigned long timer_callback_counter;
extern unsigned long last_timer_callback_counter;
extern unsigned long timer_copy_unk;

extern unsigned char* g_kevinrandom_seed;
extern const char* aReservememoryO;
extern const char* aReservememoryOutOfMemory;
extern const char* aMemoryManagerB;
extern const char* aResizememoryNo;
extern const char* aResizememoryCa;
extern const char* aSFileError;
extern const char* aSFileError_0;
extern const char* aSFileError_1;
extern const char* aSInvalidPackTy;
extern const char* aLocateshape4_4sShapeNotF;
extern const char* aLocatesound4_4sSoundNotF;
extern char* audiodriverstring;

extern unsigned short gState_game_frame;
extern short is_audioloaded;
extern void far* songfileptr;
extern void far* voicefileptr;
extern char textresprefix; // = 'e'
extern char* shapeexts[];
extern unsigned char palmap[];

extern int* material_clrlist_ptr;
extern int* material_clrlist_ptr_cpy;
extern int* material_clrlist2_ptr;
extern int* material_clrlist2_ptr_cpy;
extern int* material_patlist_ptr;
extern int* material_patlist_ptr_cpy;
extern int* material_patlist2_ptr;
extern int* material_patlist2_ptr_cpy;
extern unsigned short someZeroVideoConst;

extern void ensure_file_exists(int unk);

extern void far* load_song_file(const char* filename);
extern void far* load_voice_file(const char* filename);
extern void far* load_sfx_file(const char* filename);
extern void far* file_load_shape2d_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_res_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_nofatal(char* shapename);
extern void far* file_load_shape2d_nofatal2(char* shapename);
extern void far* init_audio_resources(void far* songptr, void far* voiceptr, const char* name);
extern void load_audio_finalize(void far* audiores);
extern short audio_load_driver(char* driver, short a2, short a3);
extern void audio_unload(void);
extern short audio_toggle_flag2(void);
extern short audio_toggle_flag6(void);
extern void audio_stop_unk(void);
extern void audiodrv_atexit(void);

extern void check_input(void);
extern void input_do_checking(int unk);
extern void kb_exit_handler(void);
extern void kb_shift_checking1(void);
extern void kb_shift_checking2(void);
extern void kb_reg_callback(int code, void (far* callback)(void));
extern void do_mrl_textres(void);
extern void do_joy_restext(void);
extern void do_key_restext(void);
extern void do_mof_restext(void);
extern void do_pau_restext(void);
extern void do_dos_restext(void);
extern void do_sonsof_restext(void);

extern short mouse_init(short a1, short a2);
extern void mouse_draw_opaque_check(void);

extern void video_set_mode4(void);
extern void video_set_mode7(void);
extern void video_set_mode_13h(void);

extern void shape3d_load_car_shapes(char* carid, char* oppcarid);

extern void load_palandcursor(void);
extern void sprite_set_1_size(unsigned short left, unsigned short right, unsigned short top, unsigned short height);
extern void sprite_clear_1_color(unsigned char);
extern void sprite_blit_to_video(struct SPRITE far* sprite);

extern void timer_setup_interrupt(void);

extern short set_criterr_handler(short (far* callback)(void));
extern void libsub_quit_to_dos_alt(short a1);
extern void fatal_error(const char*, ...);
extern short do_dea_textres(void);

extern void* _memcpy(void*, const void*, unsigned);
extern char* _strcpy(char* dest, const char* src);
extern char* _strcat(char* dest, const char* src);
extern int _strcmp(const char* dest, const char* src);
extern int _stricmp(const char* dest, const char* src);
extern unsigned _strlen(const char* str);
extern void far* __fmemcpy(void far*, const void far*, unsigned);
extern unsigned _abs(unsigned);
extern int _rand(void);
extern void _srand(unsigned int);

#ifdef RESTUNTS_DOS
#define memcpy _memcpy
#define strcpy _strcpy
#define strcat _strcat
#define strlen _strlen
#define fmemcpy __fmemcpy
#define strcmp _strcmp
#define stricmp _stricmp
#define abs _abs
#define printf _printf
#define rand _rand
#define srand _srand
#else
#define MK_FP(x, y) ((x << 4) + y)
#define FP_SEG(x) 0
#define FP_OFF(x) (size_t)x
#endif

#endif
