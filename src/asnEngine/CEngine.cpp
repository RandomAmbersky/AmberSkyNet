//[1576] Задумав дело, не говори о нем: не удастся - засмеют. Питтак

//#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

#include "CEngine.h"
#include "CPluginFactory.h"
#include "CResourceSystem.h"

#include "ASN_utils.h"

#include "asnFileSystem/asnFileSystem.h"
#include "asnLog/asnLog.h"
#include "asnDataParser/asnDataParser.h"
#include "IDevice.h"
#include "IEventManager.h"
#include "INetSupervisor.h"
#include "ISound.h"
#include "CSound_empty.h"

//#include "IPhysicManager.h"

#include "ASN_utils.h"
#include <iostream>
#include <fstream>

#include "os_deps.h"

#define ENGINE this

using namespace std;

void ParseIniFile(const std::string& FileName,CEngine *Engine)
{
    std::string GroupName="Root_";

    char buffer[512];
    char *Param;
    char *Value;
    std::string ParamStr;
    std::string ValueStr;

    std::ifstream infile(FileName.c_str());

    if (! infile.is_open() )
    {
        //      fprintf(stderr,"Can't load %s file as ini-file!\n",FileName.c_str());
        return;
    };

//    fprintf(stdout,"Using ini-file: %s\n",FileName.c_str());

    while(!infile.eof())
    {
        infile.getline(buffer, sizeof(buffer)); //читаем строку из файла
        if ( buffer[0]==';' )
            continue; //продолжаем господа, это комментарий!
        Param = strtok(buffer, "="); // берём первый токен до знака =
        if(Param == NULL)
            continue; // если такого нет - продолжаем цикл
        ParamStr=Param; //в string его !
        if (ParamStr[0]=='[')
        {
            GroupName=Trim(ParamStr);
        } //  имя группы [Группа] в ini-файле
        else
        {
            Value = strtok(NULL, "="); // читаем что там дальше, надеюсь знака = больше не будет
            if ( Value!=NULL )
            { // если получилось
                ValueStr=Value; // в string его
                 printf("{%s}={%s}\n",(GroupName+Trim(ParamStr)).c_str(),Trim(ValueStr).c_str());
                Engine->SetEnvParam(ASN_Str(GroupName+Trim(ParamStr)).c_str(),Trim(ValueStr).c_str() );
            }
            else
            {
                //       printf("{%s}={%s}\n",(GroupName+Trim(ParamStr)).c_str(),);
                //            printf("Param: {%s}\n",(GroupName+Trim(ParamStr)).c_str());
                //            Engine->SetEnvParam(GroupName+Trim(ParamStr),"1"); //ставим значение по умолчанию
            };
        };
        // printf("BUFFER: %s\n",buffer);
    };


};

/*
void FindINI(std::string DirName, bool Recursive, CEngine *Engine){
    os_finddata_t fdata;
	os_dir_t hFile;

// если путь не заканчивается слэшем ставим его
	if (DirName[DirName.length()-1]!='/') DirName=DirName+'/';

// начали
	hFile = os_findfirst((DirName).c_str(), &fdata);
	if (!hFile) return;

	do
	{

	if (fdata.name=="..") continue;
	if (fdata.name==".") continue;

	if (fdata.subdir & Recursive) {
//     printf("scan dir: %s\n", fdata.name.c_str());
     ScanDirectory(fdata.dir_name+fdata.name,true, Engine);
     }
    else {
//     printf("find file: %s\n", fdata.name.c_str());
     std::string FileExt=GetFileExt(DirName+fdata.name);
     if (FileExt==ASN_MODULE_EXTENSION_NAME) ParseIniFile(DirName+fdata.name, Engine);
         }

	} while (os_findnext( hFile, &fdata )!= NULL);
	os_findclose(hFile);

};
*/

//int alive = 0;
//int calc_update = 0;

/*
int ThreadFunc(void* data){

    CEngine* ptrEngine = (CEngine*) data;

    while(true){
//	if (alive==0) return 0;
//	if (alive==1)
//	    if ( !ptrCore->Process() ) { cout << "proc end" << endl; return 0; }
	calc_update = 1;
	ptrEngine->EngineSafeUpdate();
	calc_update = 0;
//	cout << "thread.. " << endl;
	SDL_Delay(1*100);
//	ptrEngine->EngineDraw();
	}

};
*/

