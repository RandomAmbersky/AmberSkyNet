#ifndef _IMATERIAL_H
#define _IMATERIAL_H

#include "IBaseObject.h"

class IMaterial: public virtual IBaseObject {
public:

//virtual
//virtual ~IMaterial(){};
virtual std::string IamMaterial()=0;
//virtual void SetMaterial()=0;

};

#endif
