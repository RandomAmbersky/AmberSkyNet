#ifndef _ICNODEMODIFIER_H
#define _ICNODEMODIFIER_H

#include "INodeModifier.h"
#include "CBaseObject.h"
#include <string>
//#include <list>
//#include <map>

#include "ASN_SafeVector_template.h"

typedef ASN_SafeVector< INodeModifier > safe_mod_list;

class CModifierPack: public IModifierPack, public CBaseObject {

    public:
        CModifierPack(IEngine *engine);
        ~CModifierPack();

        virtual UINT AddModifier(INodeModifier* modifier);
        virtual UINT GetModifierCount();
        virtual INodeModifier* GetModifier(UINT id);
        virtual bool DelModifier(UINT id);

        virtual bool Modify(INode *node,float tms);

    protected:
        safe_mod_list ModifPack;

};

//class CModifierList: public IModifierList {
//
//    public:
//        CModifierList();
//        virtual ~CModifierList();
//        virtual bool AddModifierPack(IModifierPack* modifier_pack);
//        virtual IModifierPack* GetModifierPackByName(const std::string& PackName);
//        virtual bool DelModifierPack(IModifierPack* modifier_pack);
//        virtual bool Modify();
//    protected:
//        std::map<std::string, IModifierPack*> ModifList;
//
//};

#endif
