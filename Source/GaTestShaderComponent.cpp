/**************************************************************************
*
* File:		GaTestShaderComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestShaderComponent.h"

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
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestShaderComponent );

void GaTestShaderComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_", &GaTestShaderComponent::Material_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "ObjectUniformBuffer_", &GaTestShaderComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaTestShaderComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaTestShaderComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaTestShaderComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaTestShaderComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent_", &GaTestShaderComponent::MaterialComponent_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaTestShaderComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestShaderComponent::GaTestShaderComponent():
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
GaTestShaderComponent::~GaTestShaderComponent()
{
}

//////////////////////////////////////////////////////////////////////////
// render
//virtual 
void GaTestShaderComponent::render( ScnRenderContext & RenderContext )
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
			
	// Set material components for view.
	RenderContext.pViewComponent_->setMaterialParameters( MaterialComponent_ );
	
	// Render primitive.				
	RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
		[
			GeometryBinding = GeometryBinding_.get(),
			ProgramBinding = MaterialComponent_->getProgramBinding(),
			RenderState = MaterialComponent_->getRenderState(),
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
				RsTopologyType::TRIANGLE_STRIP, 0, 4 );
		} );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestShaderComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


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
			*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
		} );

	RsGeometryBindingDesc GeometryBindingDesc;
	GeometryBindingDesc.setVertexDeclaration( VertexDeclaration_.get() );
	GeometryBindingDesc.setVertexBuffer( 0, VertexBuffer_.get(), sizeof( GaVertex ) );
	GeometryBinding_ = RsCore::pImpl()->createGeometryBinding( GeometryBindingDesc, getFullName().c_str() );

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
void GaTestShaderComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	Parent->detach( MaterialComponent_ );
	MaterialComponent_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestShaderComponent::getAABB() const
{
	return MaAABB();
}
