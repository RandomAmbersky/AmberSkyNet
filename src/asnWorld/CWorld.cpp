// пивовар Иван Таранов
// пивовар Иван Таранов
// обожал бомбить баранов
// как напьется пива "Пит"
// так на пастбище летит

#include "IEngine.h"

#include "CWorld.h"
//#include "IPluginFactory.h"

#include "IFileSystem.h"
#include "IDataParser.h"

#include "INodeModifier.h"

#include "CBoundingBox.h"

#include "ASN_utils.h"

#include "IDevice.h"
#include "ILog.h"
#include "INode.h"

#include <iostream>
#define ENGINE Engine //у нас уже есть указатель на Engine

using namespace std;


//extern DataLeaf Object2Leaf(IDataParser* parser, DataLeaf parent, IBaseObject *node,
                                //std::string& LeafName, asn_List& delList);

/*
// из данных в ноды
INode *CWorld::Leaf2Data(IDataParser* parser, DataLeaf parent)
{
    printf("leaf2data\n");

    asn_ext_param_list* tempParamList = createList();
    printf("leaf2data2\n");
    parser->GetLeafParamList(parent,tempParamList);
    printf("leaf2data3\n");
    //asn_List tempParamList=parser->GetParamList(parent);
    std::string nodeName;
    //if ( tempParamList.find("BaseType")==tempParamList.end() )
    nodeName = parser->GetLeafName(parent);
    printf("leaf2data4: %s\n", nodeName.c_str() );


    //else
    //nodeName=tempParamList["BaseType"];

//    printf("XML node name: %s \n",nodeName.c_str());

    uint tmpID=createNode( nodeName.c_str() );
    printf("leaf2data5\n");
    printf("create node ok\n");
    INode *tmpNode;

    if (tmpID==0)
        tmpNode=root_node; // если id=0 - корневая нода, через getNode её получить не выйдет
    else
        tmpNode=getNode ( tmpID );


printf("getNodee ok %d\n",tempParamList->nums);

if (!tmpNode) return NULL;

    for( UINT i=0; i<tempParamList->nums; ++i)
    {

	printf("param name=%s\n", tempParamList->param_list[i].name);

        if ( strcmp(tempParamList->param_list[i].name,"Type")==0)
        {}
        else if ( strcmp(tempParamList->param_list[i].name,"BaseClass")==0)
        {}
        else if ( strcmp(tempParamList->param_list[i].name,"Name")==0 )
        {

            if ( NodeNameList.find( tempParamList->param_list[i].value )==NodeNameList.end() )
            {
                tmpNode->SetParam("Name", tempParamList->param_list[i].value );
                NodeNameList[ tempParamList->param_list[i].value ]=tmpNode;
                //                             printf("Add node name %s in name list\n",tmpNode->GetName());
            }
            else
            {
                LOGGER->ErrMsgStr(3,"Node in the wolrd already have name "+ASN_Str(tempParamList->param_list[i].value)+"!");
            }
        }
        else
            tmpNode->SetParam(tempParamList->param_list[i].name, tempParamList->param_list[i].value );

        if ( strcmp(tempParamList->param_list[i].name,"Pos")==0)
        {
            if ( CheckStrToVector( tempParamList->param_list[i].value ) )
            tmpNode->setPos(  StrToVector( tempParamList->param_list[i].value ) );
            else LOGGER->ErrMsg(2,"Pos error!");
//            } catch( const char *msg ) { LOGGER->ErrMsg(2,msg); return tmpNode; }
        }

        if ( strcmp(tempParamList->param_list[i].name,"Rot")==0)
        {
            if ( CheckStrToVector( tempParamList->param_list[i].value ) )
            tmpNode->setRot(  StrToVector( tempParamList->param_list[i].value ) );
            else LOGGER->ErrMsg(2,"Rot error!");
//            } catch( const char *msg ) { LOGGER->ErrMsg(2,msg); }

        }

        if ( strcmp(tempParamList->param_list[i].name,"Size")==0)
        {
//            try {
            if ( CheckStrToVector( tempParamList->param_list[i].value ) )
            tmpNode->setSize(  StrToVector( tempParamList->param_list[i].value ) );
            else LOGGER->ErrMsg(2,"Size error!");
//            } catch( const char *msg ) { LOGGER->ErrMsg(2,msg); }
        }

        if ( strcmp(tempParamList->param_list[i].name,"CenterPoint")==0)
        {
//            try {
            if ( CheckStrToVector( tempParamList->param_list[i].value ) )
            tmpNode->setCenterPoint(  StrToVector( tempParamList->param_list[i].value ) );
            else LOGGER->ErrMsg(2,"CenterPoint error!");
//             } catch( const char *msg ) { LOGGER->ErrMsg(2,msg); }

        }

    };

    //asn_List::iterator it_e=tempParamList.end();
    //    it=tempParamList.find("Pos");
    //if (it!=it_e)
    //tmpNode->setPos(  StrToVector( (*it).second  ) );

    //it=tempParamList.find("Rot");
    //if (it!=it_e)
    //tmpNode->setRot(  StrToVector( (*it).second  ) );

    //it=tempParamList.find("Size");
    //if (it!=it_e)
    //tmpNode->setSize(  StrToVector( (*it).second  ) );

    //it=tempParamList.find("CenterPoint");
    //if (it!=it_e)
    //tmpNode->setCenterPoint(  StrToVector( (*it).second  ) );

    printf("get first child...\n");

    DataLeaf childLeaf=parser->GetFirstChildLeaf(parent);
    INode *childNode;

    if (childLeaf!=NULL)
    {
        do
        {
            std::string childLeafName = parser->GetLeafName(childLeaf); // читаем имя дочернего узла

            // если мы попали на описание модификаторов - добавляем модификаторы
            if (childLeafName=="ModifierPack")
            {

                IModifierPack* nodeModPack = (IModifierPack*)tmpNode->GetPtrParam("ModifierPack");
                DataLeaf modLeaf=parser->GetFirstChildLeaf(childLeaf);
                if (modLeaf!=NULL)
                {

                    INodeModifier *myModifier = (INodeModifier *)ENGINE->CreateObject( parser->GetLeafName(modLeaf) );

                    asn_ext_param_list* modParamList= createList();
                    parser->GetLeafParamList(modLeaf,modParamList);

                    for( UINT i=0; i<modParamList->nums; ++i)
                    {

                        if ( strcmp(modParamList->param_list[i].name,"Type")==0)
                        {}
                        else if ( strcmp(modParamList->param_list[i].name,"BaseClass")==0)
                        {}
                        else
                            myModifier->SetParam(modParamList->param_list[i].name, modParamList->param_list[i].value );
                    }

                    deleteList( modParamList );


                    nodeModPack->AddModifier(myModifier);

                }
                while ( (modLeaf=parser->GetNextSiblingLeaf(modLeaf))!=NULL)
                    ;

            }
            else
            {

                childNode=Leaf2Data(parser,childLeaf);

                tmpNode->addNode(childNode);

            }
        }
        while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL);

    };

    deleteList( tempParamList );

    return tmpNode;
};
*/

