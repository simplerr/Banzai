#include "common\Vector.h"
#include "common\Graphics.h"
#include "StatusText.h"

//! Constructor.
StatusText::StatusText()
{
	setText("nothing", RED, 0);
	mPos = Vector(0, 0);
	mColor = RED;
}

//! Constructor.
/**
@param text The text to draw.
@param time How long to draw the text (seconds).
*/
StatusText::StatusText(string text, float x, float y, float time)
{
	setText(text, RED, time);
	mPos = Vector(x, y);
}

//! Destructor.
StatusText::~StatusText()
{

}

//! Draws the status text.
void StatusText::draw()
{
	if(mTime > 0)	{
		if(mColor == RED)
			gGraphics->drawBigDxFont(mText, mPos.x, mPos.y, 0xffff0000);
		else if(mColor == GREEN)
			gGraphics->drawBigDxFont(mText, mPos.x, mPos.y, 0xff00ff00);
	}
}
	
//! Updates the timer.
void StatusText::update(float dt)
{
	mTime -= dt;
}
	
//! Set the text and how long to draw it.
void StatusText::setText(string text, Color color, float time)
{
	mText = text;
	mTime = time;
	mColor = color;
}