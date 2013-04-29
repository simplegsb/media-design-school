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

physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
								  physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
								  physx::PxPairFlags& pairFlags, const void* constantBlock,
								  physx::PxU32 constantBlockSize);
