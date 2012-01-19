#ifndef STATUSTEXT_H
#define STATUSTEXT_H

#include <string>
#include "common\\Vector.h"
#include "Enums.h"
using namespace std;

//! Draws messages explaining what the player did wrong.
class StatusText
{
public:
	StatusText();
	StatusText(string text, float x, float y, float time = 2.0f);
	~StatusText();

	void draw();
	void update(float dt);
	void setText(string text, Color color = RED, float time = 2.0f);
private:
	Vector	mPos;
	string	mText;
	Color	mColor;
	float	mTime;
};

#endif