#ifndef _C_CAMERA_H
#define _C_CAMERA_H

#include "ICamera.h"
#include "CBaseObject.h"
#include "quaternion.h"

#include "matrix.h"

class CCamera: public ICamera, public CBaseObject {
public:

CCamera(IEngine *_Engine);
virtual ~CCamera();

virtual void LookAt(const CVector& _Pos);

virtual void View();
virtual void SetScreen(float startX, float startY, float sizeX, float sizeY); // в процентах от экрана
virtual void SetPerspective(float fov, float aspect, float zNear, float zFar);
virtual void SetScale(float scale);

virtual void MoveForward(const float val);
virtual void MoveStrafe(const float val);
virtual void MoveUp(const float val);

virtual void SetPos(const CVector& _Pos);
virtual const CVector& GetPos() const;

virtual void SetRotX(const float val);
virtual void SetRotY(const float val);

virtual void LookAtMouse(int Pos_X, int Pos_Y);

virtual void GetRot(float& RotX, float& RotY) const;

float psi, phi;

CVector Dir; //направление камеры
CVector DirY; //вектор, перпендикул€рный направлению камеры по y
CVector DirZ; //вектор, перпендикул€рный направлению камеры по y

//CVector Speed;
virtual bool AddChild(IBaseObject* child);
virtual bool DelChild(UID childID);

protected:

float screen[4];
//int oldWidth;
//int oldheight;

Matrix NodeMatrix; // матрица вида
Matrix ProjectMatrix; // матрица проекции

CVector Pos; // координаты
void CalcDir();

};

#endif
