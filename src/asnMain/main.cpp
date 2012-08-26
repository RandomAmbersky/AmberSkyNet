/// простая демка инициализации и отрисовки

#include <stdlib.h>
#include "EngineStarter.h"
#include "IPhysicManager.h"
#include "ASN_Params.h"

#define RANDOM(x) (rand() % (x))

//#include "INetProtocolHandler.h"
#include "Interfaces/ASNIWorld3D.h"

IWorld *myWorld; // указатель на мир
ASNIWorld3D *my3dWorld; //указатель на 3d-интерфейс мира

//IWorld3d *myWorld3d;
ICamera *myCam; // указатель на камеру
//*ICamera *myCam2; // указатель на камеру*/

bool look_in_mouse=true;
bool k_MouseClick=false;
float FOV;

////////  Создать объект сцены по базовому имени ///////
INode* CreateBaseNode(std::string NodeType)
{

    printf("create base node %s...\n", NodeType.c_str() );

//    UINT tmpId=myWorld->createNode( NodeType.c_str() );
//    INode* myNode = myWorld->getNode( tmpId );

    IBaseObject* myObj = dynamic_cast<IBaseObject*>( ENGINE->CreateObject( NodeType.c_str() ) );
    if (!myObj) return NULL;

    printf("create base node %s...\n", myObj->GetType() );

    INode* myNode = dynamic_cast<INode*>(myObj);//->AsType("Node");
    if (!myNode) return NULL;

    printf("create base node %s...\n",NodeType.c_str());

    myWorld->AddChild (myNode);

    //IModifierPack* nodeModPack = (IModifierPack*)myNode->GetPtrParam("ModifierPack");

    //if(nodeModPack) printf("ModOk %s\n",nodeModPack->GetType() );

        // добавляем Rotate-модификатор
    //INodeModifier* myModifier = (INodeModifier*) ENGINE->CreateObject("NodeModifierRotate");

    //if (myModifier) {
        //nodeModPack->AddModifier( myModifier );

    //char buf[1024];  // вроде должно хватить :)
    //sprintf(buf,"%f %f %f",1.0f*rand()/1000.0f,1.0f*rand()/1000.0f,1.0f*rand()/1000.0f);
    //myModifier->SetParam("RotSpeed",buf);

    //printf("ok\n");
    //}

    return myNode;
};

////// Уничтожить объект сцены ///////
void DestroyFlyingObject(INode *node)
{
    INode *tmpNode=CreateBaseNode("Explode");
    tmpNode->SetPos ( node->GetPos() );
    node->Release();
    //    delete node;
    //    ItemsNum--;
};

/////// Случайно располагаем объект на сцене
void SetRandomParams(INode *myNode)
{
    if (!myNode)
        return;


    CVector Pos = CVector( RANDOM(100), RANDOM(100), 200 /*RANDOM(100)*/ );
    CVector Rot = CVector( 1*RANDOM(360), 1*RANDOM(360), 1*RANDOM(360) );
    CVector Size = CVector( RANDOM(10), RANDOM(10), RANDOM(10) );

    CVector Color = CVector( RANDOM(100)/100.0f, RANDOM(100)/100.0f, RANDOM(100)/100.0f );

    myNode->SetPos( Pos );
    myNode->SetRot(Rot);
    //myNode->setSize(Size);

    myNode->SetParam("CanDel","Yes"); // объект может быть уничтожен

    char buf[1024];
        sprintf(buf,"%f %f %f",Color.v[0],Color.v[1],Color.v[2]);
    printf("Color : %s\n", buf);
    //myNode->SetParam("Color",buf);
    sprintf(buf,"%f", RANDOM(100)/100.0f );
    //myNode->SetParam("Alpha", buf );
      printf("%s\n",myNode->GetParam("CanDel"));

    //IPhysicManager* worldPhysic = (IPhysicManager*) myWorld->GetPtrParam("PhysicManager");
    if (PHYSICS) PHYSICS->AddNode(myNode,"box");

};

