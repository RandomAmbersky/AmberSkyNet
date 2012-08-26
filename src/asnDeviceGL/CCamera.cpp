#include "IEngine.h"

#include "CCamera.h"

#include "matrix.h"
#include "quaternion.h"

#define DEVICER ((IDevice*)ENGINE->GetPtrParam("DeviceManager"))

#include "IDevice.h"
#define ENGINE Engine //у нас уже есть указатель на Engine

#include "ASN_utils.h"
#include "IEventListener.h"
#include "IEventManager.h"

CCamera::CCamera(IEngine *_Engine):CBaseObject(_Engine)
{
    Engine=_Engine;

    SetParam("BaseClass","Camera");
    SetParam("Type","CameraSimple");
    SetParam("Name","None");

    SetPtrParam("Camera", this);

    NodeMatrix.LoadIdentity();
    ProjectMatrix.LoadIdentity();

    screen[0]=0.01;
    screen[1]=0.01;
    screen[2]=0.98;
    screen[3]=0.98;

    psi=phi=0.0;
    Pos=CVector(0,0,0);
    CalcDir();

	ProjectMatrix.Perspective(60.0f, 4.0f/3.0f, 1.0f, 65535);

    SetScale(10);

}

CCamera::~CCamera(){

    printf("CCamera::~CCamera()\n");

}

void CCamera::SetScreen(float startX, float startY, float endX, float endY)
{
    screen[0]=startX;
    screen[1]=startY;
    screen[2]=endX;
    screen[3]=endY;
}

void CCamera::SetPerspective(float fov, float aspect, float zNear, float zFar)
{
    printf("set perspective...\n");
    ProjectMatrix.Perspective(fov, aspect, zNear, zFar);
    printf("set perspective ok\n");

}

void CCamera::View()
{

//    printf("My cam view!!! \n");

float width=(float)StrToInt( ENGINE->GetEnvParam("[DeviceManager]width"));
float height=(float)StrToInt( ENGINE->GetEnvParam("[DeviceManager]height"));

//StrToInt(

    int startX=(int)(width*screen[0]);
    int startY=(int)(height*screen[1]);
    int sizeX=(int)(width*screen[2]);
    int sizeY=(int)(height*screen[3]);

    DEVICER->SetScreen( startX, startY, sizeX, sizeY );
//    printf("%d %d %d %d %f %f \n", startX, startY, sizeX, sizeY, width*screen[2], height*screen[3]  );
    //printf("My cam view!!! \n");
    // ух, посмотрим ща по направлению..
    //setScale(10);
    NodeMatrix.LookAt( Pos, Pos + Dir, CVector(0,0,1));
    //DEVICER->SetProjectMatrix(ProjectMatrix.m_data);
    //DEVICER->SetModelMatrix(NodeMatrix.m_data);
    //ProjectMatrix.LoadIdentity();
    Matrix tmpMatrix = NodeMatrix;
    //tmpMatrix.Scale( CVector(10,10,10) );
    DEVICER->SetMatrixs(tmpMatrix.m_data,ProjectMatrix.m_data);
    //DEVICER->SetScale( CVector(10,10,10) );
//    printf("My cam view ok! \n");
}


void CCamera::MoveForward(const float val)
{

    Pos+=Dir*val;

}

void CCamera::MoveStrafe(const float val)
{

    Pos+=DirY*val;

}

void CCamera::MoveUp(const float val)
{

    Pos+=DirZ*val;

}

void CCamera::SetPos(const CVector& _Pos)
{

    Pos=_Pos;

}

void CCamera::SetScale(float scale){
    NodeMatrix.m_data[0]  = scale;
    NodeMatrix.m_data[5]  = scale;
    NodeMatrix.m_data[10] = scale;

    //ProjectMatrix.Scale( CVector(sca));
}

