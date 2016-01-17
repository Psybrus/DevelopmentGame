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

PSY_SAMPLER_2D( ResolveTex );

void pixelMain()
{
	vec4 Colour = PSY_SAMPLE_2D( ResolveTex, VsTexCoord0.xy );
	outputFrag[0] = linearToGamma( vec4( Colour.xyz, 1.0 ) );
}

#endif

