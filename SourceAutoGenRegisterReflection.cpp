#include "GaAnimationControllerComponent.h"
#include "GaCameraComponent.h"
#include "GaLevelComponent.h"
#include "GaTestModelComponent.h"
#include "GaTestParticleComponent.h"
#include "GaTestSelectionComponent.h"
#include "GaTestShaderComponent.h"
void AutoGenRegisterReflection()
{
	GaAnimationControllerComponent::StaticRegisterClass();
	GaCameraComponent::StaticRegisterClass();
	GaLevelComponent::StaticRegisterClass();
	GaTestModelComponent::StaticRegisterClass();
	GaTestParticleComponent::StaticRegisterClass();
	GaTestSelectionComponent::StaticRegisterClass();
	GaTestShaderComponent::StaticRegisterClass();
}
