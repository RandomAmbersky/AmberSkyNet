#include "CFileHTTP.h"

#include "INetSupervisor.h"
#include "ASN_utils.h"
#include <iostream>
#include <errno.h>

//#ifndef _WIN32
//# include <sys/types.h>
//# include <sys/stat.h>
//#else
//#include <windows.h>
//#include <direct.h>
//#include <io.h>
//#endif

//#include <fcntl.h>
//#include <stdio.h>

#include "os_deps.h"
#include "ILog.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

//#include "CFile.h"

//char test_buffer[]="test buffer";

using namespace std;

void hanlde_HTTP(INetProtocolHandler *handler, void *info) {

cout << "hanlde_HTTP" << endl;

if (info) {
    if ( strcmp( (const char*)info, "release" )==0 ) {

	handler->SetPtrParam("Handler", NULL); // и не приходи больше сюда :)
	CFileHTTP* myFile = (CFileHTTP*) handler->GetPtrParam("HandlerClass");
	if (myFile) myFile->CloseHTTPSocket();
    }
return;
}
    int buf_size;

    if ( sscanf(handler->GetParam("BufferSize"),"%d",&buf_size) !=1 ) return;

    const char* buffer = (const char*) handler->GetPtrParam("Buffer");
//    printf("test buffer: %s\n", (const char*) handler->GetPtrParam("TestBuffer") );
    CFileHTTP* myFile = (CFileHTTP*) handler->GetPtrParam("HandlerClass");
    if (myFile) {
/*
    printf("===========================================\n");
    printf( "Incoming buffer size: %d\n", buf_size );
    printf( "LoadSize: %s\n", handler->GetParam("LoadSize") );
    printf("===========================================\n");
    printf("[Header]Status-Code:%s\n", handler->GetParam("[Header]Status-Code") );
    printf("[Header]Content-Length:%s\n", handler->GetParam("[Header]Content-Length") );
*/
    if ( strcmp(handler->GetParam("[Header]Status-Code"),"404")==0 ) {
    	handler->SetPtrParam("Handler", NULL); // и не приходи больше сюда :)
	myFile->CloseHTTPSocket();
	return;
	}

//    printf( "Server: %s\n", myFile->Server.c_str() );
//    printf( "Path: %s\n", myFile->Path.c_str() );

// файл не найден

    if (!buffer) { myFile->CloseHTTPSocket(); }
    else { myFile->AcceptBufer( buffer, buf_size); }

    };

//  FILE *f=fopen("textfile.txt","ab+");
//  fwrite(buffer,buf_size,1,f);
//  fclose(f);

};

void CFileHTTP::AcceptBufer(const char* in_buffer, int buf_size){

  std::string SandBox=ENGINE->GetEnvParam("[Paths]DataPath");// запоминаем адрес "песочницы" данных

  std::string DiskPath = SandBox+"net/"+Server+"/";

  printf("try create dir %s\n", DiskPath.c_str() );

//os_make_dir( DiskPath.c_str() );

//try {

//  int res =
    os_make_dir( DiskPath.c_str() );
  // mkdir( DiskPath.c_str() );
   int res = errno;

   if (errno==2)
    LOGGER->ErrMsgStr(2, std::string(" can't create dir ")+DiskPath+" !");

//} catch ( exception e ) {
//   int errn=0;
//    LOGGER->ErrMsgStr(2, IntToStr( errn ) +std::string(" - can't create dir ")+DiskPath+" !");
//}



//d::string resu = IntToStr( res );

    // ( res ) {
//      LOGGER->ErrMsgStr(2, resu +std::string(" - can't create dir ")+DiskPath+" !");
//      }

    std::string Current = Path.substr(1);

    int i=0;
    while (i!=-1) {

//    printf("path:[%s]\n",Current.c_str() );
    i=Current.find("/");

    if (i!=-1) {
//    printf( );
	DiskPath = DiskPath+Current.substr(0,i)+"/";
	os_make_dir( DiskPath.c_str() );
	Current=Current.substr(i+1, Current.length()-i);
        printf("path:[%s]\n",DiskPath.c_str() );
        }

    }

  DiskPath=DiskPath+Current+".tmp";

  printf("%s try save to %s\n", FileName.c_str(), DiskPath.c_str() );
//  printf("try save to %s\n", LocalFileName.c_str() );

  FILE *f=fopen(DiskPath.c_str(),"ab+");
  fwrite(in_buffer,buf_size,1,f);
  fclose(f);

  SizeInBytes = SizeInBytes + buf_size;
  printf("Download: %d / %s\n", SizeInBytes, myNet->GetParam("[Header]Content-Length") );

//    state = ASN_FILE_NOT_FOUND;
};

