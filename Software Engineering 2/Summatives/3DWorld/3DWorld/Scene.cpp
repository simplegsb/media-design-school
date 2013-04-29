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
#include <gazengine/model/ModelFactory.h>
#include <gazengine/model/Plane.h>
#include <gazengine/physics/BulletBody.h>
#include <gazengine/physics/PhysicsFactory.h>
#include <gazengine/physics/PhysXBody.h>

#include "Scene.h"

void CreateCube(SimpleTree& _rRoot, const Matrix44& _rTransformation)
{
	Entity* pCube = new Entity();
	Body::Material material;
	material.density = 1.0f;
	material.friction = 0.5f;
	material.restitution = 0.5f;

	Model* pModel = ModelFactory::getInstance()->createCube(5.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pCube->addComponent(pModel);
	pModel->setEntity(pCube);

	SimpleTree* pNode = new SimpleTree;
	pNode->setTransformation(_rTransformation);
	pNode->setModel(pModel);
	_rRoot.addChild(pNode);

	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pModel, _rTransformation, false);
	//static_cast<BulletBody*>(pBody)->setNode(pNode);
	static_cast<PhysXBody*>(pBody)->setNode(pNode);
	pCube->addComponent(pBody);
	pBody->setEntity(pCube);

	GazEngine::addEntity(pCube);
}

void CreatePrismRectangle(SimpleTree& _rRoot, const Matrix44& _rTransformation)
{
	Entity* pPrismRectangle = new Entity();
	ModelFactory* pFactory = ModelFactory::getInstance();
	Body::Material material;
	material.density = 1.0f;
	material.friction = 0.5f;
	material.restitution = 0.5f;

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_rTransformation);
	_rRoot.addChild(pRootNode);

	Model* pSquare = pFactory->createSquare(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pPrismRectangle->addComponent(pSquare);
	pSquare->setEntity(pPrismRectangle);

	SimpleTree* pSquareNode = new SimpleTree;
	getTranslation3(pSquareNode->getTransformation()).Y() = 5.0f;
	pSquareNode->setModel(pSquare);
	pRootNode->addChild(pSquareNode);

	Body* pSquareBody = PhysicsFactory::getInstance()->createBody(material, pSquare,
		pSquareNode->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquareBody)->setNode(pSquareNode);
	static_cast<PhysXBody*>(pSquareBody)->setNode(pSquareNode);
	pPrismRectangle->addComponent(pSquareBody);
	pSquareBody->setEntity(pPrismRectangle);

	Model* pPrism0 = pFactory->createPrism(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrismRectangle->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangle);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 10.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(material, pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism0Body)->setNode(pPrism0Node);
	static_cast<PhysXBody*>(pPrism0Body)->setNode(pPrism0Node);
	pPrismRectangle->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangle);

	Model* pPrism1 = pFactory->createPrism(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrismRectangle->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangle);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -10.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(material, pPrism1,
		pPrism1Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism1Body)->setNode(pPrism1Node);
	static_cast<PhysXBody*>(pPrism1Body)->setNode(pPrism1Node);
	pPrismRectangle->addComponent(pPrism1Body);
	pPrism1Body->setEntity(pPrismRectangle);

	GazEngine::addEntity(pPrismRectangle);
}

