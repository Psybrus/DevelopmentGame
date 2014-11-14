#include "GaTestModelComponent.h"
#include "GaLevelComponent.h"
#include "GaTestShaderComponent.h"
#include "GaAnimationControllerComponent.h"
#include "GaCameraComponent.h"
#include "GaTestSelectionComponent.h"
#include "GaTestParticleComponent.h"
void AutoGenRegisterReflection()
{
	GaTestModelComponent::StaticRegisterClass();
	GaLevelComponent::StaticRegisterClass();
	GaTestShaderComponent::StaticRegisterClass();
	GaAnimationControllerComponent::StaticRegisterClass();
	GaCameraComponent::StaticRegisterClass();
	GaTestSelectionComponent::StaticRegisterClass();
	GaTestParticleComponent::StaticRegisterClass();
}
