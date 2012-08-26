#include <stdlib.h>
#include "EngineStarter.h"

#define RANDOM(x) (rand() % (x))

IWorld *myWorld; // указатель на мир
ICamera *myCam; // указатель на камеру
IScriptManager *myAI;
IDevice *myGraphic;
IEngine *myEngine; // указатель на Engine для остальных модулей

bool look_in_mouse;
bool k_MouseClick;

std::string MapName;

extern void upload_scripts(IScriptManager* manager);
extern void update_ai(IScriptManager* manager);

/*
// найти куда указывает курсор мышки и проверить
// что именно там находится
void FindMousePointer(UINT x,UINT y)
{

    CVector Start=CVector();
    CVector End=CVector();

    // переводим координаты курсора мышки в отрезок мира
    DEVICER->MouseToWorld(x,y,Start,End);

    asn_ext_ptr_list* myList = new asn_ext_ptr_list();
    // получаем список нод, которые наш отрезок "проткнул"
    myWorld->getNodesByPos(Start,End,myList);

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
*/

// Draw-функция
int DrawCallback(void* Params)
{


    //рисуем наш мир
    //myWorld->Draw();

    //myCam2->setPos(myCam->getPos()-CVector(1,1,1));

    //"смотрим" в нашу камеру
    //myCam2->View();
    //рисуем наш мир

    // "смотрим" в нашу камеру. это надо для корректного определения
    // отрезка "протыкающего" мир в FindMousePointer

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
//        myCam->View();
        //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    myCam->View();
    myWorld->Draw();

    CVector4 color = CVector4( 0, 1, 1, 1 );
    CVector4 color2 = CVector4( 1, 0, 1, 1 );

    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"FPS: %d ",DEVICER->FPS);
    DEVICER->SetColor(color2);
    DEVICER->PrintAt(0,0,buf);

    DEVICER->SetColor(color);
    DEVICER->PrintAt(1,1,buf);

    return true;
};

