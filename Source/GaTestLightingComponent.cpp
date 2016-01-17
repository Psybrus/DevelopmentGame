/**************************************************************************
*
* File:		GaTestLightingComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestLightingComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsImGui.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestLightingComponent );

void GaTestLightingComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Model_", &GaTestLightingComponent::Model_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
	};

	ReRegisterClass< GaTestLightingComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaTestLightingComponent >()
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestLightingComponent::GaTestLightingComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestLightingComponent::~GaTestLightingComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
void GaTestLightingComponent::update( BcF32 Tick )
{
	if( ImGui::Begin( "Test Menu" ) )
	{
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::ColorEditMode( ImGuiColorEditMode_RGB );
		float Colour[3] =
		{
			TestModelUniforms_.MaterialBaseColour_.x(),
			TestModelUniforms_.MaterialBaseColour_.y(),
			TestModelUniforms_.MaterialBaseColour_.z()
		};
		if( ImGui::ColorEdit3( "Base Colour", Colour ) )
		{
			TestModelUniforms_.MaterialBaseColour_ = MaVec4d( Colour[0], Colour[1], Colour[2], 1.0f );
		}

		ImGui::SliderFloat( "Metallic", &TestModelUniforms_.MaterialMetallic_, 0.0f, 1.0f );
		ImGui::SliderFloat( "Specular", &TestModelUniforms_.MaterialSpecular_, 0.0f, 1.0f );
		ImGui::SliderFloat( "Roughness", &TestModelUniforms_.MaterialRoughness_, 0.0f, 1.0f );

		ImGui::EndGroup();
		ImGui::Separator();
	}
	ImGui::End();

	TestModelComponent_->setUniforms( TestModelUniforms_ );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestLightingComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	BcU32 Idx = 0;
	BcF32 Width = 0.0f;

	MaMat4d RotationTransform;
	RotationTransform.rotation( MaVec3d( 0.0f, BcPI, 0.0f ) );
	MaMat4d EntityTransform;
	EntityTransform.scale( MaVec3d( 10.0f, 10.0f, 10.0f ) );
	EntityTransform = EntityTransform * RotationTransform;

	// Create model entities.
	BcF32 MaxSize = 8.0f;
	for( BcF32 Z = 0.0f; Z <= 1.0f; Z += 1.0f / MaxSize )
	{
		for( BcF32 X = 0.0f; X <= 1.0f; X += 1.0f / MaxSize )
		{
			BcF32 Y = 0.0f;
			BcF32 XPosition = ( X - 0.5f ) * MaxSize * 2.0f;
			BcF32 YPosition = 2.0f;
			BcF32 ZPosition = ( Z - 0.5f ) * MaxSize * 2.0f;
			auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );
			auto ModelComponent = Entity->attach< ScnModelComponent >( "ModelComponent", Model_ );
			EntityTransform.translation( MaVec3d( XPosition, YPosition, ZPosition ) );
			Entity->setLocalMatrix( EntityTransform );

			ScnShaderMaterialUniformBlockData MaterialData;
			MaterialData.MaterialBaseColour_ = MaVec4d( 1.000f, 0.766f, 0.336f, 1.000f );
			MaterialData.MaterialMetallic_ = 0.0f;
			MaterialData.MaterialSpecular_ = Z;
			MaterialData.MaterialRoughness_ = X;
			ModelComponent->setUniforms( MaterialData );
			ModelComponent->setLit( true );
		}
	}

	for( BcF32 Z = 0.0f; Z <= 1.0f; Z += 1.0f / MaxSize )
	{
		for( BcF32 X = 0.0f; X <= 1.0f; X += 1.0f / MaxSize )
		{
			BcF32 Y = 0.0f;
			BcF32 XPosition = ( X - 0.5f ) * MaxSize * 2.0f;
			BcF32 YPosition = 4.0f;
			BcF32 ZPosition = ( Z - 0.5f ) * MaxSize * 2.0f;
			auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );
			auto ModelComponent = Entity->attach< ScnModelComponent >( "ModelComponent", Model_ );
			EntityTransform.translation( MaVec3d( XPosition, YPosition, ZPosition ) );
			Entity->setLocalMatrix( EntityTransform );

			ScnShaderMaterialUniformBlockData MaterialData;
			MaterialData.MaterialBaseColour_ = MaVec4d( 1.000f, 0.766f, 0.336f, 1.000f );
			MaterialData.MaterialMetallic_ = Z;
			MaterialData.MaterialSpecular_ = 0.0f;
			MaterialData.MaterialRoughness_ = X;
			ModelComponent->setUniforms( MaterialData );
			ModelComponent->setLit( true );
		}
	}

	// Spawn the test model.
	BcF32 XPosition = 0;
	BcF32 YPosition = 4.0f;
	BcF32 ZPosition = 0;
	auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );
	TestModelComponent_ = Entity->attach< ScnModelComponent >( "ModelComponent", Model_ );
	EntityTransform.scale( MaVec3d( 100.0f, 100.0f, 100.0f ) );
	EntityTransform = EntityTransform * RotationTransform;
	EntityTransform.translation( MaVec3d( XPosition, YPosition, ZPosition ) );
	Entity->setLocalMatrix( EntityTransform );

	TestModelUniforms_.MaterialBaseColour_ = MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f );
	TestModelUniforms_.MaterialMetallic_ = 0.0f;
	TestModelUniforms_.MaterialSpecular_ = 0.0f;
	TestModelUniforms_.MaterialRoughness_ = 0.0f;
	TestModelComponent_->setUniforms( TestModelUniforms_ );
	TestModelComponent_->setLit( true );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestLightingComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
