#ifndef _I_DATAPARSER_H
#define _I_DATAPARSER_H

#include "ASN_Params.h"

//#include "IManager.h"

typedef void* DataLeaf;

class IDataParser {
public:
    virtual ~IDataParser(){}

    // начало парсинга
    virtual DataLeaf StartParse(void *data)=0;
    // конец парсинга
    virtual void EndParse()=0;

    //получаем наше дерево в виде const chars*
    virtual const char* GetTreeAsChars()=0;

    // функции разбора XML
    virtual const char *GetLeafName(DataLeaf data)=0;
    virtual const char *GetAsText(DataLeaf data)=0;

    // получаем секцию по её имени
    virtual DataLeaf GetSectionByName(DataLeaf data, const char * LeafName)=0;

    virtual DataLeaf GetParent(DataLeaf data)=0;
    virtual DataLeaf GetFirstChildLeaf(DataLeaf data)=0;
    virtual DataLeaf GetNextSiblingLeaf(DataLeaf data)=0;

    virtual void GetLeafParamList(DataLeaf data, asn_ext_param_list* list)=0;
    virtual const char *GetAttribute(DataLeaf data, const char* name)=0;

    // функции создания XML
    virtual DataLeaf CreateRootLeaf()=0;
    virtual bool SaveDataTree(const char * FileName )=0;
    virtual DataLeaf AddLeafWithName(DataLeaf parent, const char *LeafName)=0;
    virtual bool SetParam(DataLeaf data, const char * Name, const char * Value )=0;

};

#endif
