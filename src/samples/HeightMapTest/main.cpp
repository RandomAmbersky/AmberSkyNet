/// демка работы с картой высот

#include <stdlib.h>
#include "EngineStarter.h"

#define RANDOM(x) (rand() % (x))

//#include "GL/gl.h"

#include "Interfaces/ASNITerrain.h"
#include "Interfaces/ASNIWorld3D.h"

IWorld *myWorld; // указатель на мир
ICamera *myCam; // указатель на камеру
//*ICamera *myCam2; // указатель на камеру*/

// ссылка на карту и размеры
float *customMap;
int width;
int height;

//float detail;
//float dt_detail;

bool gravity_on;
bool look_in_mouse;
bool k_MouseClick;
float FOV;

bool proceed; // обрабатывать террайн или нет

/*
void CreateCustomMap(){

    if(customMap) {free(customMap);
    printf("free data map ok...\n");
    };

    customMap = new float [ 100 * 100 ];
    printf("create new data map ok...\n");

    for (int i=0;i<100;i++)
    for (int j=0;j<100;j++){
        customMap[i*100+j]= RANDOM(50)/50.0;
        };

    printf("map data fill ok...\n");

};

void LoadMap(){

    if(customMap) {free(customMap);
    printf("free data map ok...\n");
    };

    INode* terrainNode = myWorld->findNode("myTerrain");

    printf("find node ok..\n");

    // получаем интерфейс
    ASNITerrain *myTerrain=(ASNITerrain*)terrainNode->getInterface("ASNITerrain");
    printf("get interface ok..\n");

    if (!myTerrain) printf("NOT IMPLEMENTED INTERFACE!s");

    const float* data_map=myTerrain->GetHeightMap(width,height);
    printf("upload height into map ok...\n");

    customMap = new float [ width * height ];
    printf("create new data map ok...\n");

    for (int i=0;i<height;i++)
    for (int j=0;j<width;j++){
        customMap[i*width+j]= data_map[i*width+j];
        };

    printf("map data fill ok...\n");

};

void ProceedCustomMap(){

    INode* terrainNode = myWorld->findNode("myTerrain");

    printf("find node ok..\n");

    // получаем интерфейс
    ASNITerrain *myTerrain=(ASNITerrain*)terrainNode->getInterface("ASNITerrain");
    printf("get interface ok..\n");

    if (!myTerrain) printf("NOT IMPLEMENTED INTERFACE!s");

    int old_width = width;
    int old_height = height;
    const float* data_map=myTerrain->GetHeightMap(width,height);

    if ( (width!=old_width) || (height!=old_height) ) {
        if (customMap) free(customMap);
        customMap = new float [width * height ];
        memset(customMap,width * height * sizeof(float),0 );
    };

    for (int i=1;i<height-1;i++)
    for (int j=1;j<width-1;j++){
        customMap[i*width+j]= data_map[i*width+j-1]+data_map[(i-1)*width+j]+data_map[i*width+j]+data_map[i*width+j+1]+data_map[(i+1)*width+j];
        customMap[i*width+j]/=6.0;
//        if (RANDOM(10)>9)
//        customMap[i*100+j]+=RANDOM(10)/10.0;
//        if (RANDOM(10)<1)
//        customMap[i*100+j]-=RANDOM(10)/10.0;

//        if (customMap[i*100+j]>1000.0) customMap[i*100+j]=1000.0;
//        if (customMap[i*100+j]<-10.0) customMap[i*100+j]=-10.0;

        };

//    printf("map data proceed ok...\n");

};

void UploadCustomMap(){

    INode* terrainNode = myWorld->findNode("myTerrain");

    printf("find node ok..\n");

    // получаем интерфейс
    ASNITerrain *myTerrain=(ASNITerrain*)terrainNode->getInterface("ASNITerrain");
    printf("get interface ok..\n");

    if (!myTerrain) printf("NOT IMPLEMENTED INTERFACE!\n");

    myTerrain->UploadHeightMap(width,height,customMap);
    printf("upload height into map ok...\n");

};

// найти куда указывает курсор мышки и проверить
// что именно там находится
void FindMousePointer(UINT x,UINT y)
{

    printf("try upload..\n");

    CVector Start=CVector();
    CVector End=CVector();

    // переводим координаты курсора мышки в отрезок мира
    DEVICER->MouseToWorld(x,y,Start,End);
    asn_ext_ptr_list* myList = new asn_ext_ptr_list();

    // получаем интерфейс
    ASNIWorld3D *my3dWorld=(ASNIWorld3D*)myWorld->getInterface("ASNIWorld3d");

    my3dWorld->getNodesByPos(Start,End,myList);

    if ( myList->nums>0)
    {
        //printf("nums: %d\n",myList->nums);
        for ( UINT i=0; i<myList->nums; i++ )
        {

            printf("Name: %s(%d)\n", ((INode*)myList->ptr_list[i].pointer)->GetName(),
                                    ((INode*)myList->ptr_list[i].pointer)->getID()
             );
            //if ( std::string( ((INode*)myList->ptr_list[i].pointer)->GetParam("CanDel") ) == "Yes" )
            {
            //DestroyFlyingObject( (INode*)myList->ptr_list[i].pointer );
            //deletePtrList(myList);

            //((INode*)myList->ptr_list[i].pointer)->SetParam("Texture","Brick wall.jpg");

            return;
            }

        };
    };

    deletePtrList(myList);

};

*/
bool get_matrix=false;

