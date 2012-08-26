#ifndef _IEVENTLISTENER_H
#define _IEVENTLISTENER_H

// "прослушиватель событий
#include "IBaseObject.h"

class IEventListener: public virtual IBaseObject  {

    public:
        virtual ~IEventListener(){};
        virtual bool Listen(int Event)=0;

};

#endif