CEngine::CEngine( int argc, char *argv[] )
{

std::string window_name;

if (argc>0) {

    SetEnvParam("ProgName", argv[0]);
    window_name = GetFileName(argv[0]);

    SetEnvParam("[DeviceManager]window_name", window_name.c_str() );

for (int i=0; i<argc; i++){
 printf("%s \n", argv[i] );
}

}  // argc>0

    engine_state = 0;

    // значения по умолчанию
    SetEnvParam("[Paths]DataPath","../data/");
    SetEnvParam("[Paths]PluginsPath","./plugins/");
    SetEnvParam("[Paths]LogPath","./");

    SetEnvParam("[Setup]Console", "1");

    SetEnvParam("[Modules]LogManager", "LogTxt");
    SetEnvParam("[Modules]FileManager", "FileSystemASN");
    SetEnvParam("[FileManager]DataReader", "FSDisk, FSZIP"); // если закомментировать смысла в FileSystem не будет :)

    mainWorld = NULL;

    CPluginFactory *myPluginFactory = new CPluginFactory(this);
    SetPtrParam("PluginManager", myPluginFactory);

    IResourceSystem *myRes = new CResourceSystem();
    ((CResourceSystem*)myRes)->Engine=this;
    SetPtrParam("ResourceSystem",myRes);

//    FindINI(".", true, this);

    ParseIniFile("AmberSkyNet_default.ini",this); // parse original
    ParseIniFile("AmberSkyNet.ini",this);

    if ( window_name!="AmberSkyNet" ) {
        ParseIniFile(std::string(window_name+".ini").c_str(),this);
    }
//    printf("%s\n", GetEnvParam("Console") );

    if (strcmp(GetEnvParam("[Setup]Console"), "1")==0) {

    } else {

    freopen("log.txt", "w+t", stdout);       //РїРµСЂРµРЅР°РїСЂР°РІР»СЏРµРј РёС… РІ РЅСѓР¶РЅС‹Р№ Р»РѕРі С„Р°Р№Р»
    freopen("err.txt", "w+t", stderr);

    }

//    CDataParserLoader* myLoad = new CDataParserLoader();
    myPluginFactory->LoadGenerator( new CDataParserLoader() );
    myPluginFactory->LoadGenerator( new CLogLoader() );
    myPluginFactory->LoadGenerator( new CFSLoader() );

    myPluginFactory->ScanDirectory(GetEnvParam("[Paths]PluginsPath"),true);

    // создаём менеджер логов по имени
    ILog *myLog = dynamic_cast<ILog*>( myPluginFactory->CreateObject( GetEnvParam("[Modules]LogManager") ) );

    if (!myLog)
        printf("Log %s can't created!\n",GetEnvParam("[Modules]LogManager") );
    else {
//  ManagerList.AddItem(myLog); // not add, Ф.Л. ХДБМСЕФУС РПУМЕДОЙН!
    SetPtrParam("LogManager", myLog);
    }

    std::map<std::string, std::string> ManagerContent; // УРЙУПЛ РПФЕОГЙБМШОЩИ НЕОЕДЦЕТПЧ, УОБЮБМБ УЛБОЙТХЕН РЕТЕНЕООЩЕ УТЕДЩ. ЧЩДЕМСС ЙЪ ОЙИ ЙНЕОБ НЕОЕДЦЕТПЧ

    std::map<std::string,std::string>::iterator it = Content.begin();

    while ( it!=Content.end() ) {
//	printf( "[%s]\n", (*it).first.c_str() );
        std::string SectionName( (*it).first.begin(), (*it).first.begin() + (*it).first.find_last_of("]")+1 );
        if ( strcmp(SectionName.c_str(),"[Modules]")==0 ){ // если имя параметра начинается с этого
            // отсекаем имя секции от параметра
            std::string ManagerName( (*it).first.begin() + (*it).first.find_last_of("]")+1, (*it).first.end() );
	    if ( strcmp(ManagerName.c_str(), "LogManager")!=0){
        	 std::string ManagerClassName = (*it).second;
//        	 IManager* myManager = (IManager*)myPluginFactory->CreateType( (*it).second );
//        	 if (!myManager) printf("Can't create %s manager as %s", ManagerName.c_str(), (*it).second.c_str() );
//        	 else {
	              printf("[%s]==[%s]\n", ManagerName.c_str(), (*it).second.c_str() );
		      ManagerContent[ManagerName] = (*it).second;
//        	      ManagerList.AddItem(myManager);
//        	      SetPtrParam(ManagerName.c_str(), myManager);
//        	      }
        	  }
            }
        it++;
    }

    ////// шерстим все параметры, отбираем те, в которых в начале есть [Modules]

    std::map<std::string,std::string>::iterator it_mc = ManagerContent.begin();

    while ( it_mc!=ManagerContent.end() ) {

//      	 std::string ManagerClassName = (*it).second;
        	 IManager* myManager = dynamic_cast<IManager*>( myPluginFactory->CreateObject( (*it_mc).second.c_str() ));
        	 if (!myManager) printf("Can't create %s manager as %s", (*it_mc).first.c_str(), (*it_mc).second.c_str() );
        	 else {
        	      ManagerList.AddItem(myManager);
        	      SetPtrParam( (*it_mc).first.c_str(), myManager);
		      printf("[%s]~~~[%s]\n", (*it_mc).first.c_str(), (*it_mc).second.c_str() );
        	      }
        it_mc++;
        //cout << "..." << endl;
    }

/*
    // создаём менеджер логов по имени
    ILog *myLog = (ILog *)myPluginFactory->CreateType( GetEnvParam("[Modules]LogManager") );
    SetPtrParam("LogManager",myLog);

    if (!myLog)
        printf("Log %s can't created!\n",GetEnvParam("[Modules]LogManager") );

    myLog->LogMsg("AmberSkyNet Log started");

    IFileSystem *myFS = (IFileSystem *)myPluginFactory->CreateType( GetEnvParam("[Modules]FileManager") );
    SetPtrParam("FileManager",myFS);

    if (!myFS)
        myLog->ErrMsgStr(1,std::string("File System can't init :(")+ASN_Str(GetEnvParam("[Modules]FileManager")));

    myLog->LOG("AmberSkyNet File system  started");

//    IDevice *myDevice = (IDevice *)myPluginFactory->CreateType( GetEnvParam("[Modules]DeviceManager") );
//    SetPtrParam("DeviceManager",myDevice);
//    if (!myDevice)
//        myLog->ErrMsgStr(1,std::string("Device System can't init :(")+GetEnvParam("[Modules]DeviceManager"));
//
//    myLog->LOG("AmberSkyNet DeviceGL started");

    void* myDevice = myPluginFactory->CreateType( GetEnvParam("[Modules]DeviceManager") );
    SetPtrParam("DeviceManager",myDevice);
    if (!myDevice)
        myLog->ErrMsgStr(1,std::string("Device System can't init :(")+GetEnvParam("[Modules]DeviceManager"));

    myLog->LOG("AmberSkyNet DeviceGL started");

//    IEventManager *myEvMan = (IEventManager *)myPluginFactory->CreateType( GetEnvParam("[Modules]EventManager") );
//    SetPtrParam("EventManager",myEvMan);
//    if (!myEvMan)
//        myLog->ErrMsgStr(1,std::string("Event System can't init :(")+GetEnvParam("[Modules]EventManager"));
//    myLog->LOG("AmberSkyNet EventManagerSDL started");

    myDevice = myPluginFactory->CreateType( GetEnvParam("[Modules]EventManager") );
    SetPtrParam("EventManager",myDevice);
    if (!myDevice)
        myLog->ErrMsgStr(1,std::string("Event System can't init :(")+GetEnvParam("[Modules]EventManager"));
    myLog->LOG("AmberSkyNet ScriptSystem started");

    // начинаем инициализировать звук
    SetPtrParam("SoundManager",NULL);
    if ( ASN_Str(GetEnvParam("[SoundManager]SoundEnabled"))=="1")
    {
        ISound *mySound = (ISound *)myPluginFactory->CreateType( GetEnvParam("[Modules]SoundManager") );
        SetPtrParam("SoundManager",mySound);
        if (!mySound)
        {
            myLog->ErrMsgStr(1,std::string("Sound System can't init :(")+GetEnvParam("[Modules]SoundManager"));
            //SetEnvParam("[SoundManager]SoundEnabled","0");
        };
    }

    // если звук отключен, то создаём заглушку для звука
    if ( !GetPtrParam("SoundManager") ) {
        SetPtrParam("SoundManager",new CSoundSDL_empty(this) );
    };

    if ( ASN_Str(GetEnvParam("[Modules]NetManager")) !="N/A")
    {
        INetSupervisor *myNet = (INetSupervisor*)myPluginFactory->CreateType( GetEnvParam("[Modules]NetManager") );
        SetPtrParam("NetManager",myNet);
        if (!myNet)
            myLog->ErrMsgStr(1,std::string("Network System can't init :(")+GetEnvParam("[Modules]NetManager"));
    }

    //IPhysicManager* myPhys = (IPhysicManager*)myPluginFactory->CreateType( "PhysicManager" );
    //if (!myPhys)
    //myLog->ErrMsg(1,"PhysicManager System can't init :(");
    //SetPtrParam("PhysicManager", myPhys);

    void* myDevice_ptr = myPluginFactory->CreateType( GetEnvParam("[Modules]ScriptManager") );
    SetPtrParam("ScriptManager",myDevice_ptr);
    if (!myDevice_ptr)
        myLog->ErrMsgStr(1,std::string("Script System can't init :(")+GetEnvParam("[Modules]ScriptManager"));
    myLog->LOG("AmberSkyNet ScriptSystem started");
*/
//    myLog->LOG("AmberSkyNet Engine started");
};

