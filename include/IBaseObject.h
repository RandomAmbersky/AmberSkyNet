#ifndef _IBASE_OBJECT_H
#define _IBASE_OBJECT_H

// базовый объект дл€ классов движка AmberSkyNet

// const после функции - это значит что она не мен€ет члены класса
#include <stdio.h>
#include <string.h>

#include "ASN_Types.h"
#include "ASN_Params.h"
#include "Interfaces/ASNInterface.h"

//#include "IEngine.h"

//#include <map>

//class IEngine;
class IBaseObject;

class IBaseObjectVisitor {
    public:
    virtual void Visit( IBaseObject* obj)=0;
    virtual void AfterVisit( IBaseObject* obj)=0;
};


class IBaseObject {
public:

//IBaseObject(){ num_count=1; }
virtual ~IBaseObject(){ printf("IBaseObject::~IBaseObject()\n"); };
//virtual int AddCount()=0;//{num_count++; return num_count;};
virtual int Release()=0;

virtual const char* GetBaseClass()=0;
virtual const char* GetType()=0;
virtual const char* GetName()=0;

virtual UID GetID()=0;
virtual UID GetParentID()=0;

virtual bool AddChild(IBaseObject* child)=0;
virtual bool DelChild(UID childID)=0;

virtual UINT GetChildCount()=0;
virtual IBaseObject* GetChild(UID childID)=0;
virtual IBaseObject* FindNode(const char* NodeName)=0;

virtual void GetChildren(asn_ext_ptr_list* list)=0;
virtual void FindChildrenByInterface( const char* NodeInterfaceName, asn_ext_ptr_list* list, bool Recursive=true)=0;
virtual void FindChildrenByType( const char* NodeTypeName, asn_ext_ptr_list* list, bool Recursive=true )=0;
virtual void FindChildrenByBaseType( const char* NodeBaseTypeName, asn_ext_ptr_list* list, bool Recursive=true )=0;

virtual void Visit(IBaseObjectVisitor* visitor, bool Recursive=true )=0;

//virtual void Vizit( CALLBACK_VIZIT_FUNCTION myF )=0;
//const char* GetBaseClass(){ return GetParam("BaseClass"); }
//const char* GetType(){ return GetParam("Type"); }
//const char* GetName(){ return GetParam("Name"); }

// функции установки/чтени€ указателей на менеджеры и пр.
virtual void SetPtrParam(const char* param_name, void *param_value)=0;
virtual void *GetPtrParam(const char*param_name)=0;
virtual void DelPtrParam(const char* param_name)=0;

// функции установки/чтени€ переменных
virtual void SetParam(const char* param_name, const char* param_value)=0;
virtual const char* GetParam(const char* param_name)=0;
virtual void DelParam(const char* param_name)=0;

virtual void GetParamList(asn_ext_param_list* list)=0; //получить список параметров

virtual ASNInterface* GetInterface(const char* interface_name)=0; //получить интерфейс к объекту,
//позвол€ющий с ним более углубленно работать

/*
virtual void* AsType(const char* type_name){

    void* my_obj = GetPtrParam(type_name);
    return my_obj;

}
*/

// "гуманистические" функции чтени€ переменных :)
const char* GetStrParam(const char* param_name, const char *defaultValue=NULL)
{
    const char* ret_value=GetParam(param_name);
    if ( strcmp(ret_value, ASN_NOT_FOUND_PARAM) == 0) return defaultValue;
    return ret_value;
};

const int GetIntParam(const char* param_name, const int defaultValue=0)
{
    int ret_value;
    const char* ret=GetParam(param_name);
    if ( strcmp(ret, ASN_NOT_FOUND_PARAM) == 0) return defaultValue;
    if (sscanf(ret,"%d",&ret_value)==1) return ret_value;
    return defaultValue;
};

const float GetFloatParam(const char* param_name, const float defaultValue=0.0f)
{
    float ret_value;
    const char* ret=GetParam(param_name);
    if ( strcmp(ret, ASN_NOT_FOUND_PARAM) == 0) return defaultValue;
    if (sscanf(ret,"%f",&ret_value)==1) return ret_value;
    return defaultValue;
};

const float GetDoubleParam(const char* param_name, const double defaultValue=0.0f)
{
    double ret_value;
    const char* ret=GetParam(param_name);
    if ( strcmp(ret, ASN_NOT_FOUND_PARAM) == 0) return defaultValue;
    if (sscanf(ret,"%lg",&ret_value)==1) return ret_value;
    return defaultValue;
};

// "гуманистические" функции записи переменных :)
void SetParam(const char* param_name, const int param_value)
{
    char buf[64];
    sprintf(buf, "%d", param_value);
    SetParam(param_name, buf);
}

void SetParam(const char* param_name, const float param_value)
{
    char buf[64];
    sprintf(buf, "%f", param_value);
    SetParam(param_name, buf);
}

void SetParam(const char* param_name, const double param_value)
{
    char buf[80];
    sprintf(buf, "%lg", param_value);
    SetParam(param_name, buf);
}

};

#endif

