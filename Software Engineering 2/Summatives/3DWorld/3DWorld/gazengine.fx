// /////////////////////////
// Inputs/Outputs
// /////////////////////////

struct VertexShaderInput
{
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 clipPosition : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 worldPosition : POSITION;
};

struct GeometryShaderOutput
{
	float4 clipPosition : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 worldPosition : POSITION;
	uint primitiveId : SV_PrimitiveID;
};

// /////////////////////////
// Samplers
// /////////////////////////

SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// /////////////////////////
// States
// /////////////////////////

DepthStencilState defaultStencil
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = Less;

    StencilEnable = true;
    StencilReadMask = 0xFFFFFFFF;
    StencilWriteMask = 0xFFFFFFFF;

    BackFaceStencilFunc = Always;
    BackFaceStencilDepthFail = Keep;
    BackFaceStencilPass = Keep;
    BackFaceStencilFail = Keep;

    FrontFaceStencilFunc = Always;
    FrontFaceStencilDepthFail = Keep;
    FrontFaceStencilPass = Keep;
    FrontFaceStencilFail = Keep;
};

// /////////////////////////
// Constants
// /////////////////////////

float PI = 3.1416;

// /////////////////////////
// Variables
// /////////////////////////

float4x4 cameraTransformation;
float3 cameraTranslation;
float4x4 worldTransformation;

Texture2D normalMap;
Texture2D texture2d;

// /////////////////////////
// Functions
// /////////////////////////

float3x3 createRotationMatrix(float3 axis, float angle)
{
	float3x3 rotationMatrix;

	rotationMatrix[0][0] = cos(angle) + pow(axis.x, 2) * (1 - cos(angle));
	rotationMatrix[0][1] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
	rotationMatrix[0][2] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);

	rotationMatrix[1][0] = axis.y * axis.x * (1 - cos(angle)) + axis.z * sin(angle);
	rotationMatrix[1][1] = cos(angle) + pow(axis.y, 2) * (1 - cos(angle));
	rotationMatrix[1][2] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);

	rotationMatrix[2][0] = axis.z * axis.x * (1 - cos(angle)) - axis.y * sin(angle);
	rotationMatrix[2][1] = axis.z * axis.y * (1 - cos(angle)) + axis.x * sin(angle);
	rotationMatrix[2][2] = cos(angle) + pow(axis.z, 2) * (1 - cos(angle));

	return rotationMatrix;
}

float4x4 createRotationMatrix(float4 axis, float angle)
{
	float4x4 rotationMatrix;

	rotationMatrix[0][0] = cos(angle) + pow(axis.x, 2) * (1 - cos(angle));
	rotationMatrix[0][1] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
	rotationMatrix[0][2] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);
	rotationMatrix[0][3] = 0;

	rotationMatrix[1][0] = axis.y * axis.x * (1 - cos(angle)) + axis.z * sin(angle);
	rotationMatrix[1][1] = cos(angle) + pow(axis.y, 2) * (1 - cos(angle));
	rotationMatrix[1][2] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);
	rotationMatrix[1][3] = 0;

	rotationMatrix[2][0] = axis.z * axis.x * (1 - cos(angle)) - axis.y * sin(angle);
	rotationMatrix[2][1] = axis.z * axis.y * (1 - cos(angle)) + axis.x * sin(angle);
	rotationMatrix[2][2] = cos(angle) + pow(axis.z, 2) * (1 - cos(angle));
	rotationMatrix[2][3] = 0;

	rotationMatrix[3][0] = 0;
	rotationMatrix[3][1] = 0;
	rotationMatrix[3][2] = 0;
	rotationMatrix[3][3] = 1;

	return rotationMatrix;
}

// /////////////////////////
// Vertex Shaders
// /////////////////////////

VertexShaderOutput defaultVertexShader(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	output.color = input.color;

	float4x4 worldRotation = worldTransformation;
	worldRotation[3][0] = 0.0f;
	worldRotation[3][1] = 0.0f;
	worldRotation[3][2] = 0.0f;
	worldRotation[3][3] = 1.0f;
	float4 normal4 = mul(float4(input.normal, 1.0f), worldRotation);
	float3 normal3 = {normal4.x, normal4.y, normal4.z};
	output.normal = normal3;

	float4x4 clipTransformation = mul(worldTransformation, cameraTransformation);
	output.clipPosition = mul(float4(input.position, 1.0f), clipTransformation);

	output.texCoord = input.texCoord;

	float4 worldPosition4 = mul(float4(input.position, 1.0f), worldTransformation);
	float3 worldPosition3 = {worldPosition4.x, worldPosition4.y, worldPosition4.z};
	output.worldPosition = worldPosition3;
	
	return output;
}

// /////////////////////////
// Pixel Shaders
// /////////////////////////

float4 defaultPixelShader(VertexShaderOutput input) : SV_Target
{
	return input.color;
}

float4 texturedPixelShader(VertexShaderOutput input) : SV_Target
{
	return texture2d.Sample(linearSampler, input.texCoord);
}

// /////////////////////////
// Techniques
// /////////////////////////

technique10 defaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, defaultPixelShader()));
    }
}

technique10 texturedTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, texturedPixelShader()));
    }
}
