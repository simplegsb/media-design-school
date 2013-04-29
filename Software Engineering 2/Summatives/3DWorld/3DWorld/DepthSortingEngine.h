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
#pragma once

#include <gazengine/engine/Engine.h>
#include <gazengine/graph/SimpleTree.h>
#include <gazengine/rendering/Direct3D10RenderingEngine.h>

class CDepthSortingEngine : public Engine
{
	public:
		class CDepthTestComparator
		{
			public:
				CDepthTestComparator(const SimpleTree* _kpCameraNode);

				bool operator()(const SimpleTree* _pLhs, const SimpleTree* _pRhs) const;

			private:
				const SimpleTree* m_kpCameraNode;
		};

		CDepthSortingEngine(SimpleTree* _pUnsortedScene, const SimpleTree* _kpCameraNode);

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& entity);

		void setRenderingEngine(Direct3D10RenderingEngine* _pRenderingEngine);

	private:
		const SimpleTree* m_kpCameraNode;

		Direct3D10RenderingEngine* m_pRenderingEngine;

		SimpleTree* m_pSortedScene;

		SimpleTree* m_pUnsortedScene;

		void getNodes(SimpleTree& _rNode, std::vector<SimpleTree*>& _rNodes);
};
