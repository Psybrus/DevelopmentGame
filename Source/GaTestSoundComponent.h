/**************************************************************************
*
* File:		GaTestSoundComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestSoundComponent_H__
#define __GaTestSoundComponent_H__

#include "Psybrus.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"

#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestSoundComponent > GaTestSoundComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestSoundComponent
class GaTestSoundComponent:
	public ScnRenderableComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestSoundComponent, ScnRenderableComponent );

	GaTestSoundComponent();
	virtual ~GaTestSoundComponent();
	
	virtual void update( BcF32 Tick );
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	virtual MaAABB getAABB() const;

private:
	ScnSoundRef Sound_;
	ScnSoundEmitterComponentRef SoundEmitter_;
	
	std::vector< BcF32 > VisFFT_;
	std::vector< BcF32 > VisWave_;
};

#endif

