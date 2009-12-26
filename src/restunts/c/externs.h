#ifndef RESTUNTS_EXTERNS_H
#define RESTUNTS_EXTERNS_H

extern unsigned short pspofs;
extern unsigned short pspseg;
extern unsigned short word_3FF82;
extern unsigned short word_3FF84;

extern struct RESOURCE* resptr1;
extern struct RESOURCE* resptr2;
extern struct RESOURCE* resendptr1;
extern struct RESOURCE* resendptr2;
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

#endif
