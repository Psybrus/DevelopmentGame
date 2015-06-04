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

#include "System/Debug/DsImGui.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/SysKernel.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_BASIC( GaMenuEntry );
REFLECTION_DEFINE_DERIVED( GaTestSelectionComponent );

void GaMenuEntry::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Name_", &GaMenuEntry::Name_ ),
		new ReField( "Entity_", &GaMenuEntry::Entity_, bcRFF_SHALLOW_COPY ),
	};
		
	ReRegisterClass< GaMenuEntry >( Fields );
}

void GaTestSelectionComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Options_", &GaTestSelectionComponent::Options_, bcRFF_IMPORTER ),
		new ReField( "RunAutomatedTest_", &GaTestSelectionComponent::RunAutomatedTest_, bcRFF_IMPORTER ),
		new ReField( "TestMaxTime_", &GaTestSelectionComponent::TestMaxTime_, bcRFF_IMPORTER ),

		new ReField( "SelectedEntry_", &GaTestSelectionComponent::SelectedEntry_ ),
		new ReField( "PreviousSpawned_", &GaTestSelectionComponent::PreviousSpawned_ ),
		new ReField( "TestTime_", &GaTestSelectionComponent::TestTime_ ),
	};
		
	ReRegisterClass< GaTestSelectionComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaTestSelectionComponent >( 
					"Update", ScnComponentPriority::DEFAULT_UPDATE - 1 )
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestSelectionComponent::GaTestSelectionComponent():
	SelectedEntry_( BcErrorCode ),
	RunAutomatedTest_( BcFalse ),
	TestMaxTime_( 1.0f ),
	TestTime_( 1.0f )
{
	SelectedEntry_ = 0;
}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestSelectionComponent::~GaTestSelectionComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTestSelectionComponent::update( BcF32 Tick )
{
	if( RunAutomatedTest_ )
	{
		TestTime_ -= Tick;
		if( TestTime_ < 0.0f )
		{
			TestTime_ += TestMaxTime_;

			SelectedEntry_ = ( SelectedEntry_ + 1 ) % Options_.size();
			LoadEntity( SelectedEntry_ );
		}
	}

	static MaVec2d WindowPos = MaVec2d( 10.0f, 10.0f );
	static bool ShowOpened = true;
	ImGui::SetNextWindowPos( WindowPos );
	if ( ImGui::Begin( "Test Menu", &ShowOpened, ImVec2( 0.0f, 0.0f ), 0.3f, 0 ) )
	{
		ImGui::BeginGroup();

		// Selection menu.
		BcU32 Idx = 0;
		for( const auto& Option : Options_ )
		{
			if( ImGui::SmallButton( Option.Name_.c_str() ) )
			{
				LoadEntity( Idx );
			}
			++Idx;
		}

		ImGui::EndGroup();
	}
	WindowPos = ImGui::GetWindowPos();
	ImGui::End();

	ScnDebugRenderComponent::pImpl()->drawGrid( 
		MaVec3d( 0.0f, 0.0f, 0.0f ),
		MaVec3d( 500.0f, 0.0f, 500.0f ),
		1.0f,
		10.0f,
		0 );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestSelectionComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	using namespace std::placeholders;

#if !PLATFORM_HTML5
	if( DsCore::pImpl() )
	{
		for (BcU32 Idx = 0; Idx < Options_.size(); ++Idx)
		{
			const auto& Option(Options_[Idx]);
			BcU32 handle = DsCore::pImpl()->registerFunction(Option.Name_, std::bind(&GaTestSelectionComponent::LoadEntity, this, Idx));
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

	Super::onDetach( Parent );
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

	PSY_LOG( "Starting %s", Options_[SelectedEntry_].Name_.c_str() );

	auto TemplateEntity = Options_[SelectedEntry_].Entity_;
	ScnEntitySpawnParams SpawnEntity(
		"SpawnedEntity", 
		TemplateEntity,
		MaMat4d(),
		getParentEntity() );

	PreviousSpawned_ = ScnCore::pImpl()->spawnEntity(SpawnEntity);
	BcAssertMsg(PreviousSpawned_.isValid(), "We expect everything to have been preloaded.");
}
