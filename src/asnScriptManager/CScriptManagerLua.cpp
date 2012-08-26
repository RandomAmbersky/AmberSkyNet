#include "CScriptManagerLua.h"

//#define LUA_API static

#include "lua_user.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "IFileSystem.h"
#include "IFile.h"
#include "ILog.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#include <iostream>
using namespace std;

static const luaL_reg lualibs[] = {
  {"base", luaopen_base},
  {"table", luaopen_table},
  {"io", luaopen_io},
  {"string", luaopen_string},
  {"math", luaopen_math},
  {"debug", luaopen_debug},
  {"loadlib", luaopen_loadlib},
  {NULL, NULL}
};

static void l_openstdlibs (lua_State *l) {
  const luaL_reg *lib = lualibs;
  for (; lib->func; lib++) {
    lib->func(l);  /* open library */
    lua_settop(l, 0);  /* discard any results */
  }

//	lua_strlibopen(l);
};

CScriptManagerLua::CScriptManagerLua(IEngine* engine):CBaseObject(engine){
    SetParam("BaseClass","ScriptManager");
    SetParam("Type","ScriptManagerLua");
//    SetParam("Name","-");
    CurrentObject = NULL;

    cout << "Lua start.." << endl;
    l_state = lua_open();
    l_openstdlibs(l_state);  /* open libraries */

    std::string TestString = "a = 1+1;\nprint(a);\n";

    lua_dostring( l_state, "print('Test Message from LuaManager: Hello Lua World!')" );
    lua_dostring( l_state, TestString.c_str() );

    LOGGER->LogMsg("+CScriptManagerLua");

};

CScriptManagerLua::~CScriptManagerLua(){
    lua_close( l_state );
LOGGER->LogMsg("-CScriptManagerLua");
};


void CScriptManagerLua::InitManager(){

    ExecuteFile("script_test.lua",NULL);
    printf("CScriptManagerLua init...\n");
};

int CScriptManagerLua::Release(){
    delete this;
    printf("CScriptManagerLua released...\n");
    return 0;
};

void CScriptManagerLua::Update(float tms){

};

void* CScriptManagerLua::Execute(const char* buffer, void* CurrentObject_){

    void* Old;
    if (CurrentObject_) { Old=CurrentObject; CurrentObject=CurrentObject_; }

	lua_dostring( l_state, buffer );

    if (CurrentObject) { CurrentObject=Old; }

	return NULL;//CurrentObject;
};

void* CScriptManagerLua::ExecuteFile(const char* fileName, void* CurrentObject_){

    void* Old;

    if (CurrentObject_) { Old=CurrentObject; CurrentObject=CurrentObject_; }

    IFile *myFile=FILER->GetFile( fileName );
    if (!myFile) {
	    if (CurrentObject_) CurrentObject=Old;
	    return NULL; //файла нет такого
	    }

    myFile->FileOpen();
    void *data=myFile->LoadInMem();
    if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM) {
	    if (CurrentObject_) CurrentObject=Old;
	    return NULL; // не грузится он :(
	    }

    char* chr_data = (char*)data;
    chr_data[myFile->GetSizeInBytes()-1]='\0';
    lua_dostring( l_state, chr_data );

    if (CurrentObject_) CurrentObject=Old;

    return NULL;
};

void CScriptManagerLua::AddFunction (std::string func_name, CALLBACK_ASN_FUNCTION func){
    lua_register(l_state, func_name.c_str(), (lua_CFunction) func);
};

int CScriptManagerLua::Script_gettop(void *ptr){
    return lua_gettop((lua_State *)ptr);
};

const char *CScriptManagerLua::Script_tostring(void *ptr,int num){
    return lua_tostring((lua_State *)ptr,num);
};

double CScriptManagerLua::Script_tonumber(void *ptr,int num){
    return lua_tonumber((lua_State *)ptr,num);
};

void CScriptManagerLua::Script_pushstring(void *ptr,const char *s){
    return lua_pushstring((lua_State *)ptr,s);
};

void CScriptManagerLua::Script_pushnumber(void *ptr,double num){
    return lua_pushnumber((lua_State *)ptr,num);
};

int CScriptManagerLua::Script_error(void *ptr){
    return lua_error((lua_State *)ptr);
};

int CScriptManagerLua::Script_isstring(void *ptr,int num){
    return lua_isstring((lua_State *)ptr,num);
};

int CScriptManagerLua::Script_isnumber(void *ptr,int num){
    return lua_isnumber((lua_State *)ptr,num);
};
