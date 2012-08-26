//matrix.cpp

#include "CVector.h"
#include "matrix.h"
#include "math.h"

Matrix::Matrix()
{

LoadIdentity();

}


Matrix::~Matrix()
{


}

/*
void Matrix::MakeSquish(Point3D * point, Facet3D * facet)
{
	facet->MakeDCoefficient();

	facet->m_d += 1.0;

	float dot = facet->m_flatnormal.m_x * point->m_x +
				facet->m_flatnormal.m_y * point->m_y +
				facet->m_flatnormal.m_z * point->m_z +
				facet->m_d;

	m_data[0]= dot - facet->m_flatnormal.m_x * point->m_x;
	m_data[4]=  0  - facet->m_flatnormal.m_y * point->m_x;
	m_data[8]=  0  - facet->m_flatnormal.m_z * point->m_x;
	m_data[12]=  0  - facet->m_d * point->m_x;

	m_data[1]=  0  - facet->m_flatnormal.m_x * point->m_y;
	m_data[5]= dot - facet->m_flatnormal.m_y * point->m_y;
	m_data[9]=  0  - facet->m_flatnormal.m_z * point->m_y;
	m_data[13]=  0  - facet->m_d * point->m_y;

	m_data[2]=   0  - facet->m_flatnormal.m_x * point->m_z;
	m_data[6]=   0  - facet->m_flatnormal.m_y * point->m_z;
	m_data[10]= dot - facet->m_flatnormal.m_z * point->m_z;
	m_data[14]=  0  - facet->m_d * point->m_z;

	m_data[3]=   0 - facet->m_flatnormal.m_x;
	m_data[7]=   0 - facet->m_flatnormal.m_y;
	m_data[11]=  0  - facet->m_flatnormal.m_z;
	m_data[15]= dot - facet->m_d;

	facet->m_d -= 1.0;

}
*/

/*
void Matrix::MakeFromVectorsRow(Point3D * a,Point3D * b, Point3D * c,Point3D * d)
{
	m_data[0]=a->m_x; m_data[1]= a->m_y; m_data[2]=a->m_z;  m_data [3] = 0;
	m_data[4]=b->m_x; m_data[5]= b->m_y; m_data[6]=b->m_z;  m_data [7] = 0;
	m_data[8]=c->m_x; m_data[9]= c->m_y; m_data[10]=c->m_z; m_data [11] = 0;
	m_data[12]=d->m_x;m_data[13]= d->m_y; m_data[14]= d->m_z; m_data [15] = 1;




}
*/

/*
void Matrix::MakeFromVectorsColumn(Point3D * a,Point3D * b, Point3D * c, Point3D * d)
{
	m_data[0]=a->m_x; m_data[4]= a->m_y; m_data[8]=a->m_z;  m_data [12] = d->m_x;
	m_data[1]=b->m_x; m_data[5]= b->m_y; m_data[9]=b->m_z;  m_data [13] = d->m_y;
	m_data[2]=c->m_x; m_data[6]= c->m_y; m_data[10]=c->m_z; m_data [14] = d->m_z;
	m_data[3]=0; m_data[7]= 0; m_data[11]=0; m_data [15] = 1;


}
*/

void Matrix::operator *=(Matrix & matrix)
{
	Matrix temp;

	for (int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			temp.m_data[i+j*4] = (m_data[i] * matrix.m_data[j*4]) +
								(m_data[i+4] * matrix.m_data[1+j*4]) +
								(m_data[i+8] * matrix.m_data[2+j*4]) +
								(m_data[i+12] * matrix.m_data[3+j*4]);
		}
	}

	*this = temp;
	/*
	m_data[3] = 0;
	m_data[7] = 0;
	m_data[11] = 0;
	m_data[15] = 1;
	*/
}

