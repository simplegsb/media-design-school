#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include <gazengine/direct3d10/model/Direct3D10LODMesh.h>
#include <gazengine/direct3d10/model/Direct3D10ModelFactory.h>
#include <gazengine/direct3d10/rendering/DepthClearingDirect3D10Renderer.h>
#include <gazengine/direct3d10/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/direct3d10/rendering/Direct3D10ShaderFactory.h>
#include <gazengine/direct3d10/rendering/SimpleDirect3D10Renderer.h>
#include <gazengine/direct3d10/rendering/StencilClearingDirect3D10Renderer.h>
#include <gazengine/Events.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/Messages.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "AnimatedCube.h"
#include "GrassIsGreenerRenderer.h"

using namespace std;

void advanceIntro();
void createCamera();
void createLights();
void createModels();
void createSceneGraph();
void createTreeModels(float size0, float size1, float size2);
void enterIntroPhase1();
void enterIntroPhase2();
void enterIntroPhase3();
void exitIntro();
DWORD WINAPI loadChuckOnThread(LPVOID data);
DWORD WINAPI loadDecorationsOnThread(LPVOID data);
void onKeyboardButton(const void* message);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

Direct3D10Camera* camera = NULL;
SimpleTree* cameraNode = NULL;
Direct3D10Renderer* chuckIntroRenderer = NULL;
Direct3D10Shader* chuckIntroShader = NULL;
SimpleTree* chuckNode = NULL;
Direct3D10Renderer* chuckRenderer = NULL;
AnimatedCube* cube = NULL;
SimpleTree* cubeNode = NULL;
SimpleTree* decorationNode = NULL;
Direct3D10Renderer* decorationRenderer = NULL;
bool endIntro = false;
SimpleTree* introCameraNode = NULL;
SimpleTree* introCameraPivotNode = NULL;
bool introPhase1Complete = false;
bool introPhase2Complete = false;
bool introPhase3Complete = false;
bool introStarted = false;
bool isChuckCartoon = true;
bool isCloseLODUsed = false;
bool isGreyUsed = true;
bool isNormalMapUsed = true;
bool isOutlineRendered = true;
bool isShadowVolumeRendered = false;
Direct3D10Renderer* outlineRenderer = NULL;
Direct3D10RenderingEngine* renderingEngine = NULL;
Direct3D10ShaderFactory* shaderFactory = NULL;
Timer timer;
Direct3D10LODMesh* tree0 = NULL;
Direct3D10LODMesh* tree1 = NULL;
Direct3D10LODMesh* tree2 = NULL;
Direct3D10LODMesh* tree3 = NULL;
Direct3D10LODMesh* tree4 = NULL;
Direct3D10LODMesh* tree5 = NULL;
Direct3D10LODMesh* tree6 = NULL;
Direct3D10LODMesh* tree7 = NULL;
Direct3D10Mesh* treeLOD0 = NULL;
Direct3D10Mesh* treeLOD1 = NULL;
Direct3D10Mesh* treeLOD2 = NULL;
SimpleTree* treeNode0 = NULL;
SimpleTree* treeNode1 = NULL;
SimpleTree* treeNode2 = NULL;
SimpleTree* treeNode3 = NULL;
SimpleTree* treeNode4 = NULL;
SimpleTree* treeNode5 = NULL;
SimpleTree* treeNode6 = NULL;
SimpleTree* treeNode7 = NULL;
Direct3D10Renderer* worldIntroRenderer = NULL;
Direct3D10Shader* worldIntroShader = NULL;
SimpleTree* worldNode = NULL;
SimpleTree* worldNode0 = NULL;
SimpleTree* worldNode1 = NULL;
SimpleTree* worldNode2 = NULL;
SimpleTree* worldNode3 = NULL;
SimpleTree* worldNode4 = NULL;
Direct3D10Renderer* worldRenderer = NULL;

