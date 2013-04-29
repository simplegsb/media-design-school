#include "gazengine.fx"
#include "lighting.fx"

// /////////////////////////
// Variables
// /////////////////////////

Light theOnlyLight;
Light headlight0Light;
Light headlight1Light;
Light turretLight;

// /////////////////////////
// States
// /////////////////////////

DepthStencilState enableDepth
{
    DepthEnable = true;
};

DepthStencilState disableDepth
{
    DepthEnable = false;
};

// /////////////////////////
// Pixel Shaders
// /////////////////////////

float4 headlightPixelShader(VertexShaderOutput input) : SV_Target
{
	return float4(
		applyPointLight(theOnlyLight, input, material) +
		applySpotLight(headlight0Light, input, material) +
		applySpotLight(headlight1Light, input, material) +
		applySpotLight(turretLight, input, material), 1.0f);
}

float4 litPixelShader(VertexShaderOutput input) : SV_Target
{
	return float4(applyPointLight(theOnlyLight, input, input.color), 1.0f);
}

// /////////////////////////
// Techniques
// /////////////////////////

technique10 headlightTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, headlightPixelShader()));

        SetDepthStencilState(enableDepth, 1);
    }
}

technique10 litNoDepthTestTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, litPixelShader()));

        SetDepthStencilState(disableDepth, 1);
    }
}

technique10 litTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, litPixelShader()));

        SetDepthStencilState(enableDepth, 1);
    }
}
