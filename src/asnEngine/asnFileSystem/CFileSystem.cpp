//// �Ѩ �������!!!!!!

#include "CFileSystem.h"
#include "CFileSource.h"
#include "CFileRemote.h"

#include "ILog.h"
//#include "IPluginFactory.h"

#define ENGINE Engine //� ��� ��� ���� ��������� �� Engine
#include "ASN_utils.h"

#include <iostream>
#include <fstream>

using namespace std;

CFileSystem::CFileSystem(IEngine *_Engine):CBaseObject(_Engine) {
Engine=_Engine;

LOGGER->LogMsg("+CFileSystem"); //�������� �������� ������� ������

SandBox=ENGINE->GetEnvParam("[Paths]DataPath");// ���������� ����� "���������" ������

std::string FileReaders=ENGINE->GetEnvParam("[FileManager]DataReader");

std::string FileReader="";

SetParam("BaseClass","FileSystem");
SetParam("Type","FileSystem");

//"��������" ������� �������� ������
//IFileSource *myFS =new CFileSource(Engine);
//AddFileSource("FSDisk",myFS);

 int pos=0;
	  while(pos!=-1) {
	     pos=FileReaders.find_first_of(',',0);
	     FileReader=FileReaders.substr(0,pos);
	     FileReader=Trim(FileReader);
	     AddFileSource(FileReader.c_str());
	     FileReaders=FileReaders.substr(pos+1);
	     }

//IFileSource *myFS = (IFileSource *)((IPluginFactory*)Engine->GetPtrParam("PluginManager"))->CreateType( "FSDisk" );
//if (myFS) FileSources[ "FSDisk" ]=myFS;  //���� ������:
//else LOGGER->ErrMsg(1,"FSDisk can't init :(");

AddPath("",false); //��������� ����� � ������� - ����� �����-�� ���� ����� � ����� ?
//AddPath("common/",true); //��������� ����� "common" � ������ � ����������

};


CFileSystem::~CFileSystem(){

//printf("~CFileSystem...\n");

//��������� � ������� ��� �������� �����
std::map<std::string,IFile *>::iterator _it=FileList.begin();
    for (int i=FileList.size(); i>0; i--){
//printf("try released...\n");
    (*_it).second->Release();
//printf("released...\n");
    _it++;
    };

//printf("CFileSystem::~CFileSystem()\n");

//������� ��� ��������� ������

    std::map<std::string,IFileSource *>::iterator it=RemoteFileSources.begin();
    for (int i=RemoteFileSources.size(); i>0; i--){
    (*it).second->Release();
    it++;
    };

    it=FileSources.begin();
    for (int i=FileSources.size(); i>0; i--){
    (*it).second->Release();
    it++;
    };

LOGGER->LogMsg("-CFileSystem");
};

void CFileSystem::InitManager(){

printf("CFileSystem init...\n");

};

int CFileSystem::Release(){
    printf("CFileSystem released...\n");
    delete this;
    return 0;
};

void CFileSystem::Update(float tms){


};

bool CFileSystem::AddPath(const char* Path, bool Recursive){

    std::string TruePath=SandBox+Path;

    std::map<std::string,IFileSource *>::iterator it=FileSources.begin();
    for (int i=FileSources.size(); i>0; i--){
    (*it).second->AddPath(TruePath, Recursive);
    it++;
    };

//cout << "CFileSystem add path: " << Path << endl;
//    std::string tPath = Path;

    if ( GetProtocolStr( Path )=="") Paths.push_back( Path ); //������ �� ������
    else RemotePaths.push_back( Path );

cout << "CFileSystem add path: " << Path << endl;

//    it=RemoteFileSources.begin();
//    for (int i=RemoteFileSources.size(); i>0; i--){
//    (*it).second->AddPath(Path, Recursive); // no sandbox ! :)
//    it++;
//    };

};

