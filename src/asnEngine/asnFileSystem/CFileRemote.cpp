#include "CFileRemote.h"

#include <iostream>
#include <fstream>

using namespace std;

CFileRemote::CFileRemote(CFileSystem* FileSystem){

     myFS = FileSystem;
     ProxyFile = NULL;
     tempFile  = NULL;
     state=ASN_FILE_WAIT; // файл ожидает загрузки

};

CFileRemote::~CFileRemote(){

cout << "CFileRemote::~CFileRemote() " << endl;

    FileClose();
    if (tempFile) tempFile->Release();
    if (ProxyFile) ProxyFile->Release();

}

void CFileRemote::Release(){

cout << "CFileRemote::FileClose() " << endl;

	delete this;

}

asn_File_State CFileRemote::LoadNext(){

	if ( it_pr==RemotePaths.end() ) { state=ASN_FILE_NOT_FOUND; return ASN_FILE_NOT_FOUND; }

	CurPath = (*it_pr)+ (*it_p)+FileName;

	it_p++;
	if ( it_p==Paths.end() ) { it_p==Paths.begin(); it_pr++; }

	cout << "RemoteFile try open: " << CurPath.c_str() << endl;

	tempFile = myFS->GetFileRemote( CurPath.c_str() );

	if (!tempFile) return LoadNext();

	return tempFile->FileOpen();

}

asn_File_State CFileRemote::FileOpen(){

//cout << "CFileRemote::FileOpen() " << state << endl;

	if (state==ASN_FILE_NOT_FOUND) return ASN_FILE_NOT_FOUND; // успокойтесь, файла нет

	if (ProxyFile) { state = ProxyFile->FileOpen();
//cout << "ProxyFile->FileOpen()" << state << endl;
return state; }

	asn_File_State ret_s;

// делаем новый TempFile
	if (!tempFile ) {
		ret_s = LoadNext();
		if (ret_s == ASN_FILE_NOT_FOUND) return ASN_FILE_NOT_FOUND;
	}

	if (tempFile) {
		ret_s = tempFile->FileOpen();
		if (ret_s == ASN_FILE_NOT_FOUND) { tempFile->Release(); tempFile = NULL; return FileOpen(); }
		if ( ret_s == ASN_FILE_DISK_OPEN ) { // о да! мы сделали это!
		ProxyFile = tempFile;
		tempFile = NULL;
		state=ret_s;
		return ret_s;
		}

	}

	return state;

}

void CFileRemote::FileClose(){
cout << "CFileRemote::FileClose() " << endl;

	if (ProxyFile) return ProxyFile->FileClose();

}

asn_File_State CFileRemote::GetState(){
	if (ProxyFile) return ProxyFile->GetState();
return state;
}

unsigned int CFileRemote::GetSizeInBytes(){
	if (ProxyFile) return ProxyFile->GetSizeInBytes();
	return 0;
};


void *CFileRemote::LoadInMem(){
	FileOpen(); // вдруг мы еще не открыли его как следует, а уже пытаемся загрузить в память :)
	if (ProxyFile) { //ProxyFile->LoadInMem();
//	cout<< "LoadInMemProxy File" << state << ProxyFile->GetState() << endl;
	void* buf = ProxyFile->LoadInMem();
	state = ProxyFile->GetState();
	return buf;
	}
	return NULL; // слушай, какой LoadInMem, тут файла самого пока нет

}

void CFileRemote::FreeMem(){

	if (ProxyFile) ProxyFile->FreeMem();

}

void CFileRemote::Seek(unsigned int Position, int From){

	if (ProxyFile) ProxyFile->Seek(Position, From);

}

void CFileRemote::ReadBuf(void *buf, unsigned int size){

	if (ProxyFile) ProxyFile->ReadBuf(buf, size);

}

void CFileRemote::AddRemotePath(const char* RemPath){

    	RemotePaths.push_back( RemPath );
	it_pr = RemotePaths.begin();
	cout << "CFileRemote add rem path: " << RemPath << endl;

}

void CFileRemote::AddPath(const char* Path){

    Paths.push_back( Path );
    it_p = Paths.begin();

	cout << "CFileRemote add path: " << Path << endl;

}

