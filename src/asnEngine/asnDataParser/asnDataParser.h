//#ifdef _WIN32
//#include <windows.h>
//#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CDataParserXML.h"

class CDataParserLoader: public IAsnGenerator {
public:
    CDataParserLoader() {

    GeneratorInfo.name="XML data parser System";
    GeneratorInfo.type="DataParserXML";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="XML data parser based on tinyXML";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CDataParserLoader() { };

    void *GetClass(const char* Type){
    if ( strcmp(Type,"DataParserXML")==0) return new CDataParserXML(Engine);
//    if (Type=="LogNo") return new CLogNo(Engine);
    else return NULL;

    };

    IBaseObject *GetObject(const char* Type){
//    if ( strcmp(Type,"DataParserXML")==0) return new CDataParserXML(Engine);
//    if (Type=="LogNo") return new CLogNo(Engine);
//    else 
    return NULL;

    };

};

//extern "C" 
//#ifdef _WIN32 
//__declspec(dllexport) 
//#endif
//void GetPlugin( IAsnGenerator** ppOutput )
//{
//     *ppOutput = new CLoader();
//};

