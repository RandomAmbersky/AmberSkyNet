#include "CTexturePack.h"
#include "ILog.h"

#include <stdio.h>

#define ENGINE Engine //у нас уже есть указатель на Engine

CTexturePack::CTexturePack(IEngine *_Engine){
Engine=_Engine;
TextureList.clear();
}

CTexturePack::~CTexturePack(){

// подчищаем за собой
p_CTexture= TextureList.begin();
    for (UINT i=TextureList.size(); i>0; i--){
        delete (*p_CTexture).second;
        p_CTexture++;
        };

}

CTexture *CTexturePack::LoadTexture(std::string filename){

fprintf(stdout, "TexturePack load texture %s\n",filename.c_str());

CTexture *New_Texture = FindTexture(filename);
if (New_Texture!=NULL) return New_Texture; // уже есть такая текстура
//текстуры нет
New_Texture = new CTexture();
New_Texture->TexFile=NULL;
New_Texture->TexName=filename;
TextureList[filename]= New_Texture;
LOGGER->LogMsgStr("TEXTURE PACK: Add new texture in pack - "+filename);
return New_Texture;
}

void CTexturePack::Reset(){

p_CTexture= TextureList.begin();
    for (UINT i=TextureList.size(); i>0; i--){
        if (  (*p_CTexture).second->TexFile ) {
                (*p_CTexture).second->TexFile->FreeMem();
                (*p_CTexture).second->TexFile->FileClose(); }
        (*p_CTexture).second->tex_state=ASN_FILE_NONE;
        (*p_CTexture).second->ID=0;
        p_CTexture++;
        };

}

CTexture *CTexturePack::FindTexture(std::string FileName){
p_CTexture=TextureList.find(FileName);
if (p_CTexture==TextureList.end()) return NULL;
return (*p_CTexture).second;
}

std::string CTexturePack::FindTexture(CTexture *Texture){
FindTexture(Texture->TexName);

    p_CTexture=TextureList.begin();
    for (int i=TextureList.size(); i>0; i--){
    if ((*p_CTexture).second==Texture) return (*p_CTexture).first;
    p_CTexture++;
    }
return "N/A";
}
