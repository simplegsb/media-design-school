//=============================================================================
// color.fx                                                                         
//
// Transforms and colors geometry.
//=============================================================================

#include "lighting.fx"

cbuffer cbPerObject
{
	float3 cameraTranslation;
	float4x4 finalTransformation;
	
	float4 parallelLightAmbient;
	float3 parallelLightAttenuation;
	float4 parallelLightDiffuse;
	float3 parallelLightDirection;
	float parallelLightRange;
	float4 parallelLightSpecular;
	float parallelLightStrength;
	float3 parallelLightTranslation;
	
	float4 pointLightAmbient;
	float3 pointLightAttenuation;
	float4 pointLightDiffuse;
	float3 pointLightDirection;
	float pointLightRange;
	float4 pointLightSpecular;
	float pointLightStrength;
	float3 pointLightTranslation;
};

void defaultVertexShader(
	float3 position : POSITION,
	float4 color : COLOR,
	float4 normal : NORMAL,
	out float3 oPosition : POSITION,
	out float4 oTransformedPosition : SV_POSITION,
	out float4 oColor : COLOR,
	out float4 oNormal : NORMAL)
{
    oColor = color;
	oNormal = normal;
	oPosition = position;
	oTransformedPosition = mul(float4(position, 1.0f), finalTransformation);
}

float4 defaultPixelShader(
	float3 position : POSITION,
	float4 transformedPosition : SV_POSITION,
	float4 color : COLOR,
	float4 normal : NORMAL) : SV_Target
{
	SurfaceInfo surface;
	surface.pos = position;
	surface.normal = normal;
	surface.diffuse = color;
	surface.spec = color;
	
	Light parallelLight;
	parallelLight.pos = parallelLightTranslation;
	parallelLight.dir = parallelLightDirection;
	parallelLight.ambient = parallelLightAmbient;
	parallelLight.diffuse = parallelLightDiffuse;
	parallelLight.spec = parallelLightSpecular;
	parallelLight.att = parallelLightAttenuation;
	parallelLight.spotPower = parallelLightStrength;
	parallelLight.range = parallelLightRange;
	
	Light pointLight;
	pointLight.pos = pointLightTranslation;
	pointLight.dir = pointLightDirection;
	pointLight.ambient = pointLightAmbient;
	pointLight.diffuse = pointLightDiffuse;
	pointLight.spec = pointLightSpecular;
	pointLight.att = pointLightAttenuation;
	pointLight.spotPower = pointLightStrength;
	pointLight.range = pointLightRange;
	
    return float4(ParallelLight(surface, parallelLight, cameraTranslation) +
		PointLight(surface, pointLight, cameraTranslation), 1.0f);
}

float4 cartoonPixelShader(
	float3 position : POSITION,
	float4 transformedPosition : SV_POSITION,
	float4 color : COLOR,
	float4 normal : NORMAL) : SV_Target
{
	SurfaceInfo surface;
	surface.pos = position;
	surface.normal = normal;
	surface.diffuse = color;
	surface.spec = color;
	
	Light parallelLight;
	parallelLight.pos = parallelLightTranslation;
	parallelLight.dir = parallelLightDirection;
	parallelLight.ambient = parallelLightAmbient;
	parallelLight.diffuse = parallelLightDiffuse;
	parallelLight.spec = parallelLightSpecular;
	parallelLight.att = parallelLightAttenuation;
	parallelLight.spotPower = parallelLightStrength;
	parallelLight.range = parallelLightRange;
	
	Light pointLight;
	pointLight.pos = pointLightTranslation;
	pointLight.dir = pointLightDirection;
	pointLight.ambient = pointLightAmbient;
	pointLight.diffuse = pointLightDiffuse;
	pointLight.spec = pointLightSpecular;
	pointLight.att = pointLightAttenuation;
	pointLight.spotPower = pointLightStrength;
	pointLight.range = pointLightRange;
	
	color = float4(ParallelLight(surface, parallelLight, cameraTranslation) +
		PointLight(surface, pointLight, cameraTranslation), 1.0f);
		
	color.x = round(color.x * 5.0f) / 5.0f;
	color.y = round(color.y * 5.0f) / 5.0f;
	color.z = round(color.z * 5.0f) / 5.0f;
	
    return color;
}

technique10 defaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, defaultPixelShader()));
    }
}

technique10 cartoonTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, cartoonPixelShader()));
    }
}
