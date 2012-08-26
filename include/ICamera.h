#ifndef _I_CAMERA_H
#define _I_CAMERA_H

#include "IBaseObject.h"
#include "CVector.h"

#include "INode.h"

class ICamera: public virtual IBaseObject {
public:

virtual ~ICamera(){}

virtual void LookAt(const CVector& Pos)=0;

virtual void View()=0;
virtual void SetScreen(float startX, float startY, float sizeX, float sizeY)=0;

virtual void SetPerspective(float fov, float aspect, float zNear, float zFar)=0;

virtual void MoveForward(const float val)=0;
virtual void MoveStrafe(const float val)=0;
virtual void MoveUp(const float val)=0;

virtual void SetPos(const CVector& Pos)=0;
virtual const CVector& GetPos() const =0;

virtual void SetRotX(const float val)=0;
virtual void SetRotY(const float val)=0;

virtual void GetRot(float& RotX, float& RotY) const =0;

virtual void LookAtMouse(int Pos_X, int Pos_Y)=0;

};

#endif
