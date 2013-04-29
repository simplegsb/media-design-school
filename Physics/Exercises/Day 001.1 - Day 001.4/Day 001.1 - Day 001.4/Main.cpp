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
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//

#include <assert.h>
#include <iostream>

#include <gazengine/math/Vector3.h>

void day0011();
void day0012();
void day0013();
void day0014();
bool intersect(const Vector3& lineSegmentStart, const Vector3& lineSegmentFinish, const Vector3& planeNormal,
			   const Vector3& planePosition);
int intersect(const Vector3& planeNormal, const Vector3& planePosition, const Vector3& point);
bool intersect(const Vector3& triangleA, const Vector3& triangleB, const Vector3& triangleC,
			   const Vector3& planeNormal, const Vector3& planePosition);
int main();

const int BEHIND = -1;
const int INFRONT = 1;
const int ON_PLANE = 0;

void day0011()
{
	std::cout << std::endl;
	std::cout << "Day 001.1 :: Legrange's formula" << std::endl;
	std::cout << std::endl;

	Vector3 a0(0.0f, 0.0f, 0.0f);
	Vector3 b0(0.0f, 0.0f, 0.0f);
	Vector3 c0(0.0f, 0.0f, 0.0f);

	// a x (b x c)
	Vector3 lhs0 = a0.crossProduct(b0.crossProduct(c0));

	// b(a . c) - c(a . b)
	Vector3 rhs0 = b0 * a0.dotProduct(c0) - c0 * a0.dotProduct(b0);

	std::cout << "a = " << a0 << std::endl;
	std::cout << "b = " << b0 << std::endl;
	std::cout << "c = " << c0 << std::endl;

	std::cout << "a x (b x c) :: " << lhs0 << std::endl;
	std::cout << "b(a . c) - c(a . b) :: " << rhs0 << std::endl;
	std::cout << "a x (b x c) == b(a . c) - c(a . b) :: ";
	if (lhs0 == rhs0)
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	Vector3 a1(1.0f, 0.0f, 0.0f);
	Vector3 b1(0.0f, 1.0f, 0.0f);
	Vector3 c1(0.0f, 0.0f, 1.0f);

	// a x (b x c)
	Vector3 lhs1 = a1.crossProduct(b1.crossProduct(c1));

	// b(a . c) - c(a . b)
	Vector3 rhs1 = b1 * a1.dotProduct(c1) - c1 * a1.dotProduct(b1);

	std::cout << "a = " << a1 << std::endl;
	std::cout << "b = " << b1 << std::endl;
	std::cout << "c = " << c1 << std::endl;

	std::cout << "a x (b x c) :: " << lhs1 << std::endl;
	std::cout << "b(a . c) - c(a . b) :: " << rhs1 << std::endl;
	std::cout << "a x (b x c) == b(a . c) - c(a . b) :: ";
	if (lhs1 == rhs1)
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	Vector3 a2(1.2f, 7.4f, 8.5f);
	Vector3 b2(7.4f, 2.3f, 5.6f);
	Vector3 c2(3.8f, 0.4f, 9.6f);

	// a x (b x c)
	Vector3 lhs2 = a2.crossProduct(b2.crossProduct(c2));

	// b(a . c) - c(a . b)
	Vector3 rhs2 = b2 * a2.dotProduct(c2) - c2 * a2.dotProduct(b2);

	std::cout << "a = " << a2 << std::endl;
	std::cout << "b = " << b2 << std::endl;
	std::cout << "c = " << c2 << std::endl;

	std::cout << "a x (b x c) :: " << lhs2 << std::endl;
	std::cout << "b(a . c) - c(a . b) :: " << rhs2 << std::endl;
	std::cout << "a x (b x c) == b(a . c) - c(a . b) :: ";
	if (lhs2 == rhs2)
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}
}

void day0012()
{
	std::cout << std::endl;
	std::cout << "Day 001.2 :: Plane vs. Point Function" << std::endl;
	std::cout << std::endl;

	Vector3 planeNormal(1.0f, 0.0f, 0.0f);
	Vector3 planePoint(0.0f, 0.0f, 0.0f);
	Vector3 point0(-1.0f, 0.0f, 0.0f);
	Vector3 point1(1.0f, 0.0f, 0.0f);
	Vector3 point2(0.0f, 0.0f, 0.0f);

	int result0 = intersect(planeNormal, planePoint, point0);

	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;
	std::cout << "point :: " << point0 << std::endl;
	std::cout << "position :: ";
	if (result0 == BEHIND)
	{
		std::cout << "BEHIND" << std::endl;
	}
	else if (result0 == INFRONT)
	{
		std::cout << "INFRONT" << std::endl;
	}
	else if (result0 == ON_PLANE)
	{
		std::cout << "ON_PLANE" << std::endl;
	}

	std::cout << std::endl;

	int result1 = intersect(planeNormal, planePoint, point1);

	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;
	std::cout << "point :: " << point1 << std::endl;
	std::cout << "position :: ";
	if (result1 == BEHIND)
	{
		std::cout << "BEHIND" << std::endl;
	}
	else if (result1 == INFRONT)
	{
		std::cout << "INFRONT" << std::endl;
	}
	else if (result1 == ON_PLANE)
	{
		std::cout << "ON_PLANE" << std::endl;
	}

	std::cout << std::endl;

	int result2 = intersect(planeNormal, planePoint, point2);

	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;
	std::cout << "point :: " << point2 << std::endl;
	std::cout << "position :: ";
	if (result2 == BEHIND)
	{
		std::cout << "BEHIND" << std::endl;
	}
	else if (result2 == INFRONT)
	{
		std::cout << "INFRONT" << std::endl;
	}
	else if (result2 == ON_PLANE)
	{
		std::cout << "ON_PLANE" << std::endl;
	}
}

