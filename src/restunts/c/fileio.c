#include <dos.h>

#define PAGE_SIZE 0x4000
#define PAGE_GAP  0x400

extern char* aSFileError;
extern void fatal_error(char*, ...);

// Minimal stdio.h "support".
#ifndef __STDIO_H
#define __STDIO_H
typedef unsigned size_t;
typedef int FILE;

int g_errno = 0;

FILE* fopen(const char* path, const char* mode)
{
	unsigned short segm = FP_SEG(path);
	unsigned short offs = FP_OFF(path);
	FILE* handle;

	(void)mode;

	__asm {
		push ds
		mov  ah, 3Dh // Open file
		mov  al, 0 //_A_RDONLY
		mov  ds, segm
		mov  dx, offs
		int  21h
		jnc  short open_ok
		mov  ax, 0
		mov  g_errno, 1
	open_ok:
		mov  handle, ax
		pop ds
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

size_t fread(void far* dst, size_t size, size_t nmemb, FILE* file)
{
	unsigned short segm = FP_SEG(dst);
	unsigned short offs = FP_OFF(dst);

	size_t res;
	size *= nmemb;
	
	__asm {
		push ds
		mov  ah, 3Fh // Read from file
		mov  bx, file
		mov  ds, segm
		mov  dx, offs
		mov  cx, size
		int  21h
		jnc  short read_ok
		mov  ax, 0
		mov  g_errno, 1
	read_ok:
		mov  res, ax
		pop ds
	}
	
	return res;
}

int ferror(FILE* file)
{
	int res = g_errno;
	(void)file;
	g_errno = 0;
	return res;
}
#endif

char far* load_binary_file(char *filename, unsigned short dstoff, unsigned short dstseg, int fatal)
{
	int readlen;
	unsigned short curseg = dstseg;
	FILE* file;

	if ((file = fopen(filename, "r"))) {
		do {
			readlen = fread(MK_FP(curseg, dstoff), PAGE_SIZE, 1, file);
			curseg += PAGE_GAP;
		} while (readlen == PAGE_SIZE);

		fclose(file);

		if (!ferror(file)) {
			return MK_FP(dstseg, dstoff);
		}
	}

	if (fatal) {
		fatal_error(aSFileError, filename);
	}

	return MK_FP(0, 0);
}

char far* load_binary_file_nofatal(char *filename, unsigned short dstoff, unsigned short dstseg)
{
	return load_binary_file(filename, dstoff, dstseg, 0);
}

char far* load_binary_file_fatal(char *filename, unsigned short dstoff, unsigned short dstseg)
{
	return load_binary_file(filename, dstoff, dstseg, 1);
}
