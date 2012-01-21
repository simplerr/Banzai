#ifndef LOCALSTATE_H
#define LOCALSTATE_H

#include "common\d3dUtil.h"
#include "common\GameState.h"
#include "StatusText.h"

class Menu;
class Game;
class Board;
class LocalPlayer;
class Piece;
class PiecesCaptured;

//! Active when playing against someone on the same computer.
class LocalState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void addCapturedPiece(Color color, PieceType type);
	void displayCheckMate();

	static LocalState* Instance() {
		return &mLocalState;
	}
protected:
	LocalState() {};
private:
	static LocalState mLocalState;
	IDirect3DTexture9* mBkgd;
	IDirect3DTexture9* mGreyBkgd;

	PiecesCaptured*	mPiecesCaptured;
	Piece*			mSelectedPiece;
	LocalPlayer*	mActivePlayer;
	LocalPlayer*	mPlayer2;
	Board*			mBoard;
	StatusText		mStatusText;
};

#endif