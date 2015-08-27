#include "GaTrailComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTrailComponent );

void GaTrailComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_;", &GaTrailComponent::Material_, bcRFF_IMPORTER ),
	};

	ReRegisterClass< GaTrailComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTrailComponent::GaTrailComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTrailComponent::~GaTrailComponent()
{

}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTrailComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
}
