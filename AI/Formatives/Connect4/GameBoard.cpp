/**
 *	@file Connect4\GameBoard.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "GameBoard.h"
#include "Circle.h"
#include "Connect4Context.h"
#include "Mouse.h"

#ifdef _NO_MALLOC_CLONE
const static UINT32 uMAX_CLONES = 100;
#endif

UINT32 CGameBoard::sm_uEvaluationCount = 0;

const FLOAT32 fPIECE_RADIUS = 20.0f;
const FLOAT32 fPIECE_SPACING = 6.0f;
const UINT32 colPLAYER_ONE = 0xFFFF0000;
const UINT32 colPLAYER_TWO = 0xFFFFFF00;
const UINT32 colEMPTY = 0xFF000000;
const UINT32 colHIGHLIGHT = 0xFF00FF00;

void CGameBoard::InitCloneSystem( void )
{
#ifdef _NO_MALLOC_CLONE
	for ( UINT32 i=0;i<uMAX_CLONES; i++ )
	{
		CGameBoard* pClone = new CGameBoard;
		pClone->Init( m_iWidth, m_iHeight );
		pClone->m_pCloneSource = this;
		m_CloneList.push_front( pClone );
	}
#endif
}

void CGameBoard::ShutdownCloneSystem( void )
{
#ifdef _NO_MALLOC_CLONE
	for ( UINT32 i=0;i<uMAX_CLONES; i++ )
	{
		CGameBoard* pClone = m_CloneList.front();
		if ( NULL == pClone )
		{
			assert_now( "Not all clones were released back to the pool" );
			return;
		}
		m_CloneList.pop_front();
		delete pClone;
	}
#endif
}


CGameBoard::~CGameBoard( void )
{
	delete[] m_pPositions;
}

void CGameBoard::ReleaseClone( void )
{
	assert( m_pCloneSource );
#ifdef _NO_MALLOC_CLONE
	m_pCloneSource->m_CloneList.push_front( this );
#else
	delete this;
#endif

}

bool CGameBoard::MakeMove( int iColumn, EBoardPos ePlayer )
{
	if ( iColumn > m_iWidth )
	{
		return false;
	}

	int iBottomPos = 0;
	while( iBottomPos < m_iHeight && EMPTY == GetBoardPos( iColumn, iBottomPos ) )
	{
		iBottomPos++;
	}
	if ( iBottomPos == 0 )
	{
		return false;
	}

	m_pPositions[iColumn+(iBottomPos-1)*m_iWidth] = ePlayer;
	m_iNumFree--;
	return true;
}


CBoard::EBoardPos CGameBoard::GetBoardPos( int x, int y ) const
{
	assert( x < m_iWidth && y < m_iHeight );
	if ( x < m_iWidth && y< m_iHeight )
	{
		return m_pPositions[m_iWidth*y + x];
	}
	return BOARD_ERROR;
}

void CGameBoard::SetBoardPos( int iX, int iY, EBoardPos ePlayer )
{
	assert( iX < m_iWidth && iY < m_iHeight );
	if ( iX < m_iWidth && iY< m_iHeight )
	{
		m_pPositions[m_iWidth*iY + iX] = ePlayer;
	}
}


CBoard* CGameBoard::CreateClone( void ) const
{
#ifdef _NO_MALLOC_CLONE
	const CGameBoard* pRoot = m_pCloneSource?m_pCloneSource:this;
	CGameBoard* pClone = pRoot->m_CloneList.front();
	pRoot->m_CloneList.pop_front();
	if ( NULL == pClone )
	{
		assert_now( "Not enough clones available" );
		return NULL;
	}
#else
	CGameBoard* pClone = new CGameBoard;
#endif
	//for ( int i=0; i<m_iWidth*m_iHeight; i++ )
	//{
	//	pClone->m_pPositions[i] = m_pPositions[i];
	//}
	memcpy( pClone->m_pPositions, m_pPositions, m_iWidth*m_iHeight*sizeof( m_pPositions[0] ) );
	return pClone;
}

/**
 *	Draw the board.
 *	Should be properly decoupled, oops.
 */
