#include <string>
#include "common\Graphics.h"
#include "common\DirectInput.h"
#include "common\Vector.h"
#include "Board.h"
#include "Textures.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Enums.h"
#include "StatusText.h"

//! The constructor.
/**
@param squareSize The width and height for each square.
*/
Board::Board(int squareSize, bool flipped)
{
	// Load the textures
	mBlack = gGraphics->loadTexture("imgs\\black.bmp");
	mWhite = gGraphics->loadTexture("imgs\\white.bmp");
	mBorder = gGraphics->loadTexture("imgs\\border.bmp");
	mTextures = new Textures();
	mFlipped = flipped;

	// Set board position and square size
	mSquareSize = squareSize;
	mTopLeft = Vector(20, 20);

	// Create the pieces and place them at the correct locations
	initPieces(flipped);

	// Set off screen positions
	mInvalid = Position(-10, -10);
	mSelected = Position(-10, -10);
}
	
//! Destructor
Board::~Board()
{
	// Clear all pieces
	clearPieces();

	// Release all textures
	ReleaseCOM(mBlack);
	ReleaseCOM(mWhite);
	ReleaseCOM(mBorder);
	delete mTextures;
}

//! Creates all pieces and sets their location.
void Board::initPieces(bool flipped)
{
	int whiteY = 7;
	int blackY = 0;

	if(flipped)	{
		whiteY = 0;
		blackY = 7;
	}

	// Pawns
	for(int i = 0; i < 8; i++)	{
		if(!flipped)	{
			addPiece(PAWN, BLACK, i, 1);
			addPiece(PAWN, WHITE, i, 6);
		}
		else	{
			addPiece(PAWN, BLACK, i, 6);
			addPiece(PAWN, WHITE, i, 1);
		}
	}

	// Rooks
	addPiece(ROOK, BLACK, 0, blackY);
	addPiece(ROOK, BLACK, 7, blackY);
	addPiece(ROOK, WHITE, 0, whiteY);
	addPiece(ROOK, WHITE, 7, whiteY);

	// Knights
	addPiece(KNIGHT, BLACK, 1, blackY);
	addPiece(KNIGHT, BLACK, 6, blackY);
	addPiece(KNIGHT, WHITE, 1, whiteY);
	addPiece(KNIGHT, WHITE, 6, whiteY);

	// Bishops
	addPiece(BISHOP, BLACK, 2, blackY);
	addPiece(BISHOP, BLACK, 5, blackY);
	addPiece(BISHOP, WHITE, 2, whiteY);
	addPiece(BISHOP, WHITE, 5, whiteY);

	if(!flipped)
	{
		// Queens
		addPiece(QUEEN, BLACK, 3, blackY);
		addPiece(QUEEN, WHITE, 3, whiteY);
	
		// Kings
		addPiece(KING, BLACK, 4, blackY);
		addPiece(KING, WHITE, 4, whiteY);
	}
	else
	{
		// Queens
		addPiece(QUEEN, BLACK, 4, blackY);
		addPiece(QUEEN, WHITE, 4, whiteY);
	
		// Kings
		addPiece(KING, BLACK, 3, blackY);
		addPiece(KING, WHITE, 3, whiteY);
	}
	
}

//! Draws the board.
/**
Draws the background colors, pieces and action effects (invalid move, selected piece).
*/
void Board::draw()
{
	bool white = true;	// Used to get the right pattern

	// Draw the border
	//gGraphics->drawTexture(mBorder, 380, 380, 740, 740);

	// Draw the background grid
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0;  y < 8; y++)
		{
			// White square
			if(white)	{
				gGraphics->drawTexture(mWhite, mTopLeft.x + x*mSquareSize + mSquareSize/2, mTopLeft.y + y*mSquareSize + mSquareSize/2, mSquareSize, mSquareSize);	// white
				white = false;
			}
			// Black square
			else	{
				gGraphics->drawTexture(mBlack, mTopLeft.x + x*mSquareSize + mSquareSize/2, mTopLeft.y + y*mSquareSize + mSquareSize/2, mSquareSize, mSquareSize);	// black
				white = true;
			}
		}
		white = !white;
	}

	// Draw invalid move effect
	gGraphics->drawRect(mTopLeft.x + mInvalid.x*mSquareSize + mSquareSize/2, mTopLeft.y + mInvalid.y*mSquareSize + mSquareSize/2, mSquareSize, mSquareSize, true, D3DCOLOR_ARGB(255, 255, 0, 0));

	// Draw selected effect
	gGraphics->drawRect(mTopLeft.x + mSelected.x*mSquareSize + mSquareSize/2, mTopLeft.y + mSelected.y*mSquareSize + mSquareSize/2, mSquareSize, mSquareSize, true, D3DCOLOR_ARGB(255, 0, 255, 0));

	// Loop through and draw the pieces
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)
	{
		Piece* piece = (*iter);
		gGraphics->drawTexture(piece->getTexture(), mTopLeft.x + piece->getPos().x * mSquareSize + mSquareSize/2, mTopLeft.y + piece->getPos().y * mSquareSize + mSquareSize/2, mSquareSize, mSquareSize);
	}
}

