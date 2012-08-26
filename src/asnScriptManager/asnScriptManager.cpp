#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CScriptManagerLua.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Script Manager";
    GeneratorInfo.type="ScriptManagerLua";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="AmberSkyNet Script Manager based on Lua Lang";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
//    void *GetClass(const char* Type){
//    if ( strcmp(Type,"ScriptManagerLua")==0 ) return new CScriptManagerLua(Engine);
//    return NULL;
//    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"ScriptManagerLua")==0 ) return new CScriptManagerLua(Engine);

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

