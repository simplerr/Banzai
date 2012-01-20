#ifndef PLAYINGONLINE_W
#define PLAYINGONLINE_W

#include "common\d3dUtil.h"
#include "common\GameState.h"
#include "RakPeerInterface.h"
#include "StatusText.h"
#include "Enums.h"
#include <Windows.h>

class Menu;
class Game;
class Board;
class OnlinePlayer;
class Piece;
class Chat;
class Database;
class GUI;
class ServerList;
class Peer;

//! Active when playing online.
class PlayingOnline : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();

	RakNet::RakPeerInterface*	getPeer();

	string getPlayerName();
	OnlinePlayer* getPlayer();
	void setPlayerName(string name);

	bool startServer();
	bool connectToServer(string ip);
	void stateChange();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static PlayingOnline* Instance() {
		return &mPlayingOnline;
	}
protected:
	PlayingOnline() {};
private:
	static PlayingOnline		mPlayingOnline;
	IDirect3DTexture9*			mBkgd;

	OnlinePlayer*				mPlayer;
	GUI*						mGui;
	Peer*						mPeer;
	bool						mRunsServer;
	bool						mChangeState;
};

#endif