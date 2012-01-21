#include <Windows.h>
#include "common\Graphics.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "MenuState.h"
#include "PlayingOnline.h"
#include "GUI.h"
#include "Chat.h"
#include "PiecesCaptured.h"
#include "OnlinePlayer.h"
#include "Database.h"
#include "Sound.h"

void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial");

//! Constructor.
GUI::GUI()
{
	// Load all textures
	mOpacity		=	gGraphics->loadTexture("imgs\\opacity.bmp");
	mGreyBkgd		=	gGraphics->loadTexture("imgs\\grey_bkgd.bmp");
	mOpponentLogo	=	gGraphics->loadTexture("imgs\\opponent_logo.bmp");

	// Create the status text
	mStatusText		=	StatusText("Invalid position!", 675, 380, 0.0f);

	// Create components.
	mPiecesCaptured =	new PiecesCaptured(700, 80);
	mChat			=	new Chat();

	// Build the upper panel.
	buildUpperPanel();
}
	
//! Destructor.
GUI::~GUI()
{
	// Release textures.
	ReleaseCOM(mOpacity);
	ReleaseCOM(mGreyBkgd);
	ReleaseCOM(mOpponentLogo);

	// Delete components.
	delete mPiecesCaptured;
	delete mChat;
}

//! Update the status text.
void GUI::update(float dt)
{
	mStatusText.update(dt);
}

//! Draw the DirectX part of the GUI.
/**
@note Most of the GUI is windows controls and is therefor not in here.
*/
void GUI::draw()
{
	// Gray backround on the left panel
	gGraphics->drawTexture(mGreyBkgd, 865, 140, 377, 240);
	mPiecesCaptured->draw();

	// Names
	if(mPlayer->getOpponent() != "#NOVALUE")
		gGraphics->drawText(mPlayer->getName() + " vs " + mPlayer->getOpponent(), 680, 20, BIG_DX, 8, D3DCOLOR_ARGB(255, 175, 77, 146));

	// Draw the status text
	mStatusText.draw();

	// Draw a opacity layer if the player is waiting for an opponent or an answer on the rematch request.
	if(mPlayer->waitingOnOpponent())
		gGraphics->drawTexture(mOpacity, 340, 340, 640, 640);
}
//! Create the stat part of the gui.
void GUI::initStats()
{
	return;

	// Coordinates
	int x = 777 + 140;
	int y = 340;

	// Get data from the database
	int wins = gDatabase->getWins(mPlayer->getOpponent());
	int losses = gDatabase->getLosses(mPlayer->getOpponent());

	// Create the controls
	char buffer[256];
	string total = "\n\n\n     Wins:	"+string(itoa(wins, buffer, 10))+"\n\n";
	total += "     Losses:	"+string(itoa(losses, buffer, 10))+"\n";

	mOpponentInfo = CreateWindow("Static", total.c_str(), WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
    x, y, 235, 130, gGame->getMainWnd(), NULL, gGame->getAppInst(), NULL);
	
	setFont(mOpponentInfo, 18);

	mPlayerName = CreateWindow("Static", mPlayer->getOpponent().c_str(), WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		20, 10, 100, 20, mOpponentInfo, NULL, gGame->getAppInst(), NULL);

	setFont(mPlayerName, 20, FW_HEAVY);
}

//! Builds the upper panel containing the resign and applaud button.
void GUI::buildUpperPanel()
{
	mResign = CreateWindow("Button", "Resign", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, 970, 270, 80, 40,
		gGame->getMainWnd(), (HMENU)IDC_RESIGN_BUTTON, gGame->getAppInst(), NULL);

	mApplaud = CreateWindow("Button", "Applaud", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED, 870, 270, 80, 40,
		gGame->getMainWnd(), (HMENU)IDC_APPLAUD_BUTTON, gGame->getAppInst(), NULL);

	setFont(mResign, 20);
	setFont(mApplaud, 20);
}

