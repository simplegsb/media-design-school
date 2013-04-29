/**
 *	@file MobMoleMadness\Mole.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Mole.h"
#include "Circle.h"
#include "Box2D.h"
#include "MMMContext.h"
#include "AStarSearch.h"
#include "Blackboard.h"
#include "Events.h"
#include "Math.h"

#ifndef _EDITOR
#include "Squirrel.h"
#endif


#include "GDE/Core/Rect.h"

#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

using namespace std;

CMole::CMole( void )
#ifndef _EDITOR
	: m_pMoleAgent(NULL)
#endif
{
}

CMole::~CMole( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Release();
	}

#ifndef _EDITOR
	if (m_pMoleAgent != NULL)
	{
		delete m_pMoleAgent;
	}
#endif
}

/**
 *	Initialise the Mole at a given position and texture name, returning succes.
 */
bool CMole::Init( CFVec2Arg vPosition, const char *strTexture, FLOAT32 fHeading, FLOAT32 fDisturbanceRange, FLOAT32 fSightRange )
{
	m_bHighlighted = false;
	m_vPosition = vPosition;
	m_fHeading = fHeading;
	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->FindResouceById( GDE::CResourceId( strTexture ) );
	if ( NULL == m_pSprite )
	{
		GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource( strTexture );
		if ( NULL == pTexture )
		{
			assert_now( "Unable to find Image \"%s\" for Mole", strTexture );
			return false;
		}

		GDE::SFRect TexRect;
		TexRect.m_fX = 0.0f;
		TexRect.m_fY = 0.0f;
		TexRect.m_fWidth = (FLOAT32)pTexture->GetWidth();
		TexRect.m_fHeight = (FLOAT32)pTexture->GetHeight();
		m_pSprite = CMMMContext::GetInstance().GetSpriteM()->CreateSprite( GDE::CResourceId( strTexture ), pTexture, TexRect );
		m_pSprite->SetHotSpot( TexRect.m_fWidth*0.5f, TexRect.m_fHeight*0.5f );
		pTexture->Release();	// now held inside the sprite.
	}

	m_vDestination = SFVec2(Math::GetRandomFloat(0.0f, 800.0f), Math::GetRandomFloat(0.0f, 600.0f));

#ifndef _EDITOR
	m_pMoleAgent = new CMoleAgent(*this, fDisturbanceRange, fSightRange);
#else
	fDisturbanceRange;
	fSightRange;
#endif

	return true;
}

void CMole::Draw( void )
{
	m_pSprite->RenderEx( m_vPosition.X(), m_vPosition.Y(), m_fHeading, 1.0f, 1.0f );
	if ( m_bHighlighted )
	{
		CCircle Bounds;
		Bounds.Initialise( m_vPosition, fMOLE_RADIUS );
		CMMMContext::GetInstance().DrawCircle( Bounds, 0xFFFF0000 );
	}

#ifndef _EDITOR
	if (CMMMContext::GetInstance().IsDrawingMoleState())
	{
		CCircle radiusCircle;
		radiusCircle.Initialise(m_vPosition, fMOLE_RADIUS);
		CMMMContext::GetInstance().DrawCircle(radiusCircle, 0xFF00FF00);

		CCircle destinationCircle;
		destinationCircle.Initialise(m_vDestination, 5.0f );
		CMMMContext::GetInstance().DrawCircle(destinationCircle, 0xFF8800FF);
	}

	m_pMoleAgent->Draw();
#endif
}

bool CMole::Intersects( const CCircle& Circle ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fMOLE_RADIUS );
	return Circle.Overlaps( Bounds );
}

bool CMole::Intersects( const CBox2D& Box ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fMOLE_RADIUS );
	return Box.Intersects( Bounds );
}

#ifndef _EDITOR
void CMole::Update(FLOAT32 _fTimeDelta)
{
	m_pMoleAgent->Update(_fTimeDelta);

	// Turn.
	m_fHeading += m_pMoleAgent->GetRotationalSpeed() * _fTimeDelta;

	// Update position.
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_fHeading);
	CFVec2 vVelocity = vHeading * m_pMoleAgent->GetSpeed() * _fTimeDelta;
	m_vPosition += vVelocity;

	if ((m_vDestination - m_vPosition).Magnitude() < fMOLE_RADIUS)
	{
		Messages::send(DESTINATION_REACHED_EVENT, this);
	}

	// finally check if that pesky rodent got caught:
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fMOLE_RADIUS );
	if ( CMMMContext::GetInstance().GetSquirrel()->Intersects( Bounds ) )
	{
		CMMMContext::GetInstance().GetSquirrel()->LoseLife();
		Messages::send(DESTINATION_REACHED_EVENT, this);
	}
}
#endif
