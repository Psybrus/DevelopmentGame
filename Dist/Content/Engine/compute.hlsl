struct vertex
{
	float4 Position_;
	float4 Normal_;
	float4 Tangent_;
	float4 Colour_;
	float4 TexCoord_;
};

RWByteAddressBuffer iBuffer;
RWByteAddressBuffer oBuffer;

[numthreads(1,1,1)]
void main (uint3 id : SV_DispatchThreadID)
{
	uint invocationIndex = id.x;
	uint address = invocationIndex * 80;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address));
} 
