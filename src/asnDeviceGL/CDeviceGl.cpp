#include "CDeviceGl.h"
//#include "CDeviceGl.h"
//#include "CDeviceGl.h"

#include "ILog.h"
#include "IFileSystem.h"

#define TWOPI 2*3.14
#define PID2 3.14/2

#include "CEventManagerSDL.h"
//#define GL_GLEXT_PROTOTYPES 1

//#include <GL/gl.h>   // header for standard OpenGL calls (gl_ calls )
#include "lib/GLee.h"
//#include "lib/extgl.h"
//#include "lib/DdsLoader.h"
//#include "lib/Data.h"
//#include "lib/Texture.h"
//#include <GL/glext.h>

//#include "lib/geometry.h"

#include <GL/glu.h>

#include <SDL/SDL_image.h>

#include <math.h>

#include "ASN_utils.h"

#include "CDrawObject.h"

#include "CMaterial.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#include "Primitive_libs.cpp"

#include "CFontTexture.h"
#include "CFontTTF.h"

#include "CBaseObject.h"

#define VBO_ON 1

void CreateSphere(CVector c,double r,int n);

struct Vertex
{
	// GL_C4UB_V3F
	unsigned char r, g, b, a;
	float x, y, z;
};

Vertex g_lineVertices[] =
{
	{ 255,   0,   0, 255,  0.0f, 0.0f, 0.0f }, // red = +x Axis
	{ 255,   0,   0, 255,  5.0f, 0.0f, 0.0f },
	{   0, 255,   0, 255,  0.0f, 0.0f, 0.0f }, // green = +y Axis
	{   0, 255,   0, 255,  0.0f, 5.0f, 0.0f },
	{   0,   0, 255, 255,  0.0f, 0.0f, 0.0f }, // blue = -z Axis
	{   0,   0, 255, 255,  0.0f, 0.0f,-5.0f }
 };

    float theta = 0.0f;

// This gets a pixel out of surface
Uint32 getpixel(SDL_Surface *bmp, int x, int y) {
int bpp = bmp->format->BytesPerPixel;
Uint8 *p = (Uint8*)bmp->pixels + y * bmp->pitch + x*bpp;
switch(bpp) {
case 1:
return *p;
case 2:
return *(Uint16*)p;
case 3:
if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
return p[0] << 16 | p[1] << 8 | p[2];
else
return p[0] | p[1] << 8 | p[2] << 16;
case 4:
return *(Uint32*)p;
default:
//((CEventLog *)MegaBase->GetVoidParam("CEventLog"))->LogMsg("DEVICE: I can't do it!\n");
return 0;
}
}

/*
    int convertMouseButton(int button)
    {
        switch (button)
        {
          case SDL_BUTTON_LEFT:
              return ASN_MOUSE_BUTTON_LEFT;
              break;
          case SDL_BUTTON_RIGHT:
              return ASN_MOUSE_BUTTON_RIGHT;
              break;
          case SDL_BUTTON_MIDDLE:
              return ASN_MOUSE_BUTTON_MIDDLE;
              break;
          case SDL_BUTTON_WHEELUP:
              return ASN_MOUSE_WHEEL_UP;
              break;
          case SDL_BUTTON_WHEELDOWN:
              return ASN_MOUSE_WHEEL_WHEELDOWN;
              break;
        }

      return 0;
        };
*/

////////////////////// Well... thank's to GunGine !!!! /////////////
///// т.к. gluBuild2DMipmaps плохо работает с текстурами, имеющими альфа-канал
//// (черные прямоугольники вместо цвета), то пришлось добавить сюда
//// кусочек из gunGine
GLint round2(GLint n)
{
	GLint m;

	for (m = 1; m < n; m *= 2);

	/* m>=n */
	if (m - n <= n - m / 2) {
		return m;
	}
	else {
		return m / 2;
	}
}

static GLint
bytes_per_pixel(GLenum format, GLenum type)
{
	GLint n, m;

	switch (format) {
	case GL_COLOR_INDEX:
	case GL_STENCIL_INDEX:
	case GL_DEPTH_COMPONENT:
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
	case GL_ALPHA:
	case GL_LUMINANCE:
		n = 1;
		break;
	case GL_LUMINANCE_ALPHA:
		n = 2;
		break;
	case GL_RGB:
	case GL_BGR:
		n = 3;
		break;
	case GL_RGBA:
	case GL_BGRA:
#ifdef GL_EXT_abgr
	case GL_ABGR_EXT:
#endif
		n = 4;
		break;
	default:
		n = 0;
	}

	switch (type) {
	case GL_UNSIGNED_BYTE:
		m = sizeof(GLubyte);
		break;
	case GL_BYTE:
		m = sizeof(GLbyte);
		break;
	case GL_BITMAP:
		m = 1;
		break;
	case GL_UNSIGNED_SHORT:
		m = sizeof(GLushort);
		break;
	case GL_SHORT:
		m = sizeof(GLshort);
		break;
	case GL_UNSIGNED_INT:
		m = sizeof(GLuint);
		break;
	case GL_INT:
		m = sizeof(GLint);
		break;
	case GL_FLOAT:
		m = sizeof(GLfloat);
		break;
	default:
		m = 0;
	}

	return n * m;
}

GLint Custom_gluBuild2DMipmaps(GLenum target, GLint components,
		  GLsizei width, GLsizei height, GLenum format,
		  GLenum type, const void *data)
{
   GLint w, h, maxsize;
   void *image, *newimage;
   GLint neww, newh, level, bpp;
   int error;
   GLboolean done;
   GLint retval = 0;
   GLint unpackrowlength, unpackalignment, unpackskiprows, unpackskippixels;
   GLint packrowlength, packalignment, packskiprows, packskippixels;

   if (width < 1 || height < 1)
      return 0;

   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);

   w = round2(width);
   if (w > maxsize) {
      w = maxsize;
   }
   h = round2(height);
   if (h > maxsize) {
      h = maxsize;
   }

   bpp = bytes_per_pixel(format, type);
   if (bpp == 0) {
      /* probably a bad format or type enum */
      return 0;
   }

   /* Get current glPixelStore values */
   glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpackrowlength);
   glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackalignment);
   glGetIntegerv(GL_UNPACK_SKIP_ROWS, &unpackskiprows);
   glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpackskippixels);
   glGetIntegerv(GL_PACK_ROW_LENGTH, &packrowlength);
   glGetIntegerv(GL_PACK_ALIGNMENT, &packalignment);
   glGetIntegerv(GL_PACK_SKIP_ROWS, &packskiprows);
   glGetIntegerv(GL_PACK_SKIP_PIXELS, &packskippixels);

   /* set pixel packing */
   glPixelStorei(GL_PACK_ROW_LENGTH, 0);
   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   glPixelStorei(GL_PACK_SKIP_ROWS, 0);
   glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

   done = GL_FALSE;

   if (w != width || h != height) {
      /* must rescale image to get "top" mipmap texture image */
      image = malloc((w + 4) * h * bpp);
      if (!image) {
	 return 0;
      }
      error = gluScaleImage(format, width, height, type, data,
			    w, h, type, image);
      if (error) {
	 retval = error;
	 done = GL_TRUE;
      }
   }
   else {
      image = (void *) data;
   }

   level = 0;
   while (!done) {
      if (image != data) {
	 /* set pixel unpacking */
	 glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	 glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	 glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
      }

      glTexImage2D(target, level, components, w, h, 0, format, type, image);

      if (w == 1 && h == 1)
	 break;

      neww = (w < 2) ? 1 : w / 2;
      newh = (h < 2) ? 1 : h / 2;
      newimage = malloc((neww + 4) * newh * bpp);
      if (!newimage) {
	 return 0;
      }

      error = gluScaleImage(format, w, h, type, image,
			    neww, newh, type, newimage);
      if (error) {
	 retval = error;
	 done = GL_TRUE;
      }

      if (image != data) {
	 free(image);
      }
      image = newimage;

      w = neww;
      h = newh;
      level++;
   }

   if (image != data) {
      free(image);
   }

   /* Restore original glPixelStore state */
   glPixelStorei(GL_UNPACK_ROW_LENGTH, unpackrowlength);
   glPixelStorei(GL_UNPACK_ALIGNMENT, unpackalignment);
   glPixelStorei(GL_UNPACK_SKIP_ROWS, unpackskiprows);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpackskippixels);
   glPixelStorei(GL_PACK_ROW_LENGTH, packrowlength);
   glPixelStorei(GL_PACK_ALIGNMENT, packalignment);
   glPixelStorei(GL_PACK_SKIP_ROWS, packskiprows);
   glPixelStorei(GL_PACK_SKIP_PIXELS, packskippixels);

   return retval;
}
////////////////////// Well... thank's to GunGine (end) !!!! /////////////


int CDeviceGl::Release(){
    printf("CDeviceGl release...\n");
    delete this;
    return 0;
}

void CDeviceGl::Update(float tms){

}

CDeviceGl::CDeviceGl(IEngine *engine):CBaseObject(engine) {
Engine=engine;

SetParam("BaseClass","Device");
SetParam("Type","DeviceGl");


CEventManagerSDL *myEv = new CEventManagerSDL( Engine );
myEv->SetParam("Name", "EventManager");
Engine->AddCustomManager( myEv );
//myEv->SetPtrParam( "EventManager", myEv);
LOGGER->LogMsg("+CDeviceGl");

}

void CDeviceGl::InitManager(){

printf("CDeviceGl init...\n");
ScreenData=NULL;
framesPerSecond=0.0f;
lastTime=0.0f;
FrameInterval=0.0f;
FPS=0;
TexturePack = new CTexturePack(Engine);
CurrentTexture=NULL;
MouseCursor=true;
//LOGGER->LogMsg("+CDeviceGl");
//printf("CDeviceGl init1...\n");
DefaultMaterial=AddMaterial("DefaultMaterial");
LOGGER->LogMsg("CDeviceGl: Default material created.");
//printf("CDeviceGl init2...\n");
BuildFont("Default","FreeSans.ttf");
//BuildFont("Default","default_font.png");
CurrentFontSize=1.0f;
LOGGER->LogMsg("CDeviceGl: Default font created.");
SetFont("Default");
//printf("CDeviceGl init3...\n");
CurrentMaterial=(CMaterial*)((void*)DefaultMaterial);
CurrentDrawObject=NULL;
//CurrentFont=NULL;
//CVector4 CurrentColor=CVector4(1,1,1,1);
//printf("CDeviceGl init4...\n");
//SetColor( CVector4(1,1,1,1) ); //can't set color because screen can't init :(
//printf("CDeviceGl init5...\n");

if ( std::string(ENGINE->GetEnvParam("[DeviceManager]ClearColor"))!=ASN_NOT_FOUND_PARAM )
    SetParam( "ClearColor", ENGINE->GetEnvParam("[DeviceManager]ClearColor") );
else {
    ENGINE->SetEnvParam("[DeviceManager]ClearColor", "0 0 0");
    ClearColor = CVector( 0, 0, 0 );
    }

ENGINE->SetEnvParam("[DeviceManager]mode", IntToStr(ASN_GRAPH_3D).c_str());

GLeeInit();

        if ( GLEE_ARB_vertex_buffer_object ) {
                        LOGGER->LogMsg("ARB_vertex_buffer enabled.");
                        VBO_enabled=true;
                        } else {
                        LOGGER->LogMsg("ARB_vertex_buffer not enabled.");
//                        LOGGER->ErrMsg(0,"ARB_vertex_buffer can't enabled!\n Please update you'r hardware or video drivers..");
                        VBO_enabled=false;
                        };
}

