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
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/model/Plane.h>
#include <gazengine/model/shape/Cube.h>
#include <gazengine/model/shape/Sphere.h>
#include <gazengine/physics/PhysicsFactory.h>
#include <gazengine/physics/PhysXBody.h>

#include "EntityCategories.h"
#include "Scene.h"

using namespace physx;

vector<Entity*> gWallBlocks;

Entity* CreateBall(float _fRadius, SimpleTree& _rParentNode)
{
	Entity* pBall = new Entity(EntityCategories::BALL);

	// The model
	Sphere* pSphere = new Sphere(Vector2(), _fRadius);
	pSphere->setColour(BALL_COLOUR);
	pSphere->setLevelOfDetail(10);
	pBall->addComponent(pSphere);
	pSphere->setEntity(pBall);

	// The physical body
	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	PhysXBody* pBody = static_cast<PhysXBody*>(PhysicsFactory::getInstance()->createBody(material, pSphere,
		BALL_POSITION, true));
	pBall->addComponent(pBody);
	pBody->setEntity(pBall);

	// Collision detection
	PxRigidDynamic* pRigidDynamic = pBody->getActor()->isRigidDynamic();
	PxFilterData filterData;
	filterData.word0 = EntityCategories::BALL;
	filterData.word1 = EntityCategories::WALL_BLOCK;
	PxShape* shapes;
	pRigidDynamic->getShapes(&shapes, 1);
	shapes->setSimulationFilterData(filterData);
	pRigidDynamic->setContactReportThreshold(DESTRUCTION_FORCE_THRESHOLD);

	// The scene
	SimpleTree* pNode = new SimpleTree;
	setTranslation(pNode->getTransformation(), BALL_POSITION);
	pNode->setModel(pSphere);
	pBody->setNode(pNode);
	_rParentNode.addChild(pNode);

	GazEngine::addEntity(pBall);
	return pBall;
}

Entity* CreateFloor(SimpleTree& _rParentNode)
{
	Entity* pFloor = new Entity;

	// The model
	Plane* pPlane = new Plane(Vector3(0.0f, 1.0f, 0.0f), FLOOR_POSITION);
	pFloor->addComponent(pPlane);
	pPlane->setEntity(pFloor);

	// The visible model
	Cube* pVisibleFloor = new Cube(Vector2(), 500.0f);
	pVisibleFloor->setColour(Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	pFloor->addComponent(pVisibleFloor);
	pVisibleFloor->setEntity(pFloor);

	// The physical body
	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pPlane, FLOOR_POSITION, false);
	pFloor->addComponent(pBody);
	pBody->setEntity(pFloor);

	// The scene
	SimpleTree* pNode = new SimpleTree;
	pNode->setModel(pPlane);
	pBody->setNode(pNode);
	_rParentNode.addChild(pNode);

	SimpleTree* pVisibleNode = new SimpleTree;
	getTranslation3(pVisibleNode->getTransformation()) = FLOOR_POSITION + Vector3(0.0f, -500.0f, -500.0f);
	pVisibleNode->setModel(pVisibleFloor);
	_rParentNode.addChild(pVisibleNode);

	GazEngine::addEntity(pFloor);
	return pFloor;
}

void CreateWall(float _fBlockSize, SimpleTree& _rParentNode)
{
	CreateWallLayer(Vector3(2.5f, -30.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(-2.5f, -20.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(2.5f, -10.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(-2.5f, -0.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(2.5f, 10.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(-2.5f, 20.0f, 0.0f), _fBlockSize, _rParentNode);
	CreateWallLayer(Vector3(2.5f, 30.0f, 0.0f), _fBlockSize, _rParentNode);
}

Entity* CreateWallBlock(const Vector3& _rPosition, float _fBlockSize, SimpleTree& _rParentNode)
{
	Matrix44 transformation;
	setTranslation(transformation, _rPosition);

	return CreateWallBlock(transformation, _fBlockSize, _rParentNode);
}

Entity* CreateWallBlock(const Matrix44& _rTransformation, float _fBlockSize, SimpleTree& _rParentNode)
{
	Entity* pWallBlock = new Entity(EntityCategories::WALL_BLOCK);

	// The model
	Cube* pCube = new Cube(Vector2(), _fBlockSize);
	pCube->setColour(Vector4(Math::getRandomFloat(0.0f, 1.0f), Math::getRandomFloat(0.0f, 1.0f),
		Math::getRandomFloat(0.0f, 1.0f), 1.0f));
	pWallBlock->addComponent(pCube);
	pCube->setEntity(pWallBlock);

	// The physical body
	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	PhysXBody* pBody = static_cast<PhysXBody*>(PhysicsFactory::getInstance()->createBody(material, pCube,
		getTranslation3(_rTransformation), true));
	pWallBlock->addComponent(pBody);
	pBody->setEntity(pWallBlock);

	// Collision detection
	PxRigidDynamic* pRigidDynamic = pBody->getActor()->isRigidDynamic();
	PxFilterData filterData;
	filterData.word0 = EntityCategories::WALL_BLOCK;
	filterData.word1 = EntityCategories::BALL | EntityCategories::WALL_BLOCK;
	PxShape* shapes;
	pRigidDynamic->getShapes(&shapes, 1);
	shapes->setSimulationFilterData(filterData);
	pRigidDynamic->setContactReportThreshold(DESTRUCTION_FORCE_THRESHOLD);

	// The scene
	SimpleTree* pNode = new SimpleTree;
	pNode->setTransformation(_rTransformation);
	pNode->setModel(pCube);
	pBody->setNode(pNode);
	_rParentNode.addChild(pNode);

	GazEngine::addEntity(pWallBlock);
	return pWallBlock;
}

void CreateWallLayer(const Vector3& _rPosition, float _fBlockSize, SimpleTree& _rParentNode)
{
	CreateWallBlock(Vector3(-30.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(-20.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(-10.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(0.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(10.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(20.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
	CreateWallBlock(Vector3(30.0f, 0.0f, -DISTANCE_TO_WALL) + _rPosition, _fBlockSize, _rParentNode);
}
