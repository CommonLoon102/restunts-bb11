#include "NATIVE.HH"

#define TEMPLE_FILE_HANDLE_COUNT 64
#define TEMPLE_FILE_FIRST_HANDLE 5
#define TEMPLE_FILE_INVALID_HANDLE 0
#define TEMPLE_FILE_NAME_CAPACITY 13
#define TEMPLE_FILE_GROWTH_MINIMUM 4096
#define TEMPLE_FILE_NOT_FOUND 2
#define TEMPLE_FILE_ACCESS_DENIED 5
#define TEMPLE_FILE_BAD_HANDLE 6
#define TEMPLE_FILE_MEMORY_ERROR 8
#define TEMPLE_FILE_INVALID_SEEK 25
#define TEMPLE_FILE_WRITE_ERROR 29
#define TEMPLE_FILE_OPEN_EXISTING 0
#define TEMPLE_FILE_CREATE 1
#define TEMPLE_FILE_SEEK_BEGIN 0
#define TEMPLE_FILE_SEEK_CURRENT 1
#define TEMPLE_FILE_SEEK_END 2

class CTempleFile
{
  legacy_u8 *data;
  legacy_u8 *path;
  legacy_u64 length,capacity,position;
  legacy_u8 dirty,used;
};

CTempleFile temple_files[TEMPLE_FILE_HANDLE_COUNT];
legacy_s16 temple_file_error=0;
CDirEntry *temple_file_matches=NULL,*temple_file_match=NULL;
legacy_u8 temple_file_match_name[TEMPLE_FILE_NAME_CAPACITY];
MemSet(temple_files,0,sizeof(temple_files));

// Package names are uppercase. Normalize DOS separators as well as case.
legacy_u8 *TempleFilePath(legacy_u8 *source)
{
  legacy_u64 index,length=StrLen(source);
  legacy_u8 *path;
  if(!length||length>=TEMPLE_PATH_CAPACITY) {
    temple_file_error=TEMPLE_FILE_NOT_FOUND;
    return NULL;
  }
  path=MAlloc(length+1);
  for(index=0;index<=length;index++) {
    if(source[index]=='\\') {
      path[index]='/';
    } else {
      path[index]=ToUpper(source[index]);
    }
  }
  return path;
}

CTempleFile *TempleFileHandle(legacy_u16 handle)
{
  if(handle<TEMPLE_FILE_FIRST_HANDLE||handle>=TEMPLE_FILE_HANDLE_COUNT||
      !temple_files[handle].used) {
    temple_file_error=TEMPLE_FILE_BAD_HANDLE;
    return NULL;
  }
  return&temple_files[handle];
}

legacy_u16 RsDosFileOpen(legacy_s8 *source,legacy_s16 create)
{
  legacy_u16 noreg handle;
  legacy_s64 length=0;
  legacy_u8 *path;
  CTempleFile *file;
  temple_file_error=0;
  path=TempleFilePath(source);
  if(!path) {
    return TEMPLE_FILE_INVALID_HANDLE;
  }
  for(handle=TEMPLE_FILE_FIRST_HANDLE;handle<TEMPLE_FILE_HANDLE_COUNT;handle++) {
    if(!temple_files[handle].used) {
      break;
    }
  }
  if(handle==TEMPLE_FILE_HANDLE_COUNT) {
    Free(path);
    temple_file_error=TEMPLE_FILE_BAD_HANDLE;
    return TEMPLE_FILE_INVALID_HANDLE;
  }
  file=&temple_files[handle];
  MemSet(file,0,sizeof(CTempleFile));
  if(create==TEMPLE_FILE_OPEN_EXISTING) {
    if(!FileFind(path,NULL,FUF_JUST_FILES)) {
      Free(path);
      temple_file_error=TEMPLE_FILE_NOT_FOUND;
      return TEMPLE_FILE_INVALID_HANDLE;
    }
    file->data=FileRead(path,&length);
    if(!file->data||length<0) {
      Free(file->data);
      Free(path);
      temple_file_error=TEMPLE_FILE_ACCESS_DENIED;
      return TEMPLE_FILE_INVALID_HANDLE;
    }
    file->length=length;
    file->capacity=length;
  } else {
    file->data=MAlloc(TEMPLE_FILE_GROWTH_MINIMUM);
    file->capacity=TEMPLE_FILE_GROWTH_MINIMUM;
    file->dirty=TRUE;
  }
  file->path=path;
  file->used=TRUE;
  return handle;
}

legacy_s16 RsDosFileClose(legacy_u16 handle)
{
  CTempleFile *file=TempleFileHandle(handle);
  CDirEntry entry;
  legacy_s16 noreg result=0;
  legacy_u8 noreg found;
  if(!file) {
    return-1;
  }
  temple_file_error=0;
  if(file->dirty) {
    legacy_s64 cluster=FileWrite(file->path,file->data,file->length);
    found=FileFind(file->path,&entry,FUF_JUST_FILES);
    if(!cluster||!found||entry.size!=file->length) {
      temple_file_error=TEMPLE_FILE_WRITE_ERROR;
      result=-1;
    }
    if(found) {
      Free(entry.full_name);
    }
  }
  Free(file->data);
  Free(file->path);
  MemSet(file,0,sizeof(CTempleFile));
  return result;
}

