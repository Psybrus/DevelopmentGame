/**************************************************************************
*
* File:		GaTestInstancingComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestInstancingComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsImGui.h"

#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestInstancingComponent );

void GaTestInstancingComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Models_", &GaTestInstancingComponent::Models_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "Scales_", &GaTestInstancingComponent::Scales_, bcRFF_IMPORTER ),
	};

	ReRegisterClass< GaTestInstancingComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaTestInstancingComponent >()
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestInstancingComponent::GaTestInstancingComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestInstancingComponent::~GaTestInstancingComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaTestInstancingComponent::update( BcF32 Tick )
{
	if( ImGui::Begin( "Test Menu" ) )
	{
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::EndGroup();
		ImGui::Separator();
	}
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestInstancingComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	BcU32 Idx = 0;
	BcF32 Width = 0.0f;

	MaMat4d RotationTransform;
	RotationTransform.rotation( MaVec3d( 0.0f, BcPI, 0.0f ) );
	MaMat4d EntityTransform;
	EntityTransform.scale( MaVec3d( 1.0f, 1.0f, 1.0f ) );
	EntityTransform = EntityTransform * RotationTransform;

	// Create model entities.
	BcF32 MaxSize = 32.0f;
	BcRandom RNG;
	BcAssert( Models_.size() > 0 );
	for( BcF32 Z = 0.0f; Z <= 1.0f; Z += 1.0f / MaxSize )
	{
		for( BcF32 X = 0.0f; X <= 1.0f; X += 1.0f / MaxSize )
		{
			BcF32 Y = 0.0f;
			BcF32 XPosition = ( X - 0.5f ) * MaxSize * 2.0f;
			BcF32 YPosition = 2.0f;
			BcF32 ZPosition = ( Z - 0.5f ) * MaxSize * 2.0f;
			auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );

			auto Idx = RNG.randRange( 0, Models_.size() - 1 );
			auto Model = Models_[ Idx ];
			auto Scale = Scales_[ Idx ];

			auto ModelComponent = Entity->attach< ScnModelComponent >( "ModelComponent", Model );
			EntityTransform.identity();
			EntityTransform.scale( Scale );
			EntityTransform.translation( MaVec3d( XPosition, YPosition, ZPosition ) );
			Entity->setLocalMatrix( EntityTransform * RotationTransform );

			ScnShaderMaterialUniformBlockData MaterialData;
			bool IsLit = RNG.randRange( 0, 32 ) < 30;
			MaterialData.MaterialBaseColour_ = MaVec4d( 1.0f, 0.0f, 1.0f, 1.0f );
			if( IsLit )
			{
				MaterialData.MaterialBaseColour_ = MaVec4d( 
					RNG.randRealRange( 0.0f, 1.0f ),
					RNG.randRealRange( 0.0f, 1.0f ),
					RNG.randRealRange( 0.0f, 1.0f ),
					1.0f );
			}
			MaterialData.MaterialMetallic_ = RNG.randRealRange( 0.0f, 1.0f );
			MaterialData.MaterialSpecular_ = RNG.randRealRange( 0.0f, 1.0f );
			MaterialData.MaterialRoughness_ = RNG.randRealRange( 0.0f, 1.0f );
			ModelComponent->setUniforms( MaterialData );
			ModelComponent->setLit( IsLit );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestInstancingComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
