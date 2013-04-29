// /////////////////////////
// States
// /////////////////////////

DepthStencilState outlineStencil
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;

    StencilEnable = true;
    StencilReadMask = 0x1;
    StencilWriteMask = 0x0;

    BackFaceStencilFunc = Not_Equal;
    BackFaceStencilDepthFail = Keep;
    BackFaceStencilPass = Keep;
    BackFaceStencilFail = Keep;

    FrontFaceStencilFunc = Not_Equal;
    FrontFaceStencilDepthFail = Keep;
    FrontFaceStencilPass = Keep;
    FrontFaceStencilFail = Keep;
};

DepthStencilState preOutlineStencil
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = Less;

    StencilEnable = true;
    StencilReadMask = 0x0;
    StencilWriteMask = 0x1;

    BackFaceStencilFunc = Always;
    BackFaceStencilDepthFail = Keep;
    BackFaceStencilPass = Replace;
    BackFaceStencilFail = Keep;

    FrontFaceStencilFunc = Always;
    FrontFaceStencilDepthFail = Keep;
    FrontFaceStencilPass = Replace;
    FrontFaceStencilFail = Keep;
};

// /////////////////////////
// Functions
// /////////////////////////

void createBody(
	VertexShaderOutput vertexIn0,
	VertexShaderOutput vertexIn1,
	float size,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream);
void createCap(
	VertexShaderOutput vertexIn0,
	VertexShaderOutput vertexIn1,
	float size,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream);
float4 getOffsetPosition(float4 offsetFromPosition, float4 offsetFromHeading, float offsetAngle, float offsetDistance);

void createBody(
	VertexShaderOutput vertexIn0,
	VertexShaderOutput vertexIn1,
	float size,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
	GeometryShaderOutput vertexOut0;
	vertexOut0.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 0.5f, size);
	vertexOut0.color = vertexIn0.color;
	vertexOut0.normal = vertexIn0.normal;
	vertexOut0.primitiveId = primitiveId;
	vertexOut0.texCoord = vertexIn0.texCoord;
	vertexOut0.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut1;
	vertexOut1.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 1.5f, size);
	vertexOut1.color = vertexIn0.color;
	vertexOut1.normal = vertexIn0.normal;
	vertexOut1.primitiveId = primitiveId;
	vertexOut1.texCoord = vertexIn0.texCoord;
	vertexOut1.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut2;
	vertexOut2.clipPosition =
		getOffsetPosition(vertexIn1.clipPosition, vertexIn0.clipPosition, PI * 1.5f, size);
	vertexOut2.color = vertexIn1.color;
	vertexOut2.normal = vertexIn1.normal;
	vertexOut2.primitiveId = primitiveId;
	vertexOut2.texCoord = vertexIn1.texCoord;
	vertexOut2.worldPosition = vertexIn1.worldPosition;

	GeometryShaderOutput vertexOut3;
	vertexOut3.clipPosition =
		getOffsetPosition(vertexIn1.clipPosition, vertexIn0.clipPosition, PI * 0.5f, size);
	vertexOut3.color = vertexIn1.color;
	vertexOut3.normal = vertexIn1.normal;
	vertexOut3.primitiveId = primitiveId;
	vertexOut3.texCoord = vertexIn1.texCoord;
	vertexOut3.worldPosition = vertexIn1.worldPosition;

	outputStream.Append(vertexOut0);
	outputStream.Append(vertexOut1);
	outputStream.Append(vertexOut2);
	outputStream.Append(vertexOut3);

	outputStream.RestartStrip();
}

void createCap(
	VertexShaderOutput vertexIn0,
	VertexShaderOutput vertexIn1,
	float size,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
	GeometryShaderOutput vertexOut0;
	vertexOut0.clipPosition = vertexIn0.clipPosition;
	vertexOut0.color = vertexIn0.color;
	vertexOut0.normal = vertexIn0.normal;
	vertexOut0.primitiveId = primitiveId;
	vertexOut0.texCoord = vertexIn0.texCoord;
	vertexOut0.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut1;
	vertexOut1.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 0.5f, size);
	vertexOut1.color = vertexIn0.color;
	vertexOut1.normal = vertexIn0.normal;
	vertexOut1.primitiveId = primitiveId;
	vertexOut1.texCoord = vertexIn0.texCoord;
	vertexOut1.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut2;
	vertexOut2.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 0.25f, size);
	vertexOut2.color = vertexIn0.color;
	vertexOut2.normal = vertexIn0.normal;
	vertexOut2.primitiveId = primitiveId;
	vertexOut2.texCoord = vertexIn0.texCoord;
	vertexOut2.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut3;
	vertexOut3.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 2.0f, size);
	vertexOut3.color = vertexIn0.color;
	vertexOut3.normal = vertexIn0.normal;
	vertexOut3.primitiveId = primitiveId;
	vertexOut3.texCoord = vertexIn0.texCoord;
	vertexOut3.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut4;
	vertexOut4.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 1.75f, size);
	vertexOut4.color = vertexIn0.color;
	vertexOut4.normal = vertexIn0.normal;
	vertexOut4.primitiveId = primitiveId;
	vertexOut4.texCoord = vertexIn0.texCoord;
	vertexOut4.worldPosition = vertexIn0.worldPosition;

	GeometryShaderOutput vertexOut5;
	vertexOut5.clipPosition =
		getOffsetPosition(vertexIn0.clipPosition, vertexIn1.clipPosition, PI * 1.5f, size);
	vertexOut5.color = vertexIn0.color;
	vertexOut5.normal = vertexIn0.normal;
	vertexOut5.primitiveId = primitiveId;
	vertexOut5.texCoord = vertexIn0.texCoord;
	vertexOut5.worldPosition = vertexIn0.worldPosition;

	outputStream.Append(vertexOut1);
	outputStream.Append(vertexOut2);
	outputStream.Append(vertexOut0);
	outputStream.Append(vertexOut3);
	outputStream.Append(vertexOut4);
	outputStream.Append(vertexOut5);

	outputStream.RestartStrip();
}

float4 getOffsetPosition(float4 offsetFromPosition, float4 offsetFromHeading, float offsetAngle, float offsetDistance)
{
	float4 ndcFromPosition = offsetFromPosition / offsetFromPosition.w;
	float4 ndcFromHeading = offsetFromHeading / offsetFromHeading.w;
	float4 ndcHeading = ndcFromPosition - ndcFromHeading;

	float4 offset;
	offset.x = ndcHeading.x * cos(offsetAngle) - ndcHeading.y * sin(offsetAngle);
	offset.y = ndcHeading.x * sin(offsetAngle) + ndcHeading.y * cos(offsetAngle);
	offset.z = 0.0f;
	offset.w = 0.0f;
	offset = normalize(offset);
	offset *= offsetDistance;

	return (ndcFromPosition + offset) * offsetFromPosition.w;
}

// /////////////////////////
// Geometry Shaders
// /////////////////////////

[maxvertexcount(48)]
void outlineGeometryShader(
	triangle VertexShaderOutput input[3],
	uint primitiveId : SV_PrimitiveID,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
	[unroll]
	for (int index = 0; index < 3; index++)
	{
		int vertex0Index = index;
		int vertex1Index = (index + 1) % 3;

		createBody(input[vertex0Index], input[vertex1Index], 0.01f, primitiveId, outputStream);
		createCap(input[vertex0Index], input[vertex1Index], 0.01f, primitiveId, outputStream);
		createCap(input[vertex1Index], input[vertex0Index], 0.01f, primitiveId, outputStream);
	}
}

// /////////////////////////
// Pixel Shaders
// /////////////////////////

float4 blackPixelShader(VertexShaderOutput input) : SV_Target
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

// /////////////////////////
// Techniques
// /////////////////////////

technique10 outlineTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, defaultVertexShader()));
        SetGeometryShader(CompileShader(gs_4_0, outlineGeometryShader()));
        SetPixelShader(CompileShader(ps_4_0, blackPixelShader()));

        SetDepthStencilState(outlineStencil, 1);
    }
}
