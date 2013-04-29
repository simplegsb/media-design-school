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

void CheckInside();
void OnKeyPress(const void* data);
void OnMousePress(const void* data);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

GDIMesh* g_pPoint = NULL;
Text* g_pResult = NULL;
GDIMesh* g_pTrianglePoints = NULL;
GDIMesh* g_pTriangleSolid = NULL;

void CheckInside()
{
	Vector2 toTriangle0 = g_pTrianglePoints->getVertices()[0] - g_pPoint->getVertices()[0];
	toTriangle0.normalise();
	Vector2 toTriangle1 = g_pTrianglePoints->getVertices()[1] - g_pPoint->getVertices()[0];
	toTriangle1.normalise();
	Vector2 toTriangle2 = g_pTrianglePoints->getVertices()[2] - g_pPoint->getVertices()[0];
	toTriangle2.normalise();

	float angleSum = 0.0f;

	angleSum += acosf(toTriangle0.dotProduct(toTriangle1));
	angleSum += acosf(toTriangle1.dotProduct(toTriangle2));
	angleSum += acosf(toTriangle2.dotProduct(toTriangle0));

	if (abs(Math::PI * 2.0f - angleSum) < 0.0001f)
	{
		g_pResult->setText("Point is Inside!");
	}
	else
	{
		g_pResult->setText("Point is Outside!");
	}
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
		g_pPoint->getVertices().clear();
		g_pResult->setText("");
		g_pTrianglePoints->getVertices().clear();
		g_pTriangleSolid->getVertices().clear();
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
		if (g_pTrianglePoints->getVertices().size() == 3)
		{
			return;
		}

		g_pTrianglePoints->getVertices().push_back(position);
		if (g_pTrianglePoints->getVertices().size() == 3)
		{
			g_pTriangleSolid->getVertices().insert(g_pTriangleSolid->getVertices().end(),
				g_pTrianglePoints->getVertices().begin(), g_pTrianglePoints->getVertices().end());

			if (g_pPoint->getVertices().size() == 1)
			{
				CheckInside();
			}
		}
	}
	else if (pEvent->button == Mouse::RIGHT)
	{
		if (g_pPoint->getVertices().size() == 1)
		{
			return;
		}

		g_pPoint->getVertices().push_back(position);

		if (g_pTrianglePoints->getVertices().size() == 3)
		{
			CheckInside();
		}
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 002.1", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	Entity* allModels = new Entity;
	g_pPoint = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pPoint->setPrimitiveType(Model::POINTS);
	allModels->addComponent(g_pPoint);
	g_pResult = new Text(Vector2(10.0f, 10.0f), "");
	allModels->addComponent(g_pResult);
	g_pTrianglePoints = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pTrianglePoints->setPrimitiveType(Model::POINTS);
	allModels->addComponent(g_pTrianglePoints);
	g_pTriangleSolid = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	allModels->addComponent(g_pTriangleSolid);
	GazEngine::addEntity(allModels);

	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyPress);
	Messages::registerRecipient(Events::MOUSE_BUTTON, OnMousePress);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
