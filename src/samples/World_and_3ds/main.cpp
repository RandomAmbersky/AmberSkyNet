/// проста€ демка инициализации и отрисовки

#include "EngineStarter.h"
#include "Interfaces/ASNIWorld3D.h"

#define RANDOM(x) (rand() % (x))

IWorld *myWorld; // указатель на мир
ICamera *myCam; // указатель на камеру
/*ICamera *myCam2; // указатель на камеру*/

bool look_in_mouse;
bool k_MouseClick;

////////  —оздать объект сцены по базовому имени ///////
INode* CreateBaseNode(std::string NodeType)
{
    //printf("create base node...\n");
    UINT tmpId=myWorld->createNode( NodeType.c_str() );
    INode* myNode = myWorld->getNode( tmpId );

    IModifierPack* nodeModPack = (IModifierPack*)myNode->GetPtrParam("ModifierPack");

    // добавл€ем Rotate-модификатор
    INodeModifier* myModifier = (INodeModifier*) ENGINE->CreateObject("NodeModifierRotate");
    if (myModifier)
        nodeModPack->AddModifier( myModifier );
    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"%f %f %f",1.0f*rand()/10000.0f,1.0f*rand()/10000.0f,1.0f*rand()/10000.0f);
    myModifier->SetParam("RotSpeed",buf);

    return myNode;
};

////// ”ничтожить объект сцены ///////
void DestroyFlyingObject(INode *node)
{
    INode *tmpNode=CreateBaseNode("Explode");
    tmpNode->setPos ( node->getPos() );
    node->Release();
    //    delete node;
};

/////// —лучайно располагаем объект на сцене
void SetRandomParams(INode *myNode)
{
    if (!myNode)
        return;


    CVector Pos = CVector( RANDOM(100), RANDOM(100), RANDOM(100) );
    CVector Rot = CVector( 1*RANDOM(360), 1*RANDOM(360), 1*RANDOM(360) );

    myNode->setPos(Pos);
    myNode->setRot(Rot);

    myNode->SetParam("CanDel","Yes"); // объект может быть уничтожен
    //  printf("%s\n",myNode->GetParam("CanDel"));

};

////// ƒобавить на сцену случайный объект
void AddFlyingObject()
{
    // случайно выбираем тип объекта
    UINT state=RANDOM(7);
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
    else if (state==6)
        SetRandomParams( CreateBaseNode("Gnome") );
};

// найти куда указывает курсор мышки и проверить
// что именно там находитс€
void FindMousePointer(UINT x,UINT y)
{

    CVector Start=CVector();
    CVector End=CVector();

    // переводим координаты курсора мышки в отрезок мира
    DEVICER->MouseToWorld(x,y,Start,End);

    asn_ext_ptr_list* myList = new asn_ext_ptr_list();

    ASNIWorld3D *my3dWorld=(ASNIWorld3D*)myWorld->getInterface("ASNIWorld3d");
    my3dWorld->getNodesByPos(Start,End,myList);

    // получаем список нод, которые наш отрезок "проткнул"
    //myWorld->getNodesByPos(Start,End,myList);

    if ( myList->nums>0)
    {

        for ( UINT i=0; i<myList->nums; i++ )
        {

            if ( std::string( ((INode*)myList->ptr_list[i].pointer)->GetParam("CanDel") ) == "Yes" )
            {
            DestroyFlyingObject( (INode*)myList->ptr_list[i].pointer );
            deletePtrList(myList);
            return;
            }

            //it++;
        };
    };

    deletePtrList(myList);

};

// Draw-функци€
bool DrawCallback(void* Params)
{

    //рисуем наш мир
    //myWorld->Draw();

    //myCam2->setPos(myCam->getPos()-CVector(1,1,1));

    //"смотрим" в нашу камеру
    //myCam2->View();
    //рисуем наш мир

    // "смотрим" в нашу камеру. это надо дл€ корректного определени€
    // отрезка "протыкающего" мир в FindMousePointer

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
        myCam->View();
        FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    myWorld->Draw();

    CVector4 color = CVector4( 0, 1, 1, 1 );
    CVector4 color2 = CVector4( 1, 0, 1, 1 );

    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"FPS: %d Hello World! йцукенгшщзхъфывапролджэ€чсмитьбю…÷” ≈Ќ√Ўў«’Џ‘џ¬јѕ–ќЋƒ∆Ёя„—ћ»“№Ѕё®",DEVICER->FPS);
    DEVICER->SetColor(color2);
    DEVICER->PrintAt(0,0,buf);

    DEVICER->SetColor(color);
    DEVICER->PrintAt(1,1,buf);


    return true;
};

