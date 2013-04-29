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

	Model* pModel = ModelFactory::getInstance()->createCubeMesh(5.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pModel->setMaterial(CreateCubeModelMaterial());
	pCube->addComponent(pModel);
	pModel->setEntity(pCube);

	SimpleTree* pNode = new SimpleTree;
	pNode->setTransformation(_rTransformation);
	pNode->setModel(pModel);
	_rRoot.addChild(pNode);

	Body* pBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pModel, _rTransformation, false);
	//static_cast<BulletBody*>(pBody)->setNode(pNode);
	static_cast<PhysXBody*>(pBody)->setNode(pNode);
	pCube->addComponent(pBody);
	pBody->setEntity(pCube);

	GazEngine::addEntity(pCube);
}

Model::Material CreateCubeModelMaterial()
{
	Model::Material material;

	Vector4 colour(0.5f, 0.5f, 0.25f, 1.0f);
	material.ambient = colour * 0.4f;
	material.diffuse = colour * 0.9f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.2f;
	material.specularPower = 0.4f;

	return material;
}

Body::Material CreateObstacleBodyMaterial()
{
	Body::Material material;

	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;

	return material;
}

void CreatePrismRectangle(SimpleTree& _rRoot, const Matrix44& _rTransformation)
{
	Entity* pPrismRectangle = new Entity();
	ModelFactory* pFactory = ModelFactory::getInstance();

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_rTransformation);
	_rRoot.addChild(pRootNode);

	//Model* pSquare = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare = pFactory->createBoxMesh(5.0f, 5.0f, 0.1f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare->setMaterial(CreatePrismRectangleModelMaterial());
	pPrismRectangle->addComponent(pSquare);
	pSquare->setEntity(pPrismRectangle);

	SimpleTree* pSquareNode = new SimpleTree;
	//getTranslation3(pSquareNode->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquareNode->getTransformation()).Y() = 4.9f;
	pSquareNode->setModel(pSquare);
	pRootNode->addChild(pSquareNode);

	Body* pSquareBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pSquare,
		pSquareNode->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquareBody)->setNode(pSquareNode);
	static_cast<PhysXBody*>(pSquareBody)->setNode(pSquareNode);
	pPrismRectangle->addComponent(pSquareBody);
	pSquareBody->setEntity(pPrismRectangle);

	Model* pPrism0 = pFactory->createPrismMesh(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism0->setMaterial(CreatePrismRectangleModelMaterial());
	pPrismRectangle->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangle);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 10.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism0Body)->setNode(pPrism0Node);
	static_cast<PhysXBody*>(pPrism0Body)->setNode(pPrism0Node);
	pPrismRectangle->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangle);

	Model* pPrism1 = pFactory->createPrismMesh(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism1->setMaterial(CreatePrismRectangleModelMaterial());
	pPrismRectangle->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangle);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -10.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism1,
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

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_rTransformation);
	_rRoot.addChild(pRootNode);

	Model* pCube = pFactory->createCubeMesh(5.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	pCube->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrismRectangleCube->addComponent(pCube);
	pCube->setEntity(pPrismRectangleCube);

	SimpleTree* pCubeNode = new SimpleTree;
	pCubeNode->setModel(pCube);
	pRootNode->addChild(pCubeNode);

	Body* pCubeBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pCube,
		pCubeNode->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pCubeBody)->setNode(pCubeNode);
	static_cast<PhysXBody*>(pCubeBody)->setNode(pCubeNode);
	pPrismRectangleCube->addComponent(pCubeBody);
	pCubeBody->setEntity(pPrismRectangleCube);

	//Model* pSquare0 = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare0 = pFactory->createBoxMesh(5.0f, 5.0f, 0.1f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare0->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrismRectangleCube->addComponent(pSquare0);
	pSquare0->setEntity(pPrismRectangleCube);

	SimpleTree* pSquare0Node = new SimpleTree;
	//getTranslation3(pSquare0Node->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquare0Node->getTransformation()).Y() = 4.9f;
	getTranslation3(pSquare0Node->getTransformation()).Z() = 10.0f;
	pSquare0Node->setModel(pSquare0);
	pRootNode->addChild(pSquare0Node);

	Body* pSquare0Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pSquare0,
		pSquare0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquare0Body)->setNode(pSquare0Node);
	static_cast<PhysXBody*>(pSquare0Body)->setNode(pSquare0Node);
	pPrismRectangleCube->addComponent(pSquare0Body);
	pSquare0Body->setEntity(pPrismRectangleCube);

	Model* pPrism0 = pFactory->createPrismMesh(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism0->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrismRectangleCube->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 20.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism0Body)->setNode(pPrism0Node);
	static_cast<PhysXBody*>(pPrism0Body)->setNode(pPrism0Node);
	pPrismRectangleCube->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangleCube);

	//Model* pSquare1 = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare1 = pFactory->createBoxMesh(5.0f, 5.0f, 0.1f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare1->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrismRectangleCube->addComponent(pSquare1);
	pSquare1->setEntity(pPrismRectangleCube);

	SimpleTree* pSquare1Node = new SimpleTree;
	//getTranslation3(pSquare1Node->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquare1Node->getTransformation()).Y() = 4.9f;
	getTranslation3(pSquare1Node->getTransformation()).Z() = -10.0f;
	pSquare1Node->setModel(pSquare1);
	pRootNode->addChild(pSquare1Node);

	Body* pSquare1Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pSquare1,
		pSquare1Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pSquare1Body)->setNode(pSquare1Node);
	static_cast<PhysXBody*>(pSquare1Body)->setNode(pSquare1Node);
	pPrismRectangleCube->addComponent(pSquare1Body);
	pSquare1Body->setEntity(pPrismRectangleCube);

	Model* pPrism1 = pFactory->createPrismMesh(5.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism1->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrismRectangleCube->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -20.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism1,
		pPrism1Node->getAbsoluteTransformation(), false);
	//static_cast<BulletBody*>(pPrism1Body)->setNode(pPrism1Node);
	static_cast<PhysXBody*>(pPrism1Body)->setNode(pPrism1Node);
	pPrismRectangleCube->addComponent(pPrism1Body);
	pPrism1Body->setEntity(pPrismRectangleCube);

	GazEngine::addEntity(pPrismRectangleCube);
}

