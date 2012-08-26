#ifndef _ASN_LIST_
#define _ASN_LIST_

#include <map>

template< class T > class ASN_List
{
public:

void SetParam(const std::string& param_name,const T* param_value){
Content[param_name]=param_value;
};

const T* GetParam(const std::string& param_name){
std::map<std::string,<T*>>::iterator it=Content.find(param_name);
if (it!=Content.end()) return (*it).second;
else return std::string(ASN_NOT_FOUND_PARAM);
};

void DelParam(const std::string& param_name){

std::map<std::string,T*>::iterator it=Content.find(param_name);
if (it!=Content.end()) Content.erase(it);

};

protected:

 std::map< std::string,T* > asn_List;

};

//typedef ASN_List< std::string > ASN_StringList;

#endif
