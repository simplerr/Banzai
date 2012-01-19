#include "common\Graphics.h"
#include "PiecesCaptured.h"
#include "Textures.h"

//! Constructor.
PiecesCaptured::PiecesCaptured(int x, int y, int piecesPerRow)
{
	mX = x;
	mY = y;
	mPiecesPerRow = piecesPerRow;
}
	
//! Destructor.
PiecesCaptured::~PiecesCaptured()
{

}

//! Draws all the captured pieces.
void PiecesCaptured::draw()
{
	int size = 32;
	// Draw the white caputured pieces.
	for(auto i = 0; i < mWhiteCaptured.size(); i++)
	{
		int x = i % mPiecesPerRow;
		int y = i / mPiecesPerRow;
		
		gGraphics->drawTexture(gTextures->getTexture(mWhiteCaptured[i], WHITE), mX + x * (size+10), mY + y * (size+10), size, size);
	}
	
	// Draw the black caputured pieces.
	for(auto i = 0; i < mBlackCaptured.size(); i++)
	{
		int x = i % mPiecesPerRow;
		int y = i / mPiecesPerRow;

		gGraphics->drawTexture(gTextures->getTexture(mBlackCaptured[i], BLACK), mX + x * (size+10), mY + 100 + y * (size+10), size, size);
	}
}

//! Add a captured piece.
/**
@param color The color of the piece.
@param type The piece type - QUEEN, PAWN, KING etc.
*/
void PiecesCaptured::add(Color color, PieceType type)
{
	if(color == WHITE)
		mWhiteCaptured.push_back(type);
	else
		mBlackCaptured.push_back(type);
}

//! Removes all captured pieces from the lists.
void PiecesCaptured::clear()
{
	mWhiteCaptured.clear();
	mBlackCaptured.clear();
}