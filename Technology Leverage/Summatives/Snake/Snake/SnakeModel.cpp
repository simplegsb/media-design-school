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
#include "SnakeModel.h"

using namespace std;

CSnakeModel::CSnakeModel(unsigned int _uiInitialLength, unsigned int _uiGrowLength, const Vector2& _vInitialPosition,
		float _fGirth, COLORREF _colour) :
	m_bVisible(true),
	m_fGirth(_fGirth),
	m_segments(),
	m_uiGrowLength(_uiGrowLength)
{
	float fHalfGirth = _fGirth / 2.0f;
	Vector2 vInitialSegmentPosition = _vInitialPosition;

	for (unsigned int uiIndex = 0; uiIndex < _uiInitialLength; uiIndex++)
	{
		std::vector<Vector2> vertices(5, vInitialSegmentPosition);
		vertices[0].x -= fHalfGirth;
		vertices[0].y -= fHalfGirth;
		vertices[1].x += fHalfGirth;
		vertices[1].y -= fHalfGirth;
		vertices[2].x += fHalfGirth;
		vertices[2].y += fHalfGirth;
		vertices[3].x -= fHalfGirth;
		vertices[3].y += fHalfGirth;
		vertices[4].x -= fHalfGirth;
		vertices[4].y -= fHalfGirth;

		m_segments.push_back(new GDIMesh(vertices, _colour));
		vInitialSegmentPosition.y += _fGirth;
	}
}

CSnakeModel::~CSnakeModel()
{
	for (list<GDIMesh*>::iterator segmentIter = m_segments.begin();
		segmentIter != m_segments.end(); segmentIter = m_segments.erase(segmentIter))
	{
		delete *segmentIter;
	}
}

Vector2 CSnakeModel::getCenter(const GDIMesh& _krSegment) const
{
	float fGirth = _krSegment.getVertices()[1].x - _krSegment.getVertices()[0].x;
	float fHalfGirth = fGirth / 2.0f;

	return Vector2(_krSegment.getVertices()[0].x + fHalfGirth,
			_krSegment.getVertices()[0].y + fHalfGirth);
}

AABB2 CSnakeModel::getBox(list<GDIMesh*>::const_iterator _segmentIter) const
{
	float fGirth = (*_segmentIter)->getVertices()[1].x - (*_segmentIter)->getVertices()[0].x;
	float fHalfGirth = fGirth / 2.0f;

	AABB2 headBox;
	headBox.center = getCenter(**_segmentIter);
	headBox.halfDimension = fHalfGirth;

	return headBox;
}

vector<AABB2> CSnakeModel::getBoxes() const
{
	vector<AABB2> boxes;
	boxes.reserve(m_segments.size());

	for (list<GDIMesh*>::const_iterator segmentIter = m_segments.begin();
		segmentIter != m_segments.end(); segmentIter++)
	{
		boxes.push_back(getBox(segmentIter));
	}

	return boxes;
}

AABB2 CSnakeModel::getHeadBox() const
{
	return getBox(m_segments.begin());
}

Texture* CSnakeModel::getNormalMap() const
{
	return NULL;
}

Model::PrimitiveType CSnakeModel::getPrimitiveType() const
{
	return Model::NA;
}

vector<Vector2> CSnakeModel::getSegmentPositions() const
{
	vector<Vector2> segmentPositions;
	segmentPositions.reserve(m_segments.size());

	for (list<GDIMesh*>::const_iterator segmentIter = m_segments.begin();
		segmentIter != m_segments.end(); segmentIter++)
	{
		segmentPositions.push_back(getCenter(**segmentIter));
	}

	return segmentPositions;
}

Texture* CSnakeModel::getTexture() const
{
	return NULL;
}

void CSnakeModel::grow()
{
	list<GDIMesh*>::iterator segmentIter = m_segments.end();
	segmentIter--;
	GDIMesh* pTailModel = *segmentIter;
	segmentIter--;
	GDIMesh* pAnteTailModel = *segmentIter;
	float fDifferenceX = pTailModel->getVertices()[0].x - pAnteTailModel->getVertices()[0].x;
	float fDifferenceY = pTailModel->getVertices()[0].y - pAnteTailModel->getVertices()[0].y;

	vector<Vector2> vertices = pTailModel->getVertices();
	for (unsigned int uiIndex = 0; uiIndex < vertices.size(); uiIndex++)
	{
		vertices[uiIndex].x += fDifferenceX;
		vertices[uiIndex].y += fDifferenceY;
	}

	m_segments.push_back(new GDIMesh(vertices, pTailModel->getColour()));
}

