#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <vector>
#include "common\Vector.h"
#include "Enums.h"

using namespace std;

class Piece;
class Textures;
class Pawn;

//! Contains all the pieces.
class Board
{
public:
	Board(int squareSize, bool flipped = false);
	~Board();

	void draw();

	void initPieces(bool flipped);
	void clearPieces();
	void addPiece(PieceType type, Color color, int x, int y);
	void removePiece(Piece* piece);
	void reset();

	void		setInvalid(int x, int y);
	void		setSelected(int x, int y);
	Position	toGridPos(Vector pos);
	Vector		toGlobalPos(Position pos);
	void		promotePawn(Pawn* pawn);
	bool		validMove(Piece* piece, int x, int y);
	bool		checkMate(Color color);

	Piece*		getPieceAt(int x, int y);
	Piece*		getPiece(PieceType type, Color color);
	Textures*	getTextures();

	bool		resultsInCheck(Piece* piece, int x, int y);	// If piece moves to (x,y) will that result in a check on yourself?
	Piece*		getCheckers(Color color, int& checkers);
	int			getSquareSize();
	void		setMovingOffset(Vector offset);
	Vector		getMovingOffset();
private:
	// Helper functions.
	bool		canMoveKing(Color color);
	bool		canMoveTo(Color color, int x, int y);
	bool		canIntersectChecker(Color color, Piece* checker);
private:
	list<Piece*>		mPieces;
	Vector				mTopLeft;
	Textures*			mTextures;
	IDirect3DTexture9*	mBlack;
	IDirect3DTexture9*	mWhite;
	IDirect3DTexture9*  mBorder;
	
	Position	mInvalid;
	Position	mSelected;
	Vector		mMovingOffset;
	int			mSquareSize;
	bool		mFlipped;
};

#endif