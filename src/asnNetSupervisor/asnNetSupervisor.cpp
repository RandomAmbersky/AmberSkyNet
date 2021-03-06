#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNetSupervisor.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet NetManager";
    GeneratorInfo.type="NetSupervisorSDL"; //,IRC_DCC_CHAT_Client
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Simple net manager based on SDL_net library";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"NetSupervisorSDL")==0) return new CNetSupervisor(Engine);
//    if ( strcmp(Type,"IRC_Client")==0) return new NH_IRC(Engine);
//    if (Type=="IRC_DCC_CHAT_Client") return new NH_IRC_DCC_CHAT(Engine);

    return NULL;

    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"NetSupervisorSDL")==0) return new CNetSupervisor(Engine);

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
};

