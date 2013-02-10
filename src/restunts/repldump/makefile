TARGET=dos
OBJDIR = ..\$(TARGET)\build

ASM = tasm /m2 /s
CC = bcc /c /u- /mm /B /n$(OBJDIR) /DRESTUNTS_DOS
LINK = wlink

ASMTARGETS = $(OBJDIR)\seg041.obj $(OBJDIR)\seg000.obj $(OBJDIR)\seg001.obj $(OBJDIR)\seg002.obj $(OBJDIR)\seg003.obj $(OBJDIR)\seg004.obj $(OBJDIR)\seg005.obj $(OBJDIR)\seg006.obj $(OBJDIR)\seg007.obj $(OBJDIR)\seg008.obj $(OBJDIR)\seg009.obj $(OBJDIR)\seg010.obj $(OBJDIR)\seg011.obj $(OBJDIR)\seg012.obj $(OBJDIR)\seg013.obj $(OBJDIR)\seg014.obj $(OBJDIR)\seg015.obj $(OBJDIR)\seg016.obj $(OBJDIR)\seg017.obj $(OBJDIR)\seg018.obj $(OBJDIR)\seg019.obj $(OBJDIR)\seg020.obj $(OBJDIR)\seg021.obj $(OBJDIR)\seg022.obj $(OBJDIR)\seg023.obj $(OBJDIR)\seg024.obj $(OBJDIR)\seg025.obj $(OBJDIR)\seg026.obj $(OBJDIR)\seg027.obj $(OBJDIR)\seg028.obj $(OBJDIR)\seg029.obj $(OBJDIR)\seg030.obj $(OBJDIR)\seg031.obj $(OBJDIR)\seg032.obj $(OBJDIR)\seg033.obj $(OBJDIR)\seg034.obj $(OBJDIR)\seg035.obj $(OBJDIR)\seg036.obj $(OBJDIR)\seg037.obj $(OBJDIR)\seg038.obj $(OBJDIR)\seg039.obj $(OBJDIR)\dseg.obj
CTARGETS = $(OBJDIR)\repldump.obj

repldump.exe: asmfiles $(OBJDIR)\repldump.obj
	$(LINK) @&&|
name repldump
system dos
option nocase
option packcode=0
option packdata=0
option map
option nodefaultlibs
file { $(ASMTARGETS) $(CTARGETS) }
library cm.lib
#libfile c0m.obj
order
	clname STUNTSC
	clname STUNTSD
	clname STACK
	clname DATA
	clname BSS
	clname CODE
	clname ENDSEG
|
	copy repldump.exe s:\stunts

asmfiles:
	cd ..\asm
        if exist makefile make /DTARGET=dos
	cd ..\repldump

$(OBJDIR)\repldump.obj: repldump.c
	$(CC) $?

clean:
	del $(OBJDIR)\repldump.obj
	cd ..\asm
	if exist makefile make /DTARGET=dos clean
	cd ..\c
	if exist makefile make /DTARGET=dos clean
	cd ..\repldump
	del repldump.exe
	del repldump.map
