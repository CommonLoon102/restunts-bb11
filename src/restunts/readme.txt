the file game.idb, an ida 5.2 project, is missing for legal reasons

directories:
	asm - contains asm files from the original stunts executable
	c - contains ported c code
	crt - contains linkable binaries for medium model startup and crt from borland c++ 3.1
	build - all the makefiles put intermediate obj files here


tools used:

	- tasm 4.1 (dos, from borland c 3.1)
	- borland make 5.2 (win32, from borland c++ 5.5, patched binary)
	- wlink 1.8 (win32, from open watcom c/c++ 1.8)
	- ida 5.2 (win32)
	- borland c++ 3.1 (dos)
	- a custom .idc script for ida that generates fixed asm for recompilation by tasm


notes regarding make:

borland make was chosen because the first makefile was written for the dos version. ultimately the
dos version ran into memory problems with the largest asm-files. too late and too lazy to find a 
new make utility, the windows version of borland make, included in borland c++ 5.5, was used 
instead. but:

borland make 5.2 uses MAKEFILE.@@@ as the filename for temporary inline files.
wlink interprets @ in a special way on the command line and will not take the inline file as a parameter.
as such make.exe was patched to use MAKEFILE.!!! as the temporary filename instead.


notes regarding wlink:

wlink from the open watcom c/c++ compiler suite was chosen because it supports detailed control on
how to order segment classes in the final executable image.

our requirement is to put the original code and data first in the file, exactly as in the original 
game, only patching up function calls and data access. 
tlink and optlink were originally used, but would insist on putting the CODE and DATA segment 
classes first in the executable, conflicting with the crt, and in particular the default segment 
class names. the final executable would then crash 


notes regarding the crt:

wlink was not able to link with the supplied cm.lib from borland c++ 3.1, but was able to link
with the separate obj files.


porting a function from asm to c:

1. anders.idc needs to be aware of all the functions that have been ported, so it generates asm that does not conflict with the ported code.
add a line in PortFuncName().

2. regenerate the asm files from within ida (or rename the function manually)

3. add a function stub to one of the existing .c files in the c directory
3.1 if you want to add a new .c-file to the project, you need to 
	- add a target for it in c/makefile
	- add a reference to the obj in  CTARGETS in makefile

4. if you need something in the crt that isnt linked yet, add references to required crt .objs in the makefile after the libfile directive for wlink.

