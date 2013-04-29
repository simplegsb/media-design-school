/**
 *	@file Connect4\Board.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BOARD_H__Included_963429982
#define _BOARD_H__Included_963429982

/**
 *	Represents the game board.  Can be passed accross the dll boundaries.
 */
class CBoard
{
public:
	enum EBoardPos
	{
		EMPTY = 0,
		PLAYER1 = 1,
		PLAYER2 = 2,
		BOARD_ERROR = 3
	};

	virtual int GetWidth( void ) const = 0;
	virtual int GetHeight( void ) const = 0;
	virtual EBoardPos GetBoardPos( int x, int y ) const = 0;

	virtual void	ReleaseClone( void ) = 0;

	/**
	 *	Clone the game board.
	 */
	virtual CBoard* CreateClone( void ) const = 0;

	/**
	 *	@return number of complete lines of 4 that PLAYER1 is ahead by.
	 */
	virtual int EvaluateScore( void ) const = 0;

	/**
	 *	Drop a players icon into the board at the given row.
	 *	@return true if there is space to drop it.
	 */
	virtual bool MakeMove( int iColumn, EBoardPos ePlayer ) = 0;

	/**
	 *	Return true if the player has clicked the mouse anywhere.
	 */
	virtual bool PositionToColumn( FLOAT32 fX, FLOAT32 fY, int& iColumn ) const = 0;

	/**
	 *	@param fX Filled with the mouse X coordinate.
	 *	@param fY Filled with the mouse Y coordinate.
	 *	@param bWasClicked Filled with a bool to say if the mouse was clicked.
	 */
	virtual bool GetMouseInfo( FLOAT32& fX, FLOAT32& fY, bool& bWasClicked ) const = 0;

	/**
	 *	@param iColumn	The column to highlight a possible move for, or -1 if none.
	 */
	virtual bool HighlightPossibleMove( int iColumn, int iPlayerIdentifier ) = 0;
protected:
	virtual ~CBoard( void ) {}
};

#endif //_BOARD_H__Included_963429982

