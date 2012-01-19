#include "King.h"
#include "Board.h"
#include "Rook.h"
#include "Piece.h"

//! Constructor.
King::King(Color color, int x, int y) : Piece(color, x, y, KING)
{
	mFirstMove = true;
	mCastled = false;
}

//! Destructor.
King::~King()
{

}

bool King::pinning(int x, int y)
{
	bool pinning = true;
	Position delta;
	delta.x = x - getPos().x;
	delta.y = y - getPos().y;

	// Normal movement
	if(abs(delta.x) + abs(delta.y) <= 2 && abs(delta.x) <= 1 && abs(delta.y) <= 1)
	{
		// Moves on another piece
		if(getBoard()->getPieceAt(x, y) != NULL) {
			// Friendly piece?
			if(getBoard()->getPieceAt(x, y)->getColor() == getColor())
				pinning = false;
		}
	}
	else
		pinning = false;

	// Check for castling
	if(mFirstMove)
	{
		// 2 to the right or left
		if(abs(delta.x) == 2 && delta.y == 0)
		{
			// Default is black right castling
			int dir = 1;
			int rookX = 7;
			int rookY = 0;

			// Left castling
			if(delta.x < 0) {
				dir = -1;
				rookX = 0;
			}

			// White color
			if(getColor() == WHITE)
				rookY = 7;

			// No piece in the way
			if(getBoard()->getPieceAt(getPos().x + dir, y) == NULL && getBoard()->getPieceAt(getPos().x + 2*dir, y) == NULL)
			{
				// The rook haven't moved
				Piece* piece = getBoard()->getPieceAt(rookX, rookY);
				if(piece != NULL && piece->getType() == ROOK)	
				{
					Rook* rook = dynamic_cast<Rook*>(piece);
					if(rook->firstMove())	{

						// Not check?
						int checkers;
						if(getBoard()->getCheckers(getColor(), checkers) == NULL)
						{
							// The path is not under attack
							if(!getBoard()->resultsInCheck(this, getPos().x + dir, y) && !getBoard()->resultsInCheck(this, getPos().x + 2*dir, y))
							{
								pinning = true;
								mCastled = true;
							}
							
						}
					}
				}
			}
		}
	}

	return pinning;
}

void King::moved()
{
	mFirstMove = false;

	// Move the rook. NOTE: Ugly..
	if(mCastled)
	{
		if(getColor() == WHITE)
		{
			if(getPos().x == 2)
				getBoard()->getPieceAt(0, 7)->setPos(3, 7);
			else if(getPos().x == 6)
				getBoard()->getPieceAt(7, 7)->setPos(5, 7);
		}
		else if(getColor() == BLACK)
		{
			if(getPos().x == 2)
				getBoard()->getPieceAt(0, 0)->setPos(3, 0);
			else if(getPos().x == 6)
				getBoard()->getPieceAt(7, 0)->setPos(5, 0);
		}

		mCastled = false;
	}
}

bool King::firstMove()
{
	return mFirstMove;
}