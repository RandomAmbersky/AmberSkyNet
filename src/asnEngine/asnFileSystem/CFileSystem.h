#ifndef _C_FILESYSTEM_
#define _C_FILESYSTEM_

#include "IFileSystem.h"
#include "IFileSource.h"
#include "IEngine.h"

#include "CBaseObject.h"

#include <vector>
#include <map>

class CFileSystem: public IFileSystem, public CBaseObject {
public:
CFileSystem(IEngine *_Engine);
~CFileSystem();

void InitManager();
int Release();
void Update(float tms);

bool AddPath(const char* Path, bool Recursive);
IFile *GetFile(const char* FileName);

// добавл€ем источник файлов
bool AddFileSource (const char* FileSourceName);
bool AddCustomFileSource (const char* FileSourceName,void *FileSource);
//bool AddFileSource (const char* FileSourceName,void *FileSource);

// спецом дл€ CRemoteFile, просто пытаемс€ скачать с удаленных источников не сохран€€ ни в кеше, нигде
IFile* GetFileRemote(const char* FileName);

protected:
IFile *FindFile(const std::string& FileName);
IFile *LoadFromCache(const std::string& FileName);
IFile *LoadFromSources(const std::string& FileName);

//IEngine *Engine;
std::string SandBox;

std::vector<std::string> Paths; // заданные в AddPath пути
std::vector<std::string> RemotePaths; // заданные в AddPath пути с указанием протокола <протокол>://<путь>

std::map<std::string,IFileSource *> FileSources; //источники файлов
std::map<std::string,IFileSource *> RemoteFileSources; //источники файлов
std::map<std::string,IFile *> FileList; //кэш открытых файлов
};

#endif
