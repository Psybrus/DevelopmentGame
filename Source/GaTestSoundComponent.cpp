/**************************************************************************
*
* File:		GaTestSoundComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestSoundComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Sound/SsCore.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsImGui.h"
#include "System/Debug/DsCore.h"

#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestSoundComponent );

void GaTestSoundComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Sound_", &GaTestSoundComponent::Sound_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "SoundEmitter_", &GaTestSoundComponent::SoundEmitter_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
	};

	ReRegisterClass< GaTestSoundComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaTestSoundComponent >()
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestSoundComponent::GaTestSoundComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestSoundComponent::~GaTestSoundComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTestSoundComponent::update( BcF32 Tick )
{
	static BcRandom Rand;

	static float Ticker = 0.0f;
	Ticker += Tick;
	if( Ticker > 0.2f )
	{
		Ticker -= 0.2f;
		SoundEmitter_->setPitch( ( Rand.randReal() + 2.0f ) * 0.5f );
		SoundEmitter_->play( Sound_, true );
	}

	if( ImGui::Begin( "Test Menu" ) )
	{
		ImGui::Separator();
		SsCore::pImpl()->getVisualisationData( VisFFT_, VisWave_ );
		ImGui::BeginGroup();
		if( VisFFT_.size() > 0 )
		{
			ImGui::PlotHistogram( "FFT", VisFFT_.data(), VisFFT_.size(), 0, nullptr, 0.0f, 8.0f );
		}
		if( VisWave_.size() > 0 )
		{
			ImGui::PlotLines( "Wave", VisWave_.data(), VisWave_.size(), 0, nullptr, -1.0f, 1.0f );
		}
		ImGui::EndGroup();
		ImGui::Separator();

		DsCore::pImpl()->drawObjectEditor( nullptr, this, getClass(), 0 );
	}
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestSoundComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	SoundEmitter_ = Parent->getComponentAnyParentByType< ScnSoundEmitterComponent >();
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestSoundComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestSoundComponent::getAABB() const
{
	return MaAABB();
}
