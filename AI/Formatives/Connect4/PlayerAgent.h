/**
 *	@file Connect4\PlayerAgent.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PLAYERAGENT_H__Included_521479540
#define _PLAYERAGENT_H__Included_521479540

#include "GDE/Core/Bridge.h"

class CBoard;
class IPlayerAgentImp;

/**
 *	Game side of the bridge, choosing a move.
 */
class CPlayerAgent : public GDE::TBridge<IPlayerAgentImp, CREATE_DLL_ID( 'C','N','C','4' )>
{
public:
	typedef GDE::TBridge<IPlayerAgentImp, CREATE_DLL_ID( 'C','N','C','4' )> TSuperClass; //!< Convenience typedef

	CPlayerAgent( void )
		: TSuperClass()
	{	// empty.
	}
	bool	StartChooseNextColumn( CBoard* pBoard, int iPlayerIdentifier );
	bool	IsNextColumnChosen( int& iRow );
	void	SetMaximumDepth( int iDepth );
protected:
};

#endif //_PLAYERAGENT_H__Included_521479540

