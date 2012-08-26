#include "CBaseObject.h"
#include <stdio.h>

#include "ILog.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#include <iostream>
using namespace std;

CBaseObject::CBaseObject(IEngine *_engine):myID(0),parent(NULL),Engine(_engine), num_count(0)
{
    num_count=1;
    Engine=_engine;
    parent=NULL;

    SetParam("BaseClass","BaseObject");
    SetParam("Type","CBaseObject");
    SetParam("Name","-");

    SetPtrParam( "BaseObject", this );
    //_it_list=Content.begin();
    printf("CBaseObject::CBaseObject\n");
}

CBaseObject::~CBaseObject()
{

    printf("CBaseObject::~CBaseObject(%s)\n", Content["Type"].c_str());

    if (parent) {
        printf("CBaseObject::~CBaseObject(%s) have a parent %d(%s)!\n", Content["Type"].c_str(), parent->GetType(), parent->GetID() );
        parent->DelChild(myID);
        }

    CBaseObject_map::iterator it_ = Childs.begin();
//    int cnt = Childs.size();
    while ( Childs.size()>0 ){

        CBaseObject* myObj = dynamic_cast<CBaseObject*>((*it_).second);
        myObj->parent = NULL; // чтобы не сработал (parent)->delChild(myID) у удаляемой ноды, снимаем указатель
        DelChild( myObj->GetID() );

        myObj->Release();

        it_ = Childs.begin();
//    cnt--;
    }

    printf("CBaseObject::~CBaseObject(%s) OK\n", Content["Type"].c_str());

}

//int CBaseObject::AddCount()
//{
//    num_count++;
//    return num_count;
//};

int CBaseObject::Release()
{

std::string obj_type = GetParam("Type");

//    printf("CBaseObject: Release IBaseObject %d %s\n",num_count, GetParam("Type") );
//    printf("CBaseObject::Release(%s)\n",obj_type.c_str() );
//    num_count--;
//    if (num_count==0)
//    {

//    if ( it_==Childs.end() ) return false;
//   Childs.erase(it_);
//    return true;
	printf("CBaseObject::Release(%s) try del this\n", obj_type.c_str());

        delete this;

	printf("CBaseObject::Release(%s) del this OK\n", obj_type.c_str());

        return 0;
//    }
//    return num_count;

}

bool CBaseObject::AddChild(IBaseObject* child){

    const char* my_type = GetParam("BaseClass");
    const char* child_type = child->GetParam("BaseClass");
    const char* child_type1 = child->GetParam("Type");
    UID childID = child->GetID();

    cout << "CBaseObject::addChild try [" << my_type << "]<--["<< child_type << "(" << child_type1 << ")("<< childID << ")]";

    CBaseObject_map::iterator it_ = Childs.find(childID);
    if ( it_!=Childs.end() ) return false; // уже есть :)

    //
    //dynamic_cast<CBaseObject*>((*it_).second)
    CBaseObject* myObj = dynamic_cast<CBaseObject*>(child);
    myObj->parent = this;
    Childs[childID] = myObj;
    //Childs_UID.push_back(childID);
    //cout << "try [" << my_type << "]<--["<< child_type << "(" << child_type1 << ")("<< childID << ")]";
    //LOGGER->ErrMsgStr(3, "Can't attach "+std::string(my_type)+"-"+child_type );

    return true; // все что угодно соединим!!! :)

}

bool CBaseObject::DelChild(UID childID){

    const char* my_type = GetParam("BaseClass");
//    const char* child_type = child->GetParam("BaseClass");

    cout << "try [" << my_type << "]-x-["<< childID << "]" << endl;
    CBaseObject_map::iterator it_ = Childs.find(childID);

    if ( it_==Childs.end() ) return false;

    cout << "try [" << GetParam("Type") << "]-x-["<< (*it_).second->GetType() << "]" << endl;

    dynamic_cast<CBaseObject*>((*it_).second)->parent = NULL;
    Childs.erase(it_);
//    Childs_UID.erase(childID);
    return true;

}

UINT CBaseObject::GetChildCount(){
    return Childs.size();
}

IBaseObject* CBaseObject::FindNode(const char* Name){

    IBaseObject* res = NULL;
    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();
    while (cnt>0) {
    if ( strcmp( (*it_).second->GetName(), Name)==0 ) return (*it_).second;
    if ( res=(*it_).second->FindNode( Name ) ) return res;
    it_++;
    cnt--;
    }

    return NULL;

}

IBaseObject* CBaseObject::GetChild(UID childID){
    CBaseObject_map::iterator it_ = Childs.find(childID);
    if (it_!=Childs.end()) return (*it_).second;
    return NULL;
}


void CBaseObject::GetChildren(asn_ext_ptr_list* myList){
    clearPtrList( myList );

    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();
    while (cnt>0) {
     addToPtrList(myList,"node",(*it_).second);
    it_++;
    cnt--;
    }

}

void CBaseObject::FindChildrenByInterface( const char* NodeInterfaceName, asn_ext_ptr_list* myList, bool Recursive){

    clearPtrList( myList );
    FindChildsByInterface_Internal( NodeInterfaceName, myList, Recursive );

}

void CBaseObject::FindChildsByInterface_Internal( const char* NodeInterfaceName, asn_ext_ptr_list* myList, bool Recursive){

    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();
    while (cnt>0) {
     if ( (*it_).second->GetInterface(NodeInterfaceName) )
        addToPtrList(myList,"node",(*it_).second);
    if (Recursive) dynamic_cast<CBaseObject*>((*it_).second)->FindChildsByInterface_Internal(NodeInterfaceName, myList, Recursive);
    it_++;
    cnt--;
    }
}

