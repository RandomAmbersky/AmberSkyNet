//[848] �� ��� ������� ������ ���� ������.
//�. �������

#include "CTexture.h"

CTexture::CTexture(){
    TexFile=NULL;
    ID=0;
    width=1;
    height=1;
    bpp=0;
    tex_state=ASN_FILE_NONE;
    alpha=true;
    filter_min = 0;//GL_LINEAR_MIPMAP_LINEAR;
    filter_mag = 0;//GL_LINEAR;
    repeat=true;
}

CTexture::~CTexture(){

}
