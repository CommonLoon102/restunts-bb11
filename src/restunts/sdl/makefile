SDLPATH = s:\src\sdl
CC = cl /MD /c /I$(SDLPATH)\include /I../c /DRESTUNTS_SDL
LINK = cl
LINKFLAGS = /link /LIBPATH:$(SDLPATH)\lib /SUBSYSTEM:console
OBJDIR = build

CTARGETS = $(OBJDIR)\fileio.obj
#$(OBJDIR)\fileio.obj $(OBJDIR)\math.obj $(OBJDIR)\memmgr.obj $(OBJDIR)\restunts.obj $(OBJDIR)\shape3d.obj $(OBJDIR)\shape2d.obj
PROJTARGETS = $(OBJDIR)\main.obj sdl.lib sdlmain.lib

restunts-sdl.exe: $(OBJDIR)\main.obj cfiles
	$(LINK) $(PROJTARGETS) $(CTARGETS) $(LINKFLAGS) /OUT:$<

$(OBJDIR)\main.obj: main.cpp
	$(CC) /Fo$< main.cpp

cfiles:
	cd ..\c
	if exist makefile make /DTARGET=sdl
	cd ..\sdl

clean:
	del $(OBJDIR)\main.obj
	cd ..\c
	if exist makefile make /DTARGET=sdl clean
	cd ..\sdl
	del restunts-sdl.exe
	del restunts-sdl.map
