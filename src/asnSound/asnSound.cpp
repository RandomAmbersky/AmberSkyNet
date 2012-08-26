#ifdef _WIN32
#include <windows.h>
#endif

#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CSoundSDL_mixer.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Sound System";
    GeneratorInfo.type="SoundSDL_mixer";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Some sound class based on SDL_mixer library";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
//    void *GetClass(const char* Type){
//    if ( strcmp(Type,"SoundSDL_mixer")==0) return new CSoundSDL_mixer(Engine);
//    else return NULL;
//    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"SoundSDL_mixer")==0) return new CSoundSDL_mixer(Engine);

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