void CGameBoard::Draw( void )
{
	CCircle Circle;
	UINT32 colCurrent;

	bool bHighlighted = false;

	for ( int y=0; y<m_iHeight; y++ )
	{
		for ( int x=0; x<m_iWidth; x++ )
		{
			switch( GetBoardPos( x, y ) )
			{
			default:
			case BOARD_ERROR:
				assert_now( "Error with board" );
			case EMPTY:
				colCurrent = colEMPTY;
				break;
			case PLAYER1:
				colCurrent = colPLAYER_ONE;
				break;
			case PLAYER2:
				colCurrent = colPLAYER_TWO;
				break;
			}
			const FLOAT32 fXPos = fPIECE_RADIUS+fPIECE_SPACING+(fPIECE_SPACING + fPIECE_RADIUS*2)*x;
			const FLOAT32 fYPos = fPIECE_RADIUS+fPIECE_SPACING+(fPIECE_SPACING + fPIECE_RADIUS*2)*y;
			Circle.Initialise( SFVec2( fXPos, fYPos ), fPIECE_RADIUS );
			CConnect4Context::GetInstance().DrawFilledCircle( Circle, colCurrent );
			if ( false == bHighlighted )
			{
				if ( m_iColumnHighlighted == x )
				{
					CConnect4Context::GetInstance().DrawCircle( Circle, colHIGHLIGHT );			
				}
			}
		}
	}
}

void CGameBoard::Init( int iWidth, int iHeight )
{
	m_pCloneSource = NULL;
	m_pPositions = new EBoardPos[ iWidth*iHeight ];
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iColumnHighlighted = -1;
	Reset();
}

void CGameBoard::Reset( void )
{
	for ( int i=0; i<m_iWidth*m_iHeight; i++ )
	{
		m_pPositions[i] = EMPTY;
	}
	m_iNumFree = m_iWidth*m_iHeight;
}

#ifdef _FACTOR_IN_POTENTIAL
/**
 *	Xparam ePlayer The player to evaluate for.
 *	Xreturn number of complete lines of 4 that PLAYER1 is ahead by.
 */
int CGameBoard::EvaluateScorePotential( EBoardPos ePlayer ) const
{
	int iPotentialScore = 0;
	int iScore = 0;
	int iRunLength = 0;
	int iPotentialRunLength = 0;

	// start by counting hosizontals:
	for ( int y=0; y<m_iHeight; y++ )
	{
		for ( int x=0; x<m_iWidth; x++ )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}

		}
		iRunLength = 0;
		iPotentialRunLength = 0;
	}
	// now count verticals:
	for ( int x=0; x<m_iWidth; x++ )
	{
		for ( int y=0; y<m_iHeight; y++ )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}

		}
		iRunLength = 0;
		iPotentialRunLength = 0;
	}
	// finally count diaganols. start running down the left.
	for ( int startY = 0; startY < m_iHeight; startY++ )
	{
		// stepping up right:
		int x = 0;
		int y = startY;
		while ( y >= 0 && x < m_iWidth )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}

			y--;
			x++;
		}
		iRunLength = 0;
		iPotentialRunLength = 0;

		// stepping down left:
		x = 0;
		y = startY;
		while ( x < m_iWidth && y < m_iHeight )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}

			y++;
			x++;
		}
		iRunLength = 0;
		iPotentialRunLength = 0;
	}

	// now run accros the bottom/top
	for ( int startX = 1; startX < m_iWidth; startX++ )
	{
		// stepping up right (from the bottom)
		int x = startX;
		int y = m_iHeight-1;
		while ( y >= 0 && x < m_iWidth )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}

			y--;
			x++;
		}
		iRunLength = 0;
		iPotentialRunLength = 0;

		// stepping down left (from the top)
		x = startX;
		y = 0;
		while ( x < m_iWidth && y < m_iHeight )
		{
			switch ( EvaluatePoint( x, y, ePlayer ) )
			{
			case OURS:
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			case FREE:
				iPotentialRunLength++;
				if ( 4==iPotentialRunLength )
				{
					iPotentialScore++;
				}	
				break;
			default:
				iRunLength = 0;
				iPotentialRunLength = 0;
			}
			y++;
			x++;
		}
		iRunLength = 0;
		iPotentialRunLength = 0;
	}
	

	iPotentialScore -= iScore;
	return iScore * iSCORE_MULTIPLIER + iPotentialScore;
}
#endif // _FACTOR_IN_POTENTIAL

