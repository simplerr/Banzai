#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

//! Represents the pawn.
class Pawn : public Piece
{
public:
	Pawn(Color color, int x, int y);
	~Pawn();

	bool pinning(int x, int y);
	void moved();
private:
	bool mFirstMove;
};

#endif