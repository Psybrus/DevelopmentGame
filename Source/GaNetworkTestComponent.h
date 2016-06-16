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
#include "System/Network/NsSession.h"

//////////////////////////////////////////////////////////////////////////
// GaNetworkTestComponent
class GaNetworkTestComponent:
	public ScnComponent,
	public EvtPublisher,
	public NsSessionHandler
{
public:
	REFLECTION_DECLARE_DERIVED( GaNetworkTestComponent, ScnComponent );

	GaNetworkTestComponent();
	virtual ~GaNetworkTestComponent();
	
	void update( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;
	
private:
	// NsSessionHandler
	void onAdvertisedSystem( const char* Name, const char* Address, BcU16 Port, BcU32 Ping ) override {}
	void onConnectionAccepted() override {}
	void onConnectionFailed() override {}
	void onSystemConnectionLost( NsGUID SystemGUID ) override {}
	void onSystemDisconnect( NsGUID SystemGUID ) override {}
	void onSystemConnected( NsGUID SystemGUID ) override {}

private:
	BcBool IsServer_;

	class NsSession* Session_;
	class NsEventProxy* NetworkedProxy_;
};
