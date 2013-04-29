// /////////////////////////
// Functions
// /////////////////////////

float4 cartoonify(float4 color, float cartoonificationFactor)
{
	float4 output;

	output.r = round(color.r * cartoonificationFactor) / cartoonificationFactor;
	output.g = round(color.g * cartoonificationFactor) / cartoonificationFactor;
	output.b = round(color.b * cartoonificationFactor) / cartoonificationFactor;
	output.a = color.a;

    return output;
}