////// Добавить на сцену случайный объект
void AddFlyingObject()
{
    // случайно выбираем тип объекта
    UINT state=RANDOM(6);
    if (state==0)
        SetRandomParams( CreateBaseNode("Axe") );
    else if (state==1)
        SetRandomParams( CreateBaseNode("Column") );
    else if (state==2)
        SetRandomParams( CreateBaseNode("Stairs") );
    else if (state==3)
        SetRandomParams( CreateBaseNode("Helmet") );
    else if (state==4)
        SetRandomParams( CreateBaseNode("Sword") );
    else if (state==5)
        SetRandomParams( CreateBaseNode("Chair") );
    //else if (state==6)
    //    SetRandomParams( CreateBaseNode("Bogat") );
};

// найти куда указывает курсор мышки и проверить
// что именно там находится
void FindMousePointer(UINT x,UINT y)
{

    CVector Start=CVector();
    CVector End=CVector();

    // переводим координаты курсора мышки в отрезок мира
    DEVICER->MouseToWorld(x,y,Start,End);
    asn_ext_ptr_list* myList = createPtrList();

    // получаем интерфейс
    //ASNIWorld3D *my3dWorld=(ASNIWorld3D*)myWorld->getInterface("ASNIWorld3d");

    my3dWorld->GetNodesByPos(Start,End,myList);

    if ( myList->nums>0)
    {
	if(SOUND) SOUND->PlayFX("Shot.wav");
        printf("nums: %d\n",myList->nums);
        for ( UINT i=0; i<myList->nums; i++ )
        {

            INode* myNode=(INode*)myList->ptr_list[i].pointer;
            printf("Name: %s %s (%d)\n", myNode->GetName(),
                                    myNode->GetType(),
                                    myNode->GetID()
             );
            //if ( std::string( ((INode*)myList->ptr_list[i].pointer)->GetParam("CanDel") ) == "Yes" )
//            {
            //DestroyFlyingObject( (INode*)myList->ptr_list[i].pointer );
            //deletePtrList(myList);

            //((INode*)myList->ptr_list[i].pointer)->SetParam("Texture","Brick wall.jpg");


// физика
//                    INode* myNode=(INode*)myList->ptr_list[i].pointer;
                    IPhysicObject* myPhysicNode = (IPhysicObject*)myNode->GetPtrParam("PhysicObject");
                    if (myPhysicNode) {
                    float rotX, rotY;
                    myCam->GetRot(rotX, rotY);
                    myPhysicNode->AddForce(CVector(100.0*cos(rotX*PI/180.0f),100.0*sin(rotX*PI/180.0f),100.0*tan(rotY*PI/180.0f)));
                    }

           //it++;
        };
	    deletePtrList(myList);

            return;
//            }


    };

    deletePtrList(myList);

};

bool UpdatePointers(){

 myWorld = ENGINE->GetWorld();
    if (!myWorld) {
        printf("Can't get world! Exiting... :(\n");
        return false;
    };

    my3dWorld=(ASNIWorld3D*)myWorld->GetInterface("ASNIWorld3d");
    if (!my3dWorld) {
        printf("Can't get ASNIWorld3d interface from world! Exiting... :(\n");
        return false;
    };

    myCam = myWorld->GetCamera(0);
    if (!myCam) {
        printf("Can't get myCam from world! Exiting... :(\n");
        return false;
    };

    return true;
}