void advanceIntro()
{
	if (timer.getGameTime() < 5.0f)
	{
		if (!introStarted)
		{
			enterIntroPhase1();
		}

		worldIntroShader->setVar("effectTime", timer.getGameTime());
		introCameraPivotNode->getTransformation()._43 -= 15.0f * timer.getDeltaTime();
	}
	else if (timer.getGameTime() >= 5.0f && timer.getGameTime() < 10.0f)
	{
		if (!introPhase1Complete)
		{
			enterIntroPhase2();
		}

		worldIntroShader->setVar("effectTime", timer.getGameTime());
		introCameraPivotNode->getTransformation()._43 += 4.98f * timer.getDeltaTime();

		D3DXMATRIX rotation;
		D3DXMatrixRotationX(&rotation, Math::PI / 2.0f / 5.0f * timer.getDeltaTime() * -1.0f);
		introCameraPivotNode->setTransformation(rotation * introCameraPivotNode->getTransformation());
		introCameraNode->getTransformation()._43 -= 0.1f * timer.getDeltaTime();
	}
	else if (timer.getGameTime() >= 10.0f && timer.getGameTime() < 22.0f)
	{
		if (!introPhase2Complete)
		{
			enterIntroPhase3();
		}

		chuckIntroShader->setVar("effectTime", timer.getGameTime() - 10.0f);
		introCameraPivotNode->getTransformation()._43 -= 0.06f * timer.getDeltaTime();

		D3DXMATRIX rotation;
		D3DXMatrixRotationY(&rotation, Math::PI / 4.0f * timer.getDeltaTime() * -1.0f);
		introCameraNode->getTransformation() *= rotation;
	}
	else if (timer.getGameTime() >= 23.0f && timer.getGameTime() < 25.0f)
	{
		introCameraPivotNode->getTransformation()._43 -= 0.09f * timer.getDeltaTime();

		D3DXMATRIX rotation;
		D3DXMatrixRotationY(&rotation, Math::PI / 2.0f * timer.getDeltaTime() * -1.0f);
		introCameraNode->getTransformation() *= rotation;
	}
	else if (timer.getGameTime() >= 25.0f && timer.getGameTime() < 27.0f)
	{
		introCameraNode->getTransformation()._43 -= 0.75f * timer.getDeltaTime();
	}
	else if (timer.getGameTime() >= 27.0f  && !endIntro)
	{
		endIntro = true;
	}

	if (endIntro && !introPhase3Complete)
	{
		exitIntro();
	}
}

void createCamera()
{
	camera = new Direct3D10Camera();
	camera->setPerspective(0.25f * Math::PI, float(800) / 600);
	renderingEngine->setCamera(camera);
}

void createLights()
{
	Direct3D10Light* ambient = new Direct3D10Light("ambient");
	ambient->setAmbientComponent(D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f));
	ambient->setDiffuseComponent(D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f));
	ambient->setDirection(D3DXVECTOR3(-0.3f, -0.3f, 0.4f));
	ambient->setSpecularComponent(D3DXCOLOR(0.05f, 0.05f, 0.05f, 1.0f));
	renderingEngine->addLight(ambient);

	Direct3D10Light* sun = new Direct3D10Light("sun");
	sun->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	sun->setAttenuation(D3DXVECTOR3(0.05f, 0.0005f, 0.00005f));
	sun->setDirection(D3DXVECTOR3(-0.3f, -0.3f, 0.4f));
	sun->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	sun->setRange(1000.0f);
	sun->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	sun->setStrength(0.1f);
	sun->setTranslation(D3DXVECTOR3(150.0f, 150.0f, -150.0f));
	renderingEngine->addLight(sun);
}

