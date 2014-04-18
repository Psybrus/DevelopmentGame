/**************************************************************************
*
* File:		GaCameraComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	15/12/12	
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#include "GaCameraComponent.h"

#include "System/Os/OsCore.h"

#include "Base/BcMath.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaCameraComponent );

void GaCameraComponent::StaticRegisterClass()
{
	ReRegisterClass< GaCameraComponent >();
}


//////////////////////////////////////////////////////////////////////////
// initialise
void GaCameraComponent::initialise( const Json::Value& Object )
{
	Super::initialise();

	CameraState_ = STATE_IDLE;
	NextCameraState_ = STATE_IDLE;
	CameraDistance_ = 64.0f;
	CameraZoom_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// preUpdate
//virtual
void GaCameraComponent::preUpdate( BcF32 Tick )
{
	Super::update( Tick );

	// Update state.
	switch( CameraState_ )
	{
	case STATE_IDLE:
		{
			OsCore::pImpl()->getClient( 0 )->setMouseLock( BcFalse );
		}
		break;

	case STATE_ROTATE:
		{
			OsCore::pImpl()->getClient( 0 )->setMouseLock( BcTrue );
			BcF32 RotateSpeed = 0.25f;
			CameraRotation_ += BcVec3d( LastMouseEvent_.MouseDY_, LastMouseEvent_.MouseDX_, 0.0f ) * RotateSpeed * Tick;
		}
		break;

	case STATE_PAN:
		{
			OsCore::pImpl()->getClient( 0 )->setMouseLock( BcTrue );

			BcF32 PanSpeed = 4.0f;
			BcMat4d CameraRotationMatrix = getCameraRotationMatrix();
			BcVec3d OffsetVector = BcVec3d( LastMouseEvent_.MouseDX_, LastMouseEvent_.MouseDY_, 0.0f ) * CameraRotationMatrix;
			CameraTarget_ += OffsetVector * Tick * PanSpeed;
		}
		break;
	}

	CameraDistance_ += CameraZoom_ * Tick;
	CameraDistance_ = BcClamp( CameraDistance_, 1.0f, 4096.0f );
	CameraZoom_ = 0.0f;

	BcMat4d Matrix;
	BcVec3d ViewDistance = BcVec3d( 0.0f, 0.0f, CameraDistance_ );
	BcMat4d CameraRotationMatrix = getCameraRotationMatrix();
	ViewDistance = ViewDistance * CameraRotationMatrix;
	BcVec3d ViewFromPosition = CameraTarget_ + ViewDistance;

	Matrix.lookAt( ViewFromPosition, CameraTarget_, BcVec3d( CameraRotationMatrix.row1().x(), CameraRotationMatrix.row1().y(), CameraRotationMatrix.row1().z() ) );
	Matrix.inverse();
	//Matrix.rotation( BcVec3d( BcPIDIV2 - ( BcPI / 16.0f ), 0.0f, 0.0f ) );
	//Matrix.translation( BcVec3d( 0.0f, -4.0f, -2.0f ) );
	getParentEntity()->setLocalMatrix( Matrix );

	CameraState_ = NextCameraState_;

	// clear event.
	BcMemZero( &LastMouseEvent_, sizeof( LastMouseEvent_ ) );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaCameraComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


	OsEventInputMouse::Delegate OnMouseDown = OsEventInputMouse::Delegate::bind< GaCameraComponent, &GaCameraComponent::onMouseDown >( this );
	OsEventInputMouse::Delegate OnMouseUp = OsEventInputMouse::Delegate::bind< GaCameraComponent, &GaCameraComponent::onMouseUp >( this );
	OsEventInputMouse::Delegate OnMouseMove = OsEventInputMouse::Delegate::bind< GaCameraComponent, &GaCameraComponent::onMouseMove >( this );
	OsEventInputMouse::Delegate OnMouseWheel = OsEventInputMouse::Delegate::bind< GaCameraComponent, &GaCameraComponent::onMouseWheel >( this );

	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, OnMouseDown );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEUP, OnMouseUp );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEMOVE, OnMouseMove );
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEWHEEL, OnMouseWheel );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaCameraComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	OsCore::pImpl()->unsubscribeAll( this );
}

//////////////////////////////////////////////////////////////////////////
// onMouseDown
eEvtReturn GaCameraComponent::onMouseDown( EvtID ID, const OsEventInputMouse& Event )
{
	if( NextCameraState_ == STATE_IDLE )
	{
		if( Event.ButtonCode_ == 2 )
		{
			NextCameraState_ = STATE_ROTATE;
		}
		else if( Event.ButtonCode_ == 1 )
		{
			NextCameraState_ = STATE_PAN;
		}

		LastMouseEvent_ = Event;
	}

	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMouseUp
eEvtReturn GaCameraComponent::onMouseUp( EvtID ID, const OsEventInputMouse& Event )
{
	if( NextCameraState_ != STATE_IDLE )
	{
		NextCameraState_ = STATE_IDLE;
	}

	return evtRET_PASS;
}

//////////////////////////////////////////////////////////////////////////
// onMouseMove
eEvtReturn GaCameraComponent::onMouseMove( EvtID ID, const OsEventInputMouse& Event )
{
	LastMouseEvent_ = Event;

	return evtRET_PASS;
}
//////////////////////////////////////////////////////////////////////////
// onMouseWheel
eEvtReturn GaCameraComponent::onMouseWheel( EvtID ID, const OsEventInputMouse& Event )
{
	if( Event.ButtonCode_ == 3 )
	{
		CameraZoom_ -= ( CameraDistance_ * CameraDistance_ ) * 0.02f;
	}
	else if( Event.ButtonCode_ == 4 )
	{
		CameraZoom_ += ( CameraDistance_ * CameraDistance_ ) * 0.02f;
	}

	CameraZoom_ = BcClamp( CameraZoom_, -4096.0f, 4096.0f );

	LastMouseEvent_ = Event;
	
	return evtRET_PASS;
}
	
//////////////////////////////////////////////////////////////////////////
// getCameraRotationMatrix
BcMat4d GaCameraComponent::getCameraRotationMatrix() const
{
	BcMat4d CameraPitchMatrix;
	BcMat4d CameraYawMatrix;
	CameraPitchMatrix.rotation( BcVec3d( CameraRotation_.x(), 0.0f, 0.0f ) );
	CameraYawMatrix.rotation( BcVec3d( 0.0f, CameraRotation_.y(), 0.0f ) );
	return CameraPitchMatrix * CameraYawMatrix;
}