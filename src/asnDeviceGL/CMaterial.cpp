
#include "CMaterial.h"
#include "ASN_utils.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#include <string>
#include <map>

CMaterial *CurrentMaterial;

void ResetTexture(const std::string &Texture){
CurrentMaterial->MaterialTexture=NULL;
}


CMaterial::CMaterial(IEngine *_Engine):CBaseObject(_Engine){
Engine=_Engine;

printf("Create material...\n");

SetParam("BaseClass","Material");
SetParam("Type","MaterialSimple");
SetParam("Texture",ASN_NOT_FOUND_PARAM);
SetParam("Texture_repeat","0");

MaterialTexture=NULL;

TextureOnly=false;

SetParam("Ambient","0.0 0.0 0.0");
SetParam("Diffuse","1.0 1.0 1.0");
SetParam("Specular","0.0 0.0 0.0");
SetParam("Emission","0.0 0.0 0.0");
SetParam("Shininess","0.0");

}

void CMaterial::SetMaterial(){

}

void CMaterial::SetParam(const char* param_name, const char* param_value){
std::string ParamName=param_name;
std::string ParamValue=param_value;

if (ParamName=="Texture") {MaterialTexture=NULL;Texture=ParamValue;
fprintf(stdout,"Material: Set texture %s\n",param_value);
}

if (ParamName=="Texture_repeat") {
 if (ParamValue=="1") TextureRepeat=true;
 else TextureRepeat=false;
 };

if (ParamName=="TextureOnly") {
 if (ParamValue=="1") TextureOnly=true;
 else TextureOnly=false;
 };

if (ParamName=="Ambient") {

CVector p=StrToVector(ParamValue);
AmbientColor[0]=p.v[0];
AmbientColor[1]=p.v[1];
AmbientColor[2]=p.v[2];
AmbientColor[3]=1.0f;
}
if (ParamName=="Diffuse") {
CVector p=StrToVector(ParamValue);

DiffuseColor[0]=p.v[0];
DiffuseColor[1]=p.v[1];
DiffuseColor[2]=p.v[2];
DiffuseColor[3]=1.0f;

}
if (ParamName=="Specular") {
CVector p=StrToVector(ParamValue);

SpecularColor[0]=p.v[0];
SpecularColor[1]=p.v[1];
SpecularColor[2]=p.v[2];
SpecularColor[3]=1.0f;

}

if (ParamName=="Emission") {
CVector p=StrToVector(ParamValue);

EmissionColor[0]=p.v[0];
EmissionColor[1]=p.v[1];
EmissionColor[2]=p.v[2];
EmissionColor[3]=1.0f;

}

if (ParamName=="Shininess") Shininess = StrToFloat(ParamValue);

//Content[param_name]=param_value;

fprintf(stdout,"Material: Set %s param %s\n",param_name, param_value);

CBaseObject::SetParam(param_name,param_value);


}

CMaterial::~CMaterial(){

}

std::string CMaterial::IamMaterial(){
printf("I am Material with texture %s!!!!\n",Texture.c_str());
}
