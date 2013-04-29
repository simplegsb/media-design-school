#include <d3d10.h>

#include "GrassIsGreenerRenderer.h"

#include <gazengine/direct3d10/model/Direct3D10Mesh.h>

GrassIsGreenerRenderer::GrassIsGreenerRenderer(Direct3D10Renderer* renderer, const SimpleTree& chuckNode) :
	chuckNode(chuckNode),
	renderer(renderer)
{
}

GrassIsGreenerRenderer::~GrassIsGreenerRenderer()
{
	if (renderer != NULL)
	{
		delete renderer;
	}
}

void GrassIsGreenerRenderer::dispose()
{
	renderer->dispose();
}

void GrassIsGreenerRenderer::init()
{
	renderer->init();

	D3DXMATRIX chuckTransformation = chuckNode.getAbsoluteTransformation();
	D3DXVECTOR3 chuckPosition(chuckTransformation._41, chuckTransformation._42, chuckTransformation._43);
	getShader()->setVar("chuckPosition", chuckPosition);
}

void GrassIsGreenerRenderer::render(const Circle& model)
{
	renderer->render(model);
}

void GrassIsGreenerRenderer::render(const Mesh& model)
{
	renderer->render(model);
}