bool get_matrix=false;
// Draw-функция
int DrawCallback(void* Params)
{

//printf("draw callback!\n");
    //рисуем наш мир
    //myWorld->Draw();

    //myCam2->setPos(myCam->getPos()-CVector(1,1,1));

    //"смотрим" в нашу камеру
    //myCam2->View();
    //рисуем наш мир

    // "смотрим" в нашу камеру. это надо для корректного определения
    // отрезка "протыкающего" мир в FindMousePointer

    //DEVICER->PushMatrix();

    myCam->View();

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
//        myCam->View();
        FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    //DEVICER->SetScale( CVector(0.1,0.1,0.1) );

    myWorld->Draw();
//    if ( myWorld->Draw() ) myCam->View();

    CVector pos=myCam->GetPos();

    //DEVICER->PopMatrix();

    //DEVICER->SetFontSize(1.0);
    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"привет!FPS: %d Hello World! X:%f Y:%f Z:%f",DEVICER->FPS,pos.v[0],pos.v[1],pos.v[2]);

    DEVICER->PrintAt(0,0,buf);
    DEVICER->StartMode2D(false);

    int width;
    int height;

 sscanf(ENGINE->GetEnvParam("[DeviceManager]width"),"%d",&width);
 sscanf(ENGINE->GetEnvParam("[DeviceManager]height"),"%d",&height);

    float text_width=10;
    float text_height=10;

//DEVICER->SetFontSize(2.0);
    DEVICER->GetTextWH(buf,text_width,text_height);
    float pos_x=((float)width-text_width)/2.0;
    float pos_y=((float)height-text_height)/2.0;

//    printf("%f %f \n",text_width,text_height);
//    printf("%f %f \n",pos_x,pos_y);

    CVector4 Color_a = CVector4(1,1,0,1);
    CVector4 Color_b = CVector4(1,0,0,0.5);

 //DEVICER->SetScale( CVector(1.0/800.0,1.0/600.0,1) );
 //DEVICER->SetScale( CVector(1.0/800.0,1.0/600.0,1) );

//    DEVICER->SetFontSize(1.0);
//    DEVICER->SetColor(Color_b);
//    DEVICER->PrintAt(11,11,buf);
//    DEVICER->SetFontSize(1.01);
    DEVICER->SetColor(Color_a);
    DEVICER->PrintAt(pos_x,pos_y,buf);

    //DEVICER->SetFontSize(1.0);
    DEVICER->EndMode2D();

    //Pop2DMode();
    //DEVICER->PopMatrix();
    //DEVICER->SetMatrixs(Model_Matrix, Project_Matrix);
    //DEVICER->SetMatrixs(Model_Matrix, Project_Matrix);

    //printf("draw callback ok\n");
    return false;
};

