#pragma version 430 core
#define SKIP_VERSION

#include <PsybrusTypes.psh>

precision highp float;
precision highp int;

#define readwrite

layout (local_size_x = 1) in;

struct vertex
{
	float4 Position_;
	float4 Normal_;
	float4 Tangent_;
	float4 Colour_;
	float4 TexCoord_;
};

layout(std430) readonly buffer iBuffer
{
	vertex Input[];
} In;

layout(r32f) readonly uniform image2D iTexture;

layout(std430) writeonly buffer oBuffer
{
	vertex Ouput[];
} Out;

layout(r32f) uniform image2D oTexture;


void main()
{
#if 0
	uint invocationIndex = gl_GlobalInvocationID.x;
	Out.Ouput[invocationIndex].Position_ = In.Input[invocationIndex].Position_;
	Out.Ouput[invocationIndex].Normal_ = In.Input[invocationIndex].Normal_;
	Out.Ouput[invocationIndex].Tangent_ = In.Input[invocationIndex].Tangent_;
	Out.Ouput[invocationIndex].Colour_ = In.Input[invocationIndex].Colour_;
	Out.Ouput[invocationIndex].TexCoord_ = In.Input[invocationIndex].TexCoord_;
#endif
	int2 id = int2(gl_GlobalInvocationID.xy);


	float texel = 
		imageLoad( iTexture, id + int2(-1, 0 ) ).x +
		imageLoad( iTexture, id + int2( 1, 0 ) ).x +
		imageLoad( iTexture, id + int2( 0, 1 ) ).x + 
		imageLoad( iTexture, id + int2( 0,-1 ) ).x;
	texel = texel * 0.5;
	texel -= imageLoad( oTexture, id ).x;
	texel = ( texel - ( texel * 0.0001 ) ) * 1.0;

	imageStore(oTexture, id, (texel.xxxx));
}
