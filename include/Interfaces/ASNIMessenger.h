#ifndef _ASNI_MESSENGER_H
#define _ASNI_MESSENGER_H

#include "ASN_Params.h"
#include "IBaseObject.h"
#include "CVector.h"

#include "INode.h"
#include "ICamera.h"

#include "Interfaces/ASNInterface.h"

//// ������������ ����� ���� - ������� ������� ������

class ASNIMessenger: public ASNInterface {
    public:
    const char* GetType(){ return "ASNIMessenger"; }

};

#endif
