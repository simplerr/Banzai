#include "common\Graphics.h"
#include "common\Menu.h"
#include "common\DirectInput.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "PlayingOnline.h"
#include "MenuState.h"
#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "OnlinePlayer.h"
#include "Input.h"
#include "Chat.h"
#include <WindowsX.h>
#include <richedit.h>
#include "resource1.h"
#include "mysql++.h"
#include "Database.h"
#include <Windows.h>
#include "GUI.h"
#include "ServerList.h"
#include "Peer.h"

PlayingOnline PlayingOnline::mPlayingOnline;

void PlayingOnline::init(Game* game)
{
	// important!
	setGame(game);

	mPeer			= new Peer();
	mBkgd			= gGraphics->loadTexture("imgs\\bkgd.bmp");
	mPlayer			= new OnlinePlayer(WHITE);
	mGui			= new GUI();
	mChangeState	= false;

	mGui->setPlayer(mPlayer);
	mPeer->setPlayer(mPlayer);
	mPeer->setGui(mGui);
	mPlayer->setGui(mGui);
}

void PlayingOnline::cleanup()
{
	ReleaseCOM(mBkgd);
	mGui->clear();
	delete mPeer;
	delete mPlayer;
	delete mGui;
}

void PlayingOnline::update(double dt)
{
	mPlayer->update(dt);
	mPeer->update(dt);
	mGui->update(dt);

	// Ask the user if he really wants to leave when after he pressed enter
	if(gInput->keyPressed(VK_ESCAPE))	{
		int result = MessageBox(0, "Are you sure you want to exit the match?", "Leaving match", MB_YESNO | MB_ICONQUESTION);
		if(result == IDYES)
			changeState(MenuState::Instance());
	}

	// Change the state if true
	if(mChangeState)
		changeState(MenuState::Instance());
}

void PlayingOnline::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBkgd, 600, 400, 1200, 800);	

	// Draw the player which draws the gui and board
	mPlayer->draw();
}

//! Handle window messages for the Gui.
LRESULT PlayingOnline::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(mGui != NULL)
		return mGui->msgProc(msg, wParam, lParam);
	else
		return 0;
}

//! Get the name of the player.
string PlayingOnline::getPlayerName()
{
	return mPlayer->getName();
}

//! Start a server(game).
bool PlayingOnline::startServer()
{
	mPlayer->init(HOST);
	return mPeer->startServer();
}
	
//! Connect to a server(game).
bool PlayingOnline::connectToServer(string ip)
{
	mPlayer->init(CLIENT);
	return mPeer->connectToServer(ip);
}

//! Set the name of the player.
void PlayingOnline::setPlayerName(string name)
{
	mPlayer->setName(name);
}

//! Change the state next frame.
void PlayingOnline::stateChange()
{
	mChangeState = true;
}

//! Return the RakNet peer.
RakNet::RakPeerInterface* PlayingOnline::getPeer()
{
	return mPeer->getPeer();
}