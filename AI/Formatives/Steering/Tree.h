/**
 *	@file AI_Steering\Tree.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TREE_H__Included_1129830863
#define _TREE_H__Included_1129830863

#include "FVector2.h"

class CCircle;

/**
 *	A tree represents a circular item the cat has to route around.
 */
class CTree
{
public:
	// no constructor, initialises instead.
	void Initialise( CFVec2Arg vPosition, FLOAT32 fRadius );
	void DrawTrunk( void );
	void DrawCanopy( void );
	bool Intersects( const CCircle& Circle ) const;

	void ResetIntersection( void );
	void SetIntersectedByCat( void );
protected:
	CFVec2	m_vPos;			//!< The centre position of the tree.
	FLOAT32	m_fRadius;		//!< The radius of the tree trunk to be avoided.
	bool	m_bIntersected;	//!< Does this tree currently intersect the cat? If so then it must draw highlighted somehow.
};

#endif //_TREE_H__Included_1129830863

