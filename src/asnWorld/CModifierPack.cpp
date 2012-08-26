#include "CModifierPack.h"

CModifierPack::CModifierPack(IEngine *engine):CBaseObject(engine){
    SetParam("BaseClass","ModifierPack");
    SetParam("Type","ModifierPack");
    SetParam("Name","-");
};

CModifierPack::~CModifierPack(){

    for (UINT i=0; i<ModifPack.GetSize(); i++)
    {
        INodeModifier *myMod=(INodeModifier*)ModifPack.GetItem(i);

        if (myMod)
            myMod->Release();
    }

    printf("CModifierPack::~CModifierPack()...\n");

};

UINT CModifierPack::AddModifier(INodeModifier* modifier){
    UINT curModNum = ModifPack.AddItem( modifier );
    return curModNum;
};

INodeModifier* CModifierPack::GetModifier(UINT id){
    return (INodeModifier*) ModifPack.GetItem( id );
};

UINT CModifierPack::GetModifierCount(){
    return ModifPack.GetSize();
}

bool CModifierPack::DelModifier(UINT id){

    INodeModifier *myMod=(INodeModifier*)ModifPack.GetItem(id);
    if (myMod)
    {
        myMod->Release();
        ModifPack.DelItem(id);
        return false;
    }

    return true;

};

bool CModifierPack::Modify(INode *node,float tms){

//printf("modify..");

    for (UINT i=0; i<ModifPack.GetSize(); i++)
    {

        INodeModifier *myMod=(INodeModifier*)ModifPack.GetItem(i);

        if  (myMod)
            //myMod->Modify(node,tms);
            if ( myMod->Modify(node,tms)== 1 ) ModifPack.DelItem(i);

    }

};

/*
CModifierList::CModifierList(){

};

CModifierList::~CModifierList(){

};

bool CModifierList::AddModifierPack(IModifierPack* modifier_pack){

    IModifierPack* testPack;
    // читаем имя обьекта (т.к. он наследуется от BaseObject )
    std::string PackName = modifier_pack->GetName();

    if ( (testPack=GetModifierPackByName(PackName)) != NULL ) {
//        LOGGER->ErrMsg(2,"ModifierPack "+PackName+" exist!");
        return true;//testPack->Release();
        };
    ModifList[PackName]=modifier_pack;
    return false;

};

IModifierPack* CModifierList::GetModifierPackByName(const std::string& PackName){

    std::map<std::string, IModifierPack*>::iterator _it=ModifList.find(PackName);
    if ( _it == ModifList.end() ) return NULL;
    return (*_it).second;

};

bool CModifierList::DelModifierPack(IModifierPack* modifier_pack){

};

bool CModifierList::Modify(){

};
*/

