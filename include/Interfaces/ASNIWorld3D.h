#ifndef _ASNI_WORLD3D_H
#define _ASNI_WORLD3D_H

#include "ASN_Params.h"
#include "IBaseObject.h"
#include "CVector.h"

#include "INode.h"
#include "ICamera.h"

#include "Interfaces/ASNInterface.h"

//// Интерфейсный класс мира - уровень игровой логики

class ASNIWorld3D: public ASNInterface {
    public:
    const char* GetType(){ return "ASNIWorld3D"; }
    virtual void GetNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)=0;// получаем список нод,

};

#endif
