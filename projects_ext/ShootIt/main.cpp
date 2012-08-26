//Разница между умным человеком и дураком в том, 
// что дурак повторяет чужие глупости, а умный придумывает свои. 
//Неизв

#include <iostream>
#include <fstream>

#include <stdlib.h>
#include "CVector.h"

#include "EngineStarter.h"

#include "Interfaces/ASNIWorld3D.h"

//#include "IDevice.h"
//#include "IDevice.h"
//#include "ILog.h"
//#include "ISound.h"

#include "ICamera.h"

#include "../../src/asnCommon/ASN_utils.h"
//#include "ASN_utils.h"

#include "INode.h"
#include "IWorld.h"

#ifdef _WIN32
#include <windows.h>
#endif


#define RANDOM(x) (rand() % (x))

using namespace std;

//std::string Result;

//const std::string& FloatToString(const float& vec){
//
//sprintf(buf,"%f",vec);
//Result=std::string (buf);
//return Result;

//};
//
//float StringToFloat(const std::string& str){
//float a;
//if (sscanf(str.c_str(),"%f",&a)!=1) a=0;
//return a;
//};

//const std::string VectorToString(const CVector vec){
//
//sprintf(buf,"%f %f %f",vec.v[0],vec.v[1],vec.v[2]);
//Result=std::string (buf);
//return Result;
//
//};

//const CVector StringToVector(const std::string& str){
//
//float a, b,c;
//if (sscanf(str.c_str(),"%f %f %f",&a,&b,&c)!=3) return 
//CVector( 0,0,0 );
//return CVector(a,b,c);
//
//};

bool k_MouseClick; //кликнули мышкой 

float BaseRotSpeed; // максимальная скорость вращения 
float BaseMoveSpeed; // максимальная скорость перемещения

int PointSumm; // набранные очки
int ItemsNum; // сколько предметов на сцене
int MaxLevel; // максимальный уровень

bool SoundEnabled;
bool animate;
bool need_exit;

float WorldSpeed;

int CamCountMoved; //сколько раз мы "дёрнули" камеру

float AverageFPS; // средний FPS

std::string MusicName;

IWorld *myWorld; // указатель на мир
ICamera *myCam;
IFile * remFile;

int start_param=ASN_DEV_CANRESIZE;
// создать новый летающий предмет по имени
int width = 640;
int height = 480;

char buf[1024];  // вроде должно хватить :)

INode* CreateBaseNode(std::string NodeType) {
    UINT tmpId=myWorld->createNode( NodeType.c_str() );
//    printf("CreateBaseNode ID: %d\n",tmpId);
    return myWorld->getNode( tmpId );
};

// уничтожить летающий объект
void DestroyFlyingObject(INode *node,bool Explose){

    if (Explose) {
     if (SoundEnabled) SOUND->PlayFX("Shot.wav");
    INode *tmpNode=CreateBaseNode("Explode");
    tmpNode->setPos ( node->getPos() );
    }
    else if (SoundEnabled) SOUND->PlayFX("ball01.wav");
    delete node;
    ItemsNum--;
};

// установить случайным образом позицию, угол наклона и скорости движения-вращения 
void SetRandomParams(INode *myNode){
if (!myNode) return;

    CVector Pos = CVector( 5, RANDOM(70)+10, 100 );
    CVector Rot = CVector( 0*RANDOM(360), 0*RANDOM(360), 0*RANDOM(360) );
//printf("ok.. try set params..\n");

    myNode->setPos(Pos);
    myNode->setRot(Rot);

//    CVector Tor_AnglesSpeed = CVector( 1*BaseRotSpeed*rand(), 1*BaseRotSpeed*rand()/100.0f, 0*BaseRotSpeed*rand()/100.0f );
//    CVector Tor_PosSpeed = CVector( 0*BaseMoveSpeed*rand()/1000.0f, 0*BaseMoveSpeed*rand()/1000.0f, -1*BaseMoveSpeed*rand()/1000.0f );

//    cout << BaseRotSpeed << endl;
    CVector Tor_AnglesSpeed = CVector( BaseRotSpeed*RANDOM(100), 0*BaseRotSpeed*RANDOM(100), 0 );
    CVector Tor_PosSpeed = CVector( 0, 0, -BaseMoveSpeed*RANDOM(30) );

//    Tor_AnglesSpeed.v[0] = BaseRotSpeed*rand()/1000.0f;
//    Tor_AnglesSpeed.v[1] = BaseRotSpeed*rand()/1000.0f;
//    Tor_AnglesSpeed.v[2] = BaseRotSpeed*rand()/1000.0f;

    cout << " AnglesSpeed " << endl;
//    float tmp = RANDOM(10);
//    cout << BaseRotSpeed << endl;
//    cout << tmp << "==" << FloatToStr( tmp ) << rand() << endl;
//    cout << rand() << "==" << BaseRotSpeed*100.0 << endl;

//    cout << Tor_AnglesSpeed.v[0] << endl;
//    cout << Tor_AnglesSpeed.v[1] << endl;
//    cout << Tor_AnglesSpeed.v[2] << endl;

// пока классов модификаторов элементов нет, храним и вычисляем всё "руками"
// цепляем на ноду параметры угловой и позиционной скорости
    myNode->SetParam( "AnglesSpeed", VectorToStr(Tor_AnglesSpeed).c_str() );

cout << VectorToStr(Tor_AnglesSpeed).c_str() << endl;

    myNode->SetParam( "DownSpeed", FloatToStr(Tor_PosSpeed.v[2]).c_str() );

    myNode->SetParam("CanDel","Yes"); // объект может быть уничтожен кликом "мышки"
};

