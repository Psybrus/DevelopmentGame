/**************************************************************************
*
* File:		GaAnimationControllerComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	15/12/12	
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#include "GaAnimationControllerComponent.h"

#include "Base/BcRandom.h"
#include "System/Content/CsPackage.h"

#include "System/Scene/ScnEntity.h"
#include "System/Scene/Animation/ScnAnimationTreeBlendNode.h"
#include "System/Scene/Animation/ScnAnimationTreeTrackNode.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaAnimationControllerComponent );

void GaAnimationControllerComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "pRootTrack_", &GaAnimationControllerComponent::pRootTrack_, bcRFF_TRANSIENT ),
		new ReField( "pIdleTrack_", &GaAnimationControllerComponent::pIdleTrack_, bcRFF_TRANSIENT ),
		new ReField( "pReloadTrack_", &GaAnimationControllerComponent::pReloadTrack_, bcRFF_TRANSIENT ),
		new ReField( "AnimIdle_", &GaAnimationControllerComponent::AnimIdle_, bcRFF_SHALLOW_COPY ),
		new ReField( "AnimFire_", &GaAnimationControllerComponent::AnimFire_, bcRFF_SHALLOW_COPY ),
		new ReField( "AnimReload_", &GaAnimationControllerComponent::AnimReload_, bcRFF_SHALLOW_COPY ),
	};
		
	ReRegisterClass< GaAnimationControllerComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaAnimationControllerComponent::initialise( const Json::Value& Object )
{
	Super::initialise();

	AnimIdle_ = ScnAnimationRef( getPackage()->getCrossRefResource( Object[ "anim_idle" ].asUInt() ) );
	AnimFire_ = ScnAnimationRef( getPackage()->getCrossRefResource( Object[ "anim_fire" ].asUInt() ) );
	AnimReload_ = ScnAnimationRef( getPackage()->getCrossRefResource( Object[ "anim_reload" ].asUInt() ) );
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaAnimationControllerComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	static BcF32 Ticker = 0.0f;
	Ticker += Tick * 0.1f;

	pRootTrack_->setBlendValue( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaAnimationControllerComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	
	ScnAnimationComponentRef Animation = getParentEntity()->getComponentByType< ScnAnimationComponent >();
	pRootTrack_ = Animation->findNodeByType< ScnAnimationTreeBlendNode >( "Root" );
	pIdleTrack_ = Animation->findNodeByType< ScnAnimationTreeTrackNode >( "IdleTrack_0" );
	pReloadTrack_ = Animation->findNodeByType< ScnAnimationTreeTrackNode >( "ReloadTrack_0" );
	
	pIdleTrack_->queueAnimation( AnimFire_ );
	pReloadTrack_->queueAnimation( AnimReload_ );

	BcF32 Speed = 0.9f + ( BcRandom::Global.randReal() + 1.0f * 0.1f );
	pIdleTrack_->setSpeed( Speed );
	pReloadTrack_->setSpeed( Speed );
}
