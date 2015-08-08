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
// GaTestTextureComponent
class GaTestTextureComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestTextureComponent, ScnRenderableComponent );

	GaTestTextureComponent();
	virtual ~GaTestTextureComponent();
	
	virtual void render( class ScnViewComponent* pViewComponent, RsFrame* pFrame, RsRenderSort Sort );

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
	ScnMaterialComponentRef MaterialComponent1D_;
	ScnMaterialComponentRef MaterialComponent2D_;
	ScnMaterialComponentRef MaterialComponent3D_;

	ScnTexture* Texture1D_;
	ScnTexture* Texture2D_;
	ScnTexture* Texture3D_;
};

#endif

