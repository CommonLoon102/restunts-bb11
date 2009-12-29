#ifndef RESTUNTS_FILEIO_H
#define RESTUNTS_FILEIO_H

const char* file_find(const char* query);
const char* file_find_next();

unsigned short file_paras(const char* filename, int fatal);
unsigned short file_paras_fatal(const char* filename);
unsigned short file_paras_nofatal(const char* filename);

unsigned short file_decomp_paras(const char* filename, int fatal);
unsigned short file_decomp_paras_fatal(const char* filename);
unsigned short file_decomp_paras_nofatal(const char* filename);

void far* file_read(const char* filename, void far* dst, int fatal);
void far* file_read_fatal(const char* filename, void far* dst);
void far* file_read_nofatal(const char* filename, void far* dst);

short file_write(const char* filename, void far* src, unsigned long length, int fatal);
short file_write_fatal(const char* filename, void far* src, unsigned long length);
short file_write_nofatal(const char* filename, void far* src, unsigned long length);

void far* file_decomp(const char* filename, int fatal);
void far* file_decomp_fatal(const char* filename);
void far* file_decomp_nofatal(const char* filename);

void far* file_load_resfile(const char* filename);
void far* file_load_resource(int type, const char* filename);
void file_load_audiores(const char* songfile, const char* voicefile, const char* name);

#endif
