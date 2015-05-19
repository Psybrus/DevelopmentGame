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
// GaLevelEntity
struct GaLevelEntity
{
	REFLECTION_DECLARE_BASIC( GaLevelEntity );
	GaLevelEntity(){};

	ScnEntityRef Basis_;
	BcName Name_;
	MaVec3d Position_;
	MaVec3d Rotation_;
};

//////////////////////////////////////////////////////////////////////////
// GaLevelComponent
class GaLevelComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaLevelComponent, ScnComponent );

	GaLevelComponent();
	virtual ~GaLevelComponent();
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	
private:
	std::vector< GaLevelEntity > Entities_;

};

#endif

