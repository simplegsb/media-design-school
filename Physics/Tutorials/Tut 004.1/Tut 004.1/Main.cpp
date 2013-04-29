//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//
#include <gazengine/GazEngine.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/model/Text.h>

#include <gazengine/windowing/WinApiEngine.h>

#include <gazengine/model/GDIMesh.h>
#include <gazengine/rendering/GDIRenderer.h>
#include <gazengine/rendering/GDIRenderingEngine.h>

#include <gazengine/model/Direct3D10Mesh.h>
#include <gazengine/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/rendering/Direct3D10ShaderFactory.h>
#include <gazengine/rendering/SimpleDirect3D10Renderer.h>
#include <gazengine/scene/Direct3D10Camera.h>

#include <gazengine/rendering/FreeGLUTRenderer.h>
#include <gazengine/windowing/FreeGLUTEngine.h>

#include <gazengine/model/shape/Sphere.h>
#include <gazengine/rendering/OpenGLRenderingEngine.h>
#include <gazengine/rendering/SimpleOpenGLRenderer.h>
#include <gazengine/scene/OpenGLCamera.h>

void CreateModels(SimpleTree* _pRoot, ID3D10Device& _rDevice)
{
	Entity* pModels = new Entity;

	Text* pText = new Text(Vector2(10.0f, 10.0f), "Rendered in all windows!");
	pModels->addComponent(pText);
	pText->setEntity(pModels);
	SimpleTree* pTextNode = new SimpleTree;
	getTranslation3(pTextNode->getTransformation()).X() = -350.0f;
	getTranslation3(pTextNode->getTransformation()).Y() = 250.0f;
	getTranslation3(pTextNode->getTransformation()).Z() = -1.0f;
	pTextNode->setModel(pText);
	_pRoot->addChild(pTextNode);

	vector<Vector2> gdiVertices;
	gdiVertices.push_back(Vector2(400.0f, 250.0f));
	gdiVertices.push_back(Vector2(300.0f, 350.0f));
	gdiVertices.push_back(Vector2(500.0f, 350.0f));
	GDIMesh* pGdiModel = new GDIMesh(gdiVertices, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pModels->addComponent(pGdiModel);
	pGdiModel->setEntity(pModels);

	vector<Vertex> d3dVertices;
	Vertex vertex0;
	vertex0.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	vertex0.position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	Vertex vertex1;
	vertex1.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	vertex1.position = D3DXVECTOR3(100.0f, -50.0f, 0.0f);
	Vertex vertex2;
	vertex2.color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	vertex2.position = D3DXVECTOR3(-100.0f, -50.0f, 0.0f);
	d3dVertices.push_back(vertex0);
	d3dVertices.push_back(vertex1);
	d3dVertices.push_back(vertex2);
	Direct3D10Mesh* pD3dModel = new Direct3D10Mesh(_rDevice, d3dVertices);
	pModels->addComponent(pD3dModel);
	pD3dModel->setEntity(pModels);
	SimpleTree* pD3dNode = new SimpleTree;
	getTranslation3(pD3dNode->getTransformation()).Z() = -1.0f;
	pD3dNode->setModel(pD3dModel);
	_pRoot->addChild(pD3dNode);

	Sphere* pOpenGlModel = new Sphere(Vector2(0.0f, 0.0f), 100.0f);
	pOpenGlModel->setColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	pOpenGlModel->setLevelOfDetail(10);
	pModels->addComponent(pOpenGlModel);
	pOpenGlModel->setEntity(pModels);
	SimpleTree* pOpenGlNode = new SimpleTree;
	getTranslation3(pOpenGlNode->getTransformation()).Z() = -101.0f;
	pOpenGlNode->setModel(pOpenGlModel);
	_pRoot->addChild(pOpenGlNode);

	GazEngine::addEntity(pModels);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
	// Windows API / GDI
    WinApiEngine* pGdiWindowEngine = new WinApiEngine(L"Tut 004.1 - Windows API / GDI", instance, iCommandShow);
    pGdiWindowEngine->createWindow();
	GazEngine::addEngine(pGdiWindowEngine);

	GDIRenderingEngine* pGdiRenderingEngine = new GDIRenderingEngine(pGdiWindowEngine->getWindow());
	pGdiRenderingEngine->addRenderer(new GDIRenderer);
	pGdiRenderingEngine->setClearingColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	GazEngine::addEngine(pGdiRenderingEngine);

	// Windows API / Direct 3D 10
    WinApiEngine* pD3dWindowEngine = new WinApiEngine(L"Tut 004.1 - Windows API / Direct 3D 10", instance, iCommandShow);
    pD3dWindowEngine->createWindow();
	GazEngine::addEngine(pD3dWindowEngine);

	Direct3D10RenderingEngine* pD3dRenderingEngine = new Direct3D10RenderingEngine(pD3dWindowEngine->getWindow());
	SimpleDirect3D10Renderer* pD3dRenderer = new SimpleDirect3D10Renderer(*pD3dRenderingEngine->getDevice());
	Direct3D10ShaderFactory d3dShaderFactory(*pD3dRenderingEngine->getDevice());
	pD3dRenderer->setShader(d3dShaderFactory.createShader(L"gazengine.fx", "defaultTechnique"));
	pD3dRenderingEngine->addRenderer(pD3dRenderer);
	Camera* pD3dCamera = new Direct3D10Camera;
	//pD3dCamera->lookAt(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f));
	pD3dCamera->setOrthogonal(800.0f, 600.0f);
	pD3dRenderingEngine->setCamera(pD3dCamera);
	pD3dRenderingEngine->setClearingColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	GazEngine::addEngine(pD3dRenderingEngine);

	// Free GLUT / Open GL
	GazEngine::addEngine(new FreeGLUTEngine("Tut 004.1 - Free GLUT / Open GL"));

	OpenGLRenderingEngine* pOpenGlRenderingEngine = new OpenGLRenderingEngine;
	pOpenGlRenderingEngine->addRenderer(new FreeGLUTRenderer(new SimpleOpenGLRenderer));
	Camera* pOpenGlCamera = new OpenGLCamera;
	//pOpenGlCamera->lookAt(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f));
	pOpenGlCamera->setOrthogonal(800.0f, 600.0f);
	pOpenGlRenderingEngine->setCamera(pOpenGlCamera);
	pOpenGlRenderingEngine->setClearingColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	GazEngine::addEngine(pOpenGlRenderingEngine);

	// Tree of Models
	SimpleTree root;
	CreateModels(&root, *pD3dRenderingEngine->getDevice());
	pD3dCamera->setNode(&root);
	pD3dRenderingEngine->setTree(&root);
	pOpenGlCamera->setNode(&root);
	pOpenGlRenderingEngine->setTree(&root);

	// GO!!!
	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
