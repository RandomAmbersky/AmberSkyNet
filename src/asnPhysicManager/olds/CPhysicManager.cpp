#include "CPhysicManager.h"
#include "ILog.h"
#include <math.h>
#include "ASN_Utils.h"
#include "matrix.h"

#include "INodeModifier.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

CPhysicAdapter::CPhysicAdapter(IEngine* _Engine):CBaseObject(_Engine){

};

CPhysicAdapter::~CPhysicAdapter(){

    printf("-CPhysicAdapter\n");
    myPhysManager->DelNode(myNode);

};

int CPhysicAdapter::Modify(INode *node,float tms){

        if (myNode!=node) return 0;

        const dReal* p_pos = dBodyGetPosition(myPhysObject->myBody);
        const dReal* R = dBodyGetRotation(myPhysObject->myBody);

        CVector pos = CVector(p_pos[0],p_pos[1],p_pos[2]);

  Matrix matrix;

  matrix.m_data[0]=R[0];
  matrix.m_data[1]=R[4];
  matrix.m_data[2]=R[8];
  matrix.m_data[3]=0;
  matrix.m_data[4]=R[1];
  matrix.m_data[5]=R[5];
  matrix.m_data[6]=R[9];
  matrix.m_data[7]=0;
  matrix.m_data[8]=R[2];
  matrix.m_data[9]=R[6];
  matrix.m_data[10]=R[10];
  matrix.m_data[11]=0;
  matrix.m_data[12]=p_pos[0];
  matrix.m_data[13]=p_pos[1];
  matrix.m_data[14]=p_pos[2];
  matrix.m_data[15]=1;

    	//float pitch=p_rot[0]*180.0f/M_PI;
        //float yaw=p_rot[1]*180.0f/M_PI;
        //float roll=p_rot[2]*180.0f/M_PI;

        //printf("%f %f %f\n", pitch,roll,yaw);
    	//CVector rot = CVector(pitch,yaw,roll);

    	CVector rot = matrix.GetRot();

    	rot.v[0]=rot.v[0]*180.0f/M_PI;
        rot.v[1]=rot.v[1]*180.0f/M_PI;
        rot.v[2]=rot.v[2]*180.0f/M_PI;

    	myNode->setPos(pos);
    	myNode->setRot( rot );

        CVector size = myNode->getSize();
        dGeomBoxSetLengths(myPhysObject->myGeom, size.v[0], size.v[1], size.v[2] );

};


dWorldID ptr_world;
dJointGroupID ptr_contactgroup;
dGeomID ground;

/*
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{

	int i, n;


	const int N = 10;
	dContact contact[N];
	n = dCollide (o1, o2, N, &contact[0].geom, sizeof(dContact));
	if (n > 0)
	{
		for (i=0; i<n; i++)
		{
			//contact[i].surface.mode = dContactBounce | dContactSoftCFM ;
			//contact[i].surface.mu = 2000; //5000;

      contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = 100.0;
      contact[i].surface.soft_erp = 0.96;
      contact[i].surface.soft_cfm = 0.02;


			contact[i].surface.bounce = 0.3;
			contact[i].surface.bounce_vel = 0.2;
            contact[i].surface.soft_cfm = 0.01;
  			dJointID c = dJointCreateContact (ptr_world,ptr_contactgroup,&contact[i]);
			dJointAttach(c,dGeomGetBody(o1), dGeomGetBody(o2));
		}
	}


  /*
  int i,n;

  // only collide things with the ground
  //int g1 = (o1 == ground || o1 == ground_box);
  //int g2 = (o2 == ground || o2 == ground_box);
  //if (!(g1 ^ g2)) return;

  const int N = 10;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
	dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.3;
      dJointID c = dJointCreateContact (ptr_world,ptr_contactgroup,&contact[i]);
      dJointAttach (c,
		    dGeomGetBody(contact[i].geom.g1),
		    dGeomGetBody(contact[i].geom.g2));
    }
  }

}
*/

static void SilenceMessage(int errnum, const char *msg, va_list ap){

};

