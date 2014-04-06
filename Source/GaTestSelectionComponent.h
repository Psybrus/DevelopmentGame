/**************************************************************************
*
* File:		GaTestSelectionComponent.h
* Author:	Neil Richardson 
* Ver/Date:
* Description:
*		Test selection component.
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestSelectionComponent_H__
#define __GaTestSelectionComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef CsResourceRef< class GaTestSelectionComponent > GaTestSelectionComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestSelectionComponent
class GaTestSelectionComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( ScnComponent, GaTestSelectionComponent );

	void								initialise( const Json::Value& Object );

	virtual void						update( BcF32 Tick );

	virtual void						onAttach( ScnEntityWeakRef Parent );
	
private:
	struct TMenuEntry
	{
		ScnEntityRef EntityToSpawn_;
	};

	std::vector< TMenuEntry >			Options_;
	BcU32								SelectedEntry_;
	ScnFontComponentRef					FontComponent_;
	ScnCanvasComponentRef				Canvas_;

	BcMat4d								Projection_;
};

#endif

