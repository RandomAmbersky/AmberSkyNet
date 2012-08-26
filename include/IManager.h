#ifndef _I_MANAGER_
#define _I_MANAGER_

#include "IBaseObject.h"

class IManager: public virtual IBaseObject {
    public:
        virtual void InitManager()=0;
//        virtual void ReleaseManager()=0;
        virtual void Update(float tms)=0;
};

#endif
