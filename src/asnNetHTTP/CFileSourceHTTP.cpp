#include "CFileSourceHTTP.h"
#include "CFileHTTP.h"
#include "ILog.h"
#include "os_deps.h"
#include "INetSupervisor.h"

#include "ASN_utils.h"

#include <iostream>

using namespace std;

#define ENGINE Engine //у нас уже есть указатель на Engine

CFileSourceHTTP::CFileSourceHTTP(IEngine *_Engine):CBaseObject(_Engine){
//    Engine=_Engine;
    SetParam("BaseClass","FileSource");
    SetParam("Type","FSHTTP");
    LOGGER->LogMsg("+CFileSourceHTTP"); //менеджер файловой системы создан
};

CFileSourceHTTP::~CFileSourceHTTP(){
    LOGGER->LogMsg("-CFileSourceHTTP");
};

int CFileSourceHTTP::Release(){
    delete this;
    return 0;
};

IFile *CFileSourceHTTP::LoadFile(const std::string& FileName){
int i;

//    cout << "CFileSourceHTTP: try LoadFile " << endl;;

    //FileName.c_str() << endl;

//    fprintf(stdout,"HTTP try find %s\n", FileName.c_str());

if ( GetProtocolStr( FileName )!="http") return NULL;

    if (!NETTER) {
        cout << "can't find NetManager :(" << endl;
        return NULL;
    }

 // если явно указан протокол - создаём файл
    fprintf(stdout,"HTTP try find %s\n", FileName.c_str());
    CFileHTTP* myFile = new CFileHTTP(this);
    myFile->FileName=FileName;
    myFile->Engine=Engine;
    myFile->SizeInBytes = 0;
    myFile->CurPos = 0;
    return myFile;

};

void CFileSourceHTTP::AddPath(const std::string& PathName, bool Recursive){

//    cout << "CFileSourceHTTP: try add " << PathName.c_str() << endl;

    // если это не http, то ничего не добавляем.. это не наши проблемы
    if ( GetProtocolStr( PathName )!="http") return;

    Paths.push_back(PathName);
    fprintf(stdout,"HTTP add path %s\n",PathName.c_str());

};
