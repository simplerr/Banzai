#include <Windows.h>
#include "common\Graphics.h"
#include "common\Menu.h"
#include "MenuState.h"
#include "LocalState.h"
#include "PlayingOnline.h"
#include "LobbyState.h"
#include "Game.h"
#include "Input.h"
#include "resource1.h"

MenuState MenuState::mMenuState;

void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial");

void MenuState::init(Game* game)
{
	// important!
	setGame(game);

	// Not really the center
	Vector center(545, 250);
	int width = 140;
	int height = 80;
	int margin = 15;
	int fontSize = 30;
	DWORD weight = FW_DONTCARE;

	// Create the menu controls
	mhOnlineButton = CreateWindow("Button", "Online", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, center.x - width/2, center.y,
		width, height, gGame->getMainWnd(), (HMENU)IDC_ONLINE_BUTTON, gGame->getAppInst(), NULL);

	mhLocalButton = CreateWindow("Button", "Local", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, center.x - width/2, 
		center.y + height+margin, width, height, gGame->getMainWnd(), (HMENU)IDC_LOCAL_BUTTON, gGame->getAppInst(), NULL);

	mhAboutButton = CreateWindow("Button", "About", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, center.x - width/2,
		center.y + 2*(height+margin), width, height, gGame->getMainWnd(), (HMENU)IDC_ABOUT_BUTTON, gGame->getAppInst(), NULL);

	// Set the fonts
	setFont(mhOnlineButton, fontSize, weight); 
	setFont(mhLocalButton, fontSize, weight);
	setFont(mhAboutButton, fontSize, weight);

	// Load the bkgd and logo textures
	mBkgd = gGraphics->loadTexture("imgs\\bkgd.bmp");
	mLogo = gGraphics->loadTexture("imgs\\logo.bmp");

	// Hack
	mLobbyDelay = 0;
}

void MenuState::cleanup()
{
	destroyWindows();
	ReleaseCOM(mBkgd);
	ReleaseCOM(mLogo);
}

void MenuState::update(double dt)
{
	if(mLobbyDelay == 2)
		changeState(LobbyState::Instance());
}

void MenuState::draw()
{
	gGraphics->drawTexture(mBkgd, 520, 400, 1200, 800);
	gGraphics->drawTexture(mLogo, 1090/2, 120, 673, 176);
	gGraphics->drawText("by simpler", 970, 640, TextType::SMALL_DX);

	if(mLobbyDelay == 1)
		mLobbyDelay++;
}

//! Handles window messages.
LRESULT MenuState::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_ONLINE_BUTTON:
			destroyWindows();
			mLobbyDelay = 1;
			break;
		case IDC_LOCAL_BUTTON:
			changeState(LocalState::Instance());
			break;
		case IDC_ABOUT_BUTTON:
			MessageBox(gGame->getMainWnd(), "You can visit my homepage at http://simplersnet.com \n\nGame coded in C++ with DirectX for graphics.\n\nThanks for playing!\n\nCheers,\nsimpler"
				, "Banzai!", 0);
			break;
		}
	}

	return 0;
}

//! Destroys the window controls associated with the menu.
void MenuState::destroyWindows()
{
	DestroyWindow(mhOnlineButton);
	DestroyWindow(mhLocalButton);
	DestroyWindow(mhAboutButton);
}