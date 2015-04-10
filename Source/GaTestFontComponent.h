/**************************************************************************
*
* File:		GaTestFontComponent.h
* Author:	Neil Richardson 
* Ver/Date:		
* Description:
*		
*		
*
*
* 
**************************************************************************/

#ifndef __GaTestFontComponent_H__
#define __GaTestFontComponent_H__

#include "Psybrus.h"
#include "System/Scene/ScnComponent.h"

#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"
#include "System/Scene/Rendering/ScnFont.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaTestFontComponent > GaTestFontComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTestFontComponent
class GaTestFontComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTestFontComponent, ScnComponent );

	GaTestFontComponent();
	virtual ~GaTestFontComponent();
	
	virtual void update( BcF32 Tick );

	virtual void onAttach( ScnEntityWeakRef Parent );
	virtual void onDetach( ScnEntityWeakRef Parent );

private:
	std::vector< ScnFontComponentRef > FontComponents_;
	ScnCanvasComponentRef Canvas_;
	ScnMaterialComponentRef DebugMaterial_;

	BcU32 CurrFontComponent_;
};

#endif

