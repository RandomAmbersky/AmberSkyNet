#include "ASN_Types.h"

#include "CTextlist.h"
#include <stdio.h>

//#ifndef NotFound
//static std::string NotFound=ASN_NOT_FOUND_PARAM;
//#endif

std::string ret;

CTextlist::CTextlist(std::string File_Name, int size){
FileName=File_Name;
Size=size;
it=TextList.begin();
//TextList.clear();
//Cache.clear();
};

CTextlist::~CTextlist(){
//TextList.clear();
};

void CTextlist::End(){
it=TextList.end();
it--;
};

void CTextlist::Begin(){
it=TextList.begin();
};

const char* CTextlist::GetIncString(){
ret=(*it);
if (it!=TextList.end()) it++;
return ret.c_str();
};

const char* CTextlist::GetDecString(){
ret=(*it);
if (it!=TextList.begin()) it--;
//printf("try get %s %d \n",(*it).c_str(),TextList.size());
return ret.c_str();
};

bool CTextlist::IsFirst(){
    if (it==TextList.begin()) return true;
    else return false;
};

bool CTextlist::IsEnd(){
    if (it++==TextList.end()) {it--;return true;}
    it--;
    return false;
};

void CTextlist::Add(std::string Line){
TextList.push_back(Line);
//CurPos++;
if (TextList.size()>Size) {
//    TextList.pop_front();
    printf("Clear once...\n");
    }
it=TextList.begin();
};

asn_textlist *CTextlist::GetList(int start, int num){
Cache.clear();
//if (
//Cache[0]=TextList[start];
return &Cache;
};

CTextlistMap::CTextlistMap(){
//MegaBase=Mega_base;
};

CTextlistMap::~CTextlistMap(){
std::map<std::string,CTextlist*>::iterator _it=ListMap.begin();
int i=ListMap.size();
while (i>0){
delete (*_it).second;
i--;
_it++;
};

};

void CTextlistMap::Add(const char* MapName, const char* Line){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()){
CTextlist *NewTextList= new CTextlist();
ListMap[ASN_Str(MapName)]=NewTextList;
_it=ListMap.find(MapName);
printf("create new %s \n",MapName);
};
printf("try add %s into %s\n",MapName,Line);
((*_it).second)->Add(Line);
};

CTextlist *CTextlistMap::GetTextList(std::string MapName){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return NULL;
else return (*_it).second;
};

void CTextlistMap::GetList(const char* MapName, asn_ext_chars_list* list, int start, int num){

    clearCharsList(list);

std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return;
asn_textlist *tmpList = (*_it).second->GetList(start,num);

    asn_textlist::iterator it=tmpList->begin();
    for (UINT i=0;i<tmpList->size();i++){
        addCharsToList( list, (*it).c_str() );
        it++;
    };


};

//void CTextlistMap::Add(const std::string& MapName, std::string Line){
//std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
//if (_it==ListMap.end()) return;
//};

void CTextlistMap::Begin(const char* MapName){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return;
(*_it).second->Begin();
};

void CTextlistMap::End(const char* MapName){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return;
(*_it).second->End();
};

const char* CTextlistMap::GetIncString(const char* MapName){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return ASN_NOT_FOUND_PARAM;
//CTextlist* myList=(*_it).second;
return (*_it).second->GetIncString();
};

const char* CTextlistMap::GetDecString(const char* MapName){
std::map<std::string,CTextlist*>::iterator _it=ListMap.find(MapName);
if (_it==ListMap.end()) return ASN_NOT_FOUND_PARAM;
return (*_it).second->GetDecString();
};