// Draw-функция
int DrawCallback(void* Params)
{

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
    // "смотрим" в нашу камеру. это надо для корректного определения
    // отрезка "протыкающего" мир в FindMousePointer
//        FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    //DEVICER->SetScale( CVector(0.1,0.1,0.1) );
    myCam->View();
    myWorld->Draw();

    CVector pos=myCam->GetPos();

    //DEVICER->PopMatrix();

    //DEVICER->SetFontSize(1.0);
    char buf[1024];  // вроде должно хватить :)
    sprintf(buf,"FPS: %d X:%f Y:%f Z:%f",DEVICER->FPS,pos.v[0],pos.v[1],pos.v[2]);

//    DEVICER->PrintAt(0,0,buf);
    DEVICER->StartMode2D(false);

    int s_width;
    int s_height;

 sscanf(ENGINE->GetEnvParam("[DeviceManager]width"),"%d",&s_width);
 sscanf(ENGINE->GetEnvParam("[DeviceManager]height"),"%d",&s_height);

    float text_width=10;
    float text_height=10;

    //DEVICER->SetFontSize(2.0);
   DEVICER->GetTextWH(buf,text_width,text_height);
   float pos_x=((float)s_width-text_width)/2.0;
   float pos_y=((float)s_height-text_height)/2.0;
   CVector4 Color_a = CVector4(1,1,0,1);
   CVector4 Color_b = CVector4(1,0,0,0.5);

    DEVICER->SetColor(Color_a);
    DEVICER->PrintAt(pos_x,text_height,buf);

    DEVICER->EndMode2D();

    return true;
};