void Matrix::MakePitch(double a)
{
	double rad = (double)(3.141592654*(double)a/(double)180);
	m_data[0]=1; m_data[4]= 0;      m_data[8]=0;          m_data [12] = 0;
	m_data[1]=0; m_data[5]= (double)cos(rad); m_data[9]=(double)sin(rad);    m_data [13] = 0;
	m_data[2]=0; m_data[6]= (double)-sin(rad);m_data[10]=(double)cos(rad); m_data [14] = 0;
	m_data[3]=0;m_data[7]= 0;     m_data[11]= 0;        m_data [15] = 1;


}
void Matrix::MakeRoll(double a)
{
	double rad = (double)(3.141592654*(double)a/(double)180);
	m_data[0]=(double)cos(rad); m_data[4]= (double)sin(rad);   m_data[8]=0; m_data [12] = 0;
	m_data[1]=(double)-sin(rad);   m_data[5]= (double)cos(rad); m_data[9]=0; m_data [13] = 0;
	m_data[2]=0;         m_data[6]= 0;         m_data[10]=1; m_data [14] = 0;
	m_data[3]=0;        m_data[7]= 0;        m_data[11]=0; m_data [15] = 1;
}

void Matrix::MakeYaw(double a)
{
	double rad = (double)(3.141592654*(double)a/(double)180);
	m_data[0]=(double)cos(rad); m_data[4]= 0; m_data[8]=(double)-sin(rad);  m_data [12] = 0;
	m_data[1]=0;         m_data[5]= 1; m_data[9]= 0;        m_data [13] = 0;
	m_data[2]=(double)sin(rad);   m_data[6]= 0; m_data[10]=(double)cos(rad); m_data [14] = 0;
	m_data[3]=0;        m_data[7]= 0; m_data[11]= 0;	  m_data [15] = 1;
}

/*
void Matrix::SetRotationAngles(float x, float y, float z)
{
	x = DtoR(x);
	x = DtoR(x);
	y = DtoR(y);
	z = DtoR(z);

	double cr = cos( x );
	double sr = sin( x );
	double cp = cos( y );
	double sp = sin( y );
	double cy = cos( y );
	double sy = sin( y );

	Matrix temp;

	temp.LoadIdentity();

	m_data[0] = ( double )( cp*cy );
	m_data[1] = ( double )( cp*sy );
	m_data[2] = ( double )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	m_data[4] = ( double )( srsp*cy-cr*sy );
	m_data[5] = ( double )( srsp*sy+cr*cy );
	m_data[6] = ( double )( sr*cp );

	m_data[8] = ( double )( crsp*cy+sr*sy );
	m_data[9] = ( double )( crsp*sy-sr*cy );
	m_data[10] = ( double )( cr*cp );


}
*/
/*
void Matrix::SetTranslation(float x, float y, float z)
{

	m_data[12]=x; m_data[13]= y; m_data[14]=z;


}*/

/*
void Matrix::InverseRotate( Point3D * point )
{
	Point3D temp;

	temp.m_x = point->m_x*m_data[0]+point->m_y*m_data[1]+point->m_z*m_data[2];
	temp.m_y = point->m_x*m_data[4]+point->m_y*m_data[5]+point->m_z*m_data[6];
	temp.m_z = point->m_x*m_data[8]+point->m_y*m_data[9]+point->m_z*m_data[10];

	*point = temp;
}

void Matrix::InverseTranslate( Point3D * point )
{
	point->m_x -= m_data[12];
	point->m_y -= m_data[13];
	point->m_z -= m_data[14];
}
*/

//void Matrix::Rotate(float x, float y, float z)
//{
/*
	Matrix temp;

	float cx = cosf(pitch);
	float sx = sinf(pitch);
	float cy = cosf(yaw);
	float sy = sinf(yaw);
	float cz = cosf(roll);
	float sz = sinf(roll);

	temp.m_data[0] =  cy*cz;
	temp.m_data[1] = -sz*cy;
	temp.m_data[2] =  sy;
	temp.m_data[3] =  0.0f;

	tempM10 =  sx*sy*cz + sz*cx;
	tempM11 = -sx*sy*sz + cx*cz;
	tempM12 = -sx*cy;
	tempM13 =  0.0f;

	tempM20 = -sy*cx*cz + sx*sz;
	tempM21 =  sz*sy*cx + cz*sx;
	tempM22 =  cx*cy;
	tempM23 =  0.0f;

	tempM30 =  0.0f;
	tempM31 =  0.0f;
	tempM32 =  0.0f;
	tempM33 =  1.0f;
*/
//}

