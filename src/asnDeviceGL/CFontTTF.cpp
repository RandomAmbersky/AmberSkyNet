#include "CFontTTF.h"
#include "ILog.h"

#include "IFileSystem.h"

#define ENGINE Engine //у нас уже есть указатель на Engine
    CFontTTF::CFontTTF(const char *filename){

    font_not_loaded=true;
	FontName=filename;
	xcur=ycur=0;
	font_file=NULL;

    }

   CFontTTF::~CFontTTF(){

   // FT_Done_Face(face);
   // FT_Done_FreeType(library);

   }

   float CFontTTF::getWidth(const unsigned char text){
//    printf("FTrim[%d][0] = %d\n",(int)text,char_left[text]);
     return (float)char_left[text];
//    return Width;
   }

   float CFontTTF::getHeight(const unsigned char text) {
    //return (float)abs(char_top[text]); //в случае русских букв выдаёт жуткое
    return 16.0f;
   }

   float CFontTTF::getSpace(const unsigned char text) {
    return 0.0;//(float)char_left[text];
   }

   void CFontTTF::UnloadFont(){
   if ( font_not_loaded ) return;

   // FT_Done_Face(face);
   // FT_Done_FreeType(library);
   font_not_loaded=true;

   }

   void CFontTTF::printAt(float x, float y, float Size, const char* text){
//      printf("try print: %s\n",text);
   if (font_not_loaded){
//      printf("try load font %s.\n", FontName.c_str() );
    if (!font_file) font_file = FILER->GetFile( FontName.c_str() );
//        printf("try load font file...\n");
    if (!font_file) return; // файл не найден вообще
//        printf("try file open...\n");
    font_file->FileOpen();
//       printf("try file load in mem...\n");
     const FT_Byte *data=(const FT_Byte *)font_file->LoadInMem(); // грузим его в память
    // если не получилось загрузить - возврат
    if (font_file->GetState()!=ASN_FILE_DISK_IN_MEM) return;
	if (!data) return;
//	printf("ok\n");
    // Здесь использован код из FreeType Tutorial
	FT_Init_FreeType ( &library );
	//	FT_New_Face( library, FontName.c_str(), 0, &face );
	FT_New_Memory_Face ( library, data, font_file->GetSizeInBytes(), 0, &face );
//       printf("ok2\n");
	FT_Set_Char_Size(
			face,    /* handle to face object           */
	16*64,       /* char_width in 1/64th of points  */
	16*64,   /* char_height in 1/64th of points */
	300,     /* horizontal device resolution    */
	300 );   /* vertical device resolution      */

	FT_Set_Pixel_Sizes(
			face,   /* handle to face object */
	16,      /* pixel_width           */
	16 );   /* pixel_height          */


	FT_Select_Charmap(
			face,               /* target face object */
	FT_ENCODING_UNICODE );

	// рендерим все символы в большую текстуру

	glGenTextures( 1, &TextureID);
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

//	unsigned char * buf = new unsigned char[16 * 16 * 256 * 4];
	unsigned char * buf = new unsigned char[16 * 16 * 256 * 2];
	unsigned char tp = 0xff;
//	printf("ok3\n");

	for (int t2 = 1; t2 < 16; t2++)
	{
//		printf("ok4\n");
		for (int t1 = 0; t1 < 16; t1++)
		{
//			printf("ok5\n");

			int charcode = (t2 * 16 + t1);

			// вы будете удивлены, но эта строчка является
			// "поддержкой кодировки cp1251" :)
			// смысл ее в том, что FT требует для себя код символа unicode
			// а компилятор кодирует строки в обычный cp1251
			// поэтому мы перекодируем символ в unicode
			if (charcode > 127) charcode = charcode + 0x410 - 192;

			int glyph_index = FT_Get_Char_Index( face,  charcode  );
			int load_flags = FT_LOAD_DEFAULT;
//			printf("ok6\n");
			// Рендерим глиф (можно сказать "символ" в понятиях FT)
			FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;
			FT_Load_Glyph(face, glyph_index, load_flags );
//			printf("ok7\n");
			FT_Render_Glyph( face->glyph, render_mode );	// здесь
//			printf("ok8\n");
			FT_Bitmap * bitmap = &face->glyph->bitmap;
//			printf("ok9\n");
//			face->glyph;
			// теперь сохраняем информацию о размерах символа
			char_left[t2 * 16 + t1] =  face->glyph->advance.x >>6;//(int) (face->glyph->advance.x / 64.0f);
			char_top[t2 * 16 + t1] =  (int) -face->glyph->bitmap_top;

			// копируем изображение символа из результата работы FT в нашу abc-тексутуру
			for (int j = 0; j < 16; j++)
			{

				for (int i = 0; i < 16; i++)
				{
					if ((i < bitmap->width) & (j < bitmap->rows) ) tp = bitmap->buffer[j*bitmap->width + i]; else tp = 0;
					buf[ 2*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 0] = tp;
					buf[ 2*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 1] = tp;
//					buf[ 4*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 0] = tp;
//					buf[ 4*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 1] = tp;
//					buf[ 4*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 2] = tp;
//					buf[ 4*( (j + 16 * t2 ) * 256 + i + t1 * 16 ) + 3] = 0xff;
				}

			}



		}

//     FT_Done_Face(face);
//     FT_Done_FreeType(library);
// 	printf("ok3\n");
	}


	// создаем текстуру OpenGL из abc-текстуры

//	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA /*GL_LUMINANCE_ALPHA/* GL_RGBA*/, GL_UNSIGNED_BYTE, buf);

glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buf);

	delete (buf);
	font_not_loaded=false;

   FT_Done_Face(face);
   FT_Done_FreeType(library);

   }; //   if (font_not_loaded){   загрузили шрифт

   	int t = 0;
