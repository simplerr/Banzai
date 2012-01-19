#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include "common\d3dUtil.h"
#include "common\Runnable.h"
#include "common\Camera.h"
#include "common\GfxStats.h"
#include "common\Animation.h"
#include "Enums.h"

using namespace std;

class Board;
class Piece;
class Player;
class GameState;
class Input;

/*
	Allways add _WINSOCKAPI_ to the preproccessor when using RakNet!
*/

//! Heart of the program. Contains all the game states.
class Game : public Runnable
{
public:
	Game(HINSTANCE hInstance, std::string caption, int width, int height, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	void init();
	void update(float t, float dt);
	void draw();

	void changeState(GameState* state);
	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Camera*		mCamera;
	GfxStats*	mGfxStats;
	GameState*	mGameState;
};

#endif