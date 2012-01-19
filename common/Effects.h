//#ifndef EFFECT_H
//#define EFFECT_H
//
//#include <string>
//#include "d3dUtil.h"
//#include "Vector.h"
//using namespace std;
//
//class BaseFX
//{
//public:
//	BaseFX(string file, string tech);
//	~BaseFX();
//
//	virtual void update(float dt) {};
//	virtual void draw() {};
//
//	virtual void build();
//protected:
//	ID3DXEffect* mEffect;
//	D3DXHANDLE mhTech;
//	string mFile;
//};
//
//class HollowCircleFX : public BaseFX
//{
//public:
//	HollowCircleFX();
//	~HollowCircleFX();
//
//	void draw(IDirect3DTexture9* texture, float x, float y, float radius, float hollowRadius, float rotation = 0);
//	void build();
//private:
//	// Effect parameters
//	D3DXHANDLE mhTex;
//	D3DXHANDLE mhRadius;
//	D3DXHANDLE mhBorder;
//};
//
//class GlowFX : public BaseFX
//{
//public:
//	GlowFX();
//	~GlowFX();
//
//	void update(float dt);
//	void draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float glowRadius, Vector glowSpot, float rotation = 0);
//	void build();
//private:
//	D3DXHANDLE mhGlowCounter;
//	D3DXHANDLE mhGlowSpot;
//	D3DXHANDLE mhGlowRadius;
//	float mCounter;
//};
//
//class BorderFX : public BaseFX
//{
//public:
//	BorderFX();
//	~BorderFX();
//
//	void draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float border, float rotation = 0);
//	void build();
//private:
//	D3DXHANDLE mhBorder;
//	D3DXHANDLE mhWidth;
//	D3DXHANDLE mhHeight;
//};
//
//class ScrollFX : public BaseFX
//{
//public:
//	ScrollFX();
//	~ScrollFX();
//
//	void draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float offset, float viewDistance, float fadeLen = 20.0f);
//	void build();
//private:
//	D3DXHANDLE mhHeight;
//	D3DXHANDLE mhWidth;
//	D3DXHANDLE mhViewDist;
//	D3DXHANDLE mhOffset;
//	D3DXHANDLE mhFadeLen;
//};
//
//#endif