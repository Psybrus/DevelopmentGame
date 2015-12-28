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

#include "System/Scene/ScnCore.h"

//////////////////////////////////////////////////////////////////////////
// PsyGameInit
void PsyGameInit()
{

}

//////////////////////////////////////////////////////////////////////////
// PsyLaunchGame
void PsyLaunchGame()
{
#if 1
	ScnEntitySpawnParams CameraEntityParams = 
	{
		"CameraEntity_0", "default", "CameraEntity",
		MaMat4d(),
		nullptr
	};

	ScnCore::pImpl()->spawnEntity( CameraEntityParams );

	ScnEntitySpawnParams ScreenEntityParams = 
	{
		"MenuEntity_0", "default", "MenuEntity",
		MaMat4d(),
		nullptr
	};

	ScnCore::pImpl()->spawnEntity( ScreenEntityParams );
#endif
}
