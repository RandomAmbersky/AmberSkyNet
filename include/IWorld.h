#ifndef _IWORLD_H
#define _IWORLD_H

#include "ASN_Params.h"
#include "IBaseObject.h"
#include "CVector.h"

#include "INode.h"
#include "ICamera.h"

//// Интерфейсный класс мира - уровень игровой логики

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
//virtual void getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)=0;// получаем список нод,
// которые "проткнул" наш отрезок с координатами Start, End

//virtual INode *getNode( UINT id )=0; //получаем ноду напрямую по уникальному для мира ID
//virtual bool delNode( UINT id )=0;  //удаляем ноду по ID
//virtual UINT addNode( INode* node)=0; //добавляем внешнюю ноду, на выходе - её ID в мире

//virtual UINT getMaxID()=0; //получаем максимальный ID. Может пригодится для перебора нод в цикле по ID.
//virtual UINT getNodesNum()=0; //получаем общее число нод в мире

// создаём элемент сцены
//virtual UINT createNode( const char *InitString )=0; // на выходе - ID узла сцены
//virtual INode *findNode( const char *NodeName )=0;  //получить дочернюю ноду с именем Name

// загрузить или сохранить мир
//virtual bool LoadWorld(const char *FileName)=0;
//virtual bool SaveWorld(const char *FileName)=0;

//создаём параметры базового объекта, чтобы при вызове createNode мы могли бы использовать
//"базовые" имена объектов в функции createNode
//virtual bool SetBaseObject(const char *ObjName, const char *InitString)=0;

//  рекурсивная функция поиска параметров у базового объекта
//  (который может ссылаться на другой базовый объект)
//virtual const char *GetBaseObjectParam(const char *ObjName, const char *ParamName)=0;

//virtual void* GetInterface(const char* interface_name)=0;

//protected:
    //virtual void SetNodeID(INode *node, UINT id){node->id=id;}
};

#endif