legacy_u16 RsDosFileRead(legacy_u16 handle,legacy_u8 *destination,legacy_u16 length)
{
  CTempleFile *file=TempleFileHandle(handle);
  legacy_u64 count;
  if(!file) {
    return TEMPLE_FILE_INVALID_HANDLE;
  }
  temple_file_error=0;
  if(file->position>=file->length) {
    return 0;
  }
  count=MinU64(length,file->length-file->position);
  MemCpy(destination,file->data+file->position,count);
  file->position+=count;
  return count;
}

legacy_u16 RsDosFileWrite(legacy_u16 handle,legacy_u8 *source,legacy_u16 length)
{
  CTempleFile *file=TempleFileHandle(handle);
  legacy_u64 end,capacity;
  legacy_u8 *data;
  if(!file) {
    return TEMPLE_FILE_INVALID_HANDLE;
  }
  temple_file_error=0;
  end=file->position+length;
  if(end>file->capacity) {
    capacity=MaxU64(end,file->capacity+TEMPLE_FILE_GROWTH_MINIMUM);
    data=MAlloc(capacity);
    if(!data) {
      temple_file_error=TEMPLE_FILE_MEMORY_ERROR;
      return TEMPLE_FILE_INVALID_HANDLE;
    }
    MemCpy(data,file->data,file->length);
    Free(file->data);
    file->data=data;
    file->capacity=capacity;
  }
  if(file->position>file->length) {
    MemSet(file->data+file->length,0,file->position-file->length);
  }
  MemCpy(file->data+file->position,source,length);
  file->position=end;
  file->length=MaxU64(file->length,end);
  file->dirty=TRUE;
  return length;
}

legacy_s16 RsDosFileSeek(legacy_u16 handle,legacy_s32 offset,legacy_s16 origin)
{
  CTempleFile *file=TempleFileHandle(handle);
  legacy_s64 position=offset;
  if(!file) {
    return-1;
  }
  if(origin==TEMPLE_FILE_SEEK_CURRENT) {
    position+=file->position;
  } else if(origin==TEMPLE_FILE_SEEK_END) {
    position+=file->length;
  } else if(origin!=TEMPLE_FILE_SEEK_BEGIN) {
    temple_file_error=TEMPLE_FILE_INVALID_SEEK;
    return-1;
  }
  if(position<0||position>I32_MAX) {
    temple_file_error=TEMPLE_FILE_INVALID_SEEK;
    return-1;
  }
  temple_file_error=0;
  file->position=position;
  return 0;
}

legacy_s32 RsDosFileTell(legacy_u16 handle)
{
  CTempleFile *file=TempleFileHandle(handle);
  if(!file) {
    return-1;
  }
  return file->position;
}

legacy_s16 RsDosFileError()
{
  legacy_s16 noreg result=temple_file_error;
  temple_file_error=0;
  return result;
}

legacy_s16 RsDosFileRemove(legacy_s8 *source)
{
  legacy_u8 *path=TempleFilePath(source);
  legacy_s16 noreg result=-1;
  if(path) {
    // Do not pass DOS wildcards to a destructive filesystem operation.
    if(!StrOcc(path,'*')&&!StrOcc(path,'?')&&FileFind(path,NULL,FUF_JUST_FILES)) {
      if(Del(path)==1) {
        result=0;
      }
    }
    Free(path);
  }
  if(result) {
    temple_file_error=TEMPLE_FILE_ACCESS_DENIED;
  } else {
    temple_file_error=0;
  }
  return result;
}

legacy_s8 *RsDosFileFindNext()
{
  while(temple_file_match) {
    CDirEntry *entry=temple_file_match;
    temple_file_match=entry->next;
    if(StrLen(entry->name)<TEMPLE_FILE_NAME_CAPACITY) {
      StrCpy(temple_file_match_name,entry->name);
      return temple_file_match_name;
    }
  }
  return NULL;
}

legacy_s8 *RsDosFileFindFirst(legacy_s8 *query)
{
  legacy_u8 *path=TempleFilePath(query);
  DirTreeDel(temple_file_matches);
  temple_file_matches=NULL;
  temple_file_match=NULL;
  if(!path) {
    return NULL;
  }
  temple_file_matches=FilesFind(path,FUF_JUST_FILES);
  temple_file_match=temple_file_matches;
  Free(path);
  return RsDosFileFindNext();
}

legacy_void TempleFileShutdown()
{
  legacy_u16 noreg handle;
  for(handle=TEMPLE_FILE_FIRST_HANDLE;handle<TEMPLE_FILE_HANDLE_COUNT;handle++) {
    if(temple_files[handle].used) {
      RsDosFileClose(handle);
    }
  }
  DirTreeDel(temple_file_matches);
  temple_file_matches=NULL;
  temple_file_match=NULL;
}
