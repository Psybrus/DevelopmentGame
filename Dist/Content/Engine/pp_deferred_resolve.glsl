#include <Psybrus.glsl>

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

PSY_SAMPLER_2D( AlbedoTex );
PSY_SAMPLER_2D( MaterialTex );
PSY_SAMPLER_2D( NormalTex );
PSY_SAMPLER_2D( VelocityTex );
PSY_SAMPLER_2D( DepthTex );

void pixelMain()
{
	vec4 GBufferAlbedo  = gammaToLinear( PSY_SAMPLE_2D( AlbedoTex, VsTexCoord0.xy ) );

	// x - Metallic
	// y - Specular
	// z - Roughness
	vec4 GBufferMaterial  = PSY_SAMPLE_2D( MaterialTex, VsTexCoord0.xy );

	vec3 GBufferNormal  = normalize( PSY_SAMPLE_2D( NormalTex, VsTexCoord0.xy ).xyz * 2.0 - 1.0 );
	vec4 Velocity  = PSY_SAMPLE_2D( VelocityTex, VsTexCoord0.xy );
	vec4 Depth  = PSY_SAMPLE_2D( DepthTex, VsTexCoord0.xy );

	vec4 SSPosition = vec4( VsTexCoord0.xy * 2.0 - 1.0, Depth.x * 2.0 - 1.0, 1.0 );
	SSPosition = mul( InverseClipTransform_, SSPosition );
	vec3 WorldPosition = SSPosition.xyz / SSPosition.w;
	vec3 EyePosition = InverseViewTransform_[3].xyz;

	Material InMaterial;
	InMaterial.Colour_ = MaterialBaseColour_.xyz * GBufferAlbedo.xyz;
	InMaterial.Metallic_ = GBufferMaterial.x;
	InMaterial.Specular_ = GBufferMaterial.y;
	InMaterial.Roughness_ = GBufferMaterial.z;

	Light InLight;
	InLight.Position_ = LightPosition_[ 0 ].xyz;
	InLight.Colour_ = LightDiffuseColour_[ 0 ].xyz;
	InLight.AttenuationCLQ_ = LightAttn_[ 0 ].xyz;
	vec3 TotalSurface = BRDF_Default( InLight, InMaterial, EyePosition, WorldPosition.xyz, GBufferNormal.xyz );

	outputFrag[0] = vec4( TotalSurface, 1.0 );
}

#endif

