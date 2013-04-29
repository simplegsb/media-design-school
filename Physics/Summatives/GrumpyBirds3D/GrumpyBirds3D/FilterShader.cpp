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
#include "EntityCategories.h"
#include "FilterShader.h"

using namespace physx;

PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
						   PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags,
						   const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
{
	// let triggers through
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eRESOLVE_CONTACTS | PxPairFlag::eSWEPT_INTEGRATION_LINEAR;

	if(filterData0.word0 == EntityCategories::GLASS ||
		filterData0.word0 == EntityCategories::PIG ||
		filterData0.word0 == EntityCategories::STONE ||
		filterData0.word0 == EntityCategories::WOOD ||
		filterData1.word0 == EntityCategories::GLASS ||
		filterData1.word0 == EntityCategories::PIG ||
		filterData1.word0 == EntityCategories::STONE ||
		filterData1.word0 == EntityCategories::WOOD)
	{
		pairFlags |= PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND;
	}

	return PxFilterFlag::eDEFAULT;
}
