#pragma once

#include <string>
#include "common\GameState.h"
#include "common\d3dUtil.h"
using namespace std;

class ServerList;

//! Active when user browses active games.
class LobbyState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();

	void connectToServer(string ip);
	void hostServer();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static LobbyState* Instance() {
		return &mLobbyState;
	}
protected:
	LobbyState() {};
private:
	static LobbyState	mLobbyState;
	IDirect3DTexture9*	mBkgd;
	IDirect3DTexture9*  mLogo;
	ServerList*			mServerList;
	string				mUsername;
};