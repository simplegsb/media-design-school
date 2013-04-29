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

#include <gazengine/math/MathFunctions.h>

#include "DepthSortingEngine.h"

using namespace std;

CDepthSortingEngine::CDepthTestComparator::CDepthTestComparator(const SimpleTree* _kpCameraNode) :
	m_kpCameraNode(_kpCameraNode)
{
}

bool CDepthSortingEngine::CDepthTestComparator::operator()(const SimpleTree* _kpLhs, const SimpleTree* _kpRhs) const
{
	Matrix44 cameraTransform = m_kpCameraNode->getAbsoluteTransformation();
	Vector3 cameraPosition = getTranslation3(cameraTransform);
	Matrix44 lhsTransform = _kpLhs->getAbsoluteTransformation();
	Vector3 toLhs = getTranslation3(lhsTransform) - cameraPosition;
	Matrix44 rhsTransform = _kpRhs->getAbsoluteTransformation();
	Vector3 toRhs = getTranslation3(rhsTransform) - cameraPosition;
	float toLhsm = toLhs.getMagnitude();
	float toRhsm = toLhs.getMagnitude();

	return toLhs.getMagnitudeSquared() > toRhs.getMagnitudeSquared();
};

CDepthSortingEngine::CDepthSortingEngine(SimpleTree* _pUnsortedScene, const SimpleTree* _kpCameraNode) :
	m_kpCameraNode(_kpCameraNode),
	m_pRenderingEngine(NULL),
	m_pSortedScene(NULL),
	m_pUnsortedScene(_pUnsortedScene)
{
}

void CDepthSortingEngine::addEntity(Entity* /*entity*/)
{
}

void CDepthSortingEngine::advance()
{
	if (m_pSortedScene != NULL)
	{
		delete m_pSortedScene;
	}
	m_pSortedScene = new SimpleTree;
	m_pRenderingEngine->setTree(m_pSortedScene);

	vector<SimpleTree*> nodes;
	getNodes(*m_pUnsortedScene, nodes);
	CDepthTestComparator comparator(m_kpCameraNode);
	sort(nodes.begin(), nodes.end(), comparator);

	for (unsigned int index = 0; index < nodes.size(); index++)
	{
		SimpleTree* pNode = new SimpleTree;
		pNode->setModel(nodes[index]->getModel());
		pNode->setTransformation(nodes[index]->getAbsoluteTransformation());
		m_pSortedScene->addChild(pNode);
	}
}

void CDepthSortingEngine::destroy()
{
}

void CDepthSortingEngine::getNodes(SimpleTree& _rNode, vector<SimpleTree*>& _rNodes)
{
	if (_rNode.getModel() != NULL)
	{
		_rNodes.push_back(&_rNode);
	}

	for (unsigned int index = 0; index < _rNode.getChildren().size(); index++)
	{
		getNodes(*_rNode.getChildren()[index], _rNodes);
	}
}

void CDepthSortingEngine::init()
{
}

void CDepthSortingEngine::removeEntity(const Entity& /*entity*/)
{
}

void CDepthSortingEngine::setRenderingEngine(Direct3D10RenderingEngine* _pRenderingEngine)
{
	m_pRenderingEngine = _pRenderingEngine;
}
