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
	REFLECTION_DECLARE_DERIVED( GaMindwaveVisualiserComponent, ScnComponent );

	GaMindwaveVisualiserComponent();
	virtual ~GaMindwaveVisualiserComponent();
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );
	
	eEvtReturn onMindwaveData( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onMindwaveEEGPower( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onMindwaveEEGRaw( EvtID ID, const EvtBaseEvent& Event );

private:

};

#endif

