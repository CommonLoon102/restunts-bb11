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

extern void far* load_song_file(const char* filename);
extern void far* load_voice_file(const char* filename);
extern void far* load_sfx_file(const char* filename);
extern void far* file_load_shape2d_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_res_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_nofatal(char* shapename);
extern void far* file_load_shape2d_nofatal2(char* shapename);
extern void far* init_audio_resources(void far* songptr, void far* voiceptr, const char* name);
extern void load_audio_finalize(void far* audiores);
extern void fatal_error(const char*, ...);
extern int do_dea_textres(void);
extern unsigned long timer_get_counter(void);
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
