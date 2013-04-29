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

#include <cmath>

#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Text.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

bool CheckColliding();
bool CheckSeparatingAxis(Vector2 separatingAxis);
void OnKeyPress(const void* data);
void OnMousePress(const void* data);
void SplitTriangle();
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

GDIMesh* g_pPolygonA = NULL;
GDIMesh* g_pPolygonB = NULL;
Text* g_pResult = NULL;

bool CheckColliding()
{
	if (!CheckSeparatingAxis(g_pPolygonA->getVertices()[1] - g_pPolygonA->getVertices()[0]))
	{
		return false;
	}

	if (!CheckSeparatingAxis(g_pPolygonA->getVertices()[2] - g_pPolygonA->getVertices()[1]))
	{
		return false;
	}

	if (!CheckSeparatingAxis(g_pPolygonA->getVertices()[0] - g_pPolygonA->getVertices()[2]))
	{
		return false;
	}

	if (!CheckSeparatingAxis(g_pPolygonB->getVertices()[1] - g_pPolygonB->getVertices()[0]))
	{
		return false;
	}

	if (!CheckSeparatingAxis(g_pPolygonB->getVertices()[2] - g_pPolygonB->getVertices()[1]))
	{
		return false;
	}

	if (!CheckSeparatingAxis(g_pPolygonB->getVertices()[0] - g_pPolygonB->getVertices()[2]))
	{
		return false;
	}

	return true;
}

bool CheckSeparatingAxis(Vector2 separatingAxis)
{
	separatingAxis.rotate(Math::PI / 2.0f);
	separatingAxis.normalise();

	float polygonAmaxX = 0.0f;
	float polygonAminX = 10000.0f;
	float polygonAmaxY = 0.0f;
	float polygonAminY = 10000.0f;
	for (unsigned int index = 0; index < g_pPolygonA->getVertices().size(); index++)
	{
		float projectionDot = g_pPolygonA->getVertices()[index].dotProduct(separatingAxis);
		Vector2 projectedVertex = separatingAxis * projectionDot;

		if (projectedVertex.x < polygonAminX)
		{
			polygonAminX = projectedVertex.x;
		}
		if (projectedVertex.x > polygonAmaxX)
		{
			polygonAmaxX = projectedVertex.x;
		}
		if (projectedVertex.y < polygonAminY)
		{
			polygonAminY = projectedVertex.y;
		}
		if (projectedVertex.y > polygonAmaxY)
		{
			polygonAmaxY = projectedVertex.y;
		}
	}

	float polygonBmaxX = 0.0f;
	float polygonBminX = 10000.0f;
	float polygonBmaxY = 0.0f;
	float polygonBminY = 10000.0f;
	for (unsigned int index = 0; index < g_pPolygonB->getVertices().size(); index++)
	{
		float projectionDot = g_pPolygonB->getVertices()[index].dotProduct(separatingAxis);
		Vector2 projectedVertex = separatingAxis * projectionDot;

		if (projectedVertex.x < polygonBminX)
		{
			polygonBminX = projectedVertex.x;
		}
		if (projectedVertex.x > polygonBmaxX)
		{
			polygonBmaxX = projectedVertex.x;
		}
		if (projectedVertex.y < polygonBminY)
		{
			polygonBminY = projectedVertex.y;
		}
		if (projectedVertex.y > polygonBmaxY)
		{
			polygonBmaxY = projectedVertex.y;
		}
	}

	if ((polygonBminX > polygonAmaxX || polygonAminX > polygonBmaxX) ||
		(polygonBminY > polygonAmaxY || polygonAminY > polygonBmaxY))
	{
		return false;
	}

	return true;
}

void OnKeyPress(const void* data)
{
	const KeyboardButtonEvent* pEvent = static_cast<const KeyboardButtonEvent*>(data);

	if (pEvent->buttonState != Button::UP)
	{
		return;
	}

	if (pEvent->button == Keyboard::R)
	{
		g_pPolygonA->getVertices().clear();
		g_pPolygonB->getVertices().clear();
		g_pResult->setText("");
	}
}

void OnMousePress(const void* data)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(data);

	if (pEvent->buttonState != Button::UP)
	{
		return;
	}

	Vector2 position(static_cast<float>(pEvent->x), static_cast<float>(pEvent->y));

	if (pEvent->button == Mouse::LEFT)
	{
		if (g_pPolygonA->getVertices().size() == 3)
		{
			return;
		}

		g_pPolygonA->getVertices().push_back(position);

		if (g_pPolygonA->getVertices().size() == 3 && g_pPolygonB->getVertices().size() == 3)
		{
			if (CheckColliding())
			{
				g_pResult->setText("Poygons are Colliding!");
			}
			else
			{
				g_pResult->setText("Poygons are not Colliding!");
			}
		}
	}
	else if (pEvent->button == Mouse::RIGHT)
	{
		if (g_pPolygonB->getVertices().size() == 3)
		{
			return;
		}

		g_pPolygonB->getVertices().push_back(position);

		if (g_pPolygonA->getVertices().size() == 3 && g_pPolygonB->getVertices().size() == 3)
		{
			if (CheckColliding())
			{
				g_pResult->setText("Poygons are Colliding!");
			}
			else
			{
				g_pResult->setText("Poygons are not Colliding!");
			}
		}
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 002.4", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	Entity* allModels = new Entity;
	g_pPolygonA = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pPolygonA->setPrimitiveType(Model::TRIANGLE_STRIP);
	allModels->addComponent(g_pPolygonA);
	g_pPolygonB = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pPolygonB->setPrimitiveType(Model::TRIANGLE_STRIP);
	allModels->addComponent(g_pPolygonB);
	g_pResult = new Text(Vector2(10.0f, 10.0f), "");
	allModels->addComponent(g_pResult);

	GazEngine::addEntity(allModels);

	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyPress);
	Messages::registerRecipient(Events::MOUSE_BUTTON, OnMousePress);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