void CEngine::Release()
{
    printf("try release engine...\n");
    delete this;
};

CEngine::~CEngine()
{

//    ((ILog *)GetPtrParam("LogManager"))->LOG("AmberSkyNet Engine say bye...");

//    for (UINT i=0; i<WorldList.GetSize(); i++)
    //{
        //IWorld *myWorld=(IWorld*)WorldList.GetItem(i);
    printf("CEngine: Release mainWorld..\n");
      if ( mainWorld) mainWorld->Release();
      mainWorld=NULL;
    printf("CEngine: Release mainWorld OK\n");

//    }

    for (UINT i=0; i<ManagerList.GetSize(); i++)
    {
        IManager* myManager = (IManager*)ManagerList.GetItem(i);
        printf( "try release manager: %s\n", myManager->GetType() );
        myManager->Release(); // ЛПТТЕЛФОП ХДБМСЕН НЕОЕДЦЕТЩ
        printf( "release manager OK\n" );
    }

    //    for (UINT i=0; i<Listeners.GetSize(); i++){
    //      IWorld *myWorld=(IWorld*)WorldList.GetItem(i);
    //      myWorld->Release();
    //    }

/*
    INetSupervisor *myNet = (INetSupervisor *)GetPtrParam("NetManager");
    if (myNet)
        delete myNet;

    ISound *mySound = (ISound *)GetPtrParam("SoundManager");
    if (mySound)
        delete mySound;

    IEventManager *myEvn = (IEventManager *)GetPtrParam("EventManager");
    if (myEvn)
        myEvn->Release();

    IDevice *myDevice = (IDevice *)GetPtrParam("DeviceManager");
    if (myDevice)
        myDevice->Release();


    printf("delete FileSystem\n");

    IFileSystem *myFS = (IFileSystem *)GetPtrParam("FileManager");
    if (myFS)
        myFS->Release();
*/
    ILog *myLog = (ILog *)GetPtrParam("LogManager");
    myLog->LOG("AmberSkyNet Engine ended");

    if (myLog)
        myLog->Release();

    printf("delete PluginFactory\n");

    CPluginFactory *myPluginFactory = (CPluginFactory *)GetPtrParam("PluginManager");
    if (myPluginFactory)
        delete myPluginFactory;

    printf("Engine completely deleted\n");

};


bool CEngine::AddCustomManager( IManager* CustomManager ){

    cout << "Name:"       << CustomManager->GetName() << endl;
    cout << "Type:"       << CustomManager->GetType() << endl;
    cout << "BaseClass: " << CustomManager->GetBaseClass() << endl;

    ManagerList.AddItem(CustomManager);
    SetPtrParam( CustomManager->GetName(), CustomManager );

};

void CEngine::SetPtrParam(const char* param_name, void *param_value)
{
    VoidContent[param_name]=param_value;
};

void *CEngine::GetPtrParam(const char* param_name)
{
    std::map<std::string,void *>::iterator v_it=VoidContent.find(param_name);
    if (v_it!=VoidContent.end())
        return (*v_it).second;

    return NULL;

};

void CEngine::DelPtrParam(const char* param_name)
{
    std::map<std::string,void *>::iterator v_it=VoidContent.find(param_name);
    if (v_it!=VoidContent.end())
        VoidContent.erase(v_it);
};

// функции установки/чтения переменных окружения
void CEngine::SetEnvParam(const char* param_name, const char* param_value)
{
    //printf("Set param [%s]={%s}\n",param_name.c_str(),param_value.c_str());
    Content[param_name]=param_value;
    //printf("Set param [%s]=%s\n",param_name.c_str(),param_value.c_str());
};

const char* CEngine::GetEnvParam(const char* param_name)
{

    std::map<std::string,std::string>::iterator it=Content.find(param_name);
    if (it!=Content.end())
        return (*it).second.c_str();
    else
        return ASN_NOT_FOUND_PARAM;

};

void CEngine::DelEnvParam(const char* param_name)
{

    std::map<std::string,std::string>::iterator it = Content.find(param_name);
    if (it!=Content.end())
        Content.erase(it);

};



IBaseObject* CEngine::CreateObject(const char *ObjName)
{

cout << "CEngine::CreateObject " << ObjName << endl;

    IBaseObject* object = CreateBaseObject( ObjName );
    if (object) return object;

//    cout << "Create object " << ObjName << endl;
    CPluginFactory* myFactory = (CPluginFactory*) GetPtrParam("PluginManager");
    if (!myFactory) { cout << "PluginManager not found!" << endl; }
//    cout << "Create object " << ObjName << endl;
//    std::string myObjName = ObjName;
    object = myFactory->CreateObject(ObjName); // TODO IBASE_OBJECT
//    cout << "Create object " << ObjName << endl;
    if (!object) LOGGER->ErrMsgStr(2,std::string("Engine can't create")+ObjName+" object!");
//    cout << "Create object " << ObjName << endl;
//    cout << "create object:" << endl;
//    cout <<

    return object;
};

//void* CEngine::CreateObjectsByTemplate(const char *TemplateType, void *Template)
//{}
//;

/*
bool CEngine::SetWorld(UINT id)
{

    if (  WorldList.GetItem( id )!=NULL)
    {
        numCurrentWorld=id;
        return false;
    };

    return true;

};
*/

//bool CEngine::SetWorld(IWorld* customWorld)
//{

