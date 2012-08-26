#include <stdlib.h>
#include "IScriptManager.h"
#include "IWorld.h"
#include "IDevice.h"
#include "IEngine.h"
#include "vector"

#define RANDOM(x) (rand() % (x))

#include <time.h>

extern IWorld *myWorld; // указатель на мир
extern IScriptManager* myAI;
extern IDevice *myGraphic;
extern IEngine *myEngine; // указатель на Engine для остальных модулей

#include "EngineStarter.h"

double currentFPS; // глобальное значение FPS, чтобы не обращаться каждый раз из процедуры

static char buffer[ 80 ];

const char *get_time(){
time_t time_of_day=time(NULL);
strftime( buffer, 80, "%H:%M:%S %s", localtime( &time_of_day ) );
return buffer;
};

//INode* myCurNode;// = (INode*) myAI->GetCurrentObject();

std::vector<INode*> roboList;

INode* CreateBaseNode(std::string NodeType) {
//    UINT tmpId=myWorld->createNode( NodeType.c_str() );
//    printf("CreateBaseNode ID: %d\n",tmpId);
//    return myWorld->getNode( tmpId );
    IBaseObject* myObj = dynamic_cast<IBaseObject*>( ENGINE->CreateObject( NodeType.c_str() ) );
    if (!myObj) return NULL;

    printf("create base node %s...\n", myObj->GetType() );

    INode* myNode = dynamic_cast<INode*>(myObj);//->AsType("Node");
    if (!myNode) { myObj->Release(); return NULL; }

    printf("create base node %s...\n",NodeType.c_str());

    myWorld->AddChild (myNode);
    return myNode;
};

/////////////////////////////////////////////////////////////////////////////////////
// установить случайным образом позицию, угол наклона и скорости движения-вращения
/////////////////////////////////////////////////////////////////////////////////////

void SetRandomParams(INode *myNode){
    if (!myNode) return;

    CVector Pos = CVector( RANDOM(90)+10, RANDOM(90)+10, 1 );
    myNode->SetPos(Pos);

//    cout << Pos.v[0] << "<==>" << Pos.v[1] << endl;

    CVector Rot = CVector( 0, 0, RANDOM(90)+10 );
    myNode->SetRot(Rot);

    myNode->SetParam("Brain","robo_brain.lua");
};

///////////////////////////////////////////////////////////////////////////////////
//////////////////////// привязка скриптов ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int AddRobot(void *Params){

//    cout << "add robot " <<endl;
    INode* myNode = CreateBaseNode("RobBot");
    SetRandomParams( myNode );
    roboList.push_back( myNode );

    return 0; // скриптовая функция ничего не возвращает

};

int DelRobot(void *Params){

    //cout << "del robot " <<endl;
    return 0;

};

int MoveForward(void *Params){

    int n = myAI->Script_gettop(Params);
    double dx = 100.0;


    if (n==1) dx = myAI->Script_tonumber(Params,1);

//    if (dx> 10.0) dx = 10.0; // ограничиваем макс. скорость


    INode* myNode = (INode*) myAI->GetCurrentObject();

    CVector Pos = myNode->GetPos();
    CVector Rot = myNode->GetRot();

    Pos.v[0]+=dx*cos(Rot.v[2]*GRAD_PI2)*currentFPS;
    Pos.v[1]+=dx*sin(Rot.v[2]*GRAD_PI2)*currentFPS;

//    cout << Pos.v[0] << " " << Pos.v[1] << endl;

    myNode->SetPos(Pos);

    return 0;
};

int RotateTo(void* Params){

    int n = myAI->Script_gettop(Params);
    if (n<1) { myAI->Script_error(Params); return 0; }

    INode* myNode = (INode*) myAI->GetCurrentObject();
    double value = myAI->Script_tonumber(Params,1);

    CVector Rot = myNode->GetRot();
    Rot.v[2]+=value;
    myNode->SetRot(Rot);

    return 0;

};

