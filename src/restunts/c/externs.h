#ifndef RESTUNTS_EXTERNS_H
#define RESTUNTS_EXTERNS_H

#ifdef RESTUNTS_SDL
#define far
#endif

extern unsigned short pspofs;
extern unsigned short pspseg;
extern unsigned short word_3FF82;
extern unsigned short word_3FF84;

extern struct MEMCHUNK* resptr1;
extern struct MEMCHUNK* resptr2;
extern struct MEMCHUNK* resendptr1;
extern struct MEMCHUNK* resendptr2;
extern unsigned short resmaxsize;

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

extern short is_audioloaded;
extern void far* songfileptr;
extern void far* voicefileptr;
extern char textresprefix; // = 'e'
extern char* shapeexts;

extern void far* load_song_file(const char* filename);
extern void far* load_voice_file(const char* filename);
extern void far* load_sfx_file(const char* filename);
extern void far* file_load_shape2d_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_res_nofatal_thunk(const char* filename);
extern void far* file_load_shape2d_nofatal2(const char* filename);
extern void far* init_audio_resources(void far* songptr, void far* voiceptr, const char* name);
extern void load_audio_finalize(void far* audiores);
extern void fatal_error(const char*, ...);
extern int do_dea_textres(void);
extern unsigned long file_decomp_rle(void far* src, void far* dst, unsigned paras);
extern int get_unflip_size(void far* memchunk);
extern void* _memcpy(void*, const void*, unsigned);
extern char* _strcpy(char* dest, const char* src);
extern char* _strcat(char* dest, const char* src);
extern int _strcmp(const char* dest, const char* src);
extern void far* __fmemcpy(void far*, const void far*, unsigned);

#ifdef RESTUNTS_DOS
#define memcpy _memcpy
#define strcpy _strcpy
#define strcat _strcat
#define fmemcpy __fmemcpy
#define strcmp _strcmp
#endif

#endif