IFile *CFileSystem::LoadFromCache(const std::string& FileName){

    std::map<std::string,IFile *>::iterator p_CFile=FileList.find(FileName);

    //����� � ���� ���
    if (p_CFile==FileList.end()) return NULL;

    IFile* myFile=(*p_CFile).second;
    asn_File_State myState=myFile->GetState(); //������ ������ �����
    //// ���� ���� ��� ������ �� ����� :) ��� �������� � ������ :) ��� ����� ��� � �� ����� :(
    if ( (myState==ASN_FILE_DISK_OPEN) || (myState==ASN_FILE_DISK_IN_MEM)
    || (myState==ASN_FILE_NOT_FOUND) ) return myFile; //���������� ��������� �� ����
    myFile->FileOpen(); //�������� ��� ������� �����
    return myFile; //���������� ��������� �� ����

};

IFile *CFileSystem::LoadFromSources(const std::string& FileName){

//������ ���� �� ���� ����������:
    std::map<std::string,IFileSource *>::iterator it=FileSources.begin();
    IFile* myFile=NULL;

    for (int i=0;i<FileSources.size(); i++){
    if ( (myFile=(*it).second->LoadFile(FileName)) ) {
        LOGGER->LogMsgStr( ASN_Str("���� ")+ASN_Str(FileName)+" ������ � ���������: "+(*it).first);
        FileList[FileName]=myFile;
        return myFile;
    }
    it++;
    };

// ���� � ��������� ���������� �� ����� - �������� ������ ��������� �����-�������, ������� �����
// ������ �� ��������� �������� ����������


    CFileRemote* myFileR = new CFileRemote( this );
	myFileR->FileName = FileName;
	myFileR->SizeInBytes=0;
	myFileR->CurPos=0;

    std::vector<std::string>::iterator it2=Paths.begin();

    for (int i=Paths.size(); i>0; i--){
	myFileR->AddPath( (*it2).c_str() );
//cout << (*it2) << endl;
	it2++;
    }

    it2=RemotePaths.begin();

    for (int i=RemotePaths.size(); i>0; i--){
	myFileR->AddRemotePath( (*it2).c_str() );
	it2++;
    }

	FileList[FileName]=myFileR;
	return myFileR;

/*
    it=RemoteFileSources.begin();
    for (int i=0;i<RemoteFileSources.size(); i++){
    if ( (myFile=(*it).second->LoadFile(FileName)) ) {
        LOGGER->LogMsgStr( ASN_Str("���� ")+ASN_Str(FileName)+" ������ � ���������: "+(*it).first);
        FileList[FileName]=myFile;
        return myFile;
    }
    it++;
    };
*/

    return NULL;
};

// ������� ���������� �� ������ CFileSource, �� ����� - ������ ���� � ��������� ��������� <��������>://<����>
IFile *CFileSystem::GetFileRemote(const char* FileName){

	IFile* myFile=NULL;

	std::map<std::string,IFileSource *>::iterator it = RemoteFileSources.begin();
	for (int i=0;i<RemoteFileSources.size(); i++){
    		if ( (myFile=(*it).second->LoadFile(FileName)) ) {
        	LOGGER->LogMsgStr( ASN_Str("���� ")+ASN_Str(FileName)+" ������ � ���������: "+(*it).first);
// � ���� �� ���������, �.�. ��� ������� ���������� �� ������ CFileSource,
// ������� ��� ��� �������� � LoadFromSources ��� ���������� � ���
//        	FileList[FileName]=myFile;
        	return myFile;
    		}
    	it++;
    	};

	return NULL; // ������ ���

};

