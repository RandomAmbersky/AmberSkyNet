#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CNodeGUI_TextBox.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Simple GUI Manager";
    GeneratorInfo.type="NodeGUI_TextBox";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Simple 3d GUI elements";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";

      };

    ~CLoader() { };
    void *GetClass(const char* Type){
    if ( strcmp(Type,"NodeGUI_TextBox")==0) return new CNodeGUI_TextBox(Engine);
    return NULL;

    };

    IBaseObject* GetObject(const char* Type){

    if ( strcmp(Type,"NodeGUI_TextBox")==0) return new CNodeGUI_TextBox(Engine);

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

