#ifndef _CWORLD_H
#define _CWORLD_H

//#include "IWorld3D.h"

#include "ASN_Types_internal.h"

#include "ASN_SafeVector_template.h"

#include "CVector.h"

#include "INode.h"
#include "CBaseObject.h"
#include "IDataParser.h"
#include "IPhysicManager.h"

#include <vector>
#include <map>

typedef ASN_SafeVector< ICamera > safe_cam_list;

class CWorld: public virtual IWorld, public virtual CBaseObject {
public:
CWorld(IEngine *_Engine);
~CWorld();

virtual UINT AddCamera(const char* camType);
virtual UINT AddCamera(ICamera* cam);
virtual ICamera* GetCamera(UINT id);
virtual bool DelCamera(UINT id);

virtual bool Draw();
virtual bool Update(float tms);
//virtual void getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list);

//virtual INode *getNode( UINT id );
//virtual bool delNode( UINT id );
virtual bool AddNode( INode* node );

//virtual UINT getMaxID();
//virtual UINT getNodesNum();

// создаём элемент сцены
//virtual UINT createNode( const char *InitString );

//virtual INode *findNode( const char *NodeName );

//virtual bool LoadWorld( const char *FileName);
//virtual bool SaveWorld( const char *FileName);

// устанавливаем параметры BaseObject
//virtual bool SetBaseObject(const char *ObjName, const char *InitString);
//  рекурсивная функция поиска параметров у базового объекта
//  (который может ссылаться на другой базовый объект)
//virtual const char *GetBaseObjectParam(const char *ObjName, const char *ParamName);

virtual bool AddChild(IBaseObject* child);
virtual bool DelChild(UID childID);

//NodeList_typ NodeListInCut; //пригодится нам это для функции getNodesByPos

virtual ASNInterface* GetInterface(const char* interface_name);
//virtual void* GetInterface(const char* interface_name);

protected:
/*
void loadBaseObjects(IDataParser* parser, DataLeaf parent); // загружаем из парсера базовые объекты
void loadWorld(IDataParser* parser, DataLeaf parent); // загружаем из парсера "мир" объекты

INode *Leaf2Data(IDataParser* parser, DataLeaf parent); //сделаем ноду
void Node2Leaf(IDataParser* parser, DataLeaf parent, INode *node);
*/

//bool IDisValid( UINT id );
//std::map<std::string, INode*> NodeNameList; //список именованых нод
//std::vector<INode*> NodeList; //храним все ID ноды
INode *root_node; // корневая нода к которой всё цепляем

safe_cam_list CameraList;

//asn_List BaseObjectList;

//IEngine *Engine;
//IPhysicManager* WorldPhysicManager;
};

#endif