CDeviceGl::~CDeviceGl(){

    PostDrawList3D.clear();
    PostDrawList2D.clear();
//    PostDrawText3D.clear();
    PostDrawText3D.clear();
    PostDrawText2D.clear();

    CenterMouse(true); // show cursor

if (ScreenData) {delete []ScreenData; ScreenData=NULL;}
delete TexturePack;
LOGGER->LogMsg("-CDeviceGl");

// удаляем объекты и материалы
std::map<std::string,IMaterial*>::iterator _it= MaterialList.begin();
    for (UINT i=MaterialList.size(); i>0; i--){
        (*_it).second->Release();
        _it++;
        };

std::map<std::string,IDrawObject*>::iterator _it2=DrawObjectList.begin();
    for (UINT i=DrawObjectList.size(); i>0; i--){
        delete (*_it2).second;
        _it2++;
        };

std::map<std::string,IFont*>::iterator _it3=FontList.begin(); //список шрифтов
    for (UINT i=FontList.size(); i>0; i--){
        delete (*_it3).second;
        _it3++;
        };

	printf("start poll event..\n");
        //while (
	SDL_PollEvent(NULL);
	//) // flush events queue
        //{
          //SDL_Event event;
	  printf("poll event...\n");
          //SDL_WaitEvent(&event);
	  //printf("wait poll event..\n");
       //};

	//SDL_Quit();

}

bool CDeviceGl::Init(int width, int height, int bpp, int param){

Uint32 video_flags  = SDL_OPENGL;

old_param=param;
old_bpp=bpp;

TexturePack->Reset();

std::map<std::string,IFont*>::iterator _it3=FontList.begin(); //список шрифтов
    for (UINT i=FontList.size(); i>0; i--){
        (*_it3).second->UnloadFont();
        _it3++;
        };

if (param & ASN_DEV_FULLSCREEN) video_flags   |= SDL_FULLSCREEN;
if (param & ASN_DEV_CANRESIZE)  video_flags   |= SDL_RESIZABLE;
if (param & ASN_DEV_NOFRAME)    video_flags   |= SDL_NOFRAME;

    printf("SDL initialization...");
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO)==-1)
	{
 LOGGER->ErrMsgStr(1,"SDL_Init: "+std::string(SDL_GetError()));
	return true;
	}
 LOGGER->LogMsg("SDL_Init: Video OK");
    printf(" done\n");

// если bpp=0 - определяем bpp экрана
//	if ( bpp == 0 ) {
//		if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
//			bpp = 8;
//		} else {
//			bpp = 8;
//		}
//	}

/*
	int rgb_size[3];
		switch (bpp) {
	    case 8:
		rgb_size[0] = 3;
		rgb_size[1] = 3;
		rgb_size[2] = 3;
		break;
	    case 15:
	    case 16:
		rgb_size[0] = 5;
		rgb_size[1] = 5;
		rgb_size[2] = 5;
		break;
            default:
		rgb_size[0] = 8;
		rgb_size[1] = 8;
		rgb_size[2] = 8;
		break;
	}
*/
//	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rgb_size[0] );
//	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, rgb_size[1] );
//	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, rgb_size[2] );

//    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       // we aren't going to use the stencil buffer
//    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated per pixel -
//    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
//    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
//    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);

// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

// SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
// SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
// SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

//	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 4 );
//	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 4 );
//	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 4 );

//	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware

    if(VideoInfo == NULL)                                     // if this query fails
    {
        LOGGER->ErrMsgStr(1,"DEVICE: Failed getting Video Info: "+std::string(SDL_GetError()));          // report error
        return true;
    }

//    video_flags   |= SDL_RESIZABLE;                      // the window must be resizeable
    video_flags   |= SDL_DOUBLEBUF;

   printf("Setting video mode...");
   MainWindow = SDL_SetVideoMode(width,height,bpp,video_flags);
   printf(" done\n");

if( MainWindow == NULL )                            // if window creation failed
    {
        LOGGER->ErrMsgStr(1,"DEVICE: Failed to Create Window: " + std::string(SDL_GetError()));         // report error
        return 1;
    }

    ENGINE->SetEnvParam("[DeviceManager]width",IntToStr(width).c_str());
    ENGINE->SetEnvParam("[DeviceManager]height",IntToStr(height).c_str());
    ENGINE->SetEnvParam("[DeviceManager]bpp",IntToStr(bpp).c_str());
    ENGINE->SetEnvParam("[DeviceManager]video_flags",IntToStr(param).c_str());

    LOGGER->LogMsgStr("Videocard: "+ std::string( (char*)glGetString(GL_VENDOR) ));
    LOGGER->LogMsgStr("Rendener: "+ std::string( (char *)glGetString(GL_RENDERER)));
    LOGGER->LogMsgStr("OpenGL version: " + std::string( (char *)glGetString(GL_VERSION)));
    LOGGER->LogMsgStr("OpenGL extensions: " + std::string( (char *)glGetString(GL_EXTENSIONS)));


        SDL_WM_SetCaption( ENGINE->GetEnvParam("[DeviceManager]window_name") ,"AmberSkyNet");  // set the window caption (first argument) and icon caption (2nd arg)

    SDL_EnableUNICODE(1);

//    if ( SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL) )
//    {
//        ((CEventLog *)MegaBase->GetVoidParam("EventLog"))->LogMsg("DEVICE: Failed enabling key repeat: %s",SDL_GetError());
//        return 1;
//    }

//   while (SDL_PollEvent(NULL)) // flush events queue
//    {
//        SDL_Event event;
//        SDL_WaitEvent(&event);
//    };

        if (ScreenData) {delete []ScreenData; ScreenData=NULL;}
        ScreenData = new char[width*height*4];

//        while (SDL_PollEvent(NULL)) // flush events queue
//        {
//          SDL_Event event;
//          SDL_WaitEvent(&event);
//       };


//	extgl_Initialize();


//	glPolygonOffset ( -1, -1 );

//    drawPoly ( p, 4 );

    glEnable ( GL_POLYGON_OFFSET_FILL );

	glEnable(GL_DEPTH_TEST);
	glShadeModel ( GL_SMOOTH );

    glEnable(GL_NORMALIZE);

    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_MULTISAMPLE_ARB);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV,GL_NICEST);

//	glShadeModel ( GL_FLAT );
//	glDepthFunc(GL_LEQUAL);

//		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

//glEnable(GL_COLOR_MATERIAL);

//VBO_enabled=false;

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

////// Создаём кубик!!!
IDrawObject *myObj= AddDrawObject("_Cube");

myObj->SetDrawType(ASN_DRAW_LINES);
myObj->SetDataBuffer( (void *) CubeBuffer , sizeof (float)*8, 8);
myObj->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);
myObj->SetDataOffset(ASN_DATA_UV_BUFF, 3*sizeof (float) );
myObj->SetDataOffset(ASN_DATA_NORMAL_BUFF, 5*sizeof (float) );
myObj->SetIndexBuffer( (void *) CubeIndex , sizeof (int)*2, 12 );

printf("_Cube ok\n");

return false;
}


//    /* Print all information about a key event */
//void PrintKeyInfo( SDL_KeyboardEvent *key ){
//        /* Is it a release or a press? */
//        if( key->type == SDL_KEYUP )
//            printf( "Release:- " );
//        else
//            printf( "Press:- " );
//
//        /* Print the hardware scancode first */
//        printf( "Scancode: 0x%02X", key->keysym.scancode );
//        /* Print the name of the key */
//        printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );
//        /* We want to print the unicode info, but we need to make */
//        /* sure its a press event first (remember, release events */
//        /* don't have unicode info                                */
//        if( key->type == SDL_KEYDOWN ){
//            /* If the Unicode value is less than 0x80 then the    */
//            /* unicode value can be used to get a printable       */
//            /* representation of the key, using (char)unicode.    */
//            printf(", Unicode: " );
//            if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
//                printf( "%c (0x%04X)", (char)key->keysym.unicode,
//                        key->keysym.unicode );
//
//                 wchar_t *S=L"coolman";
//
//                 char *t=(char*)SDL_iconv_string("ASCII", "UTF-8", (char*)S, SDL_strlen((const char*)S)+1);
//                 printf("convert: %s\n",t);
////            }
////            else{
////                printf( "? (0x%04X)", key->keysym.unicode );
////            }
//        }
//        printf( "\n" );
//        /* Print modifier info */
////        PrintModifiers( key->keysym.mod );
//    }
//

