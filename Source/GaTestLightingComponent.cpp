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
		.addAttribute( new ScnComponentProcessor() );
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
// onAttach
//virtual
void GaTestLightingComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	BcU32 Idx = 0;
	BcF32 Width = 0.0f;

	// Create model entities.
	BcF32 MaxSize = 8.0f;
	for( BcF32 Z = 0.0f; Z <= 1.0f; Z += 1.0f / MaxSize )
	{
		for( BcF32 X = 0.0f; X <= 1.0f; X += 1.0f / MaxSize )
		{
			BcF32 Y = 0.0f;
			BcF32 XPosition = ( X - 0.5f ) * MaxSize * 2.0f;
			BcF32 YPosition = ( ( Y - 0.5f ) * MaxSize * 2.0f ) + MaxSize;
			BcF32 ZPosition = ( Z - 0.5f ) * MaxSize * 2.0f;
			auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );
			auto ModelComponent = Entity->attach< ScnModelComponent >( "ModelComponent", Model_ );
			Entity->setLocalPosition( MaVec3d( XPosition, YPosition, ZPosition ) );

			ScnShaderMaterialUniformBlockData MaterialData;
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
			BcF32 YPosition = ( ( Y - 0.5f ) * MaxSize * 2.0f ) + MaxSize * 2.0f;
			BcF32 ZPosition = ( Z - 0.5f ) * MaxSize * 2.0f;
			auto Entity = Parent->attach< ScnEntity >( "ModelEntity" );
			auto ModelComponent = Entity->attach< ScnModelComponent >( "ModelComponent", Model_ );
			Entity->setLocalPosition( MaVec3d( XPosition, YPosition, ZPosition ) );

			ScnShaderMaterialUniformBlockData MaterialData;
			MaterialData.MaterialMetallic_ = Z;
			MaterialData.MaterialSpecular_ = 0.0f;
			MaterialData.MaterialRoughness_ = X;
			ModelComponent->setUniforms( MaterialData );
			ModelComponent->setLit( true );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestLightingComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
