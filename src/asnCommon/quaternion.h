#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "CVector.h"

class Quaternion {
public:
	Quaternion operator *(Quaternion q);
	void CreateMatrix(double *pMatrix);
	void CreateFromAxisAngle( const CVector& Axis, float degrees);
	Quaternion();
	virtual ~Quaternion();

	inline float Norma()
	{
		return sqrt(x*x + y*y + z*z + w*w);
		
	}
	// Нормализация
	inline void Normalize()
	{
		float norm = Norma();
		x/= norm;
		y/= norm;
		z/= norm;
		w/= norm;
		
	}

private:
	float w;
	float z;
	float y;
	float x;	
};

/*
class Quaternion {
public:
	inline Quaternion() { x=0; y=0; z=0; w=1; }
	inline Quaternion( const float _x, const float _y, const float _z, const float _w)
	{ x=_x; y=_y; z=_z; w=_w; };

	inline void FromAxisAngle(float Angle, CVector Axis)
	{
		double rad=(Angle * 0.5f)*M_PI/180;
		w = (float)cos(rad);
		double scale = sin(rad);
		Axis.Normalize();
		x=float(Axis.v[0]*scale);
		y=float(Axis.v[1]*scale);
		z=float(Axis.v[2]*scale);
		Normalize();
	}
	inline float Norma()
	{
		return sqrt(x*x + y*y + z*z + w*w);
		
	}
	// Нормализация
	inline void Normalize()
	{
		float norm = Norma();
		x/= norm;
		y/= norm;
		z/= norm;
		w/= norm;
		
	}

float x,y,z,w;

};

*/

#endif
