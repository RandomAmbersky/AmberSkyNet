#ifndef OS_DEPS_H
#define OS_DEPS_H

#include <string>
#include <dirent.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

typedef void* os_dir_t;

struct os_finddata_t{
std::string dir_name;
std::string name;
bool subdir;
};

// закрываем поиск
extern void os_findclose(os_dir_t dir);

// ищем следующий файл
extern os_dir_t os_findnext(os_dir_t dir, os_finddata_t *data);

// начали поиск
extern os_dir_t os_findfirst(const char *dirname,os_finddata_t *data);

extern int os_make_dir(const char* dirname);

#endif
