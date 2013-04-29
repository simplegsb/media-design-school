/**
 *	@file MobMoleMadness\Acorn.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Acorn.h"
#include "Circle.h"
#include "Box2D.h"

#include "MMMContext.h"

#include "GDE/Core/Rect.h"

#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"


CAcorn::~CAcorn( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Release();
	}
}


/**
 *	Initialise the acorn at a given position and texture name, returning succes.
 */
bool CAcorn::Init( CFVec2Arg vPosition, const char *strTexture )
{
	m_eState = ES_Available;

	m_bHighlighted = false;
	m_vPosition = vPosition;
	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->FindResouceById( GDE::CResourceId( strTexture ) );
	if ( NULL == m_pSprite )
	{
		GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource( strTexture );
		if ( NULL == pTexture )
		{
			assert_now( "Unable to find Image \"%s\" for acorn", strTexture );
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
	return true;
}

void CAcorn::Draw( void )
{
	if ( m_eState != ES_Available )
	{
		return;
	}
	m_pSprite->Render( m_vPosition.X(), m_vPosition.Y() );
	if ( m_bHighlighted )
	{
		CCircle Bounds;
		Bounds.Initialise( m_vPosition, fACORN_RADIUS );
		CMMMContext::GetInstance().DrawCircle( Bounds, 0xFFFF0000 );
	}
}

bool CAcorn::Intersects( const CCircle& Circle ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fACORN_RADIUS );
	return Circle.Overlaps( Bounds );
}

bool CAcorn::Intersects( const CBox2D& Box ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fACORN_RADIUS );
	return Box.Intersects( Bounds );
}

void CAcorn::SetState( EState eState )
{
	m_eState = eState;
}