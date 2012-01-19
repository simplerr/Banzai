#ifndef KING_H
#define KING_H

#include "Piece.h"

//! Represents the king.
class King : public Piece
{
public:
	King(Color color, int x, int y);
	~King();

	bool pinning(int x, int y);
	void moved();

	bool firstMove();
private:
	bool mFirstMove;
	bool mCastled;	// True if the king castled in the last move
};

#endif