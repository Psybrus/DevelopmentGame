/**************************************************************************
*
* File:		GaTestCloudComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestCloudComponent_H__
#define __GaTestCloudComponent_H__

#include "Psybrus.h"
#include "System/Renderer/RsUniquePointers.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestCloudComponent > GaTestCloudComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestCloudBlockData
struct GaTestCloudBlockData
{
	REFLECTION_DECLARE_BASIC( GaTestCloudBlockData );
	GaTestCloudBlockData(){};

	/// Used to animate clouds.
	MaVec4d CloudTimer_;

	/// Scale at which to render clouds.
	MaVec4d CloudScale_;

	/// Threshold for hard edge of clouds.
	BcF32 CloudThreshold_;

	// Padding for 16 byte alignment.
	BcF32 CloudPadding0_;
	BcF32 CloudPadding1_;
	BcF32 CloudPadding2_;
};

//////////////////////////////////////////////////////////////////////////
// GaTestCloudComponent
class GaTestCloudComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestCloudComponent, ScnRenderableComponent );

	GaTestCloudComponent();
	virtual ~GaTestCloudComponent();

	void drawTest( 
		const MaMat4d& Transform, ScnMaterialComponent* Material,
		ScnRenderContext & RenderContext );
	
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

	GaTestCloudBlockData TestUniformBlock_;

	ScnMaterialRef Material3D_;
	ScnMaterialComponentRef MaterialComponent3D_;
	ScnTexture* Texture3D_;
};

#endif

