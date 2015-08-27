#ifndef __GaTrailComponent_H__
#define __GaTrailComponent_H__

#include "Psybrus.h"

#include "System/Scene/Rendering/ScnRenderableComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaTrailComponentRef
typedef ReObjectRef< class GaTrailComponent > GaTrailComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTrailVertex
struct GaTrailVertex
{
	/// Centre position of trail.
	MaVec4d Position_;

	/// Normal of trail.
	MaVec4d Normal_;

	/// xy = Texcoord, z = Offset, w = Width
	MaVec4d TexCoord_; 

	/// Colour.
	RsColour Colour_;
};

//////////////////////////////////////////////////////////////////////////
// GaTrailComponent
class GaTrailComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTrailComponent, ScnRenderableComponent );

	GaTrailComponent();
	virtual ~GaTrailComponent();
	
	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	MaAABB getAABB() const override;
	void render( ScnRenderContext & RenderContext ) override;
	
	static void updateTrails( const ScnComponentList& Components );

private:
	/// Material to render with.
	class ScnMaterial* Material_;
	/// Width of trail in world units.
	BcF32 Width_;
	/// Max length of trail in world units.
	BcF32 MaxLength_;
	/// Distance to start new segment.
	BcF32 SegmentDistance_;

	/// 
	BcF32 DistanceMoved_;
	size_t MaxTrailHistory_;
	std::vector< MaMat4d > TrailHistory_;
	class ScnMaterialComponent* MaterialComponent_;

	RsVertexDeclarationUPtr VertexDeclaration_;
	RsBufferUPtr VertexBuffer_;
	RsBufferUPtr UniformBuffer_;
	ScnShaderObjectUniformBlockData	ObjectUniforms_;


	static SysFence UpdateFence_;
};

#endif

