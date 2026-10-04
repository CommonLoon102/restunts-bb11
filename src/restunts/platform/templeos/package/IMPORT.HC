// First Cd to your private data CD root. Existing game files are never replaced.
#include "LEGACY.HH"
#include "VERIFY.HH"

#define TEMPLE_IMPORT_DEFAULT "C:/Restunts"

// Keep the CD source for later RestuntsImport calls in this task.
legacy_u8 *temple_import_source_directory=DirCur;

legacy_void RestuntsImport(legacy_u8 *installation=TEMPLE_IMPORT_DEFAULT)
{
  legacy_u8 *previous=DirCur;
  legacy_u8 *target=DirNameAbs(installation);
  legacy_u8 *program=MStrPrint("%s/PORT/LOAD.HC",target);
  legacy_u8 *source=MStrPrint("%s/DATA",temple_import_source_directory);
  legacy_u8 *data=MStrPrint("%s/DATA",target);
  legacy_u8 *mask=MStrPrint("%s/*",data);
  CDirEntry *existing=NULL;
  legacy_s64 verified=0;
  try {
    if(!FileFind(program,NULL,FUF_JUST_FILES)) {
      Print("Restunts is not installed in %s. Install the public package first.\n",target);
      Print("For another installation, call RestuntsImport(\"D:/Restunts\");\n");
    } else {
      existing=FilesFind(mask,FUF_RECURSE|FUF_JUST_FILES);
      if(existing) {
        Print("Game files already exist in %s. Nothing was copied.\n",data);
        Print("Use an installation with an empty DATA directory to preserve saves.\n");
      } else {
        Print("Importing your game data into %s\n",data);
        verified=TemplePackageCopyVerified(source,data);
        Print("Imported and verified %d files.\n",verified);
        Print("In a fresh task, run:\nCd(\"%s\");\n#include \"PLAY.HC\";\n",target);
      }
    }
  } catch {
    PrintErr("Import incomplete. Existing game files were not replaced.\n"
            "Inspect any partial import at %s before retrying.\n",
        data);
    DirTreeDel(existing);
    Free(mask);
    Free(data);
    Free(source);
    Free(program);
    Free(target);
    Cd(previous);
    Free(previous);
    Fs->catch_except=FALSE;
  }
  DirTreeDel(existing);
  Free(mask);
  Free(data);
  Free(source);
  Free(program);
  Free(target);
  Cd(previous);
  Free(previous);
}

RestuntsImport();
