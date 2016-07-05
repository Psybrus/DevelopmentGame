#pragma version 430 core
#define SKIP_VERSION

#include <PsybrusTypes.psh>
#include <PsybrusUtility.psh>

precision highp float;
precision highp int;

layout (local_size_x = 1, local_size_y = 1) in;

layout(rgba16f) readonly uniform image2D aInputTexture;
layout(r16f) writeonly uniform image2D aOutputTexture;

#endif

float luminance( float3 Input )
{
	return dot( Input, float3( 0.2126, 0.7152, 0.0722 ) );
}

void main()
{
	int2 iId = int2(gl_GlobalInvocationID.xy) * 2;
	int2 oId = int2(gl_GlobalInvocationID.xy);
	float3 texel = 
		imageLoad( aInputTexture, iId + int2( 0, 0 ) ).xyz +
		imageLoad( aInputTexture, iId + int2( 1, 0 ) ).xyz +
		imageLoad( aInputTexture, iId + int2( 0, 1 ) ).xyz +
		imageLoad( aInputTexture, iId + int2( 1, 1 ) ).xyz;
	imageStore( aOutputTexture, oId, float4( luminance( texel / 4.0 ), 0.0, 0.0, 0.0 ) );
}
