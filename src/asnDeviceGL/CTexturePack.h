#ifndef _CTEXTURE_PACK_H
#define _CTEXTURE_PACK_H

#include "IEngine.h"
#include "CTexture.h"

#include <map>

class CTexturePack {
public:
CTexturePack(IEngine *_Engine);
~CTexturePack();
virtual CTexture *LoadTexture(std::string filename);
CTexture *FindTexture(std::string FileName);
virtual std::string FindTexture(CTexture *Texture);
void Reset();
private:
IEngine *Engine;
std::map<std::string,CTexture *> TextureList;
std::map<std::string,CTexture *>::iterator p_CTexture;
};
#endif
