#include <Psybrus.psh>

////////////////////////////////////////////////////////////////////////
// VS_INPUT
#define VS_INPUT		VertexDefault

////////////////////////////////////////////////////////////////////////
// VS_OUTPUT
struct VS_OUTPUT
{
	float4 Position_	: SV_POSITION;
	float4 Colour_		: COLOR0;
	float4 TexCoord0_	: TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////
// PS_OUTPUT
struct PS_OUTPUT
{
	float4 Colour_		: SV_TARGET;
};

////////////////////////////////////////////////////////////////////////
// vertexMain
VS_OUTPUT vertexMain( VS_INPUT i )
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	float4 WorldPosition;
	PSY_MAKE_WORLD_SPACE_VERTEX( WorldPosition, i.Position_ );
	PSY_MAKE_CLIP_SPACE_VERTEX( o.Position_, WorldPosition );
	o.Colour_ = i.Colour_;
	o.TexCoord0_ = i.TexCoord0_;
	return o;
}

//////////////////////////////////////////////////////////////////////////
// Samplers
PSY_SAMPLER_2D( DiffuseTex );
PSY_SAMPLER_2D( OpacityTex );

////////////////////////////////////////////////////////////////////////
// pixelMain
PS_OUTPUT pixelMain( VS_OUTPUT i )
{
	PS_OUTPUT o = (PS_OUTPUT)0;
	float4 Colour = PSY_SAMPLE_2D( DiffuseTex, i.TexCoord0_.xy );
	o.Colour_ = Colour * i.Colour_;
	return o;
}

////////////////////////////////////////////////////////////////////////
// pixelMainMask
PS_OUTPUT pixelMainMask( VS_OUTPUT i )
{
	PS_OUTPUT o = (PS_OUTPUT)0;
	float4 Opacity = PSY_SAMPLE_2D( OpacityTex, i.TexCoord0_.xy );
	clip( Opacity.r < 0.1 ? -1.0 : 1.0);
	float4 Colour = PSY_SAMPLE_2D( DiffuseTex, i.TexCoord0_.xy );
	o.Colour_ = Colour * i.Colour_;
	return o;
}

