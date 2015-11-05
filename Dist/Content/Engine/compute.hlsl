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

Texture2D<uint4> iTexture;
RWTexture2D<uint4> oTexture;

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

	int2 Coord = int2(0, 0);
	for(int i = 0; i < 4; ++i)
	{	
		for(int j = 0; j < 4; ++j)	
		{
			oTexture[int2(i, j)] = uint4(255, 0, 0, 255);
		}
	}
} 
