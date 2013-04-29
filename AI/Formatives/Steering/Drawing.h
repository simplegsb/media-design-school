//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#ifndef DRAWING_H_
#define DRAWING_H_

#include "SteeringAgent.h"

/**
 * Functions that draw objects.
 */
namespace Drawing
{
	/**
	 * Draws the given cat.
	 *
	 * @param _krCat The cat to draw.
	 */
	void DrawCat(const CCat& _krCat);

	/**
	 * Draws the given steering agent (visual debug information).
	 *
	 * @param _krSteeringAgent The steering agent to draw.
	 */
	void DrawSteeringAgent(const CSteeringAgent& _krSteeringAgent);
}

#endif /* DRAWING_H_ */
