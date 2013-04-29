#include "gazengine.fx"

#include "lighting.fx"

// /////////////////////////
// Variables
// /////////////////////////

Light theOnlyLight;

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

float4 litPixelShader(VertexShaderOutput input) : SV_Target
{
	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	return float4(applyPointLight(surface, theOnlyLight, cameraTranslation), 1.0f);
}

// /////////////////////////
// Techniques
// /////////////////////////

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
