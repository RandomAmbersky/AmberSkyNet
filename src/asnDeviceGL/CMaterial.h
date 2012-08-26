#ifndef _CMATERIAL_H
#define _CMATERIAL_H

#include "CVector.h"
#include "IMaterial.h"
#include "CBaseObject.h"
#include "CTexture.h"

class CMaterial: public IMaterial, public CBaseObject {
public:
CMaterial(IEngine *_Engine);
~CMaterial();

virtual void SetMaterial();
virtual void SetParam(const char* param_name, const char* param_value);

std::string IamMaterial();

CTexture *MaterialTexture;
std::string Texture;

float AmbientColor[4];
float DiffuseColor[4];
float SpecularColor[4];
float EmissionColor[4];

float Shininess;

// использовать только текстуру, на материал не обращать внимания
bool TextureOnly;
bool TextureRepeat;

// функции от BaseObject
// функции установки/чтения указателей на менеджеры и пр.
//void SetPtrParam(const std::string& param_name, void *param_value){ BaseObject::SetPtrParam(param_name,param_value); }
//void *GetPtrParam(const std::string& param_name) { return BaseObject::GetPtrParam(param_name); }
//void DelPtrParam(const std::string& param_name) {}

// функции установки/чтения переменных
//const std::string& GetParam(const std::string& param_name) { return BaseObject::GetParam(param_name); }
//void DelParam(const std::string& param_name) { BaseObject::DelParam(param_name); }

};

#endif