UINT CWorld::AddCamera(const char* camType)
{

    ICamera *myCamera = dynamic_cast<ICamera*>( ENGINE->CreateObject(camType) );
    if (!myCamera){
        printf("[false]\n");
        return 0;
    }

    UINT curCamNum = CameraList.AddItem( myCamera );
    CBaseObject::AddChild( myCamera );

    return curCamNum;

};

UINT CWorld::AddCamera(ICamera* cam){

    if (!cam)
        return 0;

    cout <<  "CWorld::addCamera\n" << endl;
    UINT curCamNum = CameraList.AddItem( cam );
    //cout <<  "CWorld::addCamera OK1\n" << endl;
    CBaseObject::AddChild( cam );
    //cout <<  "CWorld::addCamera OK2\n" << endl;

    return curCamNum;

};

ICamera* CWorld::GetCamera(UINT id)
{
    return (ICamera*) CameraList.GetItem( id );
};

bool CWorld::DelCamera(UINT id)
{

    ICamera *myCamera=(ICamera*)CameraList.GetItem(id);
    if (myCamera)
    {
        myCamera->Release();
        CameraList.DelItem(id);
        return false;
    }

    return true;

};

/*
// добавляем параметры ноды
void CWorld::Node2Leaf(IDataParser* parser, DataLeaf parent, INode *node)
{

    std::string nodeName;

    asn_List delList; // не пишем в структуру то, что содержится здесь

    if ( std::string(node->GetParam("BaseType"))==ASN_NOT_FOUND_PARAM)
        nodeName = node->GetType();
    else
    {
        // проверяем что надо заносить в структуру, а что - уже есть в BaseType
        nodeName=std::string(node->GetParam("BaseType"));
        StrToList( BaseObjectList[nodeName], delList );

        asn_List::iterator it=delList.begin();

    }

    // два этих поля заполняются автоматически ( а BaseType помещаем в качестве имени ноды )
    delList["Type"]="-";
    if ( std::string(node->GetName())=="-")
        delList["Name"]="-"; //если имя не установлено - тоже добавляем в список удаления
    delList["BaseClass"]="-";
    delList["BaseType"]="-";

    DataLeaf tempLeaf=Object2Leaf(parser,parent,node, nodeName, delList);

    parser->SetParam(tempLeaf,"Pos", VectorToStr(node->getPos()).c_str() );
    parser->SetParam(tempLeaf,"Rot", VectorToStr(node->getRot()).c_str() );
    parser->SetParam(tempLeaf,"Size", VectorToStr(node->getSize()).c_str() );
    parser->SetParam(tempLeaf,"CenterPoint", VectorToStr(node->getCenterPoint()).c_str() );


    /*

    //    std::string nodeIDName = "Node_"+IntToStr( node->getID() );


    DataLeaf tempLeaf=parser->AddLeafWithName(parent, nodeName.c_str());

    //                    if ( std::string(node->GetName())!="-") parser->SetParam(tempLeaf,"Name", node->GetName() );
    //                    parser->SetParam(tempLeaf,"Type", node->GetType() );
    //                    parser->SetParam(tempLeaf,"BaseClass", node->GetBaseClass().c_str() );

    asn_ext_param_list* tempParamList= new asn_ext_param_list();
    node->getParamList(tempParamList);

    //asn_List tempParamList=node->getParamList();
    //asn_List::iterator it=tempParamList.begin();

    asn_List::iterator dl_it=delList.end();

    for( UINT i=0; i<tempParamList->nums; ++i)
    {

        if ( delList.find( tempParamList->param_list[i].name ) == dl_it )
            parser->SetParam(tempLeaf, tempParamList->param_list[i].name , tempParamList->param_list[i].value );
        //it++;
    };

    deleteList( tempParamList );



    IModifierPack* nodeModPack = (IModifierPack*)node->GetPtrParam("ModifierPack");

    if ( nodeModPack->GetModifierCount()>0 )
    { // если число модификаторов хотя бы 1

        DataLeaf PackLeaf=parser->AddLeafWithName(tempLeaf, "ModifierPack" /*nodeModPack->GetType() );

        std::string modName;

        for (UINT i=0; i<nodeModPack->GetModifierCount(); i++)
        {
            INodeModifier *myMod=nodeModPack->GetModifier(i);

            if (myMod)
            {

                delList.clear();

                delList["Type"]="-";
                if ( std::string(myMod->GetName())=="-")
                    delList["Name"]="-"; //если имя не установлено - тоже добавляем в список удаления
                delList["BaseClass"]="-";
                delList["BaseType"]="-";

                modName=myMod->GetType();

                Object2Leaf(parser,PackLeaf, myMod, modName, delList);

            }
            //myMod->Release();
        }

    };

    //получаем список нод у нашей ноды
    asn_ext_ptr_list* NodesListInRoot= createPtrList();
    node->getNodeList(NodesListInRoot);

    //NodeList_typ NodesListInRoot=node->getNodeList();
    //NodeList_typ::iterator it_=NodesListInRoot.begin();

    // а теперь каждую ноду переводим в данные и цепляем к корневой
    for( UINT i=0; i<NodesListInRoot->nums; i++)
    {

        Node2Leaf(parser, tempLeaf, ( INode*)NodesListInRoot->ptr_list[i].pointer );

        //it_++;
    };

    deletePtrList( NodesListInRoot );
};
*/

