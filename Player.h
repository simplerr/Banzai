#pragma once

#include "Enums.h"
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

	void    resetBoard();

	void	setBoard(Board* board);
	void	setName(string name);
	void	setOpponent(string name);
	void	setColor(Color color);
	void	setSelected(int x, int y);
	void	setInvalid(int x, int y);

	Color	getColor();
	Board*	getBoard();
	Piece*	getSelectedPiece();
	bool	getCheckMate();
	string	getName();
	string	getOpponent();
private:
	Piece*	mSelectedPiece;
	Board*	mBoard;
	Color	mColor;

	string	mName;
	string	mOpponent;
};