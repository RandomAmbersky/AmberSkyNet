#ifndef _C_DATAPARSER_H
#define _C_DATAPARSER_H

#include "IDataParser.h"
#include "CBaseObject.h"

#include "IEngine.h"
//#include "pugixml.hpp"

#include "lib/tinyXML/tinyxml.h"

class CDataParserXML: public IDataParser /*, public CBaseObject*/ {
public:
    CDataParserXML(IEngine *Engine);
    ~CDataParserXML();

//    void InitManager();
//    void ReleaseManager();

    DataLeaf StartParse(void *data);
    void EndParse();
    const char* GetTreeAsChars();

    // функции разбора XML
    const char *GetLeafName(DataLeaf data);

    DataLeaf GetParent(DataLeaf data);
    DataLeaf GetSectionByName(DataLeaf data, const char * LeafName);
    DataLeaf GetFirstChildLeaf(DataLeaf data);
    DataLeaf GetNextSiblingLeaf(DataLeaf data);
    void* GetInnerRawData(DataLeaf data);

    void GetLeafParamList(DataLeaf data, asn_ext_param_list* list);
    const char *GetAttribute(DataLeaf data, const char* name);
    const char *GetAsText(DataLeaf data);


    // функции создания XML
    DataLeaf CreateRootLeaf();
    bool SaveDataTree(const char * FileName );
    DataLeaf AddLeafWithName(DataLeaf parent, const char *LeafName);
//    bool SetLeafName(DataLeaf data, const char *LeafName);
//    bool AddChildLeaf(DataLeaf data, DataLeaf child);
    bool SetInnerRawData(DataLeaf data, void *raw, unsigned int size);
    bool SetParam(DataLeaf data, const char * Name, const char * Value );


protected:
    TiXmlDocument *doc;
//    pugi::xml_document doc;
};

#endif