//! Displays a check mate message.
void GUI::displayCheckMate(bool won)
{
	string title = "Check mate";
	string text = "Congratulations you won!\nDo you want to offer a rematch?";

	if(!won)
		text = "You were defeated!\nDo you want to offer a rematch?";

	int msgId = MessageBox(gGame->getMainWnd(), text.c_str(), title.c_str(), MB_ICONASTERISK  | MB_YESNOCANCEL);

	if(msgId == IDYES)	{
		mPlayer->requestRematch();
		mPlayer->setWaitingOnAnswer(true);
		setStatus("Waiting on answer!", RED, 99.0f);
	}
}

//! Displays a opponent left game message.
bool GUI::displayOpponentLeft()
{
	int msgId = MessageBox(gGame->getMainWnd(), "Your opponent left the game.\nGo back to the main menu?", "Opponent left", MB_ICONEXCLAMATION | MB_OK);

	return true;
}

//! Displays a rematch request messsage.
void GUI::displayRematchRequest()
{
	// Accept rematch request from opponent
	RakNet::BitStream bitstream;
	string title = "Rematch request";
	string text = "Your opponent is offering you a rematch\nDo you accept it?";

	// Get answer
	int msgId = MessageBox(gGame->getMainWnd(), text.c_str(), title.c_str(), MB_ICONQUESTION | MB_YESNOCANCEL);

	// Check answer
	if(msgId == IDYES)	{
		mPlayer->restartMatch();
		gSound->playEffect(NEW_GAME_SOUND);
		bitstream.Write((unsigned char)ID_ACCEPT_REMATCH);
	}
	else if(msgId = IDNO)
		bitstream.Write((unsigned char)ID_DECLINE_REMATCH);

	// Send answer to opponent.
	PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

//! Set the player. NOTE: Needed?
void GUI::setPlayer(OnlinePlayer* player)
{
	mPlayer = player;
}

//! Change the status text.
void GUI::setStatus(string text, Color color, float time)
{
	mStatusText.setText(text, color, time);
}

//! Add a capture.
void GUI::addCapture(Color color, PieceType type)
{
	mPiecesCaptured->add(color, type);
}

//! Return the chat.
Chat* GUI::getChat()
{
	return mChat;
}

//! Clear window controls.
void GUI::clear()
{
	DestroyWindow(mResign);
	DestroyWindow(mApplaud);
	DestroyWindow(mUpperPanel);
	DestroyWindow(mPlayerName);
	DestroyWindow(mOpponentInfo);
}

//! Clear only the stats window controls.
void GUI::clearStats()
{
	DestroyWindow(mOpponentInfo);
	DestroyWindow(mPlayerName);
}

//! Display a rematch decline message.
void GUI::displayRematchDecline()
{
	MessageBox(gGame->getMainWnd(), "Your opponent denied the rematch request.\nGo back to the main menu?", "Rematch denied", MB_ICONEXCLAMATION | MB_OK);
}

//! Reset the captured pieces.
void GUI::clearCaptures()
{
	mPiecesCaptured->clear();
}

//! Procedure that handles the window messages from the gui components.
/**
@note Calls Chat::msgProc(...)
*/
LRESULT GUI::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		// The resign button was pressed
		if(wParam == IDC_RESIGN_BUTTON)
		{
			// Do the user really want to leave?
			int answer = MessageBox(gGame->getMainWnd(), "Are you sure you want to leave?", "Leaving game", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1);

			if(answer == IDYES)	{
				// Tell the opponent that you left.
				RakNet::BitStream bitstream;
				bitstream.Write((unsigned char)ID_LEFT_GAME);
				PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				PlayingOnline::Instance()->stateChange();
			}
		}
		// The applaud button was pressed
		else if(wParam == IDC_APPLAUD_BUTTON)
		{
			// Tell the opponent that you left.
			RakNet::BitStream bitstream;
			bitstream.Write((unsigned char)ID_SENT_APPLAUD);
			PlayingOnline::Instance()->getPeer()->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		break;
	}

	// Let the chat handle messages as well
	return mChat->msgProc(msg, wParam, lParam);
}