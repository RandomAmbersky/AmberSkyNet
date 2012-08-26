#ifndef _CEVLISCAMLOOKER_H
#define _IEVLISCAMLOOKER_H

// "прослушиватель событий
#include "IEventListener.h"
#include "IEngine.h"
#include "ICamera.h"

#include "CBaseObject.h"

class CEvListCamLooker: public IEventListener, public CBaseObject  {

    public:
        CEvListCamLooker(IEngine *engine);
        ~CEvListCamLooker();
        bool Listen(int Event);
        void SetPtrParam(const char* param_name, void *param_value);
        void SetParam(const char* param_name, const char* param_value);
    protected:
//    bool k_MoveLeft, k_MoveRight, k_MoveForvard, k_MoveBackward, k_MoveUp, k_MoveDown;
//    IEngine *Engine;
    ICamera *myCam;
};

#endif
