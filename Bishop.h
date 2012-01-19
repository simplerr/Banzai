#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

//! Represents the bishop.
class Bishop : public Piece
{
public:
	Bishop(Color color, int x, int y);
	~Bishop();

	bool pinning(int x, int y);
};

#endif