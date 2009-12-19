ASM = tasm
CC = bcc /c /u- /mm
LINK = tlink
SED = sed

OBJDIR = ..\build

all: $(OBJDIR)\math.obj $(OBJDIR)\resource.obj

$(OBJDIR)\math.obj: math.c
	$(CC) -n$(OBJDIR) $? 

$(OBJDIR)\resource.obj: resource.c
	$(CC) -n$(OBJDIR) $? 

clean:
	del $(OBJDIR)\math.obj
	del $(OBJDIR)\resource.obj