CWorld::CWorld(IEngine *_Engine):CBaseObject(_Engine)
{

    Engine=_Engine;

    SetParam("BaseClass","World");
    SetParam("Type","World");
    SetParam("Name","-");

    SetPtrParam("World", this);

    root_node=NULL;

//    if (!Engine) cout << "Engine is null!" << endl;

    LOGGER->LogMsg("+CWorld");

//    cout << "+CWorld" << endl;
//    WorldPhysicManager = NULL;
//    WorldPhysicManager = (IPhysicManager*)ENGINE->CreateObject("PhysicManager");
//    SetPtrParam("PhysicManager",WorldPhysicManager);

};


CWorld::~CWorld()
{

/*
//    for (UINT i=0; i<CameraList.GetSize(); i++)
//    {
//        ICamera *myCamera=(ICamera*)CameraList.GetItem(i);

//        if (myCamera)
//            myCamera->Release();
//    }

printf("delete world\n");

    //std::vector<INode*>::iterator it=NodeList.begin();

    for (UINT i=NodeList.size(); i>0; i--)
    {
        if ( (*it)==NULL)
        { printf("null node\n");} // пустая нода, ничего не делаем
        else
//            (*it)->Release(); // удаляем ноду
        it++;
    };

printf("delete world 2\n");

//    if (root_node)
//        root_node->Release(); //удаляем корневую ноду

//    if (WorldPhysicManager) WorldPhysicManager->ReleaseManager();
*/

    LOGGER->LogMsg("-CWorld");

};


