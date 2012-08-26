#include "CFileSystemASN.h"

#include <stdio.h>

CFileSystemASN::CFileSystemASN(IEngine *_Engine):CFileSystem(_Engine){

SetParam("BaseClass","FileSystem");
SetParam("Type","FileSystemASN");

AddPath("common/",true); //��������� ����� "common" � ������ � ����������

};

CFileSystemASN::~CFileSystemASN(){

	printf("~CFileSystemASN...\n");

};


IFile *CFileSystemASN::GetFile(const char* _FileName){

std::string FileName=_FileName;

//// ���� ������� � ����
IFile *myFile=LoadFromCache( FileName );
if (myFile) return myFile;

// �� ��������
/////// ���� � ����� ���� �����������
/////// �������� ������ ��� �� common :-)
/////// �� ���� ��� ������ ���� ��������� [local|net]/[World Name]/
//int pos=FileName.find_first_of('/');
//if (pos==-1) return CFileSystem::GetFile(FileName.c_str());
//pos=FileName.find_first_of('/',pos+1);
//if (pos==-1) return CFileSystem::GetFile(FileName.c_str());
//std::string TempName="common/"+FileName.substr(pos+1,FileName.length());
//printf("============{%s}=========\n",TempName.c_str());

//myFile=CFileSystem::GetFile(TempName.c_str());

//myFile=LoadFromSources( TempName );
//if (myFile) return myFile;
return LoadFromSources( FileName );

//if (myFile) return myFile; //����� ���� � common'�
// ����� � common'� ���, ����� ���-������ ���� ���?
//return CFileSystem::GetFile(FileName.c_str());

};
