#ifndef INFLUENCEMAP_H_
#define INFLUENCEMAP_H_

#include <vector>

#include "Vector2.h"

class InfluenceMap
{
	public:
		InfluenceMap(int width, int height, int gridElementSize);

		void addRepulsionPoint(const Vector2& repulsionPoint);

		Vector2 getInfluence(int x, int y) const;

		int getInfluenceScalar(int x, int y) const;

	private:
		std::vector<std::vector<int> > grid;

		int gridElementSize;

		int maxInfluence;

		void addInfluence(int x, int y, int xDirection, int yDirection, int influence);
};

#endif /* INFLUENCEMAP_H_ */
