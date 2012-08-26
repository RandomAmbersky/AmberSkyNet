#include "CModifiers.h"

#include "ASN_utils.h"

CModifRotate::CModifRotate(IEngine *engine):CBaseObject(engine){

SetParam("BaseClass","NodeModifier");
SetParam("Type","NodeModifierRotate");
SetParam("Name","-");

SetParam("RotSpeed"," 0.1 0.1 0.1");

};

CModifRotate::~CModifRotate(){
    //printf("-CModifRotate\n");
};

int CModifRotate::Modify(INode *node,float tms){

//printf("modify...\n");
//      node
      CVector upd = RotSpeed * tms;
      CVector rot = node->GetRot();
      rot = rot + upd;
      node->SetRot ( rot );

//printf("%f %f %f\n", upd.v[0],upd.v[1],upd.v[2]);

      return 0;

};

void CModifRotate::SetParam(const char* param_name, const char* param_value){

    if ( strcmp(param_name,"RotSpeed")==0)
                RotSpeed=StrToVector(param_value);

    CBaseObject::SetParam(param_name, param_value);

};
