#include "CPluginFactory.h"
#include "os_deps.h"

#include "ASN_utils.h"

#include "ILog.h"

//#include "ASN_List.h"

//typedef ASN_List< std::string > ASN_StringList;
//ASN_StringList MyList;

#include "CBaseObject.h"
#include "os_loadmodule.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

typedef void (*GETPLUGIN)( IAsnGenerator** );

CPluginFactory::CPluginFactory(IEngine *_Engine){
Engine=_Engine;
next_uid = 0;
};

CPluginFactory::~CPluginFactory(){
// выгружаем все загруженные библиотеки
std::map<std::string,void*>::iterator it_p=LibraryList.begin();

while(it_p!=LibraryList.end()) {
//os_UnloadObject( (*it_p).second );
fprintf(stdout,"%s: Module %s unloaded\n" ,__FILE__,(*it_p).first.c_str());
it_p++;
};

};

bool CPluginFactory::LoadASNModule(const std::string& ModuleName){
if (LibraryList.find( GetFileName(ModuleName) )!=LibraryList.end()) {
fprintf(stderr,"%s: module %s already loaded...",__FILE__,ModuleName.c_str());
return true;
}

//fprintf(stdout,"%s: module %s loading...",__FILE__,ModuleName.c_str());
fprintf(stdout,"CPluginFactory::module %s loading...",ModuleName.c_str());
/// загружаем динамический модуль
void* hLibrary = os_LoadObject(ModuleName.c_str());
if (hLibrary == NULL) {fprintf(stdout,"[ERROR!]\n"); return true; }
fprintf(stdout,"[OK]\n");

/// ќпредел€ем адрес функции GetPlugin
GETPLUGIN FPlugLoad = (GETPLUGIN)os_LoadFunction( hLibrary, "GetPlugin" );
/// ≈сли определить не удаЄтс€ - выгружаем модуль и возвращаем NULL
if (!FPlugLoad) {
fprintf(stderr,"%s: I can't find GetPlugin function in %s module :(\n",__FILE__,ModuleName.c_str());
os_UnloadObject(hLibrary);
return true;
}


//определ€ем место, где будем хранить указатель на класс-генератор экземлп€ров классов по их типу
IAsnGenerator* myGenerator = 0;
//вызываем функцию FPlugLoad - фактически это GetPlugin
FPlugLoad( &myGenerator );

if (!myGenerator) {
fprintf(stderr,"I can't init asnPlugin :(\n");
os_UnloadObject(hLibrary);
return true;
}

fprintf(stdout,"---- CPluginFactory:: load module %s -----\n", ModuleName.c_str() );

LibraryList[ GetFileName(ModuleName) ]=hLibrary; //сохран€ем модуль в списке

return LoadGenerator( myGenerator );

}

bool CPluginFactory::LoadGenerator(IAsnGenerator*  myGenerator) {
// если в myGenerator не экземпл€р класса-генератора а NULL то выгружаем модуль и выходим
myGenerator->Initialize(Engine); //инициализируем генератор

/// получаем информацию о том, кого мы загрузили :)
IAsnGeneratorInfo plug=myGenerator->GetInfo();

/*
if (LOGGER) {
LOGGER->LogMsgStr("--------- [Generator: "+ plug.name +"] ----------");
LOGGER->LogMsgStr("[type] "+plug.type);
LOGGER->LogMsgStr("[author] "+plug.author);
LOGGER->LogMsgStr("[description] "+plug.description);
LOGGER->LogMsgStr("[plugin_version] "+plug.plugin_version);
LOGGER->LogMsgStr("[required_version] "+plug.required_version);
LOGGER->LogMsgStr("----------------------------------");
};
*/
 int pos=0;
	  std::string PlugTypes=plug.type;
	  while(pos!=-1) {
	     pos=PlugTypes.find_first_of(',',0);
	     GeneratorList[PlugTypes.substr(0,pos)]=myGenerator;
//	     fprintf(stdout,"%s: register type %s\n",__FILE__, PlugTypes.substr(0,pos).c_str() );
         fprintf(stdout,"CPluginFactory::register type %s\n", PlugTypes.substr(0,pos).c_str() );

	     PlugTypes=PlugTypes.substr(pos+1);

	     }


return false; //генератор загружен, тревогу поднимать необходимости нет :)
};

