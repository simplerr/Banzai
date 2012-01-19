#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

//! Abstract game state, inherit and states from here. Uses the singleton pattern.
class GameState
{
public:
	//! Inits the state.
	virtual void init(Game* game) = 0;

	//! Cleans up the state.
	virtual void cleanup(void) = 0;

	//! Updates the state.
	virtual void update(double dt) = 0;

	//! Draws the state.
	virtual void draw() = 0;

	//! Set the states game.
	void setGame(Game* game) {mGame = game;};

	//! Change the state.
	void changeState(GameState* state) {
		mGame->changeState(state);
	}

	//! Handle window messages.
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam) {return 0;}
protected:
	//! Constructor.
	GameState(){};
private:
	bool mStateChanged;
	Game *mGame;
};

#endif