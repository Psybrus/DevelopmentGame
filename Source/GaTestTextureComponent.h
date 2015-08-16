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
	
	virtual void render( ScnRenderContext & RenderContext );

	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB getAABB() const;

private:
	class RsBuffer* ObjectUniformBuffer_;
	class RsBuffer* TestUniformBuffer_;
	class RsBuffer* IndexBuffer_;
	class RsBuffer* VertexBuffer_;
	class RsVertexDeclaration* VertexDeclaration_;

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

