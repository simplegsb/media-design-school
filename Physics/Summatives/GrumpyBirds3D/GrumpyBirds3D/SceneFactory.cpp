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
#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/model/ModelFactory.h>
#include <gazengine/model/Plane.h>
#include <gazengine/model/shape/Sphere.h>
#include <gazengine/physics/PhysicsFactory.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>

#include "EntityCategories.h"
#include "SceneFactory.h"

using namespace physx;

namespace SceneFactory
{
	Entity* CreateBird(const Matrix44& _krTransformation, unsigned short _usCategory, float _fRadius)
	{
		Entity* pBird = new Entity(_usCategory);

		// The model
		Sphere* pSphere = new Sphere(Vector2(), _fRadius);
		pSphere->setLevelOfDetail(10);
		pBird->addComponent(pSphere);
		pSphere->setEntity(pBird);

		// The physical body
		Body::Material material;
		material.density = 5.0f;
		material.friction = 0.5f;
		material.restitution = 0.5f;
		Body* pBody = PhysicsFactory::getInstance()->createBody(material, pSphere, _krTransformation, true);
		pBird->addComponent(pBody);
		pBody->setEntity(pBird);

		// The scene
		SimpleTree* pNode = new SimpleTree;
		pNode->setTransformation(_krTransformation);
		pNode->setModel(pSphere);
		pBody->setNode(pNode);

		// The timer
		Componentizer<float>* pTimer = new Componentizer<float>();
		pTimer->setValue(0.0f);
		pBird->addComponent(pTimer);
		pTimer->setEntity(pBird);

		// Slung flag
		Componentizer<bool>* pSlung = new Componentizer<bool>();
		pSlung->setValue(false);
		pBird->addComponent(pSlung);
		pSlung->setEntity(pBird);

		GazEngine::addEntity(pBird);
		GazEngine::addToWorld(pNode);

		return pBird;
	}

	PhysXBody* CreateBox(SimpleTree* _pParentNode, const Matrix44& _krTransformation, const Vector3& _krHalfExtents,
		unsigned short _usCategory, bool _bCreateBody, bool _bDynamic)
	{
		Entity* pBox = new Entity(_usCategory);

		// The model
		Model* pModel = ModelFactory::getInstance()->createBoxMesh(_krHalfExtents.Z(), _krHalfExtents.X(),
			_krHalfExtents.Y(), GetColour(_usCategory));
		pBox->addComponent(pModel);
		pModel->setEntity(pBox);

		Body* pBody = NULL;
		if (_bCreateBody)
		{
			Matrix44 absoluteTransformation;
			if (_pParentNode == NULL)
			{
				absoluteTransformation = _krTransformation;
			}
			else
			{
				absoluteTransformation = _pParentNode->getAbsoluteTransformation() * _krTransformation;
			}

			// The physical body
			pBody = PhysicsFactory::getInstance()->createBody(CreateMaterial(_usCategory), pModel,
				absoluteTransformation, _bDynamic);
			pBox->addComponent(pBody);
			pBody->setEntity(pBox);

			// Collision detection
			float fDestructionForceThreshold = GetDestructionForceThreshold(_usCategory);
			if (fDestructionForceThreshold != 0.0f)
			{
				physx::PxRigidDynamic* pRigidDynamic = static_cast<PhysXBody*>(pBody)->getActor()->isRigidDynamic();
				if (pRigidDynamic != NULL)
				{
					physx::PxFilterData filterData;
					filterData.word0 = _usCategory;
					physx::PxShape* shape;
					pRigidDynamic->getShapes(&shape, 1);
					shape->setSimulationFilterData(filterData);
					pRigidDynamic->setContactReportThreshold(fDestructionForceThreshold);
				}
			}
		}

		// The scene
		SimpleTree* pNode = new SimpleTree;
		pNode->setTransformation(_krTransformation);
		pNode->setModel(pModel);
		if (_bCreateBody)
		{
			pBody->setNode(pNode);
		}

		if (_pParentNode == NULL)
		{
			GazEngine::addToWorld(pNode);
		}
		else
		{
			_pParentNode->addChild(pNode);
		}

		GazEngine::addEntity(pBox);

		return static_cast<PhysXBody*>(pBody);
	}

