
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "os_deps.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

// закрываем поиск
void os_findclose(os_dir_t dir){
if (!dir) return;
closedir((DIR*)dir);
};

// ищем следующий файл
os_dir_t os_findnext(os_dir_t dir, os_finddata_t *data){
struct stat my_stat;
struct dirent *dptr;
if (!dir) return NULL;

dptr=readdir((DIR*)dir); //читаем из директория
if (!dptr) return NULL; // нет там ничего
data->name=dptr->d_name; //сохраняем имя в структуре, полученной в параметрах ф-ции

std::string temp_name=data->dir_name+data->name; //формируем полный путь
stat(temp_name.c_str(),&my_stat); //собираем статистику

mode_t t=my_stat.st_mode&S_IFMT;
if ( S_ISDIR(t) ) data->subdir=true; //если это поддиректорий, то устанавливаем флаг
else data->subdir=false;

return dir;
};

// начали поиск
os_dir_t os_findfirst(const char *dirname,os_finddata_t *data){
os_dir_t dir=opendir(dirname);
// сохраняем имя директория, в котором проводим поиск
data->dir_name=dirname;
//First_Dir=dirname;
// если директорий открылся нормально - сразу начинаем там поиск
if (dir) return os_findnext(dir,data);
else return NULL; // иначе - возвращаем NULL
};


int os_make_dir(const char* dirname) {

#ifndef _WIN32
    return mkdir( dirname, 0775);
#else
    return mkdir ( dirname );
#endif

};
