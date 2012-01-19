#ifndef ANIMATION_H
#define ANIMATION_H

#include "d3dUtil.h"
#include "Rect.h"

//! Handles animation of a texture.
class Animation {
public:
	Animation(int frameWidth, int frameHeight, float timePerFrame, int totalFrames, int colums);
	~Animation();

	void animate(float dt);

	void pause();			
	void resume();			
	void setFrame(int currentFrame);
	void setOffset(int offset);

	bool isPaused();
	Rect getSourceRect();
private:
	Rect mSourceRect;

	int mFrameWidth;
	int mFrameHeight;
	int mCurrentFrame;
	int mTotalFrames;
	int mColums;
	int mOffset;
	float mTimePerFrame;
	float mDeltaSum;
	bool mPaused;
};	// Class

#endif