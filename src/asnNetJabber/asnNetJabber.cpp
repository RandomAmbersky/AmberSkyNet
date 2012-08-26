#ifdef _WIN32
#include <windows.h>
#endif

#include "IAsnGenerator.h"
#include "ILog.h"
#include "IEngine.h"
#include "nh_jabber.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet NetJabber";
    GeneratorInfo.type="Jabber_Client"; //,IRC_DCC_CHAT_Client
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Simple XMPP";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };

    void *GetClass(const char* Type){

	LOGGER->LogMsg("try get type...");
//	LOGGER->LogMsg("HTTP_Client: Open");
    if ( strcmp(Type,"Jabber_Client")==0) return new NH_Jabber(Engine);

    return NULL;

    };


    IBaseObject* GetObject(const char* Type){

	LOGGER->LogMsg("try get type...");
    if ( strcmp(Type,"Jabber_Client")==0) return new NH_Jabber(Engine);

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

