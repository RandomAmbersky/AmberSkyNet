
#include "CEvListCamLooker.h"

#include "IEventManager.h"
#include "IDevice.h"
#define ENGINE Engine //у нас уже есть указатель на Engine
#include "ILog.h"

CEvListCamLooker::CEvListCamLooker(IEngine *engine):CBaseObject(engine)
{
    //k_MoveLeft, k_MoveRight, k_MoveForvard, k_MoveBackward, k_MoveUp, k_MoveDown=false;

    SetParam("BaseClass","EventListener");
    SetParam("Type","EvListCamLooker");
    SetParam("Name","None");

    SetPtrParam("EventListener", this);

    Engine=engine;
    LOGGER->LogMsg("+CEvListCamLooker");

}

CEvListCamLooker::~CEvListCamLooker()
{
    EVENTER->DelEventListener( myID );
    LOGGER->LogMsg("-CEvListCamLooker");
}


bool CEvListCamLooker::Listen(int evt)
{

//    printf("listen cam looker\n");
//      if (evt==ASN_EVNT_KEYPRESSED) {
//            if (EVENTER->getLastKey()==ASNK_F6) DEVICER->CenterMouse(true);
//      };

    if (evt==ASN_EVNT_MOUSEMOVE)
    {


//printf("listen cam looker MOSEMOVE %s\n", ENGINE->GetEnvParam("[DeviceManager]cursor") );

        // если на экране есть курсор - ничего не делаем
        if ( strcmp(ENGINE->GetEnvParam("[DeviceManager]cursor"),"1")==0 )
        {}
        else
        {
            myCam->LookAtMouse(DEVICER->mouse_x,DEVICER->mouse_y);
//  центрируем её..
//	    printf("look at mouse\n");
            DEVICER->CenterMouse(false);
        };
    };

    return false;
}


void CEvListCamLooker::SetPtrParam(const char* param_name, void *param_value)
{
    if( strcmp(param_name, "Camera") == 0)
        myCam=(ICamera*)param_value;
    else
        CBaseObject::SetPtrParam(param_name,param_value);
}

void CEvListCamLooker::SetParam(const char* param_name, const char* param_value)
{
    CBaseObject::SetParam(param_name,param_value);
}

