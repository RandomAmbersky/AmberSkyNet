#ifndef _CNODESPRITE_H
#define _CNODESPRITE_H

#include "CNode.h"
#include "IDevice.h"

class CNodeSprite: public CNode {
public:
    CNodeSprite(IEngine* engine);
    ~CNodeSprite();

bool LoadResource();
bool Draw();
char Update(float tms);

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
