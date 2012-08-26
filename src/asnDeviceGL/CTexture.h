#ifndef _CTEXTURE_H
#define _CTEXTURE_H

#include "IFile.h"
#include <string>

class CTexture {
public:
CTexture();
~CTexture();
std::string TexName;
unsigned int ID;
int width;
int height;
int bpp;
int tex_state;
bool alpha;
unsigned int filter_min;
unsigned int filter_mag;
unsigned int repeat;
IFile *TexFile;
};

#endif
