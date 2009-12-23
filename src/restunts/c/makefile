ASM = tasm
CC = bcc /c /u- /mm
LINK = tlink
SED = sed

OBJDIR = ..\build

all: $(OBJDIR)\fileio.obj $(OBJDIR)\math.obj $(OBJDIR)\resource.obj

$(OBJDIR)\fileio.obj: fileio.c
	$(CC) -n$(OBJDIR) $?

$(OBJDIR)\math.obj: math.c
	$(CC) -n$(OBJDIR) $? 

$(OBJDIR)\resource.obj: resource.c
	$(CC) -n$(OBJDIR) $? 

clean:
	del $(OBJDIR)\fileio.obj
	del $(OBJDIR)\math.obj
	del $(OBJDIR)\resource.obj

