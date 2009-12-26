#include <dos.h>
#include "externs.h"
#include "fileio.h"

#define PAGE_SIZE 0x4000
#define PAGE_GAP  0x400

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
			pop ds
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
			pop ds
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
		mov  ax, origin
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

int remove(const char* path)
{
	unsigned short segm = FP_SEG(path);
	unsigned short offs = FP_OFF(path);
	int retval;

	__asm {
		push ds
		mov  ah, 41h // Unlink file
		mov  ds, segm
		mov  dx, offs
		int  21h
		jnc  short unlink_ok
		mov  retval, -1
		mov  g_errno, ax
		jmp  short unlink_done
	unlink_ok:
		mov  retval, 0
	unlink_done:
		pop ds
	}
	
	return retval;
}
#endif

// Get number of 16-byte blocks needed to store entire file.
unsigned short file_paras(const char* filename, int fatal)
{
	long length;
	FILE* file;
	
	if ((file = fopen(filename, "rb"))) {
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

unsigned short file_paras_fatal(const char* filename)
{
	return file_paras(filename, 1);
}

unsigned short file_paras_nofatal(const char* filename)
{
	return file_paras(filename, 0);
}

// Get number of 16-byte blocks needed to store the final result of an assumed compressed file.
unsigned short file_uncomp_paras(const char* filename, int fatal)
{
	long length;
	FILE* file;
	struct compr_header { char passes; unsigned short sizel; unsigned char sizeh; } hdr;
	
	if ((file = fopen(filename, "rb"))) {
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

unsigned short file_uncomp_paras_fatal(const char* filename)
{
	return file_uncomp_paras(filename, 1);
}

unsigned short file_uncomp_paras_nofatal(const char* filename)
{
	return file_uncomp_paras(filename, 0);
}

// Read entire file to given destination. Optionally handle errors fatal.
char far* file_read(const char* filename, unsigned short dstoff, unsigned short dstseg, int fatal)
{
	int readlen;
	unsigned short curseg = dstseg;
	FILE* file;

	if ((file = fopen(filename, "rb"))) {
		// Read one page at a time.
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

// Read entire file to given destination, handle errors as fatal.
char far* file_read_fatal(const char* filename, unsigned short dstoff, unsigned short dstseg)
{
	return file_read(filename, dstoff, dstseg, 1);
}

// Read entire file to given destination, returns NULL pointer if errors occur.
char far* file_read_nofatal(const char* filename, unsigned short dstoff, unsigned short dstseg)
{
	return file_read(filename, dstoff, dstseg, 0);
}

// Read given source buffer to file. Returns a non-zero value on errors unless fatal is set.
short file_write(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length, int fatal)
{
	unsigned short retval;
	unsigned short wrtlen;
	FILE* file;
	
	if ((file = fopen(filename, "wb"))) {
		// Write one page at a time.
		while (length != 0) {
			wrtlen = length > PAGE_SIZE ? PAGE_SIZE : length;

			if (fwrite(MK_FP(srcseg, srcoff), wrtlen, 1, file) != wrtlen) {
				retval = -1;
				break;
			}
			length -= wrtlen;
			srcseg += PAGE_GAP;
		}

		fclose(file);
		
		if (!ferror(file)) {
			return 0;
		}
	}
	else {
		retval = (short)file;
	}

	if (fatal) {
		if ((short)file != retval) {
			fclose(file);
		}

		remove(filename);

		fatal_error(aSFileError_0, filename);
	}
	
	return retval;
}

// Read given source buffer to file, handle errors as fatal.
short file_write_fatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length)
{
	return file_write(filename, srcoff, srcseg, length, 1);
}

// Read given source buffer to file, returns a non-zero value on error.
short file_write_nofatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length)
{
	return file_write(filename, srcoff, srcseg, length, 0);
}