/*
void Matrix::Rotate(float x, float y, float z)
{

	//x = DtoR(x);
	//y = DtoR(y);
	//z = DtoR(z);

	Matrix rot[3];

	rot[0].MakeQuaternionAA(x, 1,0,0);//,  1);
	rot[1].MakeQuaternionAA(y, 0,1,0);//,  1);
	rot[2].MakeQuaternionAA(z, 0,0,1);//,  1);


	for(int i=0; i<3; i++)
		*this *= rot[i];

}
*/

void Matrix::MakeQuaternionAA(double a, double x, double y, double z/*, bool radians*/)
{
	double rad = a;

//	if(radians == 0)
//		rad = (double)(3.141592654*(double)a/(double)180);

	double sin_a = (double)sin( rad / 2 );
	double cos_a = (double)cos( rad / 2 );

    double qx = x * sin_a;
	double qy = y * sin_a;
    double qz = z * sin_a;
    double qw = cos_a;

//	CVector quat(x,y,z);
	double mag = (double)sqrt(qw*qw + qx*qx + qy*qy + qz*qz) ;
	qx /= mag;
	qy /= mag;
	qz /= mag;
	qw /= mag;

	double xx      = qx * qx;
    double xy      = qx * qy;
	double xz      = qx * qz;
	double xw      = qx * qw;
    double yy      = qy * qy;
	double yz      = qy * qz;
	double yw      = qy * qw;
	double zz      = qz * qz;
    double zw      = qz * qw;

	m_data[0]=1-2*(yy-zz); m_data[4]= 2*(xy-zw);   m_data[8]=2*(xz+yw);    m_data [12] = 0;
	m_data[1]=2*(xy+zw);   m_data[5]= 1-2*(xx-zz); m_data[9]=2*(yz-xw);    m_data [13] = 0;
	m_data[2]=2*(xz-yw);   m_data[6]= 2*(yz+xw);   m_data[10]=1-2*(xx-yy); m_data [14] = 0;
	m_data[3]=0;           m_data[7]= 0;           m_data[11]=0;	        m_data [15] = 1;


}

void Matrix::LoadIdentity()
{

	m_data[0]=1; m_data[4]=0; m_data[8]=0; m_data[12]=0;
    m_data[1]=0; m_data[5]=1; m_data[9]=0; m_data[13]=0;
    m_data[2]=0; m_data[6]=0; m_data[10]=1; m_data[14]=0;
    m_data[3]=0; m_data[7]=0; m_data[11]=0; m_data[15]=1;

}


void Matrix::LookAt(const CVector& pos, const CVector& target, const CVector& up ){

// gluLookAt emulated 8) R.Ambersky

/*
    CVector a_eye=pos;
    CVector a_at=target;
    CVector a_up= up;

  CVector look = a_at - a_eye;
  look.Normalize();
  a_up.Normalize();

  CVector xaxis;
  xaxis.Cross(look,a_up);
  xaxis.Normalize();

  CVector upv;
  upv.Cross(xaxis,look);
  upv.Normalize();

  m_data[0]  = xaxis.v[0]; m_data[4]  = xaxis.v[1]; m_data[8]  = xaxis.v[2]; m_data[12] = 0.f;
  m_data[1]  = upv.v[0];   m_data[5]  = upv.v[1];   m_data[9]  = upv.v[2];   m_data[13] = 0.f;
  m_data[2]  = -look.v[0]; m_data[6]  = -look.v[1]; m_data[10] = -look.v[2]; m_data[14] = 0.f;
  m_data[3] = 0;           m_data[7] =0;            m_data[11] = 0;          m_data[15] = 1.0f;

  Translate( -a_eye.v[0], -a_eye.v[1], -a_eye.v[2]);
*/

 CVector z = pos - target;
 z.Normalize();
 CVector y = up;
 CVector x;
 x.Cross(y, z);
 x.Normalize();
 y.Cross(z, x);
 y.Normalize();

	m_data[0] = x.v[0];	 m_data[4] = x.v[1];  m_data[8]  = x.v[2];  m_data[12] = -x.v[0] * pos.v[0] - x.v[1] * pos.v[1] - x.v[2] * pos.v[2];
	m_data[1] = y.v[0];	 m_data[5] = y.v[1];  m_data[9]  = y.v[2];  m_data[13] = -y.v[0] * pos.v[0] - y.v[1] * pos.v[1] - y.v[2] * pos.v[2];
	m_data[2] = z.v[0];	 m_data[6] = z.v[1];  m_data[10] = z.v[2];  m_data[14] = -z.v[0] * pos.v[0] - z.v[1] * pos.v[1] - z.v[2] * pos.v[2];
	m_data[3] = 0.0f; m_data[7] = 0.0f; m_data[11] = 0.0f; m_data[15] = 1.0f;


}

