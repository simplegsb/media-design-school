//=======================================================================================
// Hills.h                                                                         
//=======================================================================================

#ifndef Cube_H
#define Cube_H

#include <vector>

#include "Utility.h"
#include "Vertex.h"

class Cube
{
public:
	Cube();

	~Cube();

	std::vector<DWORD> createIndexData();

	std::vector<Vertex> createVertexData(D3DXCOLOR color);

	void draw();

	void drawFace(int faceIndex);

	float getHeight(float x, float z)const;

	void init(ID3D10Device* device, float mSize);

	void update(float dt);

private:
	float mSize;
	DWORD mNumFaces;
	DWORD mNumVertices;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

#endif // Cube_H