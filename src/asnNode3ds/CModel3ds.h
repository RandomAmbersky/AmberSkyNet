#ifndef _CMODEL_3DS_H
#define _CMODEL_3DS_H

//// ���������� ������, ���������� ���� � ���������,
//// ����������� �� 3ds �����
//// � ����������� � ����

#include "ASN_SimpleModel.h"
#include "IEngine.h"
#include "CVector.h"

#include "CBaseObject.h"
#include "Interfaces/ASNInterface.h"

#include "IDevice.h"

const std::string Model3ds_BaseClass=std::string("Model3ds");
const std::string Model3ds_Type=std::string("CModel3ds");

class CModel3ds: public CBaseObject {
public:

CModel3ds(IEngine* engine);
~CModel3ds();

// ��������� ������, � ������ ������� - ������� true
bool Load3ds(IEngine *Engine, void *data, UINT size); //data - ������ �� ���� �������� size, ���������� � ������
bool Draw3ds(IDevice* device, bool add_material, CVector4* node_color);

//const char* GetBaseClass(){ return Model3ds_BaseClass.c_str(); }
//const char* GetType(){  return Model3ds_Type.c_str(); }
//const char* GetName(){ return Name.c_str(); }

void SetModelName(const char* model_name){ Name = model_name;}

const char* GetModelName(){ return Name.c_str(); }

bool AutoMap;// ��������������������� ?
bool Color; //������������

float MaxSize;

CVector Real_Size;

CVector Norm_Size;

//private:

std::string Name;

/// ��������� �� ���+��������, ��������� ��� �������� �� ������
std::vector<IDrawTask*> drawPack;

// ����� ��������������� �����
std::vector<ASN_SimpleIndexMesh*> meshPack;

};

#endif
