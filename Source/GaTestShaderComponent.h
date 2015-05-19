/**************************************************************************
*
* File:		GaTestShaderComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestShaderComponent_H__
#define __GaTestShaderComponent_H__

#include "Psybrus.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestShaderComponent > GaTestShaderComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestShaderComponent
class GaTestShaderComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestShaderComponent, ScnRenderableComponent );

	GaTestShaderComponent();
	virtual ~GaTestShaderComponent();
	
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

	ScnMaterialRef Material_;
	ScnMaterialComponentRef MaterialComponent_;
};

#endif

