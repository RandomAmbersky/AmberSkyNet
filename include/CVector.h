#ifndef __CVECTOR_H__
#define __CVECTOR_H__

//***********************************************************//
// Demo:    PickObject Demo
// Author:  terror
//***********************************************************//
//#pragma once
#include "ASN_Types.h"
#include <math.h>

#define DEG2RAD(X)		(PI*(X)/180.0f)
typedef unsigned int   uint;

struct CVector
{
	CVector() {v[0] = v[1] = v[2] = 0;}
	CVector(float px, float py, float pz) {v[0] = px; v[1] = py; v[2] = pz;}
	CVector(const CVector &pVec) {v[0] = pVec.v[0]; v[1] = pVec.v[1]; v[2] = pVec.v[2];}
	CVector(const float *pVec)  {v[0] = pVec[0]; v[1] = pVec[1]; v[2] = pVec[2];}

	CVector operator=(const CVector &pVec)
		{return CVector(v[0] = pVec.v[0], v[1] = pVec.v[1], v[2] = pVec.v[2]);}
	CVector operator=(const float *ptr)
		{return CVector(v[0] = ptr[0], v[1] = ptr[1], v[2] = ptr[2]);}
	bool operator==(const CVector &pVec) const
		{return (v[0] == pVec.v[0] && v[1] == pVec.v[1] && v[2] == pVec.v[2]);}
	bool operator==(const float *pVec) const
		{return (v[0] == pVec[0] && v[1] == pVec[1] && v[2] == pVec[2]);}
	inline bool operator!=(const CVector &pVec) const
		{return !((*this) == pVec);}
	inline bool operator!=(const float *pVec)
		{return !(pVec == (*this));}
	bool operator<(const CVector vec) const
		{return ((v[0] < vec[0]) && (v[1] < vec[1]) && (v[2] < vec[2]));}
	bool operator<=(const CVector vec) const
		{return ((v[0] <= vec[0]) && (v[1] <= vec[1]) && (v[2] <= vec[2]));}
	bool operator>(const CVector vec) const
		{return ((v[0] > vec[0]) && (v[1] > vec[1]) && (v[2] > vec[2]));}
	bool operator>=(const CVector vec) const
		{return ((v[0] >= vec[0]) && (v[1] >= vec[1]) && (v[2] >= vec[2]));}

	const float &operator[](int ndx) const {return v[ndx];}
	float &operator[](int ndx)             {return v[ndx];}
	operator float*(void)                  {return v;}

	CVector &operator+=(const CVector &V) {
        v[0] += V.v[0]; v[1] += V.v[1]; v[2] += V.v[2];
//*this = *this + pVec;
	return *this;
	}
	CVector &operator-=(const CVector &pVec) {*this = *this - pVec; return *this;}
	CVector &operator*=(const CVector &pVec) {*this = *this * pVec; return *this;}
	CVector &operator*=(float val)          {*this = *this * val; return *this;}
	CVector &operator/=(const CVector &pVec) {*this = *this / pVec; return *this;}
	CVector &operator/=(float val)          {*this = *this / val; return *this;}

	CVector operator+(const CVector &pVec) const
		{return CVector(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);}
	CVector operator-(const CVector &pVec) const
		{return CVector(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);}
	CVector operator*(const CVector &pVec) const
		{return CVector(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);}
	CVector operator*(float val) const
		{return CVector(v[0] * val, v[1] * val, v[2] * val);}
	friend CVector operator*(float val, const CVector &v)
		{return CVector(v[0] * val, v[1] * val, v[2] * val);}
	CVector operator/(const CVector &pVec) const
		{return CVector(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);}
	CVector operator/(float val) const
		{return CVector(v[0] / val, v[1] / val, v[2] / val);}
	CVector operator-(void) const {return CVector(-v[0], -v[1], -v[2]);}

	CVector operator^ ( const CVector& vect ) const
	{
		CVector res;
		res.v[0] = (vect.v[1] * v[2]) - (vect.v[2] * v[1]);
		res.v[1] = (vect.v[2] * v[0]) - (vect.v[0] * v[2]);
		res.v[2] = (vect.v[0] * v[1]) - (vect.v[1] * v[0]);

		return res;
	}

	float operator | ( const CVector &vect ) const
	{ return v[0] * vect.v[0] + v[1] * vect.v[1] + v[2] * vect.v[2]; }

