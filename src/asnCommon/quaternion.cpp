
#include "ASN_Types.h"
#include "quaternion.h"
#include "math.h"

Quaternion::Quaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

Quaternion::~Quaternion()
{

}

void Quaternion::CreateFromAxisAngle(const CVector& Axis, float degrees)
{
	// First we want to convert the degrees to radians 
	// since the angle is assumed to be in radians
	float angle = degrees / 180.0f * M_PI;

	// Here we calculate the sin( theta / 2) once for optimization
	float result = sinf( angle / 2.0f );
		
	// Calcualte the w value by cos( theta / 2 )
	w = cos( angle / 2.0f );

	// Calculate the x, y and z of the quaternion
	x = Axis.v[0] * result;
	y = Axis.v[1] * result;
	z = Axis.v[2] * result;
}

void Quaternion::CreateMatrix(double *pMatrix)
{
	// Make sure the matrix has allocated memory to store the rotation data
	if(!pMatrix) return;

	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z ); 
	pMatrix[ 1] = 2.0f * (x * y + z * w);
	pMatrix[ 2] = 2.0f * (x * z - y * w);
	pMatrix[ 3] = 0.0f;  

	// Second row
	pMatrix[ 4] = 2.0f * ( x * y - z * w );  
	pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z ); 
	pMatrix[ 6] = 2.0f * (z * y + x * w );  
	pMatrix[ 7] = 0.0f;  

	// Third row
	pMatrix[ 8] = 2.0f * ( x * z + y * w );
	pMatrix[ 9] = 2.0f * ( y * z - x * w );
	pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );  
	pMatrix[11] = 0.0f;  

	// Fourth row
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;

	// Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}

Quaternion Quaternion::operator *(Quaternion q)
{
	Quaternion r;

	r.w = w*q.w - x*q.x - y*q.y - z*q.z;
	r.x = w*q.x + x*q.w + y*q.z - z*q.y;
	r.y = w*q.y + y*q.w + z*q.x - x*q.z;
	r.z = w*q.z + z*q.w + x*q.y - y*q.x;
	
	return(r);
}

