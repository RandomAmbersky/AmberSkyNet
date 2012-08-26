
#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include <stdio.h>

//////////// Cross-Platform Module Loader ///////////

void *os_LoadObject(const char *ModuleName){
//std::string myModuleName=std::string(ModuleName)+"."+ASN_MODULE_EXTENSION_NAME;
//printf("\nplug load %s\n",myModuleName.c_str());
#ifndef _WIN32
void *library_handler = dlopen(ModuleName,RTLD_NOW);
if (!library_handler){
	fprintf(stderr,"dlopen() error: %s\n", dlerror());
	return NULL;
	};
#else
void *library_handler=LoadLibrary(ModuleName);
#endif
return library_handler;
};

void *os_LoadFunction(void *handl, const char *name){
    if (handl) {
#ifndef _WIN32
    void* func_p=dlsym(handl, name);
#else
    void* func_p=(void*)GetProcAddress( (HINSTANCE)handl, name );
#endif
    return func_p;
    }
return NULL;
};

void os_UnloadObject(void *handle){
if (!handle) return;
#ifndef _WIN32
dlclose(handle);
#else
FreeLibrary( (HINSTANCE)handle);
#endif
};