#define MAX_CONTACTS 8		// maximum number of contact points per body

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i;
  // if (o1->body && o2->body) return;

  // exit without doing anything if the two bodies are connected by a joint
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  //if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;
    if (b1 && b2 && dAreConnected (b1,b2) ) return;

  dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactBounce | dContactSoftCFM;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
    contact[i].surface.bounce = 0.1;
    contact[i].surface.bounce_vel = 0.1;
    contact[i].surface.soft_cfm = 0.01;
  }
  if (int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,
			   sizeof(dContact))) {

    //printf("%d\n",numc);
    if (numc>3) numc=3;
    const dReal ss[3] = {0.02,0.02,0.02};
    for (i=0; i<numc; i++) {
      dJointID c = dJointCreateContact (ptr_world,ptr_contactgroup,contact+i);
      dJointAttach (c,b1,b2);

    }
  }

}

void CPhysicManager::setGravity(const CVector& gravity){
	dWorldSetGravity(world, gravity.v[0], gravity.v[1], gravity.v[2] );
};

CPhysicManager::CPhysicManager(IEngine* _Engine):CBaseObject(_Engine){

    SetParam("BaseClass","PhysicManager");
    SetParam("Type","CPhysicManager");
    SetParam("Name","-");

	dSetErrorHandler (SilenceMessage); //затыкаем рот идиотским ошибкам
    dSetDebugHandler (SilenceMessage); //затыкаем рот идиотским ошибкам
    dSetMessageHandler (SilenceMessage); //затыкаем рот идиотским ошибкам


    calc_time= 0.0f;

	world = dWorldCreate();
	dWorldSetGravity(world, 0, 0, -0.01 );
	space=dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);

    LOGGER->LogMsg("+CPhysicManager");

    ptr_world = world;
    ptr_contactgroup = contactgroup;

    dWorldSetCFM(world, 1e-5);
    dWorldSetERP(world, 0.2);
    dWorldSetContactMaxCorrectingVel(world, 0.9);
    dWorldSetContactSurfaceLayer(world, 0.001);
    dWorldSetAutoDisableFlag(world, 1);

//    ground =

#define SHIRINA 1000

    //dGeomID tmp = dCreateBox( space, SHIRINA, SHIRINA, 1);
    //dGeomSetPosition(tmp, SHIRINA/2, SHIRINA/2, 0); // двигаем центр координат

    //tmp = dCreateBox( space, 1, SHIRINA, SHIRINA);
    //dGeomSetPosition(tmp, 0, SHIRINA/2, SHIRINA/2); // двигаем центр координат

    //tmp = dCreateBox( space, SHIRINA, 1, SHIRINA);
    //dGeomSetPosition(tmp, SHIRINA/2, 0, SHIRINA/2); // двигаем центр координат

    dCreatePlane (space,0,0,1,0);

    //dCreatePlane (space,1,1,0,0);

    //dCreatePlane (space,0,1,0,0);
    //dCreatePlane (space,1,0,0,0);

    //dCreatePlane (space,0,0,20,0);
    //dCreatePlane (space,0,10,0,0);
    //dCreatePlane (space,10,0,0,0);

    //dCreatePlane (space,0,0,1,0);
    //dCreatePlane (space,0,0,1,0);
    //dCreatePlane (space,0,0,1,0);
    //dCreatePlane (space,0,0,1,0);
    //dCreatePlane (space,0,0,1,0);

};

CPhysicManager::~CPhysicManager(){

    dWorldDestroy( world );
    //dHashSpaceDestroy( space );
    dJointGroupDestroy( contactgroup );

    LOGGER->LogMsg("-CPhysicManager");
};

//IPhysicObject* CPhysicManager::GetPhysicObject(const char* init_string){
//};

#define DENSITY (5.0)		// density of all objects