void AddFlyingObject(){

// случайно выбираем летящий предмет

UINT state=RANDOM(6);

     if (state==0) SetRandomParams( CreateBaseNode("Axe") );
else if (state==1) SetRandomParams( CreateBaseNode("Column") );
else if (state==2) SetRandomParams( CreateBaseNode("Stairs") );
else if (state==3) SetRandomParams( CreateBaseNode("Helmet") );
else if (state==4) SetRandomParams( CreateBaseNode("Sword") );
else if (state==5) SetRandomParams( CreateBaseNode("Chair") );
else if (state==6) SetRandomParams( CreateBaseNode("Bogat") );

ItemsNum++;

};

void FindMousePointer(UINT x,UINT y){

CVector Start;//=CVector();
CVector End;//=CVector();

// переводим координаты курсора мышки в отрезок мира
DEVICER->MouseToWorld(x,y,Start,End);

// получаем список нод, которые наш отрезок "проткнул"
//NodeList_typ listNode=myWorld->getNodesByPos(Start,End);
asn_ext_ptr_list* myList = createPtrList();
ASNIWorld3D *my3dWorld=(ASNIWorld3D*)myWorld->getInterface("ASNIWorld3d");
my3dWorld->getNodesByPos(Start,End,myList);

//if ( listNode.size()>0) {

bool missed = true;

if ( myList->nums>0){

//    NodeList_typ::iterator it=listNode.begin();
    for ( UINT i=0; i<myList->nums; i++ ){

	INode* myNode = (INode*)myList->ptr_list[i].pointer;

        if ( std::string( myNode->GetParam("CanDel") ) == "Yes" ) {
                PointSumm+=11; // 11, т.к.  одно всё равно вычтут ниже :)
                missed = false;
                DestroyFlyingObject( myNode, true );
                break;
                }
    };

};// else CamCountMoved=10; // НЙНП



    if (animate)
    if (missed) {
	PointSumm-=1;  //ничего не нашли, значит не попали мы никуда
	if ( RANDOM(10) <3 ) CamCountMoved=2;
	if (SoundEnabled) SOUND->PlayFX("explos.wav");
	}
    // и это только в режиме игры, а не меню 
};


int DrawCallback(void* Params)
{

if (CamCountMoved!=0) {
CamCountMoved--;
CVector my_pos = CVector(-93.0 ,48.0 ,126.0 );
//cout << pos.v[0] << endl;
my_pos+=CVector( RANDOM(5), RANDOM(5), RANDOM(5) );
if (CamCountMoved==0) myCam->setPos( CVector(-93,48,126) );
else
myCam->setPos(my_pos);
};

// начинаем рисова-а-а-ать
//DEVICER->StartDraw();

myCam->View();

if ( k_MouseClick /*& !look_in_mouse*/ ) {
//        myCam->View();
        FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        k_MouseClick=false;
        if (!animate) animate=true;
    }

if (animate)
myWorld->Draw();

// ) myCam->View();

// после того, как на сцене установлен вид из камеры
// можно узнать - на какой объект указывает наш курсор мыши
// конечно, если режим обзора мышкой не включен, тогда курсора не видно


//if (k_MouseClick) {
//    FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
//    k_MouseClick=false;
//    };

// myWorld->Draw();

if ( PointSumm<0 ) PointSumm=0;

 DEVICER->PushMatrix();
 DEVICER->Rotate( CVector(90,-90,0) );
 DEVICER->SetScale( CVector(0.5,0.5,0.5) );

if (animate) {
 sprintf(buf,"Level %d ", MaxLevel);
 DEVICER->PrintAt(-95*2,95*2,buf);
 sprintf(buf,"Points: %d", PointSumm);
 DEVICER->PrintAt(-95,0,buf);
  }
else
 DEVICER->PrintAt(-180,130,"Click to continue...");
 DEVICER->PopMatrix();

// DEVICER->EndDraw();

    return true;
}

