#ifndef _CSCRIPT_MANAGERLUA_H
#define _CSCRIPT_MANAGERLUA_H

#include "IScriptManager.h"
#include "CBaseObject.h"

#include "lua_user.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <string>

class CScriptManagerLua: public IScriptManager, public CBaseObject{
public:

    void InitManager();
    int Release();
    void Update(float tms);

    CScriptManagerLua(IEngine* engine);
    ~CScriptManagerLua();

    void* Execute(const char* buffer, void* CurrentObject);
    void* ExecuteFile(const char* buffer, void* CurrentObject);

    void SetCurrentObject(void* CurrentObject_){ CurrentObject=CurrentObject_; }
    virtual void* GetCurrentObject(){ return CurrentObject; }

	void AddFunction (std::string func_name, CALLBACK_ASN_FUNCTION func);

        int Script_gettop(void *ptr);
	const char *Script_tostring(void *ptr,int num);
	double Script_tonumber(void *ptr,int num);
   	void Script_pushstring(void *ptr,const char *s);
	void Script_pushnumber(void *ptr,double num);
   	int Script_error(void *ptr);
   	int Script_isstring(void *ptr,int num);
	int Script_isnumber(void *ptr,int num);


    void* CurrentObject;
	lua_State *l_state;
};

#endif
