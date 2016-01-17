#include <Psybrus.psh>

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
PSY_SAMPLER_CUBE( ReflectionTex );

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
	vec3 EyePosition = InverseViewTransform_[3].xyz;
	vec3 TotalSurface = vec3( 0.0 );

	Material InMaterial;
	InMaterial.Colour_ = MaterialBaseColour_.xyz * Diffuse.xyz;
	InMaterial.Specular_ = MaterialSpecular_;
	InMaterial.Roughness_ = MaterialRoughness_;
	InMaterial.Metallic_ = MaterialMetallic_;

	vec3 ReflectionColour = PSY_SAMPLE_CUBE_LOD( ReflectionTex, reflect( normalize( VsWorldPosition.xyz - EyePosition ), Normal.xyz ), MaterialRoughness_ ).xyz;

	for( int LightIdx = 0; LightIdx < MAX_LIGHTS; ++LightIdx )
	{
		Light InLight;
		InLight.Position_ = LightPosition_[ LightIdx ].xyz;
		InLight.Colour_ = LightDiffuseColour_[ LightIdx ].xyz;
		InLight.AttenuationCLQ_ = LightAttn_[ LightIdx ].xyz;
		TotalSurface += BRDF_Default( InLight, InMaterial, EyePosition.xyz, VsWorldPosition.xyz, Normal.xyz, ReflectionColour.xyz );
	}
	TotalSurface = min( vec3( 1.0 ), TotalSurface );
	TotalSurface = linearToGamma( TotalSurface );
	writeFrag( FRAMEBUFFER_INTERNAL, vec4( TotalSurface, Diffuse.w ), Normal.xyz, vec3( 0.0, 0.0, 0.0 ) );
#else
	writeFrag( FRAMEBUFFER_INTERNAL, Diffuse * VsColour0, VsNormal.xyz, Specular.xyz );
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
