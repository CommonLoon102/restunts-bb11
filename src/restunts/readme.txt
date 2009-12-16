the file game.idb, an ida 5.2 project, is missing for legal reasons

directories:
	asm - contains asm files from the original stunts executable
	c - contains ported c code
	crt - contains linkable binaries for medium model startup and crt from borland c++ 3.1
	build - all the makefiles put intermediate obj files here


tools used:

	- tasm 4.1 (dos, warez, from borland c 3.1)
	- borland make 5.2 (win32, from borland c++ 5.5, patched)
	- wlink 1.8 (win32, from open watcom c/c++ 1.8)
	- ida 5.2 (win32, warez)


special note regarding make:
borland make 5.2 uses MAKEFILE.@@@ as the filename for temporary inline files.
wlink interprets @ in a special way on the command line and will not take the inline file as a parameter.
as such make.exe was patched to use MAKEFILE.!!! as the temporary filename instead.
