#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

//! Represents the knight.
class Knight : public Piece
{
public:
	Knight(Color color, int x, int y);
	~Knight();

	bool pinning(int x, int y);
};

#endif