//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//
#include <algorithm>

#include <gazengine/GazEngine.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/math/PhysXMatrix.h>
#include <gazengine/math/PhysXVector.h>
#include <gazengine/physics/PhysXBody.h>

#include "EntityCategories.h"
#include "SceneFactory.h"
#include "SimulationEventCallback.h"

using namespace physx;

SimulationEventCallback::SimulationEventCallback() :
	m_destroyedBoxes()
{
}

void SimulationEventCallback::createBoxFragment(const Matrix44& _krTransformation, const Vector3& _krHalfExtents,
												unsigned short _usCategory, const PxVec3& _fLinearVelocity,
												const PxVec3& _fAngularVelocity)
{
	PxRigidBody* pRigidBody = SceneFactory::CreateBox(NULL, _krTransformation, _krHalfExtents, _usCategory)
		->getActor()->isRigidBody();
	pRigidBody->setAngularVelocity(_fAngularVelocity);
	pRigidBody->setLinearVelocity(_fLinearVelocity);
}

void SimulationEventCallback::destroyBox(const PxRigidBody& _krRigidBody, const PxShape& _krShape)
{
	Entity* box = static_cast<Entity*>(_krRigidBody.userData);
	if (find(m_destroyedBoxes.begin(), m_destroyedBoxes.end(), box) != m_destroyedBoxes.end())
	{
		return;
	}

	unsigned short usCategory = box->getCategory();
	unsigned short usFragmentCategory;
	if (usCategory == EntityCategories::GLASS)
	{
	}
	else if (usCategory == EntityCategories::WOOD)
	{
		usFragmentCategory = EntityCategories::WOOD_FRAGMENT;
	}

	PxConvexMeshGeometry geometry;
	_krShape.getConvexMeshGeometry(geometry);

	PxTransform transform = _krRigidBody.getGlobalPose();
	Matrix44 transformation = PhysXMatrix::toMatrix44(transform);

	PxVec3 angularVelocity = _krRigidBody.getAngularVelocity();
	PxVec3 linearVelocity = _krRigidBody.getLinearVelocity();

	m_destroyedBoxes.push_back(box);
	GazEngine::removeEntity(*box);

	Vector3 halfExtents;
	halfExtents.X() = geometry.convexMesh->getVertices()[0].x * -0.5f;
	halfExtents.Y() = geometry.convexMesh->getVertices()[0].y * 0.5f;
	halfExtents.Z() = geometry.convexMesh->getVertices()[0].z * 0.5f;
	Vector3 halfExtentOut;
	halfExtentOut = getOut3(transformation);
	halfExtentOut.normalize();
	halfExtentOut *= halfExtents.Z();
	Vector3 halfExtentRight;
	halfExtentRight = getRight3(transformation);
	halfExtentRight.normalize();
	halfExtentRight *= halfExtents.X();
	Vector3 halfExtentUp;
	halfExtentUp = getUp3(transformation);
	halfExtentUp.normalize();
	halfExtentUp *= halfExtents.Y();
	halfExtents *= 0.9f;

	Matrix44 fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) -= halfExtentRight;
	getTranslation3(fragmentTransformation) += halfExtentUp;
	getTranslation3(fragmentTransformation) += halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) += halfExtentRight;
	getTranslation3(fragmentTransformation) += halfExtentUp;
	getTranslation3(fragmentTransformation) += halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) += halfExtentRight;
	getTranslation3(fragmentTransformation) -= halfExtentUp;
	getTranslation3(fragmentTransformation) += halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) -= halfExtentRight;
	getTranslation3(fragmentTransformation) -= halfExtentUp;
	getTranslation3(fragmentTransformation) += halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) -= halfExtentRight;
	getTranslation3(fragmentTransformation) += halfExtentUp;
	getTranslation3(fragmentTransformation) -= halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) += halfExtentRight;
	getTranslation3(fragmentTransformation) += halfExtentUp;
	getTranslation3(fragmentTransformation) -= halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	fragmentTransformation = transformation;
	getTranslation3(fragmentTransformation) += halfExtentRight;
	getTranslation3(fragmentTransformation) -= halfExtentUp;
	getTranslation3(fragmentTransformation) -= halfExtentOut;
	createBoxFragment(fragmentTransformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);

	Matrix44 fragment3Transformation = transformation;
	getTranslation3(fragmentTransformation) -= halfExtentRight;
	getTranslation3(fragmentTransformation) -= halfExtentUp;
	getTranslation3(fragmentTransformation) -= halfExtentOut;
	createBoxFragment(fragment3Transformation, halfExtents, usFragmentCategory, linearVelocity, angularVelocity);
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
{
}

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
	PxU32 pairCount)
{
	for (PxU32 index = 0; index < pairCount; index++)
    {
        const PxContactPair& pair = pairs[index];

        if (pair.events & PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND)
        {
			if (find(removed.begin(), removed.end(), pairHeader.actors[0]->userData) == removed.end())
			{
				if (pair.shapes[0]->getSimulationFilterData().word0 == EntityCategories::GLASS ||
					pair.shapes[0]->getSimulationFilterData().word0 == EntityCategories::WOOD)
				{
					destroyBox(*pairHeader.actors[0]->isRigidBody(), *pair.shapes[0]);
				}

				if (pair.shapes[0]->getSimulationFilterData().word0 == EntityCategories::PIG)
				{
					GazEngine::removeEntity(*static_cast<Entity*>(pairHeader.actors[0]->userData));
				}

				removed.push_back(pairHeader.actors[0]->userData);
			}

			if (find(removed.begin(), removed.end(), pairHeader.actors[1]->userData) == removed.end())
			{
				if (pair.shapes[1]->getSimulationFilterData().word0 == EntityCategories::GLASS ||
					pair.shapes[1]->getSimulationFilterData().word0 == EntityCategories::WOOD)
				{
					destroyBox(*pairHeader.actors[1]->isRigidBody(), *pair.shapes[1]);
				}

				if (pair.shapes[1]->getSimulationFilterData().word0 == EntityCategories::PIG)
				{
					GazEngine::removeEntity(*static_cast<Entity*>(pairHeader.actors[1]->userData));
				}

				removed.push_back(pairHeader.actors[1]->userData);
			}
        }
    }
}

void SimulationEventCallback::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{
}

void SimulationEventCallback::onTrigger(PxTriggerPair* /*pairs*/, PxU32 /*count*/)
{
}

void SimulationEventCallback::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
}