   void Clear(void)                    {v[0] = v[1] = v[2] = 0;}
   void Set(float x, float y, float z) {v[0] = x; v[1] = y; v[2] = z;}
   void Set(const CVector &p)           {v[0] = p[0]; v[1] = p[1]; v[2] = p[2];}
   void Add(const CVector &a, const CVector &b)
    {v[0] = a.v[0] + b.v[0]; v[1] = a.v[1] + b.v[1]; v[2] = a.v[2] + b.v[2];}
   void Add(const CVector &a)
    {v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2];}
   void Subtract(const CVector &a, const CVector &b)
    {v[0] = a.v[0] - b.v[0]; v[1] = a.v[1] - b.v[1]; v[2] = a.v[2] - b.v[2];}
   void Subtract(const CVector &a)
    {v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2];}
   void Multiply(const CVector &a, const CVector &b)
    {v[0] = a.v[0] * b.v[0]; v[1] = a.v[1] * b.v[1]; v[2] = a.v[2] * b.v[2];}
   void Multiply(const CVector &a)
    {v[0] *= a.v[0]; v[1] *= a.v[1]; v[2] *= a.v[2];}
   void Divide(const CVector &a, const CVector &b)
    {v[0] = a.v[0] / b.v[0]; v[1] = a.v[1] / b.v[1]; v[2] = a.v[2] / b.v[2];}
   void Divide(const CVector &a)
    {v[0] /= a.v[0]; v[1] /= a.v[1]; v[2] /= a.v[2];}
   void Scale(float val)
    {v[0] *= val; v[1] *= val; v[2] *= val;}
   void Fabs(const CVector &src) {v[0] = (float) fabs(src.v[0]);
    v[1] = (float) fabs(src.v[1]); v[2] = (float) fabs(src.v[2]);}
   void Fabs(void) {v[0] = (float) fabs(v[0]); v[1] = (float) fabs(v[1]);
    v[2] = (float) fabs(v[2]);}

	void Normalize(void);
	void GetNormal(const CVector &pVec1, const CVector &pVec2, const CVector &pVec3);
	void GetPlaneIntersect(const CVector &Start, const CVector &End, const CVector &Position, const CVector &Normal);

   float Length(void);
   float Dot(const CVector &pVec)
    {return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];}
   void Cross(const CVector &p, const CVector &q);
   float GetDistance(const CVector &dest);
	 void Set ( float val )
	 { v[0] = val; v[1] = val; v[2] = val; }

	void FindDirection ( CVector &src, CVector &dest )
	{
		v[0] = dest.v[0] - src.v[0];
		v[1] = dest.v[1] - src.v[1];
		v[2] = dest.v[2] - src.v[2];
		Normalize();
	}

//	void RotateX ( float angle );
//	void RotateZ ( float angle );
//	void RotateY ( float angle );

	inline void Min(const CVector &srv){
	if (v[0]>srv.v[0]) v[0]=srv.v[0];
	if (v[1]>srv.v[1]) v[1]=srv.v[1];
	if (v[2]>srv.v[2]) v[2]=srv.v[2];
	};

	inline void Max(const CVector &srv){
	if (v[0]<srv.v[0]) v[0]=srv.v[0];
	if (v[1]<srv.v[1]) v[1]=srv.v[1];
	if (v[2]<srv.v[2]) v[2]=srv.v[2];
	};

	inline void RotateX(float angle)
	{
		float oy=v[1],oz=v[2];
		angle=DEG2RAD(angle);
		v[1]=(float)((oy * cos(angle) )+(oz * (-sin(angle) ) ));
		v[2]=(float)((oy * sin(angle) )+(oz * (cos(angle) ) ));
	}
	// Вращает вектор вокруг оси OY
	inline void RotateY(float angle)
	{
		float ox=v[0],oz=v[2];
		angle=DEG2RAD(angle);
		v[0]=(float)((ox * cos(angle) )+(oz * (-sin(angle) ) ));
		v[2]=(float)((ox * sin(angle) )+(oz * (cos(angle) ) ));
	}
	// Вращает вектор вокруг оси OZ
	inline void RotateZ(float angle)
	{
		float ox=v[0],oy=v[1];
		angle=DEG2RAD(angle);
		v[0]=(float)((ox * cos(angle) )+(oy * (-sin(angle) ) ));
		v[1]=(float)((ox * sin(angle) )+(oy * (cos(angle) ) ));
	}

public:
   float v[3];
};


inline void CVector::Normalize(void)
{
	float length, len = 0;

	length = Length();

	if (length == 0)
		return;

	len = 1.0f / length;

	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}

inline void CVector::GetNormal(const CVector &pVec1, const CVector &pVec2, const CVector &pVec3){

    CVector Vector1, Vector2;
    Vector1.Subtract(pVec1, pVec3);
    Vector2.Subtract(pVec3, pVec2);
    Cross(Vector1, Vector2);
    Normalize();
}