Model::Material CreatePrismRectangleCubeModelMaterial()
{
	Model::Material material;

	Vector4 colour(0.8f, 0.8f, 0.8f, 1.0f);
	material.ambient = colour * 0.4f;
	material.diffuse = colour * 0.2f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.9f;
	material.specularPower = 0.9f;

	return material;
}

Model::Material CreatePrismRectangleModelMaterial()
{
	Model::Material material;

	Vector4 colour(0.3f, 0.0f, 0.0f, 1.0f);
	material.ambient = colour * 0.4f;
	material.diffuse = colour * 0.9f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.2f;
	material.specularPower = 0.4f;

	return material;
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

Body* CreateTank(SimpleTree& _rRoot, const Matrix44& _rTransformation, SimpleTree* _pChassisCameraNode,
				 SimpleTree* _pBarrelCameraNode, SimpleTree** _pTurretNode, SimpleTree** _pBarrelNode,
				 SimpleTree* _pHeadlight0Node, SimpleTree* _pHeadlight1Node, SimpleTree* _pTurretLightNode)
{
	Entity* pTank = new Entity();

	//Model* pChassis = CreateTankPart(1.5f, 1.0f, 0.75f, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pChassis = CreateTankPart(5.0f, 2.5f, 0.75f, Vector4(0.0f, 0.5f, 0.0f, 1.0f), true);
	pChassis->setMaterial(CreateTankModelMaterial());
	pTank->addComponent(pChassis);
	pChassis->setEntity(pTank);

	SimpleTree* pChassisNode = new SimpleTree;
	pChassisNode->setTransformation(_rTransformation);
	rotate(pChassisNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	pChassisNode->setModel(pChassis);
	_rRoot.addChild(pChassisNode);

	//Model* pTurret = CreateTankPart(0.5f, 0.5f, 0.25f, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pTurret = CreateTankPart(1.5f, 1.0f, 0.5f, Vector4(0.0f, 0.5f, 0.0f, 1.0f), false);
	pTurret->setMaterial(CreateTankModelMaterial());
	pTank->addComponent(pTurret);
	pTurret->setEntity(pTank);

	SimpleTree* pTurretNode = new SimpleTree;
	*_pTurretNode = pTurretNode;
	rotate(pTurretNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//getTranslation3(pTurretNode->getTransformation()).Y() = (0.75f + 0.25f) * -1.0f;
	getTranslation3(pTurretNode->getTransformation()).Y() = (0.75f + 0.5f) * -1.0f;
	pTurretNode->setModel(pTurret);
	pChassisNode->addChild(pTurretNode);

	//Model* pBarrel = ModelFactory::getInstance()->createBoxMesh(1.05f, 0.15f, 0.15f, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pBarrel = ModelFactory::getInstance()->createBoxMesh(2.05f, 0.3f, 0.3f, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	pBarrel->setMaterial(CreateTankModelMaterial());
	pTank->addComponent(pBarrel);
	pBarrel->setEntity(pTank);

	SimpleTree* pBarrelPivotNode = new SimpleTree;
	*_pBarrelNode = pBarrelPivotNode;
	//getTranslation3(pBarrelPivotNode->getTransformation()).Z() = 0.7f;
	getTranslation3(pBarrelPivotNode->getTransformation()).Z() = 1.0f;
	pTurretNode->addChild(pBarrelPivotNode);

	SimpleTree* pBarrelNode = new SimpleTree;
	//getTranslation3(pBarrelNode->getTransformation()).Z() = 1.0f;
	getTranslation3(pBarrelNode->getTransformation()).Z() = 2.0f;
	pBarrelNode->setModel(pBarrel);
	pBarrelPivotNode->addChild(pBarrelNode);

	getTranslation3(_pHeadlight0Node->getTransformation()).X() = -2.0f;
	getTranslation3(_pHeadlight0Node->getTransformation()).Z() = 5.0f;
	pChassisNode->addChild(_pHeadlight0Node);

	getTranslation3(_pHeadlight1Node->getTransformation()).X() = 2.0f;
	getTranslation3(_pHeadlight1Node->getTransformation()).Z() = 5.0f;
	pChassisNode->addChild(_pHeadlight1Node);

	getTranslation3(_pTurretLightNode->getTransformation()).Z() = 0.5f;
	pTurretNode->addChild(_pTurretLightNode);

	Body* pChassisBody = PhysicsFactory::getInstance()->createBody(CreateTankBodyMaterial(), pChassis,
		pChassisNode->getAbsoluteTransformation());
	static_cast<physx::PxRigidDynamic*>(static_cast<PhysXBody*>(pChassisBody)->getActor())->setAngularDamping(0.99f);
	static_cast<physx::PxRigidDynamic*>(static_cast<PhysXBody*>(pChassisBody)->getActor())->setLinearDamping(0.99f);
	static_cast<PhysXBody*>(pChassisBody)->setNode(pChassisNode);
	pTank->addComponent(pChassisBody);
	pChassisBody->setEntity(pTank);

	if (_pChassisCameraNode != NULL)
	{
		pChassisNode->addChild(_pChassisCameraNode);
	}
	if (_pBarrelCameraNode != NULL)
	{
		pBarrelPivotNode->addChild(_pBarrelCameraNode);
	}

	GazEngine::addEntity(pTank);

	return pChassisBody;
}

Body::Material CreateTankBodyMaterial()
{
	Body::Material material;

	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;

	return material;
}

Model::Material CreateTankModelMaterial()
{
	Model::Material material;

	Vector4 colour(0.5f, 0.5f, 0.5f, 1.0f);
	material.ambient = colour * 0.4f;
	material.diffuse = colour * 0.2f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.9f;
	material.specularPower = 0.9f;

	return material;
}

Mesh* CreateTankPart(float length, float width, float height, const Vector4& color, bool extraSlopyFront)
{
	float topLengthBack = length - sqrt(2.0f * pow(height, 2.0f));
	float topLengthFront = length - sqrt(2.0f * pow(height, 2.0f));
	if (extraSlopyFront)
	{
		topLengthFront /= 2.0f;
	}

	// Vertices
	vector<Vertex> vertices(28);
	Vector3 normal;

	// Bottom
	normal = Vector3(0.0f, -1.0f, 0.0f);
	vertices[0].position = Vector3(-width, -height, length);
	vertices[0].color = color;
	vertices[0].normal = normal;
	vertices[1].position = Vector3(width, -height, length);
	vertices[1].color = color;
	vertices[1].normal = normal;
	vertices[2].position = Vector3(width, -height, -topLengthBack);
	vertices[2].color = color;
	vertices[2].normal = normal;
	vertices[3].position = Vector3(-width, -height, -topLengthBack);
	vertices[3].color = color;
	vertices[3].normal = normal;

	// Top
	normal = Vector3(0.0f, 1.0f, 0.0f);
	vertices[4].position = Vector3(width, height, topLengthFront);
	vertices[4].color = color;
	vertices[4].normal = normal;
	vertices[5].position = Vector3(-width, height, topLengthFront);
	vertices[5].color = color;
	vertices[5].normal = normal;
	vertices[6].position = Vector3(-width, height, -topLengthBack);
	vertices[6].color = color;
	vertices[6].normal = normal;
	vertices[7].position = Vector3(width, height, -topLengthBack);
	vertices[7].color = color;
	vertices[7].normal = normal;

	// Front
	normal = Vector3(0.0f, -1.0f, 1.0f);
	normal.normalize();
	vertices[8].position = Vector3(-width, height, topLengthFront);
	vertices[8].color = color;
	vertices[8].normal = normal;
	vertices[9].position = Vector3(width, height, topLengthFront);
	vertices[9].color = color;
	vertices[9].normal = normal;
	vertices[10].position = Vector3(width, -height, length);
	vertices[10].color = color;
	vertices[10].normal = normal;
	vertices[11].position = Vector3(-width, -height, length);
	vertices[11].color = color;
	vertices[11].normal = normal;

	// Back
	normal = Vector3(0.0f, -1.0f, -1.0f);
	normal.normalize();
	vertices[12].position = Vector3(width, height, -topLengthBack);
	vertices[12].color = color;
	vertices[12].normal = normal;
	vertices[13].position = Vector3(-width, height, -topLengthBack);
	vertices[13].color = color;
	vertices[13].normal = normal;
	vertices[14].position = Vector3(-width, -height, -length);
	vertices[14].color = color;
	vertices[14].normal = normal;
	vertices[15].position = Vector3(width, -height, -length);
	vertices[15].color = color;
	vertices[15].normal = normal;

	// Left Side
	normal = Vector3(-1.0f, 0.0f, 0.0f);
	vertices[16].position = Vector3(-width, height, -topLengthBack);
	vertices[16].color = color;
	vertices[16].normal = normal;
	vertices[17].position = Vector3(-width, height, topLengthFront);
	vertices[17].color = color;
	vertices[17].normal = normal;
	vertices[18].position = Vector3(-width, -height, topLengthFront);
	vertices[18].color = color;
	vertices[18].normal = normal;
	vertices[19].position = Vector3(-width, -height, -topLengthBack);
	vertices[19].color = color;
	vertices[19].normal = normal;
	vertices[20].position = Vector3(-width, -height, length);
	vertices[20].color = color;
	vertices[20].normal = normal;
	vertices[21].position = Vector3(-width, -height, -length);
	vertices[21].color = color;
	vertices[21].normal = normal;

	// Right Side
	normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[22].position = Vector3(width, height, topLengthFront);
	vertices[22].color = color;
	vertices[22].normal = normal;
	vertices[23].position = Vector3(width, height, -topLengthBack);
	vertices[23].color = color;
	vertices[23].normal = normal;
	vertices[24].position = Vector3(width, -height, -topLengthBack);
	vertices[24].color = color;
	vertices[24].normal = normal;
	vertices[25].position = Vector3(width, -height, topLengthFront);
	vertices[25].color = color;
	vertices[25].normal = normal;
	vertices[26].position = Vector3(width, -height, -length);
	vertices[26].color = color;
	vertices[26].normal = normal;
	vertices[27].position = Vector3(width, -height, length);
	vertices[27].color = color;
	vertices[27].normal = normal;

	// Indices
	vector<int> indices(48);

	// Bottom
	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	// Top
	indices[6] = 6;
	indices[7] = 5;
	indices[8] = 4;
	indices[9] = 4;
	indices[10] = 7;
	indices[11] = 6;

	// Front
	indices[12] = 10;
	indices[13] = 9;
	indices[14] = 8;
	indices[15] = 8;
	indices[16] = 11;
	indices[17] = 10;

	// Back
	indices[18] = 14;
	indices[19] = 13;
	indices[20] = 12;
	indices[21] = 12;
	indices[22] = 15;
	indices[23] = 14;

	// Left Side
	indices[24] = 18;
	indices[25] = 17;
	indices[26] = 16;
	indices[27] = 16;
	indices[28] = 19;
	indices[29] = 18;

	indices[30] = 20;
	indices[31] = 17;
	indices[32] = 18;

	indices[33] = 21;
	indices[34] = 19;
	indices[35] = 16;

	// Right Side
	indices[36] = 24;
	indices[37] = 23;
	indices[38] = 22;
	indices[39] = 22;
	indices[40] = 25;
	indices[41] = 24;

	indices[42] = 26;
	indices[43] = 23;
	indices[44] = 24;

	indices[45] = 27;
	indices[46] = 25;
	indices[47] = 22;

	return ModelFactory::getInstance()->createMesh(vertices, indices);
}

void CreateWorld(SimpleTree& _rRoot)
{
	Entity* pWorld = new Entity();

	Model* pModel = ModelFactory::getInstance()->createCubeMesh(100.0f, Vector4(1.0f, 0.6f, 1.0f, 1.0f), true);
	pModel->setMaterial(CreateWorldModelMaterial());
	pWorld->addComponent(pModel);
	pModel->setEntity(pWorld);

	SimpleTree* pNode = new SimpleTree;
	getTranslation3(pNode->getTransformation()).Y() = 100.0f;
	pNode->setModel(pModel);
	_rRoot.addChild(pNode);

	Model* pFloorModel = new Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	pWorld->addComponent(pFloorModel);
	pFloorModel->setEntity(pWorld);

	Body* pFloorBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pFloorModel, Matrix44(),
		false);
	pWorld->addComponent(pFloorBody);
	pFloorBody->setEntity(pWorld);

	Model* pNWallModel = new Plane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 100.0f));
	pWorld->addComponent(pNWallModel);
	pNWallModel->setEntity(pWorld);

	Body* pNWallBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pNWallModel, Matrix44(),
		false);
	pWorld->addComponent(pNWallBody);
	pNWallBody->setEntity(pWorld);

	Model* pEWallModel = new Plane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(100.0f, 0.0f, 0.0f));
	pWorld->addComponent(pEWallModel);
	pEWallModel->setEntity(pWorld);

	Body* pEWallBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pEWallModel, Matrix44(),
		false);
	pWorld->addComponent(pEWallBody);
	pEWallBody->setEntity(pWorld);

	Model* pSWallModel = new Plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -100.0f));
	pWorld->addComponent(pSWallModel);
	pSWallModel->setEntity(pWorld);

	Body* pSWallBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pSWallModel, Matrix44(),
		false);
	pWorld->addComponent(pSWallBody);
	pSWallBody->setEntity(pWorld);

	Model* pWWallModel = new Plane(Vector3(1.0f, 0.0f, 0.0f), Vector3(-100.0f, 0.0f, 0.0f));
	pWorld->addComponent(pWWallModel);
	pWWallModel->setEntity(pWorld);

	Body* pWWallBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pWWallModel, Matrix44(),
		false);
	pWorld->addComponent(pWWallBody);
	pWWallBody->setEntity(pWorld);

	GazEngine::addEntity(pWorld);
}

Model::Material CreateWorldModelMaterial()
{
	Model::Material material;

	Vector4 colour(0.3f, 0.3f, 0.3f, 1.0f);
	material.ambient = colour * 1.0f;
	material.diffuse = colour * 1.0f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.0f;
	material.specularPower = 0.0f;

	return material;
}
