#ifndef _ISCRIPT_MANAGER_H
#define _ISCRIPT_MANAGER_H

#include "ASN_Params.h"

#include "IManager.h"

class IScriptManager: public IManager {
public:

//    virtual void* Execute(const char* buffer, IBaseObject* CurrentObject_)=0;
//    virtual void* ExecuteFile(const char* buffer, IBaseObject* CurrentObject_)=0;

//    virtual void SetCurrentObject(IBaseObject* CurrentObject_)=0;
//    virtual IBaseObject* GetCurrentObject()=0;

    virtual void* Execute(const char* buffer, void* CurrentObject_)=0;
    virtual void* ExecuteFile(const char* buffer, void* CurrentObject_)=0;

    virtual void SetCurrentObject(void* CurrentObject_)=0;
    virtual void* GetCurrentObject()=0;

    virtual void AddFunction (std::string func_name, CALLBACK_ASN_FUNCTION func)=0;

    virtual int Script_gettop(void *ptr)=0;
    virtual const char *Script_tostring(void *ptr,int num)=0;
	virtual double Script_tonumber(void *ptr,int num)=0;
   	virtual void Script_pushstring(void *ptr,const char *s)=0;
	virtual void Script_pushnumber(void *ptr,double num)=0;
   	virtual int Script_error(void *ptr)=0;
   	virtual int Script_isstring(void *ptr,int num)=0;
	virtual int Script_isnumber(void *ptr,int num)=0;

};

#define SCRIPTER ((IScriptManager*)ENGINE->GetPtrParam("ScriptManager"))

#endif
