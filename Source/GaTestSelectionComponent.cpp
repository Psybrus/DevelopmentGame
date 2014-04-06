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

	Projection_.orthoProjection( -32.0f, 32.0f, 18.0f, -18.0f, -1.0f, 1.0f );

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
	for( const auto& Option : Options_ )
	{
		Size = FontComponent_->drawCentered( Canvas_, Position, (*Option.EntityToSpawn_->getName()), RsColour( 1.0f, 1.0f, 1.0f, 1.0f ) );
		Position += BcVec2d( 0.0f, Size.y() );
	}

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
}
