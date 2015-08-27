#include "Psybrus.hlsl"


////////////////////////////////////////////////////////////////////////
// VS_INPUT
#define VS_INPUT		VertexDefault

////////////////////////////////////////////////////////////////////////
// VS_OUTPUT
struct VS_OUTPUT
{
	float4 Position_	: SV_POSITION;
	float4 Normal_		: NORMAL;
	float4 Colour_		: COLOR0;
	float4 TexCoord0_	: TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////
// PS_OUTPUT
struct PS_OUTPUT
{
	float4 Colour_		: SV_TARGET;
};


////////////////////////////////////////////////////////////////////////
// vertexMain
VS_OUTPUT vertexMain( VS_INPUT i )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	float Offset = i.TexCoord0_.x * 2.0 - 1.0;
 	float3 WorldPosition = i.Position_.xyz;
	float3 CrossZ = normalize( cross( ViewTransform_[ 2 ].xyz, i.Normal_.xyz ) );
	WorldPosition = WorldPosition + CrossZ * Offset;
	o.Position_ = mul( ClipTransform_, float4( WorldPosition, 1.0 ) );
	o.Colour_ = i.Colour_;
	o.TexCoord0_ = i.TexCoord0_;
	return o;
}

////////////////////////////////////////////////////////////////////////
// pixelMain
PS_OUTPUT pixelMain( VS_OUTPUT i )
{
	PS_OUTPUT o = (PS_OUTPUT)0;
	o.Colour_ = i.Colour_;
	return o;
}
