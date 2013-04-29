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
	Vector2& point = g_pPoint->getVertices()[0];
	Vector2& triangle0 = g_pTrianglePoints->getVertices()[0];
	Vector2& triangle1 = g_pTrianglePoints->getVertices()[1];
	Vector2& triangle2 = g_pTrianglePoints->getVertices()[2];

	/*
	From slide, couldn't get it to work...

	float dot00 = triangle0.dotProduct(triangle0);
	float dot01 = triangle0.dotProduct(triangle1);
	float dot02 = triangle0.dotProduct(triangle2);
	float dot11 = triangle1.dotProduct(triangle1);
	float dot12 = triangle1.dotProduct(triangle2);
	float dot20 = triangle2.dotProduct(triangle0);
	float dot21 = triangle2.dotProduct(triangle1);

	float denominator = dot00 * dot11 - dot01 * dot01;

	float v = (dot11 * dot20 - dot12 * dot01) / denominator;
	float w = (dot00 * dot21 - dot02 * dot01) / denominator;
	float u = 1 - v - w;
	*/

	float determinant = (triangle1.y - triangle2.y) * (triangle0.x - triangle2.x) + (triangle2.x - triangle1.x) * (triangle0.y - triangle2.y);

    float v = ((triangle1.y - triangle2.y) * (point.x - triangle2.x) + (triangle2.x - triangle1.x) * (point.y - triangle2.y)) / determinant;
    float w = ((triangle2.y - triangle0.y) * (point.x - triangle2.x) + (triangle0.x - triangle2.x) * (point.y - triangle2.y)) / determinant;
    float u = 1 - v - w;

	if (0 <= v && v <= 1 &&
		0 <= w && w <= 1 &&
		v + w <= 1)
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
			g_pPoint->getVertices().clear();
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
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 002.2", instance, iCommandShow);
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
