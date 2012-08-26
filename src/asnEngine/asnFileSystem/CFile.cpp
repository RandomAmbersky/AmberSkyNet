#include "CFile.h"
#include <stdio.h>
#include <stdlib.h>

CFile::CFile(CFileSource *source){
FileSource=source;
state=ASN_FILE_NONE;
p_File=NULL;
buffer=NULL;
};

CFile::~CFile(){
//printf("~CFile()...\n");
    FileClose();
//printf("~CFile closed()...\n");
};

void CFile::Release(){
//printf("CFile Release...\n");
delete this;
};

//asn_File_State CFile::GetState(){
//return state;
//};

asn_File_State CFile::FileOpen(){
//если файл уже открыт или нет - возвращаем его состояние
if ( (state==ASN_FILE_DISK_OPEN) || (state==ASN_FILE_DISK_IN_MEM)
|| (state==ASN_FILE_NOT_FOUND) ) return state;
//иначе - пробуем открыть по имени:
p_File=fopen(FileName.c_str(),"rb");
// если не можем - файл не найден
if (!p_File) { state=ASN_FILE_NOT_FOUND; return state;}
// иначе - файл открыт:
    state=ASN_FILE_DISK_OPEN;
    fseek(p_File,0, SEEK_END);
    SizeInBytes = ftell(p_File);
    fseek(p_File,0, SEEK_SET);
    return state;
};

void CFile::FileClose(){
//printf("FileClose() \n");
FreeMem();
if (p_File) {
fclose(p_File);
p_File=NULL; }
state=ASN_FILE_NONE;
};


void *CFile::LoadInMem(){
if (state==ASN_FILE_DISK_IN_MEM) return buffer;
if (state!=ASN_FILE_DISK_OPEN) return NULL;
if (p_File){
	buffer = new char[SizeInBytes]; //malloc(SizeInBytes);// new BYTE[SizeInBytes];
	fread(buffer, SizeInBytes, 1, p_File);
	fclose(p_File);
	p_File = NULL;
CurPos=0;
state=ASN_FILE_DISK_IN_MEM;
};
return buffer;
};

// освободить память
void CFile::FreeMem(){
//printf("FreeMem() \n");
if (buffer) {
free(buffer);
buffer=NULL;
}
state=ASN_FILE_NONE;
};

void CFile::ReadBuf(void *buf, unsigned int size){
		if (size)
		{
			if (state == ASN_FILE_DISK_OPEN) {
//			   printf("READ FILE FROM DISC........");
				fread(buf, size,1, p_File); }
			else if (state == ASN_FILE_DISK_IN_MEM)
			{
			      if(!buffer) return;
//			    printf("READ FILE FROM MEMORY........");

				if (CurPos+size<=SizeInBytes)
				{
					memcpy(buf, (char *)buffer + CurPos, size);
					CurPos += size;
				}
				else {
					memcpy(buf, (char *)buffer + CurPos, SizeInBytes - CurPos);
					CurPos = SizeInBytes;
				}
			}
			else
			{
			   printf("Can't READ BUFFER!\n");
			}
		}

};

// передвинуть на позицию
void CFile::Seek(unsigned int Position, int From){
		if (state == ASN_FILE_DISK_OPEN)
			fseek(p_File, Position, From);
		else if (state == ASN_FILE_DISK_IN_MEM)
		{
			switch(From) {
			case SEEK_SET:
				CurPos = Position;
				break;
			case SEEK_CUR:
				CurPos += Position;
				break;
			case SEEK_END:
				CurPos = SizeInBytes - Position;
				break;
			}
		}
		else {
			   printf("Can't READ BUFFER!\n");
		}
};

// получить размер файла в байтах;
//unsigned int CFile::GetSizeInBytes(){
//return SizeInBytes;
//};
