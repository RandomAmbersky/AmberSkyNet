#ifndef _INODE_H
#define _INODE_H

#include "ASN_Params.h"

#include "IBaseObject.h"
#include "CVector.h"

#include <map>
#include <vector>

/////// Node - элемент сцены

class INode: public virtual IBaseObject {

public:

virtual bool Draw()=0;
virtual char Update(float tms)=0;// 0 - нормально, 1- не удалось обновить, 2 - нода удалена :)
virtual bool LoadResource()=0; //false - если все ресурсы для ноды удалось загрузить

//virtual UINT getID()=0; // возвращение уникального ID ноды
//virtual void setID(UINT id)=0; // установить уникальный(в пределах одного мира) ID ноды
//virtual bool addNode(INode* node)=0; // присоединить дочерний узел
//virtual bool delNode(INode* node)=0;  // отсоединить дочерний узел, НЕ УДАЛЯЕМ НОДУ, А ОТСОЕДИНЯЕМ!
//virtual void getNodeList(asn_ext_ptr_list* list)=0; //получить список дочерних но

//virtual UINT GetChildNodeCount()=0;
//virtual INode* GetCurrentChild()=0;
//virtual void BeginChildNodeList()=0;
//virtual void EndChildNodeList()=0;
//virtual INode* NextChildNode()=0;

//virtual INode *getParent()=0;//{ return Parent; };    //получить родительский узел
//virtual void setParent(INode *parent)=0;// установить родительский узел. лучше не использовать

// функции установки и чтения значений вместо самих параметров в public-секции
// т.к. внутри реализации класса INode они могут хранится совсем не в виде CVector
// (а например в виде матрицы преобразований)
virtual void SetPos(const CVector& Pos)=0;  // Позиция
virtual void SetRot(const CVector& Rot)=0;  // Угол поворота
virtual void SetSize(const CVector& Size)=0; //размеры элемента
virtual void SetCenterPoint(const CVector& CenterPoint)=0; //точка центра (повороты, отрисовка, итп)

virtual const CVector& GetPos()=0;  // Позиция
virtual const CVector& GetRot()=0;  // Угол поворота
virtual const CVector& GetSize()=0; //размеры элемента (длина сторон параллелипипеда, который охватывает объект)
virtual const CVector& GetCenterPoint()=0; //точка центра (повороты, отрисовка, итп)

//protected:
//virtual void setParent(INode *node, INode *parent){node->Parent=parent;}; //"усыновить" ноду =)
//UINT id;
//INode *Parent;
//friend class IWorld;
};

#endif
