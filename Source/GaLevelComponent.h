/**************************************************************************
*
* File:		GaLevelComponent.h
* Author:	Neil Richardson 
* Ver/Date:	11/01/13	
* Description:
*		Level component.
*		
*
*
* 
**************************************************************************/

#ifndef __GaLevelComponent_H__
#define __GaLevelComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaLevelComponent > GaLevelComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaLevelComponent
class GaLevelComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaLevelComponent, ScnComponent );

	GaLevelComponent();
	virtual ~GaLevelComponent();
	
	void initialise( const Json::Value& Object );

	virtual void update( BcF32 Tick );

	virtual void onAttach( ScnEntityWeakRef Parent );
	
private:

};

#endif