bool CWorld::Draw()
{

//    printf("try draw..\n");
/*
    if(root_node)
    {

        bool cam_empty=true;

        for (UINT i=0; i<CameraList.GetSize(); i++)
        {
            ICamera *myCamera=(ICamera*)CameraList.GetItem(i);

            if (myCamera)
            {

                myCamera->View();
*/

	if( root_node )
                root_node->Draw();
/*
                cam_empty=false;
            }
        }

        if (cam_empty) LOGGER->ErrMsg(3, "CWorld:: Camera list is empty!");
//	printf("draw world ok\n");
        return false;
    }
    else LOGGER->ErrMsg(3, "CWorld:: Emply world, don't have a root node!");
*/
    return true;

};

bool  CWorld::Update(float tms)
{

//    WorldPhysicManager->Update(tms);

    if(root_node)
    {
        root_node->Update(tms);
        return false;
    }

    return true;
};

/*UINT CWorld::createNode( const char* InitString )
{

    IBaseObject* my_obj = (IBaseObject*)ENGINE->CreateObject( InitString );
    if (!my_obj) return 0;

    INode* myNode = dynamic_cast<INode*>(my_obj);//->AsType("Node");
    if (!myNode) return 0;

*/
/* ЧУЕ ЬФП ФЕРЕТШ Ч Engine
    asn_List::iterator it=BaseObjectList.find(InitString);

    if ( it!=BaseObjectList.end() )
    {
        /// мы нашли BaseObject с таким именем. Загружаем BaseObject
        std::string BaseString = (*it).second;
        //    printf("Find base: %s\n",InitString);

        asn_List tmpList;
        StrToList( BaseString, tmpList );

        if ( tmpList.find("Type")==tmpList.end() )
            LOGGER->ErrMsgStr(2,"Can't find TYPE param in baseObject "+ASN_Str(InitString)+"!");

        std::string Type=tmpList["Type"];
        //    printf("Find base type: %s\n",Type.c_str());

        // создаём по указанному в BaseType типу объект
        INode *myNode = (INode *)ENGINE->CreateObject(Type.c_str());
        if (!myNode)
            return 0;
        //    printf("Ok...Node created!\n");

        myNode->SetParam("BaseType", InitString );

        asn_List::iterator it=tmpList.begin();
        for( UINT i=0; i<tmpList.size(); ++i)
        {
            if ( ((*it).first)=="Pos" )
                myNode->setPos( StrToVector( ((*it).second) ) );
            else if ( ((*it).first)=="Rot" )
                myNode->setRot( StrToVector( ((*it).second) ) );
            else if ( ((*it).first)=="Size" )
                myNode->setSize( StrToVector( ((*it).second) ) );
            else if ( ((*it).first)=="CenterPoint" )
                myNode->setCenterPoint( StrToVector( ((*it).second) ) );
            else if ( ((*it).first)!="Type" )
                if ( ((*it).first)!="BaseClass" )
                    myNode->SetParam(((*it).first).c_str(), ((*it).second).c_str() );
            it++;
        };


        addNode(myNode);
        //    printf("Ok...Node %d added into World!\n", myNode->getID() );
        return myNode->getID();
    };

    ////////////////// пока закомментарим это ///////////////////
    // сначала пробуем перевести InitString в список
    //asn_List tmpList;
    //StrToList(InitString, tmpList);
    //    printf("InitString Size: %d\n",tmpList.size());
    //    asn_List::iterator it=tmpList.begin();
    //    for( UINT i=0; i<tmpList.size(); ++i){
    //    printf("%s=%s\n",((*it).first).c_str(), ((*it).second).c_str());
    //    it++;
    //    };
    ////////////////// пока закомментарим это ///////////////////

    INode *myNode = (INode *)ENGINE->CreateObject(InitString);

    if (myNode)
        addNode(myNode);
    else
    {
        LOGGER->ErrMsgStr(2,"Can't create "+std::string(InitString)+" object :(!");
        return 0;
    };

*/
//    return addNode(myNode);

