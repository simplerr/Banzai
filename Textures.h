#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include "common\d3dUtil.h"
#include "Enums.h"
using namespace std;

//! Key to use in the map.
struct TextureKey
{
	TextureKey(PieceType type, Color color)	{
		this->type = type;
		this->color = color;
	}

	PieceType type;
	Color color;
};

//! Contains all piece textures.
class Textures
{
public: 
	Textures();
	~Textures();

	IDirect3DTexture9* getTexture(PieceType type, Color color);
private:
	map<TextureKey, IDirect3DTexture9*> mTextures;
};

// The key as to have a < operator.
inline bool operator < (const TextureKey& a1, const TextureKey& a2)
{
	if (a1.type * 100 + a1.color < a2.type * 100 + a2.color)	
		return true;

	return false;
}

extern Textures* gTextures;

#endif