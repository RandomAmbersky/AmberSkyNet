// простая демка инициализации и отрисовки

#include "EngineStarter.h"

ICamera *myCam; // указатель на камеру
// нажаты ли клавиши движения
bool k_MoveLeft, k_MoveRight, k_MoveForvard, k_MoveBackward, k_MoveUp, k_MoveDown;
// сдвигали ли мышь
bool MouseMoved;

int main(int argc, char *argv[])
{

k_MoveLeft=k_MoveRight=k_MoveForvard=k_MoveBackward=k_MoveUp=k_MoveDown=false;
MouseMoved=false;

  if (!ENGINE) {  // ENGINE указывает в NULL - загрузить не получилось :(
  fprintf(stderr,"Can't load main Engine module! Exiting...\n");
  exit (1);
  };

  printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

  printf("World created.\n");

  myCam = (ICamera *)ENGINE->CreateObject("CameraSimple");
  if (!myCam) { printf("Class CameraSimple can't found :(\n");  return 1; }

  printf("Camera created.\n");

// позиционируем камеру
  myCam->setPos( CVector(-93,48,126) );
  myCam->setRotY( -32 );

// загружаем в наш мир карту
  //myWorld->LoadWorld( "map.xml" );
  printf("Map loaded.\n");

  int start_param=ASN_DEV_CANRESIZE;

  if ( DEVICER->Init(640,480,0,start_param) ) {
  printf("Ooops... video error!\n");
  return 0; }

  // устанавливаем 3d-режим графики
  DEVICER->SetMode( ASN_GRAPH_3D );

  printf("Devicer [OK]\n");

int evt;
bool non_exit=true;

// пока мы не вышли
while (non_exit){

// читаем первое событие
 evt=EVENTER->GetInput();

// цикл - пока не прочитали все события
 while (evt!=ASN_EVNT_EMPTY) {

 if (evt==ASN_EVNT_KEYPRESSED) { //если событие - нажатие клавиши, то...
    char key=(char)EVENTER->getLastKey();
//    printf("%c\n",key);
     if (key=='d')   k_MoveRight=true;
     if (key=='a')   k_MoveLeft=true;
     if (key=='w')   k_MoveForvard=true;
     if (key=='s')   k_MoveBackward=true;
     if (key=='q')   k_MoveUp=true;
     if (key=='z')   k_MoveDown=true;

// если мы нажали Esc - устанавливаем признак выхода в true
     if (EVENTER->getLastKey()==27) non_exit=false;

     }

// если клавишу отпустили - выключаем движение
 else if (evt==ASN_EVNT_KEYUP) {
    char key=(char)EVENTER->getLastKey();

     if (key=='d')   k_MoveRight=false;
     if (key=='a')   k_MoveLeft=false;
     if (key=='w')   k_MoveForvard=false;
     if (key=='s')   k_MoveBackward=false;
     if (key=='q')   k_MoveUp=false;
     if (key=='z')   k_MoveDown=false;
  }



else if (evt==ASN_EVNT_MOUSEMOVE) MouseMoved=true;

// читаем следующее событие
  evt=EVENTER->GetInput();
 };

// двигаем камеру согласно нажатым клавишам
    if (k_MoveForvard) myCam->MoveForward(50.0f *DEVICER->FrameInterval);
    if (k_MoveBackward) myCam->MoveForward(-50.0f * DEVICER->FrameInterval);
    if (k_MoveLeft) myCam->MoveStrafe(-50.0f * DEVICER->FrameInterval);
    if (k_MoveRight) myCam->MoveStrafe(50.0f * DEVICER->FrameInterval);
    if (k_MoveUp)  myCam->MoveUp(50.0f * DEVICER->FrameInterval);
    if (k_MoveDown) myCam->MoveUp(-50.0f * DEVICER->FrameInterval);

// если двигали мышку - изменяем точку зрения камеры на то, куда указывает курсор
if (MouseMoved) {
    myCam->lookAtMouse(DEVICER->mouse_x,DEVICER->mouse_y);
     // центрируем её..
    DEVICER->CenterMouse(false);
    };


// начинаем рисовать
 DEVICER->StartDraw();

// "смотрим" в нашу камеру
  myCam->View();

  DEVICER->SetMaterial( NULL );

  DEVICER->Draw( DEVICER->GetDrawObject ("_Cube") );

// заканчиваем рисовать
 DEVICER->EndDraw();

// выводим FPS
// printf("FPS: %i\n",DEVICER->FPS);

};

    printf("World deleted\n");

    myCam->Release();
    printf("Camera deleted\n");

    printf("----End----\n");
};
