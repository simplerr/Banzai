#include "Textures.h"
#include "common\Graphics.h"
#include "common\d3dUtil.h"

typedef pair<TextureKey, IDirect3DTexture9*> TexturePair;

//! Contstuctor
/**
Loads all textures.
*/
Textures::Textures()
{
	mTextures[TextureKey(PAWN, WHITE)]		=	gGraphics->loadTexture("imgs\\white_pawn.bmp");		// 0
	mTextures[TextureKey(PAWN, BLACK)]		=	gGraphics->loadTexture("imgs\\black_pawn.bmp");		// 1
	mTextures[TextureKey(KNIGHT, WHITE)]	=	gGraphics->loadTexture("imgs\\white_knight.bmp");	// 1
	mTextures[TextureKey(KNIGHT, BLACK)]	=	gGraphics->loadTexture("imgs\\black_knight.bmp");	// 2
	mTextures[TextureKey(BISHOP, WHITE)]	=	gGraphics->loadTexture("imgs\\white_bishop.bmp");	// 2
	mTextures[TextureKey(BISHOP, BLACK)]	=	gGraphics->loadTexture("imgs\\black_bishop.bmp");	// 3
	mTextures[TextureKey(ROOK, WHITE)]		=	gGraphics->loadTexture("imgs\\white_rook.png");		// 3
	mTextures[TextureKey(ROOK, BLACK)]		=	gGraphics->loadTexture("imgs\\black_rook.bmp");		// 4
	mTextures[TextureKey(QUEEN, WHITE)]		=	gGraphics->loadTexture("imgs\\white_queen.bmp");	// 4
	mTextures[TextureKey(QUEEN, BLACK)]		=	gGraphics->loadTexture("imgs\\black_queen.bmp");	// 5
	mTextures[TextureKey(KING, WHITE)]		=	gGraphics->loadTexture("imgs\\white_king.bmp");		// 5
	mTextures[TextureKey(KING, BLACK)]		=	gGraphics->loadTexture("imgs\\black_king.bmp");		// 6
}
	
//! Destructor.
Textures::~Textures()
{
	for(auto iter = mTextures.begin(); iter != mTextures.end(); iter++)
	{
		ReleaseCOM((*iter).second);
	}
}

//! Return the texture for a piece.
/**
@param type The type of piece. QUEEN, PAWN etc.
@param color The color of the piece. Black or white.
@return Pointer to the texture in the map.
*/
IDirect3DTexture9* Textures::getTexture(PieceType type, Color color)
{
	return mTextures.find(TextureKey(type, color))->second;
}