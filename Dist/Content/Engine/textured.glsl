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

	writeFrag( fragColour, Diffuse * VsColour0, VsNormal.xyz );
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

	writeFrag( fragColour, Diffuse * VsColour0, VsNormal.xyz );
}

#endif
