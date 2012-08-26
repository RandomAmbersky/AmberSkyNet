
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "os_deps.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

// ��������� �����
void os_findclose(os_dir_t dir){
if (!dir) return;
closedir((DIR*)dir);
};

// ���� ��������� ����
os_dir_t os_findnext(os_dir_t dir, os_finddata_t *data){
struct stat my_stat;
struct dirent *dptr;
if (!dir) return NULL;

dptr=readdir((DIR*)dir); //������ �� ����������
if (!dptr) return NULL; // ��� ��� ������
data->name=dptr->d_name; //��������� ��� � ���������, ���������� � ���������� �-���

std::string temp_name=data->dir_name+data->name; //��������� ������ ����
stat(temp_name.c_str(),&my_stat); //�������� ����������

mode_t t=my_stat.st_mode&S_IFMT;
if ( S_ISDIR(t) ) data->subdir=true; //���� ��� �������������, �� ������������� ����
else data->subdir=false;

return dir;
};

// ������ �����
os_dir_t os_findfirst(const char *dirname,os_finddata_t *data){
os_dir_t dir=opendir(dirname);
// ��������� ��� ����������, � ������� �������� �����
data->dir_name=dirname;
//First_Dir=dirname;
// ���� ���������� �������� ��������� - ����� �������� ��� �����
if (dir) return os_findnext(dir,data);
else return NULL; // ����� - ���������� NULL
};


int os_make_dir(const char* dirname) {

#ifndef _WIN32
    return mkdir( dirname, 0775);
#else
    return mkdir ( dirname );
#endif

};
