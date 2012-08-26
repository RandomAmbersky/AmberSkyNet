#ifndef _C_FILE_
#define _C_FILE_

#include "IFile.h"
#include "CFileSourceZIP.h"

#include <stdio.h>

class CFileZIP:public IFile{
friend class CFileSourceZIP;
public:
CFileZIP(CFileSourceZIP *source);
~CFileZIP();
void Release();
asn_File_State GetState();
asn_File_State FileOpen();
void FileClose();
void *LoadInMem();
void FreeMem();
void Seek(unsigned int Position, int From);
unsigned int GetSizeInBytes();
void ReadBuf(void *buf, unsigned int size);
private:
std::string ZipFileName; //��� ZIP-������
//asn_File_State state; //��������� �����
FILE *p_File; //��������� �� �������� ����
void *buffer; //��������� �� �������� �����
//unsigned int SizeInBytes; //��������� �� ������ �����
//unsigned int CurPos; //��������� �� ������� ������� �����
CFileSourceZIP *FileSource; //�������� �����  - ����� CSource
};

#endif