/*    if (  WorldList.GetItem( id )!=NULL)
    {
        numCurrentWorld=id;
        return false;
    };
*/
//    if (mainWorld) mainWorld->Release();
//    mainWorld = customWorld;
//    return true;
//
//};



IWorld* CEngine::GetWorld(/*UINT id*/)
{
    //return (IWorld*) WorldList.GetItem( id );
        return mainWorld;
};

IWorld* CEngine::CreateWorld(const char* WorldName){

    IWorld *myWorld = dynamic_cast<IWorld*>( CreateObject( WorldName ) );
    if (!myWorld)
    {
        printf("Sorry, %s plugin not found :(\n", WorldName );
        return NULL;
    }
    if (mainWorld) mainWorld->Release();
    mainWorld = myWorld;
    return myWorld;
};

// удаляем мир по ID
/*
bool CEngine::DelWorld(UINT id)
{

    IWorld *myWorld=(IWorld*)WorldList.GetItem(id);
    if (myWorld)
    {
        myWorld->Release();
        WorldList.DelItem(id);
        return false;
    }

    return true;
};
*/

/*
UINT CEngine::AddWorld(IWorld* customWorld)
{
    numCurrentWorld = WorldList.AddItem( customWorld );
    return numCurrentWorld;
};
*/

/*

bool CEngine::LoadWorld()
{

    // пробуем найти такой файл...
    IFile *myFile=((IFileSystem*)GetPtrParam("FileManager"))->GetFile( FileName );
    if (!myFile)
        return true; //файла нет такого
    myFile->FileOpen();
    void *data=myFile->LoadInMem();
    if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM)
        return true; // не грузится он :(

    //// пока так, а потом посмотрим...
    IDataParser* parser=(IDataParser *)((IPluginFactory *)GetPtrParam("PluginManager"))->CreateType("DataParserXML");
    if (!parser)
    {
        ((ILog*)GetPtrParam("LogManager"))->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        return true;
    };

    /// получаем корень данных
    DataLeaf root=parser->StartParse(data);

    /// получаем раздел "World"
    DataLeaf firstLeaf=parser->GetSectionByName(root,"World");
    if (!firstLeaf)
    {
        delete parser;
        return true;
    } //нет такого - возврат...

    asn_ext_param_list* mylist= createList();
    parser->GetLeafParamList(firstLeaf, mylist);
    asn_List tempParamList;
    StrPtrToList( mylist, tempParamList );
    deleteList( mylist );

    std::string worldType = tempParamList["Type"].c_str(); // тип World Manager'а

    IWorld *myWorld = (IWorld*) CreateObject ( worldType.c_str() );
    if (!myWorld)
    {
        myFile->FileClose();
        delete parser;
        return true;
    } // нет такого :(

    /// устанавливаем все параметры менеджера миров
    asn_List::iterator it=tempParamList.begin();
    for( UINT i=0; i<tempParamList.size(); ++i)
    {
        myWorld->SetParam(((*it).first).c_str(), ((*it).second).c_str() );
        it++;
    };

    firstLeaf=parser->GetFirstChildLeaf(firstLeaf);

    myFile->FileClose();
    delete parser;

    if (mainWorld) mainWorld->Release();
    mainWorld = myWorld;

//    id = WorldList.AddItem( myWorld );
//    numCurrentWorld=id;
    return false;
};
*/

//bool CEngine::SaveWorld(const char* FileName/*, UINT id*/){
//
//};


bool CEngine::EngineInit()
{

    if (engine_state!=0) return false;


    for (UINT i=0; i<ManagerList.GetSize(); i++)
    {
        IManager* myManager = (IManager*)ManagerList.GetItem(i);
	cout << "Manager: " << i << endl;
        myManager->InitManager();
	cout << "Manager: " << i << "init ok" << endl;
    }

///////////// init video ////////////////////////

    int start_param=0;

    int width = 640;
    int height = 480;

    if ( strcmp(GetEnvParam("[DeviceManager]Resize"), "1")==0 )
        start_param |= ASN_DEV_CANRESIZE;

    if ( strcmp(GetEnvParam("[DeviceManager]FullScreen"), "1")==0 )
        start_param |= ASN_DEV_FULLSCREEN;

    if ( strcmp(GetEnvParam("[DeviceManager]NoFrame"), "1")==0 )
        start_param |= ASN_DEV_NOFRAME;

    sscanf( GetEnvParam("[DeviceManager]Width") ,"%d",&width );
    sscanf( GetEnvParam("[DeviceManager]Height") ,"%d",&height );

IDevice* myDevice=(IDevice*)GetPtrParam("DeviceManager");
if (myDevice) {

if ( myDevice->Init(width, height, 0, start_param) )
    {
        printf("Ooops... video error!\n");
        return false;
    }

    if ( strcmp(GetEnvParam("[DeviceManager]MouseHide"), "1")==0 )
    myDevice->CenterMouse(false);
    else myDevice->CenterMouse(true);

}
    engine_state = 1;

    return true;

};

    int non_exit=false;
    int no_draw=false; // нужна ли отрисовка "по умолчанию"

// игровой цикл
bool CEngine::EngineStart()
{

    if (engine_state==0)
    if ( !EngineInit() ) return false;

/*    if (WorldList.GetSize()==0)
    {

        LOGGER->ErrMsg(3,"World list is empty!");
        return true;

    }
*/
    if (!mainWorld) {
        LOGGER->ErrMsg(3,"World is empty!");
        return true;
    };

    IDevice* myDevice=(IDevice*)GetPtrParam("DeviceManager");
    IEventManager* myEvManager=(IEventManager*)GetPtrParam("EventManager");

//    alive = 1;
//    coreThread = SDL_CreateThread(ThreadFunc, this);
//    if ( coreThread == NULL ) {
//	LOGGER->ErrMsg(3, std::string( std::string("Couldn't create thread: ") + SDL_GetError() ).c_str() );
//	return true;
//	}

    while ( non_exit==0 )
    {

	EngineDraw();
//	printf("end draw engine\n");
	EngineUpdate();
//	printf("end update engine\n");

    };

};

void CEngine::EngineDraw(){

//    if (calc_update) return;

	void *Draw=GetPtrParam("DrawCallback");
	void *DrawParam=GetPtrParam("DrawCallbackParam");

        IDevice* myDevice=(IDevice*)GetPtrParam("DeviceManager");
        myDevice->StartDraw();


        if (Draw)
        {
//printf("end draw engine\n");
            CALLBACK_ASN_FUNCTION myCustomCallback=(CALLBACK_ASN_FUNCTION)Draw;
            no_draw=myCustomCallback(DrawParam);

        }
        else
        {
//printf("end draw engine2\n");
//            IWorld *myWorld=(IWorld*)WorldList.GetItem(numCurrentWorld); //нумерация с 0
            if (mainWorld)
                mainWorld->Draw();

        };
//printf("end draw engine3\n");
        myDevice->EndDraw();
//printf("end draw engine4\n");
};

