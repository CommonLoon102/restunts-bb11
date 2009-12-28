#include <dos.h>
#include "externs.h"
#include "fileio.h"
#include "memmgr.h"

#define PAGE_SIZE 0x4000
#define PAGE_GAP  0x400
#define FILENAME_LEN 13

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

static struct file_find_dos {
	struct find_t dta; // DOS DTA struct
	char path[128];    // Full path to found file
	char* dirdelim;    // Last dir delimiter in path string
} g_find;

// Find file matching given query. Returns pointer to first matched filename
// including path from the query. NULL is returned on error/no hits.
// FIXME: DOS specific implementation.
const char* file_find(const char* query)
{
	char const* chsrc;
	char* chdst;
	char attrs = FA_NORMAL | FA_HIDDEN | FA_SYSTEM;
	int retval;

	__asm {
		mov  ah, 1Ah // Set DTA
		mov  dx, offset g_find.dta
		int  21h

		mov  ah, 4Eh // Find first file
		mov  cl, attrs
		mov  dx, query
		int  21h

		jnc  short find_ok
		mov  retval, -1
		jmp  short find_done
	find_ok:
		mov  retval, 0
	find_done:
	}

	// Find failed.
	if (retval) {
		return  0;
	}	

	// Copy path from query.
	chdst = g_find.dirdelim = g_find.path;
	for (chsrc = query; *chsrc; ++chsrc, ++chdst) {
		*chdst = *chsrc;
		
		if (*chdst == ':' || *chdst == '\\') {
			g_find.dirdelim = chdst;
		}
	}

	// Copy found filename to result path.
	_memcpy(g_find.dirdelim, g_find.dta.name, FILENAME_LEN);

	return g_find.path;
}

// Returns next found filename from file_find() query.
// FIXME: DOS specific implementation.
const char* file_find_next()
{
	int retval;

	__asm {
		mov  ah, 1Ah // Set DTA
		mov  dx, offset g_find.dta
		int  21h

		mov  ah, 4Fh // Find next file
		int  21h

		jnc  short findnext_ok
		mov  retval, -1
		jmp  short findnext_done
	findnext_ok:
		mov  retval, 0
	findnext_done:
	}

	// Find next failed.
	if (retval) {
		return  0;
	}

	// Copy found filename to result path.
	_memcpy(g_find.dirdelim, g_find.dta.name, FILENAME_LEN);

	return g_find.path;
}

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

// Write given source buffer to file. Returns a non-zero value on errors unless fatal is set.
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

// Write given source buffer to file, handle errors as fatal.
short file_write_fatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length)
{
	return file_write(filename, srcoff, srcseg, length, 1);
}

// Write given source buffer to file, returns a non-zero value on error.
short file_write_nofatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length)
{
	return file_write(filename, srcoff, srcseg, length, 0);
}

// Allocates, reads and returns a pointer to the contents of a binary file
void far* file_load_binary(const char* filename, int fatal) {
	void far* memptr;
	int numparas;

	memptr = mmgr_get_unk(filename);
	if (FP_SEG(memptr) != 0) return memptr;
	
	numparas = file_paras(filename, fatal);
	if (numparas == 0) return MK_FP(0, 0);
	memptr = mmgr_alloc_pages(filename, numparas);
	return file_read(filename, FP_OFF(memptr), FP_SEG(memptr), fatal);
}

void far* file_load_binary_nofatal(const char* filename) {
	return file_load_binary(filename, 0);
}

void far* file_load_binary_fatal(const char* filename) {
	return file_load_binary(filename, 1);
}

/*

void far* file_load_resource(int type, const char* filename) {
	void far* result;
	int dearesult;
	while (1) {
		switch (type) {
			case 0:
				// try load the file, if it fails, show a dialog, and retry
				result = file_load_binary_nofatal(filename);
				if (result != 0) return result;
				break;

			case 1:
				return file_load_binary_nofatal(filename);

			case 2:
				// try load a 2d shape and retry if it failed
				result = load_2dshape_nofatal_thunk(filename);
				if (result != 0) return result;
				break;

			case 3:
				// try load a 2d shape and retry if it failed
				result = load_2dshape_res_nofatal_thunk(filename);
				if (result != 0) return result;
				break;

			case 4:
				// try load a song file and retry if it failed
				result = load_song_file(filename);
				if (result != 0) return result;
				break;

			case 5:
				// try load a voice file and retry if it failed
				result = load_voice_file(filename);
				if (result != 0) return result;
				break;

			case 6:
				// try load an sfx file and retry if it failed
				result = load_sfx_file(filename);
				if (result != 0) return result;
				break;

			case 7:
				// try load a compressed file
				return file_decomp_nofatal(filename);

			case 8:
				// try load a 2d shape and retry if it failed
				result = load_2dshape_nofatal2(filename);
				if (result != 0) return result;
				break;
			default:
				break;
		}

		dearesult = do_dea_textres();
		if (dearesult == 2) return 0;
	}
}
*/

void far* file_load_resfile(const char* filename) {
	char name[0x50];
	void far* result;
	
	while (1) {
		strcpy(name, filename);
		strcat(name, ".res");
		
		result = file_load_resource(1, name);
		if (result != 0) return result;
	
		strcpy(name, filename);
		strcat(name, ".pre");
		
		result = file_load_resource(7, name);
		if (result != 0) return result;
			
		do_dea_textres();
	}
}


void far* file_load_3dres(const char* filename) {
	char name[0x50];
	void far* result;
	
	while (1) {
		strcpy(name, filename);
		strcat(name, ".p3s");
		
		result = file_load_resource(7, name);
		if (result != 0) return result;
			
		strcpy(name, filename);
		strcat(name, ".3sh");
		
		result = file_load_resource(1, name);
		if (result != 0) return result;
	
		do_dea_textres();
	}
}

void file_load_audiores(const char* songfile, const char* voicefile, const char* name) {
	void far* audiores;
	voicefileptr = file_load_resource(5, voicefile);
	songfileptr = file_load_resource(4, songfile);
	audiores = init_audio_resources(songfileptr, voicefileptr, name);
	load_audio_finalize(audiores);
	is_audioloaded = 1;
}

