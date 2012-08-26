#include "IEngine.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#include "CNode.h"

#include "INodeModifier.h"
//#include "GL/gl.h"

#include "ASN_utils.h"

#include "IDevice.h"
#include "ILog.h"

#include "IWorld.h"

CNode::CNode(IEngine *_Engine):CBaseObject(_Engine){
//Engine=_Engine;
//myObject= new BaseObject(_Engine);

CenterPoint=CVector( 0.5,0.5,0.5 );
SetParam("CenterPoint","0.5 0.5 0.5");

Size=CVector( 1.0f,1.0f,1.0f);
SetParam("Size","1.0 1.0 1.0");

out_Rot=CVector( 0,0,0 );
SetParam("Rot","0 0 0");

out_Pos=CVector( 0,0,0 );
SetParam("Pos","0 0 0");

NodeMatrix.LoadIdentity();

SetParam("BaseClass","Node");
SetParam("Type","NodeBase");
//SetParam("Name","-");

SetPtrParam("Node", this);


// создаём "упаковку" модификаторов
IModifierPack* myPack=dynamic_cast<IModifierPack*>(_Engine->CreateObject("ModifierPack"));
if (myPack) SetPtrParam("ModifierPack",myPack);

//Parent=NULL;
//id=0;
load_all=false;
//_node_it=NodeList.begin();
NodeColor = NULL;
};

///  ЧЩОЕУФЙ ЮБУФШ Ч release
// ВМБЗПДБТА ФЕВС, зПУРПДЙ....

CNode::~CNode(){

//printf("CNode: delete node %d\n",id);

//if (Parent) Parent->delNode(this); //отсоединяем от родительской ноды

// получаем ссылку на узел сцены
//IWorld *world=(IWorld*)GetPtrParam("World");
//if ( (world==NULL) & (id!=0) )
//    LOGGER->ErrMsgStr(3, "Hmm.. node "+IntToStr( id ) + " have a ID but not have a link to world manager" );

//UID old_id=id; //запоминаем текущий ID
//id=0; //присваиваем ID 0, т.е. иначе менеджер мира снова вызовет этот деструктор

//printf("CNode: delete node %d\n",old_id);

// если у нас нода была зарегистрирована в менеджере:
//if (old_id>0) {
//    printf("del node: %d\n",old_id);
//    world->delNode(old_id); //удаляем её оттуда
//    printf("del node ok: %d\n",old_id);
//    };

printf("delete node from world ok\n");

//std::vector<INode*>::iterator it=NodeList.begin();

//printf("delete from node list\n");

/*
for (UINT i=NodeList.size(); i>0; i--){
    INode *nodeForDel = (*it);
    printf("try remove node...\n");
    delNode(nodeForDel); //отсоединяем у себя ноду, она исчезает из списка и Parent её выставляется в NULL
    printf("try release node...\n");
    if (nodeForDel) nodeForDel->Release(); //удаляем ноду
    printf("try begin node...\n");
    it=NodeList.begin(); //и на начало
};
*/

// удаляем "упаковку" модификаторов
IModifierPack* myPack=(IModifierPack*)GetPtrParam("ModifierPack");
if (myPack) myPack->Release();

//fprintf(stdout,"good bye node %d...\n",old_id);
  printf("Delete CNode\n");
};

bool CNode::Draw(){

//printf("CNode: draw\n");

double Model_Matrix[16];
double Project_Matrix[16];

DEVICER->PushMatrix();
DEVICER->MultModelMatrix(NodeMatrix.m_data);
DEVICER->GetMatrixs(Model_Matrix, Project_Matrix);

ShowSize();

DEVICER->SetMatrixs(Model_Matrix, Project_Matrix);

CBaseObject_map::iterator it=Childs.begin();
for (UINT i=Childs.size(); i>0; i--){
    INode* pNode = dynamic_cast<INode*>((*it).second);
    if (pNode) pNode->Draw();
    DEVICER->SetMatrixs(Model_Matrix, Project_Matrix);
    it++;
}

//std::vector<INode*>::iterator it=NodeList.begin();
//for (UINT i=NodeList.size(); i>0; i--){

//    (*it)->Draw();
//    DEVICER->SetMatrixs(Model_Matrix, Project_Matrix);
//    it++;
//};

DEVICER->PopMatrix();

};

char CNode::Update(float tms){

CBaseObject_map::iterator it=Childs.begin();
for (UINT i=Childs.size(); i>0; i--){
    INode* pNode = dynamic_cast<INode*>((*it).second);
    if (pNode) pNode->Update(tms);
    it++;
}

//printf("update...\n");

//std::vector<INode*>::iterator it=NodeList.begin();

//for (UINT i=NodeList.size(); i>0; i--){
//printf( "%d \n", (*it)->getID());
//если нода не удалена, то передвигаем счетчик на 1
//если удалена - то счетчик сам передвинулся
//    if ((*it)->Update(tms)!=2) it++;
//};

IModifierPack* myPack=(IModifierPack*)GetPtrParam("ModifierPack");
myPack->Modify( this, tms );

//printf("update ok\n");

};