void CFileHTTP::CloseHTTPSocket(){

if ( myNet ) {

// TODO
//int LoadSize = StrToInt( myNet->GetParam("[Header]Content-Length") );
//if ( SizeInBytes == LoadSize ) state = ASN_FILE_DISK_OPEN; // скачали все что хотели, файл открыт
//else state = ASN_FILE_NOT_FOUND; // не докачали

if (SizeInBytes > 0) state = ASN_FILE_LOADING_END;
else state = ASN_FILE_NOT_FOUND;
// соединение закрыто, будем пока считать что все скачали

std::string LocalFileName_tmp = LocalFileName+".tmp";

rename( LocalFileName_tmp.c_str() , LocalFileName.c_str() );

// если функци€ вызвана не из hanlde_HTTP - то закрываем соединение сами
// если из hanlde_HTTP , то там перед этим обнул€етс€ укзаатель на Handler и ничего делать не надо
if ( myNet->GetPtrParam("Handler") ) NETTER->CloseListener(myNet->SupervisorId);

myNet = NULL;
return;
}

// присваиваем myNet null т.к. он удалитс€ при возврате из hanlde_HTTP, где указатель на пользовательский обработчик
// будет выставлен в NULL, это приведет к удалению экземпл€ра созданного класса  HTTP_Client и возврате 100 в
// CNetSupervisor
// myNet = NULL;
state = ASN_FILE_NOT_FOUND;
cout << "CloseHTTPSocket()" << endl;

}

CFileHTTP::CFileHTTP(CFileSourceHTTP *source){
cout << "CFileHTTP::CFileHTTP " << endl;
    FileSource = source;
    state=ASN_FILE_NONE;
    myNet=NULL;
    p_File=NULL;
    buffer=NULL;
    LocalFileName="";
    printf("try create http file...\n");
};

CFileHTTP::~CFileHTTP(){
    FileClose();
cout << "CFileHTTP::~CFileHTTP() " << state << endl;
}

void CFileHTTP::Release(){
cout << "CFileHTTP::Release() " << state << endl;
    delete this;
}