void CBaseObject::FindChildrenByType( const char* NodeTypeName, asn_ext_ptr_list* myList, bool Recursive ){

    clearPtrList( myList );
    FindChildsByType_Internal( NodeTypeName, myList, Recursive );

}

void CBaseObject::FindChildsByType_Internal( const char* NodeTypeName, asn_ext_ptr_list* myList, bool Recursive){

    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();
    while (cnt>0) {
     if ( strcmp( (*it_).second->GetType(), NodeTypeName)==0 )
        addToPtrList(myList,"node",(*it_).second);
    if (Recursive) dynamic_cast<CBaseObject*>((*it_).second)->FindChildsByType_Internal(NodeTypeName, myList, Recursive);
    it_++;
    cnt--;
    }
}

void CBaseObject::FindChildrenByBaseType( const char* NodeBaseTypeName, asn_ext_ptr_list* myList, bool Recursive ){

    clearPtrList( myList );
    FindChildsByType_Internal( NodeBaseTypeName, myList, Recursive );

}

void CBaseObject::FindChildsByBaseType_Internal( const char* NodeBaseTypeName, asn_ext_ptr_list* myList, bool Recursive){

    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();
    while (cnt>0) {
     if ( strcmp( (*it_).second->GetBaseClass(), NodeBaseTypeName)==0 )
        addToPtrList(myList,"node",(*it_).second);
    if (Recursive) dynamic_cast<CBaseObject*>((*it_).second)->FindChildsByType_Internal(NodeBaseTypeName, myList, Recursive);
    it_++;
    cnt--;
    }
}

void CBaseObject::Visit(IBaseObjectVisitor* Visitor, bool Recursive){

    Visitor->Visit(this);

    CBaseObject_map::iterator it_ = Childs.begin();
    int cnt = Childs.size();

if (Recursive) {

    while (cnt>0){
    (*it_).second->Visit(Visitor, true);
    it_++;
    cnt--;
    }
}
else {

    while (cnt>0){
    (*it_).second->Visit(Visitor,false);
    it_++;
    cnt--;
    }

}

    Visitor->AfterVisit(this);

}

const char* CBaseObject::GetBaseClass()
{
    return GetParam("BaseClass");
}

const char* CBaseObject::GetType()
{
    return GetParam("Type");
}

const char* CBaseObject::GetName()
{
//    printf("try get name from BaseObject\n");
    std::string tmp_name = GetParam("Name");
    return tmp_name.c_str();//GetParam("Name");
}

UID CBaseObject::GetID(){
    return myID;
}

UID CBaseObject::GetParentID(){
    if (parent) return parent->GetID();
    return 0;
}

// функции установки/чтения указателей на менеджеры и пр.
void CBaseObject::SetPtrParam(const char* param_name, void *param_value)
{
    VoidContent[std::string(param_name)]=param_value;
    //printf("Set PTR %s\n", param_name );
}

void *CBaseObject::GetPtrParam(const char* param_name)
{

//    printf("try get ptr param %s\n",param_name);
//printf("---ptr param list\n---");

//    asn_ptrList::iterator _it_list=VoidContent.begin();
//    while ( _it_list!=VoidContent.end() )
//    {
//	printf("%s:\n", (*_it_list).first.c_str() );
//        _it_list++;
//    };


/// ОЕ УТБВПФБМП :(
///    if ( strcmp( GetParam("BaseClass"), param_name ) ==0 ) return this;
///    if ( SetPtrParam( GetParam("BaseClass"), this );

    asn_ptrList::iterator v_it=VoidContent.find(param_name);
    if (v_it!=VoidContent.end())
        return (*v_it).second;
    else
        return NULL;
}

void CBaseObject::DelPtrParam(const char* param_name)
{
    asn_ptrList::iterator v_it=VoidContent.find(param_name);
    if (v_it!=VoidContent.end())
        VoidContent.erase(v_it);
}

ASNInterface* CBaseObject::GetInterface(const char* interface_name)
{
    //LOGGER->ErrMsgStr(3, std::string("Can't support ")+interface_name+" interface!");
    return NULL;
}

// функции установки/чтения переменных
void CBaseObject::SetParam(const char* param_name, const char* param_value)
{

    std::string ParamName = param_name;
    std::string ParamValue = param_value;
    //printf("Set %s = %s\n", ParamName.c_str(), ParamValue.c_str() );

    Content[ParamName]=ParamValue;
    //printf("Set %s = %s\n", ParamName.c_str(), ParamValue.c_str() );

}

const char* CBaseObject::GetParam(const char* param_name)
{
//    printf("try get param %s\n",param_name);
    std::map<std::string,std::string>::iterator it=Content.find(param_name);
    if (it!=Content.end())
        return (*it).second.c_str();
    else
        return ASN_NOT_FOUND_PARAM;
}

void CBaseObject::DelParam(const char* param_name)
{
    std::map<std::string,std::string>::iterator it=Content.find(param_name);
    if (it!=Content.end())
        Content.erase(it);
}

void CBaseObject::GetParamList(asn_ext_param_list* list)
{
    if (!list)
        return;
    clearList(list);
    asn_List::iterator _it_list=Content.begin();
    while ( _it_list!=Content.end() )
    {
        addParamToList(list, (*_it_list).first.c_str(), (*_it_list).second.c_str() );
        _it_list++;
    };

}

/*

UINT CBaseObject::GetParamListCount(){
    return Content.size();
};

const char* CBaseObject::GetCurrentParam(){
    return (*_it_list).second.c_str();
};

void CBaseObject::BeginParamList(){
    _it_list=Content.begin();
};

void CBaseObject::EndParamList(){
    _it_list=Content.end();
    _it_list--;
};

const char* CBaseObject::NextParam(){
    _it_list++;
    if  ( _it_list==Content.end() ) _it_list--;
    return GetCurrentParam();
};
*/

