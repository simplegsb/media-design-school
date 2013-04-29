// /////////////////////////
// Functions
// /////////////////////////

float3 flip(
	float distanceToEffectOrigin,
	float effectSpeed,
	float effectTime,
	float flipDuration,
	float3 flipVertexPosition,
	float3 otherVertexAPosition,
	float3 otherVertexBPosition)
{
	float flipStartTime = distanceToEffectOrigin / effectSpeed;

	[branch]
	if (effectTime < flipStartTime)
	{
		return otherVertexAPosition;
	}

	[branch]
	if (effectTime > flipStartTime + flipDuration)
	{
		return flipVertexPosition;
	}

	float3 flipAxis = otherVertexBPosition - otherVertexAPosition;
	float3 flipAxisCenter = otherVertexAPosition + (flipAxis / 2.0f);
	float3 flipAxisCenterToFlipVertex = flipVertexPosition - flipAxisCenter;
	float flipFraction = (effectTime - flipStartTime) / flipDuration;

	float3x3 rotation = createRotationMatrix(normalize(flipAxis), PI - flipFraction * PI);

	return flipAxisCenter + mul(flipAxisCenterToFlipVertex, rotation);
}
