/**
 *	@file MobMoleMadness\Squirrel.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SQUIRREL_H__Included_62669680
#define _SQUIRREL_H__Included_62669680

#include "FVector2.h"
#include "GDE/Core/Observer.h"

namespace GDE
{
	class CDataParser;
	class CSprite;
};
class CCircle;
class CAcorn;

/**
 *	The squirrel, our player's actor and hero of the scene.
 */
class CSquirrel : public GDE::IObserver
{
public:
	~CSquirrel( void );
	/**
	 *	Squirrel movements, to be mapped to input.
	 */
	enum EMovements
	{
		EMove_Left = 0,
		EMove_Right,
		EMove_Up,
		EMove_Down,
		EMove_COUNT		//!< Must come last.
	};

	// for the observer.
	virtual void	NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage );
	bool			Init( GDE::CDataParser* pParser );
	bool			RegisterInput( void );
	void			ReleaseInput( void );
	void			Update( FLOAT32 fTimeDelta );
	void			Draw( void );
	void			Spawn( void );
	bool			IsDead( void ) const { return m_uNumLives == 0; }
	UINT32			GetScore( void ) const { return m_uScore; }
	UINT32			GetLives( void ) const { return m_uNumLives; }
	UINT32			GetAcornCount( void ) const { return m_uNumAcorns; }
	CFVec2			GetPosition( void ) const { return m_vPosition; }
	void			LoseLife( void );
	void			AddLife( void );
	void			ResetLives( void );
	bool			Intersects( const CCircle& Circle ) const;
protected:
	/**
	 *	Structure for the state of each movement.
	 */
	struct SMoveBinding
	{
		//const static UINT32 uMAX_BINDING_LEN = 32;
		//char				m_strBinding[uMAX_BINDING_LEN];
		UINT32				m_uInputCode;
		bool				m_bValue;
	};
	CFVec2					m_vPosition;					//!< The current position.
	SMoveBinding			m_Movements[EMove_COUNT];		//!< Each of the movements possible.
	GDE::CSprite*			m_pSprite;						//!< Visual representation.
	FLOAT32					m_fSpeed;						//!< The speed of the squirrel.
	FLOAT32					m_fRadius;						//!< The radius of the squirrel.
	UINT32					m_uMaxAcorns;					//!< Maximum number of acorns that can be carried.
	CAcorn**				m_ppAcorns;						//!< Current acorns carried.
	UINT32					m_uNumAcorns;					//!< How many acorns are carried.
	UINT32					m_uScore;						//!< Current score.
	UINT32					m_uNumLives;					//!< Current number of lives.
	UINT32					m_uMaxLives;					//!< The maximum number of lives.
	FLOAT32					m_fMinDistSep;					//!< Minimum time between disturbances.
	FLOAT32					m_fMaxDistSep;					//!< Maximum time between disturbances.
	FLOAT32					m_fTimeToDisturbance;			//!< Time to the next disturbance.
	FLOAT32					m_fLastDistDelay;				//!< Last delay length to the disturbance/
	FLOAT32					m_fDistTimeMoving;				//!< Time since last disturbance spent moving.
	FLOAT32					m_fMinDistRad;					//!< Minimum disturbance radius.
	FLOAT32					m_fMaxDistRad;					//!< Maximum disturbance radius.
};

#endif //_SQUIRREL_H__Included_62669680

