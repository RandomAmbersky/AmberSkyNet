#include "CDataParserXML.h"

#include "ASN_Params.h"

CDataParserXML::CDataParserXML(IEngine *_Engine)//:CBaseObject(_Engine)
{
    //Engine=_Engine;
    doc=NULL;
}

/*
void CDataParserXML::InitManager() {
    printf("CDataParser init...\n");
};

void CDataParserXML::ReleaseManager() {
    printf("CDataParser released..\n");
    delete this;
};
^*/

CDataParserXML::~CDataParserXML()
{
    if (doc)
        delete doc;
}

// начало парсинга
DataLeaf CDataParserXML::StartParse(void *data)
{

    if (doc)
        delete doc;
    doc = new TiXmlDocument();
//cout << "CDataParserXML" << (char*)data << endl;
    doc->Parse((char*)data);
//cout << "CDataParserXML" << (char*)data << endl;
    return doc;//->FirstChild( "AmberSkyNetXML" )->ToElement();

};

void CDataParserXML::EndParse()
{
    if (doc){
        delete doc;
	doc = NULL;
	}

};

const char* CDataParserXML::GetTreeAsChars()
{

    TiXmlPrinter printer;
    doc->Accept( &printer );
    const char* xmlcstr = printer.CStr();

    return xmlcstr;

};

// функции разбора XML
const char *CDataParserXML::GetLeafName(DataLeaf data)
{
    return ((TiXmlElement* )data)->Value();
};

DataLeaf CDataParserXML::GetSectionByName(DataLeaf data, const char * LeafName)
{
    return((TiXmlElement *)data)->FirstChildElement( LeafName );
};

DataLeaf CDataParserXML::GetParent(DataLeaf data){

    TiXmlNode* tmp = (TiXmlNode*)data;
    tmp = tmp->Parent();
    return (TiXmlElement *)tmp;

}

DataLeaf CDataParserXML::GetFirstChildLeaf(DataLeaf data)
{
    return ((TiXmlElement*)data)->FirstChildElement();
};

DataLeaf CDataParserXML::GetNextSiblingLeaf(DataLeaf data)
{
//	cout << "CDataParserXML: " << GetAsText(data) << endl;
    return ((TiXmlElement*)data)->NextSiblingElement();
};

void* CDataParserXML::GetInnerRawData(DataLeaf data)
{
    return (void*) ((TiXmlElement*)data)->FirstChild()->Value();
};

void CDataParserXML::GetLeafParamList(DataLeaf data, asn_ext_param_list* list)
{
    if (!list)
        return;

    clearList(list);

//printf("get leaf param list\n");

    TiXmlAttribute* Cur_Attr=((TiXmlElement* )data)->FirstAttribute();
    std::string Name="";
    std::string Value="";

    if (Cur_Attr)
        do
        {

//	    printf("next...\n");
            Name=Cur_Attr->Name();
            Value=Cur_Attr->Value();
            addParamToList( list, Name.c_str(), Value.c_str() );

//	    printf("add to param list %s %s\n", Name.c_str(), Value.c_str() );

        }
        while ( Cur_Attr=Cur_Attr->Next() );

	printf("leaf param list ok\n");
};

const char *CDataParserXML::GetAttribute(DataLeaf data, const char* name){

    return ((TiXmlElement* )data)->Attribute(name);

}

DataLeaf CDataParserXML::CreateRootLeaf()
{
    //  "<AmberSkyNetXML>\n"
    //  "</AmberSkyNetXML>";
    EndParse();

    const char* demoStart =
        "<?xml version=\"1.0\" >\n";
    doc = new TiXmlDocument();
    doc->Parse(demoStart);
    //    TiXmlElement *node = doc->FirstChild( "AmberSkyNetXML" )->ToElement();
    return doc;
};

DataLeaf CDataParserXML::AddLeafWithName(DataLeaf parent, const char *LeafName)
{

    TiXmlElement* tmp= new TiXmlElement(LeafName);
    ((TiXmlElement* )parent)->LinkEndChild( tmp );
    return tmp;

};

//bool CDataParserXML::SetLeafName(DataLeaf data, const char *LeafName){
//    ((TiXmlElement* )data)->SetValue(LeafName);
//    return false;
//};

//bool CDataParserXML::AddChildLeaf(DataLeaf parent, DataLeaf child){
//    ((TiXmlElement*)parent)->LinkEndChild( (TiXmlElement*)child );
//    return false;
//};

bool CDataParserXML::SetInnerRawData(DataLeaf data, void *raw, unsigned int size)
{
    return true;
};

bool CDataParserXML::SetParam(DataLeaf data, const char * Name, const char * Value )
{
    ((TiXmlElement* )data)->SetAttribute( Name, Value );
};

bool CDataParserXML::SaveDataTree(const char * FileName )
{
    doc->SaveFile(FileName);
    return false;
};

const char* CDataParserXML::GetAsText(DataLeaf data){

    char* buff;

    TiXmlPrinter printer;
    ((TiXmlElement* )data)->Accept( &printer );
    std::string xmlcstr = printer.CStr();

//cout << "CDataParserXML:GetAsText " << xmlcstr.c_str() << endl;

    return xmlcstr.c_str();

//    const char* xmlcstr1 = xmlcstr.c_str();
//    buff = new char[sizeof(xmlcstr1)];
//    memcpy(buff, xmlcstr1, sizeof(xmlcstr1)+1);
//    return buff;

//    return ((TiXmlElement* )data)->ToText()->Value();

};
