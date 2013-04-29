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
#include <gazengine/physics/PhysXBody.h>

#include "EntityCategories.h"
#include "Scene.h"
#include "SimulationEventCallback.h"

using namespace physx;

SimulationEventCallback::SimulationEventCallback(SimpleTree* _pParentNode) :
	m_destroyedWallBlocks(),
	m_pParentNode(_pParentNode)
{
}

void SimulationEventCallback::destroyWallBlock(const PxRigidBody& _krRigidBody, const PxShape& _krShape)
{
	Entity* wallBlock = static_cast<Entity*>(_krRigidBody.userData);
	if (find(m_destroyedWallBlocks.begin(), m_destroyedWallBlocks.end(), wallBlock) != m_destroyedWallBlocks.end())
	{
		return;
	}

	PxBoxGeometry geometry;
	_krShape.getBoxGeometry(geometry);

	PxTransform transform = _krRigidBody.getGlobalPose();
	Matrix44 transformation = PhysXMatrix::toMatrix44(transform);

	PxVec3 angularVelocity = _krRigidBody.getAngularVelocity();
	PxVec3 linearVelocity = _krRigidBody.getLinearVelocity();

	m_destroyedWallBlocks.push_back(wallBlock);
	GazEngine::removeEntity(*wallBlock);

	float halfSize = geometry.halfExtents.x / 2.0f;

	Matrix44 fragment0Transformation = transformation;
	getTranslation3(fragment0Transformation) += Vector3(-halfSize, halfSize, 0.0f);
	Entity* pFragment0 = CreateWallBlock(fragment0Transformation, halfSize, *m_pParentNode);
	PxRigidBody* pFragment0RigidBody = pFragment0->getSingleComponent<PhysXBody>()->getActor()->isRigidBody();
	pFragment0RigidBody->setAngularVelocity(angularVelocity);
	pFragment0RigidBody->setLinearVelocity(linearVelocity);
	// The body only got a position in the constructor... lets give it a rotation too.
	PxTransform fragment0Transform = transform;
	fragment0Transform.p += PxVec3(-halfSize, halfSize, 0.0f);
	pFragment0RigidBody->setGlobalPose(fragment0Transform);

	Matrix44 fragment1Transformation = transformation;
	getTranslation3(fragment1Transformation) += Vector3(halfSize, halfSize, 0.0f);
	Entity* pFragment1 = CreateWallBlock(fragment1Transformation, halfSize, *m_pParentNode);
	PxRigidBody* pFragment1RigidBody = pFragment1->getSingleComponent<PhysXBody>()->getActor()->isRigidBody();
	pFragment1RigidBody->setAngularVelocity(angularVelocity);
	pFragment1RigidBody->setLinearVelocity(linearVelocity);
	// The body only got a position in the constructor... lets give it a rotation too.
	PxTransform fragment1Transform = transform;
	fragment1Transform.p += PxVec3(halfSize, halfSize, 0.0f);
	pFragment1RigidBody->setGlobalPose(fragment1Transform);

	Matrix44 fragment2Transformation = transformation;
	getTranslation3(fragment2Transformation) += Vector3(halfSize, -halfSize, 0.0f);
	Entity* pFragment2 = CreateWallBlock(fragment2Transformation, halfSize, *m_pParentNode);
	PxRigidBody* pFragment2RigidBody = pFragment2->getSingleComponent<PhysXBody>()->getActor()->isRigidBody();
	pFragment2RigidBody->setAngularVelocity(angularVelocity);
	pFragment2RigidBody->setLinearVelocity(linearVelocity);
	// The body only got a position in the constructor... lets give it a rotation too.
	PxTransform fragment2Transform = transform;
	fragment2Transform.p += PxVec3(halfSize, -halfSize, 0.0f);
	pFragment2RigidBody->setGlobalPose(fragment2Transform);

	Matrix44 fragment3Transformation = transformation;
	getTranslation3(fragment3Transformation) += Vector3(-halfSize, -halfSize, 0.0f);
	Entity* pFragment3 = CreateWallBlock(fragment3Transformation, halfSize, *m_pParentNode);
	PxRigidBody* pFragment3RigidBody = pFragment3->getSingleComponent<PhysXBody>()->getActor()->isRigidBody();
	pFragment3RigidBody->setAngularVelocity(angularVelocity);
	pFragment3RigidBody->setLinearVelocity(linearVelocity);
	// The body only got a position in the constructor... lets give it a rotation too.
	PxTransform fragment3Transform = transform;
	fragment3Transform.p += PxVec3(-halfSize, -halfSize, 0.0f);
	pFragment3RigidBody->setGlobalPose(fragment3Transform);
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
{
}

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
	PxU32 nbPairs)
{
	for (PxU32 index = 0; index < nbPairs; index++)
    {
        const PxContactPair& pair = pairs[index];

        if (pair.events & PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND)
        {
			if (pair.shapes[0]->getSimulationFilterData().word0 == EntityCategories::WALL_BLOCK)
			{
				destroyWallBlock(*pairHeader.actors[0]->isRigidBody(), *pair.shapes[0]);
			}

			if (pair.shapes[1]->getSimulationFilterData().word0 == EntityCategories::WALL_BLOCK)
			{
				destroyWallBlock(*pairHeader.actors[1]->isRigidBody(), *pair.shapes[1]);
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
