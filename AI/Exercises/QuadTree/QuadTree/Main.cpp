#include <gazengine/Events.h>
#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/gdi/model/GDIQuadTree.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Intersection.h>
#include <gazengine/math/Math.h>
#include <gazengine/Messages.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

const unsigned int CIRCLE_COUNT = 5;

void addCircles();
void detectAndRespondToCollisions();
void moveBullets();
void moveCircles();
void onMouseButton(const void* message);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

vector<GDICircle*> bullets;
vector<Vector2> bulletVelocities;
vector<GDICircle*> circles;
vector<Vector2> circleVelocities;
GDIRenderingEngine* renderingEnginePtr = NULL;
Timer timer;

void addCircles()
{
	circles.reserve(CIRCLE_COUNT);
	circleVelocities.reserve(CIRCLE_COUNT);

	for (unsigned int index = 0; index < CIRCLE_COUNT; index++)
	{
		int x = Math::getRandomInt(200, 600);
		int y = Math::getRandomInt(200, 400);
		float angle = Math::getRandomFloat(0.0f, 2 * Math::PI);

		GDICircle* circle = new GDICircle(renderingEnginePtr->getBuffer(), Vector2((float) x, (float) y), 5.0f);
		circle->setColour(RGB(0, 0, 255));
		circles.push_back(circle);
		renderingEnginePtr->getQuadTree()->insert(circle);

		Vector2 velocity(0.0f, -50.0f);
		velocity.rotate(angle);
		circleVelocities.push_back(velocity);
	}
}

void detectAndRespondToCollisions()
{
	for (unsigned int bulletIndex = 0; bulletIndex < bullets.size(); bulletIndex++)
	{
		Circle& bullet = static_cast<Circle&>(*bullets.at(bulletIndex));
		AABB2 range;
		range.center = bullet.getPosition();
		range.halfDimension = bullet.getRadius();

		vector<Model*> models = renderingEnginePtr->getQuadTree()->queryRange(range);

		for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
		{
			Circle& model = static_cast<Circle&>(*models.at(modelIndex));
			if (&bullet != &model && Intersection::intersect(bullet, model))
			{
				renderingEnginePtr->getQuadTree()->remove(bullet);
				bullets.erase(bullets.begin() + bulletIndex);
				bulletVelocities.erase(bulletVelocities.begin() + bulletIndex);

				int x = Math::getRandomInt(200, 600);
				int y = Math::getRandomInt(200, 400);
				model.setPosition(Vector2((float) x, (float) y));
			}
		}
	}
}

void moveBullets()
{
	for (unsigned int index = 0; index < bullets.size(); index++)
	{
		Vector2 newPosition = bullets.at(index)->getPosition() + bulletVelocities.at(index) * timer.getDeltaTime();
		if (newPosition.x < 0.0f || newPosition.x > 800.0f || newPosition.y < 0.0f || newPosition.y > 600.0f)
		{
			renderingEnginePtr->getQuadTree()->remove(*bullets.at(index));
			bullets.erase(bullets.begin() + index);
			bulletVelocities.erase(bulletVelocities.begin() + index);
		}
		else
		{
			bullets.at(index)->setPosition(newPosition);
			renderingEnginePtr->getQuadTree()->update(*bullets.at(index));
		}
	}
}

void moveCircles()
{
	for (unsigned int index = 0; index < circles.size(); index++)
	{
		Vector2 newPosition = circles.at(index)->getPosition() + circleVelocities.at(index) * timer.getDeltaTime();

		if (newPosition.x < 0.0f || newPosition.x > 800.0f || newPosition.y < 0.0f || newPosition.y > 600.0f)
		{
			int x = Math::getRandomInt(200, 600);
			int y = Math::getRandomInt(200, 400);
			circles.at(index)->setPosition(Vector2((float) x, (float) y));
		}
		else
		{
			circles.at(index)->setPosition(newPosition);
		}

		renderingEnginePtr->getQuadTree()->update(*circles.at(index));
	}
}

void onMouseButton(const void* message)
{
	const MouseButtonEvent* mouseButtonEvent = static_cast<const MouseButtonEvent*>(message);

	GDICircle* bullet = new GDICircle(renderingEnginePtr->getBuffer(), Vector2(400, 300), 2.0f);
	bullet->setColour(RGB(255, 0, 0));
	bullets.push_back(bullet);
	renderingEnginePtr->getQuadTree()->insert(bullet);

	Vector2 mousePosition((float) mouseButtonEvent->x, (float) mouseButtonEvent->y);
	Vector2 velocity = mousePosition - bullet->getPosition();
	velocity.normalise();
	velocity *= 100.0f;
	bulletVelocities.push_back(velocity);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine windowEngine(L"Quad Tree", instance, commandShow);
    windowEngine.init();

    GDIRenderingEngine renderingEngine(windowEngine.getWindow());
	renderingEnginePtr = &renderingEngine;
    renderingEngine.init();

	// Setup the quad tree.
	AABB2 boundary;
	boundary.center = Vector2(400.0f, 300.0f);
	boundary.halfDimension = 400.0f;
	renderingEngine.setQuadTree(new QuadTree(1, boundary));

	// Draw quad tree visual debugging.
	renderingEngine.addModel(new GDIQuadTree(renderingEngine.getBuffer(), *renderingEngine.getQuadTree()));

	// Add the circles.
	addCircles();

	// Register the mouse observer.
	Messages::registerRecipient(MOUSE_BUTTON_EVENT, &onMouseButton);

	timer.reset();

    while(windowEngine.getMessage().message != WM_QUIT)
    {
		timer.tick();

		// Advance the demo.
		moveBullets();
		moveCircles();
		detectAndRespondToCollisions();
		windowEngine.advance();
		renderingEngine.advance();
    }

	renderingEngine.destroy();
    windowEngine.destroy();

    return windowEngine.getMessage().wParam;
}
