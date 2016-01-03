/**************************************************************************
*
* File:		GaTestTextureComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestTextureComponent_H__
#define __GaTestTextureComponent_H__

#include "Psybrus.h"
#include "System/Renderer/RsUniquePointers.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestTextureComponent > GaTestTextureComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestTextureBlockData
struct GaTestTextureBlockData
{
	REFLECTION_DECLARE_BASIC( GaTestTextureBlockData );
	GaTestTextureBlockData(){};

	MaVec4d UVWOffset_;
};

//////////////////////////////////////////////////////////////////////////
// GaTestTextureComponent
class GaTestTextureComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestTextureComponent, ScnRenderableComponent );

	GaTestTextureComponent();
	virtual ~GaTestTextureComponent();

	void drawTest( 
		const MaMat4d& Transform, ScnMaterialComponent* Material,
		ScnRenderContext & RenderContext );
	
	void render( ScnRenderContext & RenderContext ) override;

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
	MaAABB getAABB() const override;

private:
	RsVertexDeclarationUPtr VertexDeclaration_;
	RsBufferUPtr IndexBuffer_;
	RsBufferUPtr VertexBuffer_;
	RsGeometryBindingUPtr GeometryBinding_;

	RsBufferUPtr ObjectUniformBuffer_;
	RsBufferUPtr TestUniformBuffer_;

	ScnMaterialRef Material1D_;
	ScnMaterialRef Material2D_;
	ScnMaterialRef Material3D_;
	ScnMaterialRef MaterialCube_;
	ScnMaterialComponentRef MaterialComponent1D_;
	ScnMaterialComponentRef MaterialComponent2D_;
	ScnMaterialComponentRef MaterialComponent3D_;
	ScnMaterialComponentRef MaterialComponentCube_;

	ScnTexture* Texture1D_;
	ScnTexture* Texture2D_;
	ScnTexture* Texture3D_;
	ScnTexture* TextureCube_;
};

#endif

