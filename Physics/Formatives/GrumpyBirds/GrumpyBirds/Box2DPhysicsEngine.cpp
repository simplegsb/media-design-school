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
// $Date: 2013-01-29 11:58:25 +1300 (Tue, 29 Jan 2013) $
// $Rev: 1129 $
//
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Circle.h>

#include "Box2DEvents.h"
#include "Box2DPhysicsEngine.h"

using namespace std;

class CContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact)
	{
		TBox2DEvent theEvent;
		theEvent.contact = contact;
		Messages::send(Box2DEvents::BEGIN_CONTACT, &theEvent);
	}

	void EndContact(b2Contact* contact)
	{
		TBox2DEvent theEvent;
		theEvent.contact = contact;
		Messages::send(Box2DEvents::END_CONTACT, &theEvent);
	}

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		TBox2DEvent theEvent;
		theEvent.contact = contact;
		theEvent.impulse = impulse;
		Messages::send(Box2DEvents::POST_SOLVE, &theEvent);
	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		TBox2DEvent theEvent;
		theEvent.contact = contact;
		theEvent.oldManifold = oldManifold;
		Messages::send(Box2DEvents::PRE_SOLVE, &theEvent);
	}
};

CBox2DPhysicsEngine::CBox2DPhysicsEngine(const Vector2& _krGravity, CBox2DGDIDrawer* _pDrawer) :
	m_pDrawer(_pDrawer),
	m_world(ToB2Vec2(_krGravity))
{
	m_world.SetContactListener(new CContactListener); // This will not be deleted!!!

	if (m_pDrawer != NULL)
	{
		m_world.SetDebugDraw(m_pDrawer);
	}
}

void CBox2DPhysicsEngine::advance()
{
	m_world.Step(1.0f / 45.0f, 8, 3);

	/*for (b2Body* pBody = m_world.GetBodyList(); pBody != NULL; pBody = pBody->GetNext())
	{
		if (pBody->IsAwake())
		{
			b2CircleShape* pCircleShape = dynamic_cast<b2CircleShape*>(pBody->GetFixtureList()->GetShape());
			if (pCircleShape != NULL)
			{
				Entity* pEntity = static_cast<Entity*>(pBody->GetUserData());
				pEntity->getSingleComponent<Circle>()->setPosition(ToVector2(pBody->GetPosition()));
			}
			b2PolygonShape* pPolygonShape = dynamic_cast<b2PolygonShape*>(pBody->GetFixtureList()->GetShape());
			if (pPolygonShape != NULL)
			{
				Entity* pEntity = static_cast<Entity*>(pBody->GetUserData());
				GDIMesh* pMesh = pEntity->getSingleComponent<GDIMesh>();
				for (int32 iIndex = 0; iIndex < pPolygonShape->GetVertexCount(); iIndex++)
				{
					pMesh->getVertices()[iIndex].x = pPolygonShape->GetVertex(iIndex).x;
					pMesh->getVertices()[iIndex].y = pPolygonShape->GetVertex(iIndex).y;
				}
			}
		}
	}*/

	if (m_pDrawer != NULL)
	{
		m_pDrawer->SetupFrame();
		m_world.DrawDebugData();
		m_pDrawer->EndFrame();
	}
}

void CBox2DPhysicsEngine::destroy()
{
}

void CBox2DPhysicsEngine::addEntity(Entity* entity)
{
	vector<CBox2DBody*> entityBodies = entity->getComponents<CBox2DBody>();
	for (unsigned int uiIndex = 0; uiIndex < entityBodies.size(); uiIndex++)
	{
		entityBodies[uiIndex]->AddToWorld(m_world);
	}
}

void CBox2DPhysicsEngine::init()
{
}

void CBox2DPhysicsEngine::removeEntity(const Entity& entity)
{
	vector<CBox2DBody*> entityBodies = entity.getComponents<CBox2DBody>();
	for (unsigned int uiIndex = 0; uiIndex < entityBodies.size(); uiIndex++)
	{
		entityBodies[uiIndex]->RemoveFromWorld(m_world);
	}
}
