#ifndef _CNODE3DS_H
#define _CNODE3DS_H

#include "IEngine.h"

#include "CNode.h"
#include "INode.h"

#include "IDrawObject.h"
#include "CVector.h"

#include "ASN_SimpleModel.h"

#include "CModel3ds.h"

typedef IDrawTask* DrawObj_ptr;

class CNode3ds: public CNode {
public:
CNode3ds(IEngine *_Engine);
~CNode3ds();

bool LoadResource();
bool Draw();
char Update(float tms);

void SetParam(const char* param_name,const char* param_value);

protected:

IMaterial *defaultMaterial;

//bool Resize();
float ModelSize;
CModel3ds *myModel;

};

#endif