void Matrix::Perspective(float fov, float aspect, float near, float far){

//	Matrix temp;

//    LoadIdentity();

		double y = tan(fov * PI / 360.0f);
		double x = y * aspect;
		m_data[0] = 1.0f / x; m_data[4] = 0.0; m_data[8] = 0.0; m_data[12] = 0.0;
		m_data[1] = 0.0; m_data[5] = 1.0f / y; m_data[9] = 0.0; m_data[13] = 0.0;
		m_data[2] = 0.0; m_data[6] = 0.0; m_data[10] = -(far + near) / (far - near); m_data[14] = -(2.0f * far * near) / (far - near);
		m_data[3] = 0.0; m_data[7] = 0.0; m_data[11] = -1.0; m_data[15] = 0.0;

/*
    double sine, cotangent, delta_z;
	double radians = (fov / 2.0f) * (PI/180.0f);

    delta_z = far - near;
    sine = sinf(radians);

    if((delta_z == 0) || (sine == 0) || (aspect == 0)) {
        LoadIdentity();
        return;
    }

    cotangent = cosf(radians) / sine;

    m_data[0] = cotangent / aspect;
    m_data[5] = cotangent;
    m_data[10] = -(far + near) / delta_z;
    m_data[11] = -1;
    m_data[14] = -2 * near * far / delta_z;
    m_data[15] = 0;

//	return pMat;
*/

}

void Matrix::Translate(float x, float y, float z)
{
	Matrix temp;

	temp.m_data[0]=1; temp.m_data[4]=0; temp.m_data[8]=0; temp.m_data[12]=x;
    temp.m_data[1]=0; temp.m_data[5]=1; temp.m_data[9]=0; temp.m_data[13]=y;
    temp.m_data[2]=0; temp.m_data[6]=0; temp.m_data[10]=1; temp.m_data[14]=z;
    temp.m_data[3]=0; temp.m_data[7]=0; temp.m_data[11]=0; temp.m_data[15]=1;

	*this *= temp;

}

void Matrix::Scale (const CVector& size) {

    Matrix temp;

		temp.m_data[0] = size.v[0]; temp.m_data[4] = 0.0; temp.m_data[8] = 0.0; temp.m_data[12] = 0.0;
		temp.m_data[1] = 0.0; temp.m_data[5] = size.v[1]; temp.m_data[9] = 0.0; temp.m_data[13] = 0.0;
		temp.m_data[2] = 0.0; temp.m_data[6] = 0.0; temp.m_data[10] = size.v[2]; temp.m_data[14] = 0.0;
		temp.m_data[3] = 0.0; temp.m_data[7] = 0.0; temp.m_data[11] = 0.0; temp.m_data[15] = 1.0;
	*this *= temp;
}

