#ifndef _C_FILESYSTEM_ASN
#define _C_FILESYSTEM_ASN

#include "CFileSystem.h"
//#include "IFileSource.h"
//#include "IEngine.h"

//#include <map>

class CFileSystemASN: public CFileSystem{
public:
CFileSystemASN(IEngine *_Engine);
~CFileSystemASN();
//bool AddPath(const std::string& Path, bool Recursive);
IFile *GetFile(const char* FileName);
// ��������� �������� ������
//bool AddFileSource (const std::string& FileSourceName,IFileSource *FileSource);

//protected:
//IFile *FindFile(const std::string& FileName);
//IEngine *Engine;
//std::string SandBox;
//std::map<std::string,IFileSource *> FileSources; //��������� ������
//std::map<std::string,IFile *> FileList; //��� �������� ������
};

#endif