inline void CVector::GetPlaneIntersect(const CVector &Start, const CVector &End, const CVector &Position, const CVector &Normal){

    CVector CA=Start-Position;
    CVector CV=Start-End;

    double CN = CA | Normal;
    double CM = CV | Normal;
    float k= CN / CM;
    (*this) = CV * k;
    (*this) = -( (*this)-Start);

}

inline float CVector::Length(void)
{
//float out;
double length = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
return sqrt( length );

/*

	__asm__ (
        "fld %1;"
        "fmul %%st,%%st;"
        "fld %2;"
        "fmul %%st,%%st;"
        "fld %3;"
        "fmul %%st,%%st;"
        "faddp;"
        "faddp;"
        "fsqrt;"
        "fstp %0;"
    : "=m" (out)
    : "m" (v[0]), "m" (v[1]), "m" (v[2])
    : "%st","%ebx","%edi"
/

);

	return out;
*/

}

inline void CVector::Cross(const CVector &p, const CVector &q)
{
	v[0] = (p.v[1] * q.v[2]) - (p.v[2] * q.v[1]);
	v[1] = (p.v[2] * q.v[0]) - (p.v[0] * q.v[2]);
	v[2] = (p.v[0] * q.v[1]) - (p.v[1] * q.v[0]);
}

inline float CVector::GetDistance(const CVector &dest)
{
	float d1 = dest[0] - v[0];
	float d2 = dest[1] - v[1];
	float d3 = dest[2] - v[2];

	return sqrt((d1 * d1) + (d2 * d2) + (d3 * d3));
}


struct CVector4
{
   CVector4() {v[0] = v[1] = v[2] = v[3]= 0;}
   CVector4(float px, float py, float pz, float pa) {v[0] = px; v[1] = py; v[2] = pz; v[3]=pa;}
   CVector4(const CVector4 &pVec)
    {v[0] = pVec.v[0]; v[1] = pVec.v[1]; v[2] = pVec.v[2]; v[3] = pVec.v[3];}
   CVector4(const float *pVec) {v[0] = pVec[0]; v[1] = pVec[1]; v[2] = pVec[2]; v[3] = pVec[3];}
   CVector4 operator=(const CVector4 &pVec)
    {return CVector4(v[0] = pVec.v[0], v[1] = pVec.v[1], v[2] = pVec.v[2], v[3]=pVec.v[3]);}
   CVector4 operator=(const float *ptr)
    {return CVector4(v[0] = ptr[0], v[1] = ptr[1], v[2] = ptr[2], v[3] = ptr[3]);}
   bool operator==(const CVector4 &pVec) const
    {return (v[0] == pVec.v[0] && v[1] == pVec.v[1] && v[2] == pVec.v[2] && v[3] == pVec.v[3]);}
   bool operator==(const float *pVec) const
    {return (v[0] == pVec[0] && v[1] == pVec[1] && v[2] == pVec[2] && v[3] == pVec[3]);}
   inline bool operator!=(const CVector4 &pVec) const
    {return !((*this) == pVec);}
   inline bool operator!=(const float *pVec)
    {return !(pVec == (*this));}
   bool operator<(const CVector4 vec) const
    {return ((v[0] < vec[0]) && (v[1] < vec[1]) && (v[2] < vec[2])  && (v[3] < vec[3]) );}
   bool operator<=(const CVector4 vec) const
    {return ((v[0] <= vec[0]) && (v[1] <= vec[1]) && (v[2] <= vec[2])  && (v[3] <= vec[3]));}
   bool operator>(const CVector4 vec) const
    {return ((v[0] > vec[0]) && (v[1] > vec[1]) && (v[2] > vec[2]) && (v[3] > vec[3]));}
   bool operator>=(const CVector4 vec) const
    {return ((v[0] >= vec[0]) && (v[1] >= vec[1]) && (v[2] >= vec[2]) && (v[3] >= vec[3]) );}

   const float &operator[](int ndx) const {return v[ndx];}
   float &operator[](int ndx)             {return v[ndx];}
   operator float*(void)                  {return v;}

public:
   float v[4];
};

struct CVector2
{
   CVector2() {v[0] = v[1] = 0;}
   CVector2(float px, float py) {v[0] = px; v[1] = py;}
   CVector2(const CVector2 &pVec)
    {v[0] = pVec.v[0]; v[1] = pVec.v[1];}
   CVector2(const float *pVec) {v[0] = pVec[0]; v[1] = pVec[1];}

