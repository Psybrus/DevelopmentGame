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
		Material->setObjectUniformBlock( ObjectUniformBuffer_.get() );
		Material->setUniformBlock( "GaTestCloudBlockData", TestUniformBuffer_.get() );
		
		// Set material components for view.
		RenderContext.pViewComponent_->setMaterialParameters( Material );
				
		// Render primitive.				
		RsCore::pImpl()->updateBuffer(
			ObjectUniformBuffer_.get(),
			0, sizeof( ScnShaderObjectUniformBlockData ),
			RsResourceUpdateFlags::ASYNC,
			[ Transform ]( RsBuffer* Buffer, const RsBufferLock& Lock )
			{
				auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
				UniformBlock->WorldTransform_ = Transform;
				UniformBlock->NormalTransform_ = Transform;
			} );

		RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
			[
				GeometryBinding = GeometryBinding_.get(),
				ProgramBinding = Material->getProgramBinding(),
				RenderState = Material->getRenderState(),
				FrameBuffer = RenderContext.pViewComponent_->getFrameBuffer(),
				Viewport = RenderContext.pViewComponent_->getViewport()
			]
			( RsContext* Context )
			{
				PSY_PROFILE_FUNCTION;
				Context->drawPrimitives( 
					GeometryBinding,
					ProgramBinding,
					RenderState,
					FrameBuffer,
					&Viewport,
					nullptr,
					RsTopologyType::TRIANGLE_STRIP, 0, 4, 0, 1  );
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

		ImGui::SliderFloat( "Cloud Threshold", &TestUniformBlock_.CloudThreshold_, 0.1f, 4.0f );
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
		TestUniformBuffer_.get(),
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
	TestUniformBlock_.CloudScale_ = MaVec4d( 8.0f, 8.0f, 8.0f, 0.0f );
	TestUniformBlock_.CloudThreshold_ = 0.8f;
	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ),
		getFullName().c_str() );


	TestUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( GaTestCloudBlockData ) ),
		getFullName().c_str() );

	BcU32 IndexBufferSize = sizeof( BcU16 ) * 4;
	IndexBuffer_ = RsCore::pImpl()->createBuffer(
		RsBufferDesc( RsResourceBindFlags::INDEX_BUFFER, RsResourceCreationFlags::STATIC, IndexBufferSize ),
		getFullName().c_str() );

	RsCore::pImpl()->updateBuffer( 
		IndexBuffer_.get(), 0, IndexBufferSize, RsResourceUpdateFlags::ASYNC,
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
			RsResourceBindFlags::VERTEX_BUFFER,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ),
		getFullName().c_str() );

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 2, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ),
		getFullName().c_str() );

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_.get(),
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

	RsGeometryBindingDesc GeometryBindingDesc;
	GeometryBindingDesc.setVertexDeclaration( VertexDeclaration_.get() );
	GeometryBindingDesc.setVertexBuffer( 0, VertexBuffer_.get(), sizeof( GaVertex ) );
	GeometryBindingDesc.setIndexBuffer( IndexBuffer_.get() );
	GeometryBinding_ = RsCore::pImpl()->createGeometryBinding( GeometryBindingDesc, getFullName().c_str() );

	const auto& Features = RsCore::pImpl()->getContext( nullptr )->getFeatures();

	if( Material3D_ )
	{
		MaterialComponent3D_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, Material3D_, ShaderPermutation );

		// Create texture.
		if( Features.Texture3D_ )
		{
			Texture3D_ = ScnTexture::New3D( 64, 64, 64, 1, RsTextureFormat::R16F, getFullName().c_str() );
			auto Slice = Texture3D_->getTexture()->getSlice( 0 );
			RsCore::pImpl()->updateTexture( 
				Texture3D_->getTexture(),
				Slice,
				RsResourceUpdateFlags::ASYNC,
				[]( RsTexture* Texture, const RsTextureLock& Lock )
				{
					BcRandom Rand( 0x1750fec7 );
					BcRandom NoiseX( Rand.rand() );
					BcRandom NoiseY( Rand.rand() );
					BcRandom NoiseZ( Rand.rand() );
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
								BcF32 Val = 0.0f;
								BcF32 Freq = 1.0f / Desc.Width_;
								BcF32 Mul = 0.5f;
								MaVec3d SamplingPos( X, Y, Z );
								for( BcU32 Idx = 0; Idx < 8; ++Idx )
								{
									Val += Mul * 
										( 0.5f * sin( SamplingPos.x() * Freq * BcPI ) + 0.5f ) * 
										( 0.5f * cos( SamplingPos.y() * Freq * BcPI ) + 0.5f ) *
										( 0.5f * -sin( SamplingPos.z() * Freq * BcPI ) + 0.5f );
									Mul *= 0.9f;
									Freq *= 2.0f;
									MaMat4d Rot;
									Rot.rotation( MaVec3d( 0.4f, 0.2f, 1.0f ) );
									SamplingPos = SamplingPos * Rot;
								}
								*Data++ = BcF32ToHalf( Val );
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
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestCloudComponent::getAABB() const
{
	return MaAABB();
}
