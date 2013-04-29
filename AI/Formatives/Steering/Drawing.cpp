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
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include "Common.h"

#include "Blackboard.h"
#include "Cat.h"
#include "Circle.h"
#include "Drawing.h"
#include "SteeringContext.h"

namespace Drawing
{
	void DrawSteering(const CSteeringAgent& _krSteeringAgent);
	void DrawTargeting(const CSteeringAgent& _krSteeringAgent);
	void DrawWhisker(const CSteeringAgent& _krSteeringAgent, const CFVec2& _rvWhisker);
	void DrawWhiskers(const CSteeringAgent& _krSteeringAgent);

	void DrawCat(const CCat& _krCat)
	{
		GDE::CRenderer* pRenderer = CSteeringContext::GetInstance().GetRenderer();
		pRenderer->GetTextureManager().GetNULLTexture()->Apply(0);

		CCircle circle;
		circle.Initialise(_krCat.GetPosition(), _krCat.GetRadius());

		UINT32 uiColour = 0xFF0000FF; // blue
		if (_krCat.IsIntersectedByCat())
		{
			uiColour = 0xFFFF0000; // red
		}
		else if (_krCat.GetSteeringAgent()->IsStuck())
		{
			uiColour = 0xFFFF8800; // orange
		}

		CSteeringContext::GetInstance().DrawCircle(circle, uiColour);	

		// now draw the heading:
		CFVec2 vHeading = _krCat.GetHeadingVector();
		vHeading *= _krCat.GetRadius();
		vHeading += _krCat.GetPosition();

		GDE::SVertex vertices[2];
		vertices[0].m_fX = _krCat.GetPosition().X();
		vertices[0].m_fY = _krCat.GetPosition().Y();
		vertices[0].m_fZ = 0.5f;
		vertices[0].m_uColour = uiColour;
		vertices[1].m_fX = vHeading.X();
		vertices[1].m_fY = vHeading.Y();
		vertices[1].m_fZ = 0.5f;
		vertices[1].m_uColour = uiColour;

		pRenderer->DrawPrimitives(GDE::PT_LineList, 1, vertices);
	}

	void DrawSteering(const CSteeringAgent& _krSteeringAgent)
	{
		UINT32 uiColour = 0xFF00FF00; // green

		CFVec2 vFront = _krSteeringAgent.GetCat().GetPosition();
		vFront += _krSteeringAgent.GetCat().GetHeadingVector() * 22.0f;
		CFVec2 vHeading = _krSteeringAgent.GetCat().GetHeadingVector() * _krSteeringAgent.GetSpeed();

		CFVec2 vRotation = vHeading;
		vRotation.RotateZ(_krSteeringAgent.GetRotationalSpeed() / 10.0f);

		GDE::SVertex vertices[3];
		vertices[0].m_fX = vFront.X();
		vertices[0].m_fY = vFront.Y();
		vertices[0].m_fZ = 0.0f;
		vertices[0].m_uColour = uiColour;
		vertices[1].m_fX = vFront.X() + vHeading.X();
		vertices[1].m_fY = vFront.Y() + vHeading.Y();
		vertices[1].m_fZ = 0.0f;
		vertices[1].m_uColour = uiColour;
		vertices[2].m_fX = vFront.X() + vRotation.X();
		vertices[2].m_fY = vFront.Y() + vRotation.Y();
		vertices[2].m_fZ = 0.0f;
		vertices[2].m_uColour = uiColour;

		GDE::CRenderer* pRenderer = CSteeringContext::GetInstance().GetRenderer();
		pRenderer->DrawPrimitives(GDE::PT_TriangleList, 1, vertices);
	}

	void DrawSteeringAgent(const CSteeringAgent& _krSteeringAgent)
	{
		DrawSteering(_krSteeringAgent);
		DrawTargeting(_krSteeringAgent);
		DrawWhiskers(_krSteeringAgent);
	}

