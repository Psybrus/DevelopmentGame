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
	
	void update( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
private:
	BcBool IsServer_;

	class NsSession* Session_;
	class NsEventProxy* NetworkedProxy_;
};
