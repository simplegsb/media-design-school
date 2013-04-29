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
#include <gazengine/EntityFactory.h>
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/model/ModelFactory.h>
#include <gazengine/model/Plane.h>
#include <gazengine/physics/PhysicsFactory.h>
#include <gazengine/rendering/RenderingFactory.h>

#include "Scene.h"

void CreateCube(const Matrix44& _krTransformation, Texture* _pTexture)
{
	Model* pModel = ModelFactory::getInstance()->createCubeMesh(5.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pModel->setMaterial(CreateCubeModelMaterial());
	pModel->setTexture(_pTexture);
	EntityFactory::createAndAddEntity(_krTransformation, pModel, 0, CreateObstacleBodyMaterial(), false);
}

Model::Material CreateCubeModelMaterial()
{
	Model::Material material;

	Vector4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	//Vector4 colour(0.5f, 0.5f, 0.25f, 1.0f);
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

void CreatePrismRectangle(SimpleTree& _rRoot, const Matrix44& _krTransformation, Texture* _pTexture)
{
	Entity* pPrismRectangle = new Entity();
	ModelFactory* pFactory = ModelFactory::getInstance();

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_krTransformation);
	_rRoot.addChild(pRootNode);

	//Model* pSquare = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare = pFactory->createBoxMesh(Vector3(5.0f, 0.1f, 5.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare->setMaterial(CreatePrismRectangleModelMaterial());
	pSquare->setTexture(_pTexture);
	pPrismRectangle->addComponent(pSquare);
	pSquare->setEntity(pPrismRectangle);

	SimpleTree* pSquareNode = new SimpleTree;
	//getTranslation3(pSquareNode->getTransformation()).Y() = 5.0f;
	getTranslation3(pSquareNode->getTransformation()).Y() = 4.9f;
	pSquareNode->setModel(pSquare);
	pRootNode->addChild(pSquareNode);

	Body* pSquareBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pSquare,
		pSquareNode->getAbsoluteTransformation(), false);
	pSquareBody->setNode(pSquareNode);
	pPrismRectangle->addComponent(pSquareBody);
	pSquareBody->setEntity(pPrismRectangle);

	Model* pPrism0 = pFactory->createPrismMesh(Vector3(5.0f, 5.0f, 5.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism0->setMaterial(CreatePrismRectangleModelMaterial());
	pPrism0->setTexture(_pTexture);
	pPrismRectangle->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangle);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 10.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	pPrism0Body->setNode(pPrism0Node);
	pPrismRectangle->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangle);

	Model* pPrism1 = pFactory->createPrismMesh(Vector3(5.0f, 5.0f, 5.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism1->setMaterial(CreatePrismRectangleModelMaterial());
	pPrism1->setTexture(_pTexture);
	pPrismRectangle->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangle);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -10.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism1,
		pPrism1Node->getAbsoluteTransformation(), false);
	pPrism1Body->setNode(pPrism1Node);
	pPrismRectangle->addComponent(pPrism1Body);
	pPrism1Body->setEntity(pPrismRectangle);

	GazEngine::addEntity(pPrismRectangle);
}