void CEngine::EngineUpdate(){

	void *Update=GetPtrParam("UpdateCallback");
        void *UpdateParam=GetPtrParam("UpdateCallbackParam");

        if (Update)
        {

            CALLBACK_ASN_FUNCTION myCustomCallback=(CALLBACK_ASN_FUNCTION)Update;
//            cout << "non_exit " << non_exit << endl;
            non_exit=myCustomCallback(NULL);
//            cout << "non_exit " << non_exit << endl;

        }
        else
        {

            IEventManager* myEvManager=(IEventManager*)GetPtrParam("EventManager");
            // читаем первое событие
            int evt=myEvManager->GetInput();
            // цикл - пока не прочитали все события
            while (evt!=ASN_EVNT_EMPTY)
            {
                evt=myEvManager->GetInput();
            }

        }

    IDevice* myDevice=(IDevice*)GetPtrParam("DeviceManager");
	float tms = 10.0f*myDevice->FrameInterval;

        for (UINT i=0; i<ManagerList.GetSize(); i++)
        {
            IManager* myManager = (IManager*)ManagerList.GetItem(i);
            myManager->Update( tms );
        }

/*        for (UINT i=0; i<WorldList.GetSize(); i++)
        {
            IWorld *myWorld=(IWorld*)WorldList.GetItem(i);
            if (myWorld)
                myWorld->Update( tms );
        }
        */
        if (mainWorld) mainWorld->Update( tms );

};


void CEngine::EngineSafeUpdate(){
	cout << "thread.. " << endl;

        IEventManager* myEvManager=(IEventManager*)GetPtrParam("EventManager");
	            int evt=myEvManager->GetInput();
            // цикл - пока не прочитали все события
            while (evt!=ASN_EVNT_EMPTY)
            {
                evt=myEvManager->GetInput();
            }


    IDevice* myDevice=(IDevice*)GetPtrParam("DeviceManager");

    if (mainWorld)
        mainWorld->Update(10.0f*myDevice->FrameInterval);

/*
        for (UINT i=0; i<WorldList.GetSize(); i++)
        {
            IWorld *myWorld=(IWorld*)WorldList.GetItem(i);
            if (myWorld)
                myWorld->Update(10.0f*myDevice->FrameInterval);
                cout << i << endl;
        }
        */

}

class SaveVisitor: public IBaseObjectVisitor {
    public:

    SaveVisitor(IDataParser* pParser, DataLeaf pRoot): myParser(pParser),myRoot(pRoot){}

    void Visit( IBaseObject* obj){

        const char* Name = obj->GetType();
        DataLeaf tempLeaf = myParser->AddLeafWithName( myRoot, Name );
        myRoot = tempLeaf;

        asn_ext_param_list* tempParamList = createList();
        obj->GetParamList(tempParamList);

        for( UINT i=0; i<tempParamList->nums; ++i){
                  myParser->SetParam(myRoot, tempParamList->param_list[i].name , tempParamList->param_list[i].value );
        };

        deleteList( tempParamList );

    }

    void AfterVisit( IBaseObject* obj){

        myRoot = myParser->GetParent( myRoot );
    }

    IDataParser* myParser;
    DataLeaf myRoot;

};

bool CEngine::SaveDataFile(const char *FileName, IBaseObject* base_root){

    IDataParser* parser = new CDataParserXML ( this );
    DataLeaf root=parser->CreateRootLeaf();

    // создаём раздел BaseObject
    // DataLeaf tempLeaf=parser->AddLeafWithName(root, "BaseObject");

    // создаём раздел World
    //DataLeaf tempLeaf=parser->AddLeafWithName(root, "World");

    // вызываем рекурсивную функцию перевода нод в цифровую форму (см. начало файла этого, да?)
    //Node2Leaf(parser, tempLeaf , root_node );

    // и сохраняем то, что напарсили в файл с указаным именем
    SaveVisitor* myVis = new SaveVisitor(parser, root);

    if (base_root) base_root->Visit( myVis);

    parser->SaveDataTree( FileName );

    delete parser;
    return true;
}