// Update-функция, если возвратит true- произойдет выход из цикла Engine
int UpdateCallback(void *Params)
{

    if (PHYSICS) PHYSICS->Update(0.5);
    //else  printf("can't update\n");

    // читаем первое событие
    int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все события
    while (evt!=ASN_EVNT_EMPTY)
    {

        //printf("get input..%d.\n", evt);

        if (evt==ASN_QUIT)
        {
                return true;
        }
        else if (evt==ASN_WINDOW_RESIZE) {

            int width_;
            int height_;

            sscanf(ENGINE->GetEnvParam("[DeviceManager]width"),"%d",&width_);
            sscanf(ENGINE->GetEnvParam("[DeviceManager]height"),"%d",&height_);

            float res = ((float)width_)/height_;

//            myCam->setPerspective(FOV, res, 0.1f, 1e5f);

        } else if (evt==ASN_MOUSE_WHEEL_UP) {
            FOV+=1.0f;
            myCam->SetPerspective(FOV, (float)4.0f/3.0f, 0.1f, 1e5f);
        }
        else if (evt==ASN_MOUSE_WHEEL_DOWN) {
            FOV-=1.0f;
            myCam->SetPerspective(FOV, (float)4.0f/3.0f, 0.1f, 1e5f);
        }
        else if (evt==ASN_EVNT_KEYPRESSED)
        { //если событие - нажатие клавиши, то...

            // если мы нажали Esc - устанавливаем признак выхода в true
            if (EVENTER->GetLastKey()==27)
            {
                return true;
            }

            else if (EVENTER->GetLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string tmpPath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
                ENGINE->SaveDataFile(tmpPath.c_str(), myWorld);
                //myWorld->SaveWorld(tmpPath.c_str());
                }

            else if (EVENTER->GetLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
                ENGINE->LoadDataFile("World.xml");
                if ( !UpdatePointers() ) return true; // не смогли загрузить World, выходим :(
                printf("LoadWorld OK\n");
                }

            else if (EVENTER->GetLastKey()==ASNK_F7) {
                    DEVICER->Init(800,600,0,ASN_DEV_CANRESIZE);
                }



//            если мы нажали PrtSrc - делаем скриншот с именем screenshotXXX
//            else if (EVENTER->getLastKey()==316)
//            {
//                DEVICER->MakeScreenshot("screenshot");
//                printf("Shoot!\n");
//            }

            else if (EVENTER->GetLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }

            // если мы нажали insert - добавляем на сцену 10 объектов
            else if (EVENTER->GetLastKey()==277)
            {
                for (int i=0;i<10;i++)
                    AddFlyingObject();
            }

//        printf("en input..%d.\n", evt);

        }
        else if (evt==ASN_EVNT_KEYUP)
        {
        // если клавишу отпустили - выключаем движение
            //        char key=(char)EVENTER->LastKey;

            //         if (EVENTER->LastKey==ASNK_d)   k_MoveRight=false;
            //         if (EVENTER->LastKey==ASNK_a)   k_MoveLeft=false;
            //         if (EVENTER->LastKey==ASNK_w)   k_MoveForvard=false;
            //         if (EVENTER->LastKey==ASNK_s)   k_MoveBackward=false;
            //         if (EVENTER->LastKey==ASNK_q)   k_MoveUp=false;
            //         if (EVENTER->LastKey==ASNK_z)   k_MoveDown=false;

        }
        else if (evt==ASN_EVNT_MOUSEDOWN)
        {
            k_MouseClick=true;
//            FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        }

//        printf("en input..%d.\n", evt);
        evt=EVENTER->GetInput();

//      printf("get input..%d.\n", evt);

/*
printf("event..%d.\n", ASN_NONE);
printf("event..%d.\n", ASN_EVNT_EMPTY);
printf("event..%d.\n", ASN_EVNT_KEYPRESSED);
printf("event..%d.\n", ASN_EVNT_KEYUP);
printf("event..%d.\n", ASN_EVNT_MOUSEMOVE);
printf("event.%d.\n", ASN_EVNT_MOUSEDOWN);
printf("event..%d.\n", ASN_EVNT_MOUSEUP);
printf("event.%d.\n", ASN_MOUSE_BUTTON_LEFT);
printf("event.%d.\n", ASN_MOUSE_BUTTON_RIGHT);
printf("event..%d.\n", ASN_MOUSE_BUTTON_MIDDLE);
printf("event..%d.\n", ASN_MOUSE_WHEEL_UP);
printf("event..%d.\n", ASN_MOUSE_WHEEL_DOWN);
printf("event..%d.\n", ASN_QUIT);
printf("event..%d.\n", ASN_WINDOW_ACTIVE_ON);
printf("event..%d.\n", ASN_WINDOW_ACTIVE_OFF);
printf("event..%d.\n", ASN_WINDOW_RESIZE);
printf("event..%d.\n", ASN_WINDOW_MOUSE_FOCUS_ON);
printf("event..%d.\n", ASN_WINDOW_MOUSE_FOCUS_OFF);
printf("event..%d.\n", ASN_WINDOW_INPUT_FOCUS_ON);
printf("event..%d.\n", ASN_WINDOW_INPUT_FOCUS_OFF);
*/
    }

/*
    if (RANDOM(100)>60)
        //printf("maxID %d\n",myWorld->getMaxID());
        for (uint i=0;i<myWorld->getMaxID();i++)
        {

            INode *myNode= myWorld->getNode( i+1 );
            if (myNode)
            {
                //           printf("[%s]\n",myNode->GetParam(www("CanDel");
                //        printf("%s\n",myNode->GetParam("CanDel"));
                if ( std::string(myNode->GetParam("CanDel")) == "Yes" )
                {
                    DestroyFlyingObject(myNode);
                    break;
                }
            };

        };
*/
//
//	printf("update ok\n");
    return false;
};

/*
void NodeListView(IBaseObject* myObj, int level){

//    if (!myObj) return;
    int i = level;
    while( i>0){ printf("-");i--; }
    printf( "(%d) %s %s\n", myObj->getID(), myObj->GetType(), myObj->GetBaseClass() );
    UINT cnt = myObj->getChildCount();
    //printf( "%d\n", cnt );
    while (cnt!=0){
        NodeListView( myObj->getChild(cnt-1), level+1 );
        cnt--;
    }
}
*/

/*
class Visitor: public IBaseObjectVisitor {
public:

    Visitor(){level=0;}

    void Visit( IBaseObject* myObj){

        int i = level;
        while( i>0){ printf("-");i--; }
        printf( "(%d) %s %s\n", myObj->getID(), myObj->GetType(), myObj->GetBaseClass() );
        level++;

    };

    void AfterVisit( IBaseObject* myObj){

        level--;

    };

    int level;

};
*/

int main(int argc, char *argv[])
{

    ENGINE_INIT(argc, argv);

//printf(":dssdssd");
    //printf("haha: %s \n", argv[0]);

//    look_in_mouse=false;

//    k_MouseClick=false;
    FOV = 60.0f;

//    printf("hello world!\n");

    if (!ENGINE)
    {  // ENGINE указывает в NULL - загрузить не получилось :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        exit (1);
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

    //FILER->AddPath("HOME/", false);
//    FILER->AddPath("http://127.0.0.1/data/", false);
//    FILER->AddPath("logo/", false);
//    FILER->AddPath("http://172.16.24.37/HOME/", false);
//    FILER->AddPath("http://172.16.24.37/HOME/common/", false);
//    IFile *myFile=FILER->GetFile("naked_nigra.jpg");

//    FILER->AddPath("http://172.16.2.152/", false);
//    FILER->AddPath("psy/", false);
//    IFile *myFile=FILER->GetFile("picture.php");

/*
    if (!myFile) return 0;


	while( true ) {

	NETTER->Check();

//	void* buf = myFile->LoadInMem();
	int state = myFile->FileOpen();

//	cout << "file state: " << state << endl;

	if (state == ASN_FILE_NOT_FOUND ) break;
//
	if (state == ASN_FILE_DISK_OPEN ) break;

	}

    cout << "file state: " << myFile->GetState() << endl;

    return 0;
*/

    printf("------------------------------------------------------------------------------------------------\n");

//    ENGINE->LoadDataFile("commonObjects.xml");
//    ENGINE->LoadDataFile("base.xml");
//for(int i=0;i<20;i++){
//    ENGINE->LoadDataFile("data_driven.old");
//    }
//    ENGINE->LoadDataFile("World.xml");
//return 0;

//ENGINE->LoadDataFile("data_driven.old");
//ENGINE->LoadDataFile("data_driven.old");
//myWorld = ENGINE->GetWorld();
//ENGINE->SaveDataFile("data_driven.old_", myWorld);

    ENGINE->LoadDataFile("data_driven.xml");
    printf("------------------------------------------------------------------------------------------------\n");

//    return 0;

    if (!EVENTER) {
        printf("Can't find EventManager! Exiting... :(\n");
        return 1;
    };

//    LOGGER->SetParam("CustomMessage","Custom message OK");
//    LOGGER->LogMsg( LOGGER->GetParam("CustomMessage") );

//    //////////////// TEST //////////////////
//    ENGINE->SetEnvParam("TestParam1","OK");
//    printf("Get TestParam1: %s\n", ENGINE->GetEnvParam("TestParam1") );
//    //////////////// TEST //////////////////

//    return 0;

    // создаём менеджер мира

//    myWorld = ENGINE->CreateWorld("World");

//    if (!myWorld)
//    {
//        printf("Sorry, WorldSimple-plugin not found :(\n");
//        return 1;
//    }

    // получаем указатель на 3d-интерфейс мира


//    DEVICER->Init(800,600,0,ASN_DEV_CANRESIZE);

    if ( !UpdatePointers() ) return 1;

/*
    myWorld = ENGINE->GetWorld();
    if (!myWorld) {
        printf("Can't get world! Exiting... :(\n");
        return 1;
    };

    printf("World created %s .\n", myWorld->GetType());

    my3dWorld=(ASNIWorld3D*)myWorld->getInterface("ASNIWorld3d");
    if (!my3dWorld) {
        printf("Can't get ASNIWorld3d interface from world! Exiting... :(\n");
        return 1;
    };


    IPhysicManager* worldPhysic = (IPhysicManager*) myWorld->GetPtrParam("PhysicManager");
    if (worldPhysic) {
//        PHYSICS->setGravity( CVector(0,0,0) );
        PHYSICS->setGravity( CVector(0,0,-9.81) );
    }
*/

    //IWorld3D *my3dWorld=(IWorld3D*)myWorld->GetInterface("World3d");

    /////////// TEST "human" GetXXXXParam ///////////////////

//    myWorld->SetParam("TestParamInt","12");
//    myWorld->SetParam("TestParamFloat","12.3456");

//    printf("TestParamInt: %d\n", myWorld->GetIntParam("TestParamInt") );
//    printf("TestParamFloat: %f\n", myWorld->GetFloatParam("TestParamFloat") );

    /////////// TEST "human" GetXXXXParam ///////////////////

    /*
    myCam2 = (ICamera *)ENGINE->CreateObject("CameraSimple");
    myCam2->setPos( CVector(-93,48,126) );
    myCam2->setRotY( -32 );

    myCam = (ICamera *)ENGINE->CreateObject("CameraSimple");
    if (!myCam)
    {
        printf("Class CameraSimple can't found :(\n");
        return 1;
    }

    printf("Camera created.\n");
    */

//    UINT cam_id = myWorld->addCamera("CameraSimple");

//    myCam = myWorld->getCamera(0);
//    if (!myCam) {
//        printf("Can't get myCam from world! Exiting... :(\n");
//        return 1;
//    };
//    return 0;
//     myWorld->addCamera("CameraSimple") );
//    myCam->setScreen(0,0,1,1/*0.5*/);

    // добавляем "Camera Walker" listener
//    IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
//    if (!myList)
//    {
//        printf("Class EvListCamWalker can't found :(\n");
//        return 1;
//    }

//    myList->SetPtrParam("Camera",myCam);
//    printf("Camera walker created.\n");

    // добавляем "Camera Looker" listener
//    myList = EVENTER->AddEventListener( "EvListCamLooker" );
//    if (!myList){
//        printf("Class EvListCamWalker can't found :(\n");
//        return 1;
//    }
//    myList->SetPtrParam("Camera",myCam);
//    printf("Camera looker created.\n");

    // позиционируем камеру
    myCam->SetPos( CVector(-93,48,126) );
    myCam->SetRotY( -32 );

//    myWorld->LoadWorld( "map.xml" );
    printf("Map loaded.\n");


//    myCam->setPerspective(FOV, (float)400/400 /*4.0f/3.0f*/, 0.1f, 1e5f);

//    bool non_exit=true;

//    ENGINE->SetWorld(myWorld);

//    ENGINE->Create
    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

//    INetProtocolHandler* myNet = NETTER->OpenClientListener("My_Jabber","Jabber","server24.dept24.spectr.okb",5222,"");
//    printf("NETTER [OK]\n");
//    myNet->SetParam("UserName","test");
//    myNet->SetParam("Password","test");
//    myNet->Send("connect","server","10");

//    if(SOUND) SOUND->PlayMusic("ball01.wav");

    ENGINE->EngineStart();

//    Visitor vis;
//    myWorld->Visit( &vis );
    //NodeListView( myWorld , 0);


//    while (true){
//    DEVICER->StartDraw();
//    DrawCallback(NULL);
//    DEVICER->EndDraw();
//    if ( UpdateCallback(NULL) ) break;
//    };

    printf("----End----\n");

    return 0;
};
