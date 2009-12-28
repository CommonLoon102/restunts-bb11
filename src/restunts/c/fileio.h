#ifndef RESTUNTS_FILEIO_H
#define RESTUNTS_FILEIO_H

const char* file_find(const char* query);
const char* file_find_next();

unsigned short file_paras(const char* filename, int fatal);
unsigned short file_paras_fatal(const char* filename);
unsigned short file_paras_nofatal(const char* filename);

unsigned short file_uncomp_paras(const char* filename, int fatal);
unsigned short file_uncomp_paras_fatal(const char* filename);
unsigned short file_uncomp_paras_nofatal(const char* filename);

char far* file_read(const char* filename, unsigned short dstoff, unsigned short dstseg, int fatal);
char far* file_read_fatal(const char* filename, unsigned short dstoff, unsigned short dstseg);
char far* file_read_nofatal(const char* filename, unsigned short dstoff, unsigned short dstseg);

short file_write(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length, int fatal);
short file_write_fatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length);
short file_write_nofatal(const char* filename, unsigned short srcoff, unsigned short srcseg, unsigned long length);

void far* file_load_resfile(const char* filename);
void far* file_load_resource(int type, const char* filename);
void file_load_audiores(const char* songfile, const char* voicefile, const char* name);

#endif
