#ifndef ANIMATEDCUBE_H_
#define ANIMATEDCUBE_H_

#include <gazengine/graph/SimpleTree.h>
#include <gazengine/direct3d10/model/Direct3D10Mesh.h>

class AnimatedCube
{
	public:
		AnimatedCube(ID3D10Device& device, float animateTime, float pauseTime, float size);

		void animate(float deltaTime);

		SimpleTree* getRoot();

	private:
		float animateTime;

		float elapsedTime;

		float pauseTime;

		SimpleTree* sideA;

		SimpleTree* sideB;

		SimpleTree* sideC;

		SimpleTree* sideD;

		SimpleTree* sideE;

		SimpleTree* sideF;

		float size;

		Direct3D10Mesh* createSide(ID3D10Device& device, float size, D3DXCOLOR color);
};

#endif /* ANIMATEDCUBE_H_ */

