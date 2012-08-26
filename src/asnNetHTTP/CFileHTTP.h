#ifndef _C_FILE_HTTP
#define _C_FILE_HTTP

#include "IFile.h"
#include "CFileSourceHTTP.h"
#include <stdio.h>

#include "INetProtocolHandler.h"

class CFileHTTP: public IFile{
friend class CFileSourceHTTP;
public:
CFileHTTP(CFileSourceHTTP *source);
~CFileHTTP();
void Release();
asn_File_State FileOpen();
void FileClose();
void *LoadInMem();
void FreeMem();
void Seek(unsigned int Position, int From);
void ReadBuf(void *buf, unsigned int size);

//// � ��� ��� http-handler'� �������� ���� 

std::string Username;
std::string Server;
std::string Path;

void AcceptBufer(const char* in_buffer, int buf_size);
void CloseHTTPSocket(); // �������, ������� ������� handler ����� ����� ���������

private:

FILE *p_File; //��������� �� �������� ���� �� �����
void *buffer; //��������� �� �������� ����� � ������
CFileSourceHTTP *FileSource; //�������� �����  - ����� CSource

std::string LocalFileName; // ��� ����� � ���� ��������� �������� �������

/// for HTTP

    INetProtocolHandler *myNet;
    IEngine* Engine;

};

#endif
