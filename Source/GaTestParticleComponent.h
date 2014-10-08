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
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestParticleComponent > GaTestParticleComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestParticleComponent
class GaTestParticleComponent:
	public ScnRenderableComponent
{
public:
	DECLARE_RESOURCE( GaTestParticleComponent, ScnRenderableComponent );

	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB getAABB() const;

private:
	ScnParticleSystemComponentRef ParticleSystem_;

	ScnSoundRef Sound_;
	ScnSoundEmitterComponentRef SoundEmitter_;
	
};

#endif

