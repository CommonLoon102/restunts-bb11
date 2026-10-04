// In a fresh task, first Cd to the installed package root.
#include "PORT/LEGACY.HH"

#define TEMPLE_PLAY_EXCEPTION 'Play'

legacy_u8 *temple_package_data_directory=NULL;

legacy_u8 TemplePlayHasData(legacy_u8 *directory)
{
  legacy_u8 *resource=MStrPrint("%s/MISC.RES",directory);
  legacy_u8 *packed_resource=MStrPrint("%s/MISC.PRE",directory);
  legacy_u8 present=FALSE;
  if(FileFind(resource,NULL,FUF_JUST_FILES)||FileFind(packed_resource,NULL,FUF_JUST_FILES)) {
    present=TRUE;
  }
  Free(resource);
  Free(packed_resource);
  return present;
}

legacy_void RestuntsPlay()
{
  legacy_u8 *root=DirCur;
  legacy_u8 *previous=DirCur;
  legacy_u8 *port=MStrPrint("%s/PORT",root);
  temple_package_data_directory=MStrPrint("%s/DATA",root);
  try {
    if(!Cd(port)) {
      PrintErr("Cannot open game source directory: %s\n",port);
      throw(TEMPLE_PLAY_EXCEPTION);
    }
    if(!TemplePlayHasData(temple_package_data_directory)) {
      Print("Stunts 1.1 game data is missing.\n"
            "Copy your own game files into %s, then run PLAY.HC again.\n"
            "You can also mount your private data CD and run its IMPORT.HC.\n",
          temple_package_data_directory);
    } else {
      ExeFile2("LOAD.HC");
      ExePrint2("Restunts(temple_package_data_directory);");
    }
  } catch {
    Free(root);
    Free(port);
    Free(temple_package_data_directory);
    temple_package_data_directory=NULL;
    Cd(previous);
    Free(previous);
    Fs->catch_except=FALSE;
  }
  Free(root);
  Free(port);
  Free(temple_package_data_directory);
  temple_package_data_directory=NULL;
  Cd(previous);
  Free(previous);
}

RestuntsPlay();
