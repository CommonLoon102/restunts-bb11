#ifndef RESTUNTS_PACKAGE_VERIFY
#define RESTUNTS_PACKAGE_VERIFY

#define TEMPLE_PACKAGE_EXCEPTION 'Package'

legacy_u8 TemplePackageCompare(legacy_u8 *source,legacy_u8 *destination)
{
  legacy_u8 *source_data=NULL,*destination_data=NULL;
  legacy_s64 source_length=-1,destination_length=-1;
  legacy_u8 noreg equal=FALSE;
  if(!FileFind(source,NULL,FUF_JUST_FILES)||!FileFind(destination,NULL,FUF_JUST_FILES)) {
    return FALSE;
  }
  try {
    source_data=FileRead(source,&source_length);
    destination_data=FileRead(destination,&destination_length);
    if(source_length>=0&&source_length==destination_length) {
      if(!source_length) {
        equal=TRUE;
      } else if(source_data&&destination_data) {
        equal=!MemCmp(source_data,destination_data,source_length);
      }
    }
  } catch {
    Free(source_data);
    Free(destination_data);
    Fs->catch_except=FALSE;
  }
  Free(source_data);
  Free(destination_data);
  return equal;
}

legacy_s64 TemplePackageCopyVerified(legacy_u8 *source_directory,legacy_u8 *destination_directory)
{
  legacy_u8 *source=DirNameAbs(source_directory);
  legacy_u8 *destination=DirNameAbs(destination_directory);
  legacy_u8 *mask,*copied_name=NULL;
  legacy_s64 source_length=StrLen(source),verified=0;
  CDirEntry *files=NULL,*entry;
  if(source_length&&source[source_length-1]=='/') {
    mask=MStrPrint("%s*",source);
    source_length--;
  } else {
    mask=MStrPrint("%s/*",source);
  }
  try {
    files=FilesFind(mask,FUF_RECURSE|FUF_JUST_FILES);
    if(!files) {
      PrintErr("Cannot read package files from %s\n",source);
      throw(TEMPLE_PACKAGE_EXCEPTION);
    }
    if(!CopyTree(source,destination)) {
      PrintErr("Copy failed. Check the destination drive and free space.\n");
      throw(TEMPLE_PACKAGE_EXCEPTION);
    }
    for(entry=files;entry;entry=entry->next) {
      copied_name=MStrPrint("%s%s",destination,entry->full_name+source_length);
      if(!TemplePackageCompare(entry->full_name,copied_name)) {
        PrintErr("Verification failed: %s\n",copied_name);
        throw(TEMPLE_PACKAGE_EXCEPTION);
      }
      Free(copied_name);
      copied_name=NULL;
      verified++;
    }
  } catch {
    DirTreeDel(files);
    Free(copied_name);
    Free(mask);
    Free(source);
    Free(destination);
    Fs->catch_except=FALSE;
  }
  DirTreeDel(files);
  Free(mask);
  Free(source);
  Free(destination);
  return verified;
}

#endif
