#ifndef VERTEX_H
#define VERTEX_H

//=============================================================================
// Vertex.h by Frank Luna (C) 2005 All Rights Reserved.
//
// This file contains all of our vertex structures.  We will add to it
// as needed throughout the book.
//=============================================================================



#include <d3dx9.h>

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

//! Vertex struct for one colored vertices.
struct RectVertex
{
	RectVertex():pos(0.0f, 0.0f, 0.0f), color(0x00000000){}
	RectVertex(float x, float y, float z):pos(x,y,z){}
	RectVertex(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	D3DCOLOR color;
	static IDirect3DVertexDeclaration9* Decl;
};

//! Vertex struct for textured vertices.
struct TextureVertex
{
	TextureVertex():pos(0.0f, 0.0f, 0.0f){}
	TextureVertex(float x, float y, float z):pos(x,y,z){}
	TextureVertex(const D3DXVECTOR3& v):pos(v){}
	
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
	static IDirect3DVertexDeclaration9* Decl;
};

//! Vertex struct for vertices without color.
struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z):pos(x,y,z){}
	VertexPos(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9* Decl;
};


#endif // VERTEX_H