int UpdateCallback(void *Params)
{

//myWorld->getMaxID();
//if (myCam) myCam->getPos();
//myCam->lookAtMouse( 0, 0 );
//CVector my_pos = CVector(-93,48,126);
//return false;

int evt=EVENTER->GetInput();

//return false;

while (evt!=ASN_EVNT_EMPTY) {

// если курсор мышки вышел из окна - переходим на паузу
  if (evt==ASN_WINDOW_MOUSE_FOCUS_OFF) {
  animate=false;
  };

  if (evt==ASN_EVNT_KEYPRESSED) {
    int key= EVENTER->getLastKey();

     if (key=='f')   {
                    start_param = start_param ^ ASN_DEV_FULLSCREEN;
                    DEVICER->Init(width,height,0,start_param);
//                    DEVICER->Init(width,height,0,start_param);
                    DEVICER->SetMode(ASN_GRAPH_3D);
                    animate=false;
                    }

     if (key=='p')   animate=!animate; //пауза анимации

//     if (key==286) { printf("SaveWorld\n"); std::string tmpPath=ENGINE->GetEnvParam("[Paths]DataPath")+"World.xml"; myWorld->SaveWorld(tmpPath.c_str());  }
//     if (key==287) { printf("LoadWorld\n"); myWorld->LoadWorld("World.xml"); }

     if (key==316) { DEVICER->MakeScreenshot("screenshot"); printf("Shoot!\n"); }

     printf("Key %d\n",key);

     if (key==27) return true;

  }

// кликали мышкой
   else if (evt==ASN_EVNT_MOUSEDOWN) k_MouseClick=true;

   evt=EVENTER->GetInput();

};

// тут мы обработали все события, которые были на данный момент

if (animate) 
for (uint i=0;i<myWorld->getMaxID();i++) {

INode *myNode= myWorld->getNode( i+1 ); // 0 - корневая нода, недоступна нам
float frame_dt = DEVICER->FrameInterval;
//cout << frame_dt << endl;

    if (myNode)
    {

    CVector Pos = myNode->getPos();
    CVector Rot = myNode->getRot();

    CVector Tor_AnglesSpeed = StrToVector ( myNode->GetParam( "AnglesSpeed" ) );
    float DownSpeed=StrToFloat( myNode->GetParam( "DownSpeed" ) );

    CVector Anim = Tor_AnglesSpeed*frame_dt;
    Rot+=Anim*WorldSpeed;

    Pos.v[2]+=DownSpeed*WorldSpeed*frame_dt;
//    cout << Pos.v[2] << endl;

     if (Pos.v[2]<=0) {
          PointSumm-=50;
          CamCountMoved=10;
          DestroyFlyingObject(myNode, false);
          AddFlyingObject();
          continue;
          }

     myNode->setPos( Pos );
     myNode->setRot( Rot );

    } //else printf( "%d IS NULL!!!!\n",i );

};

if ( PointSumm<0 ) PointSumm=0;
if ( ItemsNum < (1+PointSumm/100) ) AddFlyingObject();
if ( ItemsNum < 1 ) AddFlyingObject();
if (MaxLevel<ItemsNum) MaxLevel=ItemsNum;

//cout << CamCountMoved << endl;

//CVector my_pos = CVector(-93,48,126);
//CVector(-93,48,126);
//myCam->getPos();// = CVector(-93.0 ,48.0 ,126.0 );

//if (CamCountMoved!=0) {
//CamCountMoved--;
//CVector my_pos = 
//myCam->getPos();// = CVector(-93.0 ,48.0 ,126.0 );
//cout << pos.v[0] << endl;
//pos+=CVector( CamCountMoved*rand()/10000.0f, CamCountMoved*rand()/10000.0f, CamCountMoved*rand()/10000.0f );
//if (CamCountMoved==0) myCam->setPos( CVector(-93,48,126) );
//else
//myCam->setPos(my_pos);
//};

//AverageFPS+=(float)DEVICER->FPS;
//AverageFPS/=2.0f;

    remFile->LoadInMem();
    return false;
}


int main(int argc, char *argv[])
{

    ENGINE_INIT(argc, argv);

PointSumm=0;
ItemsNum=0;
MaxLevel=0;

WorldSpeed = 1.0f;
CamCountMoved=0; //пока дёргать не надо 

AverageFPS = 0.0f;

BaseRotSpeed =  1.0f ; // максимальная скорость вращения 
BaseMoveSpeed =  1.0f; // максимальная скорость перемещения

animate=false;
MusicName=ASN_NOT_FOUND_PARAM;


  if (!ENGINE) {  // ENGINE указывает в NULL - загрузить не получилось :(
  fprintf(stderr,"Can't load main Engine module! Exiting...\n");
  exit (1);
  };

 printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);


//FILER->AddPath("http://172.16.24.14/", false);