// Update-функция, если возвратит true- произойдет выход из цикла Engine
int UpdateCallback(void *Params)
{

//if (proceed) {
    //LoadMap();
//    ProceedCustomMap(); UploadCustomMap(); }

//UploadCustomMap();

//    PHYSICS->Update(0.5);

    //printf("update\n");

    // читаем первое событие
    int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все события
    while (evt!=ASN_EVNT_EMPTY)
    {

        if (evt==ASN_MOUSE_WHEEL_UP) {
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
            else if (EVENTER->GetLastKey()==ASNK_SPACE) {  gravity_on=!(gravity_on);/*LoadMap(); ProceedCustomMap(); UploadCustomMap();*/ }

            else if (EVENTER->GetLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string tmpPath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
//                myWorld->SaveWorld(tmpPath.c_str());
                }

            else if (EVENTER->GetLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
//                myWorld->LoadWorld("World.xml");
                }

            // если мы нажали PrtSrc - делаем скриншот с именем screenshotXXX
            else if (EVENTER->GetLastKey()==316)
            {
                DEVICER->MakeScreenshot("screenshot");
                printf("Shoot!\n");
            }

            else if (EVENTER->GetLastKey()==9)
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

////////////// вычисляем высоту камеры  /////////////

    CVector pos = myCam->GetPos();
    INode* terrainNode = dynamic_cast<INode*>( myWorld->FindNode("myTerrain") );
    if ( terrainNode) {
    ASNITerrain *myTerrain=(ASNITerrain*)terrainNode->GetInterface("ASNITerrain");
//    printf("get interface ok..\n");

    if (!myTerrain) printf("NOT IMPLEMENTED INTERFACE!\n");

     CVector pos = myCam->GetPos();

    const float height_ptr = myTerrain->GetHeight(pos.v[0],pos.v[1]);
    if ( !isnan(height_ptr) ) {
         if ( (gravity_on) || ( pos.v[2] < (height_ptr+10.0f)) ) pos.v[2]=height_ptr+10.0f;
        myCam->SetPos(pos);
    }

    }
//////////////////////////////////////////////////////////

    return false;

};

int main(int argc, char *argv[])
{

    //std::map<std::string, std::string> Mappp;

    look_in_mouse=false;
    k_MouseClick=false;
    FOV = 60.0f;
    customMap = NULL;
//    CreateCustomMap();
    proceed=false;
//    width=0;
//    height=0;
    gravity_on=false;

    std::string HeightMap="";
    std::string Texture="";
    std::string Detail="100.0";

    ENGINE_INIT(argc, argv);

    if (!ENGINE)
    {  // ENGINE указывает в NULL - загрузить не получилось :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        exit (1);
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

/*
    //////////////// TEST //////////////////
    ENGINE->SetEnvParam("TestParam1","OK");
    printf("Get TestParam1: %s\n", ENGINE->GetEnvParam("TestParam1") );
    //////////////// TEST //////////////////

    // создаём менеджер мира
    myWorld = (IWorld*)ENGINE->CreateWorld("WorldSimple3D");
    if (!myWorld)
    {
        printf("Sorry, WorldSimple-plugin not found :(\n");
        return 1;
    }

    printf("World created %s .\n", myWorld->GetType());

    myCam = myWorld->getCamera( myWorld->addCamera("CameraSimple") );
    myCam->setScreen(0,0,1,1);
    myCam->getPos();

    /*    // делаем еще одну камеру 8)
        myCam2 = myWorld->getCamera(myWorld->addCamera("CameraSimple"));
        myCam2->setScreen(0,0.5,1,0.5);
        myCam2->setPos( CVector(-100,48,126) );
        myCam2->setRotY( -32 );

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

    myCam = myWorld->getCamera(0);
    // позиционируем камеру
    myCam->setPos( CVector(-93,48,126) );
    myCam->setRotY( -32 );

//    myWorld->LoadWorld( "samples/HeightMapTest/terrain.xml" );
    ENGINE->LoadDataFile("samples/HeightMapTest/terrain.xml");
    printf("Map loaded.\n");
//    return 0;

*/

    ENGINE->LoadDataFile("samples/HeightMapTest/terrain.xml");

    myWorld = ENGINE->GetWorld();

    if (!myWorld) {
        printf("Can't get world! Exiting... :(\n");
        return 1;
    };

    printf("World created %s .\n", myWorld->GetType());


    myCam = myWorld->GetCamera(0);
    // позиционируем камеру
    myCam->SetPos( CVector(-93,48,126) );
    myCam->SetRotY( -32 );

///// разбор параметров входной строки
	for(int i = 1; i < argc; i++) {

		if(strcmp(argv[i], "-map") == 0) {
			HeightMap=argv[i+1];
			continue;
		}

		if(strcmp(argv[i], "-texture") == 0) {
			Texture=argv[i+1];
			continue;
		}

		if(strcmp(argv[i], "-detail") == 0) {
			Detail=argv[i+1];
			continue;
		}

};

    INode* terrainNode = dynamic_cast<INode*>( myWorld->FindNode("myTerrain") );
    if (terrainNode) {
    if ( HeightMap!="") terrainNode->SetParam("HeightMap",HeightMap.c_str());
    if ( Texture!="") terrainNode->SetParam("Texture",Texture.c_str());
    terrainNode->SetParam("Detail",Detail.c_str());
    }
//    else return 1;

//    myCam->setPerspective(FOV, (float)s_width/s_height /*4.0f/3.0f*/, 0.1f, 1e5f);

    int evt;
    bool non_exit=true;

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

    ENGINE->EngineStart();

    printf("----End----\n");

    return 0;
};
