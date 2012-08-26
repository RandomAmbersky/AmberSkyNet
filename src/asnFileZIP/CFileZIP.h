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
std::string ZipFileName; //имя ZIP-архива
//asn_File_State state; //состояние файла
FILE *p_File; //указатель на открытый файл
void *buffer; //указатель на открытый буфер
//unsigned int SizeInBytes; //указатель на размер файла
//unsigned int CurPos; //указатель на текущую позицию файла
CFileSourceZIP *FileSource; //источник файла  - класс CSource
};

#endif
