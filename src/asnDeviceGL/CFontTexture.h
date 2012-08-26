#ifndef _CFontTexture
#define _CFontTexture

#include "CTexture.h"
#include "IFont.h"

#include "lib/GLee.h"
//#include "lib/extgl.h"

class CFontTexture: public IFont {
public:

    CFontTexture();
   ~CFontTexture();

    virtual float getWidth(const unsigned char text);
    virtual float getHeight(const unsigned char text);
    virtual float getSpace(const unsigned char texts);

    virtual void printAt(float x, float y, float Size, const char* text);

    virtual void UnloadFont();

	// Ширина щрифта
	float Width;
	// Высота шрифта
	float Height;
	// ID Текстуры
//	GLuint TextureID;
	CTexture *FontTexture;

protected:

    bool LoadFont();
    GLuint Base; // для List
    int FTrim[256][2];


};

#endif
