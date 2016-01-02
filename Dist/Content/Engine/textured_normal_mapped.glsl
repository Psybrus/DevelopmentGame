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

void vertexMain()
{
 	vec4 WorldPosition;
	PSY_MAKE_WORLD_SPACE_VERTEX( WorldPosition, InPosition_ );
	PSY_MAKE_CLIP_SPACE_VERTEX( gl_Position, WorldPosition );
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

//////////////////////////////////////////////////////////////////////////
// Samplers
PSY_SAMPLER_2D( DiffuseTex );
PSY_SAMPLER_2D( NormalTex );
PSY_SAMPLER_2D( SpecularTex );
PSY_SAMPLER_2D( OpacityTex );

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
	Normal.xyz = Normal.xyz * 2.0f - 1.0f;
	Normal.xyz = mul( TBN, Normal.xyz );

	Diffuse = PSY_SAMPLE_2D( DiffuseTex, VsTexCoord0.xy );
	Specular = PSY_SAMPLE_2D( SpecularTex, VsTexCoord0.xy );
}

//////////////////////////////////////////////////////////////////////////
// pixelMain
void pixelMain()
{
	vec4 Diffuse;
	vec4 Normal;
	vec4 Specular;
	getMaterial( Diffuse, Normal, Specular );
	Diffuse *= VsColour0;

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

	writeFrag( fragColour, Diffuse, Normal.xyz, Specular.xyz );
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
	vec4 Diffuse;
	vec4 Normal;
	vec4 Specular;
	getMaterial( Diffuse, Normal, Specular );
	Diffuse *= VsColour0;

	writeFrag( fragColour, Diffuse * VsColour0, Normal.xyz, Specular.xyz );
}

#endif
