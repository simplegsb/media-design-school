#include <ctime>
#include <sstream>
#include <time.h>

#include "WindowApp.h"

class InitDirect3DApp : public WindowApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene();

protected:
	std::wstring adapterDetails;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	InitDirect3DApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
: adapterDetails(), WindowApp(hInstance) 
{
}

InitDirect3DApp::~InitDirect3DApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();
}

void InitDirect3DApp::initApp()
{
	WindowApp::initApp();

	IDXGIDevice* device = NULL;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **) &device);
	IDXGIAdapter* adapter = NULL;
	device->GetAdapter(&adapter);
	DXGI_ADAPTER_DESC adapterDescription;
	adapter->GetDesc(&adapterDescription);

	std::wostringstream outs;   
	outs.precision(6);
	outs << L"Adapter details:" << std::endl;
	outs << L"Description: " << adapterDescription.Description << std::endl;
	outs << L"Vendor ID: " << adapterDescription.VendorId << std::endl;
	outs << L"Device ID: " << adapterDescription.DeviceId << std::endl;
	outs << L"Sub-system ID: " << adapterDescription.SubSysId << std::endl;
	outs << L"Revision: " << adapterDescription.Revision << std::endl;
	outs << L"Dedicated video memory: " << adapterDescription.DedicatedVideoMemory << std::endl;
	outs << L"Dedicated system memory: " << adapterDescription.DedicatedSystemMemory << std::endl;
	outs << L"Shared system memory: " << adapterDescription.SharedSystemMemory << std::endl;
	adapterDetails = outs.str();
}

void InitDirect3DApp::onResize()
{
	WindowApp::onResize();
}

void InitDirect3DApp::updateScene(float dt)
{
	WindowApp::updateScene(dt);
}

void InitDirect3DApp::drawScene()
{
	WindowApp::drawScene();

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);

	if (mShowDateTime)
	{
		RECT R2 = {5, 75, 0, 0};
		std::time_t time = std::time(NULL);
		wchar_t* stringTime = _wasctime(std::localtime(&time));
		mFont->DrawText(0, stringTime, -1, &R2, DT_NOCLIP, BLACK);
	}

	if (mShowAdapter)
	{
		RECT R3 = {5, 100, 0, 0};
		mFont->DrawText(0, adapterDetails.data(), -1, &R3, DT_NOCLIP, BLACK);
	}

	mSwapChain->Present(0, 0);
}