void createModels()
{
	LPDWORD threadId = NULL;
	HANDLE chuckThread = CreateThread(NULL, 0, &loadChuckOnThread, NULL, 0, threadId);
	if(chuckThread == NULL)
    {
		loadChuckOnThread(NULL);
    }
	HANDLE decorationThread = CreateThread(NULL, 0, &loadDecorationsOnThread, NULL, 0, threadId);
	if(decorationThread == NULL)
    {
		loadDecorationsOnThread(NULL);
    }

	vector<Vertex> worldVertices;
	Direct3D10ModelFactory::loadObj("world.obj", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, worldVertices, 4902, 4902,
		5149, 9800 * 3);

	vector<Vertex> world0Vertices = worldVertices;
	Direct3D10ModelFactory::scaleVertices(world0Vertices, 10.0f);
	Direct3D10Mesh* world0 = new Direct3D10Mesh(*renderingEngine->getDevice(), world0Vertices);
	world0->setNormalMap(new Direct3D10Texture(*renderingEngine->getDevice(), L"normal.jpg", "normalMap"));
	worldNode0->setModel(world0);

	vector<Vertex> world1Vertices = worldVertices;
	Direct3D10ModelFactory::colorizeVertices(world1Vertices, D3DXCOLOR(1.0f, 0.95f, 0.0f, 1.0f));
	Direct3D10ModelFactory::scaleVertices(world1Vertices, 20.0f);
	Direct3D10Mesh* world1 = new Direct3D10Mesh(*renderingEngine->getDevice(), world1Vertices);
	world1->setNormalMap(new Direct3D10Texture(*renderingEngine->getDevice(), L"normal.jpg", "normalMap"));
	worldNode1->setModel(world1);

	vector<Vertex> world2Vertices = worldVertices;
	Direct3D10ModelFactory::colorizeVertices(world2Vertices, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	Direct3D10ModelFactory::scaleVertices(world2Vertices, 30.0f);
	Direct3D10Mesh* world2 = new Direct3D10Mesh(*renderingEngine->getDevice(), world2Vertices);
	world2->setNormalMap(new Direct3D10Texture(*renderingEngine->getDevice(), L"normal.jpg", "normalMap"));
	worldNode2->setModel(world2);

	vector<Vertex> world3Vertices = worldVertices;
	Direct3D10ModelFactory::colorizeVertices(world3Vertices, D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f));
	Direct3D10ModelFactory::scaleVertices(world3Vertices, 40.0f);
	Direct3D10Mesh* world3 = new Direct3D10Mesh(*renderingEngine->getDevice(), world3Vertices);
	world3->setNormalMap(new Direct3D10Texture(*renderingEngine->getDevice(), L"normal.jpg", "normalMap"));
	worldNode3->setModel(world3);

	vector<Vertex> world4Vertices = worldVertices;
	Direct3D10ModelFactory::colorizeVertices(world4Vertices, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	Direct3D10ModelFactory::scaleVertices(world4Vertices, 50.0f);
	Direct3D10Mesh* world4 = new Direct3D10Mesh(*renderingEngine->getDevice(), world4Vertices);
	world4->setNormalMap(new Direct3D10Texture(*renderingEngine->getDevice(), L"normal.jpg", "normalMap"));
	worldNode4->setModel(world4);
}

SimpleDirect3D10Renderer* ambientRenderer = NULL;
SimpleDirect3D10Renderer* shadowRenderer = NULL;
SimpleDirect3D10Renderer* nonShadowRenderer = NULL;

void createRenderers()
{
	shaderFactory = new Direct3D10ShaderFactory(*renderingEngine->getDevice());

	worldIntroRenderer = new SimpleDirect3D10Renderer();
	worldIntroShader = shaderFactory->createShader(L"technical_demo.fx", "worldIntroTechnique");
	worldIntroRenderer->setShader(worldIntroShader);

	chuckIntroRenderer = new SimpleDirect3D10Renderer();
	chuckIntroShader = shaderFactory->createShader(L"technical_demo.fx", "chuckIntroTechnique");
	chuckIntroRenderer->setShader(chuckIntroShader);

	chuckRenderer = new SimpleDirect3D10Renderer();
	chuckRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "chuckTechnique"));

	decorationRenderer = new SimpleDirect3D10Renderer();
	decorationRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "decorationTechnique"));

	worldRenderer = new GrassIsGreenerRenderer(new SimpleDirect3D10Renderer(), *chuckNode);
	worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldTechnique"));

	outlineRenderer = new SimpleDirect3D10Renderer();
	outlineRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "outlineTechnique"));

	ambientRenderer = new SimpleDirect3D10Renderer();
	ambientRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "ambientTechnique"));

	//shadowRenderer = new SimpleDirect3D10Renderer();
	//shadowRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "shadowVolumeTechnique"));

	nonShadowRenderer = new SimpleDirect3D10Renderer();
	nonShadowRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "nonShadowTechnique"));
}

