//=======================================================================================
// Hills.cpp                                                                         
//=======================================================================================

#include "Cube.h"

Cube::Cube()
: mSize(0), mNumFaces(6), mNumVertices(8), md3dDevice(NULL), mVB(NULL), mIB(NULL)
{
}

Cube::~Cube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

float Cube::getHeight(float x, float z)const
{
	return mSize;
}

void Cube::init(ID3D10Device* device, float size)
{
	md3dDevice = device;
	mSize = size;

	// Create the geometry and fill the vertex buffer. 
	std::vector<Vertex> vertices = createVertexData(BEACH_SAND);
 
    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.
	std::vector<DWORD> indices = createIndexData();
	
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces * 2 * 3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Cube::update(float dt)
{
}

std::vector<DWORD> Cube::createIndexData()
{
	std::vector<DWORD> indices(mNumFaces * 2 * 3); // 2 triangles per face, 3 vertices per triangle

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 7;
	indices[9] = 5;
	indices[10] = 6;
	indices[11] = 7;

	indices[12] = 0;
	indices[13] = 1;
	indices[14] = 5;
	indices[15] = 5;
	indices[16] = 4;
	indices[17] = 0;

	indices[18] = 3;
	indices[19] = 7;
	indices[20] = 2;
	indices[21] = 7;
	indices[22] = 6;
	indices[23] = 2;

	indices[24] = 6;
	indices[25] = 5;
	indices[26] = 1;
	indices[27] = 1;
	indices[28] = 2;
	indices[29] = 6;

	indices[30] = 0;
	indices[31] = 4;
	indices[32] = 7;
	indices[33] = 7;
	indices[34] = 3;
	indices[35] = 0;

	return indices;
}

std::vector<Vertex> Cube::createVertexData(D3DXCOLOR color)
{
	std::vector<Vertex> vertices(mNumVertices);
	
	vertices[0].pos = D3DXVECTOR3(-mSize, mSize, mSize);
	vertices[0].color = color;
	vertices[1].pos = D3DXVECTOR3(mSize, mSize, mSize);
	vertices[1].color = color;
	vertices[2].pos = D3DXVECTOR3(mSize, -mSize, mSize);
	vertices[2].color = color;
	vertices[3].pos = D3DXVECTOR3(-mSize, -mSize, mSize);
	vertices[3].color = color;
	vertices[4].pos = D3DXVECTOR3(-mSize, mSize, -mSize);
	vertices[4].color = color;
	vertices[5].pos = D3DXVECTOR3(mSize, mSize, -mSize);
	vertices[5].color = color;
	vertices[6].pos = D3DXVECTOR3(mSize, -mSize, -mSize);
	vertices[6].color = color;
	vertices[7].pos = D3DXVECTOR3(-mSize, -mSize, -mSize);
	vertices[7].color = color;

	return vertices;
}

void Cube::draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->DrawIndexed(mNumFaces * 2 * 3, 0, 0);
}

void Cube::drawFace(int faceIndex)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->DrawIndexed(2 * 3, faceIndex * 2 * 3, 0);
}
