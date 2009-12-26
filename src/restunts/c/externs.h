#ifndef RESTUNTS_EXTERNS_H
#define RESTUNTS_EXTERNS_H

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

extern void fatal_error(const char*, ...);
extern void* _memcpy(void*, const void*, unsigned);

#endif