//};

bool CWorld::AddNode(INode* node)
{

//    IWorld *nodeWorld = (IWorld*) node->GetPtrParam("World");

//    if ( nodeWorld==this )
//        return false;//node->getID(); // есть такое уже у нас! :-)

//    if (nodeWorld)
//    { // если это пришелец из другого мира - удалим его оттуда :)
//        nodeWorld->delNode( node->getID() );
//    };

    //node->getParent();
//    printf("try add node to world...\n");

//    if ( node->getParent() )
//        node->getParent()->delNode(node); //если у ноды есть уже родитель - удаляем оттуда ноду

//    printf("try add node to world... %s\n",node->GetParam("Name"));

    // если имя у ноды не пустое
/*
    if (std::string(node->GetName())!="-")
    {
        if ( NodeNameList.find(node->GetName())==NodeNameList.end() )
        {
            NodeNameList[node->GetName()]=node;
            //printf("Add node name %s in name list\n",node->GetName());
        }
        else
        {
            LOGGER->ErrMsgStr(3,"Node in the wolrd already have name"+std::string(node->GetName())+"!");
            return 0; //мы не можем добавить ноду с таким именем, т.к. такая нода уже есть
        }
    }
*/

    node->SetPtrParam("World",this); //устанавливаем ссылку на менеджер мира :)

    if (!root_node)
    {
	printf("add root node\n");
        root_node=node; /*NodeList.push_back( node );*/
        CBaseObject::AddChild( node );
        return true; // установили корневую ноду
    }

/*
    for( UINT i=0; i<NodeList.size(); ++i)
    {

        if(NodeList[i]==NULL)
        {
            //   printf("FIND!!! node %d\n", i + 1);
//            node->setID(i + 1);
            NodeList[i] = node;
//            root_node->addNode(node); //добавляем ноду к корневой ноде
            //   fprintf(stdout,"CWORLD: add node %d\n", i + 1);
            return (i + 1);
        }
    }
*/

//    NodeList.push_back( node );

//    node->setID(NodeList.size());
    root_node->AddChild(node); //добавляем ноду к корневой ноде
    // fprintf(stdout,"CWORLD: add node %d\n", NodeList.size());

//    CBaseObject::addChild( node );
    return true;

};

