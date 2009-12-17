ASM = tasm
CC = bcc /c /u- /v /mm
LINK = tlink
SED = sed

OBJDIR = ..\build

$(OBJDIR)\math.obj: math.c
	$(CC) -n$(OBJDIR) $? 

clean:
	del $(OBJDIR)\math.obj

