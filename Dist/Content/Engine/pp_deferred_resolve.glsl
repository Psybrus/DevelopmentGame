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
	vec4 Albedo  = gammaToLinear( PSY_SAMPLE_2D( AlbedoTex, VsTexCoord0.xy ) );
	vec4 Specular  = gammaToLinear( PSY_SAMPLE_2D( MaterialTex, VsTexCoord0.xy ) );
	vec4 Normal  = normalize( PSY_SAMPLE_2D( NormalTex, VsTexCoord0.xy ) * 2.0 - 1.0 );
	vec4 Velocity  = PSY_SAMPLE_2D( VelocityTex, VsTexCoord0.xy );
	vec4 Depth  = PSY_SAMPLE_2D( DepthTex, VsTexCoord0.xy );

#if 0
	vec3 DiffuseLight;
	vec3 SpecularLight;
	defaultLighting( Normal.xyz, DiffuseLight, SpecularLight );
#else
	vec4 SSPosition = vec4( VsTexCoord0.xy * 2.0 - 1.0, Depth.x * 2.0 - 1.0, 1.0 );
	SSPosition = mul( InverseClipTransform_, SSPosition );
	vec3 WorldPosition = SSPosition.xyz / SSPosition.w;
	vec4 EyePosition = InverseViewTransform_[3];
	vec3 DiffuseLight = vec3( 0.0 );
	vec3 SpecularLight = vec3( 0.0 );
	defaultLighting( 0, EyePosition.xyz, WorldPosition.xyz, Normal.xyz, DiffuseLight, SpecularLight );
#endif

	outputFrag[0] = vec4( ( Albedo.xyz * DiffuseLight ) + ( Specular.xyz * SpecularLight ), 1.0 );
}

#endif