bool CEngine::LoadDataFile(const char *FileName, IBaseObject* base_root){
// File2Data( FileName );
//return ;
/* МЙТЙЮЕУЛПЕ ПФУФХРМЕОЙЕ ПФ..

уОБЮБМБ ВЩМБ УДЕМБОБ ОБУФПСЭБС ЙЕТБТИЙЮЕУЛБС ТБУЛТХФЛБ ЧУЕИ ХЪМПЧ, ЧЛМАЮБС ХЪМЩ <file ..>
пДОБЛП Ч РТПГЕУУЕ ФЕУФЙТПЧБОЙС ВЩМБ ЧЩСЧМЕОБ ИЕТОШ - РБТУЕТ ОЕ РПЪЧПМСМ "РБТУЙФШ" УТБЪХ ОЕУЛПМШЛЙНЙ ЬЛЪЕНРМСТБНЙ
ЛМБУУБ ТБЪОЩЕ xml-ЖБКМЩ, РПЬФПНХ ОБЮЙОБМЙ РПСЧМСФШУС УФТБООЩЕ ЗМАЛЙ. рПУМЕ ЬФПЗП ВЩМП РТЙОСФП ТЕЫЕОЙЕ
УОБЮБМБ РТПКФЙУШ РП ЖБКМХ У ЙНЕОЕН FileName, РПМХЮЙЧ ЙЪ ОЕЗП ЧУЕ ЙНЕОБ ЧОЕЫОЙИ ЖБКМПЧ, Б РПФПН РП ЛБЦДПНХ
ЙЪ РПМХЮЕООЩИ ЖБКМПЧ У ФПК ЦЕ ГЕМША. рПУМЕ ЬФПК УФТБООПК РТПГЕДХТЩ Х ОБУ Ч result_list РПМХЮБЕФУС ТЕБМШОЩК
УРЙУПЛ ЖБКМПЧ, ЛПФПТЩК ОБДП ЪБЗТХЪЙФШ. ъБЗТХЦБЕН ЙЪ ОЕЗП ЖБКМЩ РП ПЮЕТЕДЙ. ОБЮЙОБС У ЛПОГБ
РТЙ РПНПЭЙ ЖХОЛГЙЙ File2Data, ЙЗОПТЙТХС ХЦЕ ЧУЕ ХЪМЩ c ХЛБЪБОЙЕН ЧОЕЫОЙИ ЖБКМПЧ.

*/

// base file list
    std::list<std::string> file_list;
// result file list
    std::list<std::string> result_list;

    file_list.push_back( FileName );
//    result_list.push_back( FileName );



////////////// ОБЮБМП ГЙЛМБ //////////////////////////////////
    do {
    std::string file_name = file_list.back();
    file_list.pop_back();

cout << "---------- try load " << file_name << " -------------" << endl;

    IFile *myFile=((IFileSystem*)GetPtrParam("FileManager"))->GetFile( file_name.c_str() );

    if (!myFile) {
	LOGGER->ErrMsgStr(3, "Sorry, can't find " +std::string(FileName) );
	return false;
	}

//    myFile->FileOpen();
    void *data;

    while (true ) {

    data=myFile->LoadInMem();
    if (myFile->GetState()==ASN_FILE_NOT_FOUND){
        LOGGER->ErrMsgStr(3, "Sorry, can't load XML :( "+std::string(FileName) );
        return false;
	}
    if (myFile->GetState()==ASN_FILE_DISK_IN_MEM) break;

    if (NETTER) NETTER->Check(); // ДМС ЧПЪНПЦОПУФЙ ХДБМЕООП УЛБЮБФШ ЮФП-ФП

    //cout << "state: " << myFile->GetState() << endl;

    }

    if (!data) return false;
    char* s_data = (char*)data;
    s_data[myFile->GetSizeInBytes()]='\0';

std::string tmp_data = s_data;

//cout << "---------- parse data -------------" << endl;

//cout << tmp_data << endl;

//cout << "---------- parse data -------------" << endl;

//return;

    //// УПЪДБЕН ОПЧЩК РБТУЕТ
    //IDataParser* parser=(IDataParser *)((CPluginFactory *)GetPtrParam("PluginManager"))->CreateType("DataParserXML");
    //if (!parser)
    //{
        //((ILog*)GetPtrParam("LogManager"))->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        //return;
    //};


    IDataParser* parser = new CDataParserXML ( this );
    DataLeaf root=parser->StartParse(data);

    DataLeaf childLeaf=parser->GetFirstChildLeaf(root);

    if (childLeaf!=NULL) do {

	std::string ObjName=parser->GetLeafName(childLeaf);
	printf("Leaf name: %s\n",ObjName.c_str());

	if (ObjName=="file") {
		std::string FileName_ =  parser->GetAttribute( childLeaf, "Name");
		//std::string FileName_ =  parser->GetAttribute( childLeaf, "Name");
		cout << "name : " << FileName_ << endl;
		//printf("Leaf FileName: %s\n",FileName_.c_str());
		//LoadDataFile( FileName );
		file_list.push_back( FileName_ );
	}

    } while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL );

    delete parser;
    myFile->FileClose();

    result_list.push_back( file_name );

    } while ( !file_list.empty() );

///////////// ЛПОЕГ ГЙЛМБ ////////////////////////////////////


    do {

    std::string file_name = result_list.back();
    result_list.pop_back();
    cout << "name : " << file_name << endl;
    File2Data( file_name.c_str(), base_root );

    } while (!result_list.empty() );

    return true;
}

void CEngine::File2Data(const char *FileName, IBaseObject* base_root){

// cout << GetEnvParam("[Paths]DataPath") << endl;
     cout << "-------------------------- " << "CEngine::File2Data try load " << FileName << endl;
// пробуем найти такой файл...
    IFile *myFile=((IFileSystem*)GetPtrParam("FileManager"))->GetFile( FileName );

    if (!myFile) {
	LOGGER->ErrMsgStr(3, "Sorry, can't find " +std::string(FileName) );
	return; //файла нет такого
	}

    myFile->FileOpen();
    void *data;

    while (true ) {

    data=myFile->LoadInMem();
    if (myFile->GetState()==ASN_FILE_NOT_FOUND){
        LOGGER->ErrMsgStr(3, "Sorry, can't load XML :( "+std::string(FileName) );
        return;
	}
    if (myFile->GetState()==ASN_FILE_DISK_IN_MEM) break;

    if (NETTER) NETTER->Check(); // ДМС ЧПЪНПЦОПУФЙ ХДБМЕООП УЛБЮБФШ ЮФП-ФП

//    cout << "--------- " << FileName << " state: " << myFile->GetState() << endl;

    }

    if (!data) return;

//    myFile->FileOpen();

//    void *data=myFile->LoadInMem();
//    if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM){
//        cout << "Sorry, can't load XML :( " << FileName << endl;
//        return; // не грузится он :(
//	}

    //// пока так, а потом посмотрим...
    //IDataParser* parser=(IDataParser *)((CPluginFactory *)GetPtrParam("PluginManager"))->CreateType("DataParserXML");
    //if (!parser)
    //{
        //((ILog*)GetPtrParam("LogManager"))->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        //return;
    //};
    char* s_data = (char*)data;
    s_data[myFile->GetSizeInBytes()]='\0';
//cout << "----- File2Data try parse: ------" << s_data << "----- File2Data try parse (end): ------ " << endl;
    IDataParser* parser = new CDataParserXML ( this );
    /// получаем корень данных
    DataLeaf root=parser->StartParse(data);
//cout << "----- File2Data try parse: ------" << s_data << "----- File2Data try parse (end): ------ " << endl;
    LoadData( parser, root, base_root );

    delete parser;
    myFile->FileClose();

};

