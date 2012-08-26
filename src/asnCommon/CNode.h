#ifndef _CNODE_H
#define _CNODE_H

//#include "IBaseObject.h"
#include "CVector.h"

#include "CBaseObject.h"
#include "INode.h"
#include "IEngine.h"

#include "matrix.h"

#include <vector>

/////// CNode - основной функционал узла сцены

class CNode: public INode, public CBaseObject {

public:
CNode(IEngine *_Engine);
~CNode();

void SetParam(const char* param_name,const char*param_value);
virtual bool AddChild(IBaseObject* child);

bool Draw();
char Update(float tms);
bool LoadResource(){return false;};

//UINT getID();//{return id;}
//void setID(UINT id);
//bool addNode(INode *node);
//bool delNode(INode *node);
//void getNodeList(asn_ext_ptr_list* list);

/*
virtual UINT GetChildNodeCount();
virtual INode* GetCurrentChild();
virtual void BeginChildNodeList();
virtual void EndChildNodeList();
virtual INode* NextChildNode();
*/

inline void ShowSize();

//INode *getParent();
//void setParent(INode *parent);

void SetPos(const CVector& Pos);
void SetRot(const CVector& Rot);
void SetSize(const CVector& Size);
void SetCenterPoint(const CVector& CenterPoint);

const CVector& GetPos();
const CVector& GetRot();
const CVector& GetSize();
const CVector& GetCenterPoint();

//NodeList_typ NodeList; //список всех нод

//protected:
//virtual void setParent(INode *node, INode *parent){node->Parent=parent;}; //"усыновить" ноду =)

//UINT id;
//INode *Parent;

//IEngine *Engine;

CVector CenterPoint;
CVector Size;
CVector out_Rot;
CVector out_Pos;

CVector4* NodeColor;

//std::vector<INode*> NodeList;
//std::vector<INode*>::iterator _node_it;

Matrix NodeMatrix;

bool load_all; //все ресурсы загружены


};

#endif
