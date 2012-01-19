#include "common\Graphics.h"
#include "Piece.h"
#include "Board.h"

//! Basic constructor.
/**
@param color Black or white.
@param x X coordinate to start at.
@param y Y coordinate to start at.
@param type PAWN, QUEEN, ROOK etc.
*/
Piece::Piece(Color color, int x, int y, PieceType type)
{
	setPos(x, y);
	setColor(color);
	setType(type);
	setSize(80);
	setTexture(NULL);
	setBoard(NULL);
	setCaptured(false);
}
	
//! Destructor.
Piece::~Piece()
{

}

//! Returns the position.
/**
@return The position on the board.
*/
Position Piece::getPos()
{
	return mPosition;
}
	
//! Returns the color.
/**
@return The color. Black or White.
*/
Color Piece::getColor()
{
	return mColor;
}

//! Sets the position on the board.
/**
@param x X coordinate.
@param y Y coordinate.
*/
void Piece::setPos(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

//! Sets the position on the board.
/**
@param position The position.
@note Same as setPos(int x, int y), just different arguments.
*/
void Piece::setPos(Position position)
{
	mPosition = position;
}

//! Sets the color.
/**
@param color The color, black or white.
*/
void Piece::setColor(Color color)
{
	mColor = color;
}

//! Sets the type.
/**
@param type The type. PAWN, QUEEN, KING etc.
*/
void Piece::setType(PieceType type)
{
	mType = type;
}

//! Returns the type.
/**
@return The type.
*/
PieceType Piece::getType()
{
	return mType;
}

//! Sets the width and height.
/**
@param size Size of the piece box.
@note Width = height.
*/
void Piece::setSize(int size)
{
	mSize = size;
}

//! Checks if the piece is pinning the square at (x, y).
/**
@return True if it con move/attack the square at(x, y).
*/
bool Piece::pinning(int x, int y)
{
	return false;
}

//! Returns the piece texture.
/**
@return The texture.
@note All textures points to the texture list in the Textures class.
*/
IDirect3DTexture9* Piece::getTexture()
{
	return mTexture;
}

//! Sets the texture.
/**
@param texture The texture to set.
*/
void Piece::setTexture(IDirect3DTexture9* texture)
{
	mTexture = texture;
}

//! Set the board the piece exist on.
/**
@param board The active board.
*/
void Piece::setBoard(Board* board)
{
	mBoard = board;
}

//! Returns the board the piece exist on.
/**
@return The active board
*/
Board* Piece::getBoard()
{
	return mBoard;
}

//! Performs any required tasks after the piece has moved.
/**
@note Gets called after a piece has moved. For pawns this is what prevents them to only move
2 steps the first time.
*/
void Piece::moved()
{

}

//! Indicates the piece was captured.
/**
@param captured True on capture.
*/
void Piece::setCaptured(bool captured)
{
	mCaptured = captured;
}

//! Returns if the piece is captured or not.
/**
@return True on captured.
*/
bool Piece::getCaptured()
{
	return mCaptured;
}