/*
bool CWorld::delNode( UINT id )
{
  printf("CWorld: del node: \n",id);

    if ( IDisValid(id) )
    {
*/

//    printf("ola-la!\n");

/*
	INode* node = (*NodeList.begin());//[id-1];
	if (!node) printf("SSSSSSSSSSSSSS!!!!!!!!!!\n");


	printf("id is valid %s..\n", node->GetType() );

        printf("ola-la!\n");

        // если имя у ноды не пустое
        if ( std::string(NodeList[id-1]->GetName())!="-")
        {
            // удаляем из списка именованых нод
	    //printf("erase from NodeNameList\n");
            NodeNameList.erase( NodeNameList.find( NodeList[id-1]->GetName() ) );
        }

        // если нода удаляется (ID=0) то вызов деструктора не требуется, иначе - удаляем ноду
*/
//	printf( "id: %d\n", NodeList[id-1]->getID() );
//	printf( "id: %s\n", NodeList[id-1]->GetParam("Name") );
/*
        if ( NodeList[id-1]->getID()!=0 )
            NodeList[id - 1]->Release();
        NodeList[id - 1] = NULL;
        printf("CWORLD: del node %d\n",id);
        return false;
    }

    printf("id is not valid!\n");
    return true;

};

/*
UINT CWorld::getMaxID()
{
    return NodeList.size();
};
*/

/*
UINT CWorld::getNodesNum()
{
    UINT ret=0;
    for( UINT i=0; i<NodeList.size(); ++i)
    {
        if(NodeList[i]!=NULL)
            ret++;
    }
    return ret;
};
*/

/*
INode *CWorld::getNode( UINT id )
{
    if ( IDisValid(id) )
        return NodeList[id-1];
    else
        return NULL;
};
*/

/*
INode *CWorld::findNode(const char*NodeName)
{

    std::map<std::string,INode*>::iterator _it=NodeNameList.find(NodeName);

    if ( _it==NodeNameList.end() )
        return NULL;
    return (*_it).second;

};
*/

/*
//// проверяем ID на правильность
bool CWorld::IDisValid( UINT id )
{
    if( id == 0 )
        return false;
    UINT index = id - 1;
    if( index >= (UINT)NodeList.size() || NodeList[index] == NULL )
    {
	return false;
    }

//    printf("check id = %d is valid\n", index);
    return true;

};
*/

/*
CVector HitPos[2];

// нода проткнулась отрезком  :)
inline bool NodeInCut(INode *myNode, const CVector& Start, const CVector& End)
{
    CVector Pos=myNode->getPos();
    CVector Rot=myNode->getRot();
    //    printf("Rot %f %f %f\n",Rot.v[0],Rot.v[1],Rot.v[2]);
    CVector Size=myNode->getSize();
    CVector CenterPoint=myNode->getCenterPoint();

    CBoundingBox box;
    box.EnlargeMe(Size);

    CVector tmp_1=End-Pos;
    CVector tmp_2=Start-Pos;

    tmp_1.RotateX( -Rot.v[0] );
    tmp_1.RotateY( Rot.v[1] );
    tmp_1.RotateZ( -Rot.v[2] );

    tmp_2.RotateX( -Rot.v[0] );
    tmp_2.RotateY( Rot.v[1] );
    tmp_2.RotateZ( -Rot.v[2] );

    if (box.CheckIntersect(tmp_1,tmp_2,HitPos[0],HitPos[1]))
        return true;

    return false;
    //    printf("[%d] Mouse hit pos(1): %f %f %f \n",myNode->getID(),HitPos[0].v[0],HitPos[0].v[1],HitPos[0].v[2]);
    //    printf("[%d] Mouse hit pos(2): %f %f %f \n",myNode->getID(),HitPos[1].v[0],HitPos[1].v[1],HitPos[1].v[2]);

};

void CWorld::getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)
{

    clearPtrList( list );
    //NodeListInCut.clear();

    // пока перебираем все ноды мира.. потом - будем оптимизировать
    for( UINT i=0; i<NodeList.size(); ++i)
    {

        if(NodeList[i]!=NULL)
        {
            //если нода не пустая - проверяем протыкает её отрезок или нет
            if ( NodeInCut(NodeList[i],Start,End) )
                addToPtrList(list,"node",NodeList[i]);
            //                NodeListInCut.push_back( NodeList[i] );
        }
    }


    //return NodeListInCut;
};
*/

