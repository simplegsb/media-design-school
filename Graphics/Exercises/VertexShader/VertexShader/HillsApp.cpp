//=============================================================================
// Peaks and Valleys App.cpp                                                                         
//
// Demonstrates how to construct and draw the geometry of a triangle grid.
//
// Controls:
//		None.
//
//=============================================================================

#include "WindowApp.h"
#include "Hills.h"
#include "Cube.h"


class HillsApp : public WindowApp
{
public:
	HillsApp(HINSTANCE hInstance);
	~HillsApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();
 
private:

	Hills mLand;
	Cube mCube;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10Buffer* mVertexBuffer;
	ID3D10Buffer* mIndexBuffer;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectScalarVariable* mfxGameTime;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	float mRadius;
	float mTheta;
	float mPhi;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	HillsApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

HillsApp::HillsApp(HINSTANCE hInstance)
: WindowApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mVertexBuffer(0), mIndexBuffer(0), mfxWVPVar(0), mfxGameTime(0), 
  mRadius(100.0f), mTheta(0.0f), mPhi(PI*0.4f)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
}

HillsApp::~HillsApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}

void HillsApp::initApp()
{
	WindowApp::initApp();
	
	mLand.init(md3dDevice, 129, 129, 1.0f);
	mCube.init(md3dDevice, 10.0f);

	buildFX();
	buildVertexLayouts();
}

void HillsApp::onResize()
{
	WindowApp::onResize();

	float aspect = float(mClientWidth)/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void HillsApp::updateScene(float dt)
{
	WindowApp::updateScene(dt);

	// Update camera position as a function of time.
	float x = mRadius*cosf(0.5f*mTimer.getGameTime());
	float z = mRadius*sinf(0.5f*mTimer.getGameTime());
	float y = 50.0f*sinf(0.5f*mTimer.getGameTime()) + 50.0f;
	 
	// Build the view matrix.
	D3DXVECTOR3 pos(x, y, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
}

void HillsApp::drawScene()
{
	WindowApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		mWVP = mView*mProj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxGameTime->SetFloat(getTimer().getGameTime());
        mTech->GetPassByIndex( p )->Apply(0);
		//mLand.draw();
		mCube.draw();
    }

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);

	mSwapChain->Present(0, 0);
}

void HillsApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxGameTime = mFX->GetVariableByName("gameTime")->AsScalar();
}

void HillsApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}
