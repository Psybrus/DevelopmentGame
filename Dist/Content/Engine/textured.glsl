#include <Psybrus.glsl>

//////////////////////////////////////////////////////////////////////////
// Vertex shader
#if VERTEX_SHADER

VS_IN( vec4, InPosition_, POSITION );
VS_IN( vec4, InNormal_, NORMAL );
VS_IN( vec4, InTexCoord_, TEXCOORD0 );
VS_IN( vec4, InColour_, COLOUR0 );

#if defined( PERM_MESH_SKINNED_3D )

VS_IN( vec4, InBlendWeights_, BLENDWEIGHTS );
VS_IN( vec4, InBlendIndices_, BLENDINDICES );

#elif defined( PERM_MESH_PARTICLE_3D )

VS_IN( vec4, InVertexOffset_, TANGENT );

#endif

VS_OUT( vec4, VsColour0 );
VS_OUT( vec4, VsNormal );
VS_OUT( vec4, VsTexCoord0 );

void vertexMain()
{
 	vec4 WorldPosition;
	PSY_MAKE_WORLD_SPACE_VERTEX( WorldPosition, InPosition_ );
	PSY_MAKE_CLIP_SPACE_VERTEX( gl_Position, WorldPosition );
	VsColour0 = InColour_;
	PSY_MAKE_WORLD_SPACE_NORMAL( VsNormal, InNormal_ );
	VsTexCoord0 = InTexCoord_;
}

#endif

//////////////////////////////////////////////////////////////////////////
// Pixel shader
#if PIXEL_SHADER

PS_IN( vec4, VsColour0 );
PS_IN( vec4, VsNormal );
PS_IN( vec4, VsTexCoord0 );

#if defined( PERM_RENDER_DEFERRED )
#  if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_330
out float4 fragColor[4];
#  endif
#  if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_ES_100
#    define fragColor gl_FragData
#  endif

#else
#  if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_330
out float4 fragColor;
#  endif
#  if PSY_OUTPUT_CODE_TYPE == PSY_CODE_TYPE_GLSL_ES_100
#    define fragColor gl_FragData[0]
#  endif
#endif

//////////////////////////////////////////////////////////////////////////
// Samplers
PSY_SAMPLER_2D( DiffuseTex );
PSY_SAMPLER_2D( SpecularTex );
PSY_SAMPLER_2D( OpacityTex );

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelMain()
{
	vec4 Diffuse = PSY_SAMPLE_2D( DiffuseTex, VsTexCoord0.xy );

#if defined( PERM_RENDER_DEFERRED )
	fragColor[0] = vec4( ( VsNormal.xyz + vec3( 1.0, 1.0, 1.0 ) ) / 2.0, 1.0 );
	fragColor[1] = Diffuse * VsColour0;
	fragColor[2] = vec4( 0.0, 0.0, 0.0, 1.0 );
	fragColor[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
#else
	fragColor = Diffuse * VsColour0;
#endif
}

//////////////////////////////////////////////////////////////////////////
// pixelMainMask
void pixelMainMask()
{
	vec4 Opacity = PSY_SAMPLE_2D( OpacityTex, VsTexCoord0.xy );
	if( Opacity.x < 0.5 )
	{
		discard;
	}
	vec4 Diffuse = PSY_SAMPLE_2D( DiffuseTex, VsTexCoord0.xy );

#if defined( PERM_RENDER_DEFERRED )
	fragColor[0] = vec4( ( VsNormal.xyz + vec3( 1.0, 1.0, 1.0 ) ) / 2.0, 1.0 );
	fragColor[1] = Diffuse * VsColour0;
	fragColor[2] = vec4( 0.0, 0.0, 0.0, 1.0 );
	fragColor[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
#else
	fragColor = Diffuse * VsColour0;
#endif
}

#endif
