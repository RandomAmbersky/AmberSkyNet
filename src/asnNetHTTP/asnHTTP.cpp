#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CFileSourceHTTP.h"
#include "HTTP_Client.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet HTTP modules";
    GeneratorInfo.type="FSHTTP,HTTP_Client";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Some HTTP realization";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
//    void *GetClass(const char* Type){
//    if ( strcmp(Type,"FSHTTP")==0) return new CFileSourceHTTP(Engine);
//    if ( strcmp(Type,"HTTP_Client")==0) return new HTTP_Client(Engine);
//    else return NULL;
//    };

    IBaseObject* GetObject(const char* Type){
    if ( strcmp(Type,"FSHTTP")==0) return new CFileSourceHTTP(Engine);
    if ( strcmp(Type,"HTTP_Client")==0) return new HTTP_Client(Engine);
    else return NULL;

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

