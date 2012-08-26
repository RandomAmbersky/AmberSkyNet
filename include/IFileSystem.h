#ifndef _I_FILESYSTEM_
#define _I_FILESYSTEM_

#include "IFile.h"
#include "IManager.h"

class IFileSystem: public IManager {
public:
//virtual ~IFileSystem(){};
// добавить источник файлов
virtual bool AddPath(const char* Path, bool Recursive)=0;
// открыть файл
virtual IFile *GetFile(const char* FileName)=0;
// добавляем источник файлов
virtual bool AddFileSource (const char* FileSourceName)=0;
virtual bool AddCustomFileSource (const char* FileSourceName,void *FileSource)=0;
};

#define FILER ((IFileSystem*)ENGINE->GetPtrParam("FileManager"))

#endif
