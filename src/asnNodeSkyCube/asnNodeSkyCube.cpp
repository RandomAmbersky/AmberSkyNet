#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeSkyCube.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Sky Cube Node ";
    GeneratorInfo.type="NodeSkyCube";
    GeneratorInfo.author="Meridian";
    GeneratorInfo.description="Sky Cube Node class";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"NodeSkyCube")==0) return new CNodeSkyCube(Engine);

    return NULL;
    };

    IBaseObject* GetObject(const char* Type){
    if ( strcmp(Type,"NodeSkyCube")==0) return new CNodeSkyCube(Engine);

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
