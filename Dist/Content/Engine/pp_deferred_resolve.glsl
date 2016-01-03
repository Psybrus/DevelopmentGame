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
	vec4 Albedo  = PSY_SAMPLE_2D( AlbedoTex, VsTexCoord0.xy );
	vec4 Material  = PSY_SAMPLE_2D( MaterialTex, VsTexCoord0.xy );
	vec4 Normal  = PSY_SAMPLE_2D( NormalTex, VsTexCoord0.xy );
	vec4 Velocity  = PSY_SAMPLE_2D( DepthTex, VsTexCoord0.xy );


	// Hacky lighting.
	vec3 CameraVector = mul( ViewTransform_, vec4( 0.0, 0.0, 1.0, 0.0 ) ).xyz;
	vec3 LightVector = -normalize( vec3( cos( ViewTime_.x ), -1.0, sin( ViewTime_.x ) ) );
	vec3 HalfVector = normalize( LightVector + CameraVector );
	float NdotL = dot( Normal.xyz, LightVector );
	float NdotH = dot( Normal.xyz, HalfVector );
	
	// Ambient + Diffuse
	float AmbientLight = 0.2;
	float DiffuseLight = max( NdotL, 0.0 );
	float TotalLight = min( AmbientLight + DiffuseLight, 1.0 );

	float Facing = NdotL > 0.0 ? 1.0 : 0.0;
	float Specular = ( Facing * pow( max( NdotH, 0.0 ), 1.0 ) ) * 0.2;

	outputFrag[0] = gammaToLinear( vec4( ( Albedo.xyz * ( AmbientLight + DiffuseLight ) ) + ( Material.xyz * Specular ), 1.0 ) );
}

#endif