//bool CWorld::LoadWorld(const char* FileName)
//{

//}

/*
    printf("Try load %s world\n",FileName);

    IFileSystem *FS=((IFileSystem*)ENGINE->GetPtrParam("FileManager"));
    printf("Try load %s world\n",FileName);

    IFile *myFile=FILER->GetFile( FileName );

    if (!myFile)
    {
        LOGGER->ErrMsgStr(3,"File "+std::string(FileName)+" not found :(\n");
        return true; //файла нет такого
    };
    myFile->FileOpen();
    void *data=myFile->LoadInMem();
    if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM)
    {
        LOGGER->ErrMsgStr(3,"File "+std::string(FileName)+" can't load in mem :(\n");
        return true; // не грузится он :(
    };
    printf("File size: %d",myFile->GetSizeInBytes());

    //// пока так, а потом посмотрим...
    IDataParser* parser=(IDataParser *)ENGINE->CreateObject("DataParserXML");
    if (!parser)
    {
        LOGGER->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        return true;
    };

    /// получаем корень данных
    DataLeaf root=parser->StartParse(data);
    //printf("leaf name: %s\n",parser->GetLeafName(root) );

    DataLeaf childLeaf=parser->GetFirstChildLeaf(root);
    if (childLeaf!=NULL)
        do
        {
            std::string LeafName=parser->GetLeafName(childLeaf);
            printf("leaf name: %s\n",parser->GetLeafName(childLeaf) );

            if (LeafName=="World") // загружаем мир
                loadWorld(parser,childLeaf);
            else if (LeafName=="BaseObject") // загружаем базовые объекты
                loadBaseObjects(parser,childLeaf);
            else if (LeafName=="file")
            { // загружаем еще один файл

		printf("parse file leaf\n");
                asn_ext_param_list* tmpList= createList();
		printf("tmpList created..\n");
                parser->GetLeafParamList(childLeaf,tmpList);
		printf("param list ok\n");
                asn_List tempParamList;
                StrPtrToList( tmpList, tempParamList );
                deleteList( tmpList );
		printf("end parse file leaf\n");
                LoadWorld( tempParamList["Name"].c_str() );

            }
            else
                LOGGER->ErrMsgStr(4,"Hmmm.. Can't parse "+LeafName+" section \n");

        }
        while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL);

    delete parser;
    myFile->FileClose();
    return false;
*/
//};

/*
void CWorld::loadBaseObjects(IDataParser* parser, DataLeaf parent)
{
    // загружаем из парсера базовые объекты

    DataLeaf childLeaf=parser->GetFirstChildLeaf(parent);
    if (childLeaf!=NULL)
        do
        {
            //asn_List tempParamList=parser->GetParamList(childLeaf);

            asn_ext_param_list* tmpList= createList();
            parser->GetLeafParamList(childLeaf,tmpList);
            asn_List tempParamList;
            StrPtrToList( tmpList, tempParamList );
            deleteList( tmpList );

            // получаем имя базового объекта
            std::string ObjName=parser->GetLeafName(childLeaf);
            printf("Leaf name: %s\n",ObjName.c_str());
            // конвертим параметры в строку, т.к. хранить в виде списка влом мне :)
            std::string InitString=ListToStr(tempParamList);
            // обновляем или добавляем новый BaseObject
            SetBaseObject( ObjName.c_str(), InitString.c_str() );
        }
        while ( (childLeaf=parser->GetNextSiblingLeaf(childLeaf))!=NULL);

};

void CWorld::loadWorld(IDataParser* parser, DataLeaf parent)
{
    // загружаем из парсера "мир" объекты

    /// получаем нулевую ноду
    DataLeaf firstLeaf=parser->GetFirstChildLeaf(parent);

    //  printf("Start leaf name: %s\n",parser->GetLeafName(firstLeaf) );
    //  system("PAUSE");

    /// удаляем корневую ноду, если она есть
    if (root_node)
    {
        root_node->Release();
        root_node=NULL;
    }

//    printf("delete physic manager..\n");

//    if (WorldPhysicManager) WorldPhysicManager->ReleaseManager();
//    WorldPhysicManager = (IPhysicManager*)ENGINE->CreateObject("PhysicManager");
//    SetPtrParam("PhysicManager",WorldPhysicManager);

    root_node=Leaf2Data(parser, firstLeaf);
//
	printf("root node ok");
};
*/

