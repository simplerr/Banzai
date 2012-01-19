#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

//! Represents the rook.
class Rook : public Piece
{
public:
	Rook(Color color, int x, int y);
	~Rook();

	bool pinning(int x, int y);
	void moved();

	bool firstMove();
private:
	bool mFirstMove;
};

#endif