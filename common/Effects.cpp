//#include "Effects.h"
//#include "d3dUtil.h"
//#include "Graphics.h"
//
//BaseFX::BaseFX(string file, string tech)
//{
//	// Create the FX from a .fx file
//	ID3DXBuffer* errors = 0;
//	HR(D3DXCreateEffectFromFile(gd3dDevice, (char*)file.c_str(), 
//		0, 0, D3DXSHADER_DEBUG, 0, &mEffect, &errors));
//	if( errors ) 
//		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);	
//
//	// Get handle to the technique
//	mhTech = mEffect->GetTechniqueByName((char*)tech.c_str());
//}
//
//BaseFX::~BaseFX()
//{
//	// Release FX
//	ReleaseCOM(mEffect);
//}
//
//void BaseFX::build()
//{
//	// Overwrite
//}
//
//HollowCircleFX::HollowCircleFX()	
//	: BaseFX("fxs\\hollow_circle.fx", "HollowCircleTech")
//{
//
//}
//	
//HollowCircleFX::~HollowCircleFX()
//{
//
//}
//
//void HollowCircleFX::draw(IDirect3DTexture9* texture, float x, float y, float radius, float hollowRadius, float rotation)
//{
//	D3DXVECTOR2 pos(x, y);
//
//	// Set the technique and the value on the handles
//	HR(mEffect->SetTechnique(mhTech));
//	HR(mEffect->SetTexture(mhTex, texture));
//	HR(mEffect->SetFloat(mhRadius, radius));
//	HR(mEffect->SetFloat(mhBorder, hollowRadius/2/radius));
//
//	UINT numPasses = 0;
//	HR(mEffect->Begin(&numPasses, 0));
//	for(UINT i = 0; i < numPasses; ++i)
//	{
//		HR(mEffect->BeginPass(i));
//
//		gGraphics->drawTexture(texture, x, y, radius*2, radius*2, 0xFFFFFFFF, rotation);
//
//		HR(mEffect->EndPass());
//	}
//	HR(mEffect->End());
//}
//	
//void HollowCircleFX::build()
//{
//	mhTex  = mEffect->GetParameterByName(0, "gTex");
//	mhRadius = mEffect->GetParameterByName(0, "gRadius");
//	mhBorder = mEffect->GetParameterByName(0, "gBorder");
//}
//
//GlowFX::GlowFX()
//	: BaseFX("fxs\\glow_spot.fx", "GlowSpotTech")
//{
//	mCounter = 0;
//}
//	
//GlowFX::~GlowFX()
//{
//
//}
//
//void GlowFX::update(float dt)
//{
//	mCounter += dt;
//}
//
//void GlowFX::draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float glowRadius, Vector glowSpot, float rotation)
//{
//	HR(mEffect->SetTechnique(mhTech));
//	HR(mEffect->SetFloat(mhGlowCounter, mCounter));
//	HR(mEffect->SetValue(mhGlowSpot, &glowSpot, sizeof(Vector)));
//	HR(mEffect->SetFloat(mhGlowRadius, glowRadius));
//
//	UINT numPasses = 0;
//	HR(mEffect->Begin(&numPasses, 0));
//	for(UINT i = 0; i < numPasses; ++i)
//	{
//		HR(mEffect->BeginPass(i));
//
//		gGraphics->drawTexture(texture, x, y, width, height, 0xFFFFFFFF, rotation);
//
//		HR(mEffect->EndPass());
//	}
//	HR(mEffect->End());
//}
//	
//void GlowFX::build()
//{
//	mhGlowCounter = mEffect->GetParameterByName(0, "gCounter");
//	mhGlowSpot = mEffect->GetParameterByName(0, "gGlowSpot");
//	mhGlowRadius = mEffect->GetParameterByName(0, "gRadius");
//}
//
//BorderFX::BorderFX()	
//	: BaseFX("fxs\\border_rect.fx", "BorderTech")
//{
//
//}
//	
//BorderFX::~BorderFX()
//{
//
//}
//
//void BorderFX::draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float border, float rotation)
//{
//	HR(mEffect->SetTechnique(mhTech));
//	HR(mEffect->SetFloat(mhWidth, width));
//	HR(mEffect->SetFloat(mhHeight, height));
//	HR(mEffect->SetFloat(mhBorder, border));
//
//	UINT numPasses = 0;
//	HR(mEffect->Begin(&numPasses, 0));
//	for(UINT i = 0; i < numPasses; ++i)
//	{
//		HR(mEffect->BeginPass(i));
//
//		gGraphics->drawTexture(texture, x, y, width, height, D3DCOLOR_ARGB(255, 255, 0, 0), rotation);
//
//		HR(mEffect->EndPass());
//	}
//	HR(mEffect->End());
//}
//	
//void BorderFX::build()
//{
//	mhWidth = mEffect->GetParameterByName(0, "gWidth");
//	mhHeight = mEffect->GetParameterByName(0, "gHeight");
//	mhBorder = mEffect->GetParameterByName(0, "gBorder");
//}
//
//ScrollFX::ScrollFX()	
//	: BaseFX("fxs\\scroll_effect.fx", "ScrollTech")
//{
//
//}
//	
//ScrollFX::~ScrollFX()
//{
//
//}
//
//void ScrollFX::draw(IDirect3DTexture9* texture, float x, float y, int width, int height, float offset, float viewDistance, float fadeLen)
//{
//	// Set effect parameters
//	HR(mEffect->SetTechnique(mhTech));
//	HR(mEffect->SetFloat(mhWidth, width));
//	HR(mEffect->SetFloat(mhHeight, height));
//	HR(mEffect->SetFloat(mhViewDist, viewDistance));
//	HR(mEffect->SetFloat(mhOffset, offset));
//	HR(mEffect->SetFloat(mhFadeLen, fadeLen));
//
//	UINT numPasses = 0;
//	HR(mEffect->Begin(&numPasses, 0));
//	for(UINT i = 0; i < numPasses; ++i)
//	{
//		HR(mEffect->BeginPass(i));
//
//		gGraphics->drawTexture(texture, x, y, width, height, D3DCOLOR_ARGB(255, 255, 0, 0));
//
//		HR(mEffect->EndPass());
//	}
//	HR(mEffect->End());
//}
//
//void ScrollFX::build()
//{
//	mhWidth = mEffect->GetParameterByName(0, "gWidth");
//	mhHeight = mEffect->GetParameterByName(0, "gHeight");
//	mhViewDist = mEffect->GetParameterByName(0, "gViewDist");
//	mhOffset = mEffect->GetParameterByName(0, "gOffset");
//	mhFadeLen = mEffect->GetParameterByName(0, "gFadeLen");
//}