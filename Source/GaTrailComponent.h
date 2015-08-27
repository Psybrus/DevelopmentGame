#ifndef __GaTrailComponent_H__
#define __GaTrailComponent_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaTrailComponentRef
typedef ReObjectRef< class GaTrailComponent > GaTrailComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaTrailComponent
class GaTrailComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaTrailComponent, ScnComponent );

	GaTrailComponent();
	virtual ~GaTrailComponent();
	
	virtual void onAttach( ScnEntityWeakRef Parent );
	
private:
	class ScnMaterial* Material_;
};

#endif

