#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

enum CameraDimension
{
	DIMENSION_2D,
	DIMENSION_3D
};

//! Handels camera initialization and movement.
class Camera
{
public:
	Camera(CameraDimension dim, float x, float y, int width, int height);
	~Camera();

	void update();

	void buildViewMatrix();
	void buildProjectionMatrix();
	void buildViewport();

	void move(float dx, float dy, float dz);
	void moveTarget(float dx, float dy, float dz);

	D3DVIEWPORT9	getViewport()	{return mViewport;}
	D3DXMATRIX		getView()		{return mView;}
	D3DXMATRIX		getProj()		{return mProj;}
	D3DXVECTOR3		getPos()		{return mPos;}		

	float getX()	{return mPos.x;}
	float getY()	{return mPos.y;}
	float getZ()	{return mPos.z;}
	float getWidth(void)	{return mViewport.Width;}
	float getHeight(void)	{return mViewport.Height;}

	void setPosition(D3DXVECTOR3 position)	{mPos = position;}
	void setPosition(float x, float y, float z)	{mPos = D3DXVECTOR3(x, y,z);}
	void setX(float x)	{mPos.x = x;}
	void setY(float y)	{mPos.y = y;}
	void setZ(float z)	{mPos.z = z;}

	void setTarget(D3DXVECTOR3 pos) {mTarget = pos;}
	void setTarget(float x, float y, float z) {mTarget = D3DXVECTOR3(x, y, z);}

private:
	D3DVIEWPORT9 mViewport;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	D3DXVECTOR3 mPos;
	D3DXVECTOR3 mTarget;

	CameraDimension mDimension;
};	// Class
#endif