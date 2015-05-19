/**************************************************************************
*
* File:		GaLevelComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	11/01/13	
* Description:
*		Level component.
*		
*
*
* 
**************************************************************************/

#include "GaLevelComponent.h"

#include "System/Content/CsPackage.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_BASIC( GaLevelEntity );

void GaLevelEntity::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Basis_", &GaLevelEntity::Basis_, bcRFF_SHALLOW_COPY ),
		new ReField( "Name_", &GaLevelEntity::Name_ ),
		new ReField( "Position_", &GaLevelEntity::Position_ ),
		new ReField( "Rotation_", &GaLevelEntity::Rotation_ ),
	};

	ReRegisterClass< GaLevelEntity >( Fields );
}

REFLECTION_DEFINE_DERIVED( GaLevelComponent );

void GaLevelComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Entities_", &GaLevelComponent::Entities_, bcRFF_IMPORTER ),
	};

	ReRegisterClass< GaLevelComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaLevelComponent::GaLevelComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaLevelComponent::~GaLevelComponent()
{

}


//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaLevelComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	for( auto& LevelEntity : Entities_ )
	{
		MaMat4d Transform;
		Transform.rotation( LevelEntity.Rotation_ );
		Transform.translation( LevelEntity.Position_ );

		ScnEntitySpawnParams EntityParams(
			LevelEntity.Name_,
			LevelEntity.Basis_,
			Transform,
			getParentEntity() );

		ScnCore::pImpl()->spawnEntity( EntityParams );
	}
}
