/**************************************************************************
*
* File:		GaTestComputeComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestComputeComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcRandom.h"
#include "Base/BcHalf.h"
#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// GaVertex
struct GaVertex
{
	GaVertex()
	{}

	GaVertex( MaVec4d Position, MaVec4d Normal, MaVec4d Tangent, MaVec4d Colour, MaVec4d TexCoord ):
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
	MaVec4d TexCoord_;
};

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestComputeComponent );

void GaTestComputeComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "ComputeShader_", &GaTestComputeComponent::ComputeShader_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "Material_", &GaTestComputeComponent::Material_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "ObjectUniformBuffer_", &GaTestComputeComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaTestComputeComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaTestComputeComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaTestComputeComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaTestComputeComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent_", &GaTestComputeComponent::MaterialComponent_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaTestComputeComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestComputeComponent::GaTestComputeComponent():
	ObjectUniformBuffer_(),
	TestUniformBuffer_(),
	IndexBuffer_(),
	VertexBuffer_(),
	VertexDeclaration_(),
	ComputeOutputTextures_( {} )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestComputeComponent::~GaTestComputeComponent()
{
}

//////////////////////////////////////////////////////////////////////////
// render
//virtual 
void GaTestComputeComponent::render( ScnRenderContext & RenderContext )
{
	Super::render( RenderContext );

	RsCore::pImpl()->updateBuffer( 
		ObjectUniformBuffer_.get(),
		0, sizeof( ScnShaderObjectUniformBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
			UniformBlock->WorldTransform_ = MaMat4d();
		} );


	// Set skinning parameters.
	MaterialComponent_->setObjectUniformBlock( ObjectUniformBuffer_.get() );
	
	// Set texture.
	MaterialComponent_->setTexture( "aDiffuseTex", ComputeOutputTextures_[ ComputeTextureIdx_ ] );
			
	// Set material components for view.
	RenderContext.View_->setMaterialParameters( MaterialComponent_ );
			
	// Render primitive.				
	const auto& TexDesc = ComputeOutputTextures_[ ComputeTextureIdx_ ]->getTexture()->getDesc();
	
	RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
		[
			TexDesc,
			ComputeProgramBinding = ComputeProgramBindings_[ ComputeTextureIdx_ ].get(),
			ComputeOutputBuffer = ComputeOutputBuffer_.get(),
			ComputeOutputTextures = ComputeOutputTextures_,
			ComputeTextureIdx = ComputeTextureIdx_,
			GeometryBinding = GeometryBinding_.get(),
			DrawProgramBinding = MaterialComponent_->getProgramBinding(),
			RenderState = MaterialComponent_->getRenderState(),
			FrameBuffer = RenderContext.View_->getFrameBuffer(),
			Viewport = RenderContext.View_->getViewport()
		]
		( RsContext* Context )
		{
			PSY_PROFILER_SECTION( RenderRoot, "GaTestComputeComponentRenderNode::render" );
			auto& Features = Context->getFeatures();
			if( Features.ComputeShaders_ )
			{
				Context->dispatchCompute( ComputeProgramBinding, TexDesc.Width_, TexDesc.Height_, 1 );
			}

			Context->drawPrimitives(
				GeometryBinding,
				DrawProgramBinding,
				RenderState,
				FrameBuffer,
				&Viewport,
				nullptr,
				RsTopologyType::TRIANGLE_STRIP, 0, 4, 0, 1 );
		} );
	ComputeTextureIdx_ = 1 - ComputeTextureIdx_;
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestComputeComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	
	auto& Features = RsCore::pImpl()->getContext( nullptr )->getFeatures();

	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ),
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

	BcU32 VertexBufferSize = 4 * sizeof( GaVertex );
	VertexBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsResourceBindFlags::VERTEX_BUFFER | RsResourceBindFlags::SHADER_RESOURCE,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ),
		getFullName().c_str() );

	ComputeOutputBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsResourceBindFlags::VERTEX_BUFFER | RsResourceBindFlags::UNORDERED_ACCESS,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ),
		getFullName().c_str() );

	BcBool RandomTex = BcTrue;
	RsResourceFormat TextureFormats[] = 
	{
		RsResourceFormat::R32_FLOAT,
		RsResourceFormat::R16G16_FLOAT,
		RsResourceFormat::R8G8B8A8_UNORM,
	};
	auto TextureFormat = TextureFormats[0];
	for( auto Format : TextureFormats )
	{
		if( Features.TextureFormat_[ (int)Format ] )
		{
			PSY_LOG( "INFO: Using texture format %u.", (int)Format );
			TextureFormat = Format;
			break;
		}
		else
		{
			PSY_LOG( "WARNING: Texture format %u not supported.", (int)Format );
		}
	}
	for( auto& ComputeOutputTexture : ComputeOutputTextures_ )
	{
		ComputeOutputTexture = ScnTexture::New( 
			RsTextureDesc( 
				RsTextureType::TEX2D, 
				RsResourceCreationFlags::STATIC, 
				RsResourceBindFlags::SHADER_RESOURCE | RsResourceBindFlags::UNORDERED_ACCESS,
				TextureFormat,
				1,
				512, 512, 1 ),
			getFullName().c_str() );

		RsCore::pImpl()->updateTexture( 
			ComputeOutputTexture->getTexture(),
			ComputeOutputTexture->getTexture()->getSlice(),
			RsResourceUpdateFlags::ASYNC,
			[ RandomTex ]( RsTexture* Texture, const RsTextureLock& Lock )
			{
				const auto& Desc = Texture->getDesc();
				for( BcU32 Y = 0; Y < Desc.Height_; ++Y )
				{
					BcF32* Data = reinterpret_cast< BcF32* >( 
						reinterpret_cast< BcU8* >( Lock.Buffer_ ) + Y * Lock.Pitch_ );
					for( BcU32 X = 0; X < Desc.Width_; ++X )
					{
						if( RandomTex && BcRandom::Global.randRealRange( 0.0f, 1.0f ) > 0.99f )
						{
							if( BcRandom::Global.randRealRange( 0.0f, 1.0f ) >= 0.5f )
							{
								*Data++ = ( 1.0f );
							}
							else
							{
								*Data++ = ( -1.0f );
							}
						}
						else
						{
							*Data++ = BcF32ToHalf( 0.0f );
						}
					}
				}
			} );
		RandomTex = BcFalse;
	}

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ),
		getFullName().c_str() );
	
	RsGeometryBindingDesc GeometryBindingDesc;
	GeometryBindingDesc.setVertexBuffer( 0, VertexBuffer_.get(), sizeof( GaVertex ) );
	GeometryBindingDesc.setVertexDeclaration( VertexDeclaration_.get() );
	GeometryBinding_ = RsCore::pImpl()->createGeometryBinding( GeometryBindingDesc, getFullName().c_str() );

	// Create program bindings.
	if( Features.ComputeShaders_ )
	{
		for( auto& ComputeProgramBindings : ComputeProgramBindings_ )
		{
			RsProgramBindingDesc ProgramBindingDesc;
			RsProgram* ComputeProgram = ComputeShader_->getProgram( ScnShaderPermutationFlags::NONE );
			BcU32 BufferInputSlot = ComputeProgram->findShaderResourceSlot( "iBuffer" );
			BcU32 BufferOutputSlot = ComputeProgram->findUnorderedAccessSlot( "oBuffer" );
			BcU32 TextureInputSlot = ComputeProgram->findShaderResourceSlot( "iTexture" );
			BcU32 TextureOutputSlot = ComputeProgram->findUnorderedAccessSlot( "oTexture" );

			if( BufferInputSlot != BcErrorCode )
			{
				ProgramBindingDesc.setShaderResourceView( BufferInputSlot, VertexBuffer_.get() );
			}

			if( TextureInputSlot != BcErrorCode )
			{
				ProgramBindingDesc.setShaderResourceView( TextureInputSlot, ComputeOutputTextures_[ ComputeTextureIdx_ ]->getTexture() );
			}

			if( BufferOutputSlot != BcErrorCode )
			{
				ProgramBindingDesc.setUnorderedAccessView( BufferOutputSlot, ComputeOutputBuffer_.get() );
			}

			if( TextureOutputSlot != BcErrorCode )
			{
				ProgramBindingDesc.setUnorderedAccessView( TextureOutputSlot, ComputeOutputTextures_[ 1 - ComputeTextureIdx_ ]->getTexture() );
			}

			ComputeProgramBindings = RsCore::pImpl()->createProgramBinding( ComputeProgram, ProgramBindingDesc, (*ComputeShader_->getName()).c_str() );

			// Flip input textures.
			ComputeTextureIdx_ = 1 - ComputeTextureIdx_;
		}
	}

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_.get(),
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[ VertexBufferSize ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			memset( Vertices, 0, VertexBufferSize );
			*Vertices++ = GaVertex( MaVec4d( -10.0f, -10.0f,  0.0f,  1.0f ), MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec4d( 0.0f, 0.0f, 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  10.0f, -10.0f,  0.0f,  1.0f ), MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d( -10.0f,  10.0f,  0.0f,  1.0f ), MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec4d( 0.0f, 1.0f, 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  10.0f,  10.0f,  0.0f,  1.0f ), MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec4d( 1.0f, 1.0f, 0.0f, 0.0f ) );
		} );

	RsCore::pImpl()->updateBuffer( 
		ComputeOutputBuffer_.get(),
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[ VertexBufferSize ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			memset( Vertices, 0, VertexBufferSize );
		} );

	ScnShaderPermutationFlags ShaderPermutation = 
		ScnShaderPermutationFlags::MESH_STATIC_3D |
		ScnShaderPermutationFlags::RENDER_FORWARD;

	// Attach a new material component.
	MaterialComponent_ = Parent->attach< ScnMaterialComponent >(
		BcName::INVALID, Material_, ShaderPermutation );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestComputeComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	GeometryBinding_.reset();
	for( auto& ComputeProgramBindings : ComputeProgramBindings_ )
	{
		ComputeProgramBindings.reset();
	}

	if(	ComputeOutputTextures_[0] )
	{
		ComputeOutputTextures_[0]->markDestroy();
		ComputeOutputTextures_[0] = nullptr;
	}

	if(	ComputeOutputTextures_[1] )
	{
		ComputeOutputTextures_[1]->markDestroy();
		ComputeOutputTextures_[1] = nullptr;
	}

	Parent->detach( MaterialComponent_ );
	MaterialComponent_ = nullptr;

	VertexDeclaration_.reset();
	ComputeOutputBuffer_.reset();
	VertexBuffer_.reset();
	IndexBuffer_.reset();
	ObjectUniformBuffer_.reset();
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestComputeComponent::getAABB() const
{
	return MaAABB();
}
