#include "CFontTexture.h"
#include "ILog.h"

#include "CDeviceGl.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

extern GLint Custom_gluBuild2DMipmaps(GLenum target, GLint components,
		  GLsizei width, GLsizei height, GLenum format,
		  GLenum type, const void *data);

CFontTexture::CFontTexture(){

}

CFontTexture::~CFontTexture(){

}

float CFontTexture::getWidth(const unsigned char text) {

if ( FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM )
{
   if ( LoadFont() ) return 0;
}//if (CurrentFont->FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)

//    printf("FTrim[%d][0] = %d\n",(int)text,FTrim[text][0]);
    return (float)FTrim[text][0];// TOFO
}

float CFontTexture::getHeight(const unsigned char text) {
    return 16.0;//FTrim[text][1];
}

float CFontTexture::getSpace(const unsigned char text) {

if (FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)
{
   if ( LoadFont() ) return 0.0;
}//if (CurrentFont->FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)

//    printf("FTrim[%d][1] = %d\n",(int)text,FTrim[text][1]);
    return (float)FTrim[(unsigned char)text][1];;
}

void CFontTexture::UnloadFont(){
FontTexture->tex_state=ASN_FILE_NONE;
}

bool CFontTexture::LoadFont(){

printf("Try create font...\n");

unsigned char *data=device->LoadTexture( FontTexture );
if (!data) {
 LOGGER->ErrMsg(3,"Can't create font :(");
return false;
};

    GLuint tex;
    glGenTextures(1, &tex);// Generate texture ID
    glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FontTexture->filter_min);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FontTexture->filter_mag);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (FontTexture->repeat) ? GL_REPEAT : GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (FontTexture->repeat) ? GL_REPEAT : GL_CLAMP);

Custom_gluBuild2DMipmaps(GL_TEXTURE_2D, FontTexture->bpp, FontTexture->width, FontTexture->height, FontTexture->bpp, GL_UNSIGNED_BYTE, data);
//glTexImage2D(GL_TEXTURE_2D,0,Texture->bpp,Texture->width,Texture->height,0,Texture->bpp,GL_UNSIGNED_BYTE,data);

	Width=1.0f;
	Height=1.0f;

//	BYTE *col=data;
	BYTE SpaceWidth=0;
	BYTE Zero=0;
	// Цикл по всем буквам, код символа равен x+y*16

	for (int y=0;y<16;y++)
//	for (int y=15;y>=0;y++)
		for (int x=0;x<16;x++)
		{
			// Принцип:
			// 1. Вычисляем квадрат 16х16, в котором должна быть буква.
			// 2. Ищем первую вертикальную линию содержащую точку выше
			//	  заданного порога, начиная слева и потом справа.
			// 3. Получаем FTrim - таблицу растояний между буквами.

			if ((x+16*y)==32)
			{
				FTrim[x+y*16][0]=0;
				FTrim[x+y*16][1]=16;
				continue;
			}

			int cx,cy;
			cy=y*16;
			cx=x*16-1;
			bool space=true;
			while (space)
			{
				cx++;
				for (int t=0;t<16;t++)
					if (data[(cx+(256-cy-t)*256)*3]>Zero)
					{
						space=false;
						break;
					}

			}
			FTrim[x+y*16][0]=cx-x*16;
			cy=y*16;
			cx=(x+1)*16;
			space=true;
			while (space)
			{
				cx--;
				for (int t=0;t<16;t++)
					if (data[(cx+(256-cy-t)*256)*3]>Zero)
					{
						space=false;
						break;
					}

			}
			FTrim[x+y*16][1]=cx+1-FTrim[x+y*16][0]-x*16;
			if ( FTrim[x+y*16][1]<16 )
			if ( FTrim[x+y*16][1] > SpaceWidth ) {
			   SpaceWidth=FTrim[x+y*16][1]; }
			   else SpaceWidth=SpaceWidth;
		}

