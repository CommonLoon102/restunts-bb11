// Validates the original game's compressed version resource in C:/DATA.
#include "LOAD.HC"

#define TEMPLE_RESOURCE_TEST_DIRECTORY "C:/DATA"
#define TEMPLE_RESOURCE_TEST_PACKED "misc.pre"
#define TEMPLE_RESOURCE_TEST_MISSING "TEMPLE-NOT-A-GAME-FILE.RES"
#define TEMPLE_RESOURCE_TEST_MISSING_LENGTH (-1)
#define TEMPLE_RESOURCE_TEST_OUTPUT "TEMPLE-IO.BIN"
#define TEMPLE_RESOURCE_TEST_PAGE_BYTES 16384
#define TEMPLE_RESOURCE_TEST_EXTRA_BYTES 37
#define TEMPLE_RESOURCE_TEST_WRITE_BYTES                                                           \
  (TEMPLE_RESOURCE_TEST_PAGE_BYTES+TEMPLE_RESOURCE_TEST_EXTRA_BYTES)
#define TEMPLE_RESOURCE_TEST_VERSION_ID "gver"
#define TEMPLE_RESOURCE_TEST_VERSION "Version 1.1 (Feb 12 1991)"

legacy_void TempleResourcesAssert(legacy_s64 condition,legacy_u8 *message)
{
  if(!condition) {
    PrintErr("Resource integration: %s\n",message);
    throw('Resource');
  }
}

legacy_void TempleResourcesTest()
{
  legacy_s32 length;
  legacy_u8 *resource;
  legacy_u16 matches;
  legacy_u64 index;
  legacy_u8 *write_source,*read_result;
  RsFullDataInitialize();
  RsMmgrInitConventionalArena();
  TempleResourcesAssert(Cd(TEMPLE_RESOURCE_TEST_DIRECTORY),"game data directory");
  TempleResourcesAssert(RsFileioFileGameResourceSize(TEMPLE_RESOURCE_TEST_MISSING,FALSE)==
          TEMPLE_RESOURCE_TEST_MISSING_LENGTH,
      "missing file must preserve compressed fallback");
  length=RsFileioFileGameResourceSize(TEMPLE_RESOURCE_TEST_PACKED,TRUE);
  TempleResourcesAssert(length>0,"compressed resource size");
  resource=RsFileDecompNofatal(TEMPLE_RESOURCE_TEST_PACKED);
  TempleResourcesAssert(resource!=NULL,"compressed resource decoder");
  matches=RsResourceTextEquals(
      resource,length,TEMPLE_RESOURCE_TEST_VERSION_ID,TEMPLE_RESOURCE_TEST_VERSION);
  RsMmgrRelease(resource);
  TempleResourcesAssert(matches,"decoded version directory and text");
  TempleResourcesAssert(RsFileGameVersionMatches(),"startup version probe");
  write_source=MAlloc(TEMPLE_RESOURCE_TEST_WRITE_BYTES);
  read_result=MAlloc(TEMPLE_RESOURCE_TEST_WRITE_BYTES);
  for(index=0;index<TEMPLE_RESOURCE_TEST_WRITE_BYTES;index++) {
    write_source[index]=index^(index>>TEMPLE_BYTE_BITS);
  }
  TempleResourcesAssert(RsFileWriteNofatal(TEMPLE_RESOURCE_TEST_OUTPUT,write_source,
                            TEMPLE_RESOURCE_TEST_WRITE_BYTES)==0,
      "native buffer write across file page");
  TempleResourcesAssert(RsFileRead(TEMPLE_RESOURCE_TEST_OUTPUT,read_result,FALSE)!=NULL,
      "native buffer read across file page");
  TempleResourcesAssert(MemCmp(write_source,read_result,TEMPLE_RESOURCE_TEST_WRITE_BYTES)==0,
      "native file round trip");
  RsDosFileRemove(TEMPLE_RESOURCE_TEST_OUTPUT);
  Free(read_result);
  Free(write_source);
  Print("Resource integration passed.\n");
}

TempleResourcesTest();
