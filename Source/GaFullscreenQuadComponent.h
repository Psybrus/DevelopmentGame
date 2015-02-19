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
	DECLARE_RESOURCE( GaFullscreenQuadComponent, ScnRenderableComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	virtual void						render( class ScnViewComponent* pViewComponent, RsFrame* pFrame, RsRenderSort Sort );

	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB						getAABB() const;

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

