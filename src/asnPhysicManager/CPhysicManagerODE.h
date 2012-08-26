#ifndef _CPHYSIC_MANAGER_H
#define _CPHYSIC_MANAGER_H

#include "IPhysicManager.h"
#include "CBaseObject.h"

#include "IEngine.h"
#include "INodeModifier.h"

#include "ode/ode.h"
#include <list>

class CPhysicObjectODE: public IPhysicObject, public CBaseObject {
public:
    CPhysicObjectODE(IEngine* _Engine);
    ~CPhysicObjectODE();

    void SetMass(float m);
    void AddForce(const CVector& force);

    INode* node;

	dBodyID myBody;
	dGeomID myGeom;
	dMass m;
	dSpaceID mySpace;
	//dJointID Joints;

};

class CPhysicManagerODE: public IPhysicManager, public CBaseObject {
public:
    CPhysicManagerODE(IEngine* _Engine);
    ~CPhysicManagerODE();

    void InitManager();

    virtual void SetGravity(const CVector& gravity);
//    virtual IPhysicObject* GetPhysicObject(const char* init_string);
    virtual IPhysicObject* AddNode(INode* node, const char* init_string);
//    virtual void AddNode(INode* node, IPhysicObject* customObject);
    virtual void DelNode(INode* node);

    virtual void Update(float tms);

    dWorldID world;
    dSpaceID space;
    dJointGroupID contactgroup;
    double calc_time;

    std::vector<CPhysicObjectODE*> PhysicObjectList;
};

class CPhysicAdapterODE: public virtual INodeModifier, public virtual CBaseObject {
public:
    CPhysicAdapterODE(IEngine* _Engine);
    ~CPhysicAdapterODE();
    int Modify(INode *node,float tms);
private:
    CPhysicManagerODE* myPhysManager;
    CPhysicObjectODE* myPhysObject;
    INode* myNode;
    friend class CPhysicManagerODE;

};

//#define PHYSICS ((IPhysicManager*)ENGINE->GetPtrParam("PhysicManager"))

#endif
