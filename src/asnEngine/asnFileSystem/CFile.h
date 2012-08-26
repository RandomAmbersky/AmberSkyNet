#ifndef _C_FILE_
#define _C_FILE_

#include "IFile.h"
#include "CFileSource.h"
#include <stdio.h>

class CFile:public IFile{
friend class CFileSource;
public:
CFile(CFileSource *source);
~CFile();
void Release();
asn_File_State FileOpen();
void FileClose();
void *LoadInMem();
void FreeMem();
void Seek(unsigned int Position, int From);
void ReadBuf(void *buf, unsigned int size);
private:
FILE *p_File; //указатель на открытый файл
void *buffer; //указатель на открытый буфер
CFileSource *FileSource; //источник файла  - класс CSource
};

#endif
