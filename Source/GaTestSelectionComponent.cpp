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

void GaTestSelectionComponent::StaticRegisterClass()
{
	static const ReField Fields[] = 
	{
		ReField( "Options_",			&GaTestSelectionComponent::Options_ ),
		ReField( "SelectedEntry_",		&GaTestSelectionComponent::SelectedEntry_ ),
		ReField( "PreviousSpawned_",	&GaTestSelectionComponent::PreviousSpawned_ ),
		ReField( "FontComponent_",		&GaTestSelectionComponent::FontComponent_ ),
		ReField( "Canvas_",				&GaTestSelectionComponent::Canvas_ ),
		ReField( "Projection_",			&GaTestSelectionComponent::Projection_ ),
	};
		
	ReRegisterClass< GaTestSelectionComponent >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

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

	MaMat4d TextScaleMatrix;
	TextScaleMatrix.scale( MaVec4d( 0.04f, 0.04f, 1.0f, 1.0f ) );

	FontComponent_->setAlphaTestStepping( MaVec2d( 0.4f, 0.45f ) );

	Canvas_->pushMatrix( TextScaleMatrix );

	MaVec2d Position( 0.0f, 0.0f );
	MaVec2d Size;
	for( BcU32 Idx = 0; Idx < Options_.size(); ++Idx )
	{
		const auto& Option( Options_[ Idx ] );
		const auto Colour = Idx == SelectedEntry_ ? RsColour::GREEN : RsColour::GRAY;
		Size = FontComponent_->drawCentered( Canvas_, Position, (*Option.EntityToSpawn_->getName()), Colour );
		Position += MaVec2d( 0.0f, Size.y() );
	}

	ScnDebugRenderComponent::pImpl()->drawGrid( 
		MaVec3d( 0.0f, 0.0f, 0.0f ),
		MaVec3d( 500.0f, 0.0f, 500.0f ),
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

	DsCore::pImpl()->registerFunction("Test Entity 1", std::bind(&GaTestSelectionComponent::LoadEntity, this, 0));
	DsCore::pImpl()->registerFunction("Test Entity 2", std::bind(&GaTestSelectionComponent::LoadEntity, this, 1));
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestSelectionComponent::onDetach( ScnEntityWeakRef Parent )
{
	DsCore::pImpl()->deregisterFunction("Test Entity 1");
	DsCore::pImpl()->deregisterFunction("Test Entity 2");
	OsCore::pImpl()->unsubscribeAll(this);
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
		LoadEntity(SelectedEntry_);
		BcAssertMsg( PreviousSpawned_.isValid(), "We expect everythig nto have been preloaded." );
		break;
	}
	return evtRET_PASS;
}

void GaTestSelectionComponent::LoadEntity(int Entity)
{
	SelectedEntry_ = Entity;
	if (PreviousSpawned_.isValid())
	{
		ScnCore::pImpl()->removeEntity(PreviousSpawned_);
	}

	auto TemplateEntity = Options_[SelectedEntry_].EntityToSpawn_;
	ScnEntitySpawnParams SpawnEntity =
	{
		TemplateEntity->getPackageName(), TemplateEntity->getName(), "SpawnedEntity",
		MaMat4d(),
		NULL
	};

	PreviousSpawned_ = ScnCore::pImpl()->spawnEntity(SpawnEntity);
	BcAssertMsg(PreviousSpawned_.isValid(), "We expect everything to have been preloaded.");
}
