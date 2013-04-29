/**
 *	@file Connect4\UserChoiceAgent\UserChoiceAgent.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "UserChoiceAgent.h"
#include "Board.h"

CUserChoiceAgent::~CUserChoiceAgent( void )
{
	if ( m_pTestBoard )
	{
		m_pTestBoard->ReleaseClone();
		m_pTestBoard = NULL;
	}
}


/**
 *	This function needs to be called to start the processing.
 *	@see IsNextColumnChosen,
 */
bool	CUserChoiceAgent::StartChooseNextColumn( CBoard* pBoard, int iPlayerIdentifier )
{
	m_pBoard = pBoard;
	m_pTestBoard = pBoard->CreateClone();
	m_iPlayerId = iPlayerIdentifier;
	return true;
}

/**
 *	@param iCol	The row to be chosen.
 *	@return true if a choice has been made.
 */
bool	CUserChoiceAgent::IsNextColumnChosen( int& iCol )
{
	assert( NULL != m_pTestBoard );
	FLOAT32 fX, fY;
	bool bMouseClicked;
	if ( false == m_pBoard->GetMouseInfo( fX, fY, bMouseClicked ) )
	{
		assert_now( "Unable to get mouse info" );
		return false;	// scary.
	}
	
	int iColumn = -1;
	if ( m_pBoard->PositionToColumn( fX, fY, iColumn ) )
	{
		if ( bMouseClicked )
		{
			if ( m_pTestBoard && m_pTestBoard->MakeMove( iColumn, CBoard::EBoardPos(m_iPlayerId) ) )
			{
				iCol = iColumn;
				m_pBoard->HighlightPossibleMove( -1, m_iPlayerId );
				m_pTestBoard->ReleaseClone();
				m_pTestBoard = NULL;
				return true;
			}
		}
	}
	m_pBoard->HighlightPossibleMove( iColumn, m_iPlayerId );
	return false;
}

