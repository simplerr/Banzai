#pragma once

#include "Enums.h"
#include "common\Vector.h"
#include "common\d3dUtil.h"
#include <string>
using namespace std;

class Board;
class Piece;

//! Base class representing the user playing the game.
class Player
{
public:
	Player(Color color);
	virtual ~Player();

	virtual void update(float dt);
	virtual void draw();
	virtual void handleCastling(Piece* king);
	virtual void handleCapture(Color color, PieceType type);

	ActionResult performAction();
	ActionResult moveSelectedPiece(Position pos);

	void    resetBoard();

	void	setBoard(Board* board);
	void	setName(string name);
	void	setOpponent(string name);
	void	setColor(Color color);
	void	setSelected(int x, int y);
	void	setInvalid(int x, int y);
	void	setLastMove(Position from = Position(-10, -10), Position to = Position(-10, -10));

	Color	getColor();
	Board*	getBoard();
	Piece*	getSelectedPiece();
	bool	getCheckMate();
	string	getName();
	string	getOpponent();
	Vector	getMouseOffset();
	LastMove getLastMove();

	void pieceMovedSound();
	void pieceCapturedSound();
private:
	IDirect3DTexture9*	mLine;
	IDirect3DTexture9*	mArrow;
	Piece*	mSelectedPiece;
	Board*	mBoard;
	Color	mColor;

	Vector		mMovingOffset;
	string		mName;
	string		mOpponent;
	LastMove	mLastMove;	
};