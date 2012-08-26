#ifdef _WIN32
#include <windows.h>
#endif

#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CWorldSimple3D.h"
#include "CModifierPack.h"
#include "CModifiers.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet WorldManager";
    GeneratorInfo.type="World,WorldSimple3D,ModifierPack,NodeModifierRotate";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Simple world manager's";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";

//    void* myPtr = new CWorld(NULL);

      };

    ~CLoader() { };
//    virtual void Initialize(IEngine *vEngine) {
//	cout << "init engine " << endl;
//	void* myPtr = new CWorld(Engine);
//    }
    virtual void *GetClass(const char* Type){
//    cout << "try create " << Type << endl;
    if ( strcmp(Type,"World")==0) return new CWorld(Engine);
    if ( strcmp(Type,"WorldSimple3D")==0) return new CWorldSimple3D(Engine);
    if ( strcmp(Type,"ModifierPack")==0) return new CModifierPack(Engine);
    if ( strcmp(Type,"NodeModifierRotate")==0) return new CModifRotate(Engine);

    return NULL;

    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"World")==0) return new CWorld(Engine);
    if ( strcmp(Type,"WorldSimple3D")==0) return new CWorldSimple3D(Engine);
    if ( strcmp(Type,"ModifierPack")==0) return new CModifierPack(Engine);
    if ( strcmp(Type,"NodeModifierRotate")==0) return new CModifRotate(Engine);

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