bool CSnakeModel::isVisible() const
{
	return m_bVisible;
}

void CSnakeModel::moveHeadDown()
{
	for (unsigned int uiIndex = 0; uiIndex < m_segments.back()->getVertices().size(); uiIndex++)
	{
		m_segments.front()->getVertices()[uiIndex].y += m_fGirth;
	}
}

void CSnakeModel::moveHeadLeft()
{
	for (unsigned int uiIndex = 0; uiIndex < m_segments.back()->getVertices().size(); uiIndex++)
	{
		m_segments.front()->getVertices()[uiIndex].x -= m_fGirth;
	}
}

void CSnakeModel::moveHeadRight()
{
	for (unsigned int uiIndex = 0; uiIndex < m_segments.back()->getVertices().size(); uiIndex++)
	{
		m_segments.front()->getVertices()[uiIndex].x += m_fGirth;
	}
}

void CSnakeModel::moveHeadUp()
{
	for (unsigned int uiIndex = 0; uiIndex < m_segments.back()->getVertices().size(); uiIndex++)
	{
		m_segments.front()->getVertices()[uiIndex].y -= m_fGirth;
	}
}

void CSnakeModel::moveTailToHead()
{
	for (unsigned int uiIndex = 0; uiIndex < m_segments.back()->getVertices().size(); uiIndex++)
	{
		m_segments.back()->getVertices()[uiIndex].x = m_segments.front()->getVertices()[uiIndex].x;
		m_segments.back()->getVertices()[uiIndex].y = m_segments.front()->getVertices()[uiIndex].y;
	}
}

void CSnakeModel::render(Renderer& _rRenderer) const
{
	for (list<GDIMesh*>::const_iterator segmentIter = m_segments.begin();
		segmentIter != m_segments.end(); segmentIter++)
	{
		_rRenderer.render(**segmentIter);
	}
}

void CSnakeModel::setNormalMap(Texture*)
{
}

void CSnakeModel::setSegmentPositions(const vector<Vector2>& _krSegmentPositions, unsigned int _uiSegmentPositionCount)
{
	float fGirth = m_segments.front()->getVertices()[1].x - m_segments.front()->getVertices()[0].x;
	float fHalfGirth = fGirth / 2.0f;

	list<GDIMesh*>::iterator segmentIter = m_segments.begin();
	for (unsigned int uiIndex = 0; uiIndex < _uiSegmentPositionCount; uiIndex++)
	{
		Vector2 segmentPosition = _krSegmentPositions[uiIndex];

		GDIMesh* segment;
		if (segmentIter == m_segments.end())
		{
			segment = new GDIMesh(vector<Vector2>(5), RGB(0, 0, 0));
			m_segments.push_back(segment);
		}
		else
		{
			segment = *segmentIter;
			segmentIter++;
		}

		std::vector<Vector2>& vertices = segment->getVertices();
		vertices[0].x = segmentPosition.x - fHalfGirth;
		vertices[0].y = segmentPosition.y - fHalfGirth;
		vertices[1].x = segmentPosition.x + fHalfGirth;
		vertices[1].y = segmentPosition.y - fHalfGirth;
		vertices[2].x = segmentPosition.x + fHalfGirth;
		vertices[2].y = segmentPosition.y + fHalfGirth;
		vertices[3].x = segmentPosition.x - fHalfGirth;
		vertices[3].y = segmentPosition.y + fHalfGirth;
		vertices[4].x = segmentPosition.x - fHalfGirth;
		vertices[4].y = segmentPosition.y - fHalfGirth;
	}
}

void CSnakeModel::setTexture(Texture*)
{
}

void CSnakeModel::setVisible(bool _bVisible)
{
	m_bVisible = _bVisible;
}

void CSnakeModel::switchTailToHead()
{
	m_segments.push_front(m_segments.back());
	m_segments.pop_back();
}
