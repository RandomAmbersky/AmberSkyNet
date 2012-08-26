#ifndef _MATRIX_H
#define _MATRIX_H

#include "CVector.h"

class Matrix
{
public:
	Matrix();
	~Matrix();

//	void MakeSquish(Point3D * point, Facet3D * facet);
	void RotateXYZ(const CVector& _Rot);
	void MakePitch(double a);
	void MakeRoll(double a);
	void MakeYaw(double a);
	void MakeQuaternionAA(double a, double x, double y, double z/*, bool radians = 0*/);
	void LoadIdentity();
	void Scale (const CVector& size);
//	void MakeFromVectorsRow(Point3D * a,Point3D * b, Point3D * c, Point3D * d);
//	void MakeFromVectorsColumn(Point3D * a,Point3D * b, Point3D * c,Point3D * d);
    void Perspective(float fov, float aspect, float near, float far);
	void Translate(float x, float y, float z);

	void LookAt(const CVector& pos, const CVector& target, const CVector& up);
	CVector GetRot(); 

	Matrix InvertRotTrans();
//	{
//	};

	//Brett Porter functions
//	void InverseRotateVect( Point3D * point );
//	void InverseTranslateVect( Point3D * point );
//	void SetTranslation(float x, float y, float z);
//	void SetInverseTranslation(float x, float y, float z);
//	void SetRotationRadians(float x, float y, float z);
//	void SetInverseRotationRadians(float x, float y, float z);

	void operator *=(Matrix & matrix);
	CVector operator*(const CVector &vec) const;

	double m_data[16];

};

#endif