void createSceneGraph()
{
	SimpleTree* rootNode = new SimpleTree();
	renderingEngine->setTree(rootNode);

	introCameraPivotNode = new SimpleTree();
	rootNode->addChild(introCameraPivotNode);

	introCameraNode = new SimpleTree();
	introCameraPivotNode->addChild(introCameraNode);
	camera->setNode(introCameraNode);

	chuckNode = new SimpleTree();
	rootNode->addChild(chuckNode);
	chuckNode->getTransformation()._43 = -50.0f;
	D3DXMATRIX chuckRotation;
	D3DXMatrixRotationX(&chuckRotation, Math::PI / 2.0f * -1.0f);
	chuckNode->setTransformation(chuckRotation * chuckNode->getTransformation());

	cameraNode = new SimpleTree();
	chuckNode->addChild(cameraNode);
	cameraNode->getTransformation()._42 = 1.0f;
	cameraNode->getTransformation()._43 = -2.0f;

	decorationNode = new SimpleTree();
	rootNode->addChild(decorationNode);
	D3DXMATRIX decorationRotation;

	SimpleTree* treeNode = new SimpleTree();
	decorationNode->addChild(treeNode);

	treeNode0 = new SimpleTree();
	decorationNode->addChild(treeNode0);
	treeNode0->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.4f * -1.0f);
	treeNode0->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.03f * -1.0f);
	treeNode0->getTransformation() *= decorationRotation;

	treeNode1 = new SimpleTree();
	decorationNode->addChild(treeNode1);
	treeNode1->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.4f * -1.0f);
	treeNode1->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.03f);
	treeNode1->getTransformation() *= decorationRotation;

	treeNode2 = new SimpleTree();
	decorationNode->addChild(treeNode2);
	treeNode2->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.425f * -1.0f);
	treeNode2->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.055f * -1.0f);
	treeNode2->getTransformation() *= decorationRotation;

	treeNode3 = new SimpleTree();
	decorationNode->addChild(treeNode3);
	treeNode3->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.425f * -1.0f);
	treeNode3->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.055f);
	treeNode3->getTransformation() *= decorationRotation;

	treeNode4 = new SimpleTree();
	decorationNode->addChild(treeNode4);
	treeNode4->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.45f * -1.0f);
	treeNode4->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.06f * -1.0f);
	treeNode4->getTransformation() *= decorationRotation;

	treeNode5 = new SimpleTree();
	decorationNode->addChild(treeNode5);
	treeNode5->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.45f * -1.0f);
	treeNode5->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.06f);
	treeNode5->getTransformation() *= decorationRotation;

	treeNode6 = new SimpleTree();
	decorationNode->addChild(treeNode6);
	treeNode6->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.475f * -1.0f);
	treeNode6->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.075f * -1.0f);
	treeNode6->getTransformation() *= decorationRotation;

	treeNode7 = new SimpleTree();
	decorationNode->addChild(treeNode7);
	treeNode7->getTransformation()._42 = 50.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.475f * -1.0f);
	treeNode7->getTransformation() *= decorationRotation;
	D3DXMatrixRotationZ(&decorationRotation, Math::PI * 0.075f);
	treeNode7->getTransformation() *= decorationRotation;

	cubeNode = new SimpleTree();
	decorationNode->addChild(cubeNode);
	cubeNode->getTransformation()._42 = 51.0f;
	D3DXMatrixRotationX(&decorationRotation, Math::PI * 0.35f * -1.0f);
	cubeNode->getTransformation() *= decorationRotation;

	worldNode = new SimpleTree();
	rootNode->addChild(worldNode);

	worldNode0 = new SimpleTree();
	worldNode->addChild(worldNode0);

	worldNode1 = new SimpleTree();
	worldNode->addChild(worldNode1);

	worldNode2 = new SimpleTree();
	worldNode->addChild(worldNode2);

	worldNode3 = new SimpleTree();
	worldNode->addChild(worldNode3);

	worldNode4 = new SimpleTree();
	worldNode->addChild(worldNode4);
}

