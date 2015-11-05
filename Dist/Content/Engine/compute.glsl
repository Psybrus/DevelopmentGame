#version 430 core
precision highp float;
precision highp int;

#define readwrite

layout (local_size_x = 1) in;

struct vertex
{
	vec4 Position_;
	vec4 Normal_;
	vec4 Tangent_;
	vec4 Colour_;
	vec4 TexCoord_;
};

layout(std430) readonly buffer iBuffer
{
	vertex Input[];
} In;

layout(rgba8) readonly coherent uniform image2D iTexture;

layout(std430) writeonly buffer oBuffer
{
	vertex Ouput[];
} Out;

layout(rgba8) writeonly coherent uniform image2D oTexture;


void main()
{
	uint invocationIndex = gl_GlobalInvocationID.x;
	Out.Ouput[invocationIndex].Position_ = In.Input[invocationIndex].Position_;
	Out.Ouput[invocationIndex].Normal_ = In.Input[invocationIndex].Normal_;
	Out.Ouput[invocationIndex].Tangent_ = In.Input[invocationIndex].Tangent_;
	Out.Ouput[invocationIndex].Colour_ = In.Input[invocationIndex].Colour_;
	Out.Ouput[invocationIndex].TexCoord_ = In.Input[invocationIndex].TexCoord_;

	uvec2 Coord = uvec2( 0, 0 );
	for(int i = 0; i < 4; ++i)
	{	
		for(int j = 0; j < 4; ++j)	
		{
			imageStore(oTexture, ivec2(i, j), uvec4(255, 0, 0, 255));
		}
	}
}
