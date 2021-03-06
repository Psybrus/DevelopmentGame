/**************************************************************************
*
* File:		GaTestModelComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestModelComponent_H__
#define __GaTestModelComponent_H__

#include "Psybrus.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestModelComponent > GaTestModelComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestModelComponent
class GaTestModelComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestModelComponent, ScnRenderableComponent );

	GaTestModelComponent();
	virtual ~GaTestModelComponent();
	
	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB getAABB() const;

private:
	ScnMaterialRef Material_;
	ScnMaterialComponentRef MaterialComponent_;
};

#endif

