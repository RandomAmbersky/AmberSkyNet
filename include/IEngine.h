//[1546] ����������� ������ - ��� ����, ������������ ����������� ���������� ����� ��� �����.
//�. ������
#ifndef _IENGINE_H
#define _IENGINE_H
#include <string>
#include "IManager.h"

#include "ASN_Types.h"
#include "IWorld.h"
#include "IDataParser.h"

// ������������ ����� �������� ������
class IEngine {
public:

virtual void Release()=0; //���������� Engine

// ������� ���������/������ ���������� �� ��������� � ��.
virtual void SetPtrParam(const char* param_name, void *param_value)=0;
virtual void *GetPtrParam(const char* param_name)=0;
virtual void DelPtrParam(const char* param_name)=0;

// ������� ���������/������ ���������� ���������
virtual void SetEnvParam(const char* param_name, const char* param_value)=0;
virtual const char* GetEnvParam(const char* param_name)=0;
virtual void DelEnvParam(const char* param_name)=0;

virtual bool EngineInit()=0; // ������������� ����������
virtual bool EngineStart()=0; // ������ �������� ����� ���������� Engine

virtual bool AddCustomManager( IManager* CustomManager )=0;

// ��������� ������.. ������ ������ :) Engine �� ���� �������...
virtual bool LoadDataFile(const char *FileName, IBaseObject* parent=NULL)=0;
virtual bool SaveDataFile(const char *FileName, IBaseObject* parent=NULL)=0;

// �������� World
virtual IWorld* GetWorld()=0;
// ���������� World
//virtual bool SetWorld(UINT id)=0;

// ������� ��� �� ID
//virtual bool DelWorld(UINT id)=0;

// ������� World �� baseType ��� �� ����� ������� :)
virtual IWorld* CreateWorld(const char* WorldName)=0;

// ��������� "������" World
//virtual bool SetWorld(IWorld* customWorld)=0;

//��������� ��� ��������� World �� ID
//virtual bool LoadWorld(const char* FileName)=0;
//virtual bool SaveWorld(const char* FileName)=0;

// ������ ������ �� �����... ���� ������ ����� Base-������� ���, ������ ����� ������
virtual IBaseObject* CreateObject(const char *ObjName)=0;

// ������� ������� �� ������� (�������� - XML)
//virtual void* CreateObjectsByTemplate(const char *TemplateType, void *Template)=0;
//virtual void File2Data(const char *FileName)=0;

};

#define SetEnvParamStr(X,Y) SetEnvParam(X, std::string(Y).c_str() );

#endif
