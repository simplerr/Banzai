#include <Windows.h>
#include "resource1.h"
#include "common\Graphics.h"
#include "ServerList.h"
#include "LobbyState.h"
#include "Database.h"
#include "PlayingOnline.h"
#include "MenuState.h"
#include "Input.h"

LobbyState LobbyState::mLobbyState;

string getIP();

// Used by the login dialog.
char name[256];
char pass[256];
bool badLogin = false;
bool changedState = false;

//! Procedure for the login dialog.
LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CTLCOLORSTATIC:	{
		if((HWND)lParam == GetDlgItem(hWndDlg, IDC_STATUS) && badLogin)	
			SetTextColor((HDC)wParam, RGB(255, 0, 0));	// Sets the red text.
		
		// Set the bkgd color.
		SetBkColor((HDC)wParam, RGB(240, 240, 240));
		return (INT_PTR)CreateSolidBrush(RGB(240, 240, 240));
	}
	case WM_INITDIALOG:	{
		SetFocus(GetDlgItem(hWndDlg, IDC_EDIT1));
		if(badLogin)	{
			SetWindowText(GetDlgItem(hWndDlg, IDC_STATUS), "Bad login.");
		}
		break;
	}

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_LOGIN:
			if(gDatabase == NULL)
				gDatabase = new Database();
			
			GetDlgItemText(hWndDlg, IDC_EDIT1, name, 255);
			GetDlgItemText(hWndDlg, IDC_EDIT2, pass, 255);

			EndDialog(hWndDlg, 0);
			SendMessage(gGame->getMainWnd(), IDC_TRY_LOGIN, 0, 0);
			break;
		}
		break;
	case WM_CLOSE:
		LobbyState::Instance()->changeState(MenuState::Instance());
		EndDialog(hWndDlg, 0);
		changedState = true;
		break;
	}

	return 0;
}

void LobbyState::init(Game* game)
{
	// Important!
	setGame(game);

	mServerList = NULL;
	
	mBkgd = gGraphics->loadTexture("imgs\\bkgd.bmp");
	mLogo = gGraphics->loadTexture("imgs\\logo.bmp");

	// Promp the login dialog.
	DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG1), gGame->getMainWnd(), (DLGPROC)DlgProc);
		
	if(!changedState)	{
		mServerList = new ServerList(320, 240, 400, 400);
	}
	else
		changedState = false;
}

void LobbyState::cleanup()
{
	badLogin = false;
	ReleaseCOM(mBkgd);
	ReleaseCOM(mLogo);
	delete mServerList;
}

void LobbyState::update(double dt)
{
	// Go back to the menu if ESCAPE was pressed
	if(gInput->keyPressed(VK_ESCAPE))
		changeState(MenuState::Instance());
}

void LobbyState::draw()
{
	// Draw the background and logo
	gGraphics->drawTexture(mBkgd, 600, 400, 1200, 800);	
	gGraphics->drawTexture(mLogo, 1090/2, 120, 673, 176);
	gGraphics->drawText("by simpler", 1080, 720, TextType::SMALL_DX);
}

//! Connect to a server.
/**
@param ip The ip of the server to connect to.
*/
void LobbyState::connectToServer(string ip)
{
	// Change the state and connect to the ip
	changeState(PlayingOnline::Instance());
	PlayingOnline::Instance()->connectToServer(ip);
	PlayingOnline::Instance()->setPlayerName(mUsername);
}

//! Host a server.
void LobbyState::hostServer()
{
	// Get the users IP
	string ip = getIP();

	// Add game to the database
	gDatabase->addServer(mUsername, getIP());

	// Change the state
	changeState(PlayingOnline::Instance());
	PlayingOnline::Instance()->startServer();
	PlayingOnline::Instance()->setPlayerName(mUsername);
}

//! Message procedure.
LRESULT LobbyState::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case IDC_TRY_LOGIN:
		// Invalid player
		if(!gDatabase->validUser(name, pass) && strcmp(pass, "") != 0)	{
			badLogin = true;
			DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG1), gGame->getMainWnd(), (DLGPROC)DlgProc);
		}	
		else if(strcmp(pass, "") == 0)	// Guest player
			mUsername = string(name)+" (guest)";	
		else	// Registered player
			mUsername = string(name);

		break;
	}

	// Let the server list process messages
	if(mServerList != NULL)
		mServerList->msgProc(msg, wParam, lParam);

	return 0;
}