/**************************************************************************
*
* File:		GaNetworkTestComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#pragma once

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaNetworkTestComponent
class GaNetworkTestComponent:
	public ScnComponent,
	public EvtPublisher
{
public:
	REFLECTION_DECLARE_DERIVED( GaNetworkTestComponent, ScnComponent );

	GaNetworkTestComponent();
	virtual ~GaNetworkTestComponent();
	
	virtual void update( BcF32 Tick );

	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
private:
	BcBool IsServer_;

	class NsSession* Session_;
	class NsEventProxy* NetworkedProxy_;
};