	void DrawTargeting(const CSteeringAgent& _krSteeringAgent)
	{
		UINT32 uiTargetColour = 0xFF00FF00; // green
		UINT32 uiBearingColour = 0xFFFF8800; // orange
		GDE::CRenderer* pRenderer = CSteeringContext::GetInstance().GetRenderer();

		CCircle circle;
		circle.Initialise(CBlackboard::GetInstance().GetMousePosition(), _krSteeringAgent.GetApproachRadius());

		CSteeringContext::GetInstance().DrawCircle(circle, uiTargetColour);

		GDE::SVertex vertices[2];
		vertices[0].m_fX = _krSteeringAgent.GetCat().GetPosition().X();
		vertices[0].m_fY = _krSteeringAgent.GetCat().GetPosition().Y();
		vertices[0].m_fZ = 0.0f;
		vertices[0].m_uColour = uiTargetColour;
		vertices[1].m_fX = CBlackboard::GetInstance().GetMousePosition().X();
		vertices[1].m_fY = CBlackboard::GetInstance().GetMousePosition().Y();
		vertices[1].m_fZ = 0.0f;
		vertices[1].m_uColour = uiTargetColour;

		pRenderer->DrawPrimitives(GDE::PT_LineList, 1, vertices);

		vertices[0].m_fX = _krSteeringAgent.GetCat().GetPosition().X();
		vertices[0].m_fY = _krSteeringAgent.GetCat().GetPosition().Y();
		vertices[0].m_fZ = 0.0f;
		vertices[0].m_uColour = uiBearingColour;
		vertices[1].m_fX = _krSteeringAgent.GetCat().GetPosition().X() + _krSteeringAgent.GetBearing().X() *
			_krSteeringAgent.GetCat().GetRadius();
		vertices[1].m_fY = _krSteeringAgent.GetCat().GetPosition().Y() + _krSteeringAgent.GetBearing().Y() *
			_krSteeringAgent.GetCat().GetRadius();
		vertices[1].m_fZ = 0.0f;
		vertices[1].m_uColour = uiBearingColour;

		pRenderer->DrawPrimitives(GDE::PT_LineList, 1, vertices);
	}

	void DrawWhisker(const CSteeringAgent& _krSteeringAgent, const CFVec2& _rvWhisker)
	{
		CFVec2 vAdjustedWhisker = _krSteeringAgent.GetAdjustedWhisker(_rvWhisker);
		CFVec2 vAbsoluteWhiskerStart = _krSteeringAgent.GetAbsoluteWhiskerStart(_rvWhisker);
		CFVec2 vAbsoluteWhiskerEnd = vAbsoluteWhiskerStart + vAdjustedWhisker;

		UINT32 uiColour = 0xFF00FF00; // green

		float fWhiskerIntersectionTime =
			CBlackboard::GetInstance().GetClosestWallIntersectionTime(vAbsoluteWhiskerStart, vAdjustedWhisker);
		if (fWhiskerIntersectionTime != -1.0f)
		{
			uiColour = 0xFFFF0000; // red
		}

		GDE::SVertex vertices[2];
		vertices[0].m_fX = vAbsoluteWhiskerStart.X();
		vertices[0].m_fY = vAbsoluteWhiskerStart.Y();
		vertices[0].m_fZ = 0.0f;
		vertices[0].m_uColour = uiColour;
		vertices[1].m_fX = vAbsoluteWhiskerEnd.X();
		vertices[1].m_fY = vAbsoluteWhiskerEnd.Y();
		vertices[1].m_fZ = 0.0f;
		vertices[1].m_uColour = uiColour;

		GDE::CRenderer* pRenderer = CSteeringContext::GetInstance().GetRenderer();
		pRenderer->DrawPrimitives(GDE::PT_LineList, 1, vertices);
	}

	void DrawWhiskers(const CSteeringAgent& _krSteeringAgent)
	{
		CFVec2 vScaledWhiskerFront = _krSteeringAgent.GetWhiskerFront() * _krSteeringAgent.GetSpeed();
		DrawWhisker(_krSteeringAgent, vScaledWhiskerFront);

		for (unsigned int uiIndex = 0 ; uiIndex < SIDE_WHISKER_COUNT; uiIndex++)
		{
			DrawWhisker(_krSteeringAgent, _krSteeringAgent.GetWhiskersLeft()[uiIndex]);
		}

		for (unsigned int uiIndex = 0 ; uiIndex < SIDE_WHISKER_COUNT; uiIndex++)
		{
			DrawWhisker(_krSteeringAgent, _krSteeringAgent.GetWhiskersRight()[uiIndex]);
		}
	}
}
