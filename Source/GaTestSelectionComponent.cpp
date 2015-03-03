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

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/SysKernel.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestSelectionComponent );

void GaTestSelectionComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Options_",			&GaTestSelectionComponent::Options_ ),
		new ReField( "SelectedEntry_",		&GaTestSelectionComponent::SelectedEntry_ ),
		new ReField( "PreviousSpawned_",	&GaTestSelectionComponent::PreviousSpawned_ ),
		new ReField( "FontComponent_",		&GaTestSelectionComponent::FontComponent_ ),
		new ReField( "Canvas_",				&GaTestSelectionComponent::Canvas_ ),
		new ReField( "Projection_",			&GaTestSelectionComponent::Projection_ ),
	};
		
	ReRegisterClass< GaTestSelectionComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestSelectionComponent::GaTestSelectionComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestSelectionComponent::~GaTestSelectionComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTestSelectionComponent::initialise( const Json::Value& Object )
{
	SelectedEntry_ = 0;

	Projection_.orthoProjection( -8.0f, 56.0f, 30.0f, -4.0f, -1.0f, 1.0f );

	const auto& Options( Object[ "options" ] );
	for( const auto& Option : Options )
	{
		TMenuEntry Entry = 
		{
			getPackage()->getCrossRefResource( Option.asUInt() )
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

	static BcF32 GameTimeTotal = 0.0f;
	static BcF32 FrameTimeTotal = 0.0f;
	static BcF32 GameTimeAccum = 0.0f;
	static BcF32 FrameTimeAccum = 0.0f;
	static int CaptureAmount = 60;
	static int CaptureAccum = 0;
	GameTimeAccum += SysKernel::pImpl()->getGameThreadTime();
	FrameTimeAccum += SysKernel::pImpl()->getFrameTime();
	++CaptureAccum;
	if( CaptureAccum >= CaptureAmount )
	{
		GameTimeTotal = GameTimeAccum / BcF32( CaptureAccum );
		FrameTimeTotal = FrameTimeAccum / BcF32( CaptureAccum );
		GameTimeAccum = 0.0f;
		FrameTimeAccum = 0.0f;
		CaptureAccum = 0;
	}

	BcChar PerfChars[ 128 ];
	BcSPrintf( PerfChars, "Perf: Game: %.3f ms, Frame %.3f ms", ( GameTimeTotal * 1000.0f ), ( FrameTimeTotal * 1000.0f ) );

#if 1
	ScnFontDrawParams DrawParams = ScnFontDrawParams()
		.setSize( 32.0f )
		.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::VCENTRE )
		.setMargin( 8.0f );

	Size = FontComponent_->drawText( 
		Canvas_,
		DrawParams
			.setTextColour( RsColour::BLUE ),
		Position, 
		MaVec2d( 0.0f, 0.0f ),
		PerfChars );
	Position += MaVec2d( 0.0f, Size.y() );

	for( BcU32 Idx = 0; Idx < Options_.size(); ++Idx )
	{
		const auto& Option( Options_[ Idx ] );
		const auto Colour = Idx == SelectedEntry_ ? RsColour::GREEN : RsColour::GRAY;
		Size = FontComponent_->drawText( 
			Canvas_,
			DrawParams
				.setTextColour( Colour ),
			Position, 
			MaVec2d( 0.0f, 0.0f ),
			(*Option.EntityToSpawn_->getName()) );
		Position += MaVec2d( 0.0f, Size.y() );
	}
#endif
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

	using namespace std::placeholders;
	OsCore::pImpl()->subscribe( osEVT_INPUT_KEYDOWN, this,
		std::bind( &GaTestSelectionComponent::onKeyPress, this, _1, _2 ) );

#if !PLATFORM_HTML5
	if( DsCore::pImpl() )
	{
		for (BcU32 Idx = 0; Idx < Options_.size(); ++Idx)
		{
			const auto& Option(Options_[Idx]);
			BcU32 handle = DsCore::pImpl()->registerFunction(*Option.EntityToSpawn_->getName(), std::bind(&GaTestSelectionComponent::LoadEntity, this, Idx));
			OptionsHandles_.push_back(handle);
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestSelectionComponent::onDetach( ScnEntityWeakRef Parent )
{
#if !PLATFORM_HTML5
	if( DsCore::pImpl() )
	{
		for ( BcU32 Idx = 0; Idx < OptionsHandles_.size(); ++Idx )
		{
			DsCore::pImpl()->deregisterFunction( OptionsHandles_[ Idx ] );
		}
	}
#endif

	OsCore::pImpl()->unsubscribeAll(this);
	Super::onDetach( Parent );
}
	
//////////////////////////////////////////////////////////////////////////
// onKeyPress
eEvtReturn GaTestSelectionComponent::onKeyPress( EvtID ID, const EvtBaseEvent& Event )
{
	const auto& KeyEvent = Event.get< OsEventInputKeyboard >();

	switch( KeyEvent.KeyCode_ )
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
		LoadEntity(SelectedEntry_);
		BcAssertMsg( PreviousSpawned_.isValid(), "We expect everythig nto have been preloaded." );
		break;
	}
	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// LoadEntity
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
		nullptr
	};

	PreviousSpawned_ = ScnCore::pImpl()->spawnEntity(SpawnEntity);
	BcAssertMsg(PreviousSpawned_.isValid(), "We expect everything to have been preloaded.");
}