void CreatePrismRectangleCube(SimpleTree& _rRoot, const Matrix44& _rTransformation)
{
	Entity* pPrismRectangleCube = new Entity();
	ModelFactory* pFactory = ModelFactory::getInstance();
	Body::Material material;
	material.density = 1.0f;
	material.friction = 0.5f;
	material.restitution = 0.5f;

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_rTransformation);
	_rRoot.addChild(pRootNode);

	Model* pCube = pFactory->createCube(5.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	pPrismRectangleCube->addComponent(pCube);
	pCube->setEntity(pPrismRectangleCube);

	SimpleTree* pCubeNode = new SimpleTree;
	pCubeNode->setModel(pCube);
	pRootNode->addChild(pCubeNode);

	Body* pCubeBody = PhysicsFactory::getInstance()->createBody(material, pCube,
		pCubeNode->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pCubeBody)->setNode(pCubeNode);
	static_cast<PhysXBody*>(pCubeBody)->setNode(pCubeNode);
	pPrismRectangleCube->addComponent(pCubeBody);
	pCubeBody->setEntity(pPrismRectangleCube);

	Model* pSquare0 = pFactory->createSquare(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pPrismRectangleCube->addComponent(pSquare0);
	pSquare0->setEntity(pPrismRectangleCube);

	SimpleTree* pSquare0Node = new SimpleTree;
	getTranslation3(pSquare0Node->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquare0Node->getTransformation()).Z() = 10.0f;
	pSquare0Node->setModel(pSquare0);
	pRootNode->addChild(pSquare0Node);

	Body* pSquare0Body = PhysicsFactory::getInstance()->createBody(material, pSquare0,
		pSquare0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquare0Body)->setNode(pSquare0Node);
	static_cast<PhysXBody*>(pSquare0Body)->setNode(pSquare0Node);
	pPrismRectangleCube->addComponent(pSquare0Body);
	pSquare0Body->setEntity(pPrismRectangleCube);

	Model* pPrism0 = pFactory->createPrism(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrismRectangleCube->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 20.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(material, pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism0Body)->setNode(pPrism0Node);
	static_cast<PhysXBody*>(pPrism0Body)->setNode(pPrism0Node);
	pPrismRectangleCube->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangleCube);

	Model* pSquare1 = pFactory->createSquare(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pPrismRectangleCube->addComponent(pSquare1);
	pSquare1->setEntity(pPrismRectangleCube);

	SimpleTree* pSquare1Node = new SimpleTree;
	getTranslation3(pSquare1Node->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquare1Node->getTransformation()).Z() = -10.0f;
	pSquare1Node->setModel(pSquare1);
	pRootNode->addChild(pSquare1Node);

	Body* pSquare1Body = PhysicsFactory::getInstance()->createBody(material, pSquare1,
		pSquare1Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquare1Body)->setNode(pSquare1Node);
	static_cast<PhysXBody*>(pSquare1Body)->setNode(pSquare1Node);
	pPrismRectangleCube->addComponent(pSquare1Body);
	pSquare1Body->setEntity(pPrismRectangleCube);

	Model* pPrism1 = pFactory->createPrism(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrismRectangleCube->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -20.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(material, pPrism1,
		pPrism1Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism1Body)->setNode(pPrism1Node);
	static_cast<PhysXBody*>(pPrism1Body)->setNode(pPrism1Node);
	pPrismRectangleCube->addComponent(pPrism1Body);
	pPrism1Body->setEntity(pPrismRectangleCube);

	GazEngine::addEntity(pPrismRectangleCube);
}

void CreateShapes(SimpleTree& _rRoot)
{
	Matrix44 prismRectangleCube0Transformation;
	setTranslation(prismRectangleCube0Transformation, Vector3(85.0f, 5.0f, 20.0f));
	CreatePrismRectangleCube(_rRoot, prismRectangleCube0Transformation);

	Matrix44 prismRectangleCube1Transformation;
	rotate(prismRectangleCube1Transformation, Math::PI / 2.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	setTranslation(prismRectangleCube1Transformation, Vector3(20.0f, 5.0f, -85.0f));
	CreatePrismRectangleCube(_rRoot, prismRectangleCube1Transformation);

	Matrix44 prismRectangle0Transformation;
	setTranslation(prismRectangle0Transformation, Vector3(-90.0f, 5.0f, -20.0f));
	CreatePrismRectangle(_rRoot, prismRectangle0Transformation);

	Matrix44 prismRectangle1Transformation;
	rotate(prismRectangle1Transformation, Math::PI / 2.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	setTranslation(prismRectangle1Transformation, Vector3(-20.0f, 5.0f, 90.0f));
	CreatePrismRectangle(_rRoot, prismRectangle1Transformation);

	Matrix44 cubeTransformation;
	setTranslation(cubeTransformation, Vector3(0.0f, 5.0f, 0.0f));
	CreateCube(_rRoot, cubeTransformation);
}

void CreateWorld(SimpleTree& _rRoot)
{
	Entity* pWorld = new Entity();

	Model* pModel = ModelFactory::getInstance()->createCube(100.0f, Vector4(1.0f, 0.6f, 1.0f, 1.0f), true);
	pWorld->addComponent(pModel);
	pModel->setEntity(pWorld);

	SimpleTree* pNode = new SimpleTree;
	getTranslation3(pNode->getTransformation()).Y() = 100.0f;
	pNode->setModel(pModel);
	_rRoot.addChild(pNode);

	/*Model* pPhysicsModel = new Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	pWorld->addComponent(pModel);
	pModel->setEntity(pWorld);

	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pPhysicsModel, Matrix44), false);
	//static_cast<BulletBody*>(pBody)->setNode(pPhysicsNode);
	static_cast<PhysXBody*>(pBody)->setNode(pPhysicsNode);
	pWorld->addComponent(pBody);
	pBody->setEntity(pWorld);

	SimpleTree* pPhysicsNode = new SimpleTree;
	_rRoot.addChild(pPhysicsNode);*/

	GazEngine::addEntity(pWorld);
}