// Update-функция, если возвратит true- произойдет выход из цикла Engine
int UpdateCallback(void *Params)
{

    //printf("update\n");

    // читаем первое событие
    int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все события
    while (evt!=ASN_EVNT_EMPTY)
    {

        if (evt==ASN_QUIT)
        {
                return true;
        }

        else if (evt==ASN_EVNT_KEYPRESSED)
        { //если событие - нажатие клавиши, то...

            // если мы нажали Esc - устанавливаем признак выхода в true
            if (EVENTER->GetLastKey()==27)
            {
                return true;
            }
/*
            if (EVENTER->getLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string tmpPath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
                myWorld->SaveWorld(tmpPath.c_str());
                }

            if (EVENTER->getLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
                myWorld->LoadWorld("World.xml");
                }
*/

            // если мы нажали PrtSrc - делаем скриншот с именем screenshotXXX
            if (EVENTER->GetLastKey()==316)
            {
                DEVICER->MakeScreenshot("screenshot");
                printf("Shoot!\n");
            }

            if (EVENTER->GetLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }


        }

        else if (evt==ASN_EVNT_MOUSEDOWN)
        {
            k_MouseClick=true;
            //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        }
        evt=EVENTER->GetInput();
    }

//    ENGINE;
    //->SetEnvParam("TestParam1","OK");

//    STARTER;
//     = SCRIPTER;

    update_ai(SCRIPTER);

    //evt=
    //EVENTER;
    //->GetInput();
    //void* ptr = ENGINE;//->GetPtrParam("ScriptManager");

    return false;

};

bool UpdatePointers(){

 myWorld = ENGINE->GetWorld();
    if (!myWorld) {
        printf("Can't get world! Exiting... :(\n");
        return false;
    };

//    my3dWorld=(ASNIWorld3D*)myWorld->GetInterface("ASNIWorld3d");
    //if (!my3dWorld) {
        //printf("Can't get ASNIWorld3d interface from world! Exiting... :(\n");
        //return false;
    //};

    myCam = myWorld->GetCamera(0);
    if (!myCam) {
        printf("Can't get myCam from world! Exiting... :(\n");
        return false;
    };

    return true;
}

int main(int argc, char *argv[])
{

    MapName="robo_map.xml";

    look_in_mouse=false;
    k_MouseClick=false;

    ENGINE_INIT(argc, argv);

    if (!ENGINE)
    {  // ENGINE указывает в NULL - загрузить не получилось :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        exit (1);
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

    myEngine = ENGINE;
///////////////////// TEST //////////////////
//    ENGINE->SetEnvParam("TestParam1","OK");
//    printf("Get TestParam1: %s\n", ENGINE->GetEnvParam("TestParam1") );
/////////////////// TEST //////////////////

//    int width = 640;
//    int height = 480;

//    sscanf(ENGINE->GetEnvParam("[DeviceManager]Width") ,"%d",&width);
//    sscanf(ENGINE->GetEnvParam("[DeviceManager]Height") ,"%d",&height);

//    int start_param=ASN_DEV_CANRESIZE;
//    if ( ASN_Str( ENGINE->GetEnvParam("[DeviceManager]FullScreen")) =="1")
//        start_param=start_param||ASN_DEV_FULLSCREEN;

    ///// разбор параметров входной строки

//	for(int i = 1; i < argc; i++) {

/// включаем полноэкранный режим
//		if(strcmp(argv[i], "-fullscreen") == 0) {
//			start_param = start_param || ASN_DEV_FULLSCREEN;
//			continue;
//		}

/// задаём имя карты
//		if(strcmp(argv[i], "-map") == 0) {
//			MapName = argv[i+1];
//			continue;
//		}

///  разрешение изменяем на то, что получили в строке, например -res 1024 768
//		if(argc >= i + 2 && strcmp(argv[i], "-res") == 0) {
//			width = atoi(argv[i+1]);
//			height = atoi(argv[i+2]);
//			continue;
//		}

// по идее не понадобится совсем нам...
//		if(argc >= i + 2 && strcmp(argv[i], "-set") == 0) {
//		    ENGINE->SetEnvParam( argv[i+1] , argv[i+2] );
//			continue;
//		}

//		if( argv[i][0]=='-'){
//		    printf("Hmm.. i don't know %s\n",argv[i]);
//		    continue;
//		};
//	}


    myAI = SCRIPTER;

    if (!myAI) {
        printf("Can't get Script manager! Exiting... :(\n");
        return 1;
    }

    myGraphic = DEVICER;
    if (!myGraphic) {
        printf("Can't get Device manager! Exiting... :(\n");
        return 1;
    }

    FILER->AddPath("samples/RoboWorld/", false);
    ENGINE->LoadDataFile(MapName.c_str());
    // Camera
    UpdatePointers();

    //myWorld = ENGINE->GetWorld();
    //if (!myWorld) {
        //printf("Can't get world! Exiting... :(\n");
        //return 1;
    //};
    //printf("World created.\n");

    //myCam = myWorld->getCamera( myWorld->addCamera("CameraSimple") );
    //myCam->setScreen(0,0,1,1/*0.5*/);
    // Camera Walker listener
    //IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
    //if (!myList)
    //{
        //printf("Class EvListCamWalker can't found :(\n");
        //return 1;
    //}
    //myList->SetPtrParam("Camera", myCam);
    //printf("Camera walker created.\n");
    // Camera Looker listener
    //myList = EVENTER->AddEventListener( "EvListCamLooker" );
    //if (!myList)
    //{
        //printf("Class EvListCamWalker can't found :(\n");
        //return 1;
    //}
    //myList->SetPtrParam("Camera", myCam);
    //printf("Camera looker created.\n");
    // позиционируем камеру
    myCam->SetPos( CVector(-93,48,126) );
    myCam->SetRotY( -32 );

    // загружаем в наш мир карту

//    if ( DEVICER->Init(width,height,0,start_param) )
//    {
//        printf("Ooops... video error!\n");
//        return 0;
//    }

    // устанавливаем 3d-режим графики
//    DEVICER->SetMode( ASN_GRAPH_3D );
//    DEVICER->CenterMouse(false);

//    printf("Devicer [OK]\n");

    int evt;
    bool non_exit=true;

//    int t=ENGINE->AddWorld(myWorld);
//    printf("world num: %d\n",t);

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

///// грузим скрипты /////
    upload_scripts(SCRIPTER);

    printf("-------- START Script ----------\n");
    SCRIPTER->ExecuteFile("robo_start.lua", NULL);

    ENGINE->EngineStart();

    printf("----End----\n");

    return 0;

};
