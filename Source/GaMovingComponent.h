/**************************************************************************
*
* File:		GaMovingComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaMovingComponent_H__
#define __GaMovingComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

//////////////////////////////////////////////////////////////////////////
// GaMovingComponentRef
typedef ReObjectRef< class GaMovingComponent > GaMovingComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaMovingComponent
class GaMovingComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaMovingComponent, ScnComponent );

	GaMovingComponent();
	virtual ~GaMovingComponent();
	
	void update( BcF32 Tick );
	
	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
	MaAABB getAABB() const;

private:
	MaVec3d Position_;
	MaVec3d AxisSpeed_;
	MaVec3d AxisMagnitude_;
	BcF32 Timer_;
};

#endif

