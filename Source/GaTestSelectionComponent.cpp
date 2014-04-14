/**************************************************************************
*
* File:		GaTestSelectionComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		Test selection component.
*		
*
*
* 
**************************************************************************/

#include "GaTestSelectionComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Os/OsCore.h"

#include "System/Scene/ScnDebugRenderComponent.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaTestSelectionComponent );

BCREFLECTION_EMPTY_REGISTER( GaTestSelectionComponent );
/*
BCREFLECTION_DERIVED_BEGIN( ScnComponent, GaAnimationControllerComponent )
	BCREFLECTION_MEMBER( BcName,							Name_,							bcRFF_DEFAULT | bcRFF_TRANSIENT ),
	BCREFLECTION_MEMBER( BcU32,								Index_,							bcRFF_DEFAULT | bcRFF_TRANSIENT ),
	BCREFLECTION_MEMBER( CsPackage,							pPackage_,						bcRFF_POINTER | bcRFF_TRANSIENT ),
	BCREFLECTION_MEMBER( BcU32,								RefCount_,						bcRFF_DEFAULT | bcRFF_TRANSIENT ),
BCREFLECTION_DERIVED_END();
*/

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTestSelectionComponent::initialise( const Json::Value& Object )
{
	Super::initialise();

	SelectedEntry_ = 0;

	Projection_.orthoProjection( -8.0f, 56.0f, 30.0f, -4.0f, -1.0f, 1.0f );

	const auto& Options( Object[ "options" ] );
	for( const auto& Option : Options )
	{
		TMenuEntry Entry = 
		{
			getPackage()->getPackageCrossRef( Option.asUInt() )
		};

		Options_.push_back( Entry );
	}
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTestSelectionComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	Canvas_->clear();
	Canvas_->pushMatrix( Projection_ );

	BcMat4d TextScaleMatrix;
	TextScaleMatrix.scale( BcVec4d( 0.04f, 0.04f, 1.0f, 1.0f ) );

	FontComponent_->setAlphaTestStepping( BcVec2d( 0.4f, 0.45f ) );

	Canvas_->pushMatrix( TextScaleMatrix );

	BcVec2d Position( 0.0f, 0.0f );
	BcVec2d Size;
	for( BcU32 Idx = 0; Idx < Options_.size(); ++Idx )
	{
		const auto& Option( Options_[ Idx ] );
		const auto Colour = Idx == SelectedEntry_ ? RsColour::GREEN : RsColour::GRAY;
		Size = FontComponent_->drawCentered( Canvas_, Position, (*Option.EntityToSpawn_->getName()), Colour );
		Position += BcVec2d( 0.0f, Size.y() );
	}

	ScnDebugRenderComponent::pImpl()->drawGrid( 
		BcVec3d( 0.0f, 0.0f, 0.0f ),
		BcVec3d( 500.0f, 0.0f, 500.0f ),
		1.0f,
		10.0f,
		0 );


	Canvas_->popMatrix();
	Canvas_->popMatrix();
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestSelectionComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >();
	FontComponent_ = Parent->getComponentAnyParentByType< ScnFontComponent >();

	OsEventInputKeyboard::Delegate OnKeyPress = OsEventInputKeyboard::Delegate::bind< GaTestSelectionComponent, &GaTestSelectionComponent::onKeyPress >( this );
	OsCore::pImpl()->subscribe( osEVT_INPUT_KEYDOWN, OnKeyPress );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestSelectionComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );

}
	
//////////////////////////////////////////////////////////////////////////
// onKeyPress
eEvtReturn GaTestSelectionComponent::onKeyPress( EvtID ID, const OsEventInputKeyboard& Event )
{
	switch( Event.KeyCode_ )
	{
	case OsEventInputKeyboard::KEYCODE_UP:
		if( SelectedEntry_ > 0 )
		{
			--SelectedEntry_;
		}
		else
		{
			SelectedEntry_ = Options_.size() - 1;
		}
		break;

	case OsEventInputKeyboard::KEYCODE_DOWN:
		if( SelectedEntry_ < Options_.size() - 1 )
		{
			++SelectedEntry_;
		}
		else
		{
			SelectedEntry_ = 0;
		}
		break;

	case OsEventInputKeyboard::KEYCODE_RETURN:
		if( PreviousSpawned_.isValid() )
		{
			ScnCore::pImpl()->removeEntity( PreviousSpawned_ );
		}

		auto TemplateEntity = Options_[ SelectedEntry_ ].EntityToSpawn_;
		ScnEntitySpawnParams SpawnEntity = 
		{
			TemplateEntity->getPackageName(), TemplateEntity->getName(), "SpawnedEntity",
			BcMat4d(),
			NULL
		};

		PreviousSpawned_ = ScnCore::pImpl()->spawnEntity( SpawnEntity );
		BcAssertMsg( PreviousSpawned_.isValid(), "We expect everything to have been preloaded." );
		break;
	}
	return evtRET_PASS;
}
