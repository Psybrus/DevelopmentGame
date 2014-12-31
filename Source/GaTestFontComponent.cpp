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
		.setAlignment( ScnFontAlignment::LEFT | ScnFontAlignment::TOP )
		.setSize( 32.0f )
		.setColour( RsColour::WHITE );

	auto& FontComponent = FontComponents_[ CurrFontComponent_ ];

	FontComponent->drawText( 
		Canvas_,
		DrawParams,
		MaVec2d( 0.0f, 0.0f ),
		MaVec2d( 0.0f, 0.0f ),
		L"Left Justified" );

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
