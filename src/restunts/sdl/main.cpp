#include <map>
#include <string>
#include "SDL.h"
extern "C" {
#include "fileio.h"
#include "memmgr.h"
}

/* Function Prototypes */
void PrintKeyInfo( SDL_KeyboardEvent *key );
void PrintModifiers( SDLMod mod );

int game_thread(void* param);

extern "C" {

int stuntsmain(int argc, char** argv);

void far* fontnptr;
void far* fontdefptr;
void far* mainresptr;
short is_audioloaded;
void far* songfileptr;
void far* voicefileptr;

void fatal_error(const char* fmterr, ...) {
	printf(fmterr);
}

void far* init_audio_resources(void far* songptr, void far* voiceptr, const char* name) {
	return 0;
}

void load_audio_finalize(void far* audiores) {
}

void far* load_song_file(const char* filename) {
	return 0;
}

void far* load_voice_file(const char* filename) {
	return 0;
}

void far* load_sfx_file(const char* filename) {
	return 0;
}

void far* file_load_shape2d_nofatal_thunk(const char* filename) {
	return 0;
}

void far* file_load_shape2d_res_nofatal_thunk(const char* filename) {
	return 0;
}

void far* file_load_shape2d_nofatal(char* shapename) {
	return 0;
}

void far* file_load_shape2d_nofatal2(char* shapename) {
	return 0;
}

int do_dea_textres(void) {
	printf("do_dea_textres() (insert disk)\n");
	return 0;
}

unsigned long file_decomp_rle(void far* src, void far* dst, unsigned paras) {
	printf("file_decomp_rle() not implemented!\n");
	return 0;
}
/*
void copy_paras_reverse(unsigned short srcseg, unsigned short destseg, short paras) {
	printf("copy_paras_reverse() not implemented!\n");
}

unsigned short mmgr_resize_memory(unsigned short arg_0, unsigned short arg_2, unsigned short arg_4) {
	printf("mmgr_resize_memory() not implemented!\n");
	return 0;
}

std::map<std::string, char*> memchunks;

void far* mmgr_alloc_pages(const char* arg_0, unsigned short arg_2) {
	printf("alloc pages %s\n", arg_0);
	char* chunk = new char[arg_2 * 16];
	memchunks[arg_0] = chunk;
	return chunk;
}

void far* mmgr_get_chunk_by_name(const char* arg_0) {
	printf("get chunk by name %s\n", arg_0);
	std::map<std::string, char*>::iterator i = memchunks.find(arg_0);
	if (i == memchunks.end()) return 0;
	return i->second;
}

void far* mmgr_free(char far* ptr) {
	printf("mmgr_free() not implemented!\n");
	return 0;
}

unsigned long mmgr_get_res_ofs_diff_scaled() {
	printf("mmgr_get_res_ofs_diff_scaled() not implemented!\n");
	return 0xFDE8;
}

*/
extern void shape3d_load_car_shapes(char* carid, char* oppcarid);
void shape3d_load_all();

const char* aSInvalidPackTy = "Invalid Pack Type";

const char* aSFileError = "File error";
const char* aSFileError_0 = "File error 0";
const char* aSFileError_1 = "File error 1";
const char* aLocateshape4_4sShapeNotF = "shape not found %-4.4s";
const char* aLocatesound4_4sSoundNotF = "sound not found %-4.4s";
char textresprefix = 'e';

} // extern "C"


struct stunts_engine {
	SDL_Thread* thread;
	int argc;
	char** argv;
	int quit;
	
	stunts_engine(int _argc, char** _argv) {
		argc = _argc;
		argv = _argv;
		quit = 0;
	}
	
	bool init() {
		/* Initialise SDL */
		if( SDL_Init( SDL_INIT_VIDEO ) < 0){
			fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
			return false;
		}
	
		/* Set a video mode */
		if( !SDL_SetVideoMode( 320, 200, 0, 0 ) ){
			fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
			SDL_Quit();
			return false;
		}
	
		/* Enable Unicode translation */
		SDL_EnableUNICODE( 1 );
		return true;
	}
	
	void uninit() {
		SDL_Quit();
	}
	
	void run() {
		thread = SDL_CreateThread(game_thread, this);

		SDL_Event event;
		while( !quit ){
			while( SDL_PollEvent( &event ) ){
				switch( event.type ){
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						PrintKeyInfo( &event.key );
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					default:
						break;
				}
			}
		}
	}
	
	int run_thread() {
		// init memgr - called from init_video()
		mmgr_alloc_a000();
		
		printf("hello from game thread\n");
		mainresptr = file_load_resfile("main");
		fontdefptr = file_load_resource(0, "fontdef.fnt");
		fontnptr = file_load_resource(0, "fontn.fnt");

		shape3d_load_all();
		
		//void* tempdataptr = file_load_resource(2, "sdtitl");
		//void* titlshapeptr = locate_shape_fatal(tempdataptr, "titl");
		
		// font_set_fontdef();
		return 0;//stuntsmain(argc, argv);
	}
};


int game_thread(void* param) {
	stunts_engine* stunts = (stunts_engine*)param;
	return stunts->run_thread();
}

/* main */
int main( int argc, char *argv[] ){

	stunts_engine stunts(argc, argv);
	if (stunts.init())
		stunts.run();
	stunts.uninit();
	return 0;
}

/* Print all information about a key event */
void PrintKeyInfo( SDL_KeyboardEvent *key ){
	/* Is it a release or a press? */
	if( key->type == SDL_KEYUP )
		printf( "Release:- " );
	else
		printf( "Press:- " );

	/* Print the hardware scancode first */
	printf( "Scancode: 0x%02X", key->keysym.scancode );
	/* Print the name of the key */
	printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );
	/* We want to print the unicode info, but we need to make */
	/* sure its a press event first (remember, release events */
	/* don't have unicode info                                */
	if( key->type == SDL_KEYDOWN ){
		/* If the Unicode value is less than 0x80 then the    */
		/* unicode value can be used to get a printable       */
		/* representation of the key, using (char)unicode.    */
		printf(", Unicode: " );
		if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
			printf( "%c (0x%04X)", (char)key->keysym.unicode,
					key->keysym.unicode );
		}
		else{
			printf( "? (0x%04X)", key->keysym.unicode );
		}
	}
	printf( "\n" );
	/* Print modifier info */
	PrintModifiers( key->keysym.mod );
}

/* Print modifier info */
void PrintModifiers( SDLMod mod ){
	printf( "Modifers: " );

	/* If there are none then say so and return */
	if( mod == KMOD_NONE ){
		printf( "None\n" );
		return;
	}

	/* Check for the presence of each SDLMod value */
	/* This looks messy, but there really isn't    */
	/* a clearer way.                              */
	if( mod & KMOD_NUM ) printf( "NUMLOCK " );
	if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
	if( mod & KMOD_LCTRL ) printf( "LCTRL " );
	if( mod & KMOD_RCTRL ) printf( "RCTRL " );
	if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
	if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
	if( mod & KMOD_RALT ) printf( "RALT " );
	if( mod & KMOD_LALT ) printf( "LALT " );
	if( mod & KMOD_CTRL ) printf( "CTRL " );
	if( mod & KMOD_SHIFT ) printf( "SHIFT " );
	if( mod & KMOD_ALT ) printf( "ALT " );
	printf( "\n" );
}