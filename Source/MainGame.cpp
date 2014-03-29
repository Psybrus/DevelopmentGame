/**************************************************************************
*
* File:		MainGame.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		Main Game Entrypoint.
*		
*
*
* 
**************************************************************************/

#include "Psybrus.h"

#include "GaCameraComponent.h"
#include "GaAnimationControllerComponent.h"
#include "GaLevelComponent.h"

//////////////////////////////////////////////////////////////////////////
// GPsySetupParams
PsySetupParams GPsySetupParams( "Development Game", psySF_GAME_DEV, 1.0f / 60.0f );	

//////////////////////////////////////////////////////////////////////////
// PsyGameInit
void PsyGameInit()
{

}

//////////////////////////////////////////////////////////////////////////
// PsyLaunchGame
void PsyLaunchGame()
{
	ScnEntitySpawnParams ScreenEntityParams = 
	{
		"default", "LevelEntity", "LevelEntity_0",
		BcMat4d(),
		NULL
	};

	ScnCore::pImpl()->spawnEntity( ScreenEntityParams );

	BcVec3d A( 1.0f, 2.0f, 845.0f );
	BcVec3d B( 1.0f, -2.0f, 123.0f );

	BcAssert( BcAbs( -A.dot( B ) - B.dot( -A ) ) < 0.00001f );
}

//////////////////////////////////////////////////////////////////////////
// PsyGameRegisterResources
void PsyGameRegisterResources()
{
	CsCore::pImpl()->registerResource< GaCameraComponent >();
	CsCore::pImpl()->registerResource< GaAnimationControllerComponent >();
	CsCore::pImpl()->registerResource< GaLevelComponent >();
}

//////////////////////////////////////////////////////////////////////////
// PsyGameUnRegisterResources
void PsyGameUnRegisterResources()
{
	CsCore::pImpl()->unregisterResource< GaCameraComponent >();
	CsCore::pImpl()->unregisterResource< GaAnimationControllerComponent >();
	CsCore::pImpl()->unregisterResource< GaLevelComponent >();
}
