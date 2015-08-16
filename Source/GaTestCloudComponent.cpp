/**************************************************************************
*
* File:		GaTestCloudComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestCloudComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/Debug/DsCore.h"

#include "System/SysKernel.h"

#include "Base/BcHalf.h"
#include "Base/BcProfiler.h"
#include "Base/BcRandom.h"

//////////////////////////////////////////////////////////////////////////
// GaVertex
struct GaVertex
{
	GaVertex()
	{}

	GaVertex( MaVec4d Position, MaVec4d Normal, MaVec4d Tangent, MaVec4d Colour, MaVec2d TexCoord ):
		Position_( Position ),
		Normal_( Normal ),
		Tangent_( Tangent ),
		Colour_( Colour ),
		TexCoord_( TexCoord )
	{}

	MaVec4d Position_;
	MaVec4d Normal_;
	MaVec4d Tangent_;
	MaVec4d Colour_;
	MaVec2d TexCoord_;
};

//////////////////////////////////////////////////////////////////////////
// GaTestTextureBlockData
REFLECTION_DEFINE_BASIC( GaTestCloudBlockData );

void GaTestCloudBlockData::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "CloudTimer_", &GaTestCloudBlockData::CloudTimer_ ),
		new ReField( "CloudScale_", &GaTestCloudBlockData::CloudScale_ ),
		new ReField( "CloudThreshold_", &GaTestCloudBlockData::CloudThreshold_ ),
		new ReField( "CloudPadding0_", &GaTestCloudBlockData::CloudPadding0_ ),
		new ReField( "CloudPadding1_", &GaTestCloudBlockData::CloudPadding1_ ),
		new ReField( "CloudPadding2_", &GaTestCloudBlockData::CloudPadding2_ ),
	};

	ReRegisterClass< GaTestCloudBlockData >( Fields );
}

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestCloudComponent );

void GaTestCloudComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material3D_", &GaTestCloudComponent::Material3D_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "ObjectUniformBuffer_", &GaTestCloudComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaTestCloudComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaTestCloudComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaTestCloudComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaTestCloudComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent3D_", &GaTestCloudComponent::MaterialComponent3D_, bcRFF_TRANSIENT ),
		new ReField( "Texture3D_", &GaTestCloudComponent::Texture3D_, bcRFF_TRANSIENT ),

		new ReField( "TestUniformBlock_", &GaTestCloudComponent::TestUniformBlock_, bcRFF_NONE ),
	};
		
	ReRegisterClass< GaTestCloudComponent, Super >( Fields )
	// Add editor.
		.addAttribute( 
		new DsImGuiFieldEditor( 
			[]( DsImGuiFieldEditor* ThisFieldEditor, std::string Name, void* Object, const ReClass* Class, ReFieldFlags Flags )
			{
				GaTestCloudComponent* Component = reinterpret_cast< GaTestCloudComponent* >( Object );
				DsCore::pImpl()->drawObjectEditor( ThisFieldEditor, &Component->TestUniformBlock_, GaTestCloudBlockData::StaticGetClass(), Flags );
			} ) );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestCloudComponent::GaTestCloudComponent():
	ObjectUniformBuffer_( nullptr ),
	TestUniformBuffer_( nullptr ),
	IndexBuffer_( nullptr ),
	VertexBuffer_( nullptr ),
	VertexDeclaration_( nullptr ),
	Texture3D_( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestCloudComponent::~GaTestCloudComponent()
{
}

//////////////////////////////////////////////////////////////////////////
// drawTest
void GaTestCloudComponent::drawTest(
		const MaMat4d& Transform, ScnMaterialComponent* Material,
		ScnRenderContext & RenderContext )
{
	if( Material )
	{
		// Set parameters.
		Material->setObjectUniformBlock( ObjectUniformBuffer_ );
		Material->setUniformBlock( "GaTestCloudBlockData", TestUniformBuffer_ );
		
		// Set material components for view.
		RenderContext.pViewComponent_->setMaterialParameters( Material );
				
		// Bind material.
		Material->bind( RenderContext.pFrame_, RenderContext.Sort_ );

		// Render primitive.				
		RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
			[ this, Transform ]( RsContext* Context )
			{
				PSY_PROFILER_SECTION( RenderRoot, "GaTestCloudComponentRenderNode::render" );

				Context->updateBuffer( 
					ObjectUniformBuffer_,
					0, sizeof( ScnShaderObjectUniformBlockData ),
					RsResourceUpdateFlags::ASYNC,
					[ Transform ]( RsBuffer* Buffer, const RsBufferLock& Lock )
					{
						auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
						UniformBlock->WorldTransform_ = Transform;
						UniformBlock->NormalTransform_ = Transform;
					} );

				Context->setVertexBuffer( 0, VertexBuffer_, sizeof( GaVertex ) );
				Context->setVertexDeclaration( VertexDeclaration_ );
				Context->drawPrimitives( RsTopologyType::TRIANGLE_STRIP, 0, 4 );
			} );
	}
}

//////////////////////////////////////////////////////////////////////////
// render
//virtual 
void GaTestCloudComponent::render( ScnRenderContext & RenderContext )
{
	Super::render( RenderContext );

	if( ImGui::Begin( "Test Menu" ) )
	{
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::SliderFloat( "Cloud Threshold", &TestUniformBlock_.CloudThreshold_, 0.0f, 4.0f );
		float Scale[3] = { TestUniformBlock_.CloudScale_.x(), TestUniformBlock_.CloudScale_.y(), TestUniformBlock_.CloudScale_.z() };
		if( ImGui::SliderFloat3( "Cloud Scale", Scale, 32.0f, 256.0f ) )
		{
			TestUniformBlock_.CloudScale_.x( Scale[ 0 ] );
			TestUniformBlock_.CloudScale_.y( Scale[ 1 ] );
			TestUniformBlock_.CloudScale_.z( Scale[ 2 ] );
		}

		ImGui::EndGroup();
		ImGui::Separator();

		DsCore::pImpl()->drawObjectEditor( nullptr, this, getClass(), 0 );
	}
	ImGui::End();


	RsCore::pImpl()->updateBuffer( 
		TestUniformBuffer_,
		0, sizeof( GaTestCloudBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[ this ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto UniformBlock = reinterpret_cast< GaTestCloudBlockData* >( Lock.Buffer_ );

			auto Accum = SysKernel::pImpl()->getFrameTime();

			TestUniformBlock_.CloudTimer_ += MaVec4d( Accum, Accum * 2.0f, Accum * 0.5f, Accum * 0.25f );

			*UniformBlock = TestUniformBlock_;
		} );

	MaMat4d Transform;
	drawTest( Transform, MaterialComponent3D_, RenderContext );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestCloudComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	TestUniformBlock_.CloudTimer_ = MaVec4d( 0.0f, 0.0f, 0.0f, 0.0f );
	TestUniformBlock_.CloudScale_ = MaVec4d( 32.0f, 32.0f, 32.0f, 0.0f );
	TestUniformBlock_.CloudThreshold_ = 4.0f;
	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ) );


	TestUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( GaTestCloudBlockData ) ) );

	BcU32 IndexBufferSize = sizeof( BcU16 ) * 4;
	IndexBuffer_ = RsCore::pImpl()->createBuffer(
		RsBufferDesc( RsBufferType::INDEX, RsResourceCreationFlags::STATIC, IndexBufferSize ) );

	RsCore::pImpl()->updateBuffer( 
		IndexBuffer_, 0, IndexBufferSize, RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& BufferLock )
		{
			BcU16* Indices = reinterpret_cast< BcU16* >( BufferLock.Buffer_ );
			Indices[ 0 ] = 0;
			Indices[ 1 ] = 1;
			Indices[ 2 ] = 2;
			Indices[ 3 ] = 3;
		} );

	BcU32 VertexBufferSize = 2048 * sizeof( GaVertex );
	VertexBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsBufferType::VERTEX,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ) );

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 2, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ) );

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_,
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( -1.0f, -1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d(  1.0f, -1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( -1.0f,  1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d(  1.0f,  1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
		} );

	ScnShaderPermutationFlags ShaderPermutation = 
		ScnShaderPermutationFlags::MESH_STATIC_3D |
		ScnShaderPermutationFlags::RENDER_FORWARD |
		ScnShaderPermutationFlags::LIGHTING_NONE;

	const auto& Features = RsCore::pImpl()->getContext( nullptr )->getFeatures();

	if( Material3D_ )
	{
		MaterialComponent3D_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, Material3D_, ShaderPermutation );

		// Create texture.
		if( Features.Texture3D_ )
		{
			Texture3D_ = ScnTexture::New3D( 64, 64, 64, 1, RsTextureFormat::R16F );
			auto Slice = Texture3D_->getTexture()->getSlice( 0 );
			RsCore::pImpl()->updateTexture( 
				Texture3D_->getTexture(),
				Slice,
				RsResourceUpdateFlags::ASYNC,
				[]( RsTexture* Texture, const RsTextureLock& Lock )
				{
					BcRandom Noise;
					const auto& Desc = Texture->getDesc();
					for( BcU32 Z = 0; Z < Desc.Depth_; ++Z )
					{
						BcU16* SliceData = reinterpret_cast< BcU16* >( 
							reinterpret_cast< BcU8* >( Lock.Buffer_ ) + Z * Lock.SlicePitch_ );
						for( BcU32 Y = 0; Y < Desc.Height_; ++Y )
						{
							BcU16* Data = reinterpret_cast< BcU16* >( 
								reinterpret_cast< BcU8* >( SliceData ) + Y * Lock.Pitch_ );
							for( BcU32 X = 0; X < Desc.Width_; ++X )
							{
								const BcU32 XDiv = X / 4;
								const BcU32 YDiv = Y / 4;
								const BcU32 ZDiv = Z / 4;
								
								BcF32 XVal = 0.0f;
								BcF32 YVal = 0.0f;
								BcF32 ZVal = 0.0f;
								BcF32 XOff = 0.0f;
								BcF32 YOff = 4.0f;
								BcF32 ZOff = 8.0f;
								BcF32 Freq = 0.25f;
								BcF32 Mul = 0.5f;
								for( BcU32 Idx = 0; Idx < 4; ++Idx )
								{
									XVal += ( Noise.interpolatedNoise( ( X + XOff ) * Freq, Desc.Width_ ) + 1.0f ) * Mul;
									YVal += ( Noise.interpolatedNoise( ( Y + YOff ) * Freq, Desc.Width_ ) + 1.0f ) * Mul;
									ZVal += ( Noise.interpolatedNoise( ( Z + ZOff ) * Freq, Desc.Width_ ) + 1.0f ) * Mul;
									Mul *= 0.6f;
									Freq *= 2.0f;
									XOff *= 8.0f;
									YOff *= 16.0f;
									ZOff *= 32.0f;
								}
#if 1

								*Data++ = BcF32ToHalf( XVal + YVal + ZVal );
#else
								*Data++ = ( ( ( XDiv + YDiv + ZDiv ) & 1 ) == 0 ) ? 0xffffffff : 0xff000000;
#endif
							}
						}
					}
				} );

			// Bind.
			MaterialComponent3D_->setTexture( "aCloudTex", Texture3D_ );
		}
	}

}
	
//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestCloudComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	if(	Texture3D_ )
	{
		Texture3D_->markDestroy();
		Texture3D_ = nullptr;
	}

	if( MaterialComponent3D_ )
	{
		Parent->detach( MaterialComponent3D_ );
		MaterialComponent3D_ = nullptr;
	}

	RsCore::pImpl()->destroyResource( VertexDeclaration_ );
	RsCore::pImpl()->destroyResource( VertexBuffer_ );
	RsCore::pImpl()->destroyResource( IndexBuffer_ );
	RsCore::pImpl()->destroyResource( ObjectUniformBuffer_ );
	RsCore::pImpl()->destroyResource( TestUniformBuffer_ );
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestCloudComponent::getAABB() const
{
	return MaAABB();
}
