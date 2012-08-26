#ifndef _I_FILESYSTEM_
#define _I_FILESYSTEM_

#include "IFile.h"
#include "IManager.h"

class IFileSystem: public IManager {
public:
//virtual ~IFileSystem(){};
// �������� �������� ������
virtual bool AddPath(const char* Path, bool Recursive)=0;
// ������� ����
virtual IFile *GetFile(const char* FileName)=0;
// ��������� �������� ������
virtual bool AddFileSource (const char* FileSourceName)=0;
virtual bool AddCustomFileSource (const char* FileSourceName,void *FileSource)=0;
};

#define FILER ((IFileSystem*)ENGINE->GetPtrParam("FileManager"))

#endif
