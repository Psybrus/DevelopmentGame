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
PSY_SAMPLER_2D( DiffuseTex );
PSY_SAMPLER_2D( SpecularTex );
PSY_SAMPLER_2D( OpacityTex );

#if defined( SOFT_CLIPPING )
PSY_SAMPLER_2D( DepthTex );
#endif

//////////////////////////////////////////////////////////////////////////
// pixelAll
void pixelAll( FRAMEBUFFER_INPUT )
{
	vec4 Diffuse = PSY_SAMPLE_2D( DiffuseTex, VsTexCoord0.xy );
	vec4 Specular = vec4( 1.0, 1.0, 1.0, 1.0 );
	vec4 Normal = normalize( VsNormal );

#if defined( SOFT_CLIPPING )
	float DstDepth  = linearDepth( PSY_SAMPLE_2D( DepthTex, vec2( gl_FragCoord.x, gl_FragCoord.y ) * ViewSize_.zw ).x, NearFar_.x, NearFar_.y );
	float SrcDepth = linearDepth( gl_FragCoord.z, NearFar_.x, NearFar_.y );
	float DiffDepth = clamp( ( DstDepth - SrcDepth ) / 1.0, 0.0, 1.0 );
	Diffuse.w *= DiffDepth;

#  if defined ( PERM_RENDER_DEFERRED )
	// If we're doing soft clipping for deferred it means we're no longer rendering to the gbuffer, so we need to go linear.
	Diffuse = gammaToLinear( Diffuse );
#  endif
#endif

#if defined( PERM_RENDER_FORWARD ) && defined( PERM_LIGHTING_DIFFUSE )
	Diffuse = gammaToLinear( Diffuse ) * VsColour0;
	Specular = gammaToLinear( Diffuse ) * VsColour0;
	vec3 DiffuseLight = vec3( 0.0 );
	vec3 SpecularLight = vec3( 0.0 );
	vec4 EyePosition = ViewTransform_[3];
	for( int LightIdx = 0; LightIdx < MAX_LIGHTS; ++LightIdx )
	{
		defaultLighting( LightIdx, EyePosition.xyz, VsWorldPosition.xyz, Normal.xyz, DiffuseLight, SpecularLight );
	}
	vec3 TotalLight = linearToGamma( Diffuse.xyz * DiffuseLight + Specular.xyz * SpecularLight );
	writeFrag( FRAMEBUFFER_INTERNAL, vec4( TotalLight, Diffuse.w ), Normal.xyz, Specular.xyz );
#else
	writeFrag( FRAMEBUFFER_INTERNAL, Diffuse * VsColour0, VsNormal.xyz, vec3( 1.0, 1.0, 1.0 ) );
#endif
}

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelMain()
{
	pixelAll( FRAMEBUFFER_OUTPUT );
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

	pixelAll( FRAMEBUFFER_OUTPUT );
}

#endif
