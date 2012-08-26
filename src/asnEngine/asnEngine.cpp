#ifdef _WIN32
#include <windows.h>
#endif

#include "IAsnGenerator.h"
//#include "CDataParserXML.h"
#include "CEngine.h"

/*
class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="asnEngine";
    GeneratorInfo.type="DataParserXML";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="XML data parser";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
    void *GetClass(const std::string& Type){
    if (Type=="DataParserXML") return new CDataParserXML(Engine);
    else return NULL;

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
*/

// экспортим функцию создания класса Engine
extern "C"
#ifdef _WIN32
    __declspec(dllexport)
#endif
    void GetEngine( IEngine** ppOutput, int argc, char *argv[] )
{
    *ppOutput = new CEngine( argc, argv);
};