//! Adds a piece to the board.
/**
@param type The piece type. QUEEN, PAWN, ROOK etc.
@param x The X coordinate of the piece to add.
@param y The Y coordinate of the piece to add.
*/
void Board::addPiece(PieceType type, Color color, int x, int y)
{
	// Check type and add the right piece
	Piece* piece = NULL;
	if(type == PAWN)	{
		if(mFlipped && color == WHITE)
			piece = new Pawn(color, x, y, UP);
		else if(mFlipped && color == BLACK)
			piece = new Pawn(color, x, y, DOWN);
		else if(!mFlipped && color == BLACK)
			piece = new Pawn(color, x, y, UP);
		else if(!mFlipped && color == WHITE)
			piece = new Pawn(color, x, y, DOWN);
	}
	else if(type == KNIGHT)	
		piece = new Knight(color, x, y);
	else if(type == BISHOP)	
		piece = new Bishop(color, x, y);
	else if(type == ROOK)	
		piece = new Rook(color, x, y);
	else if(type == QUEEN)	
		piece = new Queen(color, x, y);
	else if(type == KING)	
		piece = new King(color, x, y);

	// Get the texture from the Texture handler
	piece->setTexture(mTextures->getTexture(type, color));

	// Set the active board
	piece->setBoard(this);

	// Add it to the list
	mPieces.push_back(piece);
}

//! Removes a piece from the board.
/**
@param piece The piece to remove
*/
void Board::removePiece(Piece* piece)
{
	// Remove the piece and delete it
	Piece* tmp = piece;
	mPieces.remove(piece);
	delete tmp;
}

//! Translates a coordinate to a square coordinate.
/**
@param pos The position to translate.
@return The square coordinate.
@note Used to translate the mouse coordinate.
*/
Position Board::toGridPos(Vector pos)
{
	pos -= mTopLeft;
	Position position;
	position.x = (int)pos.x / mSquareSize;
	position.y = (int)pos.y / mSquareSize;

	return position;
}

//! Sets the place to draw the invalid move effect.
/**
@param x The X coordinate.
@param y The Y coordinate.
*/
void Board::setInvalid(int x, int y)
{
	mInvalid.x = x;
	mInvalid.y = y;
}

//! Sets the place to draw the selected piece effect.
/**
@param x The X coordinate.
@param y The Y coordinate.
*/
void Board::setSelected(int x, int y)
{
	mSelected.x = x;
	mSelected.y = y;
}

//! Returns the piece at given coordinate.
/**
@param x The X coordinate.
@param y The Y coordinate.
@return The piece at (x,y), NULL if none.
*/
Piece* Board::getPieceAt(int x, int y)
{
	// Loop through and find the piece at (x,y)
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)	
	{
		Piece* piece = (*iter);
		if(piece->getPos().x == x && piece->getPos().y == y && !piece->getCaptured())
			return piece;
	}

	return NULL;
}

//! Checks if moving the piece results in check.
/**
@param piece The piece to test.
@param x The X coordinate to move the piece to.
@param y The Y coordinate to move the piece to.
*/
bool Board::resultsInCheck(Piece* piece, int x, int y)
{
	bool check = false;
	Position oldPos = piece->getPos();

	Piece* capturedPiece = getPieceAt(x, y);
	piece->setPos(x, y);

	// Find the kings position
	Position kingPos;
	kingPos = getPiece(KING, piece->getColor())->getPos();
	
	if(capturedPiece != NULL)
		capturedPiece->setCaptured(true);

	// Find out if the king gets pinned
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)
	{
		Piece* p = (*iter);
		if(p->getColor() != piece->getColor() && p->pinning(kingPos.x, kingPos.y) && !p->getCaptured())	{
			check = true;
			break;
		}
	}

	if(capturedPiece != NULL)
		capturedPiece->setCaptured(false);
	
	piece->setPos(oldPos);
	return check;
}

//! Returns the piece with given color and type.
Piece* Board::getPiece(PieceType type, Color color)
{
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)
	{
		Piece* p = (*iter);
		if(p->getType() == type && p->getColor() == color)	{
			return p;
		}
	}

	return NULL;
}

