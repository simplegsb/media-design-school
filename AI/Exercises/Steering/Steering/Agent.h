#ifndef AGENT_H_
#define AGENT_H_

#include <vector>

#include "Vector2.h"

class Agent
{
	public:
		Agent()
		{
			desiredPosition = Vector2(400.0f, 400.0f);
			desiredPositionRadius = 5.0f;
		}

		virtual ~Agent()
		{
		}

		Vector2 desiredPosition;

		float desiredPositionRadius;

		Vector2 position;

		Vector2 velocity;

		virtual void think(const std::vector<Vector2>& repulsionPoints, const std::vector<Vector2>& waypoints,
			float deltaTime) = 0;
};

#endif /* AGENT_H_ */
