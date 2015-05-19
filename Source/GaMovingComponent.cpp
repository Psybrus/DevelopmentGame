/**************************************************************************
*
* File:		GaMovingComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaMovingComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaMovingComponent );

void GaMovingComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Position_", &GaMovingComponent::Position_, bcRFF_IMPORTER ),
		new ReField( "AxisSpeed_", &GaMovingComponent::AxisSpeed_, bcRFF_IMPORTER ),
		new ReField( "AxisMagnitude_", &GaMovingComponent::AxisMagnitude_, bcRFF_IMPORTER ),

		new ReField( "Timer_", &GaMovingComponent::Timer_, bcRFF_TRANSIENT ),
	};

	ReRegisterClass< GaMovingComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaMovingComponent >()
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaMovingComponent::GaMovingComponent():
	Position_( 0.0f, 0.0f, 0.0f ),
	AxisSpeed_( 1.0f, 1.0f, 1.0f ),
	AxisMagnitude_( 10.0f, 10.0f, 10.0f ),
	Timer_( 0.0f )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaMovingComponent::~GaMovingComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaMovingComponent::update( BcF32 Tick )
{
	MaVec3d Position = MaVec3d(
		-std::cos( Timer_ * AxisSpeed_.x() ),
		std::sin( Timer_ * AxisSpeed_.y() ),
		std::cos( Timer_ * AxisSpeed_.z()) ) * AxisMagnitude_;
	Timer_ += Tick;
	getParentEntity()->setLocalPosition( Position + Position_ );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaMovingComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaMovingComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaMovingComponent::getAABB() const
{
	return MaAABB();
}
