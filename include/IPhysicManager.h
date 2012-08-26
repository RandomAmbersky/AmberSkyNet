#ifndef _IPHYSIC_MANAGER_H
#define _IPHYSIC_MANAGER_H

#include "IManager.h"
#include "INode.h"

class IPhysicObject:  public virtual IBaseObject {
public:
    virtual void SetMass(float m)=0;
    virtual void AddForce(const CVector& force)=0;
};

class IPhysicManager: public virtual IManager {
public:

    virtual void SetGravity(const CVector& gravity)=0;

//    virtual IPhysicObject* GetPhysicObject(const char* init_string)=0;
    virtual IPhysicObject* AddNode(INode* node, const char* init_string)=0;
    virtual void DelNode(INode* node)=0;

    virtual void Update(float tms)=0;
};

#define PHYSICS ((IPhysicManager*)ENGINE->GetPtrParam("PhysicManager"))

#endif
