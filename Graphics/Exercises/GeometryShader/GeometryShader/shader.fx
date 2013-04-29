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
};

struct VS_IN
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
    uint primitiveId : SV_PrimitiveID;
};

VS_OUT defaultVertexShader(VS_IN input)
{
	VS_OUT output;

    output.color = input.color;
	output.normal = input.normal;
	output.position = mul(float4(input.position, 1.0f), finalTransformation);
	
	return output;
}

VS_OUT starVertexShader(VS_IN input)
{
	VS_OUT output;

    output.color = input.color;
	output.normal = input.normal;
	output.position = float4(input.position, 1.0f);
	
	return output;
}

[maxvertexcount(6)]
void starGeometryShader(triangle VS_OUT input[3], uint primitiveId : SV_PrimitiveID, inout TriangleStream<GS_OUT> outputStream)
{
	GS_OUT output;
	[unroll]
	for(int index = 0; index < 3; index++)
	{
		output.color = input[index].color;
		output.normal = input[index].normal;
		output.position = mul(input[index].position, finalTransformation);
		output.primitiveId = primitiveId;
		outputStream.Append(output);
	}
	
	float4 toOne = input[1].position - input[0].position;
	float4 toTwo = input[2].position - input[0].position;
	float4 toThree = toOne + toTwo;
	toThree *= 2.0f / 3.0f;
	
	float4 positionThree = input[0].position + toThree;
	float4 positionFour = positionThree - toTwo;
	float4 positionFive = positionThree - toOne;
	
	output.color = input[0].color;
	output.normal = input[0].normal;
	output.position = mul(positionThree, finalTransformation);
	output.primitiveId = primitiveId;
	outputStream.Append(output);
	
	output.color = input[1].color;
	output.normal = input[1].normal;
	output.position = mul(positionFour, finalTransformation);
	output.primitiveId = primitiveId;
	outputStream.Append(output);
	
	output.color = input[2].color;
	output.normal = input[2].normal;
	output.position = mul(positionFive, finalTransformation);
	output.primitiveId = primitiveId;
	outputStream.Append(output);
}

float4 defaultPixelShader(VS_OUT input) : SV_Target
{
	return input.color;
}

float4 starPixelShader(GS_OUT input) : SV_Target
{
	return input.color;
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

technique10 starTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, starVertexShader()));
        SetGeometryShader(CompileShader(gs_4_0, starGeometryShader()));
        SetPixelShader(CompileShader(ps_4_0, starPixelShader()));
    }
}
