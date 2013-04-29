/**
 *	@file MobMoleMadness\Mole.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MOLE_H__Included_399921487
#define _MOLE_H__Included_399921487

#include "FVector2.h"
#ifndef _EDITOR
#include "MoleAgent.h"
#endif

namespace GDE
{
	class CSprite;
};

class CCircle;
class CBox2D;

/**
 *	The moles or enemy agents.
 */
class CMole
{
public:
	CMole( void );
	~CMole( void );
	bool		Init( CFVec2Arg vPos, const char* strSprite, FLOAT32 fHeading, FLOAT32 fDisturbanceRange, FLOAT32 fSightRange );
	void		Draw( void );
#ifndef _EDITOR
	void		Update( FLOAT32 fTimeDelta );
#endif
	bool		Intersects( const CCircle& Circle ) const;
	bool		Intersects( const CBox2D& Box ) const;
	CFVec2Arg	GetPosition( void ) const { return m_vPosition; }
	FLOAT32		GetHeading( void ) const { return m_fHeading; }
	const CFVec2&	GetDestination( void ) const { return m_vDestination; }
	void			SetDestination( const CFVec2& _vDestination ) { m_vDestination = _vDestination; }

	void SetHighlighted( bool bHighlighted ) { m_bHighlighted = bHighlighted; }
	bool IsHighlighted( void ) const { return m_bHighlighted; }

protected:
	CFVec2			m_vPosition;		//!< Position of the mole
	FLOAT32			m_fHeading;			//!< Heading of the mole, in radians, clockwise from looking upwards.
	GDE::CSprite*	m_pSprite;			//!< Sprite used to draw the mole.
	bool			m_bHighlighted;		//!< Is this highlighted?

#ifndef _EDITOR
	CMoleAgent*		m_pMoleAgent;
#endif

	CFVec2			m_vDestination;
};

#endif //_MOLE_H__Included_399921487

