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
#include "ClothEngine.h"

CClothEngine::CClothEngine(CCloth* _pCloth) :
	m_pCloth(_pCloth)
{
}

CClothEngine::~CClothEngine()
{
	delete m_pCloth;
}

void CClothEngine::addEntity(Entity*)
{
}

void CClothEngine::advance()
{
	m_pCloth->VerletIntegration();
	m_pCloth->SatisfyConstraints();
	m_pCloth->UpdateModels();
}

void CClothEngine::destroy()
{
}

void CClothEngine::init()
{
	m_pCloth->AddModelsToGazEngine();
}

void CClothEngine::removeEntity(const Entity&)
{
}