delete [] data;
		FTrim[32][1]=2*SpaceWidth/3;
		FontTexture->tex_state=ASN_FILE_DISK_IN_MEM;
		FontTexture->ID=tex;

		glBindTexture(GL_TEXTURE_2D, FontTexture->ID);
	float	cx;											// Содержит X координату символа
	float	cy;											// Содержит Y координату символа
    float   cwx;                                        // Ширина буквы в пикселях
	float   cwy;                                        // Высота буквы в пикселях

	cwx         = (1.0f / 256.0f) * 16;
	cwy         = (1.0f / 256.0f) * 16;

	Base=glGenLists(256);								// Создаём 256 дисплей листов
	for (int loop=0; loop<256; loop++)						// Циклимся по всем листам
	{
//    printf(" calc FTrim[%d][0] = %d\n",loop,FTrim[loop][0]);
//    printf(" calc FTrim[%d][1] = %d\n",loop,FTrim[loop][1]);

		// X и Y позиция

		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;
		glNewList(Base+loop,GL_COMPILE);				// Начинаем заполнять лист

			glBegin(GL_TRIANGLE_STRIP);							// Рисуем букву четырехугольником

				glTexCoord2f(cx+(FTrim[loop][0]+FTrim[loop][1])/256.0f,1-cy);
				glVertex2f((float)FTrim[loop][1],0);

				glTexCoord2f(cx+(FTrim[loop][0]+FTrim[loop][1])/256.0f,1-cy-cwy);
				glVertex2f((float)FTrim[loop][1],-16.0f);

				glTexCoord2f(cx+(FTrim[loop][0])/256.0f,1-cy);
				glVertex2f(0,0);

				glTexCoord2f(cx+(FTrim[loop][0])/256.0f,1-(cy+cwy));
				glVertex2f(0,-16.0f);

//				glTexCoord2f(cx+(NewFont_ptr->FTrim[loop][0]+NewFont_ptr->FTrim[loop][1])/256.0f,cy);
//				glVertex2f((float)NewFont_ptr->FTrim[loop][1]/100.0f,0);

//				glTexCoord2f(cx+(NewFont_ptr->FTrim[loop][0]+NewFont_ptr->FTrim[loop][1])/256.0f,cy+cwy);
//				glVertex2f((float)NewFont_ptr->FTrim[loop][1]/100.0f,-0.16f);

//				glTexCoord2f(cx+(NewFont_ptr->FTrim[loop][0])/256.0f,cy);
//				glVertex2f(0,0);

//				glTexCoord2f(cx+(NewFont_ptr->FTrim[loop][0])/256.0f,cy+cwy);
//				glVertex2f(0,-0.16f);


		glEnd();									// Закончили рисовать

			// Перемещаемся на другую позицию
			glTranslatef((GLfloat)(FTrim[loop][1]+1),0,0);

		glEndList();									// Закончили лист
	}													// Цикл по буквам
return false;
}

void CFontTexture::printAt(float x, float y, float Size, const char* text){

// если текстура шрифта еще не загружена - пробуем загрузить
if (FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)
{
   if ( LoadFont() ) return;
}//if (CurrentFont->FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)

    glDisable(GL_LIGHTING);

glEnable(GL_TEXTURE_2D);

	va_list		va;									// Ссылка на список аргументов

//char buf[1024];

	// Копируем текст в buf, заменяя переменные их значениями
//	va_start(va, text);
//	vsprintf(buf, text, va);
//	va_end(va);

//printf("%s\n",buf);
	// Включаем текстуру
glBindTexture(GL_TEXTURE_2D, FontTexture->ID);
//	glBindTexture(GL_TEXTURE_2D, fonts);			// Выбираем нашу текстуру шрифта

//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFunc(GL_ONE,GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)

//	glEnable(GL_ALPHA_TEST);

	glTranslatef(x,y+16.0f,0);

	glListBase(Base);
/*
	glBegin(GL_TRIANGLE_STRIP);							// Рисуем букву четырехугольником

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0]+CurrentFont->FTrim[loop][1])/256.0f,cy);
				glVertex2f((float)CurrentFont->FTrim[loop][1],0);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0]+CurrentFont->FTrim[loop][1])/256.0f,cy+cwy);
				glVertex2f((float)CurrentFont->FTrim[loop][1],-16.0f);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0])/256.0f,cy);
				glVertex2f(0,0);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0])/256.0f,(cy+cwy));
				glVertex2f(0,-16.0f);

    glEnd();
    */
	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);

//	glDisable(GL_ALPHA_TEST);

//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);

}
