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
	
	void update( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

private:
	std::vector< ScnFontComponentRef > FontComponents_;
	ScnCanvasComponentRef Canvas_;
	ScnMaterialComponentRef DebugMaterial_;

	BcU32 CurrFontComponent_;
};

#endif

