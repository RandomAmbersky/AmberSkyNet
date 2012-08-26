#ifndef _IDEVICE_H
#define _IDEVICE_H

// ���� ����. ������� - 2d, 3d
enum ASN_GRAPH_MODE_Types {
ASN_GRAPH_2D,
ASN_GRAPH_3D,
};

// ��������� �������:
#define ASN_DEV_FULLSCREEN 0x1
#define ASN_DEV_NOFRAME    0x2
#define ASN_DEV_CANRESIZE  0x4

#include "CVector.h"
#include "IDrawObject.h"
#include "IManager.h"

#include <string>

class ImageBitmap {
    public:
        virtual int GetWidth()=0;
        virtual int GetHeight()=0;
        virtual int GetBpp()=0;
        virtual unsigned char *GetData()=0;
        virtual void Release()=0;
};

class IDevice: public IManager {
public:
//virtual ~IDevice(){};

virtual bool Init(int width=640, int height=480, int bpp=32, int param=0)=0;
virtual bool SetMode(ASN_GRAPH_MODE_Types Typ)=0;

// �������� ������ �������
virtual void MakeScreenshot(const char *filename)=0;

virtual void StartDraw()=0;
virtual void EndDraw()=0;

// ����
virtual void SetColor(const CVector4& Color)=0; //RGB � 4� �������� - alpha

// ���������
virtual IMaterial *AddMaterial (const char* MaterialInfo)=0;
virtual IMaterial *GetMaterial (const char* MaterialName)=0;
virtual void SetMaterial(IMaterial *Material)=0;

// ������� �����
virtual IDrawObject *AddDrawObject(const char* SceneElemInfo)=0;
virtual IDrawObject *GetDrawObject(const char* SceneElemName)=0;

//// �������
virtual ImageBitmap* GetBitmapFromImage(const char* ImageFileName, float scale)=0;
virtual ImageBitmap* GetBitmapFromScreen(float scale)=0;

// ������������ ������
virtual bool Draw(IDrawObject *SceneElem)=0;

// ������ � ������
virtual void MouseToWorld(int mouse_x, int mouse_y, CVector& p1, CVector& p2)=0;
virtual void CenterMouse(bool ShowCursor)=0;

// ����������������
virtual void GetMatrixs(double *Model_Matrix, double *Project_Matrix)=0;
virtual void SetMatrixs(double *Model_Matrix, double *Project_Matrix)=0;
virtual void SetModelMatrix(double *Model_Matrix)=0;
virtual void MultModelMatrix(double *Model_Matrix)=0;
virtual void SetProjectMatrix(double *Project_Matrix)=0;
virtual void MultProjectMatrix(double *Project_Matrix)=0;
virtual void PushMatrix()=0;
virtual void PopMatrix()=0;

virtual void StartMode2D(bool normalize=false)=0;
virtual void EndMode2D()=0;

virtual void Rotate(const CVector& Point1)=0;
virtual void Move(const CVector& Point1)=0;
virtual void SetScale(const CVector& Point1)=0;

// ��������, � ������� ����� ��������� �����������
virtual void SetScreen(int startX, int startY, int sizeX, int sizeY)=0;

// ������
virtual void BuildFont(const char* FontName, const char *File_name)=0;
virtual void SetFont(const char* FontName)=0;
virtual void SetFontSize(const float size)=0;
virtual void PrintAt(float x,float y,const char *Text,...)=0;
// �������� ������� 1 �������
virtual void GetGlyphSize(const char Text, float &width, float &height)=0;
// �������� ������� ����� ( c ������ /n - ������� ������!
// ������� ��� ���������������� �������� �� ������ ������
virtual void GetTextWH(const char* Text, float &width, float &height)=0;

int FPS;
float FrameInterval; //�������� ����� ��������, ��� ���������� ��������

//int LastKey;
//UINT16 UnicodeKey;			/* translated character */
//unsigned short KeyModifier;         /* Alt, Ctrl, ��� */

// ��������� ����� �� ������
int mouse_x;
int mouse_y;
bool MouseCursor; //������������ �� ������ �����
unsigned char mouse_butt;
};

#define DEVICER ((IDevice*)ENGINE->GetPtrParam("DeviceManager"))

#endif