// выгрузить модуль плагина
//bool CPluginFactory::UnloadModule(const std::string& ModuleName){
//std::map<std::string,void*>::iterator _it=LibraryList.find(ModuleName);
//if (_it==LibraryList.end()) return true;
//os_UnloadObject( (*_it).second );
//LibraryList.erase( _it);
//printf("%s: Module %s unloaded" ,__FILE__,ModuleName.c_str());
//return false;
//};

void CPluginFactory::ScanDirectory(std::string DirName, bool Recursive){
    os_finddata_t fdata;
	os_dir_t hFile;

// если путь не заканчиваетс€ слэшем ставим его
	if (DirName[DirName.length()-1]!='/') DirName=DirName+'/';

// начали
	hFile = os_findfirst((DirName).c_str(), &fdata);
	if (!hFile) return;

//	IAsnGenerator* pOutput=NULL;

	do
	{

	if (fdata.name=="..") continue;
	if (fdata.name==".") continue;

	if (fdata.subdir & Recursive) {
//     printf("scan dir: %s\n", fdata.name.c_str());
     ScanDirectory(fdata.dir_name+fdata.name,true);
     }
    else {
//     printf("find file: %s\n", fdata.name.c_str());
     std::string FileExt=GetFileExt(DirName+fdata.name);
     if (FileExt==ASN_MODULE_EXTENSION_NAME) LoadASNModule(DirName+fdata.name);
         }

	} while (os_findnext( hFile, &fdata )!= NULL);
	os_findclose(hFile);

};

/*
void *CPluginFactory::CreateType(const std::string& Type){

    std::string TempName = Type;
    std::map<std::string,IAsnGenerator*>::iterator _it=GeneratorList.find(TempName);
    if (_it==GeneratorList.end()) return NULL; //никто не хочет обрабатывать такое
//    cout << "CreateType "<< Type << endl;
//    cout << (*_it).second << endl;
    void *myClass=(*_it).second->GetClass(TempName.c_str()); //вызываем генератор классов
    if (!myClass) return NULL; // не получилось :(
    if (Type=="Engine") Engine=(IEngine*)myClass; //если тип - Engine - запоминаем его, пригодитс€ :)
    return myClass;

};
*/

/// тупой загон дл€ компил€ции в глючащем CodeBlocks
#define GetObjectA GetObject

IBaseObject* CPluginFactory::CreateObject(const char* Type_){

//    std::string TempName = Type;
    std::map<std::string,IAsnGenerator*>::iterator _it=GeneratorList.find(Type_);
    if (_it==GeneratorList.end()) return NULL; //никто не хочет обрабатывать такое

    IBaseObject* myClass=(IBaseObject*) (*_it).second->GetObject(Type_); //вызываем генератор классов

    if (!myClass) {
          printf( "Can't create %s\n", Type_ );
//        LOGGER->ErrMsgStr(4,"Can't create "+std::string(Type_) );
        return NULL;
    }

// не получилось :(
//    if (Type=="Engine") Engine=(IEngine*)myClass; //если тип - Engine - запоминаем его, пригодитс€ :)

// провер€ем наследование
    const char* classType = myClass->GetType();
//    GetBaseClass();

    if ( strcmp(classType,Type_)!=0 ){
        printf( "Can't create %s -> %s \n", Type_, classType );
        myClass->Release();
        if (LOGGER) LOGGER->ErrMsgStr(3,"Error! Class "+std::string(Type_)+" have type "+classType );
        return NULL;
    }

    CBaseObject* myObj = dynamic_cast<CBaseObject*>( myClass );
    myObj->myID = next_uid;
    next_uid++;
    return myClass;

};
