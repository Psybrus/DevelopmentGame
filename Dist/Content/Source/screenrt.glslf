varying vec4 vTexCoord0;

uniform sampler2D aDiffuseTex;	

vec4 shaderMain()
{
	vec4 Colour = texture2D( aDiffuseTex, vTexCoord0.xy );
	return vec4( Colour.xyz * vColour.xyz, 1.0 );
}
