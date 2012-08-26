#include <stdlib.h>
#include <vector>
#include <string>

#include "EngineStarter.h"

ITextListMap* TextListMap; // текстовые списки дл€ окон

IWorld *myWorld; // указатель на мир
ICamera *myCam; // указатель на камеру
// нажаты ли клавиши движени€
bool k_MoveLeft, k_MoveRight, k_MoveForvard, k_MoveBackward, k_MoveUp, k_MoveDown;
// сдвигали ли мышь
bool MouseMoved;
bool look_in_mouse; // обзор мышкой

INetProtocolHandler *myNet;
std::string ServerName;

std::string LineName; // им€ линии в которую произойдЄт автозаход
std::string NickName; // ник дл€ IRC
std::string RealName; // реальное им€ пользовател€
std::string UserName; // им€ пользовател€

std::string LastLineName=""; // им€ линии, в которую было отправлено последнее сообщение
std::string Str_con=">_"; // входна€ строка

typedef std::vector<std::string> parList;

std::string JoinParam( parList::iterator _it, parList *Params )
{
std::string OtherParams="";
while(_it!=Params->end()){
//printf("Param:{%s}\n",(*_it).c_str());
OtherParams+=(*_it);
//LOGGER->LogMsg("Param:{"+(*_it)+"}");
_it++;
};
return OtherParams;
};

//extern long ucs2keysym (long ucs);
//extern long keysym2ucs(long keysym);

void ConsoleInput(unsigned short key) {

                    if (key==ASNK_BACKSPACE) {if (Str_con.size()>2) Str_con.erase(Str_con.end()-2); return;}
                    if (key==ASNK_RETURN){
                    	Str_con.erase(Str_con.begin());
                    	Str_con.erase(Str_con.end()-1);
                    	if ((*Str_con.begin())=='/') {
                    		Str_con.erase(Str_con.begin());
//                    		((CEventLog *)MegaBase->GetVoidParam("EventLog"))->LogMsg("%s",Str_con.c_str());
//                    		((IEngine *)MegaBase->GetVoidParam("Engine"))->RunScript(Str_con);
                   		}
//                   		else ((CEventLog *)MegaBase->GetVoidParam("EventLog"))->LogMsg("%s",Str_con.c_str());
//                            printf( "ooohhh: %s\n",Str_con.c_str() );
//                            LOGGER->LogMsg( Str_con );
//                    if (Str_con[0]=='/') {
//                    TextListMap->Add( "server", Str_con );
//                    myNet->Send( "PRIVMSG", "server", Str_con.c_str() );
//                    } else {
                    TextListMap->Add( LastLineName.c_str(), (std::string(myNet->GetParam("NickName"))+": "+Str_con).c_str() );
                    myNet->Send( "PRIVMSG", LastLineName.c_str(), Str_con.c_str() );
//                                        }
                   	Str_con=">_";
              		return;
                    };

	char ch;

                    if (key>127)
                         key = key - 0x410 + 192; // только –”—— »… юникод пока, понимаешь :(

                    ch=(char)key;

                    if ((char)ch){
                    printf("char: %d %c\n",key, ch);
                    Str_con.erase(Str_con.end()-1);
                    Str_con=Str_con+(ch);
                    Str_con+='_';}
		    printf("%s\n", Str_con.c_str() );
                    return;
};


