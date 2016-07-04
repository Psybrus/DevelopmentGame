#pragma version 430 core
#define SKIP_VERSION

#include <PsybrusTypes.psh>
#include <PsybrusUtility.psh>

precision highp float;
precision highp int;

layout (local_size_x = 1, local_size_y = 1) in;

#if OUTPUT_RGBA8
#  define DECODE_COLOUR gammaToLinear
#  define ENCODE_COLOUR linearToGamma

layout(rgba8) readonly uniform image2D aInputTexture;
layout(rgba8) writeonly uniform image2D aOutputTexture;

#elif OUTPUT_RGBA16F
#  define DECODE_COLOUR
#  define ENCODE_COLOUR

layout(rgba16f) readonly uniform image2D aInputTexture;
layout(rgba16f) writeonly uniform image2D aOutputTexture;
#endif


void main()
{
	int2 iId = int2(gl_GlobalInvocationID.xy) * 2;
	int2 oId = int2(gl_GlobalInvocationID.xy);
	float4 texel = 
		DECODE_COLOUR( imageLoad( aInputTexture, iId + int2( 0, 0 ) ).xyzw ) +
		DECODE_COLOUR( imageLoad( aInputTexture, iId + int2( 1, 0 ) ).xyzw ) +
		DECODE_COLOUR( imageLoad( aInputTexture, iId + int2( 0, 1 ) ).xyzw ) + 
		DECODE_COLOUR( imageLoad( aInputTexture, iId + int2( 1, 1 ) ).xyzw );
	imageStore( aOutputTexture, oId, ENCODE_COLOUR( texel / 4.0 ) );
}
