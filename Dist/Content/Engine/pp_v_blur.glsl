#include <Psybrus.psh>

//////////////////////////////////////////////////////////////////////////
// Vertex shader
#if VERTEX_SHADER

VS_IN( vec4, InPosition_, POSITION );
VS_IN( vec4, InTexCoord_, TEXCOORD0 );

VS_OUT( vec4, VsTexCoord0 );

void vertexMain()
{
	VsPosition = InPosition_.xyzw;
	VsTexCoord0 = InTexCoord_;
}

#endif

//////////////////////////////////////////////////////////////////////////
// Pixel shader
#if PIXEL_SHADER

PS_IN( vec4, VsTexCoord0 );

PSY_SAMPLER_2D( InputTexture0 );

void pixelMain()
{
	float Offset = 1.0 / InputDimensions_[ 0 ].x;
	float Radius = Radius_;
	float MaxRadius = 8.0;
	float4 Colour = float4( 0.0, 0.0, 0.0, 0.0 );
	float4 UVOffset = float4( 0.0, -Radius * Offset, 0.0, 0.0 );
	float4 UVIncr = float4( 0.0, Offset, 0.0, 0.0 );

	for( float Idx = -MaxRadius; Idx <= MaxRadius; Idx += 1.0 )
	{
		if( Idx >= -Radius && Idx <= Radius )
		{
			Colour += PSY_SAMPLE_2D( InputTexture0, VsTexCoord0.xy + UVOffset.xy );
			UVOffset += UVIncr;
		}
	}
	outputFrag[0] = float4( ( Colour / ( Radius * 2.0 + 1.0 ) ).rgb, 1.0 );
}

#endif

