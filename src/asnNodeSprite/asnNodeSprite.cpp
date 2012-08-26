#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeSprite.h"
#include "CNodeGrid2D.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Sprite SceneNode ";
    GeneratorInfo.type="NodeSprite,NodeGrid2D";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Sprite Node class, 2d grid";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
//    void *GetClass(const char* Type){
//    if ( strcmp(Type,"NodeSprite")==0) return new CNodeSprite(Engine);
//    if ( strcmp(Type,"NodeGrid2D")==0) return new CNodeGrid2D(Engine);
//
//    return NULL;
//    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"NodeSprite")==0) return new CNodeSprite(Engine);
    if ( strcmp(Type,"NodeGrid2D")==0) return new CNodeGrid2D(Engine);

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

