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

#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/Messages.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

float GetIntersectionTime(const Vector2& lineSegmentStart, const Vector2& lineSegmentFinish,
						  const Vector2& planeNormal, const Vector2& planePosition);
int Intersect(const Vector2& planeNormal, const Vector2& planePosition, const Vector2& point);
void OnKeyPress(const void* data);
void OnMousePress(const void* data);
void SplitTriangle();
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

const int BEHIND = -1;
const int INFRONT = 1;
const int ON_PLANE = 0;

GDIMesh* g_pLineSegmentPoints = NULL;
GDIMesh* g_pLineSegmentSolid = NULL;
GDIMesh* g_pTrianglePoints = NULL;
GDIMesh* g_pTriangleSolid = NULL;
GDIMesh* g_pTriangleSolid0 = NULL;
GDIMesh* g_pTriangleSolid1 = NULL;

float GetIntersectionTime(const Vector2& lineSegmentStart, const Vector2& lineSegmentFinish,
						  const Vector2& planeNormal, const Vector2& planePosition)
{
	return planeNormal.dotProduct(planePosition - lineSegmentStart) /
		planeNormal.dotProduct(lineSegmentFinish - lineSegmentStart);
}

int Intersect(const Vector2& planeNormal, const Vector2& planePosition, const Vector2& point)
{
	float referenceDistance = planeNormal.dotProduct(planePosition);
	float result = planeNormal.dotProduct(point) - referenceDistance;

	if (result < 0.0f)
	{
		return BEHIND;
	}
	else if (result > 0.0f)
	{
		return INFRONT;
	}
	else
	{
		return ON_PLANE;
	}
}

void OnKeyPress(const void* data)
{
	const KeyboardButtonEvent* pEvent = static_cast<const KeyboardButtonEvent*>(data);

	if (pEvent->buttonState != Button::UP)
	{
		return;
	}

	if (pEvent->button == Keyboard::L)
	{
		g_pLineSegmentPoints->getVertices().clear();
		g_pLineSegmentSolid->getVertices().clear();
		g_pTriangleSolid0->getVertices().clear();
		g_pTriangleSolid1->getVertices().clear();
	}
	else if (pEvent->button == Keyboard::T)
	{
		g_pTrianglePoints->getVertices().clear();
		g_pTriangleSolid->getVertices().clear();
		g_pTriangleSolid0->getVertices().clear();
		g_pTriangleSolid1->getVertices().clear();
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

			if (g_pLineSegmentPoints->getVertices().size() == 2)
			{
				SplitTriangle();
			}
		}
	}
	else if (pEvent->button == Mouse::RIGHT)
	{
		if (g_pLineSegmentPoints->getVertices().size() == 2)
		{
			return;
		}

		g_pLineSegmentPoints->getVertices().push_back(position);
		if (g_pLineSegmentPoints->getVertices().size() == 2)
		{
			g_pLineSegmentSolid->getVertices().insert(g_pLineSegmentSolid->getVertices().end(),
				g_pLineSegmentPoints->getVertices().begin(), g_pLineSegmentPoints->getVertices().end());

			if (g_pTrianglePoints->getVertices().size() == 3)
			{
				SplitTriangle();
			}
		}
	}
}

