
void WeightedOITOriginal(float4 color, float linearZ, float z, float weightFactor, out float4 accumTarget, out float4 coverageTarget)
{
	// unmultiply alpha:
	color.rgb /= color.a;
	float weight = saturate(max(min(1.0, max(max(color.r, color.g), color.b) * color.a), color.a) *
		clamp(0.001 / (1e-5 + pow(z, 4.0)), 1e-2, 3e3));

	// Blend Func: GL_ONE, GL_ONE
	accumTarget = float4(color.rgb * color.a, color.a) * weight;
	// Blend Func: GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
	coverageTarget = color.a;
}

void WeightedOITCendos(float4 color, float linearZ, float z, float weightFactor, out float4 accumTarget, out float4 coverageTarget)
{
	// Insert your favorite weighting function here. The color-based factor
	// avoids color pollution from the edges of wispy clouds. The z-based
	// factor gives precedence to nearer surfaces.
	float weight = clamp((-1 / linearZ)  * color.a * weightFactor * 10, 0.01f, 10);
	
	// Blend Func: GL_ONE, GL_ONE
	// Switch to premultiplied alpha and weight
	accumTarget = color * weight;
	//accumTarget = float4(weight.xxx * color.a, color.a) * weight;

	// Blend Func: GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
	coverageTarget = color.a;
}

void PremultAlpha(float4 color, float linearZ, float z, float weightFactor, out float4 accumTarget, out float4 coverageTarget)
{
	accumTarget = color;
	coverageTarget = 0;
}

#ifdef OIT
#define TransparentColorOutput WeightedOITCendos
#else
#define TransparentColorOutput PremultAlpha
#endif
