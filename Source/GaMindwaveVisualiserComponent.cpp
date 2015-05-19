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
REFLECTION_DEFINE_DERIVED( GaMindwaveVisualiserComponent );

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
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMindwaveVisualiserComponent::GaMindwaveVisualiserComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMindwaveVisualiserComponent::~GaMindwaveVisualiserComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaMindwaveVisualiserComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	//Canvas_ = Parent->getComponentAnyParentByType< ScnCanvasComponent >();
	//FontComponent_ = Parent->getComponentAnyParentByType< ScnFontComponent >();

	using namespace std::placeholders;

	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_DATA, this,
		std::bind( &GaMindwaveVisualiserComponent::onMindwaveData, this, _1, _2 ) );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_EEG_POWER, this,
		std::bind( &GaMindwaveVisualiserComponent::onMindwaveEEGPower, this, _1, _2 ) );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MINDWAVE_EEG_RAW, this,
		std::bind( &GaMindwaveVisualiserComponent::onMindwaveEEGRaw, this, _1, _2 ) );
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
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveData( EvtID ID, const EvtBaseEvent& Event )
{
	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMindwaveEEGPowers
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveEEGPower( EvtID ID, const EvtBaseEvent& Event )
{
	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMindwaveEEGRaw
eEvtReturn GaMindwaveVisualiserComponent::onMindwaveEEGRaw( EvtID ID, const EvtBaseEvent& Event )
{
	return evtRET_PASS;
}
