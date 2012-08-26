#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeSLTerrain.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet SecondLife-compatible Terrain";
    GeneratorInfo.type="NodeSLTerrain";
    GeneratorInfo.author="DarkHobbit";
    GeneratorInfo.description="AmberSkyNet SecondLife-compatible Terrain";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp( Type,"NodeSLTerrain")==0) return new CNodeSLTerrain(Engine);

    return NULL;
    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp( Type,"NodeSLTerrain")==0) return new CNodeSLTerrain(Engine);

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

