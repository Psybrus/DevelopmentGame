/**************************************************************************
*
* File:		GaTestLightingComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestLightingComponent_H__
#define __GaTestLightingComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

#include "System/Scene/Rendering/ScnModel.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestLightingComponent > GaTestLightingComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestLightingComponent
class GaTestLightingComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestLightingComponent, ScnComponent );

	GaTestLightingComponent();
	virtual ~GaTestLightingComponent();
	
	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
private:
	ScnModelRef Model_;
};

#endif

