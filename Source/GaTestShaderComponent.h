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
#include "System/Scene/ScnRenderableComponent.h"

#include "System/Scene/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestShaderComponent > GaTestShaderComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestShaderComponent
class GaTestShaderComponent:
	public ScnRenderableComponent
{
public:
	DECLARE_RESOURCE( ScnRenderableComponent, GaTestShaderComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );
	virtual void						render( class ScnViewComponent* pViewComponent, RsFrame* pFrame, RsRenderSort Sort );

	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB						getAABB() const;

private:
	class RsUniformBuffer*				ObjectUniformBuffer_;
	class RsUniformBuffer*				TestUniformBuffer_;
	class RsIndexBuffer*				IndexBuffer_;
	class RsVertexBuffer*				VertexBuffer_;
	class RsVertexDeclaration*			VertexDeclaration_;
	class RsPrimitive*					Primitive_;

	ScnMaterialRef						Material_;
	ScnMaterialComponentRef				MaterialComponent_;
};

#endif

