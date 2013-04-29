/**
 *	@file GDE\Maths\MathConstants.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MATHCONSTANTS_H__Included_1620409170
#define _MATHCONSTANTS_H__Included_1620409170

namespace GDE
{
	/**
	 *	@author Jon Booth.
	 *	@ingroup Maths
	 *
	 *	A very small value (currently quite big!)
	 */
	const static FLOAT32 fEPSILON	= 0.0000000001f; // very small....
	/**
	 *	@author Jon Booth.
	 *	@ingroup Maths
	 *
	 *	A very big value (currently quite small!)
	 */
	const static FLOAT32 fMASSIVE 	= 9999999999999999999999999999999999.f; // very big
	/**
	 *	@author Jon Booth.
	 *	@ingroup Maths
	 *
	 *	The pi constant.
	 */
	const static FLOAT32 fPI		= 3.141592653589793f;

	/**
	 *	@author Jon Booth.
	 *	@ingroup Maths
	 *
	 *	@param fDegs Number of degrees.
	 *	@return fDegs as radians.
	 */
	inline FLOAT32 fDegsToRad( FLOAT32 fDegs )
	{
		return (fDegs/180.0f)*fPI;
	}

	/**
	 *	@author Jon Booth.
	 *	@ingroup Maths
	 *
	 *	@param fRads Number of rafians.
	 *	@return fRads as degrees.
	 */
	inline FLOAT32 fRadsToDegs( FLOAT32 fRads )
	{
		return (fRads/fPI)*180.0f;
	}
};

#endif //_MATHCONSTANTS_H__Included_1620409170

