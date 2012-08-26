
#include "CEvListCamWalker.h"

#include "IEventManager.h"
#include "IDevice.h"
#define ENGINE Engine //у нас уже есть указатель на Engine
#include "ILog.h"

#include "ASN_utils.h"

CEvListCamWalker::CEvListCamWalker(IEngine *engine):CBaseObject(engine)
{
    Engine=engine;

    //    k_MoveLeft, k_MoveRight, k_MoveForward, k_MoveBackward, k_MoveUp, k_MoveDown=false;


    k_MoveLeft=false;
    k_MoveRight=false;
    k_MoveForward=false;
    k_MoveBackward=false;
    k_MoveUp=false;
    k_MoveDown=false;

    oneStep = 20.f;

    SetParam("BaseClass","EventListener");
    SetParam("Type","EvListCamWalker");
    SetParam("Name","None");

    SetPtrParam("EventListener", this);

    LOGGER->LogMsg("+CEvListCamWalker");
}

CEvListCamWalker::~CEvListCamWalker(){

        EVENTER->DelEventListener( myID );
        LOGGER->LogMsg("-CEvListCamWalker");
}

bool CEvListCamWalker::Listen(int evt)
{

    if (evt==ASN_EVNT_KEYPRESSED)
    {

        //если событие - нажатие клавиши, то...

        int lastKey = EVENTER->GetLastKey();

        if (lastKey==ASNK_d)
            k_MoveRight=true;
        else
        if (lastKey==ASNK_a)
            k_MoveLeft=true;
        else
        if (lastKey==ASNK_w)
            k_MoveForward=true;
        else
        if (lastKey==ASNK_s)
            k_MoveBackward=true;
        else
        if (lastKey==ASNK_q)
            k_MoveUp=true;
        else
        if (lastKey==ASNK_z)
            k_MoveDown=true;

    }
    else if (evt==ASN_EVNT_KEYUP)
    {

        int lastKey = EVENTER->GetLastKey();

        if (lastKey==ASNK_d)
            k_MoveRight=false;
        else
        if (lastKey==ASNK_a)
            k_MoveLeft=false;
        else
        if (lastKey==ASNK_w)
            k_MoveForward=false;
        else
        if (lastKey==ASNK_s)
            k_MoveBackward=false;
        else
        if (lastKey==ASNK_q)
            k_MoveUp=false;
        else
        if (lastKey==ASNK_z)
            k_MoveDown=false;

    };


    if (myCam)
    {
        float c=DEVICER->FrameInterval;

        if (k_MoveForward)
        {
            myCam->MoveForward( oneStep*c );
        }
        if (k_MoveBackward)
        {
            myCam->MoveForward( -oneStep*c );
        }
        if (k_MoveLeft)
        {
            myCam->MoveStrafe( -oneStep*c );
        }
        if (k_MoveRight)
        {
            myCam->MoveStrafe( oneStep*c );
        }
        if (k_MoveUp)
        {
            myCam->MoveUp( oneStep*c );
        }
        if (k_MoveDown)
        {
            myCam->MoveUp( -oneStep*c );
        }

    };

    return false;
}


void CEvListCamWalker::SetPtrParam(const char* param_name, void *param_value)
{
    if( strcmp(param_name, "Camera") == 0)
        myCam=(ICamera*)param_value;
    else
        CBaseObject::SetPtrParam(param_name,param_value);
}

void CEvListCamWalker::SetParam(const char* param_name, const char* param_value){
    if (strcmp(param_name, "step") == 0 )
              oneStep = StrToFloat( param_value );
    CBaseObject::SetParam(param_name,param_value);
}

