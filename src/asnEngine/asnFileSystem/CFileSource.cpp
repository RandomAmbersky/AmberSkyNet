#include "CFileSource.h"
#include "ILog.h"

#include "CFile.h"

#include "ASN_utils.h"

typedef void* os_dir_t;

struct os_finddata_t{
std::string dir_name;
std::string name;
bool subdir;
};

extern os_dir_t os_findnext(os_dir_t dir, os_finddata_t *data);
extern os_dir_t os_findfirst(const char *dirname,os_finddata_t *data);
extern void os_findclose(os_dir_t dir);

#define ENGINE Engine //у нас уже есть указатель на Engine

CFileSource::CFileSource(IEngine *_Engine):CBaseObject(_Engine){
//Engine=_Engine;

SetParam("BaseClass","FileSource");
SetParam("Type","FSDisk");

LOGGER->LogMsg("+CFileSource"); //менеджер файловой системы создан

};

CFileSource::~CFileSource(){
LOGGER->LogMsg("-CFileSource");
};

int CFileSource::Release(){
    //printf("CFileSource::Release\n");
    delete this;
};

IFile *CFileSource::LoadFile(const std::string& FileName){

if ( GetProtocolStr( FileName )!="") return NULL; // not for me

FILE *my=NULL;
int i;
    std::vector<std::string>::iterator it=Paths.begin();
    for (i=Paths.size(); i>0; i--){
//printf("try open %s\n",((*it)+FileName).c_str());

my=fopen(((*it)+FileName).c_str(),"rb");

if (my) {
    CFile *myFile=new CFile(this);
    myFile->FileName=(*it)+FileName;
    myFile->FileOpen(); //открываем его
    return myFile; //пока :)
};
    it++;
    };

return NULL; //такого файла в этом источнике нет
};

void CFileSource::AddPath(const std::string& PathName, bool Recursive){
Paths.push_back(PathName);

if (Recursive) {

    os_finddata_t fdata;
	os_dir_t hFile = os_findfirst((PathName).c_str(), &fdata);
	if (!hFile) return;

do {

	if (fdata.name=="..") continue;
	if (fdata.name==".") continue;

	if (fdata.subdir & Recursive) {
//     printf("scan dir: %s\n", fdata.name.c_str());
     std::string NewPath=fdata.dir_name+fdata.name+"/";
     AddPath(NewPath,true);
     }

} while (os_findnext( hFile, &fdata )!= NULL);
	os_findclose(hFile);
}

}

