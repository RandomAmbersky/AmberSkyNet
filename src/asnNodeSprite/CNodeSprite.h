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

    UINT Repeat;       // число повторений, 0- бесконечно
    UINT Anim_Count_X; // количество анимационных кадров по X
    UINT Anim_Count_Y; // количество анимационных кадров по Y

    UINT Start_Anim; //начальный номер анимационного фрейма
    UINT Count;      //число анимационных фреймов

    bool Billbord; // если true - всегда повёрнут к камере

    float lifeTime;
    float Speed;
    float *dataBuffer;
    IDrawObject *DrawSprite;
    IMaterial *DrawMaterial;
};

#endif