int GetRot(void* Params){

    INode* myNode = (INode*) myAI->GetCurrentObject();

    CVector Rot = myNode->GetRot();

    myAI->Script_pushnumber(Params, Rot.v[2] );
    return 1;

};

int SetStringParam(void *Params){

    int n = myAI->Script_gettop(Params);
    if (n<2) { myAI->Script_error(Params); return 0; }

    std::string ParamName = std::string( myAI->Script_tostring(Params,1));
    std::string ParamValue = std::string( myAI->Script_tostring(Params,2));
    INode* myObject = (INode*) (void*)myAI->GetCurrentObject();

    myObject->SetParam( ParamName.c_str(), ParamValue.c_str() );

    return 0;
};

int GetStringParam(void *Params){

    int n = myAI->Script_gettop(Params);
    if (n!=1) { myAI->Script_error(Params); return 0; }

    std::string ParamName = std::string( myAI->Script_tostring(Params,1) );

    INode* myObject = (INode*) myAI->GetCurrentObject();
//    if (myObject) myObject->GetName();

//    IBaseObject* myObject = (IBaseObject*) myAI->GetCurrentObject();
//    if (myObject) {
//        printf("%s\n", myObject->GetType() );
//    };

    std::string Param = myObject->GetParam( ParamName.c_str() );

    myAI->Script_pushstring(Params, Param.c_str() );
    return 1;

};

int ExecScript(void* Params){

    void* curObject = myAI->GetCurrentObject();

    int n = myAI->Script_gettop(Params);
    if (n!=1) { myAI->Script_error(Params); return 0; }

    std::string ParamName = std::string( myAI->Script_tostring(Params,1) );

    myAI->ExecuteFile(ParamName.c_str(), NULL);
    return 0;
};

int GetX(void* Params){

    INode* myNode = (INode*) myAI->GetCurrentObject();
    CVector Pos = myNode->GetPos();

    myAI->Script_pushnumber(Params, Pos.v[0] );
    return 1;
};

int GetY(void* Params){

    INode* myNode = (INode*) myAI->GetCurrentObject();
    CVector Pos = myNode->GetPos();

    myAI->Script_pushnumber(Params, Pos.v[1] );
    return 1;
};

int GetZ(void* Params){

    INode* myNode = (INode*) myAI->GetCurrentObject();
    CVector Pos = myNode->GetPos();

    myAI->Script_pushnumber(Params, Pos.v[2] );
    return 1;
};

void upload_scripts(IScriptManager* manager){

    manager->AddFunction("Run", ExecScript);

    manager->AddFunction("SetParam", SetStringParam);
    manager->AddFunction("GetParam", GetStringParam);

    manager->AddFunction("GetX", GetX);
    manager->AddFunction("GetY", GetY);
    manager->AddFunction("GetZ", GetZ);
    manager->AddFunction("GetRot", GetRot);

    manager->AddFunction("AddRobot", AddRobot);
    manager->AddFunction("DelRobot", DelRobot);
    manager->AddFunction("MoveForward", MoveForward);
    manager->AddFunction("Rotate", RotateTo);

};

void update_ai(IScriptManager* manager){
//    manager->ExecuteFile("update.lua", NULL);

    currentFPS = myGraphic->FrameInterval; // читаем не FPS, а интервал между кадрами
//    if (currentFPS< 1) currentFPS = 1;
//    cout << currentFPS << endl;
//    if (currentFPS> 100) currentFPS = 100;

    std::vector<INode*>::iterator it_ = roboList.begin();

    for (int i=0;i<roboList.size();i++){

//	IBaseObject* void_ptr = (*it_);
//	INode* temp = (INode*)(void*)void_ptr;
//	printf("--------------\n");
//	printf("%s \n", temp->GetType() );
        manager->ExecuteFile( (*it_)->GetParam("Brain"), (*it_) );
	it_++;
    }

};
