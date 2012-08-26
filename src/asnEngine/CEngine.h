//[1546] Театральный критик - это лицо, разъясняющее изумленному драматургу смысл его пьесы.
//У. Мизнер
#ifndef _CENGINE_H
#define _CENGINE_H

#include "IEngine.h"
#include "IManager.h"
#include "ASN_SafeVector_template.h"
#include <map>

//#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

typedef ASN_SafeVector< IWorld > safe_world_list;
typedef ASN_SafeVector< IManager > safe_manager_list;

// основной класс программы
class CEngine: public IEngine {
public:
CEngine(int argc=0, char *argv[]=NULL);
~CEngine();

void Release(); //уничтожаем Engine

// функции установки/чтения указателей на менеджеры и пр.
void SetPtrParam(const char* param_name, void *param_value);
void *GetPtrParam(const char* param_name);
void DelPtrParam(const char* param_name);

// функции установки/чтения переменных окружения
void SetEnvParam(const char* param_name,const char* param_value);
const char* GetEnvParam(const char* param_name);
void DelEnvParam(const char* param_name);

bool EngineInit(); // инициализация менеджеров
bool EngineStart(); // запуск игрового цикла средствами Engine

// загружаем данные.. просто данные :) Engine их сама разберёт...
bool LoadDataFile(const char *FileName, IBaseObject* base_root);
bool SaveDataFile(const char *FileName, IBaseObject* base_root);

// ЙЪ _ПДОПЗП_ ЖБКМБ ЪБЗТХЦБАФУС ДБООЩЕ, ХЪМЩ <file ..> РТПРХУЛБАФУС
void File2Data(const char *FileName, IBaseObject* base_root);


void LoadData(IDataParser* parser, DataLeaf leaf, IBaseObject* prev_ptr );

bool ObjectAddObject( IBaseObject* parent, IBaseObject* child);

void LoadBaseObject(IDataParser* parser, DataLeaf leaf);

// создаём объект по имени
virtual IBaseObject* CreateObject(const char *ObjName);

virtual IBaseObject* CreateBaseObject(const char* ObjName);

virtual IBaseObject* Leaf2Object(IDataParser* parser, DataLeaf leaf);
virtual IBaseObject* Leaf2Params(IDataParser* parser, DataLeaf leaf, IBaseObject* object );

// создать объекты по шаблону (например - XML)
//virtual void* CreateObjectsByTemplate(const char *TemplateType, void *Template);

virtual bool AddCustomManager( IManager* CustomManager );

// получить World по ID
IWorld* GetWorld();
// установить World для отрисовки по ID
//bool SetWorld(UINT id);
// удаляем мир по ID
//bool DelWorld(UINT id);
// создать World по baseType или по имени плагина :)
IWorld* CreateWorld(const char* WorldName);
// добавляем "ручной" World
//bool SetWorld(IWorld* customWorld);
//загрузить или сохранить World по ID
//bool LoadWorld(const char* FileName/*, UINT& id*/);
//bool SaveWorld(const char* FileName/*, UINT id*/);

void EngineUpdate();
void EngineSafeUpdate();
void EngineDraw();

private:

std::map<std::string,std::string> BaseObjects;

int engine_state;
// 0 - created, 1 - init

//содержимое переменных окружения. Возможно, в будущем будет заменен на отдельный
//класс-обработчик изменений значений, используемый и в элементах сцены
std::map<std::string,std::string> Content;
std::map<std::string,void *> VoidContent;
//std::map<std::string,std::string>::iterator it;

void CheckBase(const char* debug);

//safe_world_list WorldList;
safe_manager_list ManagerList;
//uint numCurrentWorld; // номер текущего мира для отрисовки

IWorld *mainWorld; // это временно, да.. потом полноценный список будет

//IWorld *myWorld; // это временно, да.. потом полноценный список будет
//    SDL_Thread* coreThread;

};

#endif