//	if (x != 0)
    xcur = 0;//(float)x;
//	if (y != 0)
    ycur = 0;//(float)y;
	float x1 = xcur,y1 = ycur;
//	float vert_step = 16.0f;
//	float hor_step = 16.0f;

	glDisable(GL_LIGHTING);
// сохраняем все параметры и включаем 2d-режим (ортогональная проекция размером с экран))
//	glPushAttrib(GL_BLEND | GL_DRAW_BUFFER | GL_COLOR_BUFFER_BIT | GL_LOGIC_OP | GL_DEPTH_BUFFER_BIT | GL_DEPTH_TEST | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_STENCIL_BUFFER_BIT );

//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(0, 320, 0, 240, -1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();

//	glDisable(GL_DEPTH_TEST);

//
glDepthMask(false);
//	glColor4f(1,1,1,1);
//	printf("TEXT: %s \n",text);
	// начинаем отрисовку квадов
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, TextureID);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)

//	glAlphaFunc(GL_GREATER,0.5f);								// Set Alpha Testing     (disable blending)
//	glEnable(GL_ALPHA_TEST);									// Enable Alpha Testing  (disable blending)


    glPushMatrix();
	glTranslatef(x,y,0);

	glBegin(GL_QUADS);
	while ( text[t] )
	{
		unsigned char ch = text[t];
		switch (ch)
		{
			case '\n':
				y1 = y1 - 16.0f;
				x1 = (float)xcur;
				break;
			default:

//                x1 = x1 + ( char_left[ch] ) /2.0f;

                float t1 = (float) (ch - 16*(ch / 16) ) / 16.0f;
                float t2 = (float)(ch / 16) / 16.0f;

//                float t1 = (float)( ch - 16.0f * ( ch / 16.0f )) / 16.0f;
//                float t2 = (float)(ch) / 16.0f / 16.0f;
//                   float diff =  1.0f / 16.0f * char_left[ch] / 16.0f ; //1.0f / 16.0f /
//                  float diff_x =  1.0f / 16.0f * char_left[ch]/ 16.0f; //1.0f / 16.0f;
                  float diff_x =  1.0f / 16.0f * (char_left[ch])/ 16.0f; //1.0f / 16.0f;
                  float diff_y =  ( 1.0f + char_top[ch]/64.0f ) / 16.0f;
                                    float diff =   0.0051f;//0*1.0f/16.0f;//(16.0f-char_left[ch])/2.0f;//( 16.0f - char_left[ch] )/2.0f;//1.0f/ 16.0f;
//printf("%f \n", diff);

//				glColor4f(0.0f,1.0f,0.0f,1.0f);

				glTexCoord2f(t1,t2);
				glVertex3f(x1*Size, (y1 - char_top[ch])*Size, 1.0f);

				glTexCoord2f(t1,t2 + diff_y);
				glVertex3f(x1*Size, (y1 - 16.0f - char_top[ch])*Size, 1.0f);

				glTexCoord2f(t1+ diff_x,t2 + diff_y);
				glVertex3f( (x1 + char_left[ch])*Size , (y1 - 16.0f - char_top[ch])*Size, 1.0f);

				glTexCoord2f(t1 + diff_x,t2);
				glVertex3f( (x1 + char_left[ch])*Size, (y1 - char_top[ch])*Size, 1.0f);

//                x1 = x1 + 16.0f;//( char_left[ch] ) /2.0f;
				x1 = x1 + char_left[ch];//diff_x*16.0f;//16.0f;//(float)char_left[ch];

/*
				float t2 = (float)(ch / 16) / 16.0f, t1 = (float) (ch - 16*(ch / 16)) / 16.0f, diff = 1.0f / 16.0f;//-0.0018f;
//				glColor4f(0.0f,1.0f,0.0f,1.0f);

				glTexCoord2f(t1,t2);
				glVertex2f(x1, y1 - char_top[ch]);

				glTexCoord2f(t1,t2 + diff);
				glVertex2f(x1, y1 - 16.0f - char_top[ch]);

				glTexCoord2f(t1 + diff,t2 + diff);
				glVertex2f(x1 + 16.0f, y1 - 16.0f - char_top[ch]);

				glTexCoord2f(t1 + diff,t2);
				glVertex2f(x1 + 16.0f, y1 - char_top[ch]);

				x1 = x1 + (float)char_left[ch];
*/

				break;
		}
		t++;


	}
	glEnd();
    glPopMatrix();

	glDisable(GL_BLEND);

    glDepthMask(true);

	// возвращаем состояние OpenGL к первоначальному
	glEnable(GL_DEPTH_TEST);
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//
//	glPopAttrib();
    glEnable(GL_LIGHTING);
	// сдвигаем перо
	xcur = x1;
	ycur = y1;

   }

