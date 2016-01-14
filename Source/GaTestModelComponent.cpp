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
#include "System/Scene/Rendering/ScnModel.h"
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

	// Get model component.
	//auto ModelComponent = getParentEntity()->getComponentByType< ScnModelComponent >();

	PSY_LOG( "Spawned %u model entities.\n", Idx );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestModelComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}
