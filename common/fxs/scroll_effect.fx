uniform extern texture  gTex;
uniform extern float	gWidth;
uniform extern float	gHeight;
uniform extern float	gViewDist;
uniform extern float	gOffset;
uniform extern float	gFadeLen;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

float4 ColorPS(float4 c : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{
	float4 texColor;
	
	 //Don't forget that pixel coordinates are from 0 -1
	 //Also remember that what's interesting here is the distance from the origin in the middle of the texture
	 //Therefor dx = 0.5 - tex0.x

	/*
		1.) Calculate pixel coordinate in local system
		2.) Find out it's offset (distance from origin of the scrollbox)
		3.) Test it's value against the view distance
	*/

	// Distance from the middle of the texture
	float x = tex0.x * gWidth - 0.5 * gWidth;
	float y = 0.5 * gHeight - tex0.y * gHeight;

	// gOffset is the distance from the texture to the middle of the scrollbox
	float dist = gOffset - y;

	texColor = tex2D(TexS, tex0).rgba;
	if(abs(dist) > gViewDist)
		texColor.a = 0;
	else if(abs(dist) < gViewDist && abs(dist) > gViewDist - gFadeLen)	{
		float delta = gViewDist - abs(dist);
		texColor.a *= delta/gFadeLen;
	}

	return texColor;
}

technique ScrollTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
		//vertexShader = NULL;
        pixelShader  = compile ps_2_0 ColorPS();
    }
}
