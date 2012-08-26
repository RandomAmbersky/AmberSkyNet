#ifndef _C_FILE_REMOTE_
#define _C_FILE_REMOTE_

#include "IFile.h"
#include "CFileSystem.h"

#include <vector>

class CFileRemote : public IFile {
friend class CFileSystem;
public:
CFileRemote(CFileSystem* FileSystem);
~CFileRemote();
void Release();
asn_File_State FileOpen();
void FileClose();
void *LoadInMem();
void FreeMem();
void Seek(unsigned int Position, int From);
void ReadBuf(void *buf, unsigned int size);

asn_File_State GetState();
unsigned int GetSizeInBytes();

// теперь фишки самой CFIleSystem

void AddRemotePath(const char* RemPath);
void AddPath(const char* Path);

private:

asn_File_State LoadNext(); // загружаем следующий по списку файл

std::string CurPath; // текущий путь который загружается

std::vector<std::string> Paths; // заданные в AddPath пути
std::vector<std::string> RemotePaths; // заданные в AddPath пути с указанием протокола <протокол>://<путь>

std::vector<std::string>::iterator it_p; // итератор локальных путей
std::vector<std::string>::iterator it_pr; // итератор удаленных путей

IFile* tempFile; // временный файл из FileSystem
IFile* ProxyFile; // постоянный файл

CFileSystem* myFS;
};

#endif