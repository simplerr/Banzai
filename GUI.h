#pragma once

#include <Windows.h>
#include <string>
#include "common\d3dUtil.h"
#include "StatusText.h"
#include "Enums.h"

using namespace std;

class Chat;
class PiecesCaptured;
class OnlinePlayer;

//! Handles the GUI when playing online.
class GUI
{
public:
	GUI();
	~GUI();

	void update(float dt);
	void draw();

	void clear();
	void clearCaptures();
	void clearStats();
	void addCapture(Color color, PieceType type);

	void displayCheckMate(bool won);
	bool displayOpponentLeft();
	void displayRematchRequest();
	void displayRematchDecline();
	void initStats();
	void buildUpperPanel();

	void setPlayer(OnlinePlayer* player);
	void setStatus(string text, Color color = RED, float time = 2.0f);
	Chat* getChat();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	IDirect3DTexture9*	mOpacity;
	IDirect3DTexture9*	mGreyBkgd;
	IDirect3DTexture9*	mOpponentLogo;
	PiecesCaptured*		mPiecesCaptured;
	Chat*				mChat;
	OnlinePlayer*		mPlayer;
	StatusText			mStatusText;

	// Window controls
	HWND				mOpponentInfo;
	HWND				mUpperPanel;
	HWND				mResign;
	HWND				mApplaud;
	HWND				mPlayerName; 
};