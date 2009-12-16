ASM = tasm
CC = bcc /c
LINK = tlink
SED = sed

OBJDIR = ..\build

$(OBJDIR)\math.obj: math.c
	$(CC) -n$(OBJDIR) $? 

clean:
	del *.obj
