#include "Common.h"
#include "SimpleAgent.h"

using namespace std;

SimpleAgent::SimpleAgent() :
	distanceTravelled(-1.0f), speed(100.0f), wanderingAimlessly(true)
{
}

SimpleAgent::~SimpleAgent()
{
}

void SimpleAgent::determineDesiredPosition(const vector<Vector2>& waypoints)
{
	float distanceToWaypoint = 0.0f;
	const Vector2* previousWaypoint = NULL;

	for (unsigned int index = 0; index < waypoints.size(); index++)
	{
		if (previousWaypoint != NULL)
		{
			distanceToWaypoint += (waypoints.at(index) - *previousWaypoint).length();
		}

		previousWaypoint = &waypoints.at(index);

		if ((*previousWaypoint - position).length() <= velocity.length())
		{
			distanceTravelled = distanceToWaypoint;
		}

		if (distanceToWaypoint > distanceTravelled)
		{
			break;
		}
	}

	if (previousWaypoint != NULL)
	{
		desiredPosition = *previousWaypoint;
	}
}

void SimpleAgent::followPath(const vector<Vector2>& waypoints, float deltaTime)
{
	if (wanderingAimlessly)
	{
		wanderingAimlessly = false;
	}

	determineDesiredPosition(waypoints);
	moveTowardDesiredPosition(deltaTime);

	// If the first waypoint has been reached.
	if (distanceTravelled != -1)
	{
		distanceTravelled += velocity.length();
	}
}

void SimpleAgent::moveTowardDesiredPosition(float deltaTime)
{
	velocity = desiredPosition - position;
	velocity.normalise();
	velocity *= speed * deltaTime;
	position += velocity;
}

void SimpleAgent::think(const vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints,
	float deltaTime)
{
	if (waypoints.empty())
	{
		wanderAimlessly(deltaTime);
	}
	else
	{
		followPath(waypoints, deltaTime);
	}
}

void SimpleAgent::wanderAimlessly(float deltaTime)
{
	if (!wanderingAimlessly)
	{
		desiredPosition = Vector2((float) Common::getRandomInt(100, 700), (float) Common::getRandomInt(100, 500));
		wanderingAimlessly = true;
		distanceTravelled = -1.0f;
	}

	if ((desiredPosition - position).length() <= desiredPositionRadius)
	{
		desiredPosition = Vector2((float) Common::getRandomInt(100, 700), (float) Common::getRandomInt(100, 500));
	}

	moveTowardDesiredPosition(deltaTime);
}
