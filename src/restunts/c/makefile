ASM = tasm
CC = bcc /c /u- /mm
LINK = tlink
SED = sed

OBJDIR = ..\build

all: $(OBJDIR)\fileio.obj $(OBJDIR)\math.obj $(OBJDIR)\memmgr.obj

$(OBJDIR)\fileio.obj: fileio.c
	$(CC) -n$(OBJDIR) $?

$(OBJDIR)\math.obj: math.c
	$(CC) -n$(OBJDIR) $? 

$(OBJDIR)\memmgr.obj: memmgr.c
	$(CC) -n$(OBJDIR) $? 

clean:
	del $(OBJDIR)\fileio.obj
	del $(OBJDIR)\math.obj
	del $(OBJDIR)\memmgr.obj

