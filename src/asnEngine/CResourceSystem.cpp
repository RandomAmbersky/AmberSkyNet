
#include "CResourceSystem.h"
#include "CTextlist.h"

#include <stdio.h>

CResourceSystem::CResourceSystem(){
TextListMap = new CTextlistMap();
((CTextlistMap*)TextListMap)->Engine=Engine;
};

CResourceSystem::~CResourceSystem(){
delete TextListMap;
};

bool CResourceSystem::SetResource(const char* Type,
                             const char* Name, BYTE* Resource, UINT Res_size ){

std::string Internalname=ASN_Str(Type)+ASN_Str(Name);


if ( (itRes=SourceList.find(Internalname))!=SourceList.end() ) return true; //такой ресурс есть уже!

internalRes myRes;
myRes.count=1;
myRes.size=Res_size;
myRes.data=Resource;

SourceList[Internalname]=myRes;
printf("add to resourcer %s \n", Internalname.c_str() );
return false;

};


BYTE *CResourceSystem::GetResource(const char* Type, const char* Name){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itRes=SourceList.find(Internalname))==SourceList.end() ) return NULL; //не найдено такого!
(*itRes).second.count+=1;
return (*itRes).second.data;
};

bool CResourceSystem::DropResource(const char* Type, const char* Name){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itRes=SourceList.find(Internalname))==SourceList.end() ) return true; //не найдено такого!

if ((*itRes).second.count>1) (*itRes).second.count-=1;
if ((*itRes).second.count==0) ReleaseResource(Type,Name);

    return false;
};

bool CResourceSystem::ReleaseResource(const char* Type, const char* Name){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itRes=SourceList.find(Internalname))==SourceList.end() ) return true; //не найдено такого!

delete (*itRes).second.data;
(*itRes).second.data=NULL;
    return false;

};

bool CResourceSystem::SetObject(const char* Type, const char* Name, IBaseObject *resObject){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itObj=ObjList.find(Internalname))!=ObjList.end() ) return true; //такой ресурс есть уже!

//internalObj myRes;
//myRes.count=1;
//myRes.obj=resObject;

ObjList[Internalname]=resObject;//myRes;
fprintf(stdout,"Upload resource %s",Internalname.c_str() );
return false;

};

IBaseObject *CResourceSystem::GetObject(const char* Type, const char* Name){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itObj=ObjList.find(Internalname))==ObjList.end() ) return NULL; //не найдено такого!
//(*itObj).second.count+=1;

IBaseObject* ret=(*itObj).second;//.obj
return ret;

};

bool CResourceSystem::ReleaseObject(const char* Type, const char* Name){

    std::string Internalname=ASN_Str(Type)+ASN_Str(Name);

if ( (itObj=ObjList.find(Internalname))==ObjList.end() ) return true; //не найдено такого!


//delete (*itObj).second;//.obj;
//(*itObj).second.obj=NULL;
    return false;

};

