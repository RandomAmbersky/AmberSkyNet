#include "IDataParser.h"
#include "IBaseObject.h"
#include "ASN_Types_internal.h"

DataLeaf Object2Leaf(IDataParser* parser, DataLeaf parent, IBaseObject *node,
                std::string& LeafName, asn_List& delList){

std::string nodeName;
//asn_List delList; // не пишем в структуру то, что содержится здесь

    asn_ext_param_list* tempParamList= createList();
    node->GetParamList(tempParamList);

    //asn_List tempParamList=node->getParamList();
    //asn_List::iterator it=tempParamList.begin();

    DataLeaf tempLeaf=parser->AddLeafWithName(parent, LeafName.c_str());
    asn_List::iterator dl_it=delList.end();
    asn_List::iterator dl_find=delList.end();

    for( UINT i=0; i<tempParamList->nums; ++i)
    {

        if ( ( dl_find = delList.find( tempParamList->param_list[i].name )) == dl_it )
            parser->SetParam(tempLeaf, tempParamList->param_list[i].name , tempParamList->param_list[i].value );
            else // иначе, если такой параметр есть в базовом обьекте, смотрим - не переопределен ли он в объекте
              if ( (*dl_find).second != tempParamList->param_list[i].value )
                    parser->SetParam(tempLeaf, tempParamList->param_list[i].name , tempParamList->param_list[i].value );
    };

    deleteList( tempParamList );

    return tempLeaf;
};