//remFile = FILER->GetFile("http://172.16.24.14/octopus.jpg");
//if (!remFile) {  // ENGINE указывает в NULL - загрузить не получилось :(
//  fprintf(stderr,"Can't load octopus.jpg..\n");
//  exit (1);
//  };


//while( true) {

//    myFile->LoadInMem();
//    cout << myFile->GetState() << endl;
//}
//myFile->LoadInMem();
//myFile->LoadInMem();

//myFile->LoadInMem();

//myFile->LoadInMem();

//myFile->LoadInMem();

//return 0;


///////////// GAME ///////////////

sscanf(ENGINE->GetEnvParam("[DeviceManager]Width"),"%d",&width);
sscanf(ENGINE->GetEnvParam("[DeviceManager]Height"),"%d",&height);

if ( strcmp(ENGINE->GetEnvParam("[SoundManager]SoundEnabled"),"1")==0 ) SoundEnabled = true;
else SoundEnabled = false;

if ( !SOUND ) SoundEnabled = false;

////cout << ENGINE->GetEnvParam("[SoundManager]SoundEnabled") << endl;

cout << SoundEnabled << endl;
//SoundEnabled = true;
//myCam = (ICamera *)ENGINE->CreateObject("CameraSimple");
//if (!myCam) {
//printf("Class CameraSimple can't found :(\n");
//return 1;
//}

///// разбор параметров входной строки

/*
	for(int i = 1; i < argc; i++) {

/// включаем полноэкранный режим
		if(strcmp(argv[i], "-fullscreen") == 0) {
			start_param = start_param || ASN_DEV_FULLSCREEN;
			continue;
		}

///  разрешение изменяем на то, что получили в строке, например -res 1024 768
		if(argc >= i + 2 && strcmp(argv[i], "-res") == 0) {
			width = atoi(argv[i+1]);
			height = atoi(argv[i+2]);
			continue;
		}

// по идее не понадобится совсем нам...
		if(argc >= i + 2 && strcmp(argv[i], "-set") == 0) {
		    ENGINE->SetEnvParam( argv[i+1] , argv[i+2] );
			continue;
		}

		if(argc >= i + 2 && strcmp(argv[i], "-music") == 0) {
		    MusicName=argv[i+1];
			continue;
		}
		
		if( argv[i][0]=='-'){
		    printf("Hmm.. i don't know %s\n",argv[i]);
		    continue;
		};
	}
*/

//return 0;

/*
if ( DEVICER->Init(width,height,0,start_param) ) {
printf("Ooops... video error!\n");
return 0; }

printf("Devicer [OK]\n");
*/

if (MusicName==ASN_NOT_FOUND_PARAM)
    MusicName=ENGINE->GetEnvParam("[SoundManager]MusicFile");
if (SoundEnabled) SOUND->PlayMusic( MusicName.c_str() );


//printf("Init video...\n");
//DEVICER->SetMode(ASN_GRAPH_3D);
//printf("Create world...\n");

 myWorld = (IWorld *)ENGINE->CreateWorld("WorldSimple3D");
 if (!myWorld) {printf("Sorry, WorldSimple-plugin not found :(\n"); return 0;}

 UINT cam_id = myWorld->addCamera("CameraSimple");
 myCam = myWorld->getCamera(cam_id);
 myCam->setScreen(0,0,1,1/*0.5*/);

// позиционируем камеру 
myCam->setPos( CVector(-93,48,126) );
myCam->setRotY( -32 );



// загружаем карту
myWorld->LoadWorld( "map.xml" );
//int t=ENGINE->AddWorld(myWorld);

return 0;

/*
// 
//AverageFPS+=(float)DEVICER->FPS;
//AverageFPS/=2.0f;

 myWorld->Update( WorldSpeed* DEVICER->FrameInterval );

if (CamCountMoved!=0) {
CamCountMoved--;
CVector pos=CVector(-93,48,126);
pos+=CVector( CamCountMoved*rand()/10000.0f, CamCountMoved*rand()/10000.0f, CamCountMoved*rand()/10000.0f );
//if (CamCountMoved==0) myCam->setPos( CVector(-93,48,126) );
//else 
myCam->setPos(pos);
};

// sleep(1);
};

*/

//ENGINE->SetCallbackUpd( UpdateCallback );
//ENGINE->SetCallbackDraw( DrawCallback );

ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

need_exit = false;

cout << " <<<ENGINE START<<<<<<<< " << endl;
//ENGINE->EngineStart();
cout << " <<<ENGINE END<<<<<<<< " << endl;

//  delete myWorld;
//  delete myCam;

//  sprintf(buf,"%f", AverageFPS);
//  LOGGER->LogMsg( std::string("Average FPS:"+std::string(buf)).c_str() );
//  printf("Average fps: %s\n",buf) ;
 
  return 0;
}

