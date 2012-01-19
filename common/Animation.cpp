#include "Animation.h"
#include "Graphics.h"

Animation::Animation(int frameWidth, int frameHeight, float timePerFrame, int totalFrames, int colums)
{
	mFrameWidth = frameWidth;
	mFrameHeight = frameHeight;
	mTimePerFrame = timePerFrame;
	mTotalFrames = totalFrames;
	mColums = colums;
	mDeltaSum = 0;
	mCurrentFrame = 0;
	mPaused = false;
	mOffset = 0;
}
	
Animation::~Animation()
{
	
}

void Animation::animate(float dt)
{
	if(!mPaused)
	{
		// Perform animation, calculate frame
		if(mDeltaSum >= mTimePerFrame)	{
			setFrame(mCurrentFrame + 1);
			if(mCurrentFrame >= mTotalFrames)
				setFrame(0);
			
			mDeltaSum = 0;
		}
		else
			mDeltaSum += dt;

		// Calculate the source Rect
		// Calculate frame location
		int fx = ((mCurrentFrame + mOffset) % mColums) * mFrameWidth;
		int fy = ((mCurrentFrame + mOffset) / mColums) * mFrameHeight;

		mSourceRect = Rect((float)fx, (float)fx + mFrameWidth, (float)fy, (float)fy + mFrameHeight);
	}
}

//! Set the current frame.
void Animation::setFrame(int currentFrame)
{
	mCurrentFrame = currentFrame;

	// Calculate source rect
	int fx = (mCurrentFrame % mColums) * mFrameWidth;
	int fy = (mCurrentFrame / mColums) * mFrameHeight;

	mSourceRect = Rect((float)fx, (float)fx + mFrameWidth, (float)fy, (float)fy + mFrameHeight);
}

//! Pause the animation.
void Animation::pause()			
{
	mPaused = true;
}

//! Resume the animation.
void Animation::resume()			
{
	mPaused = false;
}

//! Get the rect from the image file
/**
@return The rect from the image file to draw.
@note Range 0-1.
*/
Rect Animation::getSourceRect()	
{
	return mSourceRect;
}


//! Set on which frame to start at.
void Animation::setOffset(int offset)
{
	mOffset = offset;
}

//! Returns true if the animation is paused.
bool Animation::isPaused()		
{
	return mPaused;
}