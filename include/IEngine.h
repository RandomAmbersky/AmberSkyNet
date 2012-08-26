//[1546] Театральный критик - это лицо, разъясняющее изумленному драматургу смысл его пьесы.
//У. Мизнер
#ifndef _IENGINE_H
#define _IENGINE_H
#include <string>
#include "IManager.h"

#include "ASN_Types.h"
#include "IWorld.h"
#include "IDataParser.h"

// интерфейсный класс главного класса
class IEngine {
public:

virtual void Release()=0; //уничтожаем Engine

// функции установки/чтения указателей на менеджеры и пр.
virtual void SetPtrParam(const char* param_name, void *param_value)=0;
virtual void *GetPtrParam(const char* param_name)=0;
virtual void DelPtrParam(const char* param_name)=0;

// функции установки/чтения переменных окружения
virtual void SetEnvParam(const char* param_name, const char* param_value)=0;
virtual const char* GetEnvParam(const char* param_name)=0;
virtual void DelEnvParam(const char* param_name)=0;

virtual bool EngineInit()=0; // инициализация менеджеров
virtual bool EngineStart()=0; // запуск игрового цикла средствами Engine

virtual bool AddCustomManager( IManager* CustomManager )=0;

// загружаем данные.. просто данные :) Engine их сама разберёт...
virtual bool LoadDataFile(const char *FileName, IBaseObject* parent=NULL)=0;
virtual bool SaveDataFile(const char *FileName, IBaseObject* parent=NULL)=0;

// получить World
virtual IWorld* GetWorld()=0;
// установить World
//virtual bool SetWorld(UINT id)=0;

// удаляем мир по ID
//virtual bool DelWorld(UINT id)=0;

// создать World по baseType или по имени плагина :)
virtual IWorld* CreateWorld(const char* WorldName)=0;

// добавляем "ручной" World
//virtual bool SetWorld(IWorld* customWorld)=0;

//загрузить или сохранить World по ID
//virtual bool LoadWorld(const char* FileName)=0;
//virtual bool SaveWorld(const char* FileName)=0;

// создаём объект по имени... если такого среди Base-записей нет, создаём через плагин
virtual IBaseObject* CreateObject(const char *ObjName)=0;

// создать объекты по шаблону (например - XML)
//virtual void* CreateObjectsByTemplate(const char *TemplateType, void *Template)=0;
//virtual void File2Data(const char *FileName)=0;

};

#define SetEnvParamStr(X,Y) SetEnvParam(X, std::string(Y).c_str() );

#endif
