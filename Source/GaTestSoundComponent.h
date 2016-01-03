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
	
	void update( BcF32 Tick );
	
	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
	MaAABB getAABB() const override;

private:
	ScnSoundRef Sound_;
	ScnSoundRef Music_;
	ScnSoundEmitterComponentRef SoundEmitter_;
	ScnSoundEmitterComponentRef MusicEmitter_;
	
	std::vector< BcF32 > VisFFT_;
	std::vector< BcF32 > VisWave_;
};

#endif

