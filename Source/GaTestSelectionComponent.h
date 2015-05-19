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

#include "System/Os/OsEvents.h"
#include "System/Scene/ScnEntity.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Debug/DsCore.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestSelectionComponent > GaTestSelectionComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaMenuEntry
struct GaMenuEntry
{
	REFLECTION_DECLARE_BASIC( GaMenuEntry );
	GaMenuEntry(){};

	std::string Name_;
	ScnEntityRef Entity_;
};

//////////////////////////////////////////////////////////////////////////
// GaTestSelectionComponent
class GaTestSelectionComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestSelectionComponent, ScnComponent );

	GaTestSelectionComponent();
	virtual ~GaTestSelectionComponent();

	void update( BcF32 Tick );

	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

private:
	std::vector< GaMenuEntry > Options_;
	BcU32 SelectedEntry_;
	ScnEntityRef PreviousSpawned_;

	BcBool RunAutomatedTest_;
	BcF32 TestMaxTime_;
	BcF32 TestTime_;

	void LoadEntity(int Entity);

	std::vector< BcU32 > OptionsHandles_;

};

#endif

