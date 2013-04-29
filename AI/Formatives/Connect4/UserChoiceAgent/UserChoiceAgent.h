/**
 *	@file Connect4\UserChoiceAgent\UserChoiceAgent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _USERCHOICEAGENT_H__Included_1750156128
#define _USERCHOICEAGENT_H__Included_1750156128

#include "../PlayerAgentImp.h"
class CBoard;

class CUserChoiceAgent : public IPlayerAgentImp
{
public:
	CUserChoiceAgent( void )
		: m_pTestBoard( NULL )
	{
	}
	~CUserChoiceAgent( void );

	/**
	 *	This function needs to be called to start the processing.
	 *	@see IsNextColumnChosen,
	 */
	virtual bool	StartChooseNextColumn( CBoard* pBoard, int iPlayerIdentifier );
	/**
	 *	@param iRow	The row to be chosen.
	 *	@return true if a choice has been made.
	 */
	virtual bool	IsNextColumnChosen( int& iRow );
protected:
	void operator=(CUserChoiceAgent& Other); // incomplete assignment operator, stops compiler warning.

	CBoard*			m_pBoard;
	CBoard*			m_pTestBoard;
	int				m_iPlayerId;
};

#endif //_USERCHOICE_H__Included_1750156128

