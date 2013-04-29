/**
 *	@file Connect4\PlayerAgentImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PLAYERAGENTIMP_H__Included_251885909
#define _PLAYERAGENTIMP_H__Included_251885909

class CBoard;

class IPlayerAgentImp
{
public:
	/**
	 *	For AI solutions, set the maximum depth for them to search.
	 */
	virtual void	SetMaximumDepth( int iDepth ) { iDepth; }
	/**
	 *	This function needs to be called to start the processing.
	 *	@see IsNextColumnChosen,
	 */
	virtual bool	StartChooseNextColumn( CBoard* pBoard, int iPlayerIdentifier ) = 0;
	/**
	 *	@param iRow	The row to be chosen.
	 *	@return true if a choice has been made.
	 */
	virtual bool	IsNextColumnChosen( int& iRow ) = 0;
protected:
};

#endif //_PLAYERAGENTIMP_H__Included_251885909

