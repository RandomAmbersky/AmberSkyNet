#include "CEventManagerSDL.h"
//#include "SDL/SDL.h"

#include "ILog.h"
#include "CDeviceGl.h"
#include "ASN_utils.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

/// странный баг с переменными - если обьявить в функции GetInputInternal их,
/// то будет возвращен 0 :(

   std::string width_;
   std::string height_;

int convertMouseButton(int button)
{
        switch (button)
        {
          case SDL_BUTTON_LEFT:
              return ASN_MOUSE_BUTTON_LEFT;
              break;
          case SDL_BUTTON_RIGHT:
              return ASN_MOUSE_BUTTON_RIGHT;
              break;
          case SDL_BUTTON_MIDDLE:
              return ASN_MOUSE_BUTTON_MIDDLE;
              break;
          case SDL_BUTTON_WHEELUP:
              return ASN_MOUSE_WHEEL_UP;
              break;
          case SDL_BUTTON_WHEELDOWN:
              return ASN_MOUSE_WHEEL_DOWN;
              break;
        }

      return 0;
}

void CEventManagerSDL::InitManager(){

}

int CEventManagerSDL::Release(){
    printf("CEventManagerSDL::Release() try del..\n");
    delete this;
    printf("CEventManagerSDL::Release() del OK...\n");
    return 0;
}

void CEventManagerSDL::Update(float tms){

}

CEventManagerSDL::CEventManagerSDL(IEngine *engine):CBaseObject(engine){
    Engine = engine;

if (ASN_Str(ENGINE->GetEnvParam("[Modules]DeviceManager"))=="DeviceGl")
    device=(CDeviceGl*)DEVICER;
else {
    LOGGER->ErrMsg(2,"CEventManagerSDL manager required DeviceGl as DeviceManager!");
    device=NULL;
    }

    SetParam("BaseClass","EventManager");
    SetParam("Type","EventManagerSDL");
    SetParam("Name","None");
    LOGGER->LogMsg("+CEventManagerSDL");
}

CEventManagerSDL::~CEventManagerSDL(){

    Listeners_map::iterator it=Listeners.begin();
    for (UINT i=Listeners.size(); i>0; i--){
//        (*it)->Release();
        it++;
        };

//        while (SDL_PollEvent(NULL)) // flush events queue
//        {
//          SDL_Event event;
//          SDL_WaitEvent(&event);
//       };

    LOGGER->LogMsg("-CEventManagerSDL");
//    SDL_Quit();

}


IEventListener* CEventManagerSDL::AddEventListener( const char* ListenerType ){
    IBaseObject* myObj = ENGINE->CreateObject(ListenerType);
    IEventListener *myList = dynamic_cast<IEventListener*>(myObj);
    if (!myList) { myObj->Release(); printf("dasdasd"); return NULL; }

    Listeners[myList->GetID()] = myList ;
    return myList;
}

bool CEventManagerSDL::AddCustomEventListener( IEventListener *listener ){

    UID childID = listener->GetID();

    Listeners_map::iterator it_ = Listeners.find(childID);
    if ( it_!=Listeners.end() ) return false;

    Listeners[childID] = listener;
    return true;

}

bool CEventManagerSDL::DelEventListener( UID listenID ){

    Listeners_map::iterator it_ = Listeners.find(listenID);
    if ( it_==Listeners.end() ) return false;
    Listeners.erase(it_);
    return true;

}

int CEventManagerSDL::GetInput(){

    int event=GetInputInternal();

//   printf("CEventManagerSDL::GetInput %d\n", event=GetInputInternal() );

//    return 1;

//=GetInputInternal();
//    printf("CEventManagerSDL::GetInput %d\n", event);

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

//if ( event == 0 ) return ASN_EVNT_EMPTY;

    Listeners_map::iterator it=Listeners.begin();
//    (*it)->Listen(event);

    for (UINT i=Listeners.size(); i>0; i--){
        (*it).second->Listen(event);

//    printf("CEventManagerSDL::GetInput %d\n", event);

        it++;

        };

//    printf("CEventManagerSDL::GetInput %d\n", event);

    return event;
}

