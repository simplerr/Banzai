uniform extern texture  gTex;
uniform extern float	gCounter;
uniform extern float	gRadius;
uniform extern float2   gGlowSpot;

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

	float dx = tex0.x * 1200 - gGlowSpot.x;// * 128 -  gPos.x;
	float dy = tex0.y * 800 - gGlowSpot.y;// * 128 -  gPos.y;
	float dist = sqrt(dx*dx + dy*dy);

	if(dist < gRadius)	
	{
		texColor = tex2D(TexS, tex0).rgba;
		texColor.rgb *= max(1.09*sin(gCounter/1.4), 1);
	}
	else
	{
		texColor = tex2D(TexS, tex0).rgba;
	}

	return texColor;
}

technique GlowSpotTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
		//vertexShader = NULL;
        pixelShader  = compile ps_2_0 ColorPS();
    }
}
