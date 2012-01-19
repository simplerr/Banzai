#include "Camera.h"

Camera::Camera(CameraDimension dim, float viewport_x, float viewport_y, int viewport_width, int viewport_height)
{
	// Setup the game viewport
	mViewport.X = viewport_x;
	mViewport.Y = viewport_y;
	mViewport.Width = viewport_width;
	mViewport.Height = viewport_height;
	mViewport.MinZ = 0.0f;
	mViewport.MaxZ = 1.0f;

	mDimension = dim;
}

Camera::~Camera()
{
	//ReleaseCOM(mViewport);
}

void Camera::update()
{
	buildViewMatrix();
	buildProjectionMatrix();
}

void Camera::buildViewMatrix(void)
{
	D3DXVECTOR3 pos(mPos.x, mPos.y, mPos.z);
	D3DXVECTOR3 target(mTarget.x, mTarget.y, mTarget.z);
	D3DXVECTOR3 up(0.0f, -1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
	gd3dDevice->SetTransform(D3DTS_VIEW, &mView);
}

void Camera::buildProjectionMatrix(void)
{
	if(mDimension == DIMENSION_2D)	{
		D3DXMatrixOrthoLH(&mProj, getWidth(), getHeight(), 0, 1);
	}
	else if(mDimension == DIMENSION_3D)	{
		D3DXMatrixPerspectiveFovLH(&mProj, 3.14159f/4, 640 / 480, 1.0f, 2000.0f);
	}

	gd3dDevice->SetTransform(D3DTS_PROJECTION, &mProj);

}

void Camera::buildViewport(void)
{
	gd3dDevice->SetViewport(&mViewport);
}

void Camera::move(float dx, float dy, float dz)
{
	// Move the camera position
	mPos.x += dx;
	mPos.y += dy;
	mPos.z += dz;
	buildViewMatrix();
}

void Camera::moveTarget(float dx, float dy, float dz)
{
	// Move the target position
	mTarget.x += dx;
	mTarget.y += dy;
	mTarget.z += dz;
	buildViewMatrix();
}