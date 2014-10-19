!ifndef TARGET
TARGET=ported
!endif

RESTUNTS_OBJDIR = ../c/build/dos
RESTUNTS_OBJFILES = $(RESTUNTS_OBJDIR)\fileio.obj $(RESTUNTS_OBJDIR)\math.obj $(RESTUNTS_OBJDIR)\memmgr.obj $(RESTUNTS_OBJDIR)\restunts.obj $(RESTUNTS_OBJDIR)\shape3d.obj $(RESTUNTS_OBJDIR)\shape2d.obj $(RESTUNTS_OBJDIR)\keyboard.obj $(RESTUNTS_OBJDIR)\heapsort.obj

ASM_DIR = ../asm
ASM_OBJDIR = ../asm/build
ASM_OBJFILES = $(ASM_OBJDIR)\seg041.obj $(ASM_OBJDIR)\seg000.obj $(ASM_OBJDIR)\seg001.obj $(ASM_OBJDIR)\seg002.obj $(ASM_OBJDIR)\seg003.obj $(ASM_OBJDIR)\seg004.obj $(ASM_OBJDIR)\seg005.obj $(ASM_OBJDIR)\seg006.obj $(ASM_OBJDIR)\seg007.obj $(ASM_OBJDIR)\seg008.obj $(ASM_OBJDIR)\seg009.obj $(ASM_OBJDIR)\seg010.obj $(ASM_OBJDIR)\seg011.obj $(ASM_OBJDIR)\seg012.obj $(ASM_OBJDIR)\seg013.obj $(ASM_OBJDIR)\seg014.obj $(ASM_OBJDIR)\seg015.obj $(ASM_OBJDIR)\seg016.obj $(ASM_OBJDIR)\seg017.obj $(ASM_OBJDIR)\seg018.obj $(ASM_OBJDIR)\seg019.obj $(ASM_OBJDIR)\seg020.obj $(ASM_OBJDIR)\seg021.obj $(ASM_OBJDIR)\seg022.obj $(ASM_OBJDIR)\seg023.obj $(ASM_OBJDIR)\seg024.obj $(ASM_OBJDIR)\seg025.obj $(ASM_OBJDIR)\seg026.obj $(ASM_OBJDIR)\seg027.obj $(ASM_OBJDIR)\seg028.obj $(ASM_OBJDIR)\seg029.obj $(ASM_OBJDIR)\seg030.obj $(ASM_OBJDIR)\seg031.obj $(ASM_OBJDIR)\seg032.obj $(ASM_OBJDIR)\seg033.obj $(ASM_OBJDIR)\seg034.obj $(ASM_OBJDIR)\seg035.obj $(ASM_OBJDIR)\seg036.obj $(ASM_OBJDIR)\seg037.obj $(ASM_OBJDIR)\seg038.obj $(ASM_OBJDIR)\seg039.obj $(ASM_OBJDIR)\dseg.obj

ASMORIG_DIR = ../asmorig
ASMORIG_OBJDIR = ../asmorig/build
ASMORIG_OBJFILES = $(ASMORIG_OBJDIR)\seg041.obj $(ASMORIG_OBJDIR)\seg000.obj $(ASMORIG_OBJDIR)\seg001.obj $(ASMORIG_OBJDIR)\seg002.obj $(ASMORIG_OBJDIR)\seg003.obj $(ASMORIG_OBJDIR)\seg004.obj $(ASMORIG_OBJDIR)\seg005.obj $(ASMORIG_OBJDIR)\seg006.obj $(ASMORIG_OBJDIR)\seg007.obj $(ASMORIG_OBJDIR)\seg008.obj $(ASMORIG_OBJDIR)\seg009.obj $(ASMORIG_OBJDIR)\seg010.obj $(ASMORIG_OBJDIR)\seg011.obj $(ASMORIG_OBJDIR)\seg012.obj $(ASMORIG_OBJDIR)\seg013.obj $(ASMORIG_OBJDIR)\seg014.obj $(ASMORIG_OBJDIR)\seg015.obj $(ASMORIG_OBJDIR)\seg016.obj $(ASMORIG_OBJDIR)\seg017.obj $(ASMORIG_OBJDIR)\seg018.obj $(ASMORIG_OBJDIR)\seg019.obj $(ASMORIG_OBJDIR)\seg020.obj $(ASMORIG_OBJDIR)\seg021.obj $(ASMORIG_OBJDIR)\seg022.obj $(ASMORIG_OBJDIR)\seg023.obj $(ASMORIG_OBJDIR)\seg024.obj $(ASMORIG_OBJDIR)\seg025.obj $(ASMORIG_OBJDIR)\seg026.obj $(ASMORIG_OBJDIR)\seg027.obj $(ASMORIG_OBJDIR)\seg028.obj $(ASMORIG_OBJDIR)\seg029.obj $(ASMORIG_OBJDIR)\seg030.obj $(ASMORIG_OBJDIR)\seg031.obj $(ASMORIG_OBJDIR)\seg032.obj $(ASMORIG_OBJDIR)\seg033.obj $(ASMORIG_OBJDIR)\seg034.obj $(ASMORIG_OBJDIR)\seg035.obj $(ASMORIG_OBJDIR)\seg036.obj $(ASMORIG_OBJDIR)\seg037.obj $(ASMORIG_OBJDIR)\seg038.obj $(ASMORIG_OBJDIR)\seg039.obj $(ASMORIG_OBJDIR)\dseg.obj

OBJDIR = build\$(TARGET)
RESTUNTS_CFLAGS = /c /u- /mm /B /n$(OBJDIR) /DRESTUNTS_DOS

!if ($(TARGET)==ported)

CTARGETS = $(RESTUNTS_OBJFILES) $(ASM_OBJFILES) $(OBJDIR)\main.obj
OUTFILE = restunts.exe
CFLAGS = 

!elif ($(TARGET)==original)

CTARGETS = $(ASMORIG_OBJFILES) $(OBJDIR)\main.obj
CFLAGS = /DRESTUNTS_ORIGINAL
OUTFILE = restunto.exe

!else
!error Unsupported DOS target: $(TARGET)
!endif

ASM = tasm /m2 /s
CC = bcc  $(RESTUNTS_CFLAGS) $(CFLAGS)
LINK = wlink

restunts: $(OBJDIR)\main.obj
	$(LINK) @&&|
name $(OUTFILE)
system dos
option nocase
option packcode=0
option packdata=0
option map
option nodefaultlibs
file { $(CTARGETS) }
library cm.lib
order
	clname STUNTSC
	clname STUNTSD
	clname STACK
	clname DATA
	clname BSS
	clname CODE
	clname ENDSEG
|
	copy $(OUTFILE) s:\stunts

$(OBJDIR)\main.obj: main.c
	$(CC) -I../c $?

clean:
	del build\*.obj
	del build\ported\*.obj
	del build\original\*.obj	
	del restunts.exe
	del restunts.map
	del restunto.exe
	del restunto.map
