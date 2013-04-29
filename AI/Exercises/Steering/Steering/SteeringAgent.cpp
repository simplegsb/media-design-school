#include <cmath>

#include "Common.h"
#include "SteeringAgent.h"

using namespace std;

const float PI = 3.14159265358979323f;

SteeringAgent::SteeringAgent() :
	nextWaypointIndex(-1), previousWaypointIndex(-1), repulsionRadius(50.0f), speed(100.0f), steeringLock(5.0f),
	wanderingAimlessly(true)
{
	desiredPositionRadius = 15.0f;
}

SteeringAgent::~SteeringAgent()
{
}

void SteeringAgent::advanceDesiredPosition(const vector<Vector2>& waypoints, float distance)
{
	if (waypoints.size() == 1)
	{
		return;
	}

	if (previousWaypointIndex == -1)
	{
		previousWaypointIndex = 0;
		nextWaypointIndex = 1;
	}

	Vector2 advanceDirection = waypoints.at(nextWaypointIndex) - waypoints.at(previousWaypointIndex);
	advanceDirection.normalise();
	advanceDirection *= distance;
	Vector2 desiredPositionCopy = desiredPosition;
	desiredPositionCopy += advanceDirection;

	float distancePastNextWaypoint = (desiredPositionCopy - waypoints.at(previousWaypointIndex)).length() -
		(waypoints.at(nextWaypointIndex) - waypoints.at(previousWaypointIndex)).length();

	if (distancePastNextWaypoint > 0.0f)
	{
		desiredPosition = waypoints.at(nextWaypointIndex);

		if (waypoints.size() > nextWaypointIndex + 1)
		{
			previousWaypointIndex++;
			nextWaypointIndex++;

			advanceDesiredPosition(waypoints, distancePastNextWaypoint);
		}
	}
	else
	{
		desiredPosition += advanceDirection;
	}
}

void SteeringAgent::followPath(const vector<Vector2>& repulsionPoints, const vector<Vector2>& waypoints,
	float deltaTime)
{
	if (wanderingAimlessly)
	{
		desiredPosition = waypoints.at(0);
		wanderingAimlessly = false;
	}

	if ((desiredPosition - position).length() <= desiredPositionRadius)
	{
		advanceDesiredPosition(waypoints, 75.0f);
	}

	steerToDesiredPosition(repulsionPoints, deltaTime);
}

Vector2 SteeringAgent::getRepulsionAffect(const vector<Vector2>& repulsionPoints, float deltaTime)
{
	Vector2 cumulativeRepulsionAffect;

	for (unsigned int index = 0; index < repulsionPoints.size(); index++)
	{
		Vector2 repulsionAffect = position - repulsionPoints.at(index);
		float distanceToRepulsionPoint = repulsionAffect.length();

		if (distanceToRepulsionPoint < repulsionRadius)
		{
			float distanceIntoRepulsionArea = repulsionRadius - distanceToRepulsionPoint;
			repulsionAffect.normalise();
			repulsionAffect *= deltaTime * pow(distanceIntoRepulsionArea, 4.0f);
			cumulativeRepulsionAffect += repulsionAffect;
		}
	}

	return cumulativeRepulsionAffect;
}

void SteeringAgent::steerToDesiredPosition(const vector<Vector2>& repulsionPoints, float deltaTime)
{
	Vector2 repulsionAffect = getRepulsionAffect(repulsionPoints, deltaTime);

	// Determine possible turning angles.
	Vector2 bearing = desiredPosition - position + repulsionAffect;
	float angleToBearing = velocity.angleTo(bearing);
	float angleMax = steeringLock * deltaTime;

	// Determine turning direction.
	if (velocity.crossProduct(bearing) < 0.0f)
	{
		angleToBearing *= -1;
		angleMax *= -1;
	}

	// Determine the smallest angle.
	float angleFinal = 0.0f;
	if (angleToBearing < 0.0f)
	{
		angleFinal = max(angleToBearing, angleMax);
	}
	else if (angleToBearing > 0.0f)
	{
		angleFinal = min(angleToBearing, angleMax);
	}

	velocity.normalise();
	velocity *= speed * deltaTime;
	velocity.rotate(angleFinal);

	position += velocity;
}

void SteeringAgent::think(const vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints,
	float deltaTime)
{
	if (waypoints.empty())
	{
		wanderAimlessly(repulsionPoints, deltaTime);
	}
	else
	{
		followPath(repulsionPoints, waypoints, deltaTime);
	}
}

void SteeringAgent::wanderAimlessly(const vector<Vector2>& repulsionPoints, float deltaTime)
{
	if (!wanderingAimlessly)
	{
		desiredPosition = Vector2((float) Common::getRandomInt(100, 700), (float) Common::getRandomInt(100, 500));
		wanderingAimlessly = true;
		previousWaypointIndex = -1;
		nextWaypointIndex = -1;
	}

	if ((desiredPosition - position).length() <= desiredPositionRadius)
	{
		desiredPosition = Vector2((float) Common::getRandomInt(100, 700), (float) Common::getRandomInt(100, 500));
	}

	steerToDesiredPosition(repulsionPoints, deltaTime);
}