void createTreeModels(float size0, float size1, float size2)
{
	if (tree0 != NULL)
	{
		delete tree0;
		delete tree1;
		delete tree2;
		delete tree3;
		delete tree4;
		delete tree5;
		delete tree6;
		delete tree7;
	}

	if (treeLOD2 == NULL)
	{
		return;
	}

	tree0 = new Direct3D10LODMesh();
	tree0->addLOD(treeLOD0, size0);
	tree0->addLOD(treeLOD1, size1);
	tree0->addLOD(treeLOD2, size2);
	treeNode0->setModel(tree0);
	tree0->setCameraNode(cameraNode);
	tree0->setNode(treeNode0);

	tree1 = new Direct3D10LODMesh();
	tree1->addLOD(treeLOD0, size0);
	tree1->addLOD(treeLOD1, size1);
	tree1->addLOD(treeLOD2, size2);
	treeNode1->setModel(tree1);
	tree1->setCameraNode(cameraNode);
	tree1->setNode(treeNode1);

	tree2 = new Direct3D10LODMesh();
	tree2->addLOD(treeLOD0, size0);
	tree2->addLOD(treeLOD1, size1);
	tree2->addLOD(treeLOD2, size2);
	treeNode2->setModel(tree2);
	tree2->setCameraNode(cameraNode);
	tree2->setNode(treeNode2);

	tree3 = new Direct3D10LODMesh();
	tree3->addLOD(treeLOD0, size0);
	tree3->addLOD(treeLOD1, size1);
	tree3->addLOD(treeLOD2, size2);
	treeNode3->setModel(tree3);
	tree3->setCameraNode(cameraNode);
	tree3->setNode(treeNode3);

	tree4 = new Direct3D10LODMesh();
	tree4->addLOD(treeLOD0, size0);
	tree4->addLOD(treeLOD1, size1);
	tree4->addLOD(treeLOD2, size2);
	treeNode4->setModel(tree4);
	tree4->setCameraNode(cameraNode);
	tree4->setNode(treeNode4);

	tree5 = new Direct3D10LODMesh();
	tree5->addLOD(treeLOD0, size0);
	tree5->addLOD(treeLOD1, size1);
	tree5->addLOD(treeLOD2, size2);
	treeNode5->setModel(tree5);
	tree5->setCameraNode(cameraNode);
	tree5->setNode(treeNode5);

	tree6 = new Direct3D10LODMesh();
	tree6->addLOD(treeLOD0, size0);
	tree6->addLOD(treeLOD1, size1);
	tree6->addLOD(treeLOD2, size2);
	treeNode6->setModel(tree6);
	tree6->setCameraNode(cameraNode);
	tree6->setNode(treeNode6);

	tree7 = new Direct3D10LODMesh();
	tree7->addLOD(treeLOD0, size0);
	tree7->addLOD(treeLOD1, size1);
	tree7->addLOD(treeLOD2, size2);
	treeNode7->setModel(tree7);
	tree7->setCameraNode(cameraNode);
	tree7->setNode(treeNode7);
}

void enterIntroPhase1()
{
	introStarted = true;

	worldIntroShader->setVar("effectOrigin", D3DXVECTOR3(0.0f, 0.0f, 10.0f));
	worldIntroShader->setVar("effectSpeed", 10.0f);
	renderingEngine->addRenderer(worldIntroRenderer);
	renderingEngine->setRendererRoot(*worldIntroRenderer, *worldNode);
}

