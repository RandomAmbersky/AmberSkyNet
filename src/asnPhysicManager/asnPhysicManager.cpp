#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CPhysicManagerODE.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Physic Manager";
    GeneratorInfo.type="PhysicManagerODE";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="AmberSkyNet Physic Manager based on Bullet physic";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };

    IBaseObject*GetObject(const char* Type){
    if ( strcmp(Type,"PhysicManagerODE")==0) return new CPhysicManagerODE(Engine);

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