//! Checks if if moving piece to (x,y) is a valid move.
bool Board::validMove(Piece* piece, int x, int y)
{
	if(x >= 0 && x <= 7 && y >= 0 && y <= 7)	{
		return piece->pinning(x, y) && !resultsInCheck(piece, x, y);
	}
	else
		return false;
}

/**
@param color The color to test if it gets checked.
@param checker Gets set to the piece checking the king.
@return The amount of checkers.
*/
Piece* Board::getCheckers(Color color, int& checkers)
{
	Piece* checker = NULL;
	checkers = 0;
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)
	{
		Piece* piece = (*iter);
		Position kingPos = getPiece(KING, color)->getPos();
		if(piece->pinning(kingPos.x, kingPos.y))	{
			checkers++;
			checker = piece;
		}
	}

	return checker;
}

//! Checks if the player with the color can move any piece to (x,y).
bool Board::canMoveTo(Color color, int x, int y)
{
	for(auto iter = mPieces.begin(); iter != mPieces.end(); iter++)	{
		if((*iter)->getColor() == color && validMove((*iter), x, y))
			return true;
	}
	
	return false;
}

//! Promotes a pawn to queen.
void Board::promotePawn(Pawn* pawn)
{
	Position pos = pawn->getPos();
	Color color = pawn->getColor();
	removePiece(pawn);
	addPiece(QUEEN, color, pos.x, pos.y);
}

//! Checks if the player with the color color is check mate.
bool Board::checkMate(Color color)
{
	// Find out the piece(s) that are checking
	Piece* checker = NULL;
	int checkers = 0;
	checker = getCheckers(color, checkers);
	if(checkers != 0)
	{
		// Is there any move white can do?
		// 1.) Can he move the king out of check?
		// 2.) Can he capture the checker?
		// 3.) Can he go between the checking line?

		// Impossible to capture or intersect two checking pieces
		if(!canMoveKing(color) && checkers >= 2)
			return true;

		return !canMoveKing(color) && !canMoveTo(color, checker->getPos().x, checker->getPos().y) && !canIntersectChecker(color, checker);
	}
	else 
		return false;
}

//! Can the player move the king?
bool Board::canMoveKing(Color color)
{
	Piece* king = getPiece(KING, color);
	Position pos = king->getPos();

	if(validMove(king, pos.x+1, pos.y) || validMove(king, pos.x+1, pos.y-1) || validMove(king, pos.x, pos.y-1) || validMove(king, pos.x-1, pos.y-1) ||
		   validMove(king, pos.x-1, pos.y) || validMove(king, pos.x-1, pos.y+1) || validMove(king, pos.x, pos.y+1) || validMove(king, pos.x+1, pos.y+1))
		return true;
	else
		return false;
}

//! Can the player intersect the checking piece?
bool Board::canIntersectChecker(Color color, Piece* checker)
{
	// Impossible to intersect a Knight
	if(checker->getType() == KNIGHT)
		return false;

	Position kingPos = getPiece(KING, color)->getPos();

	// Impossible to intersect something that's one unit close
	Position diff;
	diff.x = checker->getPos().x - kingPos.x;
	diff.y = checker->getPos().y - kingPos.y;

	if(abs(diff.x) + abs(diff.y) <= 2 && abs(diff.x) <= 1 && abs(diff.y) <= 1)
	{
		return false;
	}
	else
	{
		// Vertically
		if(diff.x == 0 && diff.y != 0)
		{
			for(int i = 0; i < abs(diff.y); i++)
			{
				if(canMoveTo(color, kingPos.x, kingPos.y + i*diff.y/abs(diff.y)))	{
					return true;
				}

			}
		}
		// Horizontally
		else if(diff.x != 0 && diff.y == 0)
		{
			for(int i = 0; i < abs(diff.x); i++)
			{
				if(canMoveTo(color, kingPos.x + i*diff.x/abs(diff.x), kingPos.y))	{
					return true;
				}

			}
		}
		// Diagonally
		else if(abs(diff.x) == abs(diff.y) && diff.x != 0)
		{
			for(int i = 0; i < abs(diff.x); i++)
			{
				if(canMoveTo(color, kingPos.x + i*diff.x/abs(diff.x), kingPos.y + i*diff.y/abs(diff.y)))	{
					return true;
				}

			}
		}
	}

	return false;
}

//! Resets the board, restores pieces and their position.
void Board::reset()
{
	// Remove old pieces
	clearPieces();

	// Add new pieces
	initPieces(mFlipped);
}

//! Returns the texture handler.
Textures* Board::getTextures()
{
	return mTextures;
}

//! Remove all pieces from the list.
void Board::clearPieces()
{
	// Clear the list of pieces
	mPieces.clear();
}