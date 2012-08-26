//#ifdef _WIN32
//#include <windows.h>
//#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

#include "CLog.h"

class CLogLoader: public IAsnGenerator {
public:
    CLogLoader() {

    GeneratorInfo.name="AmberSkyNet Logs System";
    GeneratorInfo.type="LogTxt,LogNo";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Some logs class";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLogLoader() { };

    virtual void *GetClass(const char* Type){
    if ( strcmp(Type,"LogTxt")==0) return new CLog(Engine);
    if ( strcmp(Type,"LogNo")==0) return new CLogNo(Engine);
    else return NULL;

    };

    IBaseObject* GetObject(const char* Type){
	if ( strcmp(Type,"LogTxt")==0) return new CLog(Engine);
	if ( strcmp(Type,"LogNo")==0) return new CLogNo(Engine);
    else return NULL;

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

