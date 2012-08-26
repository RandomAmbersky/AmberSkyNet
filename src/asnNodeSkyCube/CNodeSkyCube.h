#ifndef CNODESKYCUBE_H
#define CNODESKYCUBE_H

#include "CNode.h"
#include "IDevice.h"

#include "ASN_SimpleModel.h"

#include "Interfaces/ASNITerrain.h"

#include "lod_terrain.h"

class CNodeSkyCube: public CNode {
public:
    CNodeSkyCube(IEngine* engine);
    ~CNodeSkyCube();

bool LoadResource();
bool Draw();
//char Update(float tms);

void SetParam(const char* param_name,const char* param_value);

protected:

    UINT current_frame;

    UINT Repeat;       // ����� ����������, 0- ����������
    UINT Anim_Count_X; // ���������� ������������ ������ �� X
    UINT Anim_Count_Y; // ���������� ������������ ������ �� Y

    UINT Start_Anim; //��������� ����� ������������� ������
    UINT Count;      //����� ������������ �������

    bool Billbord; // ���� true - ������ ������� � ������

    float lifeTime;
    float Speed;
    float *dataBuffer;
    IDrawObject *DrawSprite;
    IMaterial *DrawMaterial;
};

#endif
