// ��������� � ������� ������ � ����
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

// ����� ��� ��� ���� ����� ���� stl?
// ����� �� ����������� ������� "������" ���������� ������ POD-����

// ����� ���������� ������ POD-����?
// ����� �� ������������� � ������ ����������� � �� ��������� ������ ������,
// � ������ ������� ��� ������� dll � ������ � ��� ��� ��� �������� � ���������� �������
// ���� HGE � IrrLight

// ��� ����� POD-���?
// Plain Old Data
// � POD-����� ���������:
// 1. ��� ���������� �������������� ����, ������� wchar_t � bool
// 2. ������������, �.�. ���� ����������� � ������� ��������� ����� enum
// 3. ���������
// 4. POD-��������� (struct ��� class) � POD-����������� (union)


#endif