	void CreateFence(const Matrix44& _krTransformation, unsigned int _uiSegmentCount, bool _bWithGate)
	{
		float fCrossbarOffset = 2.5f;
		float fHalfPlankWidth = 0.5f;
		float fHalfSegmentHeight = 4.0f;
		float fHalfSegmentWidth = 6.0f;

		Entity* pConstraintEntity = new Entity;

		Matrix44 transformation = _krTransformation;
		PhysXBody* pLeftUpright = CreateBox(NULL, transformation, Vector3(fHalfPlankWidth, fHalfSegmentHeight, fHalfPlankWidth),
			EntityCategories::WOOD_FRAGMENT);

		if (_bWithGate)
		{
			getTranslation3(transformation).X() -= (fHalfSegmentWidth + fHalfPlankWidth) * 2 + 1.5f;
			PhysXBody* pGateLeftUpright = CreateBox(NULL, transformation, Vector3(fHalfPlankWidth, fHalfSegmentHeight, fHalfPlankWidth),
			EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).X() += fHalfSegmentWidth + fHalfPlankWidth;
			PhysXBody* pGateMiddleCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).Y() += fCrossbarOffset;
			PhysXBody* pGateTopCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).Y() -= fCrossbarOffset * 2.0f;
			PhysXBody* pGateBottomCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).X() += fHalfSegmentWidth + fHalfPlankWidth;
			getTranslation3(transformation).Y() += fCrossbarOffset;
			PhysXBody* pGateRightUpright = CreateBox(NULL, transformation, Vector3(fHalfPlankWidth, fHalfSegmentHeight, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateLeftUpright, pGateMiddleCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateLeftUpright, pGateTopCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateLeftUpright, pGateBottomCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateRightUpright, pGateMiddleCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateRightUpright, pGateTopCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pGateRightUpright, pGateBottomCrossbar, Matrix44()));

			PxTransform gateToJoint;
			gateToJoint.p = PxVec3(0.5f, 3.0f, 0.0f);
			gateToJoint.q = PxQuat(1.0f, 0.0f, 0.0f, 0.0f);
			PxTransform fenceToJoint = gateToJoint;
			fenceToJoint.p.x *= -1.0f;

			PxPhysics* pPhysics = static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())->getPhysics();

			PxRevoluteJoint* pGateUpperHinge = PxRevoluteJointCreate(*pPhysics,
				pGateRightUpright->getActor()->isRigidActor(), gateToJoint,
				pLeftUpright->getActor()->isRigidActor(), fenceToJoint);
			pGateUpperHinge->setBreakForce(100000.0f, 100000.0f);
			pGateUpperHinge->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);

			gateToJoint.p.y *= -1.0f;
			fenceToJoint.p.y *= -1.0f;

			PxRevoluteJoint* pGateLowerHinge = PxRevoluteJointCreate(*pPhysics,
				pGateRightUpright->getActor()->isRigidActor(), gateToJoint,
				pLeftUpright->getActor()->isRigidActor(), fenceToJoint);
			pGateLowerHinge->setBreakForce(100000.0f, 100000.0f);
			pGateLowerHinge->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);

			getTranslation3(transformation).X() += 1.5f;
		}

		getTranslation3(transformation).X() += (fHalfSegmentWidth + fHalfPlankWidth) * _uiSegmentCount;
		getTranslation3(transformation).Y() -= fHalfSegmentHeight + fHalfPlankWidth;
		PhysXBody* pGround = CreateBox(NULL, transformation, Vector3((fHalfSegmentWidth + fHalfPlankWidth) * _uiSegmentCount + fHalfPlankWidth, 0.5f, 5.0f),
			EntityCategories::GROUND);
		getTranslation3(transformation).X() -= (fHalfSegmentWidth + fHalfPlankWidth) * _uiSegmentCount;
		getTranslation3(transformation).Y() += fHalfSegmentHeight + fHalfPlankWidth;

		pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
			->createFixedConstraint(pLeftUpright, pGround, Matrix44()));

		for (unsigned int uiSegmentIndex = 0; uiSegmentIndex < _uiSegmentCount; uiSegmentIndex++)
		{
			getTranslation3(transformation).X() += fHalfSegmentWidth + fHalfPlankWidth;
			PhysXBody* pMiddleCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).Y() += fCrossbarOffset;
			PhysXBody* pTopCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).Y() -= fCrossbarOffset * 2.0f;
			PhysXBody* pBottomCrossbar = CreateBox(NULL, transformation, Vector3(fHalfSegmentWidth, fHalfPlankWidth, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			getTranslation3(transformation).X() += fHalfSegmentWidth + fHalfPlankWidth;
			getTranslation3(transformation).Y() += fCrossbarOffset;
			PhysXBody* pRightUpright = CreateBox(NULL, transformation, Vector3(fHalfPlankWidth, fHalfSegmentHeight, fHalfPlankWidth),
				EntityCategories::WOOD_FRAGMENT);

			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pLeftUpright, pMiddleCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pLeftUpright, pTopCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pLeftUpright, pBottomCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pRightUpright, pMiddleCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pRightUpright, pTopCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pRightUpright, pBottomCrossbar, Matrix44()));
			pConstraintEntity->addComponent(static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())
				->createFixedConstraint(pRightUpright, pGround, Matrix44()));

			pLeftUpright = pRightUpright;
		}

		GazEngine::addEntity(pConstraintEntity);
	}

	PhysXBody* CreateGround(float _fAltitude)
	{
		Entity* pFloor = new Entity(EntityCategories::GROUND);
		Vector3 floorPosition(0.0f, _fAltitude, 0.0f);
		Matrix44 transformation;
		setTranslation(transformation, floorPosition);

		// The model
		Model* pPlane = new Plane(Vector3(0.0f, 1.0f, 0.0f), floorPosition);
		pFloor->addComponent(pPlane);
		pPlane->setEntity(pFloor);

		// The visible model
		Model* pVisibleFloor = ModelFactory::getInstance()->createSquareMesh(1000.0f, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
		pFloor->addComponent(pVisibleFloor);
		pVisibleFloor->setEntity(pFloor);

		// The physical body
		Body* pBody = PhysicsFactory::getInstance()->createBody(CreateMaterial(EntityCategories::GROUND), pPlane,
			transformation, false);
		pFloor->addComponent(pBody);
		pBody->setEntity(pFloor);

		// The scene
		SimpleTree* pNode = new SimpleTree;
		pNode->setTransformation(transformation);
		pNode->setModel(pPlane);
		pBody->setNode(pNode);

		SimpleTree* pVisibleNode = new SimpleTree;
		pVisibleNode->setTransformation(transformation);
		pVisibleNode->setModel(pVisibleFloor);

		GazEngine::addToWorld(pNode);
		GazEngine::addToWorld(pVisibleNode);
		GazEngine::addEntity(pFloor);

		return static_cast<PhysXBody*>(pBody);
	}

	void CreateHouseOneStory(const Matrix44& _krTransformation)
	{
		Matrix44 temp = _krTransformation;

		CreateHouseStory(*GazEngine::getWorldRepresentations()[0], temp);
		getTranslation3(temp).Y() += 12.0f;
		CreateHouseRoof(*GazEngine::getWorldRepresentations()[0], temp);
	}

	void CreateHouseRoof(SimpleTree& _rParentNode, const Matrix44& _krTransformation)
	{
		CreateBox(&_rParentNode, _krTransformation, Vector3(12.0f, 1.0f, 12.0f), EntityCategories::STONE);
	}

	void CreateHouseStory(SimpleTree& _rParentNode, const Matrix44& _krTransformation)
	{
		Matrix44 temp = _krTransformation;

		// Floor
		setTranslation(temp, getTranslation3(_krTransformation));
		CreateBox(&_rParentNode, temp, Vector3(10.0f, 1.0f, 10.0f), EntityCategories::STONE);

		// Walls
		setTranslation(temp, getTranslation3(_krTransformation));
		getTranslation3(temp).X() -= 9.0f;
		getTranslation3(temp).Y() += 6.0f;
		getTranslation3(temp).Z() += 9.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::STONE);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::STONE);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::STONE);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).X() -= 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::STONE);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
		getTranslation3(temp).Z() += 2.0f;
		CreatePillar(_rParentNode, temp, EntityCategories::WOOD);
	}

	void CreateHouseTwoStory(const Matrix44& _krTransformation)
	{
		Matrix44 temp = _krTransformation;

		CreateHouseStory(*GazEngine::getWorldRepresentations()[0], temp);
		getTranslation3(temp).Y() += 12.0f;
		CreateHouseStory(*GazEngine::getWorldRepresentations()[0], temp);
		getTranslation3(temp).Y() += 12.0f;
		CreateHouseRoof(*GazEngine::getWorldRepresentations()[0], temp);
	}

	Body::Material CreateMaterial(unsigned short _usCategory)
	{
		Body::Material material;

		if (_usCategory == EntityCategories::BIRD)
		{
			material.density = 4.0f;
			material.friction = 0.5f;
			material.restitution = 0.01f;
		}
		else if (_usCategory == EntityCategories::STONE)
		{
			material.density = 2.0f;
			material.friction = 0.5f;
			material.restitution = 0.01f;
		}
		else if (_usCategory == EntityCategories::WOOD)
		{
			material.density = 2.0f;
			material.friction = 0.5f;
			material.restitution = 0.02f;
		}
		else
		{
			material.density = 0.5f;
			material.friction = 0.5f;
			material.restitution = 0.02f;
		}

		return material;
	}

	void CreatePig(const Matrix44& _krTransformation, float _fRadius)
	{
		Entity* pPig = new Entity(EntityCategories::PIG);

		// The model
		Sphere* pSphere = new Sphere(Vector2(), _fRadius);
		pSphere->setColour(Vector4(0.0f, 0.4f, 0.0f, 1.0f));
		pSphere->setLevelOfDetail(10);
		pPig->addComponent(pSphere);
		pSphere->setEntity(pPig);

		// The physical body
		Body::Material material;
		material.density = 2.0f;
		material.friction = 0.5f;
		material.restitution = 0.5f;
		Body* pBody = PhysicsFactory::getInstance()->createBody(material, pSphere, _krTransformation);
		pPig->addComponent(pBody);
		pBody->setEntity(pPig);

		// Collision detection
		physx::PxRigidDynamic* pRigidDynamic = static_cast<PhysXBody*>(pBody)->getActor()->isRigidDynamic();
		physx::PxFilterData filterData;
		filterData.word0 = EntityCategories::PIG;
		physx::PxShape* shapes;
		pRigidDynamic->getShapes(&shapes, 1);
		shapes->setSimulationFilterData(filterData);
		pRigidDynamic->setContactReportThreshold(5000.0f);

		// The scene
		SimpleTree* pNode = new SimpleTree;
		pNode->setTransformation(_krTransformation);
		pNode->setModel(pSphere);
		pBody->setNode(pNode);

		GazEngine::addToWorld(pNode);
		GazEngine::addEntity(pPig);
	}

	void CreatePillar(SimpleTree& _rParentNode, Matrix44 _transformation, unsigned short _usCategory)
	{
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 5.0f, 1.0f), _usCategory);

		/* Too slow!
		getTranslation3(_transformation).Y() -= 4.0f;
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 1.0f, 1.0f), _usCategory);
		getTranslation3(_transformation).Y() += 2.0f;
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 1.0f, 1.0f), _usCategory);
		getTranslation3(_transformation).Y() += 2.0f;
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 1.0f, 1.0f), _usCategory);
		getTranslation3(_transformation).Y() += 2.0f;
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 1.0f, 1.0f), _usCategory);
		getTranslation3(_transformation).Y() += 2.0f;
		CreateBox(&_rParentNode, _transformation, Vector3(1.0f, 1.0f, 1.0f), _usCategory);*/
	}

	void CreateScene()
	{
		PhysXBody* pGroundBody = CreateGround(-10.0f);

		Body::Material wood;
		wood.density = 0.5f;
		wood.friction = 0.5f;
		wood.restitution = 0.5f;

		Matrix44 transformation;

		setTranslation(transformation, Vector3(0.0f, -5.0f, -30.0f));
		CreateFence(transformation, 3, true);

		setTranslation(transformation, Vector3(-15.0f, -9.0f, -60.0f));
		//rotate(transformation, Math::PI * 0.4f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		CreateHouseTwoStory(transformation);

		setTranslation(transformation, Vector3(15.0f, -9.0f, -60.0f));
		//rotate(transformation, Math::PI * 0.4f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		CreateHouseOneStory(transformation);
	}

	PhysXBody* CreateSphere(SimpleTree* _pParentNode, const Matrix44& _krTransformation, unsigned short _usCategory,
		float _fRadius)
	{
		Entity* pSphere = new Entity(_usCategory);

		// The model
		Sphere* pModel = new Sphere(Vector2(), _fRadius);
		pModel->setColour(GetColour(_usCategory));
		pModel->setLevelOfDetail(10);
		pSphere->addComponent(pModel);
		pModel->setEntity(pSphere);

		// The physical body
		Matrix44 absoluteTransformation;
		if (_pParentNode == NULL)
		{
			absoluteTransformation = _krTransformation;
		}
		else
		{
			absoluteTransformation = _pParentNode->getAbsoluteTransformation() * _krTransformation;
		}

		Body::Material material;
		material.density = 2.0f;
		material.friction = 0.5f;
		material.restitution = 0.5f;
		Body* pBody = PhysicsFactory::getInstance()->createBody(material, pModel, absoluteTransformation, false);
		pSphere->addComponent(pBody);
		pBody->setEntity(pSphere);

		// The scene
		SimpleTree* pNode = new SimpleTree;
		pNode->setTransformation(_krTransformation);
		pNode->setModel(pModel);
		pBody->setNode(pNode);

		GazEngine::addEntity(pSphere);

		if (_pParentNode == NULL)
		{
			GazEngine::addToWorld(pNode);
		}
		else
		{
			_pParentNode->addChild(pNode);
		}

		return static_cast<PhysXBody*>(pBody);
	}

	Vector4 GetColour(unsigned short _usCategory)
	{
		if (_usCategory == EntityCategories::GROUND)
		{
			return Vector4(0.0f, 0.5f, 0.0f, 1.0f);
		}
		else if (_usCategory == EntityCategories::STONE)
		{
			return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else if (_usCategory == EntityCategories::SLINGSHOT)
		{
			return Vector4(0.75f, 0.75f, 0.75f, 1.0f);
		}
		else if (_usCategory == EntityCategories::WOOD ||
			_usCategory == EntityCategories::WOOD_FRAGMENT)
		{
			return Vector4(0.4f, 0.1f, 0.0f, 1.0f);
		}

		return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	float GetDestructionForceThreshold(unsigned short _usCategory)
	{
		if (_usCategory == EntityCategories::WOOD)
		{
			return 6000.0f;
		}

		return 0.0f;
	}
}
