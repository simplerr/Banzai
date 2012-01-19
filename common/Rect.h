#ifndef RECT_H
#define RECT_H

#include "Vector.h"

//! Rectangle.
class Rect
{
public:
	Rect(){};
	Rect(float _left, float _right, float _top, float _bottom)	{
		left = _left;
		right = _right;
		top = _top;
		bottom = _bottom;
	}

	Rect(float x, float y, int width, int height)	{
		left = x - width/2;
		right = x + width/2;
		top = y - height/2;
		bottom = y + height/2;
	}

	void move(float dx, float dy)	{
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}

	Vector getPos()	{
		return Vector(left + (right-left)/2, top + (bottom-top)/2);
	}

	float getWidth()	{
		return right - left;
	}
	
	float getHeight()	{
		return bottom - top;
	}

	bool pointInside(Vector point)	{
		if(point.x > left && point.x < right && point.y > top && point.y < bottom)
			return true;
		else
			return false;
	}

	float left;
	float right;
	float top;
	float bottom;
};

#endif