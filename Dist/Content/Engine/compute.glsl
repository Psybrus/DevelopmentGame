#version 430 core
precision highp float;
precision highp int;

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

layout(std430) writeonly buffer oBuffer
{
	vertex Ouput[];
} Out;

void main()
{
	uint invocationIndex = gl_GlobalInvocationID.x;
	Out.Ouput[invocationIndex].Position_ = In.Input[invocationIndex].Position_;
	Out.Ouput[invocationIndex].Normal_ = In.Input[invocationIndex].Normal_;
	Out.Ouput[invocationIndex].Tangent_ = In.Input[invocationIndex].Tangent_;
	Out.Ouput[invocationIndex].Colour_ = In.Input[invocationIndex].Colour_;
	Out.Ouput[invocationIndex].TexCoord_ = In.Input[invocationIndex].TexCoord_;
} 
