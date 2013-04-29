/**
 *	@file Connect4\GameBoard.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GAMEBOARD_H__Included_1638590223
#define _GAMEBOARD_H__Included_1638590223

#include "Board.h"


#define _NO_MALLOC_CLONE
#ifdef _NO_MALLOC_CLONE
# pragma warning( push )
# pragma warning( disable: 4127 )
# include <list>
# pragma warning ( pop )
#endif

//#define _FACTOR_IN_POTENTIAL

#ifdef _FACTOR_IN_POTENTIAL
const static int iSCORE_MULTIPLIER = 10000;
#else
const static int iSCORE_MULTIPLIER = 1;
#endif

/**
 *	Game side implementation of the board.
 */
class CGameBoard : public CBoard
{
public:
	// dll interface methods:
	virtual ~CGameBoard( void );
	virtual int GetWidth( void ) const { return m_iWidth; }
	virtual int GetHeight( void ) const { return m_iHeight; }
	virtual EBoardPos GetBoardPos( int x, int y ) const;
	virtual CBoard* CreateClone( void ) const;
	virtual void	ReleaseClone( void );
	virtual int EvaluateScore( void ) const
	{
		sm_uEvaluationCount++;
#ifndef _FACTOR_IN_POTENTIAL
		return EvaluateScore( PLAYER1 ) - EvaluateScore( PLAYER2 );
#else
		return EvaluateScorePotential( PLAYER1 ) - EvaluateScorePotential( PLAYER2 );
#endif
	}
	virtual bool MakeMove( int iColumn, EBoardPos ePlayer );

	virtual bool PositionToColumn( FLOAT32 fX, FLOAT32 fY, int& iColumn ) const;
	virtual bool GetMouseInfo( FLOAT32& fX, FLOAT32& fY, bool& bWasClicked ) const;
	virtual bool HighlightPossibleMove( int iColumn, int iPlayerIdentifier );

	// game specific methods:
	void InitCloneSystem( void );
	void ShutdownCloneSystem( void );
	void Draw( void );
	void Init( int iWidth, int iHeight );
	void Reset( void );
	void SetBoardPos( int iX, int iY, EBoardPos ePlayer );

	int EvaluateScore( EBoardPos ePlayer ) const;
#ifdef _FACTOR_IN_POTENTIAL
	int EvaluateScorePotential( EBoardPos ePlayer ) const;
#endif

	bool IsComplete( void ) const
	{
		return m_iNumFree == 0;
	}
	static void		ResetEvaluationCount( void );
	static UINT32	GetEvaluationCount( void )
	{
		return sm_uEvaluationCount;
	}
protected:
	inline EBoardPos GetBoardPosFast( int x, int y ) const
	{
		return m_pPositions[x+y*m_iWidth];
	}

	enum EPointEval
	{
		OURS = 0,
		FREE = 1,
		BLOCKED = 2
	};

	inline EPointEval EvaluatePoint( int x, int y, EBoardPos ePlayer ) const
	{
		EBoardPos ePos = GetBoardPosFast( x, y );
		if ( ePlayer == ePos )
		{
			return OURS;
		}
		if ( CBoard::EMPTY == ePos )
		{
			return FREE;
		}
		return BLOCKED;
	}

	int					m_iWidth;
	int					m_iHeight;
	int					m_iNumFree;
	int					m_iColumnHighlighted;
	EBoardPos*			m_pPositions;
	static UINT32		sm_uEvaluationCount;
	CGameBoard*			m_pCloneSource;

#ifdef _NO_MALLOC_CLONE
	mutable std::list<CGameBoard*>	m_CloneList;
#endif
};

#endif //_GAMEBOARD_H__Included_1638590223

