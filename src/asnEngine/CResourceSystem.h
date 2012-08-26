#ifndef _CRESOURCE_SYSTEM_H
#define _CRESOURCE_SYSTEM_H

#include "IEngine.h"

#include "IBaseObject.h"
#include "IResourceSystem.h"

#include <map>

struct internalRes{
BYTE *data;
UINT size;
UINT count;
};

//struct internalObj{
//IBaseObject *obj;
//UINT count;
//};

typedef std::map<std::string,internalRes>::iterator ItRes;

class CResourceSystem: public IResourceSystem {
public:
CResourceSystem();
~CResourceSystem();

bool SetResource(const char* Type, const char* Name,
                BYTE* Resource, UINT Res_size );
BYTE *GetResource(const char* Type, const char* Name);
bool ReleaseResource(const char* Type, const char* Name);

bool SetObject(const char* Type, const char* Name, IBaseObject *resObject);
IBaseObject *GetObject(const char* Type, const char* Name);
bool ReleaseObject(const char* Type, const char* Name);

ITextListMap* GetTextListMap(){ return TextListMap; };

IEngine *Engine;

protected:

ITextListMap* TextListMap;

bool DropResource(const char* Type, const char* Name);

std::map<std::string,internalRes> SourceList;
std::map<std::string,internalRes>::iterator itRes;

std::map<std::string,IBaseObject*> ObjList;
std::map<std::string,IBaseObject*>::iterator itObj;

};

#endif
