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

// ������ ����� ����� CFIleSystem

void AddRemotePath(const char* RemPath);
void AddPath(const char* Path);

private:

asn_File_State LoadNext(); // ��������� ��������� �� ������ ����

std::string CurPath; // ������� ���� ������� �����������

std::vector<std::string> Paths; // �������� � AddPath ����
std::vector<std::string> RemotePaths; // �������� � AddPath ���� � ��������� ��������� <��������>://<����>

std::vector<std::string>::iterator it_p; // �������� ��������� �����
std::vector<std::string>::iterator it_pr; // �������� ��������� �����

IFile* tempFile; // ��������� ���� �� FileSystem
IFile* ProxyFile; // ���������� ����

CFileSystem* myFS;
};

#endif