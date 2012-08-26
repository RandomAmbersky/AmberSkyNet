//  Based On Tutorial
//*   Copyright (C) 2006 by Kadenov Mirat    *
//*   mirat@topmail.kz   *

#ifndef _CFontTTF_H
#define _CFontTTF_H

#include "CTexture.h"
#include "IFont.h"

#include "IFile.h"

#include "lib/GLee.h"
//#include "lib/extgl.h"

#include "ft2build.h"
#include FT_FREETYPE_H

class CFontTTF: public IFont {
public:

    CFontTTF(const char *filename);
   ~CFontTTF();

    virtual float getWidth(const unsigned char text);
    virtual float getHeight(const unsigned char text);
    virtual float getSpace(const unsigned char texts);

    virtual void printAt(float x, float y, float Size, const char* text);

    virtual void UnloadFont();

    bool font_not_loaded;

    std::string FontName;
	IFile *font_file;

    // Ширина щрифта
	float Width;
	// Высота шрифта
	float Height;
	// ID Текстуры
	GLuint TextureID;

	FT_Library library;
	FT_Face face;

    GLuint Base; // для List
//	int FTrim[256][2];

//GLuint fonttex;				// abc-текстура
float xcur, ycur;			// текущее положение пера
int char_top[256];			// сдвиг символа по вертикали
int char_left[256];			// ширина символа по горизонтали
//FT_Library lib;				// структура для FT
//FT_Face face;

//	CTexture *FontTexture;

};

#endif