void SplitTriangle()
{
	g_pTriangleSolid0->getVertices().clear();
	g_pTriangleSolid1->getVertices().clear();

	Vector2 planeNormal = g_pLineSegmentPoints->getVertices()[1] - g_pLineSegmentPoints->getVertices()[0];
	Vector2 planePosition = g_pLineSegmentPoints->getVertices()[0];
	planeNormal.rotate(Math::PI / 2.0f);

	int point0Position = Intersect(planeNormal, planePosition, g_pTrianglePoints->getVertices()[0]);
	if (point0Position != BEHIND)
	{
		g_pTriangleSolid0->getVertices().push_back(g_pTrianglePoints->getVertices()[0]);
	}
	if (point0Position != INFRONT)
	{
		g_pTriangleSolid1->getVertices().push_back(g_pTrianglePoints->getVertices()[0]);
	}

	int point1Position = Intersect(planeNormal, planePosition, g_pTrianglePoints->getVertices()[1]);
	if (point1Position != BEHIND)
	{
		g_pTriangleSolid0->getVertices().push_back(g_pTrianglePoints->getVertices()[1]);
	}
	if (point1Position != INFRONT)
	{
		g_pTriangleSolid1->getVertices().push_back(g_pTrianglePoints->getVertices()[1]);
	}

	int point2Position = Intersect(planeNormal, planePosition, g_pTrianglePoints->getVertices()[2]);
	if (point2Position != BEHIND)
	{
		g_pTriangleSolid0->getVertices().push_back(g_pTrianglePoints->getVertices()[2]);
	}
	if (point2Position != INFRONT)
	{
		g_pTriangleSolid1->getVertices().push_back(g_pTrianglePoints->getVertices()[2]);
	}

	float segment0Time = GetIntersectionTime(g_pTrianglePoints->getVertices()[0], g_pTrianglePoints->getVertices()[1], planeNormal, planePosition);
	if (segment0Time >= 0.0f && segment0Time <= 1.0f)
	{
		Vector2 intersection = g_pTrianglePoints->getVertices()[1] - g_pTrianglePoints->getVertices()[0];
		intersection *= segment0Time;
		intersection += g_pTrianglePoints->getVertices()[0];

		g_pTriangleSolid0->getVertices().push_back(intersection);
		g_pTriangleSolid1->getVertices().push_back(intersection);
	}

	float segment1Time = GetIntersectionTime(g_pTrianglePoints->getVertices()[1], g_pTrianglePoints->getVertices()[2], planeNormal, planePosition);
	if (segment1Time >= 0.0f && segment1Time <= 1.0f)
	{
		Vector2 intersection = g_pTrianglePoints->getVertices()[2] - g_pTrianglePoints->getVertices()[1];
		intersection *= segment1Time;
		intersection += g_pTrianglePoints->getVertices()[1];

		g_pTriangleSolid0->getVertices().push_back(intersection);
		g_pTriangleSolid1->getVertices().push_back(intersection);
	}

	float segment2Time = GetIntersectionTime(g_pTrianglePoints->getVertices()[2], g_pTrianglePoints->getVertices()[0], planeNormal, planePosition);
	if (segment2Time >= 0.0f && segment2Time <= 1.0f)
	{
		Vector2 intersection = g_pTrianglePoints->getVertices()[0] - g_pTrianglePoints->getVertices()[2];
		intersection *= segment2Time;
		intersection += g_pTrianglePoints->getVertices()[2];

		g_pTriangleSolid0->getVertices().push_back(intersection);
		g_pTriangleSolid1->getVertices().push_back(intersection);
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 001.5", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	Entity* allModels = new Entity;
	g_pLineSegmentPoints = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pLineSegmentPoints->setPrimitiveType(Model::POINTS);
	allModels->addComponent(g_pLineSegmentPoints);
	g_pLineSegmentSolid = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pLineSegmentSolid->setPrimitiveType(Model::LINE_STREAM);
	allModels->addComponent(g_pLineSegmentSolid);
	g_pTrianglePoints = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pTrianglePoints->setPrimitiveType(Model::POINTS);
	allModels->addComponent(g_pTrianglePoints);
	g_pTriangleSolid = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	allModels->addComponent(g_pTriangleSolid);
	g_pTriangleSolid0 = new GDIMesh(vector<Vector2>(), RGB(255, 0, 0));
	allModels->addComponent(g_pTriangleSolid0);
	g_pTriangleSolid1 = new GDIMesh(vector<Vector2>(), RGB(0, 255, 0));
	allModels->addComponent(g_pTriangleSolid1);
	GazEngine::addEntity(allModels);

	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyPress);
	Messages::registerRecipient(Events::MOUSE_BUTTON, OnMousePress);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
