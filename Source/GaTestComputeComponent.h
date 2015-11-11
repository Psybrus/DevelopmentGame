/**************************************************************************
*
* File:		GaTestComputeComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestComputeComponent_H__
#define __GaTestComputeComponent_H__

#include "Psybrus.h"
#include "System/Renderer/RsUniquePointers.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestComputeComponent > GaTestComputeComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestComputeComponent
class GaTestComputeComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestComputeComponent, ScnRenderableComponent );

	GaTestComputeComponent();
	virtual ~GaTestComputeComponent();
	
	virtual void render( ScnRenderContext & RenderContext );

	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB getAABB() const;

private:
	RsBufferUPtr ObjectUniformBuffer_;
	RsBufferUPtr TestUniformBuffer_;
	RsBufferUPtr IndexBuffer_;
	RsBufferUPtr VertexBuffer_;
	RsVertexDeclarationUPtr VertexDeclaration_;
	RsGeometryBindingUPtr GeometryBinding_;

	ScnShaderRef ComputeShader_;
	RsBufferUPtr ComputeOutputBuffer_;
	std::array< class ScnTexture*, 2 > ComputeOutputTextures_;
	BcU32 ComputeTextureIdx_ = 0;
	std::array< RsProgramBindingUPtr, 2 > ComputeProgramBindings_;

	ScnMaterialRef Material_;
	ScnMaterialComponentRef MaterialComponent_;
};

#endif