/*
bool CNode::addNode(INode* node){

if (!node) return true;

fprintf(stdout, "CNode: addNode %d\n",node->getID());

//if (node->getParent()==this) {
//return true; // нода уже присоединена к этой ноде
//LOGGER->ErrMsgStr(3,"Node #ID "+IntToStr(node->getID())+" already connected into #ID "+IntToStr(this->getID()));
//}

//if (node->getParent()) {// если нода уже присоединена к чему-то, т.е. Parent не NULL
//node->getParent()->delNode(node); // удаляем её оттудова
//}

//NodeList.push_back(node);//добавляем её себе в список имеющихся
//node->setParent(this); //устанавливаем у неё родителя

//if (node->getParent()!=this) { // установить не удалось почему-то
//NodeList.erase( NodeList.end()-1 );
//LOGGER->ErrMsgStr(2,"Can't add node "+IntToStr(this->getID())+" <-- "+IntToStr(node->getID()));
//return true;
//};

LOGGER->LogMsgStr("Node #ID "+IntToStr(node->getID())+" added into #ID "+IntToStr(this->getID()));
return false;
};
*/

/*
bool CNode::delNode(INode* node){

//fprintf(stdout, "delNode %d\n",node->getID());
/*
std::vector<INode*>::iterator it=NodeList.begin();
for (UINT i=NodeList.size(); i>0; i--){
//    printf( "%d ? = %d\n",(*it)->getID(),node->getID());
    if ( (*it)==node ) {
//        node->setParent(NULL); //устанавливаем paren у ноды в NULL
        NodeList.erase(it);  // удаляем её из своего списка
        LOGGER->LogMsgStr("Node #ID "+IntToStr(node->getID())+" removed from #ID "+IntToStr(this->getID()));
        return false;  // всё нормально - возвращаем false
        };
    it++;
};

LOGGER->ErrMsgStr(2,"Can't del node #ID:"+IntToStr(node->getID())+" from node #ID "+IntToStr(this->getID()));

return true;

};
*/

/*
void CNode::getNodeList(asn_ext_ptr_list* list){
    clearPtrList( list );

//    std::vector<INode*>::iterator it= NodeList.begin();
//    while (it!=NodeList.end()){
//        addToPtrList( list, "-", (*it) );
//        it++;
//    }

};
*/

void CNode::SetSize(const CVector& _Size){

//NodeMatrix.Scale(_Size);
Size=_Size;
	CBaseObject::SetParam("Size", VectorToStr(Size).c_str() );
};

void CNode::SetCenterPoint(const CVector& _CenterPoint){
    CenterPoint=_CenterPoint;
	CBaseObject::SetParam("CenterPoint", VectorToStr(CenterPoint).c_str() );
};

void CNode::SetPos(const CVector& _Pos){
	NodeMatrix.m_data[12] = _Pos.v[0];
	NodeMatrix.m_data[13] = _Pos.v[1];
	NodeMatrix.m_data[14] = _Pos.v[2];
	out_Pos=_Pos;
	CBaseObject::SetParam("Pos", VectorToStr(out_Pos).c_str() );
};

void CNode::SetRot(const CVector& _Rot){

//out_Rot-=_Rot; // вычисляем изменение угла
	out_Rot=_Rot;

if (out_Rot.v[0]>360.0) out_Rot.v[0]-=360.0f;
if (out_Rot.v[1]>360.0) out_Rot.v[1]-=360.0f;
if (out_Rot.v[2]>360.0) out_Rot.v[2]-=360.0f;

if (out_Rot.v[0]<0) out_Rot.v[0]+=360.0f;
if (out_Rot.v[1]<0) out_Rot.v[1]+=360.0f;
if (out_Rot.v[2]<0) out_Rot.v[2]+=360.0f;

	CVector	temp = CVector( NodeMatrix.m_data[12], NodeMatrix.m_data[13], NodeMatrix.m_data[14]);
	NodeMatrix.RotateXYZ(_Rot);
	NodeMatrix.m_data[12] = temp.v[0];
	NodeMatrix.m_data[13] = temp.v[1];
	NodeMatrix.m_data[14] = temp.v[2];

	CBaseObject::SetParam("Rot", VectorToStr(out_Rot).c_str() );
}

const CVector& CNode::GetSize(){
return Size;
}

const CVector& CNode::GetCenterPoint(){
return CenterPoint;
}

const CVector& CNode::GetPos(){
return out_Pos;
}

const CVector& CNode::GetRot(){
return out_Rot;
}

/*
UINT CNode::getID()
{
return id;
}
*/

/*
INode *CNode::getParent(){
return Parent;
}
*/

