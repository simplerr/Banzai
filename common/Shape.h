#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <crtdbg.h>
#include <string>
#include "d3dUtil.h"
#include "Vector.h"
#include "Rect.h"

using namespace Mathematics;

//! Contains information about a contact between to shapes.
class Contact
{
public:
	Contact(){Pn = 0; Pt = 0; Pnb = 0;}

	// The arbiter holds the normal
	float Pn;	// normal impulse
	float Pt;	// tangent impulse
	float Pnb;	// normal impulse for position bias
	float massNormal;
	float massTangent;
	float bias;
	float overlap;
	Vector ra;
	Vector rb;
	Vector	pos;
};

//! Contains information about a collision between to shapes.
class Collision
{
public:
	Collision(){};
	Collision(double d, Vector axis)	{
		pushX = d*axis.x;
		pushY = d*axis.y;
		normal = axis;
	}

	void inverse()	{
		pushX *= -1;
		pushY *= -1;
		normal.x *= -1;
		normal.y *= -1;
	}

	bool	collision;
	double	pushX;
	double	pushY;
	float	overlap;
	Vector normal;
	std::vector<Contact> contactList;
};

//! Defines the geomtry of each RigidBody.
class Shape
{
public:
	Shape();

	void addPoint(Vector point);

	/// Moves the origin
	void move(float dx, float dy);
	
	/// Scales by percent, ie scalor = 1.1 scales with 10%
	void scale(float scalor);

	/// Rotation is in radians
	void rotate(float rotation);

	/// Gets called on scaling and rotation, used for broadcast collision detection
	void updateBoundingBox(Vector point);

	void resetBoundingBox();
	void resetRotation();
	
	void setOrigin(Vector position)		{mPosition = position;}
	void setRotationAxis(Vector axis);	// Local coordinate

	int		getHeight(bool rotated = true);
	int		getWidth(bool rotated = true);
	Rect	getWorldRect(void);
	Rect	getLocalRect(void)		{return mBoundingBox;}
	Vector	getOrigin()				{return mPosition;}
	Vector	GetPosition()			{return mPosition;}
	Vector	getRotationAxis()		{return mRotationAxis;}
	int		getSides()				{return mSides;}
	float	getRotation()			{return mRotation;}		// :NOTE: SOLVED???? Doesn't give correct rotation, if you take mRotation - PI/2 (90dgs) then you get the correct

	// Makes it at a lot cleaner than having it private
	std::vector<Vector> pointList;		// Local coordinates

private:
	Vector	mPosition;		// Global coordinate
	Vector	mRotationAxis;	// Local coordinate
	Rect	mBoundingBox;	// Local coordinate
	int		mSides;
	float	mRotation;
};	// Class
#endif