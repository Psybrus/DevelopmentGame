/**************************************************************************
*
* File:		GaTestParticleComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestParticleComponent_H__
#define __GaTestParticleComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestParticleComponent > GaTestParticleComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestParticleComponent
class GaTestParticleComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestParticleComponent, ScnComponent );

	GaTestParticleComponent();
	virtual ~GaTestParticleComponent();
	
	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
private:
	ScnParticleSystemComponentRef ParticleSystem_;
};

#endif