/**
 *	Xparam ePlayer The player to evaluate for.
 *	Xreturn number of complete lines of 4 that PLAYER1 is ahead by.
 */
int CGameBoard::EvaluateScore( EBoardPos ePlayer ) const
{
	int iScore = 0;
	int iRunLength = 0;
	// start by counting hosizontals:
	for ( int y=0; y<m_iHeight; y++ )
	{
		for ( int x=0; x<m_iWidth; x++ )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
		}
		iRunLength = 0;
	}
	// now count verticals:
	for ( int x=0; x<m_iWidth; x++ )
	{
		for ( int y=0; y<m_iHeight; y++ )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
		}
		iRunLength = 0;
	}
	// finally count diaganols. start running down the left.
	for ( int startY = 0; startY < m_iHeight; startY++ )
	{
		// stepping up right:
		int x = 0;
		int y = startY;
		while ( y >= 0 && x < m_iWidth )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
			y--;
			x++;
		}
		iRunLength = 0;

		// stepping down left:
		x = 0;
		y = startY;
		while ( x < m_iWidth && y < m_iHeight )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
			y++;
			x++;
		}
		iRunLength = 0;
	}

	// now run accros the bottom/top
	for ( int startX = 1; startX < m_iWidth; startX++ )
	{
		// stepping up right (from the bottom)
		int x = startX;
		int y = m_iHeight-1;
		while ( y >= 0 && x < m_iWidth )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
			y--;
			x++;
		}
		iRunLength = 0;

		// stepping down left (from the top)
		x = startX;
		y = 0;
		while ( x < m_iWidth && y < m_iHeight )
		{
			if ( GetBoardPosFast( x, y ) == ePlayer )
			{
				iRunLength++;
				if ( 4==iRunLength )
				{
					iScore++;
				}
			} else
			{
				iRunLength = 0;
			}
			y++;
			x++;
		}
		iRunLength = 0;
	}

	return iScore;
}

/**
 *	@param fX	X position to convert to a column.
 *	@param fY	Y position to convert to a column.
 *	@param iColumn	Filled in with a column.
 *	@return true if the position maps to a column.
 */
bool CGameBoard::PositionToColumn( FLOAT32 fX, FLOAT32 fY, int& iColumn ) const
{
//	 fPIECE_RADIUS+fPIECE_SPACING+(fPIECE_SPACING + fPIECE_RADIUS*2)*x;
	fY -= fPIECE_SPACING;
	if ( fY < 0.0f )
	{
		return false;
	}
	if ( fY >= fPIECE_RADIUS+(fPIECE_SPACING + fPIECE_RADIUS*2.0f)*FLOAT32(m_iHeight) )
	{
		return false;
	}

	fX -= fPIECE_SPACING;
	if ( fX < 0.0f )
	{
		return false;
	}
	fX/= (fPIECE_SPACING + fPIECE_RADIUS*2.0f);
	if ( fX >= FLOAT32(m_iWidth) )
	{
		return false;
	}
	iColumn = int(fX);
	return true;
}

/**
 *	@param fX Filled with the mouse X coordinate.
 *	@param fY Filled with the mouse Y coordinate.
 *	@param bWasClicked Filled with a bool to say if the mouse was clicked.
 */
bool CGameBoard::GetMouseInfo( FLOAT32& fX, FLOAT32& fY, bool& bWasClicked ) const
{
	CFVec2 vMousePos;
	CConnect4Context::GetInstance().GetMouse()->GetPosition( vMousePos );
	bWasClicked = CConnect4Context::GetInstance().GetMouse()->WasClicked();
	fX = vMousePos.X();
	fY = vMousePos.Y();
	return true;	
}

/**
 *	@param iColumn	The column to highlight a possible move for, or -1 if none.
 */
bool CGameBoard::HighlightPossibleMove( int iColumn, int iPlayerIdentifier )
{
	// TODO: implement this?
	iPlayerIdentifier;
	m_iColumnHighlighted = iColumn;
	return true;
}

void CGameBoard::ResetEvaluationCount( void )
{
	sm_uEvaluationCount = 0;
}