IFile *CFileSystem::GetFile(const char* FileName){
//LOGGER->LogMsg("try open:"+FileName);
//std::string LocalFileName=SandBox+FileName;

// ���� ���� �� ���� � ����
IFile *myFile=LoadFromCache( FileName );

if (myFile) return myFile;

//����� � ���� ���
return LoadFromSources ( FileName );

//std::map<std::string,IFile *>::iterator p_CFile=FileList.find(FileName);
//����� � ���� ���
//if (p_CFile==FileList.end()){

//������ ���� �� ���� ����������:

//    std::map<std::string,IFileSource *>::iterator it=FileSources.begin();

//    for (int i=0;i<FileSources.size(); i++){
//    if ( (myFile=(*it).second->LoadFile(FileName)) ) {
//        LOGGER->LogMsgStr( ASN_Str("���� ")+ASN_Str(FileName)+" ������ � ���������: "+(*it).first);
//        FileList[FileName]=myFile;
//        return myFile;
//    }
//    it++;
//    };

//return NULL; //���� �� ������ ������ :(

//}
//else { // ���� � ���� ����
//    myFile=(*p_CFile).second;
//    asn_File_State myState=myFile->GetState(); //������ ������ �����
//    //// ���� ���� ��� ������ �� ����� :) ��� �������� � ������ :) ��� ����� ��� � �� ����� :(
//    if ( (myState==ASN_FILE_DISK_OPEN) || (myState==ASN_FILE_DISK_IN_MEM)
//    || (myState==ASN_FILE_NOT_FOUND) ) return myFile; //���������� ��������� �� ����
//    myFile->FileOpen(); //�������� ��� ������� �����
//    return myFile; //���������� ��������� �� ����
//};

};

//void CFileSystem::FileClose(IFile *file){
//};

IFile *CFileSystem::FindFile(const std::string& FileName){
std::map<std::string,IFile *>::iterator p_CFile=FileList.find(FileName);
if (p_CFile==FileList.end()) return NULL;
return (*p_CFile).second;
};

bool CFileSystem::AddFileSource (const char* FileSourceName){
if (FileSources.find(FileSourceName)!=FileSources.end() ) return true; //��� ���� �����

IFileSource *myFS = dynamic_cast<IFileSource*>(Engine->CreateObject( FileSourceName ));
if (myFS) {
	   if ( myFS->isRemote() ) RemoteFileSources[ FileSourceName ]=myFS;  //���� ������:
	   else FileSources[ FileSourceName ]=myFS;  //���� ������:
           LOGGER->LogMsgStr("Add "+ASN_Str(FileSourceName)+" data reader..");
           return false;}

LOGGER->ErrMsgStr(1,ASN_Str(FileSourceName)+" can't init :(");
return true;

};

bool CFileSystem::AddCustomFileSource (const char* FileSourceName, void *FileSource){

if (FileSources.find(FileSourceName)!=FileSources.end() ) return true; //��� ���� �����
if (FileSource) {
		if ( ((IFileSource *)FileSource)->isRemote() ) RemoteFileSources[ FileSourceName ]=(IFileSource *)FileSource;
		else FileSources[ FileSourceName ]=(IFileSource *)FileSource;
                LOGGER->LogMsgStr("Add "+ASN_Str(FileSourceName)+" data reader..");
                 return false;}

LOGGER->ErrMsgStr(1,ASN_Str(FileSourceName)+" can't init :(");
return true;

};

/*
bool CFileSystem::AddFileSource (const std::string& FileSourceName,void *FileSource){
if (FileSources.find(FileSourceName)!=FileSources.end() ) return false; //��� ���� �����
if (FileSource) {FileSources[ FileSourceName ]=(IFileSource *)FileSource;
                LOGGER->LogMsgStr("Add "+FileSourceName+" data reader..");
                 return false;}

IFileSource *myFS = (IFileSource *)((IPluginFactory*)Engine->GetPtrParam("PluginManager"))->CreateType( FileSourceName );
if (myFS) {FileSources[ FileSourceName ]=myFS;  //���� ������:
           LOGGER->LogMsgStr("Add "+FileSourceName+" data reader..");
           return false;}

LOGGER->ErrMsgStr(1,FileSourceName+" can't init :(");
return true;

};
*/
