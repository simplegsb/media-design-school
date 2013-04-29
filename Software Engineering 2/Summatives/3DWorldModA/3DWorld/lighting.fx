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

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 emmissive;
    float4 specular;
    float specularPower;
};

// /////////////////////////
// Variables
// /////////////////////////

Material material;

// /////////////////////////
// Functions
// /////////////////////////

float3 applyParallelLight(Light light, float3 fragmentNormal, float3 fragmentPosition, float4 materialColour);
float3 applyPointLight(Light light, float3 fragmentNormal, float3 fragmentPosition, float4 materialColour);
float3 applyParallelLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material);
float3 applyPointLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material);
float3 applySpotLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material);
float3 getLitColor(Light light, float3 lightDirection, float3 fragmentNormal, float3 fragmentPosition,
	Material material);

float3 applyParallelLight(Light light, float3 fragmentNormal, float3 fragmentPosition, float4 materialColour)
{
	Material basicMaterial;
	basicMaterial.ambient = materialColour;
	basicMaterial.diffuse = materialColour;
	basicMaterial.emmissive = materialColour;
	basicMaterial.specular = materialColour;
	basicMaterial.specularPower = 1.0f;

	return applyParallelLight(light, fragmentNormal, fragmentPosition, basicMaterial);
}

float3 applyPointLight(Light light, float3 fragmentNormal, float3 fragmentPosition, float4 materialColour)
{
	Material basicMaterial;
	basicMaterial.ambient = materialColour;
	basicMaterial.diffuse = materialColour;
	basicMaterial.emmissive = materialColour;
	basicMaterial.specular = materialColour;
	basicMaterial.specularPower = 1.0f;

	return applyPointLight(light, fragmentNormal, fragmentPosition, basicMaterial);
}

float3 applyParallelLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material)
{
	return getLitColor(light, light.direction, fragmentNormal, fragmentPosition, material);
}

float3 applyPointLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material)
{
	float3 lightVector = light.position - fragmentPosition;
	float distanceToFragment = length(lightVector);

	[branch]
	if(distanceToFragment > light.range)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}

	lightVector = normalize(lightVector);
	float3 litColor = getLitColor(light, lightVector, fragmentNormal, fragmentPosition, material);

	return litColor / dot(light.attenuation, float3(1.0f, distanceToFragment, distanceToFragment * distanceToFragment));
}

float3 applySpotLight(Light light, float3 fragmentNormal, float3 fragmentPosition, Material material)
{
	float3 litColor;

	float3 toFragment = fragmentPosition - light.position;
	float distanceToFragment = length(toFragment);
	toFragment = normalize(toFragment);
	float angleToFragment = acos(dot(toFragment, light.direction));

	[branch]
	if (angleToFragment > light.innerAngle && angleToFragment <= light.outerAngle)
	{
		float angleRange = light.outerAngle - light.innerAngle;
		float distanceIntoRange = angleToFragment - light.innerAngle;

		litColor = applyPointLight(light, fragmentNormal, fragmentPosition, material);
		litColor *= 1.0f - (distanceIntoRange / angleRange);
	}
	else if (angleToFragment > light.outerAngle)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		litColor = applyPointLight(light, fragmentNormal, fragmentPosition, material);
	}

	float strengthRange = light.strengthAtRange - light.strengthAtOrigin;
	float strengthFromOrigin = distanceToFragment / light.range * strengthRange;
	litColor *= light.strengthAtOrigin + strengthFromOrigin;

	return litColor;
}

float3 getLitColor(Light light, float3 lightDirection, float3 fragmentNormal, float3 fragmentPosition,
	Material material)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	litColor += material.ambient * light.ambient;
	litColor += material.emmissive;	

	float diffuseFactor = dot(lightDirection, fragmentNormal);

	[branch]
	if (diffuseFactor > 0.0f)
	{
		litColor += diffuseFactor * material.diffuse * light.diffuse;

		float3 toCamera = normalize(cameraTranslation - fragmentPosition);
		float3 reflection = reflect(lightDirection, fragmentNormal);
		float specularFactor = pow(material.specularPower, max(dot(reflection, toCamera), 0.0f));

		litColor += specularFactor * material.specular * light.specular;
	}

	return litColor;
}
