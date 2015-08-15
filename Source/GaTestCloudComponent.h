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

	MaVec4d CloudTimer_;
	MaVec4d CloudScale_;
	BcF32 CloudThreshold_;
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
		class ScnViewComponent* pViewComponent, RsFrame* pFrame, 
		RsRenderSort Sort );
	
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

	GaTestCloudBlockData TestUniformBlock_;

	ScnMaterialRef Material3D_;
	ScnMaterialComponentRef MaterialComponent3D_;
	ScnTexture* Texture3D_;
};

#endif

