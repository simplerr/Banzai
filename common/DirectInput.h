//=============================================================================
// DiRectInput.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Wraps initialization of immediate mode DiRect Input, and provides 
// information for querying the state of the keyboard and mouse.
//=============================================================================

#ifndef DIRect_INPUT_H
#define DIRect_INPUT_H

#define DIRectINPUT_VERSION 0x0800
#include <dinput.h>
#include "d3dUtil.h"

class Vector;

//! Wraps up input with DirectX.
class DirectInput
{
public:
	DirectInput(HWND hwnd, DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();
	bool keyDown(char key);
	bool keyPressed(char key);
	bool keyReleased(char key);
	bool mouseDown(int button);
	bool mousePressed(int button);
	bool mouseReleased(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

	char ScanToChar(DWORD scanCode) const;
	char getInput(void);

	void	updateCursor(void);
	void	drawCursor(void);

	Vector	getCursorPos(void);
	void	setCursorPos(Vector pos);

	void	restoreCursor(void);
	void	drawCursorPos(void);
private:
	// Make private to prevent copying of members of this class.
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);
		
private:
	IDirectInput8*       mDInput;

	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256]; 
	char				 mLastKeyboardState[256];

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
	DIMOUSESTATE2        mLastMouseState;

	IDirect3DTexture9*	mCursorTexture;

	float				mCursorX;
	float				mCursorY;
	HWND				mHwnd;
};	// Class

// Global
//extern DirectInput* gDInput;
#endif