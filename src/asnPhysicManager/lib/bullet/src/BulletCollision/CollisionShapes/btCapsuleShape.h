/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BT_CAPSULE_SHAPE_H
#define BT_CAPSULE_SHAPE_H

#include "btConvexShape.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h" // for the types


///btCapsuleShape represents a capsule around the Y axis
///A more general solution that can represent capsules is the btMultiSphereShape
class btCapsuleShape : public btConvexShape
{

public:
	btCapsuleShape(btScalar radius,btScalar height);

	///CollisionShape Interface
	virtual void	calculateLocalInertia(btScalar mass,btVector3& inertia);

	/// btConvexShape Interface
	virtual btVector3	localGetSupportingVertexWithoutMargin(const btVector3& vec)const;

	virtual void	batchedUnitVectorGetSupportingVertexWithoutMargin(const btVector3* vectors,btVector3* supportVerticesOut,int numVectors) const;
	
	virtual int	getShapeType() const { return CAPSULE_SHAPE_PROXYTYPE; }

	virtual char*	getName()const 
	{
		return "CapsuleShape";
	}

	btScalar	getRadius() const
	{
		return m_implicitShapeDimensions.getX();
	}

	btScalar	getHalfHeight() const
	{
		return m_implicitShapeDimensions.getY();
	}

};



#endif //BT_CAPSULE_SHAPE_H
