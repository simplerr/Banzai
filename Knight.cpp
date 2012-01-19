#include "Knight.h"
#include "Board.h"

//! Constructor.
Knight::Knight(Color color, int x, int y) : Piece(color, x, y, KNIGHT)
{

}

//! Destructor.
Knight::~Knight()
{

}

bool Knight::pinning(int x, int y)
{
	bool pinning = false;
	Position diff;
	diff.x = x - getPos().x;
	diff.y = y - getPos().y;

	// Valid move
	if((abs(diff.x) == 1 && abs(diff.y) == 2) || (abs(diff.x) == 2 && abs(diff.y) == 1))
	{
		// Make sure it's not a piece with the same color
		if(getBoard()->getPieceAt(x, y) != NULL && getBoard()->getPieceAt(x, y)->getColor() != getColor())
			pinning = true;
		else if(getBoard()->getPieceAt(x, y) == NULL)
			pinning = true;
	}

	return pinning;
}