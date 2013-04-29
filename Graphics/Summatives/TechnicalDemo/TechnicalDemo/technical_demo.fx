#include "gazengine.fx"

#include "lighting.fx"

#include "cartoon.fx"
#include "flipper.fx"
#include "outline.fx"
#include "shadow_volume.fx"

// /////////////////////////
// States
// /////////////////////////

BlendState disableFrameBuffer
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0;
};

BlendState enableFrameBuffer
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState additiveBlending
{
    BlendEnable[0] = TRUE;
    RenderTargetWriteMask[0] = 0x0F;

    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;

    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
};

RasterizerState disableCulling
{
    CullMode = NONE;
};

RasterizerState enableCulling
{
    CullMode = BACK;
};

// /////////////////////////
// Constants
// /////////////////////////

float DECOLORIZING_RADIUS = 10.0f;

// /////////////////////////
// Variables
// /////////////////////////

Light ambientLight;
float3 effectOrigin;
float effectSpeed;
float effectTime;
Light sunLight;
float3 chuckPosition;

// /////////////////////////
// Vertex Shaders
// /////////////////////////

VertexShaderOutput decolorizingVertexShader(VertexShaderInput input)
{
	VertexShaderOutput output;

	output.color = input.color;

	float4 normal4 = mul(float4(input.normal, 1.0f), worldTransformation);
	float3 normal3 = {normal4.x, normal4.y, normal4.z};
	output.normal = normal3;

	float4x4 clipTransformation = mul(worldTransformation, cameraTransformation);
	output.clipPosition = mul(float4(input.position, 1.0f), clipTransformation);

	output.texCoord = input.texCoord;

	float4 worldPosition4 = mul(float4(input.position, 1.0f), worldTransformation);
	float3 worldPosition3 = {worldPosition4.x, worldPosition4.y, worldPosition4.z};
	output.worldPosition = worldPosition3;

	float3 toChuck = chuckPosition - worldPosition3;
	float lengthToChuck = length(toChuck);

	[branch]
	if (lengthToChuck < DECOLORIZING_RADIUS)
	{
		float fractionalLengthToChuck = lengthToChuck / DECOLORIZING_RADIUS;
		float averageColor = (output.color.r + output.color.g + output.color.b) / 3.0f;

		float redVariation = (output.color.r - averageColor) * fractionalLengthToChuck;
		output.color.r = averageColor + redVariation;

		float greenVariation = (output.color.g - averageColor) * fractionalLengthToChuck;
		output.color.g = averageColor + greenVariation;

		float blueVariation = (output.color.b - averageColor) * fractionalLengthToChuck;
		output.color.b = averageColor + blueVariation;

		output.color.a = 1.0f;
	}

	return output;
}

// /////////////////////////
// Geometry Shaders
// /////////////////////////

[maxvertexcount(4)]
void flippingGeometryShader(
	triangle VertexShaderOutput input[3],
	uint primitiveId : SV_PrimitiveID,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
	float maxDistanceToEffectOrigin = length(effectOrigin - input[0].worldPosition);
	uint flipVertexIndex = 0;
	uint otherVertexAIndex = 1;
	uint otherVertexBIndex = 2;

	[branch]
	if (length(effectOrigin - input[1].worldPosition) > maxDistanceToEffectOrigin)
	{
		maxDistanceToEffectOrigin = length(effectOrigin - input[1].worldPosition);
		flipVertexIndex = 1;
		otherVertexAIndex = 0;
		otherVertexBIndex = 2;
	}

	[branch]
	if (length(effectOrigin - input[2].worldPosition) > maxDistanceToEffectOrigin)
	{
		maxDistanceToEffectOrigin = length(effectOrigin - input[2].worldPosition);
		flipVertexIndex = 2;
		otherVertexAIndex = 0;
		otherVertexBIndex = 1;
	}

	float3 flipVertexPosition = flip(
		maxDistanceToEffectOrigin,
		effectSpeed,
		effectTime,
		0.5f,
		input[flipVertexIndex].worldPosition,
		input[otherVertexAIndex].worldPosition,
		input[otherVertexBIndex].worldPosition);

	GeometryShaderOutput flipVertex;
	flipVertex.clipPosition = mul(float4(flipVertexPosition, 1.0f), cameraTransformation);
	flipVertex.color = input[flipVertexIndex].color;
	flipVertex.normal = input[flipVertexIndex].normal;
	flipVertex.primitiveId = primitiveId;
	flipVertex.texCoord = input[flipVertexIndex].texCoord;
	flipVertex.worldPosition = flipVertexPosition;

	GeometryShaderOutput otherVertexA;
	otherVertexA.clipPosition = input[otherVertexAIndex].clipPosition;
	otherVertexA.color = input[otherVertexAIndex].color;
	otherVertexA.normal = input[otherVertexAIndex].normal;
	otherVertexA.primitiveId = primitiveId;
	otherVertexA.texCoord = input[otherVertexAIndex].texCoord;
	otherVertexA.worldPosition = input[otherVertexAIndex].worldPosition;

	GeometryShaderOutput otherVertexB;
	otherVertexB.clipPosition = input[otherVertexBIndex].clipPosition;
	otherVertexB.color = input[otherVertexBIndex].color;
	otherVertexB.normal = input[otherVertexBIndex].normal;
	otherVertexB.primitiveId = primitiveId;
	otherVertexB.texCoord = input[otherVertexBIndex].texCoord;
	otherVertexB.worldPosition = input[otherVertexBIndex].worldPosition;

	outputStream.Append(flipVertex);
	outputStream.Append(otherVertexA);
	outputStream.Append(otherVertexB);
	outputStream.Append(flipVertex);

	outputStream.RestartStrip();
}

