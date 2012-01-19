uniform extern texture  gTex;
uniform extern float	gRadius;
uniform extern float	gBorder;

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

	float dx = 0.5 - tex0.x;// * 128 -  gPos.x;
	float dy = 0.5 - tex0.y;// * 128 -  gPos.y;
	float dist = sqrt(dx*dx + dy*dy);

	if(dist < 0.5 && dist > gBorder)// &&  dist > gRadius - gBorder)
	{
		texColor = tex2D(TexS, tex0).rgba;
		texColor.a *= 0.8;
	}
	else {
		texColor = tex2D(TexS, tex0).rgba;
		texColor.rgb /= 3;
		texColor.a = 0;
	}

	return texColor;
}

technique HollowCircleTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
		//vertexShader = NULL;
        pixelShader  = compile ps_2_0 ColorPS();
    }
}
