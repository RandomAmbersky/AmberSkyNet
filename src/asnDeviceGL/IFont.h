#ifndef _I_FONT_H
#define _I_FONT_H

//enum Font_Code_Types {
//Text,
//UTF8,
//UNICODE
//};

class CDeviceGl;

#include "IEngine.h"

class IFont {
public:

    virtual ~IFont(){};

    virtual float getWidth(const unsigned char text) = 0;
    virtual float getHeight(const unsigned char text) = 0;
    virtual float getSpace(const unsigned char texts) = 0;

    virtual void printAt(float x, float y, float Size, const char* text) = 0;

    virtual void UnloadFont()=0;

    CDeviceGl *device; //указатель на device
    IEngine *Engine; //указатель на Engine
};

#endif
