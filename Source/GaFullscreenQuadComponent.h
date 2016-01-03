/**************************************************************************
*
* File:		GaFullscreenQuadComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaFullscreenQuadComponent_H__
#define __GaFullscreenQuadComponent_H__

#include "Psybrus.h"
#include "System/Renderer/RsUniquePointers.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaFullscreenQuadComponent > GaFullscreenQuadComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaFullscreenQuadComponent
class GaFullscreenQuadComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaFullscreenQuadComponent, ScnRenderableComponent );

	GaFullscreenQuadComponent();
	virtual ~GaFullscreenQuadComponent();

	void render( ScnRenderContext & RenderContext ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
	MaAABB getAABB() const override;

private:
	RsBufferUPtr ObjectUniformBuffer_;
	RsBufferUPtr TestUniformBuffer_;
	RsBufferUPtr IndexBuffer_;
	RsBufferUPtr VertexBuffer_;
	RsVertexDeclarationUPtr VertexDeclaration_;
	RsGeometryBindingUPtr GeometryBinding_;

	ScnMaterialRef Material_;
	ScnMaterialComponentRef MaterialComponent_;
};

#endif

