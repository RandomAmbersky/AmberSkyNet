// пивовар Иван Таранов
// пивовар Иван Таранов
// обожал бомбить баранов
// как напьется пива "Пит"
// так на пастбище летит

#include "IEngine.h"

#include "CWorldSimple3D.h"
//#include "IPluginFactory.h"

#include "IFileSystem.h"
#include "IDataParser.h"

#include "INodeModifier.h"

#include "CBoundingBox.h"

#include "ASN_utils.h"

#include "IDevice.h"
#include "ILog.h"
#include "INode.h"

#define ENGINE Engine //у нас уже есть указатель на Engine
//CNode3ds::CNode3ds(IEngine *_Engine):CNode(_Engine),CBaseObject(_Engine) {

#include <iostream>
using namespace std;

CWorldSimple3D::CWorldSimple3D(IEngine *_Engine):CWorld(_Engine),CBaseObject(_Engine){

cout << "CWorldSimple3D created " << endl;

    //Engine=_Engine;

    CBaseObject::SetParam("BaseClass","World");
    CBaseObject::SetParam("Type","WorldSimple3D");
    CBaseObject::SetParam("Name","-");
    root_node=NULL;

    SetPtrParam("World", this);

    LOGGER->LogMsg("+CWorldSimple3D");

};


CWorldSimple3D::~CWorldSimple3D()
{
//cout << "csilmlpe off" << endl;
    LOGGER->LogMsg("-CWorldSimple3D");
//cout << "csilmlpe off" << endl;
};

CVector HitPos[2];

// нода проткнулась отрезком  :)
inline bool NodeInCut(INode *myNode, const CVector& Start, const CVector& End)
{
    CVector Pos=myNode->GetPos();
    CVector Rot=myNode->GetRot();
    //    printf("Rot %f %f %f\n",Rot.v[0],Rot.v[1],Rot.v[2]);
    CVector Size=myNode->GetSize();
    CVector CenterPoint=myNode->GetCenterPoint();

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

class VisitorCut: public IBaseObjectVisitor {
public:

    VisitorCut(asn_ext_ptr_list* pList, const CVector& vStart, const CVector& vEnd):myList(pList),Start(vStart),End(vEnd){ }

    void Visit( IBaseObject* myObj){

    INode* myNode = dynamic_cast<INode*>( myObj );
    if (!myNode) return;

        if ( NodeInCut(myNode,Start,End) )
            addToPtrList(myList,"node",myObj);

    };

    void AfterVisit( IBaseObject* myObj){



    };

    asn_ext_ptr_list* myList;
    const CVector& Start;
    const CVector& End;

};



void CWorldSimple3D::GetNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)
{
    //printf("find mouse pointer...\n");

    clearPtrList( list );
    if (!root_node) return;

    VisitorCut myVis(list, Start, End);

    root_node->Visit( &myVis, false );

/*
    CBaseObject* root_base = dynamic_cast<CBaseObject*>(root_node);

    IBaseObject_map::iterator it_ = root_base->Childs.begin();
    int cnt = root_base->Childs.size();
    while (cnt>0) {
         INode* myNode = dynamic_cast<INode*>( (*it_).second );
         if (myNode)
         if ( NodeInCut(myNode,Start,End) ) {
            addToPtrList(list,"node",myNode);
         }
    it_++;
    cnt--;
    }
*/

//    NodeListInCut.clear();

    //printf("find mouse pointer...\n");

    // пока перебираем все ноды мира.. потом - будем оптимизировать

/*
    for( UINT i=0; i<NodeList.size(); ++i)
    {
        //printf("%d %d\n",i,NodeList.size());

        if(NodeList[i]!=NULL)
        {
            //если нода не пустая - проверяем протыкает её отрезок или нет
            if ( NodeInCut(NodeList[i],Start,End) ) {
                printf("find %s!\n",NodeList[i]->GetName() );
                addToPtrList(list,"node",NodeList[i]); }
            //                NodeListInCut.push_back( NodeList[i] );
        }
    }
*/

    //return NodeListInCut;
};

//class CWorld3D: public IWorld3D {
//    void getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list){
//        printf("NodesByPos \n");
//    };
//};

ASNInterface* CWorldSimple3D::GetInterface(const char* interface_name){
//void* CWorldSimple3D::GetInterface(const char* interface_name){
if ( strcmp(interface_name,"ASNIWorld3d") == 0 ) {
// printf("get interface %s...\n",interface_name);
 return dynamic_cast<ASNIWorld3D*>(this);//new CWorld3D();
    }
    return NULL;
};
