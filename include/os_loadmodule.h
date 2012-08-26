#ifndef OS_LOADMODULE_H
#define OS_LOADMODULE_H

//////////// Cross-Platform Module Loader ///////////
//// � ���� ������ ���� �� ���������, ����� ����� ������ SDL ������ ��� CLe266 :(((

extern void *os_LoadObject(const char *ModuleName);

extern void *os_LoadFunction(void *handle, const char *name);

extern void os_UnloadObject(void *handle);

#endif
