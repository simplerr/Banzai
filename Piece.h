#ifndef PIECE_H
#define PIECE_H

#include "common\d3dUtil.h"
#include "Enums.h"

class Board;

//! Base class for all pieces.
class Piece
{
public:
	Piece(Color color, int x, int y, PieceType type);
	virtual ~Piece();

	virtual bool pinning(int x, int y);
	virtual void moved();

	IDirect3DTexture9* getTexture();
	Board*		getBoard();
	Position	getPos();
	Color		getColor();
	PieceType	getType();

	void		setPos(int x, int y);
	void		setPos(Position position);
	void		setColor(Color color);
	void		setType(PieceType type);
	void		setSize(int size);
	void		setTexture(IDirect3DTexture9* texture);
	void		setBoard(Board* board);
	void		setCaptured(bool captured);	
	bool		getCaptured();
private:
	IDirect3DTexture9*	mTexture;
	Board*				mBoard;
	Position			mPosition;
	Color				mColor;
	PieceType			mType;
	int					mSize;
	bool				mCaptured;
};

#endif