void enterIntroPhase2()
{
	if (!introStarted)
	{
		enterIntroPhase1();
	}

	introPhase1Complete = true;

	renderingEngine->addRenderer(decorationRenderer);
	renderingEngine->setRendererRoot(*decorationRenderer, *decorationNode);
}

void enterIntroPhase3()
{
	if (!introStarted)
	{
		enterIntroPhase1();
	}

	if (!introPhase1Complete)
	{
		enterIntroPhase2();
	}

	introPhase2Complete = true;

	chuckIntroShader->setVar("effectOrigin", D3DXVECTOR3(0.0f, 0.0f, -50.0f));
	chuckIntroShader->setVar("effectSpeed", 0.08f);
	renderingEngine->addRenderer(chuckIntroRenderer);
	renderingEngine->setRendererRoot(*chuckIntroRenderer, *chuckNode);

	renderingEngine->removeRenderer(*worldIntroRenderer);
	renderingEngine->addRenderer(worldRenderer);
	renderingEngine->setRendererRoot(*worldRenderer, *worldNode);
}

void exitIntro()
{
	if (!introStarted)
	{
		enterIntroPhase1();
	}

	if (!introPhase1Complete)
	{
		enterIntroPhase2();
	}

	if (!introPhase2Complete)
	{
		enterIntroPhase3();
	}

	introPhase3Complete = true;

	camera->setNode(cameraNode);

	renderingEngine->removeRenderer(*chuckIntroRenderer);
	renderingEngine->addRenderer(chuckRenderer);
	renderingEngine->setRendererRoot(*chuckRenderer, *chuckNode);

	renderingEngine->addRenderer(outlineRenderer);

	//renderingEngine->addRenderer(ambientRenderer);
	//renderingEngine->addRenderer(shadowRenderer);
	//renderingEngine->addRenderer(nonShadowRenderer);
}

DWORD WINAPI loadChuckOnThread(LPVOID data)
{
	vector<Vertex> chuckVertices;
	Direct3D10ModelFactory::loadObj("chuck-smooth.obj", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.005f, chuckVertices, 0, 1560,
		2378, 2984 * 3);
	Direct3D10Mesh* chuck = new Direct3D10Mesh(*renderingEngine->getDevice(), chuckVertices);
	chuck->setTexture(new Direct3D10Texture(*renderingEngine->getDevice(), L"chuck.bmp"));
	chuckNode->setModel(chuck);

	return 0;
}

DWORD WINAPI loadDecorationsOnThread(LPVOID data)
{
	cube = new AnimatedCube(*renderingEngine->getDevice(), 5.0f, 1.0f, 1.0f);
	cubeNode->addChild(cube->getRoot());

	vector<Vertex> tree0Vertices;
	Direct3D10ModelFactory::loadObj("tree_lod0.obj", D3DXCOLOR(0.0f, 0.75f, 0.0f, 1.0f), 1.0f, tree0Vertices,
		5055 + 4698, 5126 + 4794, 247 + 7, 11377 * 3 + 8298 * 3);
	treeLOD0 = new Direct3D10Mesh(*renderingEngine->getDevice(), tree0Vertices);
	vector<Vertex> tree1Vertices;
	Direct3D10ModelFactory::loadObj("tree_lod1.obj", D3DXCOLOR(0.0f, 0.75f, 0.0f, 1.0f), 1.0f, tree1Vertices,
		867 + 767, 879 + 788, 58 + 7, 3231 * 3 + 1932 * 3);
	treeLOD1 = new Direct3D10Mesh(*renderingEngine->getDevice(), tree1Vertices);
	vector<Vertex> tree2Vertices;
	Direct3D10ModelFactory::loadObj("tree_lod2.obj", D3DXCOLOR(0.0f, 0.75f, 0.0f, 1.0f), 1.0f, tree2Vertices,
		248 + 207, 257 + 226, 23 + 7, 1174 * 3 + 611 * 3);
	treeLOD2 = new Direct3D10Mesh(*renderingEngine->getDevice(), tree2Vertices);

	createTreeModels(5.0f, 10.0f, 15.0f);

	return 0;
}

