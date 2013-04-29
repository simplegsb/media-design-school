#include <gazengine/math/Math.h>

#include "AnimatedCube.h"

using namespace std;

const float HALF_PI = Math::PI * 0.5f;
const float ONE_AND_A_HALF_PI = Math::PI * 1.5f;

AnimatedCube::AnimatedCube(ID3D10Device& device, float animateTime, float pauseTime, float size) :
	animateTime(animateTime),
	elapsedTime(0.0f),
	pauseTime(pauseTime),
	sideA(new SimpleTree),
	sideB(new SimpleTree),
	sideC(new SimpleTree),
	sideD(new SimpleTree),
	sideE(new SimpleTree),
	sideF(new SimpleTree),
	size(size)
{
	sideA->setModel(createSide(device, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	sideB->setModel(createSide(device, size, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));
	sideC->setModel(createSide(device, size, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));
	sideD->setModel(createSide(device, size, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));
	sideE->setModel(createSide(device, size, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));
	sideF->setModel(createSide(device, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

	/*
	    -----
	    | B |
	    |   |
	-----------------
	| E | A | C | F |
	|   |   |   |   |
	-----------------
	    | D |
	    |   |
	    -----
	*/

	sideA->addChild(sideB);
	sideA->addChild(sideC);
	sideA->addChild(sideD);
	sideA->addChild(sideE);
	sideC->addChild(sideF);

	animate(0.0f);
}

void AnimatedCube::animate(float deltaTime)
{
	elapsedTime += deltaTime;

	float fullIterationTime = animateTime + pauseTime;
	int iterationCount = static_cast<int>(floor(elapsedTime / fullIterationTime));
	float iterationStartTime = fullIterationTime * iterationCount;
	float iterationDeltaTime = elapsedTime - iterationStartTime;

	iterationDeltaTime = min(iterationDeltaTime, animateTime);

	if (iterationCount % 2 == 0)
	{
		iterationDeltaTime = animateTime - iterationDeltaTime;
	}

	float angle = Math::PI * (iterationDeltaTime / animateTime);

	D3DXMatrixRotationX(&sideB->getTransformation(), HALF_PI - angle);
	sideB->getTransformation()._42 += size;
	D3DXMatrixRotationY(&sideC->getTransformation(), ONE_AND_A_HALF_PI + angle);
	sideC->getTransformation()._41 += size;
	D3DXMatrixRotationX(&sideD->getTransformation(), HALF_PI + angle);
	D3DXMatrixRotationY(&sideE->getTransformation(), ONE_AND_A_HALF_PI - angle);
	D3DXMatrixRotationY(&sideF->getTransformation(), ONE_AND_A_HALF_PI + angle);
	sideF->getTransformation()._41 += size;
}

Direct3D10Mesh* AnimatedCube::createSide(ID3D10Device& device, float size, D3DXCOLOR color)
{
	// Vertices
	vector<Vertex> vertices(4);
	D3DXVECTOR3 toFirst;
	D3DXVECTOR3 toSecond;

	vertices[0].position = D3DXVECTOR3(0.0f, size, 0.0f);
	vertices[0].color = color;
	vertices[1].position = D3DXVECTOR3(size, size, 0.0f);
	vertices[1].color = color;
	vertices[2].position = D3DXVECTOR3(size, 0.0f, 0.0f);
	vertices[2].color = color;
	vertices[3].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[3].color = color;

	toFirst = D3DXVECTOR3(vertices[2].position - vertices[0].position);
	toSecond = D3DXVECTOR3(vertices[1].position - vertices[0].position);
	D3DXVec3Cross(&vertices[0].normal, &toFirst, &toSecond);
	D3DXVec3Normalize(&vertices[0].normal, &vertices[0].normal);
	toFirst = D3DXVECTOR3(vertices[3].position - vertices[1].position);
	toSecond = D3DXVECTOR3(vertices[2].position - vertices[1].position);
	D3DXVec3Cross(&vertices[1].normal, &toFirst, &toSecond);
	D3DXVec3Normalize(&vertices[1].normal, &vertices[1].normal);
	toFirst = D3DXVECTOR3(vertices[1].position - vertices[2].position);
	toSecond = D3DXVECTOR3(vertices[3].position - vertices[2].position);
	D3DXVec3Cross(&vertices[2].normal, &toFirst, &toSecond);
	D3DXVec3Normalize(&vertices[2].normal, &vertices[2].normal);
	toFirst = D3DXVECTOR3(vertices[2].position - vertices[3].position);
	toSecond = D3DXVECTOR3(vertices[0].position - vertices[3].position);
	D3DXVec3Cross(&vertices[3].normal, &toFirst, &toSecond);
	D3DXVec3Normalize(&vertices[3].normal, &vertices[3].normal);

	// Indices
	vector<DWORD> indices(12);

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;

	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	indices[6] = 0;
	indices[7] = 1;
	indices[8] = 2;

	indices[9] = 2;
	indices[10] = 3;
	indices[11] = 0;

	return new Direct3D10Mesh(device, vertices, indices);
}

SimpleTree* AnimatedCube::getRoot()
{
	return sideA;
}
