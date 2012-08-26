#ifndef _CDEVICE_H
#define _CDEVICE_H
#include "IDevice.h"
#include "IEngine.h"

#include "SDL/SDL.h"
#include "lib/GLee.h"
//#include "lib/extgl.h"
//#include <GL/gl.h>

#include "IFont.h"

//class IFont;
#include "CTexturePack.h"
#include "CMaterial.h"
#include "CDrawObject.h"
#include <map>
#include <list>

class CImageBitmap: public ImageBitmap {
    public:
        int GetWidth(){return width;};
        int GetHeight(){return height;};
        int GetBpp(){return bpp;};
        unsigned char *GetData(){ return data;};
        void Release(){ if (data) {free(data);data=NULL;} };

        int width;
        int height;
        int bpp;
        unsigned char *data;
};

struct ASN_Font {

	// Название шрифта
	std::string Name;
	// Ширина щрифта
	float Width;
	// Высота шрифта
	float Height;
	// ID Текстуры
	GLuint TextureID;

    GLuint Base; // для List
	int FTrim[256][2];

	CTexture *FontTexture;

};

struct CDrawTask {
IDrawObject *drawObject;
IMaterial *material;
double Model_Matrix[16];
double Project_Matrix[16];
CVector4 Color;
//double LengthToCam;
};

struct CDrawText {
double Model_Matrix[16];
double Project_Matrix[16];
CVector4 Color;
std::string text;
IFont *font;
float FontSize;
float x;
float y;
};

class CDeviceGl: public IDevice, public CBaseObject {
public:
CDeviceGl(IEngine *engine);
~CDeviceGl();

void InitManager();
int Release();
void Update(float tms);

bool Init(int width, int height, int bpp, int param);
bool SetMode(ASN_GRAPH_MODE_Types Typ);

// скриншот экрана сделать
void MakeScreenshot(const char *filename);

//int GetInput();

void StartDraw();
bool DrawObject(IDrawObject *SceneElem);
void EndDraw();

// цвет
void SetColor(const CVector4& Color);

// материалы
IMaterial *AddMaterial (const char* MaterialInfo);
IMaterial *GetMaterial (const char* MaterialName);
void SetMaterial (IMaterial *Material);

// элемент сцены
IDrawObject *AddDrawObject(const char* SceneElemInfo);
IDrawObject *GetDrawObject(const char* SceneElemName);
bool Draw(IDrawObject *DrawObject);

//// битмапы
ImageBitmap* GetBitmapFromImage(const char* ImageFileName, float scale);
ImageBitmap* GetBitmapFromScreen(float scale);

// позиционирование
void CenterMouse(bool ShowCursor);
void MouseToWorld(int mouse_x, int mouse_y, CVector& p1, CVector& p2);
void GetMatrixs(double *Model_Matrix, double *Project_Matrix);
void SetMatrixs(double *Model_Matrix, double *Project_Matrix);
void SetModelMatrix(double *Model_Matrix);
void MultModelMatrix(double *Model_Matrix);
void SetProjectMatrix(double *Project_Matrix);
void MultProjectMatrix(double *Project_Matrix);
void Rotate(const CVector& Point1);
void Move(const CVector& Point1);
void SetScale(const CVector& Point1);
void PushModelMatrix();
void PopModelMatrix();
void PushMatrix();
void PopMatrix();

virtual void StartMode2D(bool normalize);
virtual void EndMode2D();

void SetScreen(int startX, int startY, int sizeX, int sizeY);

void BuildFont(const char* FontName, const char *File_name);
void SetFont(const char* FontName);
void SetFontSize(const float size);
void PrintAt(float x,float y,const char *Text,...);
void GetGlyphSize(const char Text, float &x, float &y);
void GetTextWH(const char* Text, float &width, float &height);

void SetParam(const char* param_name,const char*param_value);

//"private:"
//void Draw(const DrawType& type);

bool PushDrawTask(IDrawObject *DrawObject); //добавляем в очередь
bool DrawTask(CDrawTask *drawTask);
bool Draw_pure(IDrawObject *DrawObject); // быстро рисуем, без анализа
void PrintAt_pure(float x,float y, const std::string& Text);

//bool PopDrawTask();

int EventSDLManagerDelegate(void* ptr);

void LoadInVideoMem(CTexture *Texture);
unsigned char *LoadTexture(CTexture *Texture, double Scale=1.0f);

int old_bpp;
int old_param;

void LoadDDS(CTexture *Texture);

ASN_GRAPH_MODE_Types LastMode;

//IEngine *Engine;
char *ScreenData;
SDL_Surface *MainWindow;


CTexturePack *TexturePack;

    float framesPerSecond;
    float lastTime;

CTexture *CurrentTexture;
IMaterial *DefaultMaterial;

  GLint    viewport[4];    // параметры viewport-a.
  GLdouble projection[16]; // матрица проекции.
  GLdouble modelview[16];  // видовая матрица.

bool VBO_enabled;

std::map<std::string,IMaterial*> MaterialList;
std::map<std::string,IDrawObject*> DrawObjectList;

ImageBitmap* ScreenBitmap;
std::map<std::string,ImageBitmap*> BitmapList;

std::map<std::string,IFont*> FontList; //список шрифтов
IFont *CurrentFont;

CMaterial* CurrentMaterial;
CDrawObject* CurrentDrawObject;
CVector4 CurrentColor;
CVector ClearColor;   // цвет пустого пространства на поле
float CurrentFontSize;

bool draw2dmode;

std::list<CDrawTask> PostDrawList3D;
std::list<CDrawText> PostDrawText3D;

std::list<CDrawTask> PostDrawList2D;
std::list<CDrawText> PostDrawText2D;

};


#endif