IPhysicObject* CPhysicManager::AddNode(INode* node, const char* init_string){

        CPhysicAdapter* myAdapter = new CPhysicAdapter(Engine);
        myAdapter->myPhysManager = this;
        myAdapter->myNode = node;

        IModifierPack* nodeModPack = (IModifierPack*)node->GetPtrParam("ModifierPack");
        nodeModPack->AddModifier( myAdapter );

        CVector pos = node->getPos();
    	CVector rot = node->getRot();
    	CVector size = node->getSize();

    	CVector center = node->getCenterPoint();

    	CPhysicObject* myObj = new CPhysicObject(Engine);
        node->SetPtrParam("PhysicObject",myObj);

        myAdapter->myPhysObject =myObj;

        myObj->node = node;

    	myObj->myBody = dBodyCreate(world);

        dBodySetPosition(myObj->myBody, pos.v[0], pos.v[1], pos.v[2] );
        dBodySetLinearVel(myObj->myBody, 0, 0, 0);
        dBodySetAngularVel(myObj->myBody, 0, 0, 0);

        dMassSetBox (&myObj->m,DENSITY,size.v[0],size.v[1],size.v[2]);
        dBodySetMass(myObj->myBody, &myObj->m);

    	float pitch=rot.v[0]*M_PI/180.0f;
        float yaw=rot.v[1]*M_PI/180.0f;
        float roll=rot.v[2]*M_PI/180.0f;

        //printf("%f %f %f\n", pitch,roll,yaw);

        dMatrix3 R;
		dRFromEulerAngles(R, pitch, yaw, roll );
		dBodySetRotation(myObj->myBody, R);

    	myObj->myGeom = dCreateBox(space, size.v[0], size.v[1], size.v[2] );
        dGeomSetBody(myObj->myGeom, myObj->myBody);

		//dGeomSetPosition(myObj->myGeom, pos.v[0]+center.v[0], pos.v[1]+center.v[1], pos.v[2]+center.v[2] );

        //dBodyAddTorque(myObj->myBody, 0, 0, 0);

        //dBodyAddTorque(myObj->myBody, 0, 1, 0);

        // преобразуем позицию ноды и углы поворота в матрицу
        //Matrix NodeMatrix;
        //NodeMatrix.LoadIdentity();

        //NodeMatrix.m_data[12] = pos.v[0];
        //NodeMatrix.m_data[13] = pos.v[1];
        //NodeMatrix.m_data[14] = pos.v[2];

        //CVector	temp = CVector( NodeMatrix.m_data[12], NodeMatrix.m_data[13], NodeMatrix.m_data[14]);
        //NodeMatrix.RotateXYZ(rot);
        //NodeMatrix.m_data[12] = temp.v[0];
        //NodeMatrix.m_data[13] = temp.v[1];
        //NodeMatrix.m_data[14] = temp.v[2];

        dGeomSetData(myObj->myGeom, node);

        //dGeomSetRotation(myObj->myGeom, R);

//      dBodySetLinearVel(Object.Body, tempVect.x, tempVect.y, tempVect.z);

        PhysicObjectList.push_back(myObj);
        printf("geom add node\n");

        return myObj;
        //dBodySetLinearVel(myObj->myBody, 0.0, 0.0, 5.0 );

};

void CPhysicManager::DelNode(INode* node){

    std::vector<CPhysicObject*>::iterator _it=PhysicObjectList.begin();
    for (int i=PhysicObjectList.size();i>0;i--){
        CPhysicObject* my = (*_it);
        if (my->node==node) {
            dBodyDestroy(my->myBody);
            dGeomDestroy(my->myGeom);
            my->Release();
            PhysicObjectList.erase(_it);
            break;
            }
        _it++;
    };

};

void CPhysicManager::Update(float tms){

        calc_time+=tms;

//		printf("update %f\n",calc_time);

//		float full_time = tms;
//		float dt=1.0/30.0f;
        //if ( dt > tms ) dt = tms;

if (calc_time>10) {
        //fast!!
		dSpaceCollide (space,0,&nearCallback);
		dWorldQuickStep(world, tms );
		dJointGroupEmpty(contactgroup);
        calc_time=0.0f;
}
else
		while (calc_time>1.0/30.0f) {

		dSpaceCollide (space,0,&nearCallback);
		dWorldQuickStep(world, (1.0/30.0f) );
		dJointGroupEmpty(contactgroup);
        calc_time-=1.0/30.0f;

		};

};



CPhysicObject::CPhysicObject(IEngine* _Engine):CBaseObject(_Engine){
    node=NULL;
};

CPhysicObject::~CPhysicObject(){

};

void CPhysicObject::setMass(float m){

};

void CPhysicObject::addForce(const CVector& force){
    if ( !dBodyIsEnabled (myBody) )
        dBodyEnable(myBody);
     dBodyAddForce(myBody, force.v[0], force.v[1], force.v[2]);
};

