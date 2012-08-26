#ifndef _CBASE_OBJECT_H
#define _CBASE_OBJECT_H

// реализаци€ базового объекта дл€ классов движка AmberSkyNet

// const после функции - это значит что она не мен€ет члены класса
#include "ASN_Types.h"
#include "ASN_Types_internal.h"

#include "IEngine.h"
#include "IBaseObject.h"

class CBaseObject;

typedef std::map<UID,CBaseObject*> CBaseObject_map;
//typedef std::list<UID> IBaseObject_vec;

class CBaseObject: public virtual IBaseObject {
public:

CBaseObject(IEngine *engine);
virtual ~CBaseObject();

//int AddCount();

virtual int Release();

const char* GetBaseClass();
const char* GetType();
const char* GetName();
UID GetID();
UID GetParentID();

// функции установки/чтени€ указателей на менеджеры и пр.
void SetPtrParam(const char* param_name, void *param_value);
void *GetPtrParam(const char*param_name);
void DelPtrParam(const char* param_name);

// функции установки/чтени€ переменных
void SetParam(const char* param_name, const char* param_value);
const char* GetParam(const char* param_name);
void DelParam(const char* param_name);

void GetParamList(asn_ext_param_list* list); //получить список параметров

ASNInterface* GetInterface(const char* interface_name); //получить интерфейс к объекту,

virtual bool AddChild(IBaseObject* child);
virtual bool DelChild(UID childID);
virtual void Visit(IBaseObjectVisitor* Visitor, bool Recursive );

virtual UINT GetChildCount();
virtual IBaseObject* GetChild(UID childID);
virtual IBaseObject* FindNode(const char* NodeName);

virtual void GetChildren(asn_ext_ptr_list* list);

void FindChildrenByType( const char* NodeTypeName, asn_ext_ptr_list* list, bool Recursive=true );
void FindChildrenByBaseType( const char* NodeBaseTypeName, asn_ext_ptr_list* list, bool Recursive=true );
void FindChildrenByInterface( const char* NodeInterfaceName, asn_ext_ptr_list* list, bool Recursive=true);

// эти функции не обнул€ют входной list
void FindChildsByType_Internal( const char* NodeTypeName, asn_ext_ptr_list* list, bool Recursive );
void FindChildsByBaseType_Internal( const char* NodeBaseTypeName, asn_ext_ptr_list* list, bool Recursive );
void FindChildsByInterface_Internal( const char* NodeInterfaceName, asn_ext_ptr_list* list, bool Recursive);

//virtual UINT GetParamListCount();
//virtual const char* GetCurrentParam();
//virtual void BeginParamList();
//virtual void EndParamList();
//virtual const char* NextParam();

//virtual void asn_ext_param_list* getParamList(); //получить список параметров

    UID myID; // for plug factory
    CBaseObject* parent; // for some interest internal CBaseObject tree manipulation

protected:

    IEngine *Engine;

    int num_count;
    asn_List Content;
    //asn_List::iterator _it_list;
    asn_ptrList VoidContent;
//    IBaseObject_list Childs_UID;
    CBaseObject_map Childs;

};

#endif
