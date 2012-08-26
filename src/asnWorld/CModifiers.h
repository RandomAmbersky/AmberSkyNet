#ifndef _CMODIFIERS_H
#define _CMODIFIERS_H

#include "INodeModifier.h"
#include "CBaseObject.h"

class CModifRotate: public INodeModifier, public CBaseObject {
    public:
    CModifRotate(IEngine* engine);
    ~CModifRotate();
    int Modify(INode *node,float tms);
    void SetParam(const char* param_name, const char* param_value);
    protected:
        CVector RotSpeed;
};

#endif
