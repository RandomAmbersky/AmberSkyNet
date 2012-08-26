#ifdef _WIN32
#include <windows.h>
#endif
#include "IAsnGenerator.h"
#include "IEngine.h"

//#include "CFileSystem.h"
//#include "CFileSystemASN.h"

//#include "CFileSource.h"
#include "CFileSourceZIP.h"

class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet File System Addons";
//    GeneratorInfo.type="FileSystem,FileSystemASN,FSDisk,FSZIP";
    GeneratorInfo.type="FSZIP";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Some filesystem class";
    GeneratorInfo.plugin_version="0.1";
    GeneratorInfo.required_version="0.1";
      };

    ~CLoader() { };
//    void *GetClass(const char* Type){
//    if ( strcmp(Type,"FileSystem")==0) return new CFileSystem(Engine);
//    if ( strcmp(Type,"FileSystemASN")==0) return new CFileSystemASN(Engine);
//    if ( strcmp(Type,"FSDisk")==0) return new CFileSource(Engine);
//    if ( strcmp(Type,"FSZIP")==0) return new CFileSourceZIP(Engine);
//    else
//    return NULL;
//    };

    IBaseObject* GetObject(const char* Type){
    if ( strcmp(Type,"FSZIP")==0) return new CFileSourceZIP(Engine);
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

