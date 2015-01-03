#include "Math/MaAABB.h"
#include "Math/MaCPUVec2d.h"
#include "Math/MaCPUVec3d.h"
#include "Math/MaCPUVec4d.h"
#include "Math/MaMat3d.h"
#include "Math/MaMat4d.h"
#include "Math/MaPlane.h"
#include "Math/MaQuat.h"
#include "System/Content/CsPackage.h"
#include "System/Content/CsPackageImporter.h"
#include "System/Content/CsRedirector.h"
#include "System/Content/CsResource.h"
#include "System/Content/CsResourceImporter.h"
#include "System/Content/CsTypes.h"
#include "System/File/FsTypes.h"
#include "System/Renderer/RsCore.h"
#include "System/Renderer/RsCoreImpl.h"
#include "System/Renderer/RsRenderState.h"
#include "System/Renderer/RsSamplerState.h"
#include "System/Renderer/RsTypes.h"
#include "System/Scene/Animation/ScnAnimation.h"
#include "System/Scene/Animation/ScnAnimationComponent.h"
#include "System/Scene/Animation/ScnAnimationPose.h"
#include "System/Scene/Animation/ScnAnimationTreeBlendNode.h"
#include "System/Scene/Animation/ScnAnimationTreeNode.h"
#include "System/Scene/Animation/ScnAnimationTreeTrackNode.h"
#include "System/Scene/Import/ScnAnimationImport.h"
#include "System/Scene/Import/ScnComponentImport.h"
#include "System/Scene/Import/ScnEntityImport.h"
#include "System/Scene/Import/ScnFontImport.h"
#include "System/Scene/Import/ScnMaterialImport.h"
#include "System/Scene/Import/ScnModelImport.h"
#include "System/Scene/Import/ScnShaderImport.h"
#include "System/Scene/Import/ScnSoundImport.h"
#include "System/Scene/Import/ScnTextureImport.h"
#include "System/Scene/Physics/ScnPhysicsBoxCollisionShape.h"
#include "System/Scene/Physics/ScnPhysicsCollisionShape.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Scene/Rendering/ScnLightComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Rendering/ScnRenderPipeline.h"
#include "System/Scene/Rendering/ScnRenderTarget.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"
#include "System/Scene/Rendering/ScnShader.h"
#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/Scene/Rendering/ScnTexture.h"
#include "System/Scene/Rendering/ScnTextureAtlas.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/ScnComponent.h"
#include "System/Scene/ScnEntity.h"
#include "System/Scene/ScnSpatialComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"
#include "System/Scene/Sound/ScnSoundListenerComponent.h"
#include "System/Sound/SoLoud/SsCoreImplSoLoud.h"
#include "System/Sound/SsCore.h"
#include "System/SysKernel.h"
#include "System/SysSystem.h"
#include "GaAnimationControllerComponent.h"
#include "GaCameraComponent.h"
#include "GaLevelComponent.h"
#include "GaTestFontComponent.h"
#include "GaTestModelComponent.h"
#include "GaTestParticleComponent.h"
#include "GaTestSelectionComponent.h"
#include "GaTestShaderComponent.h"
void AutoGenRegisterReflection()
{
	MaAABB::StaticRegisterClass();
	MaCPUVec2d::StaticRegisterClass();
	MaCPUVec3d::StaticRegisterClass();
	MaCPUVec4d::StaticRegisterClass();
	MaMat3d::StaticRegisterClass();
	MaMat4d::StaticRegisterClass();
	MaPlane::StaticRegisterClass();
	MaQuat::StaticRegisterClass();
	CsPackage::StaticRegisterClass();
	CsPackageDependencies::StaticRegisterClass();
	CsRedirectorImport::StaticRegisterClass();
	CsRedirector::StaticRegisterClass();
	CsResource::StaticRegisterClass();
	CsResourceImporterAttribute::StaticRegisterClass();
	CsResourceImporter::StaticRegisterClass();
	CsDependency::StaticRegisterClass();
	FsTimestamp::StaticRegisterClass();
	FsStats::StaticRegisterClass();
	RsCore::StaticRegisterClass();
	RsCoreImpl::StaticRegisterClass();
	RsRenderTargetBlendState::StaticRegisterClass();
	RsBlendState::StaticRegisterClass();
	RsStencilFaceState::StaticRegisterClass();
	RsDepthStencilState::StaticRegisterClass();
	RsRasteriserState::StaticRegisterClass();
	RsRenderStateDesc::StaticRegisterClass();
	RsSamplerStateDesc::StaticRegisterClass();
	RsColour::StaticRegisterClass();
	ScnAnimation::StaticRegisterClass();
	ScnAnimationComponent::StaticRegisterClass();
	ScnAnimationPose::StaticRegisterClass();
	ScnAnimationTreeBlendNode::StaticRegisterClass();
	ScnAnimationTreeNode::StaticRegisterClass();
	ScnAnimationTreeTrackNode::StaticRegisterClass();
	ScnAnimationImport::StaticRegisterClass();
	ScnComponentImport::StaticRegisterClass();
	ScnEntityImport::StaticRegisterClass();
	ScnFontImport::StaticRegisterClass();
	ScnMaterialImport::StaticRegisterClass();
	ScnModelImport::StaticRegisterClass();
	ScnShaderImport::StaticRegisterClass();
	ScnSoundImport::StaticRegisterClass();
	ScnTextureImport::StaticRegisterClass();
	ScnPhysicsBoxCollisionShape::StaticRegisterClass();
	ScnPhysicsCollisionShape::StaticRegisterClass();
	ScnPhysicsRigidBodyComponent::StaticRegisterClass();
	ScnPhysicsWorldComponent::StaticRegisterClass();
	ScnCanvasComponent::StaticRegisterClass();
	ScnDebugRenderComponent::StaticRegisterClass();
	ScnFont::StaticRegisterClass();
	ScnFontUniformBlockData::StaticRegisterClass();
	ScnFontDrawParams::StaticRegisterClass();
	ScnFontComponent::StaticRegisterClass();
	ScnLightComponent::StaticRegisterClass();
	ScnMaterial::StaticRegisterClass();
	ScnMaterialComponent::StaticRegisterClass();
	ScnModel::StaticRegisterClass();
	ScnModelComponent::StaticRegisterClass();
	ScnParticleSystemComponent::StaticRegisterClass();
	ScnRenderPipeline::StaticRegisterClass();
	ScnRenderTarget::StaticRegisterClass();
	ScnRenderableComponent::StaticRegisterClass();
	ScnShader::StaticRegisterClass();
	ScnShaderViewUniformBlockData::StaticRegisterClass();
	ScnShaderLightUniformBlockData::StaticRegisterClass();
	ScnShaderObjectUniformBlockData::StaticRegisterClass();
	ScnShaderBoneUniformBlockData::StaticRegisterClass();
	ScnShaderAlphaTestUniformBlockData::StaticRegisterClass();
	ScnSpriteComponent::StaticRegisterClass();
	ScnTexture::StaticRegisterClass();
	ScnTextureAtlas::StaticRegisterClass();
	ScnViewComponent::StaticRegisterClass();
	ScnComponentAttribute::StaticRegisterClass();
	ScnComponent::StaticRegisterClass();
	ScnEntity::StaticRegisterClass();
	ScnSpatialComponent::StaticRegisterClass();
	ScnSound::StaticRegisterClass();
	ScnSoundEmitterComponent::StaticRegisterClass();
	ScnSoundListenerComponent::StaticRegisterClass();
	SsCoreImplSoLoud::StaticRegisterClass();
	SsCore::StaticRegisterClass();
	SysKernel::StaticRegisterClass();
	SysSystem::StaticRegisterClass();
	GaAnimationControllerComponent::StaticRegisterClass();
	GaCameraComponent::StaticRegisterClass();
	GaLevelComponent::StaticRegisterClass();
	GaTestFontComponent::StaticRegisterClass();
	GaTestModelComponent::StaticRegisterClass();
	GaTestParticleComponent::StaticRegisterClass();
	GaTestSelectionComponent::StaticRegisterClass();
	GaTestShaderComponent::StaticRegisterClass();
}
