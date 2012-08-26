#ifndef _C_PLUGINFACTORY_H
#define _C_PLUGINFACTORY_H

#include "IAsnGenerator.h"
//#include "IPluginFactory.h"
#include "IEngine.h"

#include <map>

class CPluginFactory {
public:
CPluginFactory(IEngine *Engine);
~CPluginFactory();

// сканируем директорию с плагинами классов-генераторов и загружаем их
void ScanDirectory(std::string DirName, bool Recursive);

// загрузить модуль класса-генератора IAsnGenerator, true - ошибка при загрузке
bool LoadASNModule(const std::string& ModuleName);

// выгрузить модуль плагина
//bool UnloadModule(const std::string& ModuleName);
bool LoadGenerator(IAsnGenerator*  myGenerator);

// создать класс с типом Type на основе всех известных классов-генераторов
//void *CreateType(const std::string& Type);

IBaseObject* CreateObject(const char* Type);

private:

UID next_uid;

IEngine *Engine;
std::map<std::string,void*> LibraryList;
std::map<std::string,IAsnGenerator*> GeneratorList;

};

#endif