asn_File_State CFileHTTP::FileOpen(){

//cout << "CFileHTTP::FileOpen() " << state << endl;

    if ( (state==ASN_FILE_DISK_OPEN) || (state==ASN_FILE_DISK_IN_MEM)
        || (state==ASN_FILE_NOT_FOUND) ) return state; // файл уже открывалс€

    if ( state == ASN_FILE_LOADING_END ) { // файл загрузили в кэш, осталось только открыть же

    cout << "CFileHTTP try load from local as " << LocalFileName << endl;

    p_File=fopen(LocalFileName.c_str(),"rb");

    if (!p_File) { state==ASN_FILE_NOT_FOUND; return state;} // по идее не должно быть, но ведь все возможно

    state=ASN_FILE_DISK_OPEN;

    fseek(p_File,0, SEEK_END);
    SizeInBytes = ftell(p_File);
    fseek(p_File,0, SEEK_SET);

//	buffer = new char[SizeInBytes+1]; //malloc(SizeInBytes); // new BYTE[SizeInBytes];
//	cout << "malloc" << SizeInBytes << endl;
//	fread(buffer, SizeInBytes, 1, p_File);
//	std::string bu_f = (char*)buffer;

    cout << "CFileHTTP eyaaah!! " << FileName << " open as " << LocalFileName << state << endl;
//	buffer=NULL;
//    LoadInMem();
    return state;

    }

//cout << "CFileHTTP::FileOpen2() " << state << endl;

/*
// если LocalFileName заполнено, значит скачали через myNet
if ( LocalFileName!="") {

// пробуем открыть по имени:

// если не можем - возвращаем прежний статус
if (!p_File) return state;

// иначе - файл открыт:
    state=ASN_FILE_DISK_OPEN;
    fseek(p_File,0, SEEK_END);
    SizeInBytes = ftell(p_File);
    fseek(p_File,0, SEEK_SET);
    return state;

}
*/

/// охо-хо.. файла нет, значит попробуем его сделать..

    if (!myNet) {

cout << "try open listener " << state << endl;

    std::string Protocol;
//    std::string Username;
    std::string Password;
//    std::string Server;
    std::string Port;
//    std::string Path;

    printf("try parse %s\n", FileName.c_str() );

    bool res = ParseNetAdressStr(FileName, Protocol, Username, Password, Server, Port, Path);

    if ( Port=="") Port="80";
    if (Path[0]!='/') Path = "/"+Path;

    printf("%s %s %s %s %s %s \n", Protocol.c_str(), Username.c_str(), Password.c_str(), Server.c_str(), Port.c_str(), Path.c_str() );

    if (!NETTER) {
        cout << "can't find NetManager :(" << endl;
        state = ASN_FILE_NOT_FOUND;
        return state;
    }

    myNet = NETTER->OpenClientListener( FileName.c_str(),"HTTP", Server.c_str(), StrToInt(Port), "" );

  if (!myNet) {
    state = ASN_FILE_NOT_FOUND;
//   LogError(3,"can't connect to HTTP :(");
    cout << "can't connect to HTTP :(" << endl;
   return state;
  };

//     myNet->SetParam("HandlerParam1", "HandlerValue1");
//     myNet->SetPtrParam("TestBuffer", test_buffer);

     myNet->SetPtrParam("HandlerClass", this);

     myNet->SetPtrParam("Handler",(void*) hanlde_HTTP);

     myNet->Send("GET",Path.c_str(),"");

     state = ASN_FILE_LOADING; // начинаем загружать файл наш..

     std::string SandBox=ENGINE->GetEnvParam("[Paths]DataPath");// запоминаем адрес "песочницы" данных
     LocalFileName = SandBox+"net/"+Server+Path;

     printf("ok...try get\n");

    }

    return state;

};

void CFileHTTP::FileClose(){

    cout << "CFileHTTP::FileClose" << FileName <<endl;
    FreeMem();
    if (p_File) {
    fclose(p_File);
    p_File=NULL; }
    state=ASN_FILE_NONE;
    cout << "CFileHTTP::FileClosed" << FileName <<endl;
};

void *CFileHTTP::LoadInMem(){
//cout << "CFileHTTP::LoadInMem()" << ASN_FILE_NOT_FOUND <<state << endl;
    if (state==ASN_FILE_NOT_FOUND) return NULL;
//cout << "CFileHTTP::LoadInMem()" << ASN_FILE_DISK_IN_MEM << state << endl;
    if (state==ASN_FILE_DISK_IN_MEM) return buffer;
//cout << "CFileHTTP::LoadInMem()" << ASN_FILE_DISK_OPEN << state << endl;
    if (state!=ASN_FILE_DISK_OPEN) { FileOpen(); return NULL; }

//cout << "CFileHTTP::LoadInMem()" << state << endl;

    if (p_File){
	if (buffer) free(buffer);
	cout << "malloc" << SizeInBytes << endl;
	buffer = new char[SizeInBytes+1]; //malloc(SizeInBytes); // new BYTE[SizeInBytes];
//	cout << "malloc" << SizeInBytes << endl;
	fread(buffer, SizeInBytes, 1, p_File);
//	free(buffer);
//	buffer=NULL;
	fclose(p_File);
	p_File = NULL;
        CurPos=0;
	state=ASN_FILE_DISK_IN_MEM;
	cout << "CFileHTTP:: ASN_FILE_DISK_IN_MEM" << state << endl;
	return buffer;
    };

return buffer;

//    return NULL;
};

void CFileHTTP::FreeMem(){
cout << "CFileHTTP::FreeMem" << state << FileName << endl;
    if (buffer) {
        free(buffer);
	buffer=NULL;
	}

    state=ASN_FILE_NONE;
//cout << "CFileHTTP::FreeMem2 " << state << endl;
};

void CFileHTTP::Seek(unsigned int Position, int From){

};

void CFileHTTP::ReadBuf(void *buf, unsigned int size){

};