   CVector2 operator=(const CVector2 &pVec)
    {return CVector2(v[0] = pVec.v[0], v[1] = pVec.v[1]);}
   CVector2 operator=(const float *ptr)
    {return CVector2(v[0] = ptr[0], v[1] = ptr[1]);}
   bool operator==(const CVector2 &pVec) const
    {return (v[0] == pVec.v[0] && v[1] == pVec.v[1]);}
   bool operator==(const float *pVec) const
    {return (v[0] == pVec[0] && v[1] == pVec[1]);}
   inline bool operator!=(const CVector2 &pVec) const
    {return !((*this) == pVec);}
   inline bool operator!=(const float *pVec)
    {return !(pVec == (*this));}
   bool operator<(const CVector2 vec) const
    {return ((v[0] < vec[0]) && (v[1] < vec[1]));}
   bool operator<=(const CVector2 vec) const
    {return ((v[0] <= vec[0]) && (v[1] <= vec[1]));}
   bool operator>(const CVector2 vec) const
    {return ((v[0] > vec[0]) && (v[1] > vec[1]));}
   bool operator>=(const CVector2 vec) const
    {return ((v[0] >= vec[0]) && (v[1] >= vec[1]));}

   const float &operator[](int ndx) const {return v[ndx];}
   float &operator[](int ndx)             {return v[ndx];}
   operator float*(void)                  {return v;}
//   operator const float*() const {return &v;}

   CVector2 &operator+=(const CVector2 &pVec) {*this = *this + pVec; return *this;}
   CVector2 &operator-=(const CVector2 &pVec) {*this = *this - pVec; return *this;}
   CVector2 &operator*=(const CVector2 &pVec) {*this = *this * pVec; return *this;}
   CVector2 &operator*=(float val)          {*this = *this * val; return *this;}
   CVector2 &operator/=(const CVector2 &pVec) {*this = *this / pVec; return *this;}
   CVector2 &operator/=(float val)          {*this = *this / val; return *this;}

   CVector2 operator+(const CVector2 &pVec) const
    {return CVector2(v[0] + pVec.v[0], v[1] + pVec.v[1]);}
   CVector2 operator-(const CVector2 &pVec) const
    {return CVector2(v[0] - pVec.v[0], v[1] - pVec.v[1]);}
   CVector2 operator*(const CVector2 &pVec) const
    {return CVector2(v[0] * pVec.v[0], v[1] * pVec.v[1]);}
   CVector2 operator*(float val) const
    {return CVector2(v[0] * val, v[1] * val);}
   friend CVector2 operator*(float val, const CVector2 &v)
    {return CVector2(v[0] * val, v[1] * val);}
   CVector2 operator/(const CVector2 &pVec) const
    {return CVector2(v[0] / pVec.v[0], v[1] / pVec.v[1]);}
   CVector2 operator/(float val) const
    {return CVector2(v[0] / val, v[1] / val);}
   CVector2 operator-(void) const {return CVector2(-v[0], -v[1]);}


   void Clear(void)                    {v[0] = v[1] = 0;}
   void Set(float x, float y) {v[0] = x; v[1] = y;}
   void Set(const CVector2 &p)           {v[0] = p[0]; v[1] = p[1];}
   void Add(const CVector2 &a, const CVector2 &b)
    {v[0] = a.v[0] + b.v[0]; v[1] = a.v[1] + b.v[1];}
   void Add(const CVector2 &a)
    {v[0] += a.v[0]; v[1] += a.v[1];}
   void Subtract(const CVector2 &a, const CVector2 &b)
    {v[0] = a.v[0] - b.v[0]; v[1] = a.v[1] - b.v[1];}
   void Subtract(const CVector2 &a)
    {v[0] -= a.v[0]; v[1] -= a.v[1];}
   void Multiply(const CVector2 &a, const CVector2 &b)
    {v[0] = a.v[0] * b.v[0]; v[1] = a.v[1] * b.v[1];}
   void Multiply(const CVector2 &a)
    {v[0] *= a.v[0]; v[1] *= a.v[1];}
   void Divide(const CVector2 &a, const CVector2 &b)
    {v[0] = a.v[0] / b.v[0]; v[1] = a.v[1] / b.v[1];}
   void Divide(const CVector2 &a)
    {v[0] /= a.v[0]; v[1] /= a.v[1];}
   void Scale(float val)
    {v[0] *= val; v[1] *= val;}
   void Fabs(const CVector2 &src) {v[0] = (float) fabs(src.v[0]);
    v[1] = (float) fabs(src.v[1]);}
   void Fabs(void) {v[0] = (float) fabs(v[0]); v[1] = (float) fabs(v[1]);}

public:
   float v[2];
};

#endif
