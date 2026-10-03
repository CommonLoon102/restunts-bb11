// First Cd to the public package root. Existing installations are never replaced.
#include "PORT/LEGACY.HH"
#include "VERIFY.HH"

#define TEMPLE_INSTALL_DEFAULT "C:/Restunts"
#define TEMPLE_INSTALL_EXCEPTION 'Install'

// Keep the package source for later RestuntsInstall calls in this task.
legacy_u8 *temple_package_source_directory = DirCur;

legacy_void RestuntsInstall(legacy_u8 *destination = TEMPLE_INSTALL_DEFAULT)
{
	legacy_u8 *source = temple_package_source_directory;
	legacy_u8 *previous = DirCur;
	legacy_u8 *target = DirNameAbs(destination);
	legacy_u8 *data = MStrPrint("%s/DATA", target);
	legacy_s64 verified = 0;
	try {
		if (FileFind(target)) {
			Print("Destination already exists: %s\n", target);
			Print("Nothing was copied. Choose a new directory, for example:\n"
				  "RestuntsInstall(\"D:/Restunts\");\n");
		} else {
			Print("Installing in %s\n", target);
			verified = TemplePackageCopyVerified(source, target);
			if (!IsDir(data)) {
				PrintErr("The package DATA directory was not copied: %s\n", data);
				throw(TEMPLE_INSTALL_EXCEPTION);
			}
			Print("Installed and verified %d files.\n", verified);
			Print("Copy your own Stunts 1.1 game files into %s before launching.\n", data);
			Print("In a fresh task, run:\nCd(\"%s\");\n#include \"PLAY.HC\";\n", target);
		}
	} catch {
		PrintErr("Installation incomplete. Existing saves were not replaced.\n"
				 "Inspect any partial installation at %s before retrying.\n",
				 target);
		Free(data);
		Free(target);
		Cd(previous);
		Free(previous);
		Fs->catch_except = FALSE;
	}
	Free(data);
	Free(target);
	Cd(previous);
	Free(previous);
}

RestuntsInstall();
