/**
 *	@file Connect4\PlayerAgent.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "PlayerAgent.h"
#include "PlayerAgentImp.h"



/**
 *	This function needs to be called to start the processing.
 *	@see IsNextColumnChosen,
 */
bool	CPlayerAgent::StartChooseNextColumn( CBoard* pBoard, int iPlayerIdentifier )
{
	if ( m_pImplementer )
	{
		return m_pImplementer->StartChooseNextColumn( pBoard, iPlayerIdentifier );
	}
	return false;
}
/**
 *	@param iRow	The row to be chosen.
 *	@return true if a choice has been made.
 */
bool	CPlayerAgent::IsNextColumnChosen( int& iRow )
{
	if ( m_pImplementer )
	{
		return m_pImplementer->IsNextColumnChosen( iRow );
	}
	return false;
}

void	CPlayerAgent::SetMaximumDepth( int iDepth )
{
	if ( m_pImplementer )
	{
		m_pImplementer->SetMaximumDepth( iDepth );
	}
}