/*
int CDeviceGl::GetInput(){
    SDL_Event event;
if ( !SDL_PollEvent(& event) ) return ASN_EVNT_EMPTY;
            switch ( event.type )                          // what kind of event have we got ?
            {
                case SDL_QUIT :                                         // if user wishes to quit
                    return ASN_QUIT;
                case SDL_KEYDOWN:
//                    PrintKeyInfo( &event.key );
                    UnicodeKey=event.key.keysym.unicode;
                        LastKey=event.key.keysym.sym;
                    KeyModifier=event.key.keysym.mod;
                    return ASN_EVNT_KEYPRESSED;
                    break;
                case SDL_KEYUP:
                    UnicodeKey=event.key.keysym.unicode;
                    LastKey=event.key.keysym.sym;
                    KeyModifier=event.key.keysym.mod;
                    return ASN_EVNT_KEYUP;
                    break;

                case SDL_MOUSEMOTION:
                    mouse_x=(int)event.motion.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    // если фактически мышь не двигалась
                    //(видимо, сработало 2 раза подряд CenterMouse)
                    // - то и не будем беспокоить сообщением, а смотрим что там дальше
                    if ( (event.motion.xrel==0) & (event.motion.yrel==0) ) return GetInput();
                    return ASN_EVNT_MOUSEMOVE;
                	break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_x=(int)event.button.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    mouse_butt=convertMouseButton(event.button.button);
                    return ASN_EVNT_MOUSEDOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_x=(int)event.button.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    mouse_butt=convertMouseButton(event.button.button);
                    return ASN_EVNT_MOUSEUP;
                    break;
                case SDL_VIDEORESIZE :
                    ENGINE->SetEnvParam("[DeviceManager]width",IntToStr(event.resize.w));
                    ENGINE->SetEnvParam("[DeviceManager]height",IntToStr(event.resize.h));
                                  mouse_x=0;
                                  mouse_y=0;
                    Init(event.resize.w,event.resize.h,old_bpp,old_param);
                    SetMode(LastMode);
                    CenterMouse( MouseCursor );
                    return ASN_WINDOW_RESIZE;
                    break;

                case SDL_ACTIVEEVENT :
                    if ( event.active.state & SDL_APPACTIVE ) {
					if (event.active.gain) return ASN_WINDOW_ACTIVE_ON;
					     else return ASN_WINDOW_ACTIVE_OFF;
					 			}
                    if ( event.active.state & SDL_APPINPUTFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_INPUT_FOCUS_ON;
					     else return ASN_WINDOW_INPUT_FOCUS_OFF;
					 			}
                    if ( event.active.state & SDL_APPMOUSEFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_MOUSE_FOCUS_ON;
					     else return ASN_WINDOW_MOUSE_FOCUS_OFF;
					 			}
                    return GetInput();
                    break;

                default:                                    // any other event
                    return GetInput();
//                    return ASN_NONE;
                    break;
            };
};
*/
//float pos[] = { 1, 1, 1, 1 };
//float angle= 0.0f;

void CDeviceGl::StartDraw(){

//LOGGER->LogMsg("StartDraw begin");

//    glClear(GL_DEPTH_BUFFER_BIT);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearColor ( ClearColor.v[0], ClearColor.v[1], ClearColor.v[2], 1);
//    glClearColor ( 0.0, 0.5, 1.0, 1);
//   glClearColor ( 0, 0, 0, 1);
//    glClearColor ( 0.5, 0.5, 0.5, 0.0);

    glPushMatrix();

GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);// аа-а-а, нашел наконец-то где подсвечивать глобально!!

  glEnable(GL_LIGHTING);
  glEnable( GL_LIGHT0 );

  draw2dmode=false;
//LOGGER->LogMsg("StartDraw end");

}

bool CDeviceGl::PushDrawTask(IDrawObject *DrawObject){ //добавляем в очередь
CDrawTask myTask;
myTask.drawObject=DrawObject;
myTask.material=CurrentMaterial;
GetMatrixs(myTask.Model_Matrix, myTask.Project_Matrix);
myTask.Color=CurrentColor;
//myTask.LengthToCam=0.0f;
if (draw2dmode)
    PostDrawList2D.push_back(myTask);
else
    PostDrawList3D.push_back(myTask);
return true;
}

//bool CDeviceGl::PopDrawTask(){
    //std::list<CDrawTask>::iterator pp_DrawTask=PostDrawList.begin();
    //SetMatrixs((*pp_DrawTask).Model_Matrix, (*pp_DrawTask).Project_Matrix);
    //SetMaterial((*pp_DrawTask).material);
    //SetColor( (*pp_DrawTask).Color );
    //Draw_pure((*pp_DrawTask).drawObject);
    //PostDrawList.pop_back();
//};

void CDeviceGl::EndDraw(){


    if (draw2dmode) { LOGGER->ErrMsg(3,"Warning! Please call 'EndMode2D();' after StartMode2D() !");
        EndMode2D();
        draw2dmode= false;
    }

    //glDisable(GL_DEPTH_TEST);							// Отмена проверки глубины

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);

	glMatrixMode(GL_PROJECTION);						// Выбираем матрицу проектирования
	glPushMatrix();										// Сохраняем матрицу проектирования

	glMatrixMode(GL_MODELVIEW);							// Выбираем матрицу модели просмотра
	glPushMatrix();										// Сохраняем матрицу модели просмотра


    std::list<CDrawTask>::iterator pp_DrawTask=PostDrawList2D.begin();
    //printf("stack: %d\n",PostDrawList3D.size());

    for (int i=PostDrawList2D.size(); i>0; i--){
    SetMatrixs((*pp_DrawTask).Model_Matrix, (*pp_DrawTask).Project_Matrix);
    SetMaterial((*pp_DrawTask).material);
    SetColor( (*pp_DrawTask).Color );
    Draw_pure((*pp_DrawTask).drawObject);
    pp_DrawTask++;
    };


    pp_DrawTask=PostDrawList3D.begin();
//    printf("stack: %d\n",PostDrawList3D.size());

    for (int i=PostDrawList3D.size(); i>0; i--){
    SetMatrixs((*pp_DrawTask).Model_Matrix, (*pp_DrawTask).Project_Matrix);
    SetMaterial((*pp_DrawTask).material);
    SetColor( (*pp_DrawTask).Color );
    Draw_pure((*pp_DrawTask).drawObject);

    pp_DrawTask++;
    };

//printf("end draw GL\n");

    std::list<CDrawText>::iterator pp_DrawText=PostDrawText3D.begin();
    //printf("stack: %d\n",PostDrawText3D.size());

    // запоминаем наш текущий FontSize
    float temp_CurrentFontSize = CurrentFontSize;

    IFont *tmpFont = CurrentFont;

    for (int i=PostDrawText3D.size(); i>0; i--){
    SetMatrixs((*pp_DrawText).Model_Matrix, (*pp_DrawText).Project_Matrix);
    SetColor( (*pp_DrawText).Color );
    CurrentFont=(*pp_DrawText).font;
    CurrentFontSize=(*pp_DrawText).FontSize;
    PrintAt_pure((*pp_DrawText).x,(*pp_DrawText).y,(*pp_DrawText).text);
    pp_DrawText++;
    };

    pp_DrawText=PostDrawText2D.begin();

    for (int i=PostDrawText2D.size(); i>0; i--){
    SetMatrixs((*pp_DrawText).Model_Matrix, (*pp_DrawText).Project_Matrix);
    SetColor( (*pp_DrawText).Color );
    CurrentFont=(*pp_DrawText).font;
    CurrentFontSize=(*pp_DrawText).FontSize;
    PrintAt_pure((*pp_DrawText).x,(*pp_DrawText).y,(*pp_DrawText).text);
    pp_DrawText++;
    };


    CurrentFontSize=temp_CurrentFontSize;
    CurrentFont = tmpFont;
    //StartMode2D();

    //glEnable(GL_DEPTH_TEST);

     glDepthMask(true);

    PostDrawList3D.clear();
    PostDrawList2D.clear();
    PostDrawText3D.clear();
    PostDrawText2D.clear();

	glMatrixMode(GL_PROJECTION);						// Выбираем матрицу проектирования
	glPopMatrix();										// Восстанавливаем старую матрицу проектирования
	glMatrixMode(GL_MODELVIEW);							// Выбираем матрицу просмотра модели
	glPopMatrix();

//CreateSphere(CVector(5,5,5),10,40);
//	glLightfv(GL_LIGHT0, GL_POSITION, pos);
//	glColor4f ( 1, 1, 1 ,1);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);

//	glDisable(GL_TEXTURE_2D);
//	glTranslatef ( pos[0], pos[1], pos[2] );
//	gluSphere ( gluNewQuadric(), 20, 16, 16 );
//	glTranslatef ( -pos[0], -pos[1], -pos[2] );

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_2D);


    if ( std::string(ENGINE->GetEnvParam("[DeviceManager]ShowLines"))=="1") {

static CVector Zero=CVector(0,0,0);
static CVector ZeroToX=CVector(100,0,0);
static CVector ZeroToY=CVector(0,100,0);
static CVector ZeroToZ=CVector(0,0,100);

static CVector4 RedColor=CVector4(1,0,0,1);
static CVector4 GreenColor=CVector4(0,1,0,1);
static CVector4 BlueColor=CVector4(0,0,1,1);

glColor4fv(RedColor);

	glBegin(GL_LINES);
	glVertex3fv(Zero);
	glVertex3fv(ZeroToX);
	glEnd();

glColor4fv(GreenColor);

	glBegin(GL_LINES);
	glVertex3fv(Zero);
	glVertex3fv(ZeroToY);
	glEnd();

glColor4fv(BlueColor);

	glBegin(GL_LINES);
	glVertex3fv(Zero);
	glVertex3fv(ZeroToZ);
	glEnd();

//	glColor4f ( 1, 1, 1 ,1);

 	};

//glEnable(GL_TEXTURE_2D);

           glPopMatrix ();


//glLines(0,0,0);
//    glDisable(GL_TEXTURE_2D);
//    glColor3f (1,1,1);
//   	glInterleavedArrays( GL_C4UB_V3F, 0, g_lineVertices );
//    glDrawArrays( GL_LINES, 0, 6 );
//    glColor3f (1,1,1);
//    glEnable(GL_TEXTURE_2D);

    glColor4f (1,1,1,1);

    SDL_GL_SwapBuffers();


// вычисляем FPS:
    static char strFrameRate[50] = {0};          // We will store the string here for the window title
    static double frameTime = 0.0f;              // This stores the last frame's time
    double currentTime = SDL_GetTicks() * 0.001f;
    FrameInterval = (currentTime - frameTime);
    frameTime = currentTime;

    ++framesPerSecond;

    if( currentTime - lastTime > 1.0f )
    {
        FPS = (int)framesPerSecond;
        lastTime = currentTime;
        sprintf(strFrameRate, "FPS: %d", FPS );
        SDL_WM_SetCaption( ENGINE->GetEnvParam("[DeviceManager]window_name")  ,"AmberSkyNet");  // set the window caption (first argument) and icon caption (2nd arg)
//        SDL_WM_SetCaption(strFrameRate,"");            // The First argument is the window title
        framesPerSecond=0;
    }

}

