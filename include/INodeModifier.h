#ifndef _INODEMODIFIER_H
#define _INODEMODIFIER_H

// "�����������" ���
#include "IBaseObject.h"
#include "INode.h"

class INodeModifier: public virtual IBaseObject {

    public:
        virtual ~INodeModifier(){};
        virtual int Modify(INode *node,float tms)=0;
        // 0 - ��� ok
        // 1 - ����������� ������
        // 2... - ������

};

// �����, "��������" �������������
class IModifierPack: public virtual IBaseObject {

    public:
        virtual ~IModifierPack(){};

        virtual UINT AddModifier(INodeModifier* modifier)=0;
        virtual UINT GetModifierCount()=0;
        virtual INodeModifier* GetModifier(UINT id)=0;
        virtual bool DelModifier(UINT id)=0;

        virtual bool Modify(INode *node, float tms)=0;

};

// �� � ����� ���? ;)
// ������ �������������
//class IModifierList: public IBaseObject {
//
//    public:
//        virtual ~IModifierList(){};
//        virtual bool AddModifierPack(IModifierPack* modifier_pack)=0;
//        virtual IModifierPack* GetModifierPackByName(const std::string& PackName)=0;
//        virtual bool DelModifierPack(IModifierPack* modifier_pack)=0;
//        virtual bool Modify()=0;
//
//};


#endif