[maxvertexcount(4)]
void shadowVolumeGeometryShader(
	triangleadj VertexShaderOutput input[6],
	uint primitiveId : SV_PrimitiveID,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
	generateShadowVolume(input, sunLight.position, primitiveId, outputStream);
}

// /////////////////////////
// Pixel Shaders
// /////////////////////////

float4 ambientPixelShader(VertexShaderOutput input) : SV_Target
{
	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

    return float4(applyParallelLight(surface, ambientLight, cameraTranslation), 1.0f);
}

float4 cartoonPixelShader(VertexShaderOutput input) : SV_Target
{
	float4 color;

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation) +
		applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 cartoonPixelShaderDiffuseOnly(VertexShaderOutput input) : SV_Target
{
	float4 color;
	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 cartoonPixelShaderSunOnly(VertexShaderOutput input) : SV_Target
{
	float4 color;

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	color = float4(applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 litPixelShader(VertexShaderOutput input) : SV_Target
{
	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	return float4(applyParallelLight(surface, ambientLight, cameraTranslation) +
		applyPointLight(surface, sunLight, cameraTranslation), 1.0f);
}

float4 sunPixelShader(VertexShaderOutput input) : SV_Target
{
	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

    return float4(applyPointLight(surface, sunLight, cameraTranslation), 1.0f);
}

float4 texturedCartoonPixelShader(VertexShaderOutput input) : SV_Target
{
	float4 color;
	float4 textureColor = texture2d.Sample(linearSampler, input.texCoord);

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = textureColor;
	surface.specular = textureColor;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation) +
		applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 texturedCartoonPixelShaderDiffuseOnly(VertexShaderOutput input) : SV_Target
{
	float4 color;
	float4 textureColor = texture2d.Sample(linearSampler, input.texCoord);

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = textureColor;
	surface.specular = textureColor;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 texturedCartoonPixelShaderSunOnly(VertexShaderOutput input) : SV_Target
{
	float4 color;
	float4 textureColor = texture2d.Sample(linearSampler, input.texCoord);

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = textureColor;
	surface.specular = textureColor;

	color = float4(applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return cartoonify(color, 10.0f);
}

float4 worldNoNormalPixelShader(VertexShaderOutput input) : SV_Target
{
	float4 color;

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = input.normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation) +
		applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return color;
}

float4 worldPixelShader(VertexShaderOutput input) : SV_Target
{
	float4 color;
	float3 bumpNormal = normalMap.Sample(linearSampler, input.texCoord);
	float3 normal = normalize(input.normal * bumpNormal);

	Surface surface;
	surface.position = input.worldPosition;
	surface.normal = normal;
	surface.diffuse = input.color;
	surface.specular = input.color;

	color = float4(applyParallelLight(surface, ambientLight, cameraTranslation) +
		applyPointLight(surface, sunLight, cameraTranslation), 1.0f);

	return color;
}

// /////////////////////////
// General Techniques
// /////////////////////////

technique10 chuckNoCartoonTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, litPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 chuckTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 decorationTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 worldTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, decolorizingVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, worldPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 worldNoGreyNormalTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, worldNoNormalPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 worldNoGreyTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, worldPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

technique10 worldNoNormalTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, decolorizingVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, worldNoNormalPixelShader()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(preOutlineStencil, 1);
        SetRasterizerState(enableCulling);
    }
}

// /////////////////////////
// Intro Techniques
// /////////////////////////

technique10 chuckIntroTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(CompileShader(gs_4_0, flippingGeometryShader()));
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShader()));
    }
}

technique10 worldIntroTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, decolorizingVertexShader()));
        SetGeometryShader(CompileShader(gs_4_0, flippingGeometryShader()));
        SetPixelShader(CompileShader(ps_4_0, worldPixelShader()));
    }
}

// /////////////////////////
// Shadow Techniques
// /////////////////////////

technique10 ambientTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShaderDiffuseOnly()));

        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(defaultStencil, 0);
        SetRasterizerState(enableCulling);
    }
}

technique10 shadowVolumeTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(CompileShader(gs_4_0, shadowVolumeGeometryShader()));
        SetPixelShader(CompileShader(ps_4_0, defaultPixelShader()));

        //SetBlendState(disableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetBlendState(enableFrameBuffer, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(shadowVolumeStencil, 2);
        SetRasterizerState(disableCulling);
    }
}

technique10 nonShadowTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShaderSunOnly()));

        SetBlendState(additiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetDepthStencilState(nonShadowStencil, 0);
        SetRasterizerState(enableCulling);
    }
}
