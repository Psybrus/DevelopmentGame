/**************************************************************************
*
* File:		GaAnimationControllerComponent.h
* Author:	Neil Richardson 
* Ver/Date:	15/12/12	
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#ifndef __GaAnimationControllerComponent_H__
#define __GaAnimationControllerComponent_H__

#include "Psybrus.h"

#include "System/Scene/Animation/ScnAnimation.h"
#include "System/Scene/Animation/ScnAnimationComponent.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaAnimationControllerComponent > GaAnimationControllerComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaAnimationControllerComponent
class GaAnimationControllerComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaAnimationControllerComponent, ScnComponent );

	GaAnimationControllerComponent();
	virtual ~GaAnimationControllerComponent();
	
	virtual void update( BcF32 Tick );

	virtual void onAttach( ScnEntityWeakRef Parent );
	
private:
	ScnAnimationRef AnimIdle_;
	ScnAnimationRef AnimFire_;
	ScnAnimationRef AnimReload_;
	ReObjectRef< class ScnAnimationTreeBlendNode > pRootTrack_;
	ReObjectRef< class ScnAnimationTreeTrackNode > pIdleTrack_;
	ReObjectRef< class ScnAnimationTreeTrackNode > pReloadTrack_;
};

#endif

