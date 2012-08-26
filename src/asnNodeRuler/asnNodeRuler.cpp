#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeRuler.h"



class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Ruler";
    GeneratorInfo.type="NodeRuler";
    GeneratorInfo.author="DarkHobbit";
    GeneratorInfo.description="AmberSkyNet Word Developer Ruler";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      
    };
      
    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"NodeRuler")==0 ) return new CNodeRuler(Engine);

    return NULL;
    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"NodeRuler")==0 ) return new CNodeRuler(Engine);

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

