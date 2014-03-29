/**************************************************************************
*
* File:		GaCameraComponent.h
* Author:	Neil Richardson 
* Ver/Date:	15/12/12	
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#ifndef __GACAMERACOMPONENT_H__
#define __GACAMERACOMPONENT_H__

#include "Psybrus.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef CsResourceRef< class GaCameraComponent > GaCameraComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaCameraComponent
class GaCameraComponent:
	public ScnComponent
{
public:
	DECLARE_RESOURCE( ScnComponent, GaCameraComponent );

	void								initialise( const Json::Value& Object );

	virtual void						preUpdate( BcF32 Tick );

	virtual void						onAttach( ScnEntityWeakRef Parent );
	virtual void						onDetach( ScnEntityWeakRef Parent );

	eEvtReturn							onMouseDown( EvtID ID, const OsEventInputMouse& Event );
	eEvtReturn							onMouseUp( EvtID ID, const OsEventInputMouse& Event );
	eEvtReturn							onMouseMove( EvtID ID, const OsEventInputMouse& Event );
	eEvtReturn							onMouseWheel( EvtID ID, const OsEventInputMouse& Event );

	BcMat4d								getCameraRotationMatrix() const;
	
private:
	BcVec3d								CameraTarget_;
	BcVec3d								CameraRotation_;
	BcF32								CameraDistance_;
	BcF32								CameraZoom_;

	enum CameraState
	{
		STATE_IDLE = 0,
		STATE_ROTATE,
		STATE_PAN
	};

	CameraState							CameraState_;
	CameraState							NextCameraState_;
	OsEventInputMouse					LastMouseEvent_;
};

#endif

