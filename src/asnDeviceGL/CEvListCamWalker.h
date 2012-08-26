#ifndef _CEVLISCAMWALKER_H
#define _CEVLISCAMWALKER_H

// "прослушиватель событий
#include "IEventListener.h"
#include "IEngine.h"
#include "ICamera.h"
#include "CBaseObject.h"

//#define CAMERASTEP (20.0f)

class CEvListCamWalker: public IEventListener, public CBaseObject  {

    public:
        CEvListCamWalker(IEngine *engine);
        ~CEvListCamWalker();
        bool Listen(int Event);
        void SetPtrParam(const char* param_name, void *param_value);
        void SetParam(const char* param_name, const char* param_value);
    protected:
    bool k_MoveLeft, k_MoveRight, k_MoveForward, k_MoveBackward, k_MoveUp, k_MoveDown;
//    IEngine *Engine;
    ICamera *myCam;
    float oneStep;
};

#endif