bool CDeviceGl::SetMode(ASN_GRAPH_MODE_Types Typ){
LastMode=Typ;

ENGINE->SetEnvParam("[DeviceManager]mode",IntToStr(Typ).c_str());

int width_ = StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") );
int height_ = StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") );
GLfloat res_ = ((GLfloat)width_) / ((GLfloat)height_);

if (Typ==ASN_GRAPH_3D) {
glViewport(0,0, width_, height_ );
//glMatrixMode(GL_PROJECTION);
//glLoadIdentity();

//printf("CDeviceGl:SetSize(%d,%d,%f)\n", width_, height_, res_);
//glOrtho( 0, StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") ) / StrToInt( ENGINE->GetEnvParam("[DeviceManager]height")), 0, 1, -1, 1 );
//gluPerspective(45.0f,800.0f/600.f, 0.001f ,2000.0f);
//gluPerspective(45.0f,res_, 0.001f ,5000.0f);

//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();

//glEnable(GL_CULL_FACE);

//glEnable(GL_SCISSOR_TEST);
//glScissor(0, 0,width_x,width_y);

//glDepthFunc(GL_LESS);
//glEnable(GL_DEPTH_TEST);

//glPolygonMode(GL_FRONT, GL_FILL);
//glFrontFace( GL_CW );

//glEnable( GL_TEXTURE_2D );                             // Enable Texture Mapping

//glEnable(GL_DEPTH_TEST);
//glAlphaFunc(GL_GREATER,0.0f);

}
else if (Typ==ASN_GRAPH_2D){
glViewport(0,0,StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") ),StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") ));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho( -10, 10 /* StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") ) / StrToInt( ENGINE->GetEnvParam("[DeviceManager]height"))*/, -10, 10, -10, 10 );
//    glOrtho( -10,  StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") ) / StrToInt( ENGINE->GetEnvParam("[DeviceManager]height")), -10, 10, -10, 10 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);

}


void CDeviceGl::CenterMouse(bool ShowCursor){

MouseCursor=ShowCursor;
SDL_ShowCursor(ShowCursor);
mouse_x=atoi(ENGINE->GetEnvParam("[DeviceManager]width") )/2;
mouse_y=atoi(ENGINE->GetEnvParam("[DeviceManager]height") )/2;
SDL_WarpMouse(mouse_x,mouse_y);


//std::string Name="[DeviceManager]cursor";
//std::string Value="1";
//ENGINE->SetEnvParam(Name&,Value&);

if (ShowCursor)
ENGINE->SetEnvParam("[DeviceManager]cursor","1");
else
ENGINE->SetEnvParam("[DeviceManager]cursor","0");

}

void CDeviceGl::MouseToWorld(int mouse_x, int mouse_y, CVector& p1, CVector& p2)
{

/*
  GLdouble vx,vy,vz;       // координаты курсора мыши в системе координат viewport-a.
  GLdouble wx,wy,wz;       // возвращаемые мировые координаты.

//  glGetIntegerv(GL_VIEWPORT,viewport);           // узнаём параметры viewport-a.
//  GetMatrixs(modelview,projection);

  glGetDoublev(GL_PROJECTION_MATRIX,projection); // узнаём матрицу проекции.
  glGetDoublev(GL_MODELVIEW_MATRIX,modelview);   // узнаём видовую матрицу.

  // переводим оконные координаты курсора в систему координат viewport-a.
  vx = mouse_x;
  vy = mouse_y;//viewport[3] - mouse_y - 1;

//printf("viewport: %d %d %d %d\n",viewport[0],viewport[1],viewport[2],viewport[3]);

//glReadPixels(mouse_x, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);
  // вычисляем ближний конец селектирующего отрезка.
  vz = -1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p1 = CVector(wx,wy,wz);
  // вычисляем дальний конец селектирующего отрезка.
  vz = 1;
  gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
  p2 = CVector(wx,wy,wz);

//	glBegin(GL_LINES);
//	glVertex3fv(p1);
//	glVertex3fv(p2);
//	glEnd();
*/

 double      mdl[16], prj[16];
// double      Z = 0.0;
 double      x, y, z;
 double      x2, y2, z2;

 int      vx, vy, vz;
 int      viewport[4];

  glGetDoublev( GL_MODELVIEW_MATRIX, mdl );
  glGetDoublev( GL_PROJECTION_MATRIX, prj );
  glGetIntegerv( GL_VIEWPORT, viewport );

  vx = mouse_x;
  vy = mouse_y;//viewport[3] - mouse_y - 1;

//  glReadPixels( vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz );
  vz = -1;
  gluUnProject(vx, vy, vz, mdl, prj, viewport, &x, &y, &z );
  vz = 1;
  gluUnProject(vx, vy, vz, mdl, prj, viewport, &x2, &y2, &z2 );

  p1=CVector( x, y, z );
  p2=CVector( x2, y2, z2 );

//	glBegin(GL_LINES);
//	glVertex3fv(p1);
//	glVertex3fv(p2);
//	glEnd();

}

// цвет
void CDeviceGl::SetColor(const CVector4& Color){

//printf("set color...\n");

    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE/*GL_DIFFUSE*/); // убираем обе составляющие, исчезают и нормали
//GL_AMBIENT_AND_DIFFUSE
    glEnable(GL_COLOR_MATERIAL); // включаем изменение цвета материала через glColor
//printf("set color ok...\n");
                                // http://www.opengl.org/sdk/docs/man/xhtml/glColorMaterial.xml
    glColor4fv(Color.v);
    // возможно, потом будет не прямо glColor4fv, а в рендер-стейт
    // будет заноситься текущее значение glColor для последующей отрисовки

    if (Color.v[3]!=1.0) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
    else glDisable(GL_BLEND);

    CurrentColor=Color;
}

IMaterial *CDeviceGl::AddMaterial (const char* MaterialInfo){

//std::string myMaterialName=std::string( (const char*) MaterialInfo);

std::map<std::string,IMaterial*>::iterator it;

if ((it=MaterialList.find(MaterialInfo))==MaterialList.end()) {

LOGGER->LogMsg("Try create new material...");

IMaterial *myMaterial = new CMaterial ( Engine );

LOGGER->LogMsg("Try create new material...");

myMaterial->SetParam("Name",MaterialInfo);
std::string name=myMaterial->GetParam("Name");
MaterialList[name] = myMaterial;


return myMaterial;
}

return (*it).second; // уже есть такой материал;

//IBaseObject *myObj = new CBaseObject( Engine );
//myMaterial->SetParam("dd","dd");
//CTexture *test=TexturePack->LoadTexture( (const char *)MaterialInfo); //открываем текстуру

}

IMaterial *CDeviceGl::GetMaterial (const char* MaterialName){

std::map<std::string,IMaterial*>::iterator it;

if ((it=MaterialList.find(MaterialName))==MaterialList.end()) return NULL;

return (*it).second;

}

void CDeviceGl::SetMaterial (IMaterial *_Material){
// устанавливаем материал "по умолчанию"
if (!_Material) {
        _Material=DefaultMaterial;
        //glEnable(GL_COLOR_MATERIAL);
        //glColor4f(1,1,1,1);
}
else {
//glDisable(GL_COLOR_MATERIAL);
glColor4f(1,1,1,1);
CurrentColor=CVector4(1,1,1,1);
}

glDisable(GL_COLOR_MATERIAL);
glDisable(GL_BLEND);

CMaterial* Material=(CMaterial*)((void*)_Material);
//fprintf(stdout,"CDeviceGl: try set texture %s\n",Material->Texture.c_str());

if (Material->TextureOnly) {
    glDisable(GL_LIGHTING);
}
else {
  glEnable(GL_LIGHTING);

	glMaterialfv( GL_FRONT, GL_DIFFUSE, Material->DiffuseColor );
	glMaterialfv( GL_FRONT, GL_AMBIENT, Material->AmbientColor );
	glMaterialfv( GL_FRONT, GL_EMISSION, Material->EmissionColor );
	glMaterialfv( GL_FRONT, GL_SPECULAR, Material->SpecularColor );
	glMaterialf( GL_FRONT, GL_SHININESS, Material->Shininess );

};

//    glEnable( GL_AUTO_NORMAL );
//    glEnable( GL_NORMALIZE );

/*
	glPushMatrix();
	{
		glTranslatef( 0.0f, 1.2f, 0.0f );
		glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
		glRotatef( -g_fSpinZ, 0.0f, 0.0f, 1.0f );

//		glColor3f( 0.0f, 0.0f, 1.0f ); // This color won't get used unless we enable GL_COLOR_MATERIAL
//		renderSolidTeapot( 1.0f );
	}
	glPopMatrix();
*/


if (Material->Texture==ASN_NOT_FOUND_PARAM) {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
//fprintf(stdout,"material %s disable texture\n",Material->GetParam("Name").c_str());
    CurrentMaterial=Material;
return;
}

// если текстура не существует - загружаем её из TexturePack
if (!Material->MaterialTexture)
Material->MaterialTexture=TexturePack->LoadTexture( Material->Texture.c_str() );
Material->MaterialTexture->repeat=Material->TextureRepeat;

// если текстура еще не загружена в память...
//if (Material->MaterialTexture->tex_state!=ASN_FILE_DISK_IN_MEM)
if (Material->MaterialTexture->ID==0)
{
fprintf(stdout,"try call LoadInVideoMem %s\n",Material->Texture.c_str());
LoadInVideoMem(Material->MaterialTexture); //пробуем загрузить в память
if (Material->MaterialTexture->tex_state!=ASN_FILE_DISK_IN_MEM) {
    fprintf(stdout,"LoadInVideoMem [Fault!]\n");
    return; //нельзя загрузить его
// иначе - выставляем размеры текстуры прямо в материал :)
//Texture->width=Sc_w;
//Texture->height=Sc_h;
    };

 Material->SetParam("Tex_W",IntToStr(Material->MaterialTexture->width).c_str());
 Material->SetParam("Tex_H",IntToStr(Material->MaterialTexture->height).c_str());

}

//fprintf(stdout,"Ok. bind texture\n");

glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);
//    fprintf(stdout,"material %s enable texture\n",Material->GetParam("Name").c_str());

    glBindTexture(GL_TEXTURE_2D, Material->MaterialTexture->ID);
    if (Material->MaterialTexture->alpha) {
//    glEnable(GL_ALPHA_TEST);

//    glColor4f(1.0f,1.0f,1.0f,1.0f);   // Полная яркость, 50% альфа (НОВОЕ)
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE); // Функция смешивания для непрозрачности
//    glEnable(GL_BLEND);

//	glAlphaFunc(GL_GREATER,0.1f);								// Set Alpha Testing     (disable blending)
	glEnable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);

//    printf("Alpha test enabled...\n");
    }
    else glDisable(GL_ALPHA_TEST);

