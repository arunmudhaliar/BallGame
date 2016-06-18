#pragma once

#include "../core/vector2.h"

class gxUtil
{
public:
	static vector2f closestPointOnLine(vector2f& p, vector2f& a, vector2f& b)
	{
		// a-b is the line, p the point in question
		vector2f c = p - a;
		vector2f V = b - a;
		float d = V.length();

		V.normalize();
		float t = V.dot(c);

		// Check to see if 奏?is beyond the extents of the line segment
		if (t < 0.0f) return (a);
		if (t > d) return (b);

		// Return the point between 疎?and 礎?		//set length of V to t. V is normalized so this is easy
		V.x = V.x * t;
		V.y = V.y * t;

		return (a + V);
	}
};