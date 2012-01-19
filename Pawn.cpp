#include "Pawn.h"
#include "Queen.h"
#include "Board.h"

//! Constructor.
Pawn::Pawn(Color color, int x, int y) : Piece(color, x, y, PAWN)
{
	mFirstMove = true;
}

//! Destructor.
Pawn::~Pawn()
{

}

bool Pawn::pinning(int x, int y)
{
	bool pinning = false;
	Position diff;
	diff.x = x - getPos().x;
	diff.y = y - getPos().y;

	diff.y *= getColor();	// NOTE: White is -1 and Black 1

	// Moving forward. 2 step is allowed if it's the first move
	if(diff.x == 0 && (diff.y == 1 || (diff.y == 2 && mFirstMove)))	
	{
		if(getBoard()->getPieceAt(x, y) == NULL)	{
			if(diff.y == 2 && mFirstMove)	{
				if(getBoard()->getPieceAt(x, getPos().y + getColor()) == NULL)
					pinning = true;
			}
			else
				pinning = true;
		}
	}

	// Capturing diagonally.
	if(abs(diff.x) == 1 && diff.y == 1)
	{
		if(getBoard()->getPieceAt(x, y) != NULL)
		{
			if(getBoard()->getPieceAt(x, y)->getColor() != getColor())
				pinning = true;
		}

	}

	return pinning;
}

void Pawn::moved()
{
	mFirstMove = false;

	// Check if queen promotion
	if(getPos().y == 0 && getColor() == WHITE)
		getBoard()->promotePawn(this);
	else if(getPos().y == 7 && getColor() == BLACK)
		getBoard()->promotePawn(this);
}