//glEnable(GL_ALPHA_TEST);
CurrentTexture=Material->MaterialTexture;
//printf("texsture OK in mem!");

	//glEnable(GL_COLOR_MATERIAL);
    //glColor4fv(Material->DiffuseColor);

    //glMaterialfv(GL_FRONT, GL_AMBIENT, Material->AmbientColor);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, Material->SpecularColor);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, Material->DiffuseColor);

    CurrentMaterial=Material;
}

ImageBitmap* CDeviceGl::GetBitmapFromImage(const char* ImageFileName, float scale){

    CTexture* tmpTexture = TexturePack->LoadTexture( ImageFileName );
    unsigned char *data = LoadTexture(tmpTexture, scale); // may be mem leak - malloc in dll and free on exe :(

    if (data) {
    CImageBitmap* myBitmap = new CImageBitmap();
    myBitmap->data=data;
    myBitmap->height = tmpTexture->height;
    myBitmap->width = tmpTexture->width;
    myBitmap->bpp = (tmpTexture->alpha ? 4 : 3);
    return myBitmap;
    }

    return NULL;

}

ImageBitmap* CDeviceGl::GetBitmapFromScreen(float scale){
    //ImageBitmap myBitmap;// = new ImageBitmap();
    //std::string scale_tmp = ENGINE->GetEnvParam("[DeviceManager]TextureDetail");
    return NULL;
}

unsigned char *CDeviceGl::LoadTexture(CTexture *Texture, double Scale){

fprintf(stdout,"try load %s ...\n",Texture->TexName.c_str());

std::string TextureName=Texture->TexName;

fprintf(stdout,"try get file %s",TextureName.c_str());

    Texture->TexFile=FILER->GetFile(TextureName.c_str()); //открываем текстуру

    if (!Texture->TexFile) {
    LOGGER->LogMsgStr("DEVICE: Ошибка! Файл "+TextureName+" нельзя открыть");
    return NULL;
    };

fprintf(stdout,"[OK]\n");

//файл должен быть загружен в память

if (Texture->TexFile->GetState()!=ASN_FILE_DISK_IN_MEM) {
fprintf(stdout,"Try load in mem %s with size %d...",Texture->TexName.c_str(),Texture->TexFile->GetSizeInBytes());

    Texture->TexFile->LoadInMem(); //пробуем загрузить
    printf(" [STATE] %d \n", Texture->TexFile->GetState() );

    if (Texture->TexFile->GetState()!=ASN_FILE_DISK_IN_MEM) {
        fprintf(stdout, "[FAULT!]\n");
            return NULL; //нельзя загрузить его
            }
fprintf(stdout, "[OK]\n");
}

Texture->filter_min = GL_LINEAR_MIPMAP_LINEAR;
Texture->filter_mag = GL_LINEAR;

//Device->ParamList.SetParam("MinNonScaleTextureSize","1");
//double Scale=1.0;
//if (need_scale)
//Scale=(double)StrToInt(ENGINE->GetEnvParam("[DeviceManager]TextureDetail"))/100.0f;

std::string Ext=Texture->TexFile->GetName();
Ext=GetFileExt( Ext );
if ( (Ext=="dds")||(Ext=="DDS") ) { LoadDDS(Texture); return NULL; } //пока не будем заморчаваться с DDS-шрифтами
//GLuint tex;
SDL_Surface *img = NULL;
//printf("%s\n",Ext.c_str());

SDL_RWops *rw_file=SDL_RWFromMem(Texture->TexFile->LoadInMem(),Texture->TexFile->GetSizeInBytes());
if ( (Ext=="TGA")||(Ext=="tga")) img = IMG_LoadTGA_RW(rw_file); //надеемся, что это ТГА ;)
else img = IMG_Load_RW(rw_file,0);
if (
(Ext=="JPG") || (Ext=="jpg") || (Ext=="BMP") || (Ext=="bmp")
) Texture->alpha=false;
Texture->TexFile->FreeMem();
if(img == NULL){
// NOTE: Supply your own error handling here!!!
LOGGER->LogMsgStr( "DEVICE: Unable to load texture from "+ASN_Str( Texture->TexFile->GetName() )+ std::string( IMG_GetError() ));
return NULL;}
// Build the texture from the surface
// RGB == 3 bytes, RGBA == 4 bytes
//Texture->alpha=false;

int pos = 0;
int Sc_h=img->h;
int Sc_w=img->w;

// scale temporary off-line

//if (
//(Sc_h>ParamList.GetIntParam("MinNonScaleTextureSize"))
//||
//(Sc_w>ParamList.GetIntParam("MinNonScaleTextureSize"))
//)
//{

//printf("Texture scaled...");
//StrToInt(ENGINE->GetEnvParam("[DeviceManager]TextureDetail"));
//Scale=1.5f;
//Scale=(float)StrToInt(ENGINE->GetEnvParam("[DeviceManager]TextureDetail"))/100.0f;
Sc_h=(int)(floor((float)Sc_h*Scale));
Sc_w=(int)(floor((float)Sc_w*Scale));
////Sc_w=(int)(floor((float)Sc_w*(float)Sc_h/img->h));
//if ( (float)(Sc_w/2.0f)-(int)(Sc_w/2)!=0 ) Sc_w--;
//if ( (float)(Sc_h/2.0f)-(int)(Sc_h/2)!=0 ) Sc_h--;

//printf("%s %d %d %f %f\n",Texture->filename.c_str(),Sc_h,Sc_w,img->h*Scale,img->w*Scale);
//}

int dim = Sc_w * Sc_h * (Texture->alpha ? 4 : 3);
GLubyte *data;
// NOTE: If you're using C instead of C++, use the malloc - call instead
data = new GLubyte[dim];

printf("%f data size: %d\n",Scale, dim);

if(!data){
// NOTE: Supply your own error handling here!!!
LOGGER->LogMsgStr("DEVICE: Unable to create a texture from "+ASN_Str( ((CTexture *)Texture)->TexFile->GetName()) );
// Traverse trough surface and grab the pixels
return NULL;
}

//for(int y=0; y<img->h; y++) {  //грузим вверх ногами
//for(int x=0; x<img->w; x++) {

for(int y=(Sc_h-1); y>-1; y--) {
for(int x=0; x<Sc_w; x++) {
Uint8 r,g,b,a;
int cur_x=(int)(x/Scale);
int cur_y=(int)(y/Scale);

//Uint32 color = getpixel(img, (int)pos_x, (int)pos_y);
Uint32 color = getpixel(img, (int)cur_x, (int)cur_y);
if(Texture->alpha)
SDL_GetRGBA(color, img->format, &r,&g,&b,&a);
else
SDL_GetRGB(color, img->format, &r,&g,&b);

data[pos] = r; pos++;
data[pos] = g; pos++;
data[pos] = b; pos++;
if(Texture->alpha) {data[pos] = a; pos++; //if (a!=255) printf("A:%d\n",a);
                }
}
}

SDL_FreeSurface(img);
Texture->width=Sc_w;
Texture->height=Sc_h;
Texture->bpp=(Texture->alpha) ? GL_RGBA : GL_RGB;
Texture->tex_state=ASN_FILE_DISK_IN_MEM;
Texture->TexFile->FreeMem();

return data;

}

void CDeviceGl::LoadInVideoMem(CTexture *Texture){
fprintf(stdout,"try load %s in video mem...\n",Texture->TexName.c_str());

double Scale=(double)StrToInt(ENGINE->GetEnvParam("[DeviceManager]TextureDetail"))/100.0f;

unsigned char *data=LoadTexture( Texture , Scale);
if (!data) return;

GLuint tex;
glGenTextures(1, &tex);// Generate texture ID
glBindTexture(GL_TEXTURE_2D, tex);

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture->filter_min);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture->filter_mag);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (Texture->repeat) ? GL_REPEAT : GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (Texture->repeat) ? GL_REPEAT : GL_CLAMP);

GLfloat max_anisotropy;
glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy );
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,max_anisotropy);

//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_DECAL);

//int type = (Texture->alpha) ? GL_RGBA : GL_RGB;

//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
//int genmip = 0;
//glGetTexParameteriv(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, &genmip);
//if (!genmip){
//gluBuild2DMipmaps(GL_TEXTURE_2D, type, Sc_w, Sc_h, type, GL_UNSIGNED_BYTE, data);
//}
//else
//{
//MyEngine->LogMsg("hardwave generate mipmap\n");
//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
//TexImage2D(GL_TEXTURE_2D, 0, type,img->w, img->h, 0, type, GL_UNSIGNED_BYTE, data);
Custom_gluBuild2DMipmaps(GL_TEXTURE_2D, Texture->bpp, Texture->width, Texture->height, Texture->bpp, GL_UNSIGNED_BYTE, data);

//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Sc_w, Sc_h, type, GL_UNSIGNED_BYTE, data);

delete [] data;

Texture->ID=tex;

}


void CDeviceGl::GetMatrixs(double *Model_Matrix, double *Project_Matrix){
	glGetDoublev ( GL_MODELVIEW_MATRIX,  Model_Matrix   );
	glGetDoublev ( GL_PROJECTION_MATRIX, Project_Matrix );
}