// Update-функци€, если возвратит true- произойдет выход из цикла Engine
bool UpdateCallback(void *Params)
{

    //printf("update\n");

    // читаем первое событие
    int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все событи€
    while (evt!=ASN_EVNT_EMPTY)
    {

        if (evt==ASN_EVNT_KEYPRESSED)
        { //если событие - нажатие клавиши, то...

            // если мы нажали Esc - устанавливаем признак выхода в true
            if (EVENTER->getLastKey()==27)
            {
                return true;
            }

            if (EVENTER->getLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string tmpPath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
                myWorld->SaveWorld(tmpPath.c_str());
                }

            if (EVENTER->getLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
                myWorld->LoadWorld("World.xml");
                }

            // если мы нажали PrtSrc - делаем скриншот с именем screenshotXXX
            if (EVENTER->getLastKey()==316)
            {
                DEVICER->MakeScreenshot("screenshot");
                printf("Shoot!\n");
            }

            if (EVENTER->getLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }

            // если мы нажали insert - добавл€ем на сцену 10 объектов
            if (EVENTER->getLastKey()==277)
            {
                for (int i=0;i<10;i++)
                    AddFlyingObject();
            }


        }

        // если клавишу отпустили - выключаем движение
        else if (evt==ASN_EVNT_KEYUP)
        {
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
            //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        }
        evt=EVENTER->GetInput();
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
    return false;

};

int main(int argc, char *argv[])
{

    //std::map<std::string, std::string> Mappp;

    look_in_mouse=false;
    k_MouseClick=false;

    if (!ENGINE)
    {  // ENGINE указывает в NULL - загрузить не получилось :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        exit (1);
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

    //////////////// TEST //////////////////
    ENGINE->SetEnvParam("TestParam1","OK");
    printf("Get TestParam1: %s\n", ENGINE->GetEnvParam("TestParam1") );
    //////////////// TEST //////////////////

    FILER->AddPath("samples/World_and_3ds/", false);

    // создаЄм менеджер мира
    myWorld = (IWorld *)ENGINE->CreateObject("WorldSimple3D");
    if (!myWorld)
    {
        printf("Sorry, WorldSimple-plugin not found :(\n");
        return 1;
    }

    printf("World created %s .\n", myWorld->GetType());

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

    printf("Try add camere into world\n");

    UINT cam_id = myWorld->addCamera("CameraSimple");

    printf("Try add camere into world\n");

    myCam = myWorld->getCamera(cam_id);
    myCam->setScreen(0,0,1,1/*0.5*/);

    printf("Try add camere into world\n");

    /*    // делаем еще одну камеру 8)
        myCam2 = myWorld->getCamera(myWorld->addCamera("CameraSimple"));
        myCam2->setScreen(0,0.5,1,0.5);
        myCam2->setPos( CVector(-100,48,126) );
        myCam2->setRotY( -32 ); */

    // добавл€ем "Camera Walker" listener
    IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera walker created.\n");

    // добавл€ем "Camera Looker" listener
    //  IEventListener*
    myList = EVENTER->AddEventListener( "EvListCamLooker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera looker created.\n");


    // позиционируем камеру
    myCam->setPos( CVector(-93,48,126) );
    myCam->setRotY( -32 );

    //  INode *myNode=(INode*)ENGINE->CreateObject("NodeBase");
    //  if (myNode) printf("ok... \n");

    //  myWorld->createNode("NodeBase");
    //  myWorld->addNode( NULL );
    // загружаем в наш мир карту
    myWorld->LoadWorld( "World_and_3ds.xml" );
    printf("Map loaded.\n");

    int start_param=ASN_DEV_CANRESIZE;
    if ( ASN_Str( ENGINE->GetEnvParam("[DeviceManager]FullScreen")) =="1")
        start_param=start_param||ASN_DEV_FULLSCREEN;

    int width = 640;
    int height = 480;

    sscanf(ENGINE->GetEnvParam("[DeviceManager]Width") ,"%d",&width);
    sscanf(ENGINE->GetEnvParam("[DeviceManager]Height") ,"%d",&height);

    if ( DEVICER->Init(width,height,0,start_param) )
    {
        printf("Ooops... video error!\n");
        return 0;
    }

    // устанавливаем 3d-режим графики
    DEVICER->SetMode( ASN_GRAPH_3D );
    DEVICER->CenterMouse(false);

    printf("Devicer [OK]\n");

    int evt;
    bool non_exit=true;

    int t=ENGINE->AddWorld(myWorld);
    printf("world num: %d\n",t);

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

    ENGINE->EngineStart();

    printf("----End----\n");

    return 0;
};
