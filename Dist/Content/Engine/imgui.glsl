#include <Psybrus.glsl>

//////////////////////////////////////////////////////////////////////////
// Vertex shader
#if VERTEX_SHADER

VS_IN( vec4, InPosition_, POSITION );
VS_IN( vec4, InTexCoord_, TEXCOORD0 );
VS_IN( vec4, InColour_, COLOR0 );

VS_OUT( vec4, VS_COLOR0 );
VS_OUT( vec4, VS_TEXCOORD0 );

void vertexMain()
{
    gl_Position = mul( ClipTransform_, float4( InPosition_.xy, 0.0, 1.0 ) );
    VS_TEXCOORD0 = InTexCoord_;
    VS_COLOR0 = InColour_;
}

#endif

//////////////////////////////////////////////////////////////////////////
// Pixel shader
#if PIXEL_SHADER

PS_IN( vec4, VS_COLOR0 );
PS_IN( vec4, VS_TEXCOORD0 );

#if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_330
out float4 fragColor;
#endif

#if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_ES_100
#define fragColor gl_FragData[0]
#endif

//////////////////////////////////////////////////////////////////////////
// pixelDefaultMain
void pixelDefaultMain()
{
	fragColor = VS_COLOR0;
}

//////////////////////////////////////////////////////////////////////////
// pixelTexturedMain
PSY_SAMPLER_2D( DiffuseTex );

void pixelTexturedMain()
{
	vec4 Colour = PSY_SAMPLE_2D( DiffuseTex, VS_TEXCOORD0.xy );
	fragColor = Colour * VS_COLOR0;
}

#endif
