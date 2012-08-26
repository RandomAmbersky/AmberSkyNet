#ifndef _CPHYSIC_MANAGER_H
#define _CPHYSIC_MANAGER_H

#include "IPhysicManager.h"
#include "CBaseObject.h"

#include "IEngine.h"
#include "INodeModifier.h"

#include "ode/ode.h"
#include <list>

class CPhysicObject: public virtual IPhysicObject, public virtual CBaseObject {
public:
    CPhysicObject(IEngine* _Engine);
    ~CPhysicObject();

    void setMass(float m);
    void addForce(const CVector& force);

    INode* node;

	dBodyID myBody;
	dGeomID myGeom;
	dMass m;
	dSpaceID mySpace;
	//dJointID Joints;

};

class CPhysicManager: public virtual IPhysicManager, public virtual CBaseObject {
public:
    CPhysicManager(IEngine* _Engine);
    ~CPhysicManager();

    virtual void setGravity(const CVector& gravity);
//    virtual IPhysicObject* GetPhysicObject(const char* init_string);
    virtual IPhysicObject* AddNode(INode* node, const char* init_string);
//    virtual void AddNode(INode* node, IPhysicObject* customObject);
    virtual void DelNode(INode* node);

    virtual void Update(float tms);

    dWorldID world;
    dSpaceID space;
    dJointGroupID contactgroup;
    double calc_time;

    std::vector<CPhysicObject*> PhysicObjectList;
};

class CPhysicAdapter: public virtual INodeModifier, public virtual CBaseObject {
public:
    CPhysicAdapter(IEngine* _Engine);
    ~CPhysicAdapter();
    int Modify(INode *node,float tms);
private:
    CPhysicManager* myPhysManager;
    CPhysicObject* myPhysObject;
    INode* myNode;
    friend class CPhysicManager;

};

//#define PHYSICS ((IPhysicManager*)ENGINE->GetPtrParam("PhysicManager"))

#endif
