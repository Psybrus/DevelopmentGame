/**************************************************************************
*
* File:		GaMindwaveVisualiserComponent.h
* Author:	Neil Richardson 
* Ver/Date:
* Description:
*		Test selection component.
*		
*
*
* 
**************************************************************************/

#ifndef __GaMindwaveVisualiserComponent_H__
#define __GaMindwaveVisualiserComponent_H__

#include "Psybrus.h"

#include "System/Os/OsEvents.h"
#include "System/Scene/ScnEntity.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Debug/DsCore.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaMindwaveVisualiserComponent > GaMindwaveVisualiserComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaMindwaveVisualiserComponent
class GaMindwaveVisualiserComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( GaMindwaveVisualiserComponent, ScnComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );

	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );
	
	eEvtReturn							onMindwaveData( EvtID ID, const OsEventInputMindwaveData& Event );
	eEvtReturn							onMindwaveEEGPower( EvtID ID, const OsEventInputMindwaveEEGPower& Event );
	eEvtReturn							onMindwaveEEGRaw( EvtID ID, const OsEventInputMindwaveEEGRaw& Event );

private:

};

#endif