void CDeviceGl::SetMatrixs(double *Model_Matrix, double *Project_Matrix){
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(Project_Matrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(Model_Matrix);
}

void CDeviceGl::SetModelMatrix(double *Model_Matrix){
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(Model_Matrix);
}

void CDeviceGl::MultModelMatrix(double *Model_Matrix){
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixd(Model_Matrix);
}

void CDeviceGl::SetProjectMatrix(double *Project_Matrix){
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(Project_Matrix);
    glMatrixMode(GL_MODELVIEW);
}

void CDeviceGl::MultProjectMatrix(double *Project_Matrix){
    glMatrixMode(GL_PROJECTION);
    glMultMatrixd(Project_Matrix);
    glMatrixMode(GL_MODELVIEW);
}

void CDeviceGl::StartMode2D(bool normalize){
	glDisable(GL_DEPTH_TEST);							// Отмена проверки глубины
	glMatrixMode(GL_PROJECTION);						// Выбираем матрицу проектирования
	glPushMatrix();										// Сохраняем матрицу проектирования
	glLoadIdentity();									// Сбрасываем матрицу проекции

    //glOrtho( -10, 10 /* StrToInt( ENGINE->GetEnvParam("[DeviceManager]width") ) / StrToInt( ENGINE->GetEnvParam("[DeviceManager]height"))*/, -10, 10, -10, 10 );
    float prop=StrToFloat( ENGINE->GetEnvParam("[DeviceManager]width"))  /
        StrToFloat( ENGINE->GetEnvParam("[DeviceManager]height"));

	if (normalize) glOrtho(0,1,0,1,-1,1);					// Устанавливаем Ortho экран
	//glOrtho(0,640,0,480,-10,10);
	else glOrtho(0,StrToFloat( ENGINE->GetEnvParam("[DeviceManager]width")),0,StrToFloat( ENGINE->GetEnvParam("[DeviceManager]height")),-10,10);					// Устанавливаем Ortho экран

//    printf("%s %s\n", ENGINE->GetEnvParam("[DeviceManager]width"), ENGINE->GetEnvParam("[DeviceManager]height") );

	glMatrixMode(GL_MODELVIEW);							// Выбираем матрицу модели просмотра
	glPushMatrix();										// Сохраняем матрицу модели просмотра
	glLoadIdentity();

    draw2dmode=true;

}

void CDeviceGl::EndMode2D(){
	draw2dmode=false;
	glMatrixMode(GL_PROJECTION);						// Выбираем матрицу проектирования
	glPopMatrix();										// Восстанавливаем старую матрицу проектирования
	glMatrixMode(GL_MODELVIEW);							// Выбираем матрицу просмотра модели
	glPopMatrix();
}


void CDeviceGl::Rotate(const CVector& Point1){
glRotatef(Point1.v[0],1.0f,0.0f,0.0f);
glRotatef(Point1.v[1],0.0f,1.0f,0.0f);
glRotatef(Point1.v[2],0.0f,0.0f,1.0f);
}

void CDeviceGl::Move(const CVector& Point1){
glTranslatef(Point1.v[0],Point1.v[1],Point1.v[2]);
}

void CDeviceGl::SetScale(const CVector& Point1){
    glScalef(Point1.v[0],Point1.v[1],Point1.v[2]);
}

void CDeviceGl::PushMatrix(){
	glPushMatrix();
}

void CDeviceGl::PopMatrix(){
	glPopMatrix();
}

void CDeviceGl::SetScreen(int startX, int startY, int endX, int endY){

    glViewport(startX,startY,endX,endY);

}



/*

void CDeviceGl::Draw(const DrawType& type){

if (type.Type==ASN_GR_2D) {
SetMaterial(type.Material);
 GLfloat *vArray=( GLfloat*) type.VertexArray;
		glBegin(GL_QUADS);
        glTexCoord2i(0,0); glVertex2fv( vArray );
        glTexCoord2i(1,0); glVertex2fv( vArray+2 );
        glTexCoord2i(1,1); glVertex2fv( vArray+4);
        glTexCoord2i(0,1); glVertex2fv( vArray+6);
        glEnd();
}
else if (type.Type==ASN_GR_3DMESH){
//  glBegin(GL_TRIANGLES);
int i;
//  for (i=0;i<type.num_vert;i++){
//    glDisable(GL_TEXTURE_2D);
// 	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);

	SetMaterial(type.Material);

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, 1);


		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glActiveTextureARB(GL_TEXTURE0_ARB);

		glActiveTextureARB(GL_TEXTURE1);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glActiveTextureARB(GL_TEXTURE0);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glEnable(GL_TEXTURE_2D);

//    glDisable(GL_TEXTURE_2D);
//		glClientActiveTextureARB(GL_TEXTURE0_ARB+1);
//		glActiveTextureARB(GL_TEXTURE0_ARB+1);

 	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,sizeof(float)*8,type.VertexArray);
	glNormalPointer(GL_FLOAT,sizeof(float) * 8,(float *)type.VertexArray + 3);
	glTexCoordPointer(2,GL_FLOAT,sizeof(float) * 8,(float *)type.VertexArray + 6);
    glDrawArrays(GL_TRIANGLES,0,type.num_vert);
//    glDrawArrays(GL_LINES,0,type.num_vert);
  	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//	glVertexPointer(3,GL_FLOAT,sizeof(float) * 8,vertex);
//	glNormalPointer(GL_FLOAT,sizeof(float) * 8,vertex + 3);
//	glTexCoordPointer(2,GL_FLOAT,sizeof(float) * 8,vertex + 6);
//	glDrawArrays(GL_TRIANGLES,0,num_vertex);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
// //  };
//  glEnd();
};

};
*/

IDrawObject *CDeviceGl::AddDrawObject(const char* SceneElemInfo){

std::map<std::string,IDrawObject*>::iterator it;

if ((it=DrawObjectList.find(SceneElemInfo))==DrawObjectList.end()) {

CDrawObject *myDrawObject=new CDrawObject();
myDrawObject->Name=SceneElemInfo;
// если объект не уникальный, т.е. используется в нескольких местах - добавляем его в список
// уникальный объект имеет имя "-"
if (myDrawObject->Name!="-") { DrawObjectList[SceneElemInfo]=myDrawObject;
fprintf(stdout,"create draw object %s\n",SceneElemInfo); }
return myDrawObject;
}

fprintf(stdout,"get from cache %s\n",SceneElemInfo);
return (*it).second; // уже есть такой объект;


}

IDrawObject *CDeviceGl::GetDrawObject(const char* SceneElemName){
std::map<std::string,IDrawObject*>::iterator it;

if ((it=DrawObjectList.find(SceneElemName))==DrawObjectList.end()) return NULL;

return (*it).second;

}

void CreateSphere(CVector c,double r,int n);

//bool CDeviceGl::DrawTask(CDrawTask *drawTask){
//
//};

bool CDeviceGl::Draw(IDrawObject *SceneElem){
//unsigned int Num_vert;
//    printf("CDeviceGl::Draw\n");
if ( CurrentColor.v[3]<1.0)
        PushDrawTask(SceneElem);
else if ( (CurrentMaterial->MaterialTexture) && (CurrentMaterial->MaterialTexture->alpha) )
        PushDrawTask(SceneElem);
else
        Draw_pure(SceneElem);
}

bool CDeviceGl::Draw_pure(IDrawObject *SceneElem){
//    printf("Draw_pure\n");
CDrawObject *myElem=(CDrawObject *)SceneElem;

CurrentDrawObject=myElem;

//printf("SetMaterial %s\n",myElem->Name.c_str());

//if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) glEnable(GL_COLOR_MATERIAL);
//else glDisable(GL_COLOR_MATERIAL);

//printf("try draw %s\n",myElem->Name.c_str());

//& !(myElem->dynamic))


//if (myElem->dynamic){ printf("my elem dinamic\n"); }
//else
//    glEnable(GL_COLOR_MATERIAL);

#ifdef VBO_ON

    if ((VBO_enabled) & !(myElem->dynamic) ){

//printf( "VBO enabled\n");

    if (!myElem->DataBuffer) return true;

//    printf("Draw_pure2\n");

    if (myElem->vertexId==0) {

    glGenBuffersARB ( 1, &myElem->vertexId );
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB, myElem->vertexId );
    glBufferDataARB ( GL_ARRAY_BUFFER_ARB, myElem->BufSize * myElem->BufElemSize, myElem->DataBuffer, GL_STATIC_DRAW_ARB );

    if (myElem->IndexBuffer) {
    glGenBuffersARB ( 1, &myElem->indexId );
    glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, myElem->indexId );
    glBufferDataARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, myElem->IndexSize*myElem->IndexElemSize, myElem->IndexBuffer, GL_STATIC_DRAW_ARB );
	glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    };

	glBindBufferARB ( GL_ARRAY_BUFFER_ARB, 0 );

    };

//    printf("Draw_pure3\n");
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB ( GL_ARRAY_BUFFER_ARB, myElem->vertexId );
 	glVertexPointer ( 3, GL_FLOAT, myElem->BufElemSize, (GLvoid*) myElem->DataOffset[ASN_DATA_VERTEX_BUFF] );
//    printf("Draw_pure4\n");

	if (myElem->DataOffset[ASN_DATA_UV_BUFF]!=0) {
	       //glEnable(GL_COLOR_MATERIAL);
	       glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	       glTexCoordPointer ( 2, GL_FLOAT, myElem->BufElemSize, (GLvoid*) myElem->DataOffset[ASN_DATA_UV_BUFF] );
	       }
	else glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (myElem->DataOffset[ASN_DATA_NORMAL_BUFF]!=0) {
	        glEnableClientState(GL_NORMAL_ARRAY);
	        glNormalPointer( GL_FLOAT, myElem->BufElemSize, (GLvoid*) myElem->DataOffset[ASN_DATA_NORMAL_BUFF] );
	        }
	else glDisableClientState(GL_NORMAL_ARRAY);

	if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) {
             glEnableClientState(GL_COLOR_ARRAY); //glEnable(GL_COLOR_MATERIAL);
             glColorPointer( 3, GL_FLOAT, myElem->BufElemSize, (GLvoid*) myElem->DataOffset[ASN_DATA_COLOR_BUFF] );
             }
	else {/*glDisable(GL_COLOR_MATERIAL);*/ glDisableClientState(GL_COLOR_ARRAY); }

	if (myElem->IndexBuffer) glEnableClientState(GL_INDEX_ARRAY);
//    printf("Draw_pure5\n");

    if (myElem->IndexBuffer) {
//        printf("Draw_pure6\n");
        glEnableClientState(GL_INDEX_ARRAY);
        glBindBufferARB   ( GL_ELEMENT_ARRAY_BUFFER_ARB, myElem->indexId );
        glIndexPointer    ( GL_UNSIGNED_INT, 0, 0 );
//        printf("Draw_pure7\n");
        glDrawElements ( myElem->Draw_Type, myElem->IndexSize*myElem->NumPointInPrim, GL_UNSIGNED_INT, 0 );
        glBindBufferARB ( GL_ARRAY_BUFFER_ARB, 0 );
       	glBindBufferARB ( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
//        printf("Draw_pure7\n");
	} else {
//        printf("Draw_pure6\n");
    glDrawArrays(myElem->Draw_Type,0,myElem->BufSize);
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB, 0 );
//        printf("Draw_pure6\n");
    }

//    printf("draw VBO element\n");

    return false;

};  // VBO_enabled

#endif

if (!myElem->DataBuffer) return true;

