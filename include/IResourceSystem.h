#ifndef _IRESOURCE_SYSTEM_H
#define _IRESOURCE_SYSTEM_H

#include  "ASN_Types.h"
#include  "ASN_Params.h"

#include <string>
#include <list>

#include "IBaseObject.h"

//struct asn_textlist{
//asn_ext_param* param_list;
//UINT nums;
//};

class ITextListMap {
public:

virtual void GetList(const char* MapName, asn_ext_chars_list* list, int start=0, int num=30)=0;
virtual void Add(const char* MapName, const char* Line)=0;
virtual void Begin(const char* MapName)=0;
virtual void End(const char* MapName)=0;
virtual const char* GetIncString(const char* MapName)=0;
virtual const char* GetDecString(const char* MapName)=0;

};

/*
class IResource{
public:
    virtual BYTE* GetData()=0;  // �������� ������
    virtual UINT  GetSize()=0;  // �������� ������
    virtual UINT  GetCount()=0; // ������� ������������ ������
    virtual bool  Release()=0; //����������� ������
};
*/

class IResourceSystem{
public:
IResourceSystem(){}
~IResourceSystem(){}

// ������������� ����� ������
virtual bool SetResource(const char* Type,
     const char* Name, BYTE* Resource, UINT Res_size )=0;
// ���� ������
virtual BYTE *GetResource(const char* Type, const char* Name)=0;
// ����������� �� �������
virtual bool ReleaseResource(const char* Type, const char* Name)=0;

// ������������� ����� ������
virtual bool SetObject(const char* Type,
     const char* Name, IBaseObject *resObject)=0;
// ���� ������
virtual IBaseObject *GetObject(const char* Type, const char* Name)=0;
// ����������� �� �������
virtual bool ReleaseObject(const char* Type, const char* Name)=0;

virtual ITextListMap* GetTextListMap()=0;

};


#define RESOURSER ((IResourceSystem *)ENGINE->GetPtrParam("ResourceSystem"))
#define LISTMAPPER (ITextListMap *)RESOURSER->GetTextListMap()

#endif
