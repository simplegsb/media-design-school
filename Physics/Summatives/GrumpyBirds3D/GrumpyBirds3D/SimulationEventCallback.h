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
#pragma once

#include <PxPhysicsAPI.h>

class SimulationEventCallback : public physx::PxSimulationEventCallback
{
	public:
		SimulationEventCallback();

		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);

		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs,
			physx::PxU32 nbPairs);

		void onSleep(physx::PxActor** actors, physx::PxU32 count);

		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);

		void onWake(physx::PxActor** actors, physx::PxU32 count);

	private:
		std::vector<Entity*> m_destroyedBoxes;

		std::vector<void*> removed;

		void createBoxFragment(const Matrix44& _krTransformation, const Vector3& _krHalfExtents,
			unsigned short _usCategory, const physx::PxVec3& _fLinearVelocity, const physx::PxVec3& _fAngularVelocity);

		void destroyBox(const physx::PxRigidBody& _krRigidBody, const physx::PxShape& _krShape);
};
