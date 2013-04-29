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
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/input/MouseMoveEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/math/PhysXMatrix.h>
#include <gazengine/math/PhysXVector.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>

#include "EntityCategories.h"
#include "Events.h"
#include "SceneFactory.h"
#include "SlingshotEngine.h"

using namespace physx;

CSlingshotEngine::CSlingshotEngine() :
	m_bAiming(false),
	m_bPlaying(false),
	m_fPitch(0.0f),
	m_fPull(0.0f),
	m_iMouseX(0),
	m_iMouseY(0),
	m_pArm0Body(NULL),
	m_pArm0Joint(NULL),
	m_pArm1Body(NULL),
	m_pArm1Joint(NULL),
	m_pBirdBody(NULL),
	m_pSlingshotBaseNode(NULL),
	m_uiBirdCount(10)
{
}

void CSlingshotEngine::addEntity(Entity* /*_pEntity*/)
{
}

void CSlingshotEngine::advance()
{
	if (m_bPlaying && m_pBirdBody != NULL)
	{
		Matrix44 transformation = m_pSlingshotBaseNode->getTransformation();
		rotate(transformation, Math::PI * m_fPitch * 0.001f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		Vector3 out;
		out = getOut3(transformation);
		out.normalize();
		m_pBirdBody->applyForce(out * m_fPull * 100.0f);
	}

	if (m_bPlaying && m_pBirdBody != NULL && getTranslation3(m_pBirdBody->getTransformation()).Z() < -16.0f)
	{
		m_pArm0Joint->release();
		m_pArm1Joint->release();
		Messages::send(Events::BIRD_RELEASED, NULL);
		m_uiBirdCount--;

		m_pBirdBody->getEntity()->getSingleComponent<Componentizer<bool> >()->setValue(true);

		if (m_uiBirdCount > 0)
		{
			loadSlingshot();
		}
		else
		{
			m_pBirdBody = NULL;
		}
	}
}

void CSlingshotEngine::buildSlingshot()
{
	Matrix44 transformation;
	setTranslation(transformation, Vector3(0.0f, -8.0f, -15.0f));

	SceneFactory::CreateBox(NULL, transformation, Vector3(1.0f, 2.0f, 1.0f), EntityCategories::SLINGSHOT, false);
	m_pSlingshotBaseNode = GazEngine::getWorldRepresentations()[0]->getChildren().back();

	transformation.setIdentity();
	setTranslation(transformation, Vector3(-1.6f, 3.3f, 0.0f));
	rotate(transformation, Math::PI * 0.2f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	SceneFactory::CreateBox(m_pSlingshotBaseNode, transformation, Vector3(0.5f, 2.0f, 0.5f),
		EntityCategories::SLINGSHOT, false);
	SimpleTree* pArmNode = m_pSlingshotBaseNode->getChildren().back();

	transformation.setIdentity();
	setTranslation(transformation, Vector3(0.0f, 2.0f, 0.0f));
	m_pArm0Body = SceneFactory::CreateSphere(pArmNode, transformation, EntityCategories::SLINGSHOT, 1.0f);

	transformation.setIdentity();
	setTranslation(transformation, Vector3(1.6f, 3.3f, 0.0f));
	rotate(transformation, Math::PI * -0.2f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	SceneFactory::CreateBox(m_pSlingshotBaseNode, transformation, Vector3(0.5f, 2.0f, 0.5f),
		EntityCategories::SLINGSHOT, false);
	pArmNode = m_pSlingshotBaseNode->getChildren().back();

	transformation.setIdentity();
	setTranslation(transformation, Vector3(0.0f, 2.0f, 0.0f));
	m_pArm1Body = SceneFactory::CreateSphere(pArmNode, transformation, EntityCategories::SLINGSHOT, 1.0f);
}

void CSlingshotEngine::destroy()
{
	Messages::deregisterRecipient(Events::GAME_START, this);
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
	Messages::deregisterRecipient(Events::MOUSE_MOVE, this);
}

void CSlingshotEngine::init()
{
	Messages::registerRecipient(Events::GAME_START, this);
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
	Messages::registerRecipient(Events::MOUSE_MOVE, this);
}

void CSlingshotEngine::loadSlingshot()
{
	Matrix44 transformation2;
	setTranslation(transformation2, Vector3(0.0f, -2.5f, -15.0f));
	Entity* pBird = SceneFactory::CreateBird(transformation2, EntityCategories::BIRD, 1.0f);

	m_pBirdBody = pBird->getSingleComponent<PhysXBody>();
	PxRigidDynamic* pBirdDynamic = m_pBirdBody->getActor()->isRigidDynamic();

	Vector3 arm0Position = PhysXVector::toVector3(m_pArm0Body->getActor()->isRigidActor()->getGlobalPose().p);
	Vector3 arm1Position = PhysXVector::toVector3(m_pArm1Body->getActor()->isRigidActor()->getGlobalPose().p);
	Vector3 birdPosition = PhysXVector::toVector3(pBirdDynamic->getGlobalPose().p);
	Vector3 arm0ToBird = birdPosition - arm0Position;
	Matrix44 arm0LocalFrame;
	setTranslation(arm0LocalFrame, arm0ToBird * 0.5f);
	Vector3 arm1ToBird = birdPosition - arm1Position;
	Matrix44 arm1LocalFrame;
	setTranslation(arm1LocalFrame, arm1ToBird * 0.5f);
	Matrix44 birdLocalFrame;
	setTranslation(birdLocalFrame, arm0ToBird * -0.5f);

	PxPhysics* pPhysics = static_cast<PhysXPhysicsFactory*>(PhysicsFactory::getInstance())->getPhysics();

	m_pArm0Joint = PxD6JointCreate(*pPhysics, m_pArm0Body->getActor()->isRigidActor(),
		PhysXMatrix::toPxTransform(arm0LocalFrame), pBirdDynamic, PhysXMatrix::toPxTransform(birdLocalFrame));

	setTranslation(birdLocalFrame, arm1ToBird * -0.5f);

	m_pArm1Joint = PxD6JointCreate(*pPhysics, m_pArm1Body->getActor()->isRigidActor(),
		PhysXMatrix::toPxTransform(arm1LocalFrame), pBirdDynamic, PhysXMatrix::toPxTransform(birdLocalFrame));

	m_pArm0Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	m_pArm0Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	m_pArm0Joint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);
	m_pArm1Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	m_pArm1Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	m_pArm1Joint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);

	PxD6JointDrive drive(500.0f, -20.0f, PX_MAX_F32);
	m_pArm0Joint->setDrive(PxD6Drive::eX, drive);
	m_pArm0Joint->setDrive(PxD6Drive::eY, drive);
	m_pArm0Joint->setDrive(PxD6Drive::eZ, drive);
	m_pArm1Joint->setDrive(PxD6Drive::eX, drive);
	m_pArm1Joint->setDrive(PxD6Drive::eY, drive);
	m_pArm1Joint->setDrive(PxD6Drive::eZ, drive);
}

void CSlingshotEngine::receive(unsigned short _usSubject, const void* _kpMessage)
{
	if (_usSubject == Events::GAME_START)
	{
		buildSlingshot();
		loadSlingshot();
		m_bAiming = false;
		m_bPlaying = true;
		m_fPitch = 0.0f;
		m_fPull = 0.0f;
		m_uiBirdCount = 10;
		return;
	}

	if (!m_bPlaying)
	{
		return;
	}

	if (_usSubject == Events::MOUSE_BUTTON)
	{
		const MouseButtonEvent* kpEvent = static_cast<const MouseButtonEvent*>(_kpMessage);

		if (kpEvent->button == Mouse::LEFT)
		{
			if (kpEvent->buttonState == Button::DOWN)
			{
				m_bAiming = true;
			}
			else if (kpEvent->buttonState == Button::UP)
			{
				if (m_bAiming)
				{
					m_bAiming = false;
					m_fPitch = 0.0f;
					m_fPull = 0.0f;
				}
			}
		}
		else if (kpEvent->button == Mouse::WHEEL_DOWN)
		{
			m_fPull++;
		}
		else if (kpEvent->button == Mouse::WHEEL_UP)
		{
			m_fPull--;
		}
	}
	else if (_usSubject == Events::MOUSE_MOVE)
	{
		const MouseMoveEvent* kpEvent = static_cast<const MouseMoveEvent*>(_kpMessage);

		if (m_bAiming)
		{
			int iDeltaX = kpEvent->x - m_iMouseX;
			int iDeltaY = kpEvent->y - m_iMouseY;

			rotate(m_pSlingshotBaseNode->getTransformation(), Math::PI * iDeltaX * 0.001f,
				Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			m_fPitch += iDeltaY;
		}

		m_iMouseX = kpEvent->x;
		m_iMouseY = kpEvent->y;
	}
}

void CSlingshotEngine::removeEntity(const Entity& /*_krEntity*/)
{
}
