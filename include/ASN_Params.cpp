#include "ASN_Params.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

asn_ext_chars_list* CreateCharsList(){
    asn_ext_chars_list* list=new asn_ext_chars_list();
    list->values=NULL;
    list->nums=0;
    return list;
}

void clearCharsList(asn_ext_chars_list* list){
    for (UINT i=0;i<list->nums;i++)
    {
        delete [] list->values[i];
    };

    list->values=(char**)realloc(list->values, 0);

    list->nums=0;
}

void deleteCharsList(asn_ext_chars_list* list){
    clearCharsList(list);
    delete list;
}

UINT addCharsToList(asn_ext_chars_list* list, const char *Value){

    list->values=(char**)realloc(list->values, (list->nums+1)*sizeof(char*));

    list->values[list->nums]=new char[strlen(Value)+1];
    strcpy(list->values[list->nums], Value);
    list->values[list->nums][strlen(Value)]=0;

    list->nums=list->nums+1;
    return list->nums;

}

const char* getParamFromCharsList(asn_ext_chars_list* list, UINT i){

    return list->values[i];

}

asn_ext_param_list* createList(){
    asn_ext_param_list* list = new asn_ext_param_list();
    list->param_list=NULL;
    list->nums=0;
    return list;
}

void clearList(asn_ext_param_list* list)
{
//    printf("clear list..\n");
    for (UINT i=0;i<list->nums;i++)
    {
        delete list->param_list[i].name;
        delete list->param_list[i].value;
    };

    list->param_list=(asn_ext_param*)realloc(list->param_list, 0);

    list->nums=0;

}

void deleteList(asn_ext_param_list* list)
{

    clearList(list);
    delete list;

}

UINT addParamToList(asn_ext_param_list* list,  const char *Name, const char *Value)
{
    list->param_list=(asn_ext_param*)realloc(list->param_list, (list->nums+1)*sizeof(asn_ext_param));

    list->param_list[list->nums].name=new char[strlen(Name)+1];
    strcpy(list->param_list[list->nums].name, Name);
    list->param_list[list->nums].name[strlen(Name)]=0;

    list->param_list[list->nums].value=new char[strlen(Value)+1];
    strcpy(list->param_list[list->nums].value, Value);
    list->param_list[list->nums].value[strlen(Value)]=0;

    list->nums=list->nums+1;
//    printf("add param to list: nums %d\n",list->nums);
    return list->nums;
}

const asn_ext_param *getParamFromList(asn_ext_param_list* list, UINT i)
{
    return &list->param_list[i];
}

asn_ext_ptr_list* createPtrList(){
    asn_ext_ptr_list* list = new asn_ext_ptr_list();
    list->ptr_list=NULL;
    list->nums=0;
    return list;
}

void clearPtrList(asn_ext_ptr_list* list)
{

    for (UINT i=0;i<list->nums;i++)
    {
        delete list->ptr_list[i].name;
    };

    list->ptr_list=(asn_ext_ptr_param*)realloc(list->ptr_list, 0);
    list->nums=0;

}

void deletePtrList(asn_ext_ptr_list* list){
    clearPtrList(list);
    delete list;
}

UINT addToPtrList(asn_ext_ptr_list* list,  const char *Name, void *Pointer)
{

    list->ptr_list=(asn_ext_ptr_param*)realloc(list->ptr_list, (list->nums+1)*sizeof(asn_ext_ptr_param));

    list->ptr_list[list->nums].name=new char[strlen(Name)+1];
    strcpy(list->ptr_list[list->nums].name, Name);
    list->ptr_list[list->nums].name[strlen(Name)]=0;

    list->ptr_list[list->nums].pointer=Pointer;

    list->nums=list->nums+1;

    return list->nums;

}

const asn_ext_ptr_param *getPtrFromList(asn_ext_ptr_list* list, UINT i)
{
    return &list->ptr_list[i];
}

