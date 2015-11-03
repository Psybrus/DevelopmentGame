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
	ObjectUniformBuffer_( nullptr ),
	TestUniformBuffer_( nullptr ),
	IndexBuffer_( nullptr ),
	VertexBuffer_( nullptr ),
	VertexDeclaration_( nullptr )
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
		ObjectUniformBuffer_,
		0, sizeof( ScnShaderObjectUniformBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
			UniformBlock->WorldTransform_ = MaMat4d();
		} );


	// Set skinning parameters.
	MaterialComponent_->setObjectUniformBlock( ObjectUniformBuffer_ );
			
	// Set material components for view.
	RenderContext.pViewComponent_->setMaterialParameters( MaterialComponent_ );
			
	// Bind material.
	MaterialComponent_->bind( RenderContext.pFrame_, RenderContext.Sort_ );

	// Render primitive.				
	RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
		[
			ComputeProgram = ComputeShader_->getProgram( ScnShaderPermutationFlags::NONE ),
			ComputeOutputBuffer = ComputeOutputBuffer_,
			VertexBuffer = VertexBuffer_,
			VertexDeclaration = VertexDeclaration_
		]
		( RsContext* Context )
		{
			PSY_PROFILER_SECTION( RenderRoot, "GaTestComputeComponentRenderNode::render" );
			auto& Features = Context->getFeatures();
			if( Features.ComputeShaders_ )
			{
				RsDispatchBindings Bindings;
				Bindings.Buffers_[ ComputeProgram->findShaderResourceSlot( "iBuffer", RsShaderResourceType::BUFFER ) ] = VertexBuffer;
				Bindings.Buffers_[ ComputeProgram->findShaderResourceSlot( "oBuffer", RsShaderResourceType::BUFFER ) ] = ComputeOutputBuffer;
				Context->dispatchCompute( ComputeProgram, Bindings, 4, 1, 1 );
				Context->setVertexBuffer( 0, ComputeOutputBuffer, sizeof( GaVertex ) );
			}
			else
			{
				Context->setVertexBuffer( 0, VertexBuffer, sizeof( GaVertex ) );
			}

			Context->setVertexDeclaration( VertexDeclaration );
			Context->drawPrimitives( RsTopologyType::TRIANGLE_STRIP, 0, 4 );
		} );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestComputeComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ) );

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

	BcU32 VertexBufferSize = 4 * sizeof( GaVertex );
	VertexBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsBufferType::VERTEX,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ) );

	ComputeOutputBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsBufferType::VERTEX,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ) );

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ) );

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_,
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
		ComputeOutputBuffer_,
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[ VertexBufferSize ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			memset( Vertices, 0, VertexBufferSize );
		} );

	ScnShaderPermutationFlags ShaderPermutation = 
		ScnShaderPermutationFlags::MESH_STATIC_3D |
		ScnShaderPermutationFlags::RENDER_FORWARD |
		ScnShaderPermutationFlags::LIGHTING_NONE;

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

	Parent->detach( MaterialComponent_ );
	MaterialComponent_ = nullptr;

	RsCore::pImpl()->destroyResource( VertexDeclaration_ );
	RsCore::pImpl()->destroyResource( ComputeOutputBuffer_ );
	RsCore::pImpl()->destroyResource( VertexBuffer_ );
	RsCore::pImpl()->destroyResource( IndexBuffer_ );
	RsCore::pImpl()->destroyResource( ObjectUniformBuffer_ );
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestComputeComponent::getAABB() const
{
	return MaAABB();
}
