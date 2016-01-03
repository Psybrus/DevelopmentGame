#include <Psybrus.glsl>

//////////////////////////////////////////////////////////////////////////
// Vertex shader
#if VERTEX_SHADER

VS_IN( float4, InPosition_, POSITION );
VS_IN( float4, InNormal_, NORMAL );
VS_IN( float4, InTexCoord_, TEXCOORD0 );
VS_IN( float4, InColour_, COLOUR0 );

VS_OUT( float4, VsColour0 );
VS_OUT( float4, VsNormal );
VS_OUT( float4, VsTexCoord0 );

void vertexMain()
{
	float Offset = ( InTexCoord_.z * InTexCoord_.w ) * 0.5f;
	float3 WorldPosition = InPosition_.xyz;
	float3 ViewNormal = normalize( float3( mul( ViewTransform_, InNormal_ ).xy, 0.0001 ) );
	float3 CrossZ = cross( ViewNormal, float3( 0.0, 0.0, 1.0 ) );
	float4 ViewPosition = mul( ViewTransform_, float4( WorldPosition, 1.0 ) );

	VsPosition = mul( ProjectionTransform_, ViewPosition + float4( CrossZ * Offset, 0.0 ) );
	VsTexCoord0 = InTexCoord_;
	VsColour0 = InColour_;
	VsNormal = vec4( ViewNormal, 0.0 );
}

#endif

//////////////////////////////////////////////////////////////////////////
// Pixel shader
#if PIXEL_SHADER

PS_IN( float4, VsColour0 );
PS_IN( float4, VsNormal );
PS_IN( float4, VsTexCoord0 );

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelMain()
{
	writeFrag( FRAMEBUFFER_OUTPUT, VsColour0, VsNormal.xyz, float3( 0.0, 0.0, 0.0 ) );
}

#endif
