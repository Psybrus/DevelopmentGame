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

#if defined( PERM_MESH_STATIC_3D ) || defined( PERM_MESH_SKINNED_3D )
VS_IN( vec4, InBinormal_, BINORMAL );
VS_IN( vec4, InTangent_, TANGENT );
#endif

#if defined( PERM_MESH_SKINNED_3D )

VS_IN( vec4, InBlendWeights_, BLENDWEIGHTS );
VS_IN( vec4, InBlendIndices_, BLENDINDICES );

#elif defined( PERM_MESH_PARTICLE_3D )

VS_IN( vec4, InVertexOffset_, TANGENT );

#endif

VS_OUT( vec4, VsColour0 );
VS_OUT( vec4, VsNormal );
#if defined( PERM_MESH_STATIC_3D ) || defined( PERM_MESH_SKINNED_3D )
VS_OUT( vec4, VsBinormal );
VS_OUT( vec4, VsTangent );
#endif
VS_OUT( vec4, VsTexCoord0 );
VS_OUT( vec4, VsWorldPosition );

void vertexMain()
{
	PSY_MAKE_WORLD_SPACE_VERTEX( VsWorldPosition, InPosition_ );
	PSY_MAKE_CLIP_SPACE_VERTEX( gl_Position, VsWorldPosition );
	VsColour0 = InColour_;
	PSY_MAKE_WORLD_SPACE_NORMAL( VsNormal, InNormal_ );
	VsTexCoord0 = InTexCoord_;

#if defined( PERM_MESH_STATIC_3D ) || defined( PERM_MESH_SKINNED_3D )
	PSY_MAKE_WORLD_SPACE_NORMAL( VsBinormal, InBinormal_ );
	PSY_MAKE_WORLD_SPACE_NORMAL( VsTangent, InTangent_ );
#endif
}

#endif

//////////////////////////////////////////////////////////////////////////
// Pixel shader
#if PIXEL_SHADER

PS_IN( vec4, VsColour0 );
PS_IN( vec4, VsNormal );
PS_IN( vec4, VsTexCoord0 );
#if defined( PERM_MESH_STATIC_3D ) || defined( PERM_MESH_SKINNED_3D )
PS_IN( vec4, VsBinormal );
PS_IN( vec4, VsTangent );
#endif
PS_IN( vec4, VsWorldPosition );

//////////////////////////////////////////////////////////////////////////
// Samplers
PSY_SAMPLER_2D( DiffuseTex );
PSY_SAMPLER_2D( NormalTex );
PSY_SAMPLER_2D( SpecularTex );
PSY_SAMPLER_2D( OpacityTex );
PSY_SAMPLER_CUBE( ReflectionTex );

#if defined( SOFT_CLIPPING )
PSY_SAMPLER_2D( DepthTex );
#endif

//////////////////////////////////////////////////////////////////////////
// getMaterial
void getMaterial( out vec4 Diffuse, out vec4 Normal, out vec4 Specular )
{
#if defined( PERM_MESH_STATIC_3D ) || defined( PERM_MESH_SKINNED_3D )
	vec3 InNormal = normalize( VsNormal.xyz );
	vec3 InTangent = normalize( VsTangent.xyz );
	vec3 InBinormal = normalize( VsBinormal.xyz );
	mat3 TBN = mat3( InTangent, InBinormal, InNormal );
#else
	mat3 TBN = mat3( 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 );
#endif

	Normal = PSY_SAMPLE_2D( NormalTex, VsTexCoord0.xy );
	Normal.xyz = Normal.xyz * 2.0 - 1.0;
	Normal.xyz = mul( TBN, Normal.xyz );
	Normal.xyz = normalize( Normal.xyz );

	Diffuse = PSY_SAMPLE_2D( DiffuseTex, VsTexCoord0.xy );
	Specular = PSY_SAMPLE_2D( SpecularTex, VsTexCoord0.xy );
	Specular = vec4( Specular.x + Specular.y + Specular.z ) / 3.0;
}

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelAll( FRAMEBUFFER_INPUT )
{
	vec4 Diffuse;
	vec4 Normal;
	vec4 Specular;
	getMaterial( Diffuse, Normal, Specular );

	Material InMaterial;
	InMaterial.Metallic_ = MaterialMetallic_;
	InMaterial.Specular_ = MaterialSpecular_ * Specular.x;
	InMaterial.Roughness_ = MaterialRoughness_;

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

	InMaterial.Colour_ = MaterialBaseColour_.xyz * Diffuse.xyz;

	vec3 ReflectionColour = textureLod( aReflectionTex, reflect( normalize( VsWorldPosition.xyz - EyePosition ), Normal.xyz ), 8.0 ).xyz;

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


	writeFrag( FRAMEBUFFER_INTERNAL, vec4( TotalSurface, Diffuse.w ), Normal.xyz, vec3( InMaterial.Metallic_, InMaterial.Specular_, InMaterial.Roughness_ ) );
#else
	writeFrag( FRAMEBUFFER_INTERNAL, Diffuse * VsColour0, Normal.xyz, vec3( InMaterial.Metallic_, InMaterial.Specular_, InMaterial.Roughness_ ) );
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
