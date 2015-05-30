/**************************************************************************
*
* File:		GaTestSpriteComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestSpriteComponent_H__
#define __GaTestSpriteComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestSpriteComponent > GaTestSpriteComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestSpriteComponent
class GaTestSpriteComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestSpriteComponent, ScnComponent );

	GaTestSpriteComponent();
	virtual ~GaTestSpriteComponent();
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
private:
	static void setCanvasProjection( const ScnComponentList& Components );

private:
	class ScnCanvasComponent* Canvas_;

	BcF32 LightTimer_;

	ScnShaderLightUniformBlockData LightUniformBlock_;
	RsBufferUPtr LightUniformBuffer_;
};

#endif

