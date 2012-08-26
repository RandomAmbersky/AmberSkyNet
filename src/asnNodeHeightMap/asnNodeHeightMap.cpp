#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeHeightMap.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet HeightMap Terrain";
    GeneratorInfo.type="NodeHeightMap";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="AmberSkyNet Terrain builded by heightmap texture";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"NodeHeightMap")==0) return new CNodeHeightMap(Engine);

    return NULL;
    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"NodeHeightMap")==0) return new CNodeHeightMap(Engine);

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

