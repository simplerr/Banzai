#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

//! Represents the queen.
class Queen : public Piece
{
public:
	Queen(Color color, int x, int y);
	~Queen();

	bool pinning(int x, int y);

private:
	bool rookPinning(int x, int y);
	bool bishopPinning(int x, int y);
};

#endif