//printf("draw dynamic element\n");

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer ( 3, GL_FLOAT, myElem->BufElemSize, ((char*)myElem->DataBuffer)+myElem->DataOffset[ASN_DATA_VERTEX_BUFF] );

	if (myElem->DataOffset[ASN_DATA_UV_BUFF]!=0)  {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer ( 2, GL_FLOAT, myElem->BufElemSize, ((char*)myElem->DataBuffer)+myElem->DataOffset[ASN_DATA_UV_BUFF] );
		}
	else glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (myElem->DataOffset[ASN_DATA_NORMAL_BUFF]!=0) {
	    glEnableClientState(GL_NORMAL_ARRAY);
	    glNormalPointer( GL_FLOAT, myElem->BufElemSize, ((char*)myElem->DataBuffer)+myElem->DataOffset[ASN_DATA_NORMAL_BUFF] );
	    }
	else glDisableClientState(GL_NORMAL_ARRAY);

	if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) {
	    //glEnable(GL_COLOR_MATERIAL);
	    glEnableClientState(GL_COLOR_ARRAY);
	    glColorPointer( 3, GL_FLOAT, myElem->BufElemSize, ((char*)myElem->DataBuffer)+myElem->DataOffset[ASN_DATA_COLOR_BUFF] );
        }
	else {/*glDisable(GL_COLOR_MATERIAL);*/ glDisableClientState(GL_COLOR_ARRAY); }

if (myElem->IndexBuffer) glDrawElements ( myElem->Draw_Type, myElem->IndexSize*myElem->NumPointInPrim, GL_UNSIGNED_INT, myElem->IndexBuffer );
else glDrawArrays(myElem->Draw_Type,0,myElem->BufSize);



/*
    glBegin(GL_TRIANGLES);
    for (int i=0;i<myElem->IndexSize;i++){

unsigned int* index=(unsigned int*) ((char*)myElem->IndexBuffer+i*myElem->IndexElemSize);

char *pointer=( char*)myElem->DataBuffer+index[0]*myElem->BufElemSize;
if (myElem->DataOffset[ASN_DATA_NORMAL_BUFF]!=0) glNormal3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_NORMAL_BUFF]) );
if (myElem->DataOffset[ASN_DATA_UV_BUFF]!=0) glTexCoord2fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_UV_BUFF]) );
if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) glColor3fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_COLOR_BUFF]) );

glVertex3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_VERTEX_BUFF]));

pointer=(char*)myElem->DataBuffer+index[1]*myElem->BufElemSize;
if (myElem->DataOffset[ASN_DATA_NORMAL_BUFF]!=0) glNormal3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_NORMAL_BUFF]) );
if (myElem->DataOffset[ASN_DATA_UV_BUFF]!=0) glTexCoord2fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_UV_BUFF]) );
if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) glColor3fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_COLOR_BUFF]) );
glVertex3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_VERTEX_BUFF]));

pointer=(char*)myElem->DataBuffer+index[2]*myElem->BufElemSize;
if (myElem->DataOffset[ASN_DATA_NORMAL_BUFF]!=0) glNormal3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_NORMAL_BUFF]) );
if (myElem->DataOffset[ASN_DATA_UV_BUFF]!=0) glTexCoord2fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_UV_BUFF]) );
if (myElem->DataOffset[ASN_DATA_COLOR_BUFF]!=0) glColor3fv( (float*) (pointer+myElem->DataOffset[ASN_DATA_COLOR_BUFF]) );

glVertex3fv ( (float*)(pointer+myElem->DataOffset[ASN_DATA_VERTEX_BUFF]));

    };
    glEnd();
*/

return false;
}


void CreateSphere(CVector c,double r,int n)
{
   int i,j;
   double theta1,theta2,theta3;
   CVector e,p;

   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 4 || r <= 0) {
      glBegin(GL_POINTS);
      glVertex3fv(c);
      glEnd();
      return;
   }

   for (j=0;j<n/2;j++)
   {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

//      glBegin(GL_QUAD_STRIP);
    glBegin(GL_TRIANGLE_STRIP);
//    glBegin(GL_LINES);
      for (i=0;i<=n;i++) {
         theta3 = i * TWOPI / n;

         e.v[0] = cos(theta2) * cos(theta3);
         e.v[1] = sin(theta2);
         e.v[2] = cos(theta2) * sin(theta3);
         p.v[0] = c.v[0] + r * e.v[0];
         p.v[1] = c.v[1] + r * e.v[1];
         p.v[2] = c.v[2] + r * e.v[2];

         glNormal3fv(e);
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3fv(p);

         e.v[0] = cos(theta1) * cos(theta3);
         e.v[1] = sin(theta1);
         e.v[2] = cos(theta1) * sin(theta3);
         p.v[0] = c.v[0] + r * e.v[0];
         p.v[1] = c.v[1] + r * e.v[1];
         p.v[2] = c.v[2] + r * e.v[2];

         glNormal3fv(e);
         glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3fv(p);
      }
      glEnd();
   }
}

void CDeviceGl::LoadDDS(CTexture *_Texture){
/*
DdsLoader *myDdsLoader= new DdsLoader();
Data* tmpData = new Data ( _Texture->TexFile->LoadInMem(), _Texture->TexFile->GetSizeInBytes() );
Texture *tmpTexture = myDdsLoader->load( tmpData );

UINT target;

    unsigned    textureId;

    glGenTextures   ( 1, &textureId );
    glBindTexture   ( GL_TEXTURE_2D, textureId );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (_Texture->repeat) ? GL_REPEAT : GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (_Texture->repeat) ? GL_REPEAT : GL_CLAMP);

    tmpTexture->upload(GL_TEXTURE_2D,true);

//    if ( !loadTexture ( GL_TEXTURE_2D, mipmap, fileName ) )
//        return 0;
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _Texture->filter_min);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _Texture->filter_mag);

//    return textureId;

_Texture->width=tmpTexture->getWidth();
_Texture->height=tmpTexture->getHeight();
_Texture->bpp=tmpTexture->getNumComponents();
_Texture->ID=textureId;
_Texture->tex_state=ASN_FILE_DISK_IN_MEM;
_Texture->TexFile->FreeMem();

delete tmpTexture;
delete tmpData;
delete myDdsLoader;
*/
}

void CDeviceGl::MakeScreenshot(const char *filename){

//    SDL_Surface *screen;
	SDL_Surface *temp;
	unsigned char *pixels;
	int i;

//	if (!(screen->flags & SDL_OPENGL))
//	{
//		SDL_SaveBMP(temp, filename);
//		return;
//	}

	temp = SDL_CreateRGBSurface(SDL_SWSURFACE, MainWindow->w, MainWindow->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
	0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
	);
	if (temp == NULL)
		return;

	pixels = (unsigned char*) malloc(3 * MainWindow->w * MainWindow->h);
	if (pixels == NULL)
	{
		SDL_FreeSurface(temp);
		return;
	}

	glReadPixels(0, 0, MainWindow->w, MainWindow->h, GL_RGB, GL_UNSIGNED_BYTE,
pixels);

	for (i=0; i<MainWindow->h; i++)
		memcpy(((char *) temp->pixels) + temp->pitch * i, pixels + 3*MainWindow->w
* (MainWindow->h-i-1), MainWindow->w*3);
	free(pixels);


	std::string FileName=filename;
	UINT index_name=0;

	while (true){
	FILE *file=fopen(	(FileName+IntToStr(index_name)+".bmp").c_str() ,"r");  // Открываем файл
	if (!file) break; // нет пока еще такого файла
	fclose(file);
	index_name++;
	};

	SDL_SaveBMP(temp, (FileName+IntToStr(index_name)+".bmp").c_str() );
	SDL_FreeSurface(temp);
//	return;

}

void CDeviceGl::BuildFont(const char* FontName, const char *File_name){

if ( FontList.find(FontName)!=FontList.end() ) return; //уже есть такой

std::string Ext=GetFileExt( std::string(File_name) );

    if ( (Ext=="TTF") || (Ext=="ttf") ) {
    CFontTTF *myFont = new CFontTTF(File_name);
    myFont->device=this;
    myFont->Engine=Engine;
    FontList[FontName]=myFont;
    }
    else {
    CFontTexture *myFont = new CFontTexture();
    myFont->device=this;
    myFont->Engine=Engine;
    myFont->FontTexture=TexturePack->LoadTexture( File_name );
    myFont->FontTexture->alpha=false;
    FontList[FontName]=myFont;
    };

}

void CDeviceGl::SetFont(const char* FontName){
if ( FontList.find(FontName)!=FontList.end() ) CurrentFont=FontList[FontName];
    else LOGGER->ErrMsgStr(3,std::string("Font ")+FontName+" can't find in font pool");
}

void CDeviceGl::PrintAt_pure(float x,float y, const std::string& Text){
    //SetScale( CVector(CurrentFontSize,CurrentFontSize,1) );
    //PushMatrix();
    //SetScale( CVector(CurrentFontSize,CurrentFontSize,1) );
if (CurrentFont)
    CurrentFont->printAt(x, y, CurrentFontSize, Text.c_str() );
    //PopMatrix();
    //SetScale( CVector(1.0f/CurrentFontSize,1.0f/CurrentFontSize,1) );
}

void CDeviceGl::PrintAt(float x,float y,const char *Text,...){

if(CurrentFont) {
	va_list arg;
	int done;
	char *s = new char[1024*16];
	va_start (arg, Text);
	done = vsprintf (s, Text, arg);
	va_end (arg);

    CDrawText myDrawText;

    GetMatrixs(myDrawText.Model_Matrix, myDrawText.Project_Matrix);
    myDrawText.Color=CurrentColor;
    myDrawText.text = s;
    myDrawText.font = CurrentFont;
    myDrawText.FontSize = CurrentFontSize;
    myDrawText.x = x;
    myDrawText.y = y;

//    printf("%f %s\n",CurrentFontSize,myDrawText.text.c_str());

if (draw2dmode)
    PostDrawText2D.push_back(myDrawText);
else
    PostDrawText3D.push_back(myDrawText);


  //CurrentFont->printAt(x,y,s);

//	my_vfprintf(x, y, s);
	delete (s);

}

//va_list		va;
//char buf[1024*16];


	// Копируем текст в buf, заменяя переменные их значениями
//	va_start(va, Text);
//	vsprintf(buf, Text, va);
//	va_end(va);

//if(CurrentFont)
//  CurrentFont->printAt(x,y,buf);

}

