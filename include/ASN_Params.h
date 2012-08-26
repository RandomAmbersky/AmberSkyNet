// параметры и удобная работа с ними
#ifndef _ASN_PARAMS_H
#define _ASN_PARAMS_H

#include "ASN_Types.h"

struct asn_ext_chars_list
{
    char** values;
    UINT nums;
};

asn_ext_chars_list* createCharsList();
void clearCharsList(asn_ext_chars_list* list);
void deleteCharsList(asn_ext_chars_list* list);
UINT addCharsToList(asn_ext_chars_list* list, const char *Value);
const char* getParamFromCharsList(asn_ext_chars_list* list, UINT i);

struct asn_ext_param
{
    char *name;
    char *value;
};

struct asn_ext_param_list
{
    asn_ext_param* param_list;
    UINT nums;
};

asn_ext_param_list* createList();
void clearList(asn_ext_param_list* list);
void deleteList(asn_ext_param_list* list);
UINT addParamToList(asn_ext_param_list* list,  const char *Name, const char *Value);
const asn_ext_param *getParamFromList(asn_ext_param_list* list, UINT i);

struct asn_ext_ptr_param
{
    char *name;
    void *pointer;
};

struct asn_ext_ptr_list
{
    asn_ext_ptr_param* ptr_list;
    UINT nums;
};

asn_ext_ptr_list* createPtrList();
void deletePtrList(asn_ext_ptr_list* list);
void clearPtrList(asn_ext_ptr_list* list);
UINT addToPtrList(asn_ext_ptr_list* list,  const char *Name, void *Pointer);
const asn_ext_ptr_param *getPtrFromList(asn_ext_ptr_list* list, UINT i);

// зачем все это надо когда есть stl?
// чтобы из загружаемых модулей "наружу" передавать только POD-типы

// зачем передавать только POD-типы?
// чтобы не привязываться к версии компилятора и не компилить движок заново,
// а просто скачать уже готовые dll и хидеры к ним как это делается в нормальных движках
// типа HGE и IrrLight

// что такое POD-тип?
// Plain Old Data
// К POD-типам относятся:
// 1. все встроенные арифметические типы, включая wchar_t и bool
// 2. перечисления, т.е. типы обьявленные с помощью ключевого слова enum
// 3. указатели
// 4. POD-структуры (struct или class) и POD-обьединения (union)


#endif
