#include "IEngine.h"
#include "ILog.h"
#include "IFileSystem.h"
#include "IDevice.h"
#include "ICamera.h"
#include "INetProtocolHandler.h"
#include "INetSupervisor.h"
#include "IResourceSystem.h"
#include "IEventManager.h"
#include "IEventListener.h"
#include "INodeModifier.h"
#include "IScriptManager.h"
#include "ISound.h"

#include "os_loadmodule.h"
//#include "os_loadmodule.c"

//специальный модуль для загрузки dll Engine :)

class Starter {
public:
Starter() { enghLibrary=NULL; myEngine=NULL; init_engine=false;};
~Starter(){ if (myEngine) FreeEngine();
printf("Good bye, my love, good bye...\n");
}

bool init_engine;

static Starter* GetInstance( void ){
	static Starter starter;
	return &starter;
};

IEngine* GetEngine(){
    if (!myEngine) {
        if (init_engine) printf("Sorry, I can't load Engine! :( \n");
    else printf("Can't init Engine! Call ENGINE_INIIT() before use!\n");
    }
    return myEngine;
};

#ifndef _WIN32
IEngine *LoadEngine(int argc, char *argv[], const char *ModuleName="./libAmberSkyNetVR.so"){
#else
IEngine *LoadEngine(int argc, char *argv[], const char *ModuleName="AmberSkyNetVR.dll"){
#endif
if (myEngine) return myEngine;
 typedef void (*GETENGINE)( IEngine**, int argc, char *argv[] );
 enghLibrary = os_LoadObject(ModuleName);
 if (enghLibrary == NULL) { fprintf(stderr,"Can't load main Engine module! Exiting...\n"); return NULL;}
 GETENGINE FPlugLoad = (GETENGINE)os_LoadFunction( enghLibrary, "GetEngine" );
 if (!FPlugLoad) {fprintf(stderr,"I can't find GetEngine function in module :(");os_UnloadObject(enghLibrary);
 return NULL;}
 FPlugLoad( &myEngine, argc, argv );
 if (myEngine) fprintf(stdout,"Starter: AmberSkyNet Engine started...\n");
 return myEngine;
 };

void InitEngine(int argc=0, char *argv[]=NULL){
init_engine  = true;
if ( !LoadEngine(argc, argv) ) return;

myEngine->EngineInit();

for (int i=0;i<argc;i++) {
    printf("%d %s\n", i, argv[i]);
}

};

void FreeEngine(){
 if (myEngine) { myEngine->Release(); myEngine=NULL;}
 if (enghLibrary) {os_UnloadObject(enghLibrary);enghLibrary=NULL;}
 fprintf(stdout,"AmberSkyNet Engine stopped\n");
 };

private:
void *enghLibrary;
IEngine *myEngine;

 };

#define STARTER Starter::GetInstance()
#define ENGINE_INIT(X,Y) STARTER->InitEngine(X,Y)
#define ENGINE STARTER->GetEngine()

#define DEVICER ((IDevice*)ENGINE->GetPtrParam("DeviceManager"))

// теперь это делается через Engine->CreateObject(Name)
//#define PLUGGER ((IPluginFactory *)ENGINE->GetPtrParam("PluginManager"))

#define RESOURSER ((IResourceSystem *)ENGINE->GetPtrParam("ResourceSystem"))
