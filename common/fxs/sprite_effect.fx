uniform extern texture  gTex;
uniform extern float3	gColor;
uniform extern float3	gBlack, gDark, gMedium, gLight, gWhite;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	MaxAnisotropy = 8;
	//Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU  = WRAP;
    AddressV  = WRAP;	// Causes the bottom of the image to be displayed at the top when scaled a lot
};

float4 ColorPS(float4 c : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{
	float4 texColor;

	// The colors used in the images
	float black = 0.0f;
	float dark = 64.0f / 255;
	float medium = 128.0f / 255;
	float light = 160.0f / 255;
	float white = 1.0f;

	texColor = tex2D(TexS, tex0).rgba;
	
	if(texColor.r == black)
		texColor.rgb = gBlack;
	else if(texColor.r == dark)
		texColor.rgb = gDark;
	else if(texColor.r == medium)
		texColor.rgb = gMedium;
	else if(texColor.r == light)
		texColor.rgb = gLight;
	else if(texColor.r == white)
		texColor.rgb = gWhite;

	return texColor;
}

technique SpriteTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
		//vertexShader = NULL;
        pixelShader  = compile ps_2_0 ColorPS();

		//SrcBlend = ZERO;

		// Turn off alpha blending - this removes the transparency around the border when the texture is scaled a lot
		//AlphaBlendEnable = FALSE;	
    }
}
