/// наблюдатель миров из XML-файлов
#include <math.h>

#include "EngineStarter.h"
#include "Interfaces/ASNIGrid2D.h"

#define RANDOM(x) (rand() % (x))

IWorld *myWorld; // указатель на мир
ASNIGrid2D* myMap; // указатель на 2d-карту

ICamera *myCam; // указатель на камеру

bool look_in_mouse=true;
bool k_MouseClick=false;

std::string MapName;

INode* selectNode=NULL; // выбранная шашка
int select_x;
int select_y;

void FindMouseCell(){

int pos_x;
int pos_y;

// проверяем, если не щелкнули по сетке мышкой - выходим
if ( !myMap->FindMouseCell( DEVICER->mouse_x, DEVICER->mouse_y, &pos_x, &pos_y ) ) return;

    INode* myNode = myMap->GetCell( pos_x, pos_y );
    if (myNode) {
        printf( "%d %d %s\n", pos_x, pos_y, myNode->GetParam("GameColor") );
        std::string GameColor = myNode->GetParam("GameColor");
        // если это шашка - запоминаем ее
        if ( ( GameColor=="White") || (GameColor=="Black") ) {
            selectNode = myNode;
            select_x = pos_x;
            select_y = pos_y;
            }
        return;
        }

    // если у нас была выбрана шашка - проверяем на корректность хода
    if (selectNode){
        printf( "%d %d %s\n", pos_x, pos_y, selectNode->GetParam("GameColor") );
        // если щелкнули рядом - передвигаем
        if ( fabs(pos_x-select_x)==1 )
        if ( fabs(pos_y-select_y)==1 )
        {
            myMap->MoveCell(select_x, select_y, pos_x, pos_y );
            select_x = pos_x;
            select_y = pos_y;
        }


    }

};

// Draw-функция
bool DrawCallback(void* Params)
{

    //рисуем наш мир
    //myWorld->Draw();

    //myCam2->setPos(myCam->getPos()-CVector(1,1,1));

    //"смотрим" в нашу камеру
    //myCam2->View();
    //рисуем наш мир

    // "смотрим" в нашу камеру. это надо для корректного определения
    // отрезка "протыкающего" мир в FindMousePointer

    myCam->View();

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
        FindMouseCell();
        //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    myWorld->Draw();

//    if ( myWorld->Draw() ) myCam->View();

    CVector4 color = CVector4( 0, 1, 1, 1 );
    CVector4 color2 = CVector4( 1, 0, 1, 1 );

    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"FPS: %d ",DEVICER->FPS);
    DEVICER->SetColor(color2);
//    DEVICER->PrintAt(0,0,buf);

    DEVICER->SetColor(color);
//    DEVICER->PrintAt(1,1,buf);

    return true;
};

// Update-функция, если возвратит true- произойдет выход из цикла Engine
bool UpdateCallback(void *Params)
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

            if (EVENTER->GetLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }


        }

        else if (evt==ASN_EVNT_MOUSEDOWN)
        {
            k_MouseClick=true;
        }
        evt=EVENTER->GetInput();
    }

    return false;

};

int main(int argc, char *argv[])
{

    MapName="gridder.xml";

    ENGINE_INIT(argc, argv);

    if (!ENGINE)
    {  // ENGINE указывает в NULL - загрузить не получилось :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        return 1;
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

    //////////////// TEST //////////////////

    // создаём менеджер мира
    myWorld = (IWorld *)ENGINE->CreateWorld("WorldSimple3D");
    if (!myWorld)
    {
        printf("Sorry, WorldSimple3D-plugin not found :(\n");
        return 1;
    }

    printf("World created.\n");

    myCam = myWorld->GetCamera( myWorld->AddCamera("CameraSimple") );
    myCam->SetScreen(0,0,1,1/*0.5*/);

    // добавляем "Camera Walker" listener
    IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera walker created.\n");

    // добавляем "Camera Looker" listener
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
    myCam->SetPos( CVector(-93,48,126) );
    myCam->SetRotY( -32 );

    // загружаем в наш мир карту
    FILER->AddPath("samples/Gridder/", false);
    ENGINE->LoadDataFile( MapName.c_str(), myWorld );
//    myWorld->LoadWorld( MapName.c_str() );
    printf("Map loaded.\n");

    INode* myNode = dynamic_cast<INode*>( myWorld->FindNode( "Setka" ) );
    if (!myNode)
    {
        printf("Setka can't found :(\n");
        return 1;
    }

    myMap = (ASNIGrid2D*)myNode->GetInterface( "ASNIGrid2D" );
    if (!myMap)
    {
        printf("Can't get interface ASNIGrid2D :(\n");
        return 1;
    }


bool p_x=false;
    for (int j=0; j< 3; j++) {
        for (int i=1; i< 8; i++){
           IBaseObject* myObj = ENGINE->CreateObject( "WhiteCS" );
           myNode = dynamic_cast<INode*>( myObj );
           if (!myNode) { myObj->Release(); continue; }
            myNode->SetParam("GameColor", "White");
            myMap->SetCell(i-p_x, j, myNode);
            i++;
            }
            p_x=!p_x;
    }


p_x=false;
    for (int j=5; j< 8; j++) {
        for (int i=1; i< 8; i++){
            IBaseObject* myObj = ENGINE->CreateObject( "BlackCS" );
            myNode = dynamic_cast<INode*>( myObj );
            if (!myNode) { myObj->Release(); continue; }
            myNode->SetParam("GameColor", "Black");
            myMap->SetCell(i-p_x, j, myNode);
            i++;
            }
            p_x=!p_x;
    }

    bool non_exit=true;
    selectNode=NULL;

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

    ENGINE->EngineStart();

    printf("----End----\n");

    return 0;
};
