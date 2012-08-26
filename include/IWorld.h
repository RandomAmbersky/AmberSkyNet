#ifndef _IWORLD_H
#define _IWORLD_H

#include "ASN_Params.h"
#include "IBaseObject.h"
#include "CVector.h"

#include "INode.h"
#include "ICamera.h"

//// ������������ ����� ���� - ������� ������� ������

class IWorld: public virtual IBaseObject {
public:
//IWorld(IEngine *_Engine):BaseObject(_Engine){};
virtual ~IWorld(){ printf("IWorld::~IWorld()"); }

virtual UINT AddCamera(const char* camType)=0;
virtual UINT AddCamera(ICamera* cam)=0;
virtual ICamera* GetCamera(UINT id)=0;
virtual bool DelCamera(UINT id)=0;

virtual bool Draw()=0;
virtual bool Update(float tms)=0;
//virtual void getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)=0;// �������� ������ ���,
// ������� "��������" ��� ������� � ������������ Start, End

//virtual INode *getNode( UINT id )=0; //�������� ���� �������� �� ����������� ��� ���� ID
//virtual bool delNode( UINT id )=0;  //������� ���� �� ID
//virtual UINT addNode( INode* node)=0; //��������� ������� ����, �� ������ - � ID � ����

//virtual UINT getMaxID()=0; //�������� ������������ ID. ����� ���������� ��� �������� ��� � ����� �� ID.
//virtual UINT getNodesNum()=0; //�������� ����� ����� ��� � ����

// ������ ������� �����
//virtual UINT createNode( const char *InitString )=0; // �� ������ - ID ���� �����
//virtual INode *findNode( const char *NodeName )=0;  //�������� �������� ���� � ������ Name

// ��������� ��� ��������� ���
//virtual bool LoadWorld(const char *FileName)=0;
//virtual bool SaveWorld(const char *FileName)=0;

//������ ��������� �������� �������, ����� ��� ������ createNode �� ����� �� ������������
//"�������" ����� �������� � ������� createNode
//virtual bool SetBaseObject(const char *ObjName, const char *InitString)=0;

//  ����������� ������� ������ ���������� � �������� �������
//  (������� ����� ��������� �� ������ ������� ������)
//virtual const char *GetBaseObjectParam(const char *ObjName, const char *ParamName)=0;

//virtual void* GetInterface(const char* interface_name)=0;

//protected:
    //virtual void SetNodeID(INode *node, UINT id){node->id=id;}
};

#endif