void CreatePrismRectangleCube(SimpleTree& _rRoot, const Matrix44& _krTransformation, Texture* _pTexture)
{
	Entity* pPrismRectangleCube = new Entity();
	ModelFactory* pFactory = ModelFactory::getInstance();

	SimpleTree* pRootNode = new SimpleTree;
	pRootNode->setTransformation(_krTransformation);
	_rRoot.addChild(pRootNode);

	Model* pCube = pFactory->createCubeMesh(5.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	pCube->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pCube->setTexture(_pTexture);
	pPrismRectangleCube->addComponent(pCube);
	pCube->setEntity(pPrismRectangleCube);

	SimpleTree* pCubeNode = new SimpleTree;
	pCubeNode->setModel(pCube);
	pRootNode->addChild(pCubeNode);

	Body* pCubeBody = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pCube,
		pCubeNode->getAbsoluteTransformation(), false);
	pCubeBody->setNode(pCubeNode);
	pPrismRectangleCube->addComponent(pCubeBody);
	pCubeBody->setEntity(pPrismRectangleCube);

	//Model* pSquare0 = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare0 = pFactory->createBoxMesh(Vector3(5.0f, 0.1f, 5.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare0->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pSquare0->setTexture(_pTexture);
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
	pSquare0Body->setNode(pSquare0Node);
	pPrismRectangleCube->addComponent(pSquare0Body);
	pSquare0Body->setEntity(pPrismRectangleCube);

	Model* pPrism0 = pFactory->createPrismMesh(Vector3(5.0f, 5.0f, 5.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism0->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrism0->setTexture(_pTexture);
	pPrismRectangleCube->addComponent(pPrism0);
	pPrism0->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism0Node = new SimpleTree;
	rotate(pPrism0Node->getTransformation(), Math::PI, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	getTranslation3(pPrism0Node->getTransformation()).Z() = 20.0f;
	pPrism0Node->setModel(pPrism0);
	pRootNode->addChild(pPrism0Node);

	Body* pPrism0Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism0,
		pPrism0Node->getAbsoluteTransformation(), false);
	pPrism0Body->setNode(pPrism0Node);
	pPrismRectangleCube->addComponent(pPrism0Body);
	pPrism0Body->setEntity(pPrismRectangleCube);

	//Model* pSquare1 = pFactory->createSquareMesh(5.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	Model* pSquare1 = pFactory->createBoxMesh(Vector3(5.0f, 0.1f, 5.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), true);
	pSquare1->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pSquare1->setTexture(_pTexture);
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
	pSquare1Body->setNode(pSquare1Node);
	pPrismRectangleCube->addComponent(pSquare1Body);
	pSquare1Body->setEntity(pPrismRectangleCube);

	Model* pPrism1 = pFactory->createPrismMesh(Vector3(5.0f, 5.0f, 5.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pPrism1->setMaterial(CreatePrismRectangleCubeModelMaterial());
	pPrism1->setTexture(_pTexture);
	pPrismRectangleCube->addComponent(pPrism1);
	pPrism1->setEntity(pPrismRectangleCube);

	SimpleTree* pPrism1Node = new SimpleTree;
	getTranslation3(pPrism1Node->getTransformation()).Z() = -20.0f;
	pPrism1Node->setModel(pPrism1);
	pRootNode->addChild(pPrism1Node);

	Body* pPrism1Body = PhysicsFactory::getInstance()->createBody(CreateObstacleBodyMaterial(), pPrism1,
		pPrism1Node->getAbsoluteTransformation(), false);
	pPrism1Body->setNode(pPrism1Node);
	pPrismRectangleCube->addComponent(pPrism1Body);
	pPrism1Body->setEntity(pPrismRectangleCube);

	GazEngine::addEntity(pPrismRectangleCube);
}

Model::Material CreatePrismRectangleCubeModelMaterial()
{
	Model::Material material;

	Vector4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	//Vector4 colour(0.8f, 0.8f, 0.8f, 1.0f);
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

	Vector4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	//Vector4 colour(0.3f, 0.0f, 0.0f, 1.0f);
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
	CreatePrismRectangleCube(_rRoot, prismRectangleCube0Transformation,
		RenderingFactory::getInstance()->createTexture("large-bridge.jpg"));

	Matrix44 prismRectangleCube1Transformation;
	rotate(prismRectangleCube1Transformation, Math::PI / 2.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	setTranslation(prismRectangleCube1Transformation, Vector3(20.0f, 5.0f, -85.0f));
	CreatePrismRectangleCube(_rRoot, prismRectangleCube1Transformation,
		RenderingFactory::getInstance()->createTexture("large-bridge.jpg"));

	Matrix44 prismRectangle0Transformation;
	setTranslation(prismRectangle0Transformation, Vector3(-90.0f, 5.0f, -20.0f));
	CreatePrismRectangle(_rRoot, prismRectangle0Transformation,
		RenderingFactory::getInstance()->createTexture("small-bridge.jpg"));

	Matrix44 prismRectangle1Transformation;
	rotate(prismRectangle1Transformation, Math::PI / 2.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	setTranslation(prismRectangle1Transformation, Vector3(-20.0f, 5.0f, 90.0f));
	CreatePrismRectangle(_rRoot, prismRectangle1Transformation,
		RenderingFactory::getInstance()->createTexture("small-bridge.jpg"));

	Matrix44 cubeTransformation;
	setTranslation(cubeTransformation, Vector3(0.0f, 5.0f, 0.0f));
	CreateCube(cubeTransformation, RenderingFactory::getInstance()->createTexture("cube.jpg"));
}

Body* CreateTank(SimpleTree& _rRoot, const Matrix44& _rTransformation, SimpleTree* _pChassisCameraNode,
				 SimpleTree* _pBarrelCameraNode, SimpleTree** _pTurretNode, SimpleTree** _pBarrelNode,
				 SimpleTree* _pHeadlight0Node, SimpleTree* _pHeadlight1Node, SimpleTree* _pTurretLightNode,
				 Texture* _pTexture)
{
	Entity* pTank = new Entity();

	//Model* pChassis = CreateTankPart(Vector3(1.0f, 0.75f, 1.5f), Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pChassis = CreateTankPart(Vector3(2.5f, 0.75f, 5.0f), Vector4(0.0f, 0.5f, 0.0f, 1.0f), true);
	pChassis->setMaterial(CreateTankModelMaterial());
	pChassis->setTexture(_pTexture);
	pTank->addComponent(pChassis);
	pChassis->setEntity(pTank);

	SimpleTree* pChassisNode = new SimpleTree;
	pChassisNode->setTransformation(_rTransformation);
	rotate(pChassisNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	pChassisNode->setModel(pChassis);
	_rRoot.addChild(pChassisNode);

	//Model* pTurret = CreateTankPart(Vector3(0.5f, 0.25f, 0.5f), Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pTurret = CreateTankPart(Vector3(1.0f, 0.5f, 1.5f), Vector4(0.0f, 0.5f, 0.0f, 1.0f), false);
	pTurret->setMaterial(CreateTankModelMaterial());
	pTurret->setTexture(_pTexture);
	pTank->addComponent(pTurret);
	pTurret->setEntity(pTank);

	SimpleTree* pTurretNode = new SimpleTree;
	*_pTurretNode = pTurretNode;
	rotate(pTurretNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//getTranslation3(pTurretNode->getTransformation()).Y() = (0.75f + 0.25f) * -1.0f;
	getTranslation3(pTurretNode->getTransformation()).Y() = (0.75f + 0.5f) * -1.0f;
	pTurretNode->setModel(pTurret);
	pChassisNode->addChild(pTurretNode);

	//Model* pBarrel = ModelFactory::getInstance()->createBoxMesh(Vector3(0.15f, 0.15f, 1.05f), Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	Model* pBarrel = ModelFactory::getInstance()->createBoxMesh(Vector3(0.3f, 0.3f, 2.05f), Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	pBarrel->setMaterial(CreateTankModelMaterial());
	pBarrel->setTexture(_pTexture);
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
	pChassisBody->setNode(pChassisNode);
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

	Vector4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	//Vector4 colour(0.5f, 0.5f, 0.5f, 1.0f);
	material.ambient = colour * 0.4f;
	material.diffuse = colour * 0.2f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.9f;
	material.specularPower = 0.9f;

	return material;
}

Mesh* CreateTankPart(const Vector3& _krHalfExtents, const Vector4& _krColor, bool _bExtraSlopyFront)
{
	float zBack = sqrt(2.0f * pow(_krHalfExtents.Y(), 2.0f));
	float zFront = sqrt(2.0f * pow(_krHalfExtents.Y(), 2.0f));
	if (_bExtraSlopyFront)
	{
		zFront *= 2.0f;
	}

	// Vertices
	vector<Vertex> vertices(36);

	// Top
	ModelFactory::addRectangleVertexList(vertices, 0, _krColor, Vector3(-_krHalfExtents.X(), _krHalfExtents.Y(), _krHalfExtents.Z() - zFront),
		Vector3(_krHalfExtents.X() * 2.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, _krHalfExtents.Z() * -2.0f + zBack + zFront));

	// Bottom
	ModelFactory::addRectangleVertexList(vertices, 4, _krColor, Vector3(-_krHalfExtents.X(), -_krHalfExtents.Y(), -_krHalfExtents.Z()),
		Vector3(_krHalfExtents.X() * 2.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, _krHalfExtents.Z() * 2.0f));

	// Front (North)
	ModelFactory::addRectangleVertexList(vertices, 8, _krColor, Vector3(_krHalfExtents.X(), _krHalfExtents.Y(), _krHalfExtents.Z() - zFront),
		Vector3(_krHalfExtents.X() * -2.0f, 0.0f, 0.0f), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, zFront));

	// Back
	ModelFactory::addRectangleVertexList(vertices, 12, _krColor, Vector3(-_krHalfExtents.X(), _krHalfExtents.Y(), -_krHalfExtents.Z() + zBack),
		Vector3(_krHalfExtents.X() * 2.0f, 0.0f, 0.0f), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, -zBack));

	// Left (West)
	ModelFactory::addRectangleVertexList(vertices, 16, _krColor, Vector3(-_krHalfExtents.X(), _krHalfExtents.Y(), _krHalfExtents.Z() - zFront),
		Vector3(0.0f, 0.0f, _krHalfExtents.Z() * -2.0f + zBack + zFront), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f));

	ModelFactory::addTriangleVertexList(vertices, 20, _krColor, Vector3(-_krHalfExtents.X(), _krHalfExtents.Y(), _krHalfExtents.Z() - zFront),
		Vector3(0.0f, _krHalfExtents.Y() * -2.0f, zFront), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f));

	ModelFactory::addTriangleVertexList(vertices, 23, _krColor, Vector3(-_krHalfExtents.X(), _krHalfExtents.Y(), -_krHalfExtents.Z() + zBack),
		Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, -zBack));

	// Right
	ModelFactory::addRectangleVertexList(vertices, 26, _krColor, Vector3(_krHalfExtents.X(), _krHalfExtents.Y(), -_krHalfExtents.Z() + zBack),
		Vector3(0.0f, 0.0f, _krHalfExtents.Z() * 2.0f - zBack - zFront), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f));

	ModelFactory::addTriangleVertexList(vertices, 30, _krColor, Vector3(_krHalfExtents.X(), _krHalfExtents.Y(), -_krHalfExtents.Z() + zBack),
		Vector3(0.0f, _krHalfExtents.Y() * -2.0f, -zBack), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f));

	ModelFactory::addTriangleVertexList(vertices, 33, _krColor, Vector3(_krHalfExtents.X(), _krHalfExtents.Y(), _krHalfExtents.Z() - zFront),
		Vector3(0.0f, _krHalfExtents.Y() * -2.0f, 0.0f), Vector3(0.0f, _krHalfExtents.Y() * -2.0f, zFront));

	// Indices
	vector<int> indices(48);

	ModelFactory::addRectangleIndexList(indices, 0, 0);
	ModelFactory::addRectangleIndexList(indices, 6, 4);
	ModelFactory::addRectangleIndexList(indices, 12, 8);
	ModelFactory::addRectangleIndexList(indices, 18, 12);
	ModelFactory::addRectangleIndexList(indices, 24, 16);
	ModelFactory::addTriangleIndexList(indices, 30, 20);
	ModelFactory::addTriangleIndexList(indices, 33, 23);
	ModelFactory::addRectangleIndexList(indices, 36, 26);
	ModelFactory::addTriangleIndexList(indices, 42, 30);
	ModelFactory::addTriangleIndexList(indices, 45, 33);

	return ModelFactory::getInstance()->createMesh(vertices, indices);
}

void CreateTankShell(SimpleTree& _rRoot, const Matrix44& _rBarrelTransformation, const Matrix44& _rTransformation)
{
	Entity* pTankShell = new Entity();

	Model* pModel = ModelFactory::getInstance()->createPyramidMesh(0.1f, 1.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	pModel->setMaterial(CreateTankShellModelMaterial());
	pTankShell->addComponent(pModel);
	pModel->setEntity(pTankShell);

	SimpleTree* pNode = new SimpleTree;
	pNode->setTransformation(_rTransformation);
	pNode->setModel(pModel);
	_rRoot.addChild(pNode);

	Body* pBody = PhysicsFactory::getInstance()->createBody(CreateTankShellBodyMaterial(), pModel, _rTransformation);
	Vector3 barrelOut;
	barrelOut = getOut3(_rBarrelTransformation);
	barrelOut.normalize();
	pBody->setLinearVelocity(barrelOut * (300.0f / 5.0f));
	pBody->setNode(pNode);
	pTankShell->addComponent(pBody);
	pBody->setEntity(pTankShell);

	GazEngine::addEntity(pTankShell);
}

Body::Material CreateTankShellBodyMaterial()
{
	Body::Material material;

	material.density = 0.5f;
	material.friction = 0.0f;
	material.restitution = 0.5f;

	return material;
}

Model::Material CreateTankShellModelMaterial()
{
	Model::Material material;

	Vector4 colour(1.0f, 1.0f, 1.0f, 1.0f);
	material.ambient = colour;
	material.diffuse = colour;
	material.emmissive = colour;
	material.specular = colour;
	material.specularPower = 1.0f;

	return material;
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

	Vector4 colour(0.1f, 0.1f, 0.1f, 1.0f);
	material.ambient = colour * 1.0f;
	material.diffuse = colour * 1.0f;
	material.emmissive = colour * 0.0f;
	material.specular = colour * 0.0f;
	material.specularPower = 0.0f;

	return material;
}
