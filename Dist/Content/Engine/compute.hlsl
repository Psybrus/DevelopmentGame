struct vertex
{
	float4 Position_;
	float4 Normal_;
	float4 Tangent_;
	float4 Colour_;
	float4 TexCoord_;
};

ByteAddressBuffer iBuffer;
RWByteAddressBuffer oBuffer;

Texture2D<float> iTexture;
RWTexture2D<float> oTexture;

[numthreads(1,1,1)]
void main (uint3 id : SV_DispatchThreadID)
{
#if 0
	uint invocationIndex = id.x;
	uint address = invocationIndex * 80;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address)); address += 16;
	oBuffer.Store4(address, iBuffer.Load4(address));
#endif

	int width;
	int height;
	iTexture.GetDimensions( width, height );
	float texel = iTexture[ int2( int(id.x + 1) % width, int(id.y) ) ];
	oTexture[int2(int(id.x), int(id.y))] = texel;
} 
