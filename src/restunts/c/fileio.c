#include <dos.h>

#define PAGE_SIZE 0x4000
#define PAGE_GAP  0x400

extern const char* aSFileError;
extern const char* aSFileError_1;
extern void fatal_error(const char*, ...);

// Minimal stdio.h "support" untill we can link with a real CRT.
#ifndef __STDIO_H
#define __STDIO_H
typedef unsigned size_t;
typedef int FILE;

int g_errno;

FILE* fopen(const char* path, const char* mode)
{
	unsigned short segm = FP_SEG(path);
	unsigned short offs = FP_OFF(path);
	FILE* handle;

	(void)mode;
	g_errno = 0;

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

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int fseek(FILE *file, long offset, int origin)
{
	unsigned short ol = offset;
	unsigned short oh = offset >> 16;

	origin |= (0x42 << 8); // Seek file cmd as high byte

	__asm {
		mov ax, origin
		mov  bx, file
		mov  cx, oh
		mov  dx, ol
		int  21h
		jnc  short seek_ok
		mov  g_errno, 1
	seek_ok:
	}

	return 0;
}
long ftell(FILE *file)
{
	unsigned short ol;
	unsigned short oh;

	__asm {
		mov  ah, 42h // Seek file
		mov  al, SEEK_CUR
		mov  bx, file
		mov  cx, 0
		mov  dx, 0
		int  21h
		jnc  short tell_ok
		mov  g_errno, 1
	tell_ok:
		mov  oh, dx
		mov  ol, ax
	}

	return ((long)oh << 16) | ol;
}

int ferror(FILE* file)
{
	int res = g_errno;
	(void)file;
	g_errno = 0;
	return res;
}
#endif

// Get number of 16-byte blocks needed to store entire file.
unsigned short get_file_size(const char* filename, int fatal)
{
	long length;
	FILE* file;
	
	if ((file = fopen(filename, "r"))) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fclose(file);
		
		if (!ferror(file)) {
			// May overflow, but all Stunts files are rather small.
			return (length >> 4) + (length & 0xF ? 1 : 0);
		}
	}

	if (fatal) {
		fatal_error(aSFileError, filename);
	}

	return 0;
}

// Get number of 16-byte blocks needed to store an assumed compressed file.
unsigned short file_uncompressed_size(const char* filename, int fatal)
{
	long length;
	FILE* file;
	struct compr_header { char passes; unsigned short sizel; unsigned char sizeh; } hdr;
	
	if ((file = fopen(filename, "r"))) {
		fread(&hdr, sizeof(hdr), 1, file);
		fclose(file);
		
		if (!ferror(file)) {
			// May overflow, but all Stunts files are rather small.
			length = hdr.sizel | ((long)hdr.sizeh << 16);
			return (length >> 4) + (length & 0xF ? 1 : 0);
		}
	}

	if (fatal) {
		fatal_error(aSFileError_1, filename);
	}

	return 0;
}


// Read entire file to given destination. Optionally handle errors fatal.
char far* load_binary_file(const char* filename, unsigned short dstoff, unsigned short dstseg, int fatal)
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

// Read entire file to given destination, returns NULL pointer if errors occur.
char far* load_binary_file_nofatal(const char* filename, unsigned short dstoff, unsigned short dstseg)
{
	return load_binary_file(filename, dstoff, dstseg, 0);
}

// Read entire file to given destination, handle errors as fatal.
char far* load_binary_file_fatal(const char* filename, unsigned short dstoff, unsigned short dstseg)
{
	return load_binary_file(filename, dstoff, dstseg, 1);
}
