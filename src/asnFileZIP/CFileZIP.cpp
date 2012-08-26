#include "CFileZIP.h"
#include <iostream>
#include "lib/minizip/unzip.h"
#include "ASN_utils.h"

using namespace std;

CFileZIP::CFileZIP(CFileSourceZIP *source){
FileSource=source;
state=ASN_FILE_NONE;
};

CFileZIP::~CFileZIP(){
    FileClose();
};

void CFileZIP::Release(){
delete this;
};

asn_File_State CFileZIP::GetState(){
return state;
};

asn_File_State CFileZIP::FileOpen(){
std::cout << "CFileZIP::FileOpen()" << std::endl;

        if (state==ASN_FILE_DISK_IN_MEM)
                  return ASN_FILE_DISK_IN_MEM;

std::cout << "CFileZIP::FileOpen()" << std::endl;

         unzFile uf=unzOpen( ZipFileName.c_str() );
         if (uf==NULL) {
        		printf(" file %s can't open as zip :(", ZipFileName.c_str() );
        		return state;
        		}
         if(unzLocateFile(uf, GetFileName(FileName,true).c_str() ,1) != UNZ_OK) {
	       printf("file %s is not found in archive %s\n", FileName.c_str(), ZipFileName.c_str());
		   return state;}
      	if(unzOpenCurrentFile(uf) != UNZ_OK) {
		   printf("error open file %s in archive %s\n",FileName.c_str(), ZipFileName.c_str());
		   return state;}

           unz_file_info info;
           unzGetCurrentFileInfo( uf ,&info,NULL,0,NULL,0,NULL,0);
           	SizeInBytes = info.uncompressed_size;
         	buffer = malloc(SizeInBytes);
         	printf("malloc %d size", SizeInBytes);
   		unzReadCurrentFile(uf,buffer,SizeInBytes);
   		unzCloseCurrentFile(uf);
   		unzClose(uf);
   		state=ASN_FILE_DISK_IN_MEM;

    return state;
};

void CFileZIP::FileClose(){
FreeMem();
state=ASN_FILE_NONE;
};


void *CFileZIP::LoadInMem(){
std::cout << "CFileZIP::LoadInMem()" << state << std::endl;
if (state==ASN_FILE_DISK_IN_MEM) return buffer;
// если файл не в памяти - пробуем открыть его
std::cout << "CFileZIP::LoadInMem()" << std::endl;
FileOpen();
if (state!=ASN_FILE_DISK_IN_MEM) return NULL;
else return buffer;
};

// освободить память
void CFileZIP::FreeMem(){
if (buffer) {
free(buffer);
buffer=NULL;
}
state=ASN_FILE_NONE;
};

void CFileZIP::ReadBuf(void *buf, unsigned int size){
		if (size)
		{

       if (state == ASN_FILE_DISK_IN_MEM)
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
void CFileZIP::Seek(unsigned int Position, int From){
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
unsigned int CFileZIP::GetSizeInBytes(){
return SizeInBytes;
};
