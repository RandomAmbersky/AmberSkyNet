#ifndef _INODE_H
#define _INODE_H

#include "ASN_Params.h"

#include "IBaseObject.h"
#include "CVector.h"

#include <map>
#include <vector>

/////// Node - ������� �����

class INode: public virtual IBaseObject {

public:

virtual bool Draw()=0;
virtual char Update(float tms)=0;// 0 - ���������, 1- �� ������� ��������, 2 - ���� ������� :)
virtual bool LoadResource()=0; //false - ���� ��� ������� ��� ���� ������� ���������

//virtual UINT getID()=0; // ����������� ����������� ID ����
//virtual void setID(UINT id)=0; // ���������� ����������(� �������� ������ ����) ID ����
//virtual bool addNode(INode* node)=0; // ������������ �������� ����
//virtual bool delNode(INode* node)=0;  // ����������� �������� ����, �� ������� ����, � �����������!
//virtual void getNodeList(asn_ext_ptr_list* list)=0; //�������� ������ �������� ��

//virtual UINT GetChildNodeCount()=0;
//virtual INode* GetCurrentChild()=0;
//virtual void BeginChildNodeList()=0;
//virtual void EndChildNodeList()=0;
//virtual INode* NextChildNode()=0;

//virtual INode *getParent()=0;//{ return Parent; };    //�������� ������������ ����
//virtual void setParent(INode *parent)=0;// ���������� ������������ ����. ����� �� ������������

// ������� ��������� � ������ �������� ������ ����� ���������� � public-������
// �.�. ������ ���������� ������ INode ��� ����� �������� ������ �� � ���� CVector
// (� �������� � ���� ������� ��������������)
virtual void SetPos(const CVector& Pos)=0;  // �������
virtual void SetRot(const CVector& Rot)=0;  // ���� ��������
virtual void SetSize(const CVector& Size)=0; //������� ��������
virtual void SetCenterPoint(const CVector& CenterPoint)=0; //����� ������ (��������, ���������, ���)

virtual const CVector& GetPos()=0;  // �������
virtual const CVector& GetRot()=0;  // ���� ��������
virtual const CVector& GetSize()=0; //������� �������� (����� ������ ���������������, ������� ���������� ������)
virtual const CVector& GetCenterPoint()=0; //����� ������ (��������, ���������, ���)

//protected:
//virtual void setParent(INode *node, INode *parent){node->Parent=parent;}; //"���������" ���� =)
//UINT id;
//INode *Parent;
//friend class IWorld;
};

#endif
