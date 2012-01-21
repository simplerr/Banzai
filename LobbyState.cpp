#include <Windows.h>
#include "resource1.h"
#include "common\Graphics.h"
#include "ServerList.h"
#include "LobbyState.h"
#include "Database.h"
#include "PlayingOnline.h"
#include "MenuState.h"
#include "Input.h"
#include "Sound.h"

LobbyState LobbyState::mLobbyState;

string getPublicIp();
string getLocalIp();
string getText(HWND hwnd);

// Used by the login dialog.
char name[256];
char pass[256];
bool badLogin = false;
bool changedState = false;
string musername;

//! Procedure for the register dialog.
LRESULT CALLBACK RegisterDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:	{
		SetFocus(GetDlgItem(hWndDlg, IDC_USERNAME));
		break;
	}

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_REGISTER_BTN:
			if(gDatabase == NULL)
				gDatabase = new Database();

			// Get entered data
			string username = getText(GetDlgItem(hWndDlg, IDC_USERNAME));
			string password = getText(GetDlgItem(hWndDlg, IDC_PASSWORD));
			string mail = getText(GetDlgItem(hWndDlg, IDC_MAIL));

			// No empty values
			if(username != "" && password != "" && mail != "")	{
				// Add to database
				gDatabase->addUser(username, password, mail);

				GetWindowText(GetDlgItem(hWndDlg, IDC_USERNAME), name, 255);
				GetWindowText(GetDlgItem(hWndDlg, IDC_PASSWORD), pass, 255);
				EndDialog(hWndDlg, 0);
				MessageBox(gGame->getMainWnd(), "Successfully registered!\nYou will automaticly get logged in on your new account.", "Success!", 0);

				gSound->playEffect(LOGIN_SOUND);

				// Send login message, name and pass was set above
				SendMessage(gGame->getMainWnd(), IDC_TRY_LOGIN, 0, 0);
			}
			else	{
				// Close the dialog and show it again
				EndDialog(hWndDlg, 0);
				gSound->playEffect(BAD_LOGIN_SOUND);
				DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG2), gGame->getMainWnd(), (DLGPROC)RegisterDlgProc);
			}
				
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
		case IDC_REGISTER:
			// Prompt the register dialog.
			EndDialog(hWndDlg, 0);
			DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG2), gGame->getMainWnd(), (DLGPROC)RegisterDlgProc);
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

	// Prompt the login dialog.
	DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG1), gGame->getMainWnd(), (DLGPROC)DlgProc);
		
	if(!changedState)	{
		mServerList = new ServerList(345, 240, 400, 400);
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
	gGraphics->drawTexture(mBkgd, 520, 400, 1200, 800);
	gGraphics->drawTexture(mLogo, 1090/2, 120, 673, 176);
	gGraphics->drawText("by simpler", 970, 640, TextType::SMALL_DX);
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
	// Get the users public and local IP
	string publicIp = getPublicIp();
	string localIp = getLocalIp();

	// Add game to the database
	gDatabase->addServer(mUsername, getPublicIp(), getLocalIp());

	// Change the state
	changeState(PlayingOnline::Instance());
	PlayingOnline::Instance()->startServer();
	PlayingOnline::Instance()->setPlayerName(mUsername);

	// Sound
	gSound->playEffect(GAME_HOSTED_SOUND);
}

//! Message procedure.
LRESULT LobbyState::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case IDC_TRY_LOGIN:
		// Invalid player
		if(!gDatabase->validUser(name, pass) && strcmp(pass, "") != 0)	{
			gSound->playEffect(BAD_LOGIN_SOUND);
			badLogin = true;
			DialogBox(gGame->getAppInst(), MAKEINTRESOURCE(IDD_DIALOG1), gGame->getMainWnd(), (DLGPROC)DlgProc);
		}	
		else if(strcmp(pass, "") == 0)	{	// Guest player
			mUsername = string(name)+" (guest)";	
			gSound->playEffect(LOGIN_SOUND);
		}
		else {	// Registered player
			mUsername = string(name);
			gSound->playEffect(LOGIN_SOUND);
		}

		break;
	}

	// Let the server list process messages
	if(mServerList != NULL)
		mServerList->msgProc(msg, wParam, lParam);

	return 0;
}