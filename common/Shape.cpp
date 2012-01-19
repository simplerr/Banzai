#include "Shape.h"

Shape::Shape()
{
	mSides = 0;
	mBoundingBox.left = 0;
	mBoundingBox.right = 0;
	mBoundingBox.bottom = 0;
	mBoundingBox.top = 0;
	mRotation = 0;

	// Rotate around origin as standard
	mRotationAxis = Vector(0, 0, 0);
}

// The order in which the points gets added is important
void Shape::addPoint(Vector point)
{
	// Calculate the bounding box
	if(point.x < getLocalRect().left)
		mBoundingBox.left = point.x;
	else if(point.x > getLocalRect().right)
		mBoundingBox.right = point.x;

	if(point.y < getLocalRect().top)
		mBoundingBox.top = point.y;
	else if(point.y > getLocalRect().bottom)
		mBoundingBox.bottom = point.y;

	mSides++;

	pointList.push_back(point);
}

int Shape::getWidth(bool rotated)
{
	if(rotated)
		return mBoundingBox.right - mBoundingBox.left;
	else	{
		float rotation = getRotation();
		rotate(-rotation);
		int width =  mBoundingBox.right - mBoundingBox.left;
		rotate(rotation);
		return width;
	}
}

int	Shape::getHeight(bool rotated)
{
	if(rotated)
		return mBoundingBox.bottom - mBoundingBox.top;
	else	{
		float rotation = getRotation();
		rotate(-rotation);
		int height = mBoundingBox.bottom - mBoundingBox.top;
		rotate(rotation);
		return height;
	}
}


Rect Shape::getWorldRect(void)	
{
	Rect r = mBoundingBox;

	r.left += getOrigin().x;
	r.right += getOrigin().x;
	r.top += getOrigin().y;
	r.bottom += getOrigin().y;

	return r;
}

void Shape::setRotationAxis(Vector axis)	
{
	// Inverse the axis :NOTE: Weird
	axis *= -1;

	// Calculate the rotation from the rotation axis
	float rotation = atan2f(axis.y, axis.x);

	// Rotate around the origin (0,0) - this only moves the points and NOT the origin
	rotate(rotation);

	// Set the rotation axis
	mRotationAxis = axis;
}

void Shape::move(float dx, float dy)
{
	mPosition.x += dx;
	mPosition.y += dy;

}

void Shape::rotate(float rotation)
{
	// Setup variables and create the rotation matrix
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR4 vector;
	vector.z = 0;
	vector.w = 1;
	D3DXMatrixRotationZ(&rotationMatrix, rotation);

	/* Calculate the new position on the origin based on the rotation 
		- Rotating around an axis other than the origin makes  the origins position change
		- Therefor this step is done
		- mRotationAxis is the distance from the origin to the rotation axis
	*/

	// Get the rotation axis (local coordinates)
	vector.x = mRotationAxis.x;
	vector.y = mRotationAxis.y;

	// Do the rotation
	D3DXVec4Transform(&vector,  &vector, &rotationMatrix);

	// Set the new position 
	setOrigin(Vector(vector.x, vector.y) + (getOrigin() - mRotationAxis));

	// Set the new rotation axis offset
	mRotationAxis = Vector(vector.x, vector.y);

	// For proper updating of the bounding box
	resetBoundingBox();

	// Calculate each points new local position when they rotate around the axis (0,0)
	for(int i = 0; i < pointList.size(); i++)	
	{
		vector.x = pointList[i].x;	
		vector.y = pointList[i].y;

		D3DXVec4Transform(&vector,  &vector, &rotationMatrix);

		pointList[i].x = vector.x;
		pointList[i].y = vector.y;

		// Update the bounding box
		updateBoundingBox(pointList[i]);
	}

	// Add to the rotation
	mRotation += rotation;

	// To avoid big numbers
	if(mRotation > 2*3.14)
		mRotation = rotation;
	else if(mRotation < -2*3.14)
		mRotation = rotation;
}

void Shape::resetRotation()
{
	rotate(-mRotation);
}

void Shape::updateBoundingBox(Vector point)
{
	if(point.x < getLocalRect().left)
		mBoundingBox.left = point.x;
	else if(point.x > getLocalRect().right)
		mBoundingBox.right = point.x;

	if(point.y < getLocalRect().top)
		mBoundingBox.top = point.y;
	else if(point.y > getLocalRect().bottom)
		mBoundingBox.bottom = point.y;
}

void Shape::resetBoundingBox()
{
	mBoundingBox.bottom = 0;
	mBoundingBox.top = 0;
	mBoundingBox.left = 0;
	mBoundingBox.right = 0;
}

void Shape::scale(float scalor)
{
	resetBoundingBox();
	for(int i = 0; i < pointList.size(); i++)	{
		pointList[i].x *= scalor;
		pointList[i].y *= scalor;
		updateBoundingBox(pointList[i]);
	}
}