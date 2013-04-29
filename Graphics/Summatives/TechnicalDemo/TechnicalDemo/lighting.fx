// /////////////////////////
// Structures
// /////////////////////////

struct Light
{
	float3 position;
	float3 direction;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 attenuation;
	float  strength;
	float  range;
};

struct Surface
{
	float3 position;
    float3 normal;
    float4 diffuse;
    float4 specular;
};

// /////////////////////////
// Functions
// /////////////////////////

float3 applyParallelLight(Surface v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
 
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.direction;
	
	// Add the ambient term.
	litColor += v.diffuse * L.ambient;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(v.specular.a, 1.0f);
		float3 toEye     = normalize(eyePos - v.position);
		float3 R         = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
					
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.specular * L.specular;
	}
	
	return litColor;
}

float3 applyPointLight(Surface v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	// The vector from the surface to the light.
	float3 lightVec = L.position - v.position;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Add the ambient light term.
	litColor += v.diffuse * L.ambient;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(v.specular.a, 1.0f);
		float3 toEye     = normalize(eyePos - v.position);
		float3 R         = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
	
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.specular * L.specular;
	}
	
	// attenuate
	return litColor / dot(L.attenuation, float3(1.0f, d, d*d));
}

float3 applySpotlight(Surface v, Light L, float3 eyePos)
{
	float3 litColor = applyPointLight(v, L, eyePos);
	
	// The vector from the surface to the light.
	float3 lightVec = normalize(L.position - v.position);
	
	float s = pow(max(dot(-lightVec, L.direction), 0.0f), L.strength);
	
	// Scale color by spotlight factor.
	return litColor*s;
}

 
 