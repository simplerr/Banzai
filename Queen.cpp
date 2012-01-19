#include "Queen.h"
#include "Board.h"

//! Constructor.
Queen::Queen(Color color, int x, int y) : Piece(color, x, y, QUEEN)
{

}

//! Destructor.
Queen::~Queen()
{

}

bool Queen::pinning(int x, int y)
{
	// Uses both rook and bishop movement
	return rookPinning(x, y) || bishopPinning(x, y);
}

bool Queen::rookPinning(int x, int y)
{
	bool pinning = true;
	Position diff;
	diff.x = x - getPos().x;
	diff.y = y - getPos().y;

	// Used for increments
	int norm_x = (diff.x == 0 ? 0 : diff.x / abs(diff.x));
	int norm_y = (diff.y == 0 ? 0 : diff.y / abs(diff.y));

	// Can only move diagonally
	if((diff.x == 0 && diff.y != 0) || (diff.x != 0 && diff.y == 0))
	{
		for(int counter = 1; counter < (norm_x != 0 ? abs(diff.x) : abs(diff.y)); counter++)
		{
			if(getBoard()->getPieceAt(getPos().x + counter * norm_x, getPos().y + counter * norm_y) != NULL)	{
				pinning = false;
				break;
			}
		}
	}
	else
		pinning = false;

	// Check the last piece, ie (x,y)
	if(pinning)
	{
		if(getBoard()->getPieceAt(x, y) != NULL && getBoard()->getPieceAt(x, y)->getColor() == getColor())
			pinning = false;
	}

	return pinning;
}
	
bool Queen::bishopPinning(int x, int y)
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

	// Check the last piece, ie (x,y)
	if(pinning)
	{
		if(getBoard()->getPieceAt(x, y) != NULL && getBoard()->getPieceAt(x, y)->getColor() == getColor())
			pinning = false;
	}

	return pinning;
}