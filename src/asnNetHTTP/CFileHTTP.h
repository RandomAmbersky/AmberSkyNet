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

//// а тут для http-handler'а открытая инфа 

std::string Username;
std::string Server;
std::string Path;

void AcceptBufer(const char* in_buffer, int buf_size);
void CloseHTTPSocket(); // функция, которую вызовет handler когда сокет закроется

private:

FILE *p_File; //указатель на открытый файл на диске
void *buffer; //указатель на открытый буфер в памяти
CFileSourceHTTP *FileSource; //источник файла  - класс CSource

std::string LocalFileName; // имя файла в кэше локальной файловой системы

/// for HTTP

    INetProtocolHandler *myNet;
    IEngine* Engine;

};

#endif
