/**************************************************************************
*
* File:		GaTestInstancingComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestInstancingComponent_H__
#define __GaTestInstancingComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

#include "System/Scene/Rendering/ScnModel.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestInstancingComponent > GaTestInstancingComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestInstancingComponent
class GaTestInstancingComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestInstancingComponent, ScnComponent );

	GaTestInstancingComponent();
	virtual ~GaTestInstancingComponent();
	
	void update( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
private:
	std::vector< ScnModelRef > Models_;
	std::vector< MaVec3d > Scales_;
};

#endif