void CCamera::LookAt(const CVector& _Pos)
{

    Matrix m;
    m.LookAt( Pos, _Pos, CVector(0,0,1) );

    CVector tmp = m.GetRot();

    psi=-tmp.v[2]*GRAD_PI+90.0f;
    phi=-tmp.v[0]*GRAD_PI-90.0f;

    CalcDir();

}

void CCamera::GetRot(float& RotX, float& RotY) const
{
    RotX=psi;
    RotY=phi;
}

const CVector& CCamera::GetPos() const
{
    return Pos;
}

void CCamera::SetRotX(const float val)
{
    psi=val;
    CalcDir();
}

void CCamera::SetRotY(const float val)
{
    phi=val;
    CalcDir();
}

void CCamera::LookAtMouse(int Pos_X, int Pos_Y)
{

//    printf(" camera: look at mouse\n");
    //    float dHeading=(float)atoi( ENGINE->GetEnvParam("[DeviceManager]width").c_str() )/2.0f-(float)Pos_X;
    //    float dPitch=(float)atoi( ENGINE->GetEnvParam("[DeviceManager]height").c_str() )/2.0f-(float)Pos_Y;

    float dHeading = StrToFloat ( ENGINE->GetEnvParam("[DeviceManager]width") )/2.0f-(float)Pos_X;
    float dPitch = StrToFloat ( ENGINE->GetEnvParam("[DeviceManager]height") )/2.0f-(float)Pos_Y;

    psi+=dHeading*0.2;
    phi+=dPitch*0.2;


    if(phi < -89)
    {
        phi = -89;
    }
    if(phi > 89)
    {
        phi = 89;
    }

    CalcDir();

//    printf("psi:%f phi:%f\n",psi,phi);

}

void CCamera::CalcDir()
{

    Quaternion q0,q1;
    Matrix m;

    q0.CreateFromAxisAngle( CVector(0,0,1), psi);
    q1.CreateFromAxisAngle( CVector(0,1,0), -phi);

    q0=q0*q1;
    q0.CreateMatrix(m.m_data);

    Dir = m*CVector(1,0,0);
    Dir.Normalize();

    DirY.Cross( Dir,CVector(0,0,1) );
    DirY.Normalize();

    DirZ.Cross( DirY,Dir );
    DirZ.Normalize();

}

bool CCamera::AddChild(IBaseObject* child){

    printf("CCamera::addChild(%s/%s)\n", child->GetType(), child->GetBaseClass() );

    std::string myTc = child->GetBaseClass();

	if ( myTc == "EventListener") {

	    IEventListener* myList = dynamic_cast<IEventListener*>(child);//->GetPtrParam("EventListener");
	    if (!myList)  {printf("{2 is null}[error!]\n");return false;}

        printf("CCamera::addChild(%s) try EVENTER\n", child->GetType() );
        if (!EVENTER) {printf("CCamera::addChild(%s) try EVENTER fault!\n", child->GetType() ); return false;}
        EVENTER->AddCustomEventListener( myList );
        printf("CCamera::addChild(%s) OK\n", child->GetType() );

        myList->SetPtrParam("Camera", this);
        CBaseObject::AddChild( child );

	    printf("[ok]\n");
	    return true;
	}

    return false;

}

bool CCamera::DelChild(UID childID){

    printf("CCamera::delChild %d\n", childID );

    CBaseObject_map::iterator it_ = Childs.find(childID);
    if ( it_==Childs.end() ) { printf("CCamera::delChild %d FAULT\n", childID ); return false; }

   	std::string myTc = (*it_).second->GetBaseClass();
	if ( myTc == "EventListener") {
	    IEventListener* myList = dynamic_cast<IEventListener*>((*it_).second);
	    if (!myList)  {printf("{2 is null}[error!]\n");return false;}

        EVENTER->DelEventListener( childID );
        CBaseObject::DelChild( childID );
        printf("CCamera::delChild %d OK\n", childID );
        return true;
	}

    return false;

}
