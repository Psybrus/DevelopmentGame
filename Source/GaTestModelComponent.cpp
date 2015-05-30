/**************************************************************************
*
* File:		GaTestModelComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestModelComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestModelComponent );

void GaTestModelComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_", &GaTestModelComponent::Material_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "MaterialComponent_", &GaTestModelComponent::MaterialComponent_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaTestModelComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestModelComponent::GaTestModelComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestModelComponent::~GaTestModelComponent()
{

}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestModelComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	BcU32 Idx = 0;
	BcF32 Width = 0.0f;

	for( BcF32 X = -Width; X <= Width; X += 2.0f )
	{
		ScnEntitySpawnParams EntityParams = 
		{
			BcName( "ModelEntity", Idx++ ), "model_test", "ModelEntity",
			MaMat4d(),
			getParentEntity()
		};

		EntityParams.Transform_.translation( MaVec3d( 0.0f, 0.0f, X ) );
		ScnCore::pImpl()->spawnEntity( EntityParams );
	}

	PSY_LOG( "Spawned %u model entities.\n", Idx );

}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestModelComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
