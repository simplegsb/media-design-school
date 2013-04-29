//=======================================================================================
// Hills.h                                                                         
//=======================================================================================

#ifndef Hills_H
#define Hills_H

#include "Utility.h"

class Hills
{
public:
	Hills();
	~Hills();

	float getHeight(float x, float z)const;
 
	void init(ID3D10Device* device, DWORD m, DWORD n, float dx);
	void update(float dt);
	void draw();

private:
	DWORD mNumRows;
	DWORD mNumCols;

	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

#endif // Hills_H