int CEventManagerSDL::GetInputInternal(){

/*int event=device->EventSDLManagerDelegate(this);

    return event;
*/
    SDL_Event event;

//return 1;

    if ( !SDL_PollEvent(& event) ){
//        printf("ASN_EVNT_EMPTY...%d.\n", ASN_EVNT_EMPTY);
         return ASN_EVNT_EMPTY;
    };

//    printf("get input internal...\n");

            switch ( event.type )                          // what kind of event have we got ?
            {
                case SDL_QUIT :                                         // if user wishes to quit
                    return ASN_QUIT;
                case SDL_KEYDOWN:
//                    PrintKeyInfo( &event.key );
                    UnicodeKey=event.key.keysym.unicode;
                    LastKey=event.key.keysym.sym;
                    KeyModifier=event.key.keysym.mod;
                    // check PrintScr
                    if (LastKey==316){
                    DEVICER->MakeScreenshot("screenshot");
                    printf("Shoot!\n");
                     }
                    return ASN_EVNT_KEYPRESSED;
                    break;
                case SDL_KEYUP:
//                    PrintKeyInfo( &event.key );
                    UnicodeKey=event.key.keysym.unicode;
                    LastKey=event.key.keysym.sym;
                    KeyModifier=event.key.keysym.mod;
                    return ASN_EVNT_KEYUP;
                    break;
                case SDL_MOUSEMOTION:
                    DEVICER->mouse_x=(int)event.motion.x;
                    DEVICER->mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    // если фактически мышь не двигалась
                    //(видимо, сработало 2 раза подряд CenterMouse)
                    // - то и не будем беспокоить сообщением, а смотрим что там дальше
                    if ( (event.motion.xrel==0) & (event.motion.yrel==0) ) return GetInput();
                    return ASN_EVNT_MOUSEMOVE;
                	break;
                case SDL_MOUSEBUTTONDOWN:
                    DEVICER->mouse_x=(int)event.button.x;
                    DEVICER->mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    DEVICER->mouse_butt=convertMouseButton(event.button.button);
#ifdef SDL_BUTTON_WHEELUP
                    if (event.button.button==SDL_BUTTON_WHEELUP)
                        return ASN_MOUSE_WHEEL_UP;
                    if (event.button.button==SDL_BUTTON_WHEELDOWN)
                        return ASN_MOUSE_WHEEL_DOWN;
#endif
                    return ASN_EVNT_MOUSEDOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    DEVICER->mouse_x=(int)event.button.x;
                    DEVICER->mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    DEVICER->mouse_butt=convertMouseButton(event.button.button);
                    return ASN_EVNT_MOUSEUP;
                    break;
                case SDL_VIDEORESIZE :
                    width_ = IntToStr(event.resize.w);
                    height_ = IntToStr(event.resize.h);
                    ENGINE->SetEnvParam("[DeviceManager]width",width_.c_str());
                    ENGINE->SetEnvParam("[DeviceManager]height",height_.c_str());

// sscanf(ENGINE->GetEnvParam("[DeviceManager]Width"),"%d",&width);
// sscanf(ENGINE->GetEnvParam("[DeviceManager]Height"),"%d",&height);
//printf("video resize: %d %d\n",event.resize.w,event.resize.h);
// printf("video resize: %s %s\n",ENGINE->GetEnvParam("[DeviceManager]width"),
//                                ENGINE->GetEnvParam("[DeviceManager]height") );
//                    old_bpp,
//                    old_param


                                  DEVICER->mouse_x=0;
                                  DEVICER->mouse_y=0;

// увы, SDL 1.2 явно требует чтобы
// (~~) SDL will send a SDL_VIDEORESIZE event to you application,
// and you must respond to the event by re-calling SDL_SetVideoMode()

                    DEVICER->Init(event.resize.w,event.resize.h,
                    StrToInt( ENGINE->GetEnvParam("[DeviceManager]bpp") ),
                    StrToInt( ENGINE->GetEnvParam("[DeviceManager]video_flags") )
                    );

                    DEVICER->SetMode( (ASN_GRAPH_MODE_Types) StrToInt( ENGINE->GetEnvParam("[DeviceManager]mode")) );
                    DEVICER->CenterMouse( std::string(ENGINE->GetEnvParam("[DeviceManager]cursor"))=="1" );
                    return ASN_WINDOW_RESIZE;
                    break;

                case SDL_ACTIVEEVENT :
                    if ( event.active.state & SDL_APPACTIVE ) {
					if (event.active.gain) return ASN_WINDOW_ACTIVE_ON;
					     else return ASN_WINDOW_ACTIVE_OFF;
					 			}
                    if ( event.active.state & SDL_APPINPUTFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_INPUT_FOCUS_ON;
					     else return ASN_WINDOW_INPUT_FOCUS_OFF;
					 			}
                    if ( event.active.state & SDL_APPMOUSEFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_MOUSE_FOCUS_ON;
					     else return ASN_WINDOW_MOUSE_FOCUS_OFF;
					 			}
                    return GetInputInternal();
                    break;

                default:                                    // any other event
                    return GetInputInternal();
//                    return ASN_NONE;
                    break;
            };


}

