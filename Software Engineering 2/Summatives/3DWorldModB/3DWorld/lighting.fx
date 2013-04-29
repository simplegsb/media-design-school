// /////////////////////////
// Structures
// /////////////////////////

struct Light
{
	float4 ambient;
	float3 attenuation;
	float4 diffuse;
	float3 direction;
	float innerAngle;
	float outerAngle;
	float3 position;
	float range;
	float4 specular;
	float strengthAtOrigin;
	float strengthAtRange;
};

// /////////////////////////
// Functions
// /////////////////////////

float3 applyParallelLight(Light light, VertexShaderOutput input, float4 materialColour);
float3 applyParallelLight(Light light, VertexShaderOutput input, Material material);
float3 applyPointLight(Light light, VertexShaderOutput input, float4 materialColour);
float3 applyPointLight(Light light, VertexShaderOutput input, Material material);
float3 applySpotLight(Light light, VertexShaderOutput input, float4 materialColour);
float3 applySpotLight(Light light, VertexShaderOutput input, Material material);
float3 getLitColor(Light light, float3 lightDirection, float3 fragmentNormal, float3 fragmentPosition,
	Material material, float2 texCoord);

float3 applyParallelLight(Light light, VertexShaderOutput input, float4 materialColour)
{
	Material basicMaterial;
	basicMaterial.ambient = materialColour;
	basicMaterial.diffuse = materialColour;
	basicMaterial.emmissive = materialColour;
	basicMaterial.specular = materialColour;
	basicMaterial.specularPower = 1.0f;

	return applyParallelLight(light, input, basicMaterial);
}

float3 applyParallelLight(Light light, VertexShaderOutput input, Material material)
{
	return getLitColor(light, light.direction, input.normal, input.worldPosition, material, input.texCoord);
}

float3 applyPointLight(Light light, VertexShaderOutput input, float4 materialColour)
{
	Material basicMaterial;
	basicMaterial.ambient = materialColour;
	basicMaterial.diffuse = materialColour;
	basicMaterial.emmissive = materialColour;
	basicMaterial.specular = materialColour;
	basicMaterial.specularPower = 1.0f;

	return applyPointLight(light, input, basicMaterial);
}

float3 applyPointLight(Light light, VertexShaderOutput input, Material material)
{
	float3 lightVector = light.position - input.worldPosition;
	float distanceToFragment = length(lightVector);

	[branch]
	if(distanceToFragment > light.range)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}

	lightVector = normalize(lightVector);
	float3 litColor = getLitColor(light, lightVector, input.normal, input.worldPosition, material, input.texCoord);

	return litColor / dot(light.attenuation, float3(1.0f, distanceToFragment, distanceToFragment * distanceToFragment));
}

float3 applySpotLight(Light light, VertexShaderOutput input, float4 materialColour)
{
	Material basicMaterial;
	basicMaterial.ambient = materialColour;
	basicMaterial.diffuse = materialColour;
	basicMaterial.emmissive = materialColour;
	basicMaterial.specular = materialColour;
	basicMaterial.specularPower = 1.0f;

	return applySpotLight(light, input, basicMaterial);
}

float3 applySpotLight(Light light, VertexShaderOutput input, Material material)
{
	float3 litColor;

	float3 toFragment = input.worldPosition - light.position;
	float distanceToFragment = length(toFragment);
	toFragment = normalize(toFragment);
	float angleToFragment = acos(dot(toFragment, light.direction));

	[branch]
	if (angleToFragment > light.innerAngle && angleToFragment <= light.outerAngle)
	{
		float angleRange = light.outerAngle - light.innerAngle;
		float distanceIntoRange = angleToFragment - light.innerAngle;

		litColor = applyPointLight(light, input, material);
		litColor *= 1.0f - (distanceIntoRange / angleRange);
	}
	else if (angleToFragment > light.outerAngle)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		litColor = applyPointLight(light, input, material);
	}

	float strengthRange = light.strengthAtRange - light.strengthAtOrigin;
	float strengthFromOrigin = distanceToFragment / light.range * strengthRange;
	litColor *= light.strengthAtOrigin + strengthFromOrigin;

	return litColor;
}

float3 getLitColor(Light light, float3 lightDirection, float3 fragmentNormal, float3 fragmentPosition,
	Material material, float2 texCoord)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	litColor += material.ambient * light.ambient;
	litColor += material.emmissive;	

	float diffuseFactor = dot(lightDirection, fragmentNormal);

	[branch]
	if (diffuseFactor > 0.0f)
	{
		litColor += diffuseFactor * material.diffuse * light.diffuse;
		litColor = saturate(litColor);
	}

	[branch]
	if (textured)
	{
		litColor *= texture2d.Sample(linearSampler, texCoord);
	}

	[branch]
	if (diffuseFactor > 0.0f)
	{
		float3 toCamera = normalize(cameraTranslation - fragmentPosition);
		float3 reflection = reflect(lightDirection, fragmentNormal);
		float specularFactor = pow(material.specularPower, max(dot(reflection, toCamera), 0.0f));

		litColor += specularFactor * material.specular * light.specular;
	}

	return litColor;
}
