#ifndef STEERINGAGENT_H_
#define STEERINGAGENT_H_

#include <vector>

#include "Agent.h"
#include "Vector2.h"

class SteeringAgent : public Agent
{
	public:
		SteeringAgent();

		virtual ~SteeringAgent();

		void think(const std::vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints,
			float deltaTime);

	private:
		int nextWaypointIndex;

		int previousWaypointIndex;

		float repulsionRadius;

		float speed;

		float steeringLock;

		bool wanderingAimlessly;

		void advanceDesiredPosition(const std::vector<Vector2>& waypoints, float distance);

		void followPath(const std::vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints, float deltaTime);

		Vector2 getRepulsionAffect(const std::vector<Vector2>& repulsionPoints, float deltaTime);

		void steerToDesiredPosition(const std::vector<Vector2>& repulsionPoints, float deltaTime);

		void wanderAimlessly(const std::vector<Vector2>& repulsionPoints, float deltaTime);
};

#endif /* STEERINGAGENT_H_ */
