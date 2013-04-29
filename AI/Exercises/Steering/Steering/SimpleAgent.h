#ifndef SIMPLEAGENT_H_
#define SIMPLEAGENT_H_

#include <vector>

#include "Agent.h"
#include "Vector2.h"

class SimpleAgent : public Agent
{
	public:
		SimpleAgent();

		virtual ~SimpleAgent();

		void think(const std::vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints,
			float deltaTime);

	private:
		float distanceTravelled;

		float speed;

		bool wanderingAimlessly;

		void determineDesiredPosition(const std::vector<Vector2>& waypoints);

		void followPath(const std::vector<Vector2>& waypoints, float deltaTime);

		void moveTowardDesiredPosition(float deltaTime);

		void wanderAimlessly(float deltaTime);
};

#endif /* SIMPLEAGENT_H_ */
