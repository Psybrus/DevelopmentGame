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

#include "GaNetworkTestComponent.h"

#include "Base/BcRandom.h"
#include "System/Content/CsPackage.h"

#include "System/Scene/ScnEntity.h"

#include "System/Os/OsEvents.h"

#include "System/Network/NsSession.h"
#include "System/Network/NsSessionImpl.h"
#include "System/Network/NsEventProxy.h"

////////////////////////////////////////////////////////////////////////////////
// GaNetworkTestEvent
struct GaNetworkTestEvent: EvtEvent< GaNetworkTestEvent >
{
	BcU32 ClientID_;
};

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaNetworkTestComponent );

void GaNetworkTestComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "IsServer_", &GaNetworkTestComponent::IsServer_, bcRFF_IMPORTER ),
		new ReField( "Session_", &GaNetworkTestComponent::Session_, bcRFF_SHALLOW_COPY | bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaNetworkTestComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaNetworkTestComponent >()
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaNetworkTestComponent::GaNetworkTestComponent():
	IsServer_( BcFalse ),
	Session_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaNetworkTestComponent::~GaNetworkTestComponent()
{

}

/////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaNetworkTestComponent::update( BcF32 Tick )
{
	if( IsServer_ == BcFalse )
	{
		static int timer = 0;
		timer++;
		if(timer > 100)
		{
			timer = 0;
			GaNetworkTestEvent Event;
			Event.ClientID_ = getName().getID();
			PSY_LOG( "Client %u broadcasting", getName().getID() );
			EvtPublisher::publish( 1, Event, BcTrue );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaNetworkTestComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	const BcU16 Port = 1234;

	if( IsServer_ )
	{
		Session_ = new NsSessionImpl( NsSessionImpl::SERVER, 32, Port );
		NetworkedProxy_ = new NsEventProxy( this, Session_, 0 );
	}
	else
	{
		Session_ = new NsSessionImpl( NsSessionImpl::CLIENT, "127.0.0.1", Port );
		NetworkedProxy_ = new NsEventProxy( this, Session_, 0 );
	}

	EvtPublisher::subscribe( 1, this,
		[ this ]( EvtID ID, const EvtBaseEvent& InEvent )->eEvtReturn
		{
			const GaNetworkTestEvent& Event = InEvent.get< GaNetworkTestEvent >();

			if( IsServer_ )
			{
				PSY_LOG( "Server got message from client %u", Event.ClientID_ );
			}
			else
			{
				PSY_LOG( "Client %u got message from client %u", getName().getID(), Event.ClientID_ );
			}
			return evtRET_PASS;
		} );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaNetworkTestComponent::onDetach( ScnEntityWeakRef Parent )
{
	delete NetworkedProxy_;
	delete Session_;

	Super::onDetach( Parent );
}