/*
void CDeviceGl::BuildFont(const char* FontName, const char *File_name){
if ( FontList.find(FontName)!=FontList.end() ) return; //есть уже такой

printf("buid font \n");
ASN_Font *font = new ASN_Font();
font->FontTexture=TexturePack->LoadTexture( File_name );
font->FontTexture->alpha=false;
FontList[FontName]=font;

};

void CDeviceGl::SetFont(const char* FontName){
if ( FontList.find(FontName)!=FontList.end() ) CurrentFont=FontList[FontName];
};

void CDeviceGl::PrintAt(float x,float y,const char *Text,...){

// если текстура шрифта еще не загружена - пробуем загрузить
if (CurrentFont->FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)
{
printf("Try create font...\n");

CTexture *Texture=CurrentFont->FontTexture; // влом переписывать кусок кода :)

unsigned char *data=LoadTexture( CurrentFont->FontTexture , false);
if (!data) {
 LOGGER->ErrMsg(3,"Can't create font :(");
return;
};

    GLuint tex;
    glGenTextures(1, &tex);// Generate texture ID
    glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture->filter_min);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture->filter_mag);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (Texture->repeat) ? GL_REPEAT : GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (Texture->repeat) ? GL_REPEAT : GL_CLAMP);

Custom_gluBuild2DMipmaps(GL_TEXTURE_2D, Texture->bpp, Texture->width, Texture->height, Texture->bpp, GL_UNSIGNED_BYTE, data);
//glTexImage2D(GL_TEXTURE_2D,0,Texture->bpp,Texture->width,Texture->height,0,Texture->bpp,GL_UNSIGNED_BYTE,data);

	CurrentFont->Width=1.0f;
	CurrentFont->Height=1.0f;

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
				CurrentFont->FTrim[x+y*16][0]=0;
				CurrentFont->FTrim[x+y*16][1]=16;
				continue;
			}

			int cx,cy;
			cy=y*16;
			cx=x*16-1;
			BOOL space=TRUE;
			while (space)
			{
				cx++;
				for (int t=0;t<16;t++)
					if (data[(cx+(256-cy-t)*256)*3]>Zero)
					{
						space=FALSE;
						break;
					}

			}
			CurrentFont->FTrim[x+y*16][0]=cx-x*16;
			cy=y*16;
			cx=(x+1)*16;
			space=TRUE;
			while (space)
			{
				cx--;
				for (int t=0;t<16;t++)
					if (data[(cx+(256-cy-t)*256)*3]>Zero)
					{
						space=FALSE;
						break;
					}

			}
			CurrentFont->FTrim[x+y*16][1]=cx+1-CurrentFont->FTrim[x+y*16][0]-x*16;
			if (CurrentFont->FTrim[x+y*16][1]<16)
			if ( CurrentFont->FTrim[x+y*16][1] > SpaceWidth ) {
			   SpaceWidth=CurrentFont->FTrim[x+y*16][1]; }
			   else SpaceWidth=SpaceWidth;
		}

		delete [] data;
		CurrentFont->FTrim[32][1]=2*SpaceWidth/3;
		CurrentFont->FontTexture->tex_state=ASN_FILE_DISK_IN_MEM;
		CurrentFont->FontTexture->ID=tex;

		glBindTexture(GL_TEXTURE_2D, Texture->ID);
	float	cx;											// Содержит X координату символа
	float	cy;											// Содержит Y координату символа
    float   cwx;                                        // Ширина буквы в пикселях
	float   cwy;                                        // Высота буквы в пикселях

	cwx         = (1.0f / 256.0f) * 16;
	cwy         = (1.0f / 256.0f) * 16;

	CurrentFont->Base=glGenLists(256);								// Создаём 256 дисплей листов
	for (int loop=0; loop<256; loop++)						// Циклимся по всем листам
	{
		// X и Y позиция

		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;
		glNewList(CurrentFont->Base+loop,GL_COMPILE);				// Начинаем заполнять лист

			glBegin(GL_TRIANGLE_STRIP);							// Рисуем букву четырехугольником

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0]+CurrentFont->FTrim[loop][1])/256.0f,1-cy);
				glVertex2f((float)CurrentFont->FTrim[loop][1],0);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0]+CurrentFont->FTrim[loop][1])/256.0f,1-cy-cwy);
				glVertex2f((float)CurrentFont->FTrim[loop][1],-16.0f);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0])/256.0f,1-cy);
				glVertex2f(0,0);

				glTexCoord2f(cx+(CurrentFont->FTrim[loop][0])/256.0f,1-(cy+cwy));
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
			glTranslatef((GLfloat)(CurrentFont->FTrim[loop][1]+1),0,0);

		glEndList();									// Закончили лист
	}													// Цикл по буквам

}//if (CurrentFont->FontTexture->tex_state!=ASN_FILE_DISK_IN_MEM)

    glDisable(GL_LIGHTING);

glEnable(GL_TEXTURE_2D);

	va_list		va;									// Ссылка на список аргументов

char buf[1024];

	// Копируем текст в buf, заменяя переменные их значениями
	va_start(va, Text);
	vsprintf(buf, Text, va);
	va_end(va);

//printf("%s\n",buf);
	// Включаем текстуру
glBindTexture(GL_TEXTURE_2D, CurrentFont->FontTexture->ID);
//	glBindTexture(GL_TEXTURE_2D, fonts);			// Выбираем нашу текстуру шрифта

//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFunc(GL_ONE,GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)

//	glEnable(GL_ALPHA_TEST);

	glTranslatef(x,y+16.0f,0);

	glListBase(CurrentFont->Base);

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
    *
	glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);

//	glDisable(GL_ALPHA_TEST);

//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);

};
*/

void CDeviceGl::SetFontSize(const float size){
    CurrentFontSize=size;
}

void CDeviceGl::GetGlyphSize(const char Text, float &x, float &y){
    x=CurrentFontSize*CurrentFont->getWidth(Text);
    y=CurrentFontSize*CurrentFont->getHeight(Text);

//    for (int i=0;i<strlen(Text);i++) {
//        x+=CurrentFont->getWidth(Text[i])+CurrentFont->getSpace(Text[i]);
//    }
}

void CDeviceGl::GetTextWH(const char* Text, float &width, float &height){

if (!CurrentFont){

    LOGGER->ErrMsgStr(2,"DEVICE: default font can't init!\n");

    width=1;
    height=1;
    return;
};

    float max_height = 0.0; //максимальая высота буквы в строке
    float max_width = 0.0; //максимальная ширина текста в столбике
    float summ_height = 0.0; // суммарная высота всех строк текста

    width=0.0;
    height=0.0;

    for (int i=0;i<strlen(Text);i++) {
        if ( Text[i]=='\n' ) {
         if ( width > max_width ) max_width = width;
         width=0.0;
         summ_height+=max_height;
         max_height=0.0;
//         printf("N!!!\n");
//         printf("%f %f\n",max_width,summ_height);
         }
        width+=CurrentFontSize*CurrentFont->getWidth(Text[i]);
        width+=CurrentFontSize*CurrentFont->getSpace(Text[i]);

        height = CurrentFontSize*CurrentFont->getHeight(Text[i]);
        if ( height > max_height ) max_height = height;

//        printf("width: %f %f %d\n",CurrentFontSize,width, i);
    }

    if ( width > max_width ) max_width = width; //сравниваем последнюю строку

    if (max_width>0.0) width = max_width; // если у нас встречалось /n - выводит максимальную ширину

    if (summ_height>0.0) height = summ_height+max_height; // если несколько строк, то выдаем суммарную высоту
        else height = max_height; // иначе - максимальную высоту символа в строке
        //printf("width: %f %f %d\n",CurrentFontSize,width, i);

}

void CDeviceGl::SetParam(const char* param_name,const char*param_value){

if ( strcmp(param_name, "ClearColor") == 0 ) {
        // будем использовать цвет у ноды
        ClearColor = StrToVector(param_value);
    }

    CBaseObject::SetParam(param_name,param_value);
}

int CDeviceGl::EventSDLManagerDelegate(void* ptr){
/*
    CEventManagerSDL* manager=(CEventManagerSDL*)ptr;

    SDL_Event event;
if ( !SDL_PollEvent(& event) ) return ASN_EVNT_EMPTY;
            switch ( event.type )                          // what kind of event have we got ?
            {
                case SDL_QUIT :                                         // if user wishes to quit
                    return ASN_QUIT;
                case SDL_KEYDOWN:
//                    PrintKeyInfo( &event.key );
                    manager->UnicodeKey=event.key.keysym.unicode;
                    manager->LastKey=event.key.keysym.sym;
                    manager->KeyModifier=event.key.keysym.mod;
                    return ASN_EVNT_KEYPRESSED;
                    break;
                case SDL_KEYUP:
                    manager->UnicodeKey=event.key.keysym.unicode;
                    manager->LastKey=event.key.keysym.sym;
                    manager->KeyModifier=event.key.keysym.mod;
                    return ASN_EVNT_KEYUP;
                    break;

                case SDL_MOUSEMOTION:
                    mouse_x=(int)event.motion.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    // если фактически мышь не двигалась
                    //(видимо, сработало 2 раза подряд CenterMouse)
                    // - то и не будем беспокоить сообщением, а смотрим что там дальше
                    if ( (event.motion.xrel==0) & (event.motion.yrel==0) ) return EventSDLManagerDelegate(ptr);
                    return ASN_EVNT_MOUSEMOVE;
                	break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_x=(int)event.button.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    mouse_butt=convertMouseButton(event.button.button);
                    return ASN_EVNT_MOUSEDOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_x=(int)event.button.x;
                    mouse_y=StrToInt( ENGINE->GetEnvParam("[DeviceManager]height") )-(int)event.motion.y;
                    mouse_butt=convertMouseButton(event.button.button);
                    return ASN_EVNT_MOUSEUP;
                    break;
                case SDL_VIDEORESIZE :
                    ENGINE->SetEnvParam("[DeviceManager]width",IntToStr(event.resize.w));
                    ENGINE->SetEnvParam("[DeviceManager]height",IntToStr(event.resize.h));
                                  mouse_x=0;
                                  mouse_y=0;
                    Init(event.resize.w,event.resize.h,old_bpp,old_param);
                    SetMode(LastMode);
                    CenterMouse( MouseCursor );
                    return ASN_WINDOW_RESIZE;
                    break;

                case SDL_ACTIVEEVENT :
                    if ( event.active.state & SDL_APPACTIVE ) {
					if (event.active.gain) return ASN_WINDOW_ACTIVE_ON;
					     else return ASN_WINDOW_ACTIVE_OFF;
					 			}
                    if ( event.active.state & SDL_APPINPUTFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_INPUT_FOCUS_ON;
					     else return ASN_WINDOW_INPUT_FOCUS_OFF;
					 			}
                    if ( event.active.state & SDL_APPMOUSEFOCUS ) {
					if (event.active.gain) return ASN_WINDOW_MOUSE_FOCUS_ON;
					     else return ASN_WINDOW_MOUSE_FOCUS_OFF;
					 			}
                    return EventSDLManagerDelegate(ptr);
                    break;

                default:                                    // any other event
                    return EventSDLManagerDelegate(ptr);
//                    return ASN_NONE;
                    break;
            };

*/

// return ASN_EVNT_EMPTY;
}