void CEngine::LoadData(IDataParser* parser, DataLeaf leaf, IBaseObject* prev_ptr ){
cout << "CEngine::LoadData" << endl;

    DataLeaf childLeaf=parser->GetFirstChildLeaf(leaf);
    if (childLeaf==NULL) return;

do {

	std::string ObjName=parser->GetLeafName(childLeaf);
	printf("Leaf name: %s\n",ObjName.c_str());

	if (ObjName=="file") {
//		std::string FileName =  parser->GetAttribute( childLeaf, "Name");
//		//cout << "name : " << << endl;
//		printf("Leaf FileName: %s\n",ObjName.c_str());
//		LoadDataFile( FileName.c_str() );
	}
	else
	if (ObjName=="BaseObject") {

		std::string myType = "BaseObject";
		printf("Leaf {BaseObject}\n");
//		LoadBaseObject( parser, childLeaf );
		DataLeaf childLeaf2=parser->GetFirstChildLeaf(childLeaf);

//    		if (childLeaf==NULL) return;

    do {

//	for(int i=0;i< 2;i++) {

//	sparser->GetAsText( childLeaf );
	std::string ObjName=parser->GetLeafName(childLeaf2);
	printf("Leaf BaseObject Name: %s\n",ObjName.c_str());
	std::string ObjValue = parser->GetAsText( childLeaf2 );
	printf("ObjValue: %s\n",parser->GetAsText( childLeaf2 ));
//	parser->GetAsText( childLeaf );
//	BaseObjects[ ObjName ] =  "<NodeBase />";//parser->GetAsText( childLeaf

char oho[1024];
strcpy(oho, parser->GetAsText( childLeaf2 ));

	BaseObjects[ ObjName ] =  oho;//" <LogoSDL Type='NodeSprite' Texture='SDL_now.png' Size='5 5 5' Billbord='0' />";
//ObjValue;//parser->GetAsText( childLeaf );
//	}
	printf("BASE OBJECT %s - {%s}{%s}\n", ObjValue.c_str(),
	parser->GetAsText( childLeaf2 ),
	BaseObjects[ ObjName ].c_str()  );

//	BaseObjects[ ObjName ] =  parser->GetAsText( childLeaf2 );

//	if ( BaseObjects[ ObjName ]!=ObjValue) cout << "BBBIIIIIIIIIILLLLLLLAAAADDDD!!!!!" << endl;
// не разбираем что у нас внутри, а все дочерние параметры "уминаем" унутрь base object
//	LoadBaseObject( parser, childLeaf );
	//CheckBase( ObjName.c_str() );
	} while ( (childLeaf2=parser->GetNextSiblingLeaf(childLeaf2))!=NULL );


	}
	else {//if (prev_ptr) { // будем смотреть что у нас в prev_ptr

		printf("try create %s...\n",ObjName.c_str());

		IBaseObject* object = Leaf2Object( parser, childLeaf );

		if (!object) printf("%s-%s[ERROR!]\n", object->GetType(), object->GetBaseClass() );
		else {
		printf("%s-%s[OK]\n", object->GetType(), object->GetBaseClass() );
		if ( ObjectAddObject( prev_ptr, object ) ) LoadData( parser, childLeaf, object );
		else { object->Release(); printf("[false]");// ЖЙЗОС ЛБЛБС-ФП Ч ИНМ
            }
		}

	} //else { cout << "<<<<<<<<<<<<<<<< BAAAMMSSS <<<<<<<<<<<<<<< " << ObjName << endl; }

} while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL );

}


IBaseObject* CEngine::Leaf2Params(IDataParser* parser, DataLeaf leaf, IBaseObject* object ){

        asn_ext_param_list* tempParamList = createList();
        parser->GetLeafParamList(leaf,tempParamList);

        for( UINT i=0; i<tempParamList->nums; ++i)
        {

//	printf("param name=%s\n", tempParamList->param_list[i].name);

        if ( strcmp(tempParamList->param_list[i].name,"Type")==0){ }
        else if ( strcmp(tempParamList->param_list[i].name,"BaseClass")==0){ }
//        else if ( strcmp(tempParamList->param_list[i].name,"Name")==0 ){ }
        else object->SetParam(tempParamList->param_list[i].name, tempParamList->param_list[i].value );

    };

        deleteList( tempParamList );

	return object;
};

IBaseObject* CEngine::Leaf2Object(IDataParser* parser, DataLeaf leaf) {
//cout << "CEngine::Leaf2Object" << endl;

	IBaseObject* object=NULL;
// РПМХЮБЕН ЙНС ПВШЕЛФБ ЙЪ ХЪМБ
    std::string ObjName=parser->GetLeafName(leaf);
cout << "CEngine::Leaf2Object" << ObjName << endl;

//CheckBase( ObjName.c_str() );

// ЕУМЙ ОБЫМЙ ПВШЕЛФ Ч ВБЪПЧЩИ
    std::map<std::string,std::string>::iterator it=BaseObjects.find(ObjName);
    if ( it!=BaseObjects.end() ) {

cout << "CEngine::Leaf2Object CreateBaseObject " << ObjName << endl;

	object = CreateBaseObject( ObjName.c_str() );
	if (!object) return NULL;

//УПЪДБЕН ОПЧЩК РБТУЕТ ДБООЩИ
//        IDataParser* parser2=(IDataParser *)((CPluginFactory *)GetPtrParam("PluginManager"))->CreateType("DataParserXML");
//        if (!parser2){
//        ((ILog*)GetPtrParam("LogManager"))->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
//        return NULL;
//         }

//	std::string base_value = (*it).second;
//	printf("find base object: %s{%s}\n", ObjName.c_str(), base_value.c_str() );

//        DataLeaf root = parser2->StartParse( (void*)(*it).second.c_str() );

//        DataLeaf child=parser2->GetFirstChildLeaf(root);
//	ObjName = parser2->GetAttribute(child, "Type");

//	CPluginFactory* myFactory = (CPluginFactory*) GetPtrParam("PluginManager");
//	object = myFactory->CreateObject(ObjName.c_str());
//	if (!object) { delete parser2; return NULL; }

	// ХУФБОБЧМЙЧБЕН РБТБНЕФТЩ base_node
//	object = Leaf2Params( parser2, child, object );
//        LoadData( parser2, child, object );

	// ХУФБОБЧМЙЧБЕН УЧПЙ РБТБНЕФТЩ
	object = Leaf2Params( parser, leaf, object );

//        delete parser2;
        return object;

    }

	CPluginFactory* myFactory = (CPluginFactory*) GetPtrParam("PluginManager");
	object = myFactory->CreateObject(ObjName.c_str());

	if (!object) return NULL;

	return Leaf2Params( parser, leaf, object );

}

void CEngine::CheckBase(const char* debug ){

cout << "-------------------------CEngine::CheckBase(" << debug << " - begin )--------------" << endl;
	std::map<std::string,std::string>::iterator it=BaseObjects.begin();

	while (it!=BaseObjects.end() ){

	printf("find base object: %s{%s}\n", (*it).first.c_str(), (*it).second.c_str() );
	it++;

	}
cout << "-------------------------CEngine::CheckBase(" << debug << " - end )--------------" << endl;

}

