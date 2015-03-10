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
		.addAttribute( new ScnComponentAttribute( 0 ) );
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
// update
//virtual
void GaLevelComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaLevelComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	for( const auto& LevelEntity : Entities_ )
	{
		MaMat4d Transform;
		Transform.rotation( LevelEntity.Rotation_ );
		Transform.translation( LevelEntity.Position_ );

		ScnEntitySpawnParams EntityParams = 
		{
			LevelEntity.Basis_->getPackage()->getName(),
			LevelEntity.Basis_->getName(),
			LevelEntity.Name_,
			Transform,
			getParentEntity(),
			nullptr
		};

		ScnCore::pImpl()->spawnEntity( EntityParams );
	}
}
