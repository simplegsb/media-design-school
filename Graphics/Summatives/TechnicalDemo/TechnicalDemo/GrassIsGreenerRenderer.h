#ifndef GRASSISGREENERRENDERER_H_
#define GRASSISGREENERRENDERER_H_

#include <gazengine/direct3d10/rendering/Direct3D10Renderer.h>
#include <gazengine/graph/SimpleTree.h>

class GrassIsGreenerRenderer : public Direct3D10Renderer
{
	public:
		GrassIsGreenerRenderer(Direct3D10Renderer* renderer, const SimpleTree& chuckNode);

		~GrassIsGreenerRenderer();

		void dispose();

		void init();

		void render(const Circle& model);

		void render(const Mesh& model);

	private:
		const SimpleTree& chuckNode;

		Direct3D10Renderer* renderer;

		GrassIsGreenerRenderer(const GrassIsGreenerRenderer& original);

		GrassIsGreenerRenderer& operator=(const GrassIsGreenerRenderer& original);
};

#endif /* GRASSISGREENERRENDERER_H_ */