IBaseObject* CEngine::CreateBaseObject(const char* ObjName) {
//CheckBase("CreateBaseObject");
cout << "CEngine::CreateBaseObject " << ObjName << endl;


// ЕУМЙ ОБЫМЙ ПВШЕЛФ Ч ВБЪПЧЩИ
    std::map<std::string,std::string>::iterator it=BaseObjects.find(ObjName);
    if ( it==BaseObjects.end() ) return NULL;

//return NULL;
//УПЪДБЕН ОПЧЩК РБТУЕТ ДБООЩИ
        //IDataParser* parser2=(IDataParser *)((CPluginFactory *)GetPtrParam("PluginManager"))->CreateType("DataParserXML");
        //if (!parser2){
        //((ILog*)GetPtrParam("LogManager"))->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        //return NULL;
         //}
	printf("find base object: %s{%s}\n", ObjName, (*it).second.c_str() );

	IDataParser* parser2 = new CDataParserXML ( this );

	std::string base_value = (*it).second;
	printf("find base object: %s{%s}\n", ObjName, base_value.c_str() );

        DataLeaf root = parser2->StartParse( (void*)base_value.c_str() );

        DataLeaf child=parser2->GetFirstChildLeaf(root);
	ObjName = parser2->GetAttribute(child, "Type");

	CPluginFactory* myFactory = (CPluginFactory*) GetPtrParam("PluginManager");
	//CheckBase("deb1");
	IBaseObject* object = myFactory->CreateObject(ObjName);
	//CheckBase("deb2");

	if (!object) { delete parser2; return NULL; }

	// ХУФБОБЧМЙЧБЕН РБТБНЕФТЩ base_node
	object = Leaf2Params( parser2, child, object );
	//CheckBase("deb3");
        LoadData( parser2, child, object );

        delete parser2;
        return object;

}

void CEngine::LoadBaseObject(IDataParser* parser, DataLeaf leaf){

cout << "CEngine::LoadBaseObject" << endl;

    DataLeaf childLeaf=parser->GetFirstChildLeaf(leaf);

    if (childLeaf==NULL) return;

    do {

//	for(int i=0;i< 2;i++) {

//	sparser->GetAsText( childLeaf );
	std::string ObjName=parser->GetLeafName(childLeaf);
	printf("Leaf BaseObject Name: %s\n",ObjName.c_str());
	std::string ObjValue = parser->GetAsText( childLeaf );
	printf("ObjValue: %s\n",parser->GetAsText( childLeaf ));
//	parser->GetAsText( childLeaf );
//	BaseObjects[ ObjName ] =  "<NodeBase />";//parser->GetAsText( childLeaf

	BaseObjects[ ObjName ] =  ObjValue;//parser->GetAsText( childLeaf );
//	}
	printf("BASE OBJECT %s - {%s}{%s}\n", ObjValue.c_str(),
	parser->GetAsText( childLeaf ),
	BaseObjects[ ObjName ].c_str()  );
// не разбираем что у нас внутри, а все дочерние параметры "уминаем" унутрь base object
//	LoadBaseObject( parser, childLeaf );
	} while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL );

}

bool CEngine::ObjectAddObject( IBaseObject* parent, IBaseObject* child) {

    std::string myTc = child->GetBaseClass();

///// РТЕДЩДХАЭБС ОПДБ ОЙ Л ЮЕНХ ОЕ РТЙЧСЪБОБ
if ( !parent ) {

    printf( "CEngine::ObjectAddObject try NULL<-%s..\n", myTc.c_str());

    if ( myTc=="World" ) {

	IWorld* newWorld = dynamic_cast<IWorld*>(child);//(IWorld*)child->GetPtrParam("World");

	if (!newWorld) { printf("{2 is null}[error!]\n"); return false; }

	if (mainWorld) { printf("Unload old world...\n"); mainWorld->Release(); printf("Unload old world OK\n"); }
	mainWorld = newWorld;

//	printf(" %d ", mainWorld->getMaxID());

        printf("[ok]\n");

	return true;
    }

    printf("[error!]\n");
    return false;
}

    std::string myT = parent->GetBaseClass();

    printf( "CEngine::ObjectAddObject try %s<-%s..\n", myT.c_str(), myTc.c_str());

    bool res = parent->AddChild( child );

    if (!res) return false;

    CBaseObject* base_obj = dynamic_cast<CBaseObject*>( child );
    if ( base_obj->GetParentID()!=parent->GetID() ) {
        //cout << base_obj->myID << " - " << parent->getID() << endl;
        if (LOGGER) LOGGER->ErrMsgStr(3,"Error! Can't attach "+myTc+" into "+myT );
        return false;
    }

    return true;

//    if ( myT =="World" ) {

//    IWorld* myWorld = (IWorld*)parent->GetPtrParam("World");
//    if (!myWorld) {printf("{1 is null}[error!]\n");return false;}

//	if ( myTc == "Node") {

//	    INode* myNode = (INode*)child->GetPtrParam("Node");
//	    if (!myNode)  {printf("{2 is null}[error!]\n");return false;}

//	    myWorld->addNode( myNode );
//	    printf( "%d", myNode->getID() );

//	    printf("[ok]\n");
//	    return true;
//	}

//	if ( myTc == "Camera") {

//	    ICamera* myCam = (ICamera*)child->GetPtrParam("Camera");
//	    if (!myCam) {printf("{2 is null}[error!]\n");return false;}

//	    myWorld->addCamera( myCam );

//	    printf("[ok]\n");
//	    return true;
//	}
//    }

//    else
//    if ( myT =="Camera" ) {

//    ICamera* myCam = (ICamera*)parent->GetPtrParam("Camera");
//    if (!myCam) {printf("{1 is null}[error!]\n");return false;}

//    if ( myTc == "EventListener") {

//	    IEventListener* myList = (IEventListener*)child->GetPtrParam("EventListener");
//	    if (!myList)  {printf("{2 is null}[error!]\n");return false;}

//        EVENTER->AddCustomEventListener( myList );
//        myList->SetPtrParam("Camera", myCam);
        //myNodeP->addNode( myNode );

//	    printf("[ok]\n");
//	    return true;
//	}
//    }

//    else
//    if ( myT =="Node" ) {

//	if ( myTc == "Node") {

//	    INode* myNode = (INode*)child->GetPtrParam("Node");
//	    if (!myNode)  {printf("{2 is null}[error!]\n");return false;}

//	    INode* myNodeP = (INode*)parent->GetPtrParam("Node");
//	    if (!myNodeP) {printf("{1 is null}[error!]\n");return false;}

//	    myNodeP->addNode( myNode );

//	    printf("[ok]\n");
//	    return true;
//	}
//    }

    printf("[error!]\n");
    return false;

}
