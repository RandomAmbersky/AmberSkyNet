#include "EngineStarter.h"
//#include "Interfaces/ASNIWorld3D.h"

#include <stdlib.h>

IWorld* myWorld;
ICamera* myCam;

INode* bitok;

enum GAME_STATE{
    GAME_MENU,
    GAME_PLAY,
    GAME_OVER
};

GAME_STATE my_game;

void GameMenu(){
    my_game = GAME_MENU;
    INode* myNode = myWorld->findNode( "Background" );
    if (!myNode) return;
    myNode->SetParam("Texture", "background3.png" );
    printf("GameMenu\n");
};

void GamePlay(){
    my_game = GAME_PLAY;
    INode* myNode = myWorld->findNode( "Background" );
    if (!myNode) return;
    myNode->SetParam("Texture", "background.jpg" );
    printf("GamePlay\n");
};

void GameOver(){
    my_game = GAME_OVER;
    INode* myNode = myWorld->findNode( "Background" );
    if (!myNode) return;
    myNode->SetParam("Texture", "background2.jpg" );
    printf("GameOver\n");
};

bool look_in_mouse=false;
bool k_MouseClick=false;

float FrameInterval=0;

CVector sharik_pos;

#define RANDOM(x) (rand() % (x))

bool UpdateCallback(void *Params)
{



  FrameInterval = DEVICER->FrameInterval;

  int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все события
    while (evt!=ASN_EVNT_EMPTY)
    {

printf("update \n");

       if (evt==ASN_QUIT)
        {
            return true;
        }

        if (evt==ASN_EVNT_KEYUP)
        {

        }

        else if (evt==ASN_EVNT_KEYPRESSED)
        {

            if (EVENTER->getLastKey()==ASNK_ESCAPE)
            {
                if ( my_game == GAME_PLAY ) GameMenu();
                else return true;
            }
            else if (EVENTER->getLastKey()==ASNK_z)
            {

            }
            else if (EVENTER->getLastKey()==ASNK_c)
            {

            }
            else if (EVENTER->getLastKey()==ASNK_x)
            {
                if ( my_game == GAME_MENU) GamePlay();
            }

        };

       evt=EVENTER->GetInput();
    }


   return false;
};

bool DrawCallback(void* Params)
{

printf("state: %d\n", my_game);

char buf[1024];

if ( my_game == GAME_MENU ){

   myCam->View();
   myWorld->Draw();

}
else if ( my_game == GAME_PLAY ){

   myCam->View();
   myWorld->Draw();

   DEVICER->StartMode2D(true);

/*
   DEVICER->StartMode2D(true);
   DEVICER->SetFontSize(0.001);
//CVector4 Color(0,0,0);
 DEVICER->SetColor( CVector4(0,1,1,1) );
   sprintf(buf,"FLAME: %d", score/5);
//   DEVICER->SetFontSize(0.0015);
//   DEVICER->PrintAt(0.02,0.01,"Для конкурса GameDev.ru");
   DEVICER->SetFontSize(0.001);
   DEVICER->PrintAt(0.01,0.9,buf);
   sprintf(buf,"LIVES: %d", lives);
   DEVICER->PrintAt(0.8,0.9,buf);
*/

/*   CVector pos=myCam->getPos();
  float RotX, RotY;
    myCam->getRot(RotX, RotY);
    sprintf(buf,
            "FPS: %d    X:%5.1f   Y:%5.1f   Z:%5.1f   rX:%5.1f   rY:%5.1f",
            DEVICER->FPS,
            pos.v[0], pos.v[1], pos.v[2],
            RotX, RotY
    );
    DEVICER->PrintAt(20,410,buf);
*/

   DEVICER->EndMode2D();

}
else if ( my_game == GAME_OVER ) {
   myCam->View();
   myWorld->Draw();
}

   return true;
};

int main(int argc, char *argv[]){

  ENGINE_INIT(argc, argv);

  if (!ENGINE) {  // ENGINE указывает в NULL - загрузить не получилось :(
  fprintf(stderr,"Can't load main Engine module! Exiting...\n");
  exit (1);
  };


//  myWorld = ENGINE->CreateWorld("World");
//  if (!myWorld) { LogError(0,"Sorry, WorldSimple-plugin not found :("); return 1; }
//  printf("World created.\n");

  ENGINE->LoadDataFile("pole_dat.xml");

  myWorld = ENGINE->GetWorld();

  bitok = myWorld->findNode( "Bitok" );

//  UINT cam_id = myWorld->addCamera("CameraSimple");
  myCam = myWorld->getCamera(0);

  myCam->setPos( CVector(50.7,9.9,81.4) );
  myCam->setRotX( 90.0 );
  myCam->setRotY( -72.0 );
  myCam->setScreen(0,0,1,1);

// загружаем в наш мир карту
//  myWorld->LoadWorld( "pole.xml" );
//printf("Map loaded.\n");

//return 0;

  ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
  ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

//  bitok = CreateBaseNode("Bitok");
//  sharik = CreateBaseNode("Sharik");
//  backgr = CreateBaseNode("Back");

//IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
//myList->SetPtrParam("Camera",myCam);
//myList = EVENTER->AddEventListener( "EvListCamLooker" );
//myList->SetPtrParam("Camera",myCam);

  if(SOUND) SOUND->PlayMusic("alehandro.wav");

  GameMenu();

  ENGINE->EngineStart();

   printf("----End----\n");

return 0;
};