void hanlde_IRC(INetProtocolHandler *handler, void *info){
printf("Custom handler !!!!!!!!!!!!!\n");

parList *Params=(parList *)info;
parList::iterator _it=Params->begin();

if (Params->size()<2) return;

std::string Nick = (*_it);
printf("CH Nick: %s\n",Nick.c_str());
_it++;
std::string Action = (*_it);
printf("CH Action: %s\n",Action.c_str());
_it++;

// если подконнектились к серверу - заходим в линию
if ( Action == "RPL_WELCOME" ) {
    myNet->Send("JOIN", "server", LineName.c_str() );
   };

// автоответчик. можно сделать чат-бота
if ( Action == "PRIVMSG" ) {

    std::string LineName=(*_it);
    LastLineName = LineName;
    _it++;

    // если это сообщение из линии - отвечаем в линию
    if (LineName[0]=='#') {

//    printf("ittt %s\n", (*_it).c_str() );
      std::string Message;

      if ( (*_it)=="ACTION" ) {
            _it++;
            Message=Nick+" "+JoinParam(_it,Params);
            }
      else  Message=Nick+": "+JoinParam(_it,Params);
//      LOGGER->LogMsg( Message.c_str() );
      TextListMap->Add(LineName.c_str(),Message.c_str());

//         std::string mess="ƒа помолчал бы уж, "+Nick+".. (kos)";
//         handler->Send( "PRIVMSG", LineName.c_str(), mess );
         }
    // иначе - отвечаем приватно
    else {
      std::string Message=Nick+": "+JoinParam(_it,Params);
      TextListMap->Add(Nick.c_str(),Message.c_str());
      std::string mess="ƒа, € подумаю на досуге, г-н "+Nick+", над ¬ашими словами.. (kos)";
      TextListMap->Add(Nick.c_str(),Message.c_str());
//      handler->Send( "PRIVMSG", Nick.c_str(), mess );

       handler->Send( "SENDFILE", Nick.c_str(), "AmberSkyNet.png" );
        };
};

// автозаход куда приглашают
if ( Action == "INVITE" ) {
    _it++; // это наш ник по идее...
    handler->Send("JOIN", "server", (*_it).c_str() );
    };

// кто-то пришел в линию
if ( Action == "JOIN" ) {
    if ( Nick!=std::string(handler->GetParam("NickName")) ) {
        std::string mess="  нам заходит "+Nick;
        TextListMap->Add((*_it).c_str(),mess.c_str());

        mess="ѕривет тебе, о "+Nick+"!";
        TextListMap->Add((*_it).c_str(),mess.c_str());
        handler->Send("PRIVMSG", (*_it).c_str(), mess.c_str() );
        };
    };

// кто-то ушЄл из линии
if ( Action == "PART" ) {
    std::string mess="—лавный рыцарь "+Nick+" покинул нас... :(";
    TextListMap->Add((*_it).c_str(),mess.c_str());
    handler->Send("PRIVMSG", (*_it).c_str(), mess.c_str() );
    };

// кикнули за что-то
if ( Action == "KICK" ) {
    // заходим заново
    handler->Send("JOIN", "server", (*_it).c_str() );
    };

// кто-то ушЄл c irc-сервера
if ( Action == "QUIT" ) {
    std::string mess="Ёх, "+Nick+" покинул сей бренный мир... ";
    TextListMap->Add(LastLineName.c_str(),mess.c_str());
    handler->Send("PRIVMSG", LastLineName.c_str(), mess.c_str() );
    };

// пришло сообщение через DCC
//if ( Action == "PRIVMSG_DCC" ) {
//    handler->Send( "PRIVMSG_DCC", Nick.c_str(), "ѕрочту ваше тайное сообщение попозже..." );
//    };

// пришло сообщени€ о никах в линии
if ( Action == "RPL_NAMREPLY" ) {

}

// закончилось перечисление ников в линии
if ( Action == "RPL_ENDOFNAMES" ) {
    std::string MyNick=(*_it);
    _it++;
//    _it++;
    std::string LineName=(*_it);
   _it++;

//    std::string names=(*_it);
//    int temp1=names.find
//    std::string Message="ѕриветствую вас, "+(*_it)+", друзь€ мои!";
    std::string Message="¬сем привет!";
    TextListMap->Add(LineName.c_str(),(std::string(handler->GetParam("NickName"))+": "+Message).c_str());
    handler->Send( "PRIVMSG", LineName.c_str(), Message.c_str() );
};

//printf("%s\n",Params[0]);
std::string OtherParams="";
while(_it!=Params->end()){
//printf("Param:{%s}\n",(*_it).c_str());
OtherParams+=(*_it);
//LOGGER->LogMsg("Param:{"+(*_it)+"}");
_it++;
};

//LOGGER->LogMsg(OtherParams.c_str());

};

//////////////////////////////////////////////////////////////
/////////////////////////// MAIN /////////////////////////////
//////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

  ENGINE_INIT(argc, argv);

  if (!ENGINE) {  // ENGINE указывает в NULL - загрузить не получилось :(
  fprintf(stderr,"Can't load main Engine module! Exiting...\n");
  exit (1);
  };

//  int width = 640;
//  int height = 480;

//  sscanf(ENGINE->GetEnvParam("[DeviceManager]Width"),"%d",&width);
//  sscanf(ENGINE->GetEnvParam("[DeviceManager]Height"),"%d",&height);

  TextListMap=LISTMAPPER; //заполн€ем указатель

  printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

//  int start_param=ASN_DEV_CANRESIZE;

//  if ( DEVICER->Init(width,height,0,start_param) ) {
//    LogError(0,"Ooops... video error!");
//  return 0; }

// устанавливаем 2d-режим графики
//  DEVICER->SetMode( ASN_GRAPH_3D );

//  printf("Devicer [OK]\n");

