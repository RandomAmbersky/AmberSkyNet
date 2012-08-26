//[1546] ����������� ������ - ��� ����, ������������ ����������� ���������� ����� ��� �����.
//�. ������
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

// �������� ����� ���������
class CEngine: public IEngine {
public:
CEngine(int argc=0, char *argv[]=NULL);
~CEngine();

void Release(); //���������� Engine

// ������� ���������/������ ���������� �� ��������� � ��.
void SetPtrParam(const char* param_name, void *param_value);
void *GetPtrParam(const char* param_name);
void DelPtrParam(const char* param_name);

// ������� ���������/������ ���������� ���������
void SetEnvParam(const char* param_name,const char* param_value);
const char* GetEnvParam(const char* param_name);
void DelEnvParam(const char* param_name);

bool EngineInit(); // ������������� ����������
bool EngineStart(); // ������ �������� ����� ���������� Engine

// ��������� ������.. ������ ������ :) Engine �� ���� �������...
bool LoadDataFile(const char *FileName, IBaseObject* base_root);
bool SaveDataFile(const char *FileName, IBaseObject* base_root);

// �� _������_ ����� ����������� ������, ���� <file ..> ������������
void File2Data(const char *FileName, IBaseObject* base_root);


void LoadData(IDataParser* parser, DataLeaf leaf, IBaseObject* prev_ptr );

bool ObjectAddObject( IBaseObject* parent, IBaseObject* child);

void LoadBaseObject(IDataParser* parser, DataLeaf leaf);

// ������ ������ �� �����
virtual IBaseObject* CreateObject(const char *ObjName);

virtual IBaseObject* CreateBaseObject(const char* ObjName);

virtual IBaseObject* Leaf2Object(IDataParser* parser, DataLeaf leaf);
virtual IBaseObject* Leaf2Params(IDataParser* parser, DataLeaf leaf, IBaseObject* object );

// ������� ������� �� ������� (�������� - XML)
//virtual void* CreateObjectsByTemplate(const char *TemplateType, void *Template);

virtual bool AddCustomManager( IManager* CustomManager );

// �������� World �� ID
IWorld* GetWorld();
// ���������� World ��� ��������� �� ID
//bool SetWorld(UINT id);
// ������� ��� �� ID
//bool DelWorld(UINT id);
// ������� World �� baseType ��� �� ����� ������� :)
IWorld* CreateWorld(const char* WorldName);
// ��������� "������" World
//bool SetWorld(IWorld* customWorld);
//��������� ��� ��������� World �� ID
//bool LoadWorld(const char* FileName/*, UINT& id*/);
//bool SaveWorld(const char* FileName/*, UINT id*/);

void EngineUpdate();
void EngineSafeUpdate();
void EngineDraw();

private:

std::map<std::string,std::string> BaseObjects;

int engine_state;
// 0 - created, 1 - init

//���������� ���������� ���������. ��������, � ������� ����� ������� �� ���������
//�����-���������� ��������� ��������, ������������ � � ��������� �����
std::map<std::string,std::string> Content;
std::map<std::string,void *> VoidContent;
//std::map<std::string,std::string>::iterator it;

void CheckBase(const char* debug);

//safe_world_list WorldList;
safe_manager_list ManagerList;
//uint numCurrentWorld; // ����� �������� ���� ��� ���������

IWorld *mainWorld; // ��� ��������, ��.. ����� ����������� ������ �����

//IWorld *myWorld; // ��� ��������, ��.. ����� ����������� ������ �����
//    SDL_Thread* coreThread;

};

#endif
