/**************************************************************************
*
* File:		GaTestFontComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestFontComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Os/OsCore.h"
#include "System/Os/OsClient.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaTestFontComponent );

void GaTestFontComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "FontComponents_", &GaTestFontComponent::FontComponents_, bcRFF_TRANSIENT ),
		new ReField( "Canvas_", &GaTestFontComponent::Canvas_, bcRFF_TRANSIENT ),
		new ReField( "DebugMaterial_", &GaTestFontComponent::DebugMaterial_, bcRFF_TRANSIENT ),
		new ReField( "CurrFontComponent_", &GaTestFontComponent::CurrFontComponent_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaTestFontComponent, Super >( Fields )
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTestFontComponent::initialise()
{
	Super::initialise();

	CurrFontComponent_ = 0;
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTestFontComponent::initialise( const Json::Value& Object )
{
	Super::initialise();

	CurrFontComponent_ = 0;
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTestFontComponent::update( BcF32 Tick )
{
	Super::update( Tick );

	OsClient* Client = OsCore::pImpl()->getClient( 0 );
	BcF32 HalfWidth = static_cast< BcF32 >( Client->getWidth() / 2 );
	BcF32 HalfHeight = static_cast< BcF32 >( Client->getHeight() / 2 );

	MaMat4d Projection;
	Projection.orthoProjection( -HalfWidth, HalfWidth, HalfHeight, -HalfHeight, -1.0f, 1.0f );

	Canvas_->clear();
	Canvas_->pushMatrix( Projection );

	ScnFontDrawParams DrawParams = ScnFontDrawParams()
		.setSize( 32.0f )
		.setAlignmentBorder( 8.0f )
		.setTextColour( RsColour::WHITE );

	auto& FontComponent = FontComponents_[ CurrFontComponent_ ];

	const MaVec2d TextDimensions( MaVec2d( HalfWidth, HalfHeight ) );
	const MaVec2d HalfTextDimensions = TextDimensions * 0.5f;
	MaVec2d TextPosition( 0.0f, 0.0f );

	// Test alignment.
	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::TOP ),
		TextPosition,
		TextDimensions,
		L"LT\nLT" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::TOP ),
		TextPosition,
		TextDimensions,
		L"CT\nCT" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::RIGHT | ScnFontAlignment::TOP ),
		TextPosition,
		TextDimensions,
		L"RT\nRT" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::VCENTRE ),
		TextPosition,
		TextDimensions,
		L"LC\nLC" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::VCENTRE ),
		TextPosition,
		TextDimensions,
		L"CC\nCC" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::RIGHT | ScnFontAlignment::VCENTRE ),
		TextPosition,
		TextDimensions,
		L"RC\nRC" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::BOTTOM ),
		TextPosition,
		TextDimensions,
		L"LB\nLB" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::HCENTRE | ScnFontAlignment::BOTTOM ),
		TextPosition,
		TextDimensions,
		L"CB\nCB" );

	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setAlignment( ScnFontAlignment::RIGHT | ScnFontAlignment::BOTTOM ),
		TextPosition,
		TextDimensions,
		L"RB\nRB" );


	Canvas_->setMaterialComponent( DebugMaterial_ );
	Canvas_->drawLineBox( 
		TextPosition,
		TextPosition + TextDimensions,
		RsColour::GREEN,
		100 );

	Canvas_->drawLineBox( 
		TextPosition,
		TextPosition + HalfTextDimensions,
		RsColour::GREEN,
		100 );

	Canvas_->drawLineBox( 
		TextPosition + HalfTextDimensions,
		TextPosition + TextDimensions,
		RsColour::GREEN,
		100 );

	// Test wrapping.
	Canvas_->setMaterialComponent( DebugMaterial_ );
	TextPosition.y( -TextDimensions.y() );

	Canvas_->drawBox( 
		TextPosition,
		TextPosition + TextDimensions,
		RsColour::WHITE,
		0 );

	Canvas_->drawLineBox( 
		TextPosition,
		TextPosition + TextDimensions,
		RsColour::GREEN,
		100 );
	FontComponent->drawText( 
		Canvas_,
		DrawParams
			.setSize( 48.0f )
			.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::TOP )
			.setWrappingEnabled( BcTrue )
			.setTextColour( RsColour::BLACK ),
		TextPosition,
		TextDimensions,
		L"The quick brown fox jumps over the lazy dog.\n" );


	Canvas_->popMatrix();
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestFontComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Canvas_ = getComponentAnyParentByType< ScnCanvasComponent >();
	BcAssert( Canvas_.isValid() );

	DebugMaterial_ = getComponentAnyParentByType< ScnMaterialComponent >();
	BcAssert( DebugMaterial_.isValid() );

	// Font components.
	ScnFontComponentRef FontComponent;
	BcU32 FontIdx = 0;
	do
	{
		FontComponent = getComponentByType< ScnFontComponent >( FontIdx++ );

		if( FontComponent.isValid() )
		{
			FontComponents_.push_back( FontComponent );
		}
	}
	while( FontComponent.isValid() );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestFontComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}
