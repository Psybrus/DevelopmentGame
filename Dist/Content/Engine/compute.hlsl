struct vertex
{
	float4 Position_;
	float4 Normal_;
	float4 Tangent_;
	float4 Colour_;
	float4 TexCoord_;
};

RWStructuredBuffer<vertex> iBuffer;
RWStructuredBuffer<vertex> oBuffer;

[numthreads(1,1,1)]
void main (uint3 id : SV_DispatchThreadID)
{
	uint invocationIndex = id.x;
	oBuffer[invocationIndex].Position_ = iBuffer[invocationIndex].Position_;
	oBuffer[invocationIndex].Normal_ = iBuffer[invocationIndex].Normal_;
	oBuffer[invocationIndex].Tangent_ = iBuffer[invocationIndex].Tangent_;
	oBuffer[invocationIndex].Colour_ = iBuffer[invocationIndex].Colour_;
	oBuffer[invocationIndex].TexCoord_ = iBuffer[invocationIndex].TexCoord_;
} 