//bool CWorld::SaveWorld(const char*FileName)
//{

//}

//return false;
/*

    //// пока так, а потом посмотрим...
    IDataParser* parser=(IDataParser *)ENGINE->CreateObject("DataParserXML");
    if (!parser)
    {
        LOGGER->ErrMsg(3,"Sorry, DataParserXML plugin not found :(\n");
        return true;
    };

    // создаём документ
    DataLeaf root=parser->CreateRootLeaf();

    // создаём раздел BaseObject
    // DataLeaf tempLeaf=parser->AddLeafWithName(root, "BaseObject");

    // создаём раздел World
    DataLeaf tempLeaf=parser->AddLeafWithName(root, "World"/*this->GetType());

    // вызываем рекурсивную функцию перевода нод в цифровую форму (см. начало файла этого, да?)
    Node2Leaf(parser, tempLeaf , root_node );

    // и сохраняем то, что напарсили в файл с указаным именем
    parser->SaveDataTree( FileName );

    delete parser;
    return false;
    */

//};

/*
bool CWorld::SetBaseObject(const char *ObjName, const char *InitString)
{
    BaseObjectList[ObjName]=InitString;
    printf("Base object %s set init: %s\n",ObjName,InitString);
};
*/

/*
const char *CWorld::GetBaseObjectParam(const char *ObjName, const char *ParamName)
{
    return ASN_NOT_FOUND_PARAM;
};
*/

ASNInterface* CWorld::GetInterface(const char* interface_name){
//void* CWorld::GetInterface(const char* interface_name){
   LOGGER->ErrMsgStr(2, std::string("CWorld can't give ")+interface_name+" interface!");
       return NULL;
};

bool CWorld::AddChild(IBaseObject* child){

	std::string myTc = child->GetBaseClass();

//printf( "%",myFil

	if ( myTc == "Camera") {

	    ICamera* myCam = dynamic_cast<ICamera*>( child );//->GetPtrParam("Camera");
	    if (!myCam) {printf("{2 is null}[error!]\n");return false;}

	    AddCamera( myCam );

	    printf("[ok]\n");
	    return true;
	}

    if ( myTc == "Node") {

	    INode* myNode = dynamic_cast<INode*>( child ); //->GetPtrParam("Node");
	    if (!myNode)  {printf("{2 is null}[error!]\n");return false;}

	    AddNode( myNode );
//	    printf( "%d", myNode->getID() );
        //CBaseObject::addChild( myNode ); // не надо тут, пусть все цепляется к корневой ноде

	    printf("[ok]\n");
	    return true;
	}

    return false;

};

bool CWorld::DelChild(UID childID){

CBaseObject_map::iterator it_ = Childs.find(childID);
if ( it_==Childs.end() ) return false;

   	std::string myTc = (*it_).second->GetBaseClass();

///////// TEMPORARY !! /////////////////
    if (myTc == "Camera" ){
        ICamera* myCam = dynamic_cast<ICamera*>( (*it_).second );
	    if (myCam)
	    {
            for (UINT i=0; i<CameraList.GetSize(); i++)
            {
                ICamera *myCamera=(ICamera*)CameraList.GetItem(i);

                if (myCamera==myCam) DelCamera(i);
            }

	    }
    }

    CBaseObject::DelChild(childID);

}
