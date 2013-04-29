//=============================================================================
// color.fx                                                                         
//
// Transforms and colors geometry.
//=============================================================================


cbuffer cbPerObject
{
	float4x4 gWVP;
	float gameTime;
};

void VS(float3 iPosL : POSITION,
        float4 iColor : COLOR,
        out float4 oPosH  : SV_POSITION,
        out float4 oColor : COLOR)
{
	// Change the colours over time.
    oColor = iColor;
    oColor.x = abs(sin(gameTime));
    oColor.y = abs(sin(gameTime * 0.1f));
    oColor.z = abs(sin(gameTime * 0.01f));
    
	// Transform to homogeneous clip space.
	oPosH = mul(float4(iPosL, 1.0f), gWVP);
	oPosH.x += cos(gameTime) * 50.0f;
	oPosH.y += sin(gameTime) * 50.0f;
}

float4 PS(float4 posH  : SV_POSITION,
          float4 color : COLOR) : SV_Target
{
    return color;// * (posH.x + posH.y + posH.z) % 1.0f;
}
/*
RasterizerState Wireframe
{
        FillMode = Wireframe;
        CullMode = Back;
        FrontCounterClockwise = false;
};*/

technique10 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
        //SetRasterizerState(Wireframe);
    }
}