void Matrix::RotateXYZ(const CVector& _Rot){

float pitch=_Rot.v[0]*M_PI/180.0f;
float yaw=_Rot.v[1]*M_PI/180.0f;
float roll=_Rot.v[2]*M_PI/180.0f;

/*
//    Matrix temp;

	float cx = cosf(pitch);
	float sx = sinf(pitch);
	float cy = cosf(yaw);
	float sy = sinf(yaw);
	float cz = cosf(roll);
	float sz = sinf(roll);

	m_data[0] =  cy*cz;
	m_data[1] = -sz*cy;
	m_data[2] =  sy;
	m_data[3] =  0.0f;

	m_data[4] =  sx*sy*cz + sz*cx;
	m_data[5] = -sx*sy*sz + cx*cz;
	m_data[6] = -sx*cy;
	m_data[7] =  0.0f;

	m_data[8] = -sy*cx*cz + sx*sz;
	m_data[9] =  sz*sy*cx + cz*sx;
	m_data[10] =  cx*cy;
	m_data[11] =  0.0f;

	m_data[12] =  0.0f;
	m_data[13] =  0.0f;
	m_data[14] =  0.0f;
	m_data[15] =  1.0f;

//	this = temp;
//	*this *= temp;
*/

//	float	s_rad_x, c_rad_x, s_rad_y, c_rad_y, s_rad_z, c_rad_z;
//	sw_math::f_sin_cos(ang_x, s_rad_x, c_rad_x);
//	sw_math::f_sin_cos(ang_y, s_rad_y, c_rad_y);
//	sw_math::f_sin_cos(ang_z, s_rad_z, c_rad_z);

	float c_rad_x = cosf(pitch);
	float s_rad_x = sinf(pitch);
	float c_rad_y = cosf(yaw);
	float s_rad_y = sinf(yaw);
	float c_rad_z = cosf(roll);
	float s_rad_z = sinf(roll);

	m_data[0]  =  c_rad_y * c_rad_z;
	m_data[1]  = -s_rad_x * -s_rad_y * c_rad_z + c_rad_x * s_rad_z;
	m_data[2]  =  c_rad_x * -s_rad_y * c_rad_z + s_rad_x * s_rad_z;
	m_data[3]  =  0.0f;

	m_data[4]  =  c_rad_y * -s_rad_z;
	m_data[5]  = -s_rad_x * -s_rad_y * -s_rad_z + c_rad_x * c_rad_z;
	m_data[6]  =  c_rad_x * -s_rad_y * -s_rad_z + s_rad_x * c_rad_z;
	m_data[7]  =  0.0f;

	m_data[8]  =  s_rad_y;
	m_data[9]  = -s_rad_x * c_rad_y;
	m_data[10] =  c_rad_x * c_rad_y;
	m_data[11] =  0.0f;

	m_data[12] =  0.0f;
	m_data[13] =  0.0f;
	m_data[14] =  0.0f;
	m_data[15] =  1.0f;

}



CVector Matrix::GetRot(){

	float	tr_x, tr_y, c;
	CVector	vec_ang;

	vec_ang.v[1] = asinf(m_data[8]);
	c		  = cosf(vec_ang.v[1]);

	if(fabs(c) > 1e-8f)	// is Gimball lock?
	{
		tr_x = m_data[10] / c;
		tr_y = -m_data[9]  / c;

		vec_ang.v[0] = atan2f(tr_y, tr_x);

		tr_x = m_data[0] / c;
		tr_y = -m_data[4] / c;

		vec_ang.v[2] = atan2f(tr_y, tr_x);
	}else
	{
		vec_ang.v[0] = 0.0f;

		tr_x =  m_data[5];
		tr_y = m_data[1];

		vec_ang.v[2] = atan2f(tr_y, tr_x);
	}

	return vec_ang;

}

//////////////////////////////////////////////
//Brett Porter stuff
//////////////////////////////////////////////

/*
void Matrix::InverseRotateVect( Point3D * point )
{

	Point3D temp;

	temp.m_x = point->m_x*m_data[0]+point->m_y*m_data[1]+point->m_z*m_data[2];
	temp.m_y = point->m_x*m_data[4]+point->m_y*m_data[5]+point->m_z*m_data[6];
	temp.m_z = point->m_x*m_data[8]+point->m_y*m_data[9]+point->m_z*m_data[10];


	*point = temp;
}

void Matrix::InverseTranslateVect( Point3D * point )
{
	point->m_x = point->m_x - m_data[12];
	point->m_y = point->m_y - m_data[13];
	point->m_z = point->m_z - m_data[14];
}
*/

