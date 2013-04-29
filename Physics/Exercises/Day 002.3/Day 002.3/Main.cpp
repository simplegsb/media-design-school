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

bool CheckCollision();
float GetDistance(const Vector2& lineSegmentStart, const Vector2& lineSegmentFinish, const Vector2& point);
bool Intersect(const Vector2& lineSegA1, const Vector2& lineSegA2, const Vector2& lineSegB1, const Vector2& lineSegB2);
bool Intersect(const Vector2& capA1, const Vector2& capA2, float capArad, const Vector2& capB1, const Vector2& capB2, float capBrad);
void OnKeyPress(const void* data);
void OnMousePress(const void* data);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

unsigned int g_capAclicks = 0;
unsigned int g_capBclicks = 0;
Vector2 g_capA1;
Vector2 g_capB1;
GDICircle* g_pCapA1c = NULL;
GDICircle* g_pCapA2c = NULL;
GDICircle* g_pCapB1c = NULL;
GDICircle* g_pCapB2c = NULL;
GDIMesh* g_pCapAsides = NULL;
GDIMesh* g_pCapBsides = NULL;
Text* g_pResult = NULL;

bool CheckCollision()
{
	Vector2 capAlineSeg = g_pCapA2c->getPosition() - g_pCapA1c->getPosition();
	Vector2 capBlineSeg = g_pCapB2c->getPosition() - g_pCapB1c->getPosition();

	// What are r and d?
	//float s = (capAlineSeg.dotProduct(capBlineSeg) * capBlineSeg.dotProduct(r) - capAlineSeg.dotProduct(r) * capAlineSeg.dotProduct(capBlineSeg)) / d;
	//float t = (capAlineSeg.dotProduct(capAlineSeg) * capBlineSeg.dotProduct(r) - capAlineSeg.dotProduct(capBlineSeg) * capAlineSeg.dotProduct(r)) / d;

	//Vector2 intersection = g_pCapB1c->getPosition() - g_pCapA1c->getPosition() + t * capBlineSeg - s * capAlineSeg;

	// And now we do some projection stuff?

	return false;
}

float GetDistance(const Vector2& lineSegmentStart, const Vector2& lineSegmentFinish, const Vector2& point)
{
	Vector2 startToFinish = lineSegmentFinish - lineSegmentStart;
	Vector2 startToPoint = point - lineSegmentStart;

	float t = (startToPoint.x * startToFinish.x + startToPoint.y * startToFinish.y) / startToFinish.lengthSquared();

	Vector2 closestPointOnLine;
	closestPointOnLine.x = lineSegmentStart.x + t * (lineSegmentFinish.x - lineSegmentStart.x);
	closestPointOnLine.y = lineSegmentStart.y + t * (lineSegmentFinish.y - lineSegmentStart.y);

	if (closestPointOnLine.x < min(lineSegmentStart.x, lineSegmentFinish.x) ||
		closestPointOnLine.x > max(lineSegmentStart.x, lineSegmentFinish.x) ||
		closestPointOnLine.y < min(lineSegmentStart.y, lineSegmentFinish.y) ||
		closestPointOnLine.y > max(lineSegmentStart.y, lineSegmentFinish.y))
	{
		if ((closestPointOnLine - lineSegmentStart).lengthSquared() < (closestPointOnLine - lineSegmentFinish).lengthSquared())
		{
			closestPointOnLine = lineSegmentStart;
		}
		else
		{
			closestPointOnLine = lineSegmentFinish;
		}
	}

	return (closestPointOnLine - point).length();
}

bool Intersect(const Vector2& lineSegA1, const Vector2& lineSegA2, const Vector2& lineSegB1, const Vector2& lineSegB2)
{
	

	return false;
}

