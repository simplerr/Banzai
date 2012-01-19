uniform extern texture  gTex;
uniform extern float	gBorder;
uniform extern float	gWidth;
uniform extern float	gHeight;

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
	
	// Don't forget that pixel coordinates are from 0 -1
	// Also remember that what's interesting here is the distance from the origin in the middle of the texture
	// Therefor dx = 0.5 - tex0.x

	float dx = 0.5 * gWidth - tex0.x * gWidth;// * 128 -  gPos.x;
	float dy = 0.5 * gHeight - tex0.y * gHeight;// * 128 -  gPos.y;
	
	texColor = tex2D(TexS, tex0).rgba;

	if(abs(dx) < gWidth/2 - gBorder && abs(dy) < gHeight/2 - gBorder)
		texColor.a = 0;

	return texColor;
}

technique BorderTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
		//vertexShader = NULL;
        pixelShader  = compile ps_2_0 ColorPS();
    }
}