/*
void Matrix::SetTranslation(float x, float y, float z)
{

	m_data[12]=x; m_data[13]= y; m_data[14]=z;


}

void Matrix::SetInverseTranslation(float x, float y, float z)
{

	m_data[12]=-x; m_data[13]= -y; m_data[14]=-z;


}
void Matrix::SetRotationRadians(float x, float y, float z)
{
	double cr = cos( x );
	double sr = sin( x );
	double cp = cos( y );
	double sp = sin(y );
	double cy = cos( z );
	double sy = sin( z );

	m_data[0] = ( float )( cp*cy );
	m_data[1] = ( float )( cp*sy );
	m_data[2] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	m_data[4] = ( float )( srsp*cy-cr*sy );
	m_data[5] = ( float )( srsp*sy+cr*cy );
	m_data[6] = ( float )( sr*cp );

	m_data[8] = ( float )( crsp*cy+sr*sy );
	m_data[9] = ( float )( crsp*sy-sr*cy );
	m_data[10] = ( float )( cr*cp );


}

*/

/*
void Matrix::SetInverseRotationRadians(float x, float y, float z)
{

//	x = DtoR(x);
//	y = DtoR(y);
//	z = DtoR(z);*

	double cr = cos( x );
	double sr = sin( x );
	double cp = cos( y );
	double sp = sin( y );
	double cy = cos( z );
	double sy = sin( z );

	m_data[0] = ( double )( cp*cy );
	m_data[4] = ( double )( cp*sy );
	m_data[8] = ( double )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	m_data[1] = ( double )( srsp*cy-cr*sy );
	m_data[5] = ( double )( srsp*sy+cr*cy );
	m_data[9] = ( double )( sr*cp );

	m_data[2] = ( double )( crsp*cy+sr*sy );
	m_data[6] = ( double )( crsp*sy-sr*cy );
	m_data[10] = ( double )( cr*cp );


}
*/

Matrix Matrix::InvertRotTrans(){

    Matrix res;

    res.m_data[0]  =   m_data[0];
    res.m_data[1]  =   m_data[4];
    res.m_data[2]  =   m_data[8];
    res.m_data[3]  =   m_data[3];
    res.m_data[4]  =   m_data[1];
    res.m_data[5]  =   m_data[5];
    res.m_data[6]  =   m_data[9];
    res.m_data[7]  =   m_data[7];
    res.m_data[8]  =   m_data[2];
    res.m_data[9]  =   m_data[6];
    res.m_data[10] =   m_data[10];
    res.m_data[11] =   m_data[11];
    res.m_data[12] = - (m_data[0] * m_data[12] + m_data[1] * m_data[13] + m_data[2]  * m_data[14]);
    res.m_data[13] = - (m_data[4] * m_data[12] + m_data[5] * m_data[13] + m_data[6]  * m_data[14]);
    res.m_data[14] = - (m_data[8] * m_data[12] + m_data[9] * m_data[13] + m_data[10] * m_data[14]);
    res.m_data[15] =   m_data[15];

    return res;
 }

CVector Matrix::operator*(const CVector &vec) const{

		CVector ret;
		ret.v[0] = m_data[0] * vec.v[0] + m_data[4] * vec.v[1] + m_data[8] * vec.v[2] + m_data[12];
		ret.v[1] = m_data[1] * vec.v[0] + m_data[5] * vec.v[1] + m_data[9] * vec.v[2] + m_data[13];
		ret.v[2] = m_data[2] * vec.v[0] + m_data[6] * vec.v[1] + m_data[10] * vec.v[2] + m_data[14];
		return ret;


}

//    return	res;

/*
   Matrix temp;
   temp = *this;

    m_data[0]=temp
    m_data[1]=temp
    m_data[2]=temp
    m_data[3]=temp

    m_data[4]=temp
    m_data[5]=temp
    m_data[6]=temp
    m_data[7]=temp

    m_data[8]=temp
    m_data[9]=temp
    m_data[10]=temp
    m_data[11]=temp

    m_data[12]=temp
    m_data[13]=temp
    m_data[14]=temp
    m_data[15]=temp


        M[0], M[1], M[2 ], -(M[12]*M[0]) - (M[13]*M[1]) - (M[14]*M[2 ]),
        M[4], M[5], M[6 ], -(M[12]*M[4]) - (M[13]*M[5]) - (M[14]*M[6 ]),
        M[8], M[9], M[10], -(M[12]*M[8]) - (M[13]*M[9]) - (M[14]*M[10]),
		0.0f, 0.0f, 0.0f , 1.0f);
};
*/

