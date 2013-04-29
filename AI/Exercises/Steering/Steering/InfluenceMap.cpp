#include <cmath>

#include "InfluenceMap.h"

using namespace std;

InfluenceMap::InfluenceMap(int width, int height, int gridElementSize) :
	grid(width, vector<int>(height, 0)), gridElementSize(gridElementSize), maxInfluence(10)
{
}

void InfluenceMap::addInfluence(int x, int y, int xDirection, int yDirection, int influence)
{
	grid.at(x).at(y) += influence;

	if (influence == 1)
	{
		return;
	}

	if (xDirection != 0)
	{
		addInfluence(x + xDirection, y, xDirection, 0, influence - 1);
	}

	if (yDirection != 0)
	{
		addInfluence(x, y + yDirection, 0, yDirection, influence - 1);
	}

	if (xDirection != 0 && yDirection != 0)
	{
		addInfluence(x + xDirection, y + yDirection, xDirection, yDirection, influence - 1);
	}
}

void InfluenceMap::addRepulsionPoint(const Vector2& repulsionPoint)
{
	int x = floor(repulsionPoint.x + 0.5) / gridElementSize;
	int y = floor(repulsionPoint.y + 0.5) / gridElementSize;

	addInfluence(x, y, 0, 0, maxInfluence);

	addInfluence(x - 1, y, -1, 0, maxInfluence - 1);
	addInfluence(x + 1, y, 1, 0, maxInfluence - 1);
	addInfluence(x, y - 1, 0, -1, maxInfluence - 1);
	addInfluence(x, y + 1, 0, 1, maxInfluence - 1);

	addInfluence(x - 1, y - 1, -1, -1, maxInfluence - 1);
	addInfluence(x - 1, y + 1, -1, 1, maxInfluence - 1);
	addInfluence(x + 1, y - 1, 1, -1, maxInfluence - 1);
	addInfluence(x + 1, y + 1, 1, 1, maxInfluence - 1);
}

Vector2 InfluenceMap::getInfluence(int x, int y) const
{
	return Vector2();
}

int InfluenceMap::getInfluenceScalar(int x, int y) const
{
	return grid.at(x).at(y);
}