void onKeyboardButton(const void* message)
{
	const KeyboardButtonEvent* keyboardButtonEvent = static_cast<const KeyboardButtonEvent*>(message);
	if (keyboardButtonEvent->buttonState != Button::DOWN)
	{
		return;
	}

	if (introPhase3Complete && keyboardButtonEvent->character == 'A')
	{
		D3DXMATRIX rotation;
		D3DXVECTOR3 rotationAxis(chuckNode->getTransformation()._41, chuckNode->getTransformation()._42, chuckNode->getTransformation()._43);
		D3DXMatrixRotationAxis(&rotation, &rotationAxis, Math::PI * -0.01f);
		chuckNode->getTransformation() *= rotation;
	}
	else if (introPhase3Complete && keyboardButtonEvent->character == 'D')
	{
		D3DXMATRIX rotation;
		D3DXVECTOR3 rotationAxis(chuckNode->getTransformation()._41, chuckNode->getTransformation()._42, chuckNode->getTransformation()._43);
		D3DXMatrixRotationAxis(&rotation, &rotationAxis, Math::PI * 0.01f);
		chuckNode->getTransformation() *= rotation;
	}
	else if (introPhase3Complete && keyboardButtonEvent->character == 'S')
	{
		D3DXVECTOR3 originalPosition(chuckNode->getTransformation()._41, chuckNode->getTransformation()._42, chuckNode->getTransformation()._43);

		// Get the new position.
		D3DXMATRIX positionOnlyTransformation;
		D3DXMatrixIdentity(&positionOnlyTransformation);
		positionOnlyTransformation._43 = -0.3f;
		positionOnlyTransformation = positionOnlyTransformation * chuckNode->getTransformation();

		D3DXVECTOR3 newPosition(positionOnlyTransformation._41, positionOnlyTransformation._42, positionOnlyTransformation._43);
		D3DXVec3Normalize(&newPosition, &newPosition);
		newPosition *= 50.0f;

		// Get the rotation axis.
		D3DXVECTOR3 rotationAxis;
		D3DXVec3Cross(&rotationAxis, &originalPosition, &newPosition);
		D3DXMATRIX rotation;
		D3DXMatrixRotationAxis(&rotation, &rotationAxis, Math::PI * 0.005f);
		chuckNode->getTransformation() *= rotation;
	}
	else if (introPhase3Complete && keyboardButtonEvent->character == 'W')
	{
		D3DXVECTOR3 originalPosition(chuckNode->getTransformation()._41, chuckNode->getTransformation()._42, chuckNode->getTransformation()._43);

		// Get the new position.
		D3DXMATRIX positionOnlyTransformation;
		D3DXMatrixIdentity(&positionOnlyTransformation);
		positionOnlyTransformation._43 = 0.3f;
		positionOnlyTransformation = positionOnlyTransformation * chuckNode->getTransformation();

		D3DXVECTOR3 newPosition(positionOnlyTransformation._41, positionOnlyTransformation._42, positionOnlyTransformation._43);
		D3DXVec3Normalize(&newPosition, &newPosition);
		newPosition *= 50.0f;

		// Get the rotation axis.
		D3DXVECTOR3 rotationAxis;
		D3DXVec3Cross(&rotationAxis, &originalPosition, &newPosition);
		D3DXMATRIX rotation;
		D3DXMatrixRotationAxis(&rotation, &rotationAxis, Math::PI * 0.005f);
		chuckNode->getTransformation() *= rotation;
	}
	else if (!introPhase3Complete && keyboardButtonEvent->button == Keyboard::F1)
	{
		endIntro = true;
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F2)
	{
		if (isCloseLODUsed)
		{
			createTreeModels(5.0f, 10.0f, 15.0f);

			isCloseLODUsed = false;
		}
		else
		{
			createTreeModels(2.5f, 5.0f, 7.5f);

			isCloseLODUsed = true;
		}
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F3)
	{
		isNormalMapUsed = !isNormalMapUsed;

		renderingEngine->removeRenderer(*worldRenderer);
		worldRenderer = new GrassIsGreenerRenderer(new SimpleDirect3D10Renderer(), *chuckNode);

		if (isGreyUsed)
		{
			if (isNormalMapUsed)
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldTechnique"));
			}
			else
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoNormalTechnique"));
			}
		}
		else
		{
			if (isNormalMapUsed)
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoGreyTechnique"));
			}
			else
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoGreyNormalTechnique"));
			}
		}

		renderingEngine->addRenderer(worldRenderer);
		renderingEngine->setRendererRoot(*worldRenderer, *worldNode);
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F4)
	{
		renderingEngine->removeRenderer(*chuckRenderer);
		chuckRenderer = new SimpleDirect3D10Renderer();

		if (isChuckCartoon)
		{
			chuckRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "chuckNoCartoonTechnique"));

			isChuckCartoon = false;
		}
		else
		{
			chuckRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "chuckTechnique"));

			isChuckCartoon = true;
		}

		renderingEngine->addRenderer(chuckRenderer);
		renderingEngine->setRendererRoot(*chuckRenderer, *chuckNode);
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F5)
	{
		if (isOutlineRendered)
		{
			renderingEngine->removeRenderer(*outlineRenderer);

			isOutlineRendered = false;
		}
		else
		{
			outlineRenderer = new SimpleDirect3D10Renderer();
			outlineRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "outlineTechnique"));
			renderingEngine->addRenderer(outlineRenderer);

			isOutlineRendered = true;
		}
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F6)
	{
		isGreyUsed = !isGreyUsed;

		renderingEngine->removeRenderer(*worldRenderer);
		worldRenderer = new GrassIsGreenerRenderer(new SimpleDirect3D10Renderer(), *chuckNode);

		if (isGreyUsed)
		{
			if (isNormalMapUsed)
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldTechnique"));
			}
			else
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoNormalTechnique"));
			}
		}
		else
		{
			if (isNormalMapUsed)
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoGreyTechnique"));
			}
			else
			{
				worldRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "worldNoGreyNormalTechnique"));
			}
		}

		renderingEngine->addRenderer(worldRenderer);
		renderingEngine->setRendererRoot(*worldRenderer, *worldNode);
	}
	else if (introPhase3Complete && keyboardButtonEvent->button == Keyboard::F7)
	{
		if (isShadowVolumeRendered)
		{
			renderingEngine->removeRenderer(*shadowRenderer);

			isShadowVolumeRendered = false;
		}
		else
		{
			//renderingEngine->addRenderer(ambientRenderer);

			shadowRenderer = new SimpleDirect3D10Renderer();
			shadowRenderer->setShader(shaderFactory->createShader(L"technical_demo.fx", "shadowVolumeTechnique"));
			renderingEngine->addRenderer(shadowRenderer);

			//renderingEngine->addRenderer(nonShadowRenderer);

			isShadowVolumeRendered = true;
		}
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"GD2P04 - Technical Demo - The Grass is Always Greener...", instance, commandShow);
    windowEngine->init();

	renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	createCamera();
	createLights();
	createSceneGraph();
	createRenderers();
	createModels();

	Messages::registerRecipient(KEYBOARD_BUTTON_EVENT, &onKeyboardButton);

	timer.reset();

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		timer.tick();

		advanceIntro();
		cube->animate(timer.getDeltaTime());

		windowEngine->advance();
        renderingEngine->advance();
    }

	renderingEngine->destroy();
    windowEngine->destroy();

	delete treeLOD0;
	delete treeLOD1;
	delete treeLOD2;

    return windowEngine->getMessage().wParam;
}