//  ServerName="172.16.24.37";
//  ServerName="127.0.0.1";

ServerName=ENGINE->GetEnvParam("[NetManager]ServerName");

LineName=ENGINE->GetEnvParam("[NetManager]LineName");
LastLineName=LineName;

NickName=ENGINE->GetEnvParam("[NetManager]NickName");
RealName=ENGINE->GetEnvParam("[NetManager]RealName");
UserName=ENGINE->GetEnvParam("[NetManager]UserName");


std::string mess="RealName="+RealName+";UserName="+UserName+";NickName="+NickName;
//  sscanf(ENGINE->GetEnvParam("[DeviceManager]Height").c_str(),"%d",&height);

//  ServerName="172.16.2.152";
//  ServerName="irc.nov.ru";

/*
  myNet = NETTER->OpenClientListener("My_IRC","IRC",ServerName.c_str(),6667,mess.c_str() );

  printf("NETTER [OK]\n");

if (!myNet) {
  LogError(0,"can't connect to IRC :(");
  return 0;
  };

myNet->SetPtrParam("Handler",(void*)hanlde_IRC);
//myNet->SendMessage("dd","dd");
*/

//return 0;
//Cons1

int evt;
bool non_exit=true;
look_in_mouse=true;

// создаЄм менеджер мира
  myWorld = dynamic_cast<IWorld*> (ENGINE->CreateObject("WorldSimple3D"));
  if (!myWorld) {
      LogError(0,"Sorry, WorldSimple-plugin not found :("); return 1;
  }

  printf("World created.\n");
//return 0;
//  myCam = (ICamera *)ENGINE->CreateObject("CameraSimple");
//  if (!myCam) { LogError(0,"Class CameraSimple can't found :(");  return 1; }

  UINT cam_id = myWorld->addCamera("CameraSimple");
  myCam = myWorld->getCamera(cam_id);

  printf("Camera created.\n");

//return 0;
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
    myList = EVENTER->AddEventListener( "EvListCamLooker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera looker created.\n");

//return 0;

// позиционируем камеру
  myCam->setPos( CVector(-93,48,126) );
  myCam->setRotY( -32 );
  myCam->setScreen(0,0,1,1/*0.5*/);

//return 0;

// загружаем в наш мир карту
//  myWorld->LoadWorld( "chat.xml" );
//  myWorld->LoadWorld( "data_driven.xml" );
  printf("Map loaded.\n");

//return 0;

//  std::string LogName="logs/"+ENGINE->GetEnvParam("[LogManager]LogName");
  INode* Cons1 = dynamic_cast<INode*> (myWorld->findNode("Cons1"));
  if (Cons1) Cons1->SetParam("ListName",LineName.c_str());

  if (look_in_mouse) DEVICER->CenterMouse(true);

//return 0;

// пока мы не вышли

while (non_exit)
{

NETTER -> Check();

// начинаем рисовать
 DEVICER->StartDraw();
// "смотрим" в нашу камеру
 myCam->View();
// рисуем наш мир
 myWorld->Draw();

// заканчиваем рисовать
 DEVICER->EndDraw();

// читаем первое событие
 evt=EVENTER->GetInput();

// цикл - пока не прочитали все событи€
 while (evt!=ASN_EVNT_EMPTY) {

 if (evt==ASN_EVNT_KEYPRESSED) { //если событие - нажатие клавиши, то...

     int lk = EVENTER->getLastKey();
     char key=(char)lk;

     // если мы нажали Esc - устанавливаем признак выхода в true
     if (lk==27) non_exit=false;
     else if (lk==9) { look_in_mouse=!look_in_mouse; DEVICER->CenterMouse(!look_in_mouse);}
//     else if (lk==287) { printf("LoadWorld\n"); myWorld->LoadWorld("chat.xml"); }
     else if (lk==316) { DEVICER->MakeScreenshot("screenshot"); printf("Shoot!\n"); }
    // если мышкой не смотрим - прибавл€ем нажатую клавишу к нашей строке
     else if (!look_in_mouse) {
          unsigned int u_key=EVENTER->getUnicodeKey();
          ConsoleInput(u_key);
        }
  }

//else if (evt==ASN_EVNT_MOUSEMOVE)
//        if(look_in_mouse) MouseMoved=true;

// читаем следующее событие
  evt=EVENTER->GetInput();
 };

 myWorld->Update(10.0f*DEVICER->FrameInterval);

// выводим FPS
// printf("FPS: %i\n",DEVICER->FPS);
};


//    myWorld->Release();
//    printf("World deleted\n");

//    myCam->Release();
//    printf("Camera deleted\n");

};
