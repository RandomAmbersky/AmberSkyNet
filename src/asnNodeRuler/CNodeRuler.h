#ifndef _CNodeRuler_H
#define _CNodeRuler_H

#include "CNode.h"
#include "IDevice.h"

#define RISKPERMETER 10
#define FONTSCALE 32
#define FONTREDUCE (1.0/FONTSCALE)

class CNodeRuler: public CNode {
public:
    CNodeRuler(IEngine* engine);
    ~CNodeRuler();

virtual bool LoadResource();
virtual bool Draw();

virtual void SetParam(const char* param_name,const char* param_value);

protected:
    int length;
    IDrawObject* lineCloner;
    IDrawObject* body;
    float* lineBuffer;
    float* bodyBuffer;
};

#endif
