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
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#pragma once

#include <list>

#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/graph/AABB2.h>
#include <gazengine/model/Model.h>

class CSnakeModel : public Model
{
	public:
		CSnakeModel(unsigned int _uiInitialLength, unsigned int _uiGrowLength, const Vector2& _vInitialPosition,
			float _fGirth, COLORREF _colour);

		~CSnakeModel();

		std::vector<AABB2> getBoxes() const;

		AABB2 getHeadBox() const;

		Texture* getNormalMap() const;

		PrimitiveType getPrimitiveType() const;

		std::vector<Vector2> getSegmentPositions() const;

		Texture* getTexture() const;

		void grow();

		bool isVisible() const;

		void moveHeadDown();

		void moveHeadLeft();

		void moveHeadRight();

		void moveHeadUp();

		void moveTailToHead();

		void render(Renderer& _rRenderer) const;

		void setNormalMap(Texture* _pTexture);

		void setSegmentPositions(const std::vector<Vector2>& _krSegmentPositions,
			unsigned int _uiSegmentPositionCount);

		void setTexture(Texture* _pTexture);

		void setVisible(bool _bVisible);

		void switchTailToHead();

	private:
		bool m_bVisible;

		float m_fGirth;

		std::list<GDIMesh*> m_segments;

		unsigned int m_uiGrowLength;

		AABB2 getBox(std::list<GDIMesh*>::const_iterator _segmentIter) const;

		Vector2 getCenter(const GDIMesh& _krSegment) const;
};
