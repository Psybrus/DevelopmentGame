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
#include "GaTestModelComponent.h"
#include "GaTestParticleComponent.h"
#include "GaTestShaderComponent.h"
#include "GaTestSelectionComponent.h"

#include "System/Content/CsCore.h"
#include "System/Os/OsCore.h"
#include "System/Scene/ScnCore.h"

#include "Math/MaVec2d.h"
#include "Math/MaVec4d.h"

#include <memory>
#include <type_traits>

//////////////////////////////////////////////////////////////////////////
// GPsySetupParams
PsySetupParams GPsySetupParams( "Development Game", psySF_GAME_DEV, 1.0f / 60.0f );	

// Simple command allocator with naive management.
// Commands offer no way to deallocate, so it's down
// to the user to free the memory allocated when the 
// command is out of scope. This is an intentional choice.
struct CommandAllocatorSimple
{
	void* allocate( size_t Size )
	{
		auto Mem = new char[ Size ];
		Allocated_.emplace_back( Mem );
		return Mem;
	}

	void clear()
	{
		Allocated_.clear();
	}

	std::vector< std::unique_ptr< char[] > > Allocated_;
};

//////////////////////////////////////////////////////////////////////////
// PsyGameInit
namespace
{
	struct NullLambdaCapture {};
	typedef bool(NullLambdaCapture::*LambdaSignature)( int );

	struct TestCommand
	{
		template< typename _LambdaType, typename _AllocPolicy >
		static TestCommand* Create( _LambdaType& Lambda, _AllocPolicy& AllocPolicy )
		{
			
		}

		template< typename _LambdaType, typename _AllocPolicy >
		TestCommand( _LambdaType& Lambda, _AllocPolicy& AllocPolicy )
		{
			// Grab lambda call func.
			auto LambdaCallFunc = &_LambdaType::operator();
			LambdaPtr_ = reinterpret_cast< LambdaSignature >( LambdaCallFunc );

			// Copy capture.
			static_assert( std::is_trivially_copyable< _LambdaType >::value, "Must be trivially copyable" );
			static_assert( std::is_trivially_destructible< _LambdaType >::value, "Must be trivially destructible" );
			auto LambdaSize = sizeof( _LambdaType );
			auto* CaptureData = reinterpret_cast< NullLambdaCapture* >( &Lambda );
			LambdaCapture_ = AllocPolicy.allocate( LambdaSize );
			memcpy( LambdaCapture_, &Lambda, LambdaSize );

			PSY_LOG( "Size: %u", LambdaSize );
		}

		bool operator() ( int ParamA )
		{
			auto LambdaCapture = reinterpret_cast< NullLambdaCapture* >( LambdaCapture_ );
			return (LambdaCapture->*LambdaPtr_)( ParamA );
		}

		LambdaSignature LambdaPtr_;
		void* LambdaCapture_;
	};
}

void PsyGameInit()
{
	int A = 12345678;
	std::string B = "Blegh";

	auto TestLambdaNoCapture = []( int ParamA )->bool
		{
			PSY_LOG( "TestLambdaNoCapture call: %u", ParamA );
			return true;
		};

	auto TestLambdaTrivialCapture = [ A ]( int ParamA )->bool
		{
			PSY_LOG( "TestLambdaTrivialCapture call: %u, %u", ParamA, A );
			return true;
		};

	auto TestLambdaNonTrivialCapture = [ B ]( int ParamA )->bool
		{
			PSY_LOG( "TestLambdaNonTrivialCapture call: %u, %s", ParamA, B.c_str() );
			return true;
		};

	PSY_LOG( "TestLambdaNoCapture, is_trivially_copyable: %u", 
		std::is_trivially_copyable< decltype( TestLambdaNoCapture ) >::value ? 1 : 0 );
	PSY_LOG( "TestLambdaTrivialCapture, is_trivially_copyable: %u", 
		std::is_trivially_copyable< decltype( TestLambdaTrivialCapture ) >::value ? 1 : 0 );
	PSY_LOG( "TestLambdaNonTrivialCapture, is_trivially_copyable: %u", 
		std::is_trivially_copyable< decltype( TestLambdaNonTrivialCapture ) >::value ? 1 : 0 );

	PSY_LOG( "TestLambdaNoCapture, is_trivially_destructible: %u", 
		std::is_trivially_destructible< decltype( TestLambdaNoCapture ) >::value ? 1 : 0 );
	PSY_LOG( "TestLambdaTrivialCapture, is_trivially_destructible: %u", 
		std::is_trivially_destructible< decltype( TestLambdaTrivialCapture ) >::value ? 1 : 0 );
	PSY_LOG( "TestLambdaNonTrivialCapture, is_trivially_destructible: %u", 
		std::is_trivially_destructible< decltype( TestLambdaNonTrivialCapture ) >::value ? 1 : 0 );

	CommandAllocatorSimple Allocator;
	TestCommand CommandNoCapture( TestLambdaNoCapture, Allocator );
	TestCommand CommandTrivialCapture( TestLambdaTrivialCapture, Allocator );
	//TestCommand CommandNonTrivialCapture( TestLambdaNonTrivialCapture );

	CommandNoCapture( 1337 );
	CommandTrivialCapture( 1337 );
	//CommandNonTrivialCapture( 1337 );

	//exit(0);
}

//////////////////////////////////////////////////////////////////////////
// PsyLaunchGame
void PsyLaunchGame()
{
	ScnEntitySpawnParams ScreenEntityParams = 
	{
		"MenuEntity_0", "default", "MenuEntity",
		MaMat4d(),
		nullptr
	};

	ScnCore::pImpl()->spawnEntity( ScreenEntityParams );

	ScnEntitySpawnParams CameraEntityParams = 
	{
		"CameraEntity_0", "default", "CameraEntity",
		MaMat4d(),
		nullptr
	};

	ScnCore::pImpl()->spawnEntity( CameraEntityParams );
}
