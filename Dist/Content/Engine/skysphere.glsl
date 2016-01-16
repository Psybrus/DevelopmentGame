#include <Psybrus.glsl>

//////////////////////////////////////////////////////////////////////////
// If rendering particles + in deferred, enable soft clipping.
#if defined( PERM_MESH_PARTICLE_3D ) && defined ( PERM_RENDER_DEFERRED )
#define SOFT_CLIPPING
#endif

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
VS_OUT( vec4, VsWorldPosition );

void vertexMain()
{
	PSY_MAKE_WORLD_SPACE_VERTEX( VsWorldPosition, InPosition_ );
	PSY_MAKE_CLIP_SPACE_VERTEX( gl_Position, VsWorldPosition );
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
PS_IN( vec4, VsWorldPosition );

//////////////////////////////////////////////////////////////////////////
// Samplers
PSY_SAMPLER_CUBE( ReflectionTex );

//////////////////////////////////////////////////////////////////////////
// pixelAll
void pixelAll( FRAMEBUFFER_INPUT )
{
	vec4 Diffuse = textureLod( aReflectionTex, VsNormal.xyz, 4.0 );
	vec4 Specular = vec4( 1.0, 1.0, 1.0, 1.0 );
	vec4 Normal = normalize( VsNormal );

	writeFrag( FRAMEBUFFER_INTERNAL, Diffuse * VsColour0, VsNormal.xyz, Specular.xyz );
}

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelMain()
{
	pixelAll( FRAMEBUFFER_OUTPUT );
}

#endif