/*
void CNode::setID(UINT _id)
{
id=_id;
}
*/

/*
void CNode::setParent(INode* _parent){
Parent=_parent;
}
*/

void CNode::SetParam(const char* param_name,const char* param_value){

    if ( strcmp(param_name, "Color") == 0 ) {
        // будем использовать цвет у ноды
        if (!NodeColor) NodeColor = new CVector4(1,1,1,1);
        if (!CheckStrToVector(param_value)) LOGGER->ErrMsg(2,"Color error!");
        CVector tmpColor = StrToVector(param_value);
        NodeColor->v[0]=tmpColor.v[0];
        NodeColor->v[1]=tmpColor.v[1];
        NodeColor->v[2]=tmpColor.v[2];

        printf("Set node color: %f %f %f %f\n",NodeColor->v[0],
                                 NodeColor->v[1],
                                  NodeColor->v[2],
                                  NodeColor->v[3]);
    }

    else if ( strcmp(param_name, "Alpha") == 0 ) {
        // будем использовать цвет у ноды
        if (!NodeColor) NodeColor = new CVector4(1,1,1,1);
        NodeColor->v[3] = StrToFloat(param_value);
    }
    else if ( strcmp(param_name, "Pos") == 0 ) {
        if (!CheckStrToVector(param_value)) LOGGER->ErrMsg(2,"Pos error!");
        CVector ParamValue=StrToVector(param_value);
        SetPos( ParamValue );
    }
    else if ( strcmp(param_name, "Rot") == 0 ) {
        if (!CheckStrToVector(param_value)) LOGGER->ErrMsg(2,"Rot error!");
        CVector ParamValue=StrToVector(param_value);
        SetRot( ParamValue );
    }
    else if ( strcmp(param_name, "Size") == 0 ) {
        if (!CheckStrToVector(param_value)) LOGGER->ErrMsg(2,"Size error!");
        CVector ParamValue=StrToVector(param_value);
        SetSize( ParamValue );
    }
    else if ( strcmp(param_name, "CenterPoint") == 0 ) {
        if (!CheckStrToVector(param_value)) LOGGER->ErrMsg(2,"CenterPoint error!");
        CVector ParamValue=StrToVector(param_value);
        SetCenterPoint( ParamValue );
    }

    CBaseObject::SetParam(param_name,param_value);
};


void CNode::ShowSize(){

if ( std::string(GetParam("ShowSize"))=="1") {

//    printf("show size\n");
    DEVICER->SetScale( Size );
    IDrawObject *myObj = DEVICER->GetDrawObject ("_Cube");
    DEVICER->SetMaterial( NULL );
    if (NodeColor) DEVICER->SetColor(*NodeColor);
    DEVICER->Draw( myObj );

};

}

//std::string CNode::GetBaseClass(){
//return GetParam("BaseClass");
//};

//std::string CNode::GetType() {
//return GetParam("Type");
//};

//std::string CNode::GetName() {
//return GetParam("Name");
//};


/*
void CNode::SetPtrParam(const std::string& param_name, void *param_value){
VoidContent[param_name]=param_value;
};

void *CNode::GetPtrParam(const std::string& param_name) {
asn_ptrList::iterator v_it=VoidContent.begin();
VoidContent.find(param_name);
if (v_it!=VoidContent.end()) return (*v_it).second;
else return NULL;
};

void CNode::DelPtrParam(const std::string& param_name){
asn_ptrList::iterator v_it=VoidContent.find(param_name);
if (v_it!=VoidContent.end()) VoidContent.erase(v_it);
};

void CNode::SetParam(const std::string& param_name,const std::string& param_value){
Content[param_name]=param_value;
};

void CNode::SetCharParam(const char *param_name,const char *param_value){
Content[std::string(param_name)]=std::string(param_value);
};

const std::string& CNode::GetParam(const std::string& param_name) {

std::map<std::string,std::string>::iterator it=Content.find(param_name);
if (it!=Content.end()) return (*it).second;
else return ASN_NOT_FOUND_PARAM_STR;
};

void CNode::DelParam(const std::string& param_name){
std::map<std::string,std::string>::iterator it=Content.find(param_name);
if (it!=Content.end()) Content.erase(it);
};
*/
bool CNode::AddChild(IBaseObject* child){

    std::string myTc = child->GetBaseClass();
	if ( myTc == "Node") {

	    INode* myNode = dynamic_cast<INode*>(child);//->GetPtrParam("Node");
	    if (!myNode)  {printf("{2 is null}[error!]\n");return false;}

//	    INode* myNodeP = (INode*)parent->GetPtrParam("Node");
//	    if (!myNodeP) {printf("{1 is null}[error!]\n");return false;}

//	    myNodeP->
	    //addNode( myNode );
        CBaseObject::AddChild( myNode );

	    printf("[ok]\n");
	    return true;
	}

	return false;

}
