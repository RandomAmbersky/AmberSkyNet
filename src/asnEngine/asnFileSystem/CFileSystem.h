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

// ��������� �������� ������
bool AddFileSource (const char* FileSourceName);
bool AddCustomFileSource (const char* FileSourceName,void *FileSource);
//bool AddFileSource (const char* FileSourceName,void *FileSource);

// ������ ��� CRemoteFile, ������ �������� ������� � ��������� ���������� �� �������� �� � ����, �����
IFile* GetFileRemote(const char* FileName);

protected:
IFile *FindFile(const std::string& FileName);
IFile *LoadFromCache(const std::string& FileName);
IFile *LoadFromSources(const std::string& FileName);

//IEngine *Engine;
std::string SandBox;

std::vector<std::string> Paths; // �������� � AddPath ����
std::vector<std::string> RemotePaths; // �������� � AddPath ���� � ��������� ��������� <��������>://<����>

std::map<std::string,IFileSource *> FileSources; //��������� ������
std::map<std::string,IFileSource *> RemoteFileSources; //��������� ������
std::map<std::string,IFile *> FileList; //��� �������� ������
};

#endif
