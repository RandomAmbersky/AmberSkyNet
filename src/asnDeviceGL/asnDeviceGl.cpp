#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CDeviceGl.h"
#include "CCamera.h"
#include "CEventManagerSDL.h"
#include "CEvListCamWalker.h"
#include "CEvListCamLooker.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {
    GeneratorInfo.name="AmberSkyNet Device System";
    GeneratorInfo.type="DeviceGl,EventManagerSDL,EvListCamWalker,EvListCamLooker,CameraSimple";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Device OpenGl and SDL manager";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const char* Type){
//    if (Type=="LogTxt") return new CLog(Engine);
    if ( strcmp(Type, "DeviceGl")==0) return new CDeviceGl(Engine);
    if ( strcmp(Type, "CameraSimple")==0) return new CCamera(Engine);
    if ( strcmp(Type, "EventManagerSDL")==0) return new CEventManagerSDL(Engine);
    if ( strcmp(Type, "EvListCamWalker")==0) return new CEvListCamWalker(Engine);
    if ( strcmp(Type, "EvListCamLooker")==0) return new CEvListCamLooker(Engine);

    else return NULL;

    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type, "DeviceGl")==0) return new CDeviceGl(Engine);
    if ( strcmp(Type, "CameraSimple")==0) return new CCamera(Engine);
    if ( strcmp(Type, "EventManagerSDL")==0) return new CEventManagerSDL(Engine);
    if ( strcmp(Type, "EvListCamWalker")==0) return new CEvListCamWalker(Engine);
    if ( strcmp(Type, "EvListCamLooker")==0) return new CEvListCamLooker(Engine);

    return NULL;

    };

};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void GetPlugin( IAsnGenerator** ppOutput )
{
     *ppOutput = new CLoader();
}

