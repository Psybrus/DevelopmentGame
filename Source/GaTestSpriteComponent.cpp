/**************************************************************************
*
* File:		GaTestSpriteComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestSpriteComponent.h"

#include "System/Scene/Rendering/ScnCanvasComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"

#include "System/SysKernel.h"

#include "Base/BcMath.h"
#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestSpriteComponent );

void GaTestSpriteComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Canvas_", &GaTestSpriteComponent::Canvas_, bcRFF_TRANSIENT ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaTestSpriteComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor(
			{
				ScnComponentProcessFuncEntry(
					"Set canvas projection",
					ScnComponentPriority::CANVAS_CLEAR + 1,
					std::bind( &GaTestSpriteComponent::setCanvasProjection, _1 ) ),
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestSpriteComponent::GaTestSpriteComponent():
	Canvas_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestSpriteComponent::~GaTestSpriteComponent()
{

}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestSpriteComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Canvas_ = getParentEntity()->getComponentAnyParentByType< ScnCanvasComponent >();

	BcU32 Idx = 0;
	BcF32 Width = 0.0f;

	for( BcF32 X = -Width; X <= Width; X += 2.0f )
	{
		ScnEntitySpawnParams EntityParams = 
		{
			BcName( "SpriteEntity", Idx++ ), "sprite_test", "SpriteEntity",
			MaMat4d(),
			getParentEntity()
		};

		EntityParams.Transform_.translation( MaVec3d( 0.0f, 0.0f, X ) );
		ScnCore::pImpl()->spawnEntity( EntityParams );
	}

	LightUniformBuffer_.reset( RsCore::pImpl()->createBuffer(
		RsBufferDesc( 
			RsBufferType::UNIFORM, 
			RsResourceCreationFlags::STREAM, 
			sizeof( LightUniformBlock_ ) ) ) );

	// Setup buffer for lighting.
	auto MaterialComponent = getParentEntity()->getComponentAnyParentByType< ScnMaterialComponent >();
	BcU32 UniformBlock = MaterialComponent->findUniformBlock( "ScnShaderLightUniformBlockData" );
	MaterialComponent->setUniformBlock( UniformBlock, LightUniformBuffer_.get() );

	PSY_LOG( "Spawned %u sprite entities.\n", Idx );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestSpriteComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	LightUniformBuffer_.reset();
}

//////////////////////////////////////////////////////////////////////////
// setCanvasProjection
//static
void GaTestSpriteComponent::setCanvasProjection( const ScnComponentList& Components )
{
	for( auto Component : Components )
	{
		BcAssert( Component->isTypeOf< GaTestSpriteComponent >() );
		auto* TestComponent = static_cast< GaTestSpriteComponent* >( Component.get() );

		OsClient* Client = OsCore::pImpl()->getClient( 0 );

		BcF32 PixelW = static_cast< BcF32 >( Client->getWidth() ) / 3.0f;
		BcF32 PixelH = static_cast< BcF32 >( Client->getHeight() ) / 3.0f;

		BcF32 EdgeL = floorf( -PixelW / 2.0f );
		BcF32 EdgeR = EdgeL + PixelW;
		BcF32 EdgeT = floorf( -PixelH / 2.0f );
		BcF32 EdgeB = EdgeT + PixelH;

		MaMat4d Projection;
		Projection.orthoProjection( 
			EdgeL, 
			EdgeR, 
			EdgeB, 
			EdgeT, 
			-1.0f, 1.0f );
		MaMat4d InvProjection = Projection;
		InvProjection.inverse();

		TestComponent->Canvas_->clear();
		TestComponent->Canvas_->pushMatrix( Projection );

		// Light.
		TestComponent->LightTimer_ += SysKernel::pImpl()->getFrameTime();

		RsLight Light;
		Light.createAttenuationValues( 1.0f, 4.0f, 0.0f );

		memset( &TestComponent->LightUniformBlock_, 0, sizeof( TestComponent->LightUniformBlock_ ) );
		TestComponent->LightUniformBlock_.LightPosition_[ 0 ] = 
			( MaVec4d( 
				BcCos( TestComponent->LightTimer_ ), 
				-BcSin( TestComponent->LightTimer_ ), 
				0.0f, 
				0.0f ) * 32.0f ) * Projection;
		TestComponent->LightUniformBlock_.LightAmbientColour_[ 0 ] = RsColour( 0.5f, 0.5f, 0.5f, 1.0f );
		TestComponent->LightUniformBlock_.LightDiffuseColour_[ 0 ] = RsColour( 2.0f, 0.0f, 0.0f, 1.0f );
		TestComponent->LightUniformBlock_.LightAttn_[ 0 ] = MaVec4d( Light.AttnC_, Light.AttnL_, Light.AttnQ_, 0.0 );


		RsCore::pImpl()->updateBuffer(
			TestComponent->LightUniformBuffer_.get(),
			0, sizeof( TestComponent->LightUniformBlock_ ),
			RsResourceUpdateFlags::ASYNC,
			[ TestComponent ]( RsBuffer* Buffer, const RsBufferLock& Lock )
			{
				memcpy( Lock.Buffer_, &TestComponent->LightUniformBlock_, 
					sizeof( TestComponent->LightUniformBlock_ ) );
			} );

		auto MaterialComponent = TestComponent->getComponentAnyParentByType< ScnMaterialComponent >( "debug" );
		auto LightPos = TestComponent->LightUniformBlock_.LightPosition_[ 0 ] * InvProjection;
		auto LightCol = TestComponent->LightUniformBlock_.LightDiffuseColour_[ 0 ];
		/*
		TestComponent->Canvas_->setMaterialComponent( MaterialComponent );
		TestComponent->Canvas_->drawSprite( 
			LightPos.xy(), 
			MaVec2d( 8.0f, 8.0f ), 0,
			RsColour::WHITE, 10 );
		*/
	}
}
