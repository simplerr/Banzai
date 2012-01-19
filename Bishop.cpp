#include "Bishop.h"
#include "Board.h"

//! Basic constructor.
/**
@param color Black or white.
@param x X coordinate to start at.
@param y Y coordinate to start at.
*/
Bishop::Bishop(Color color, int x, int y) : Piece(color, x, y, BISHOP)
{

}

//! Destructor.
Bishop::~Bishop()
{

}

bool Bishop::pinning(int x, int y)
{
	bool pinning = true;
	Position diff;
	diff.x = x - getPos().x;
	diff.y = y - getPos().y;

	// Used for increments
	int norm_x = (diff.x == 0 ? 0 : diff.x / abs(diff.x));
	int norm_y = (diff.y == 0 ? 0 : diff.y / abs(diff.y));

	// Can only move diagonally
	if(abs(diff.x) == abs(diff.y))
	{
		for(int counter = 1; counter < abs(diff.x); counter++)
		{
			if(getBoard()->getPieceAt(getPos().x + counter * norm_x, getPos().y + counter * norm_y) != NULL)	{
				pinning = false;
				break;
			}
		}
	}
	else
		pinning = false;

	// Check the last piece, which is (x,y)
	if(pinning)
	{
		if(getBoard()->getPieceAt(x, y) != NULL && getBoard()->getPieceAt(x, y)->getColor() == getColor())
			pinning = false;
	}

	return pinning;
}