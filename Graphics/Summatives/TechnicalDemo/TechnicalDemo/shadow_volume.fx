// /////////////////////////
// States
// /////////////////////////

DepthStencilState shadowVolumeStencil
{
    DepthEnable = true;
    DepthWriteMask = ZERO;
    DepthFunc = Less;

    StencilEnable = true;
    StencilReadMask = 0x2;
    StencilWriteMask = 0x2;

    BackFaceStencilFunc = Always;
    BackFaceStencilDepthFail = Keep;
    BackFaceStencilPass = Invert;
    BackFaceStencilFail = Keep;

    FrontFaceStencilFunc = Always;
    FrontFaceStencilDepthFail = Keep;
    FrontFaceStencilPass = Invert;
    FrontFaceStencilFail = Keep;
};

DepthStencilState nonShadowStencil
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = Less_Equal;

    StencilEnable = true;
    StencilReadMask = 0x2;
    StencilWriteMask = 0x0;

    BackFaceStencilFunc = Equal;
    BackFaceStencilDepthFail = Keep;
    BackFaceStencilPass = Keep;
    BackFaceStencilFail = Keep;

    FrontFaceStencilFunc = Equal;
    FrontFaceStencilDepthFail = Keep;
    FrontFaceStencilPass = Keep;
    FrontFaceStencilFail = Keep;
};

// /////////////////////////
// Functions
// /////////////////////////

void extrudeEdgeIfPartOfSilhouette(
	float3 toLight,
	VertexShaderOutput vertexA,
	VertexShaderOutput vertexB,
	VertexShaderOutput adjacentVertex,
	float3 lightPosition,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{
    float3 adjacentFaceNormal =
		cross(vertexB.worldPosition - adjacentVertex.worldPosition, vertexA.worldPosition - adjacentVertex.worldPosition);

    //[branch]
    //if (dot(adjacentFaceNormal, toLight) <= 0.0f)
    {
		float3 vertexAExtruded = vertexA.worldPosition + 1000.0f * normalize(vertexA.worldPosition - lightPosition);
		float3 vertexBExtruded = vertexB.worldPosition + 1000.0f * normalize(vertexB.worldPosition - lightPosition);

		GeometryShaderOutput vertex0;
		vertex0.clipPosition = vertexA.clipPosition;
		vertex0.color = vertexA.color;
		vertex0.normal = vertexA.normal;
		vertex0.primitiveId = primitiveId;
		vertex0.texCoord = vertexA.texCoord;
		vertex0.worldPosition = vertexA.worldPosition;

		GeometryShaderOutput vertex1;
		vertex1.clipPosition = mul(float4(vertexAExtruded, 1.0f), cameraTransformation);
		vertex1.color = vertexA.color;
		vertex1.normal = vertexA.normal;
		vertex1.primitiveId = primitiveId;
		vertex1.texCoord = vertexA.texCoord;
		vertex1.worldPosition = vertexAExtruded;

		GeometryShaderOutput vertex2;
		vertex2.clipPosition = vertexB.clipPosition;
		vertex2.color = vertexB.color;
		vertex2.normal = vertexB.normal;
		vertex2.primitiveId = primitiveId;
		vertex2.texCoord = vertexB.texCoord;
		vertex2.worldPosition = vertexB.worldPosition;

		GeometryShaderOutput vertex3;
		vertex3.clipPosition = mul(float4(vertexBExtruded, 1.0f), cameraTransformation);
		vertex3.color = vertexB.color;
		vertex3.normal = vertexB.normal;
		vertex3.primitiveId = primitiveId;
		vertex3.texCoord = vertexB.texCoord;
		vertex3.worldPosition = vertexBExtruded;

		outputStream.Append(vertex0);
		outputStream.Append(vertex1);
		outputStream.Append(vertex2);
		outputStream.Append(vertex3);

		outputStream.RestartStrip();
	}
}

void generateShadowVolume(
	VertexShaderOutput input[6],
	float3 lightPosition,
	uint primitiveId,
	inout TriangleStream<GeometryShaderOutput> outputStream)
{	
    float3 faceNormal =
		cross(input[2].worldPosition - input[0].worldPosition, input[4].worldPosition - input[0].worldPosition);
    float3 toSunLight = lightPosition - input[0].worldPosition; // Approximated

    //[branch]
    //if (dot(faceNormal, toSunLight) > 0.0f)
    {
        extrudeEdgeIfPartOfSilhouette(toSunLight, input[0], input[2], input[1], lightPosition, primitiveId, outputStream);
        extrudeEdgeIfPartOfSilhouette(toSunLight, input[2], input[4], input[3], lightPosition, primitiveId, outputStream);
        extrudeEdgeIfPartOfSilhouette(toSunLight, input[4], input[0], input[5], lightPosition, primitiveId, outputStream);
    }
}