bool Intersect(const Vector2& capA1, const Vector2& capA2, float capArad, const Vector2& capB1, const Vector2& capB2, float capBrad)
{
	if (Intersect(capA1, capA2, capB1, capB2))
	{
		return true;
	}

	float distance0 = GetDistance(capA1, capA2, capB1);
	float distance1 = GetDistance(capA1, capA2, capB2);
	float distance2 = GetDistance(capB1, capB2, capA1);
	float distance3 = GetDistance(capB1, capB2, capA2);

	float distance = distance0;
	if (distance1 < distance)
	{
		distance = distance1;
	}
	if (distance2 < distance)
	{
		distance = distance2;
	}
	if (distance3 < distance)
	{
		distance = distance3;
	}

	if (distance < capArad + capBrad)
	{
		return true;
	}

	return false;
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
		g_capAclicks = 0;
		g_capBclicks = 0;
		g_pCapA1c->setVisible(false);
		g_pCapA2c->setVisible(false);
		g_pCapB1c->setVisible(false);
		g_pCapB2c->setVisible(false);
		g_pCapAsides->getVertices().clear();
		g_pCapAsides->setVisible(false);
		g_pCapBsides->getVertices().clear();
		g_pCapBsides->setVisible(false);
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
		if (g_capAclicks == 0)
		{
			g_capA1 = position;
			g_pCapA1c->setPosition(position);
		}
		else if (g_capAclicks == 1)
		{
			g_pCapA1c->setRadius((g_pCapA1c->getPosition() - position).length());
			g_pCapA1c->setVisible(true);
		}
		else if (g_capAclicks == 2)
		{
			g_pCapA2c->setPosition(position);
			g_pCapA2c->setRadius(g_pCapA1c->getRadius());
			g_pCapA2c->setVisible(true);

			Vector2 normalRad = g_pCapA1c->getPosition() - g_pCapA2c->getPosition();
			normalRad.rotate(Math::PI / 2.0f);
			normalRad.normalise();
			normalRad *= g_pCapA1c->getRadius();

			g_pCapAsides->getVertices().push_back(g_pCapA1c->getPosition() + normalRad);
			g_pCapAsides->getVertices().push_back(g_pCapA2c->getPosition() + normalRad);
			g_pCapAsides->getVertices().push_back(g_pCapA1c->getPosition() - normalRad);
			g_pCapAsides->getVertices().push_back(g_pCapA2c->getPosition() - normalRad);
			g_pCapAsides->setVisible(true);

			if (g_capBclicks >= 2)
			{
				if (Intersect(g_pCapA1c->getPosition(), g_pCapA2c->getPosition(), g_pCapA1c->getRadius(),
					g_pCapB1c->getPosition(), g_pCapB2c->getPosition(), g_pCapB1c->getRadius()))
				{
					g_pResult->setText("The Capsules Intersect!");
				}
				else
				{
					g_pResult->setText("The Capsules Do Not Intersect!");
				}
			}
		}

		g_capAclicks++;
	}
	else if (pEvent->button == Mouse::RIGHT)
	{
		if (g_capBclicks == 0)
		{
			g_capB1 = position;
			g_pCapB1c->setPosition(position);
		}
		else if (g_capBclicks == 1)
		{
			g_pCapB1c->setRadius((g_pCapB1c->getPosition() - position).length());
			g_pCapB1c->setVisible(true);
		}
		else if (g_capBclicks == 2)
		{
			g_pCapB2c->setPosition(position);
			g_pCapB2c->setRadius(g_pCapB1c->getRadius());
			g_pCapB2c->setVisible(true);

			Vector2 normalRad = g_pCapB1c->getPosition() - g_pCapB2c->getPosition();
			normalRad.rotate(Math::PI / 2.0f);
			normalRad.normalise();
			normalRad *= g_pCapB1c->getRadius();

			g_pCapBsides->getVertices().push_back(g_pCapB1c->getPosition() + normalRad);
			g_pCapBsides->getVertices().push_back(g_pCapB2c->getPosition() + normalRad);
			g_pCapBsides->getVertices().push_back(g_pCapB1c->getPosition() - normalRad);
			g_pCapBsides->getVertices().push_back(g_pCapB2c->getPosition() - normalRad);
			g_pCapBsides->setVisible(true);

			if (g_capBclicks >= 2)
			{
				if (Intersect(g_pCapA1c->getPosition(), g_pCapA2c->getPosition(), g_pCapA1c->getRadius(),
					g_pCapB1c->getPosition(), g_pCapB2c->getPosition(), g_pCapB1c->getRadius()))
				{
					g_pResult->setText("The Capsules Intersect!");
				}
				else
				{
					g_pResult->setText("The Capsules Do Not Intersect!");
				}
			}
		}

		g_capBclicks++;
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 002.3", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	Entity* allModels = new Entity;
	g_pCapA1c = new GDICircle(Vector2(), 0.0f);
	g_pCapA1c->setVisible(false);
	allModels->addComponent(g_pCapA1c);
	g_pCapA2c = new GDICircle(Vector2(), 0.0f);
	g_pCapA2c->setVisible(false);
	allModels->addComponent(g_pCapA2c);
	g_pCapB1c = new GDICircle(Vector2(), 0.0f);
	g_pCapB1c->setVisible(false);
	allModels->addComponent(g_pCapB1c);
	g_pCapB2c = new GDICircle(Vector2(), 0.0f);
	g_pCapB2c->setVisible(false);
	allModels->addComponent(g_pCapB2c);
	g_pCapAsides = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pCapAsides->setPrimitiveType(Model::LINE_LIST);
	allModels->addComponent(g_pCapAsides);
	g_pCapBsides = new GDIMesh(vector<Vector2>(), RGB(0, 0, 0));
	g_pCapBsides->setPrimitiveType(Model::LINE_LIST);
	allModels->addComponent(g_pCapBsides);
	g_pResult = new Text(Vector2(10.0f, 10.0f), "");
	allModels->addComponent(g_pResult);

	GazEngine::addEntity(allModels);

	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyPress);
	Messages::registerRecipient(Events::MOUSE_BUTTON, OnMousePress);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