void day0013()
{
	std::cout << std::endl;
	std::cout << "Day 001.3 :: Line Segment vs. Plane Function" << std::endl;
	std::cout << std::endl;

	Vector3 planeNormal(1.0f, 0.0f, 0.0f);
	Vector3 planePoint(0.0f, 0.0f, 0.0f);
	Vector3 point0(-2.0f, 0.0f, 0.0f);
	Vector3 point1(-1.0f, 0.0f, 0.0f);
	Vector3 point2(0.0f, 0.0f, 0.0f);
	Vector3 point3(1.0f, 0.0f, 0.0f);
	Vector3 point4(2.0f, 0.0f, 0.0f);

	std::cout << "lineSegmentStart :: " << point0 << std::endl;
	std::cout << "lineSegmentFinish :: " << point1 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point0, point1, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "lineSegmentStart :: " << point1 << std::endl;
	std::cout << "lineSegmentFinish :: " << point2 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point1, point2, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "lineSegmentStart :: " << point2 << std::endl;
	std::cout << "lineSegmentFinish :: " << point2 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point2, point2, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "lineSegmentStart :: " << point2 << std::endl;
	std::cout << "lineSegmentFinish :: " << point3 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point2, point3, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "lineSegmentStart :: " << point1 << std::endl;
	std::cout << "lineSegmentFinish :: " << point3 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point1, point3, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "lineSegmentStart :: " << point3 << std::endl;
	std::cout << "lineSegmentFinish :: " << point4 << std::endl;
	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;

	std::cout << "intersect :: ";
	if (intersect(point3, point4, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}
}

void day0014()
{
	std::cout << std::endl;
	std::cout << "Day 001.3 :: Triangle vs. Plane Function" << std::endl;
	std::cout << std::endl;

	Vector3 planeNormal(1.0f, 0.0f, 0.0f);
	Vector3 planePoint(0.0f, 0.0f, 0.0f);
	Vector3 triangle00(-2.0f, 0.0f, 0.0f);
	Vector3 triangle01(2.0f, 0.0f, 0.0f);
	Vector3 triangle02(0.0f, 2.0f, 0.0f);
	Vector3 triangle10(-6.0f, 0.0f, 0.0f);
	Vector3 triangle11(-2.0f, 0.0f, 0.0f);
	Vector3 triangle12(-4.0f, 2.0f, 0.0f);

	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;
	std::cout << "triangle0 :: " << triangle00 << std::endl;
	std::cout << "triangle1 :: " << triangle01 << std::endl;
	std::cout << "triangle2 :: " << triangle02 << std::endl;

	std::cout << "intersect :: ";
	if (intersect(triangle00, triangle01, triangle02, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "planeNormal :: " << planeNormal << std::endl;
	std::cout << "planePoint :: " << planePoint << std::endl;
	std::cout << "triangle0 :: " << triangle10 << std::endl;
	std::cout << "triangle1 :: " << triangle11 << std::endl;
	std::cout << "triangle2 :: " << triangle12 << std::endl;

	std::cout << "intersect :: ";
	if (intersect(triangle10, triangle11, triangle12, planeNormal, planePoint))
	{
		std::cout << "true" << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}
}

bool intersect(const Vector3& lineSegmentStart, const Vector3& lineSegmentFinish, const Vector3& planeNormal,
			   const Vector3& planePosition)
{
	int startPosition = intersect(planeNormal, planePosition, lineSegmentStart);
	int finishPosition = intersect(planeNormal, planePosition, lineSegmentFinish);

	if (startPosition == BEHIND && finishPosition == INFRONT ||
		startPosition == INFRONT && finishPosition == BEHIND ||
		startPosition == ON_PLANE ||
		finishPosition == ON_PLANE)
	{
		return true;
	}

	return false;
}

int intersect(const Vector3& planeNormal, const Vector3& planePosition, const Vector3& point)
{
	float referenceDistance = planeNormal.dotProduct(planePosition);
	float result = planeNormal.dotProduct(point) - referenceDistance;

	if (result < 0.0f)
	{
		return BEHIND;
	}
	else if (result > 0.0f)
	{
		return INFRONT;
	}
	else
	{
		return ON_PLANE;
	}
}

bool intersect(const Vector3& triangleA, const Vector3& triangleB, const Vector3& triangleC,
			   const Vector3& planeNormal, const Vector3& planePosition)
{
	if (intersect(triangleA, triangleB, planeNormal, planePosition))
	{
		return true;
	}

	if (intersect(triangleB, triangleC, planeNormal, planePosition))
	{
		return true;
	}

	if (intersect(triangleC, triangleA, planeNormal, planePosition))
	{
		return true;
	}

	return false;
}

int main()
{
	day0011();
	day0012();
	day0013();
	day0014();

	char pause;
	std::cin >> pause;
}
