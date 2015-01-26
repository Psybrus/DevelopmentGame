/**************************************************************************
*
* File:		GaMindwaveVisualiserComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		Test selection component.
*		
*
*
* 
**************************************************************************/

#include "GaMindwaveVisualiserComponent.h"
 
#include "System/Content/CsPackage.h"
#include "System/Os/OsCore.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/SysKernel.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaMindwaveVisualiserComponent );

void GaMindwaveVisualiserComponent::StaticRegisterClass()
{
	/*
	ReField* Fields[] = 
	{
		new ReField( "Options_",			&GaMindwaveVisualiserComponent::Options_ ),
		new ReField( "SelectedEntry_",		&GaMindwaveVisualiserComponent::SelectedEntry_ ),
		new ReField( "PreviousSpawned_",	&GaMindwaveVisualiserComponent::PreviousSpawned_ ),
		new ReField( "FontComponent_",		&GaMindwaveVisualiserComponent::FontComponent_ ),
		new ReField( "Canvas_",				&GaMindwaveVisualiserComponent::Canvas_ ),
		new ReField( "Projection_",			&GaMindwaveVisualiserComponent::Projection_ ),
	};
	*/

	ReRegisterClass< GaMindwaveVisualiserComponent, Super >( )
		.addAttribute( new ScnComponentAttribute( 1000 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaMindwaveVisualiserComponent::initialise( const Json::Value& Object )
{
	Super::initialise();
}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaMindwaveVisualiserComponent::update( BcF32 Tick )
{
	Super::update( Tick );

}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaMindwaveVisualiserComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	//Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >();
	//FontComponent_ = Parent->getComponentAnyParentByType< ScnFontComponent >();

	OsEventInputMindwaveData::Delegate OnMindwaveData = 
		OsEventInputMindwaveData::Delegate::bind< GaMindwaveVisualiserComponent, &GaMindwaveVisualiserComponent::onMindwaveData >( this );

	OsEventInputMindwaveEEGPower::Delegate OnMindwaveEEGPower = 
		OsEventInputMindwaveEEGPower::Delegate::bind< GaMindwaveVisualiserComponent, &GaMindwaveVisualiserComponent::onMindwaveEEGPower >( this );

	OsEventInputMindwaveEEGRaw::Delegate OnMindwaveEEGRaw = 
		OsEventInputMindwaveEEGRaw::Delegate::bind< GaMindwaveVisualiserComponent, &GaMindwaveVisualiserComponent::onMindwaveEEGRaw >( this );

	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_DATA, OnMindwaveData );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_EEG_POWER, OnMindwaveEEGPower );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_EEG_RAW, OnMindwaveEEGRaw );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaMindwaveVisualiserComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );
	Super::onDetach( Parent );
}
	
//////////////////////////////////////////////////////////////////////////
// onMindwaveData
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveData( EvtID ID, const OsEventInputMindwaveData& Event )
{
	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMindwaveEEGPowers
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveEEGPower( EvtID ID, const OsEventInputMindwaveEEGPower& Event )
{
	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMindwaveEEGRaw
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveEEGRaw( EvtID ID, const OsEventInputMindwaveEEGRaw& Event )
{
	return evtRET_PASS;
}
