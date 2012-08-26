#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNode3ds.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet SceneNode based on 3ds format";
    GeneratorInfo.type="Node3ds,NodeBase";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Some 3ds static mesh Node class";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };
      
    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"Node3ds")==0) return new CNode3ds(Engine);
    if ( strcmp(Type,"NodeBase")==0) return new CNode(Engine);
    else return NULL;
   
    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"Node3ds")==0) return new CNode3ds(Engine);
    if ( strcmp(Type,"NodeBase")==0) return new CNode(